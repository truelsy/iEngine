/*
 * Session.cpp
 *
 *  Created on: 2014. 6. 20.
 *      Author: mega
 */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/throw_exception.hpp>
#include <boost/asio.hpp>

#include "../Session.h"
#include "../Static.h"


namespace Network {

Session::Session()
	: _socket(Static::Instance().GETIOSERVICE()),
	  _strand(Static::Instance().GETIOSERVICE())
{
	_observer = std::shared_ptr<Observer>(NULL);
}

Session::~Session()
{
	Finalize();
	Close();
}

void
Session::Initialize()
{

}

void
Session::Finalize()
{

}

void
Session::Close()
{
	if (_socket.is_open())
	{
		DLOG(INFO) << __FUNCTION__ << ": Session Close!!";

		//try {
		//	m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		//} catch (std::exception & e) {
		//	std::cout << "Close Exception : " << e.what() << std::endl;
		//}

		boost::system::error_code ec;
		_socket.close(ec);

		if (ec)
		{
			LOG(ERROR) << __FUNCTION__ << ": _socket Close Failed. cause(" << ec.message() << ")";
		}
	}

	if (_packet)
	{
		_packet = NULL;
	}

	if (_observer)
	{
		_observer = NULL;
	}

	_sendPacketQueue.clear();
	_readPacketQueue.clear();
}

void
Session::DoStart(boost::asio::ip::tcp::socket socket)
{
	_socket = std::move(socket);

	int native = _socket.native();
	int timeout = 180;
	int interval = 10;
	int probes = 3;
	int on = 1;

	int ret_keepalive = setsockopt(native, SOL_SOCKET, SO_KEEPALIVE, (void *) &on, sizeof(int));

	// wait timeout default 72000 -> 180
	int ret_keepidle = setsockopt(native, SOL_TCP, TCP_KEEPIDLE, (void *) &timeout, sizeof(int));

	// retry interval -> 10 sec
	int ret_keepintvl = setsockopt(native, SOL_TCP, TCP_KEEPINTVL, (void *) &interval, sizeof(int));

	// retry count -> 3
	int ret_keepinit = setsockopt(native, SOL_TCP, TCP_KEEPCNT, (void *) &probes, sizeof(int));

	if (ret_keepalive || ret_keepidle || ret_keepintvl || ret_keepinit)
	{
		LOG(ERROR) << __FUNCTION__ << ": Failed to enable keep alive on TCP client socket!";
	}

	/// SO_REUSEADDR 옵션을 사용하면, TIME_WAIT 상태에 있는 PORT를 사용할 수 있다.
	//_socket.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

	/// socket 옵션을 이용해서 TIME_WAIT이 발생하지 않도록 하는 코드
	//_socket.set_option(boost::asio::ip::tcp::acceptor::linger(true, 0));

	//_socket.set_option(boost::asio::ip::tcp::acceptor::keep_alive(true));

	if (_observer)
	{
		_observer->OnOpen();
	}

	DoReadHeader();
}

void
Session::DoReadHeader()
{
	_packet = Static::Instance().GETPACKETPOOL_READ()->NEW();
	_packet->SetHeaderSize(4);

//	m_Socket.async_read_some(
//			boost::asio::buffer(&m_pBuf[m_nBufCursor], m_nRemainSize),
//			m_Strand.wrap(
//			boost::bind(&Session::OnRead, this,
//				boost::asio::placeholders::error,
//				boost::asio::placeholders::bytes_transferred)
//			)
//	);

	boost::asio::async_read(
		_socket,
		boost::asio::buffer(_packet->GetBuf().data(), _packet->GetHeaderSize()),
		_strand.wrap(boost::bind(&Session::OnReadHeader, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred))
	);
}


void
Session::OnReadHeader(const boost::system::error_code & error, size_t bytes_transferred)
{
	// Socket Close
	if (0 >= bytes_transferred || error)
	{
		if (_observer)
		{
			_observer->OnClose();
		}

		return;
	}

	int bodySize = static_cast<int>(* _packet->GetBuf().data()) - _packet->GetHeaderSize();
	_packet->SetBodySize(bodySize);

	DoReadBody();
}


void
Session::DoReadBody()
{
	boost::asio::async_read(
		_socket,
		boost::asio::buffer(_packet->GetBuf().data() + _packet->GetHeaderSize(), _packet->GetBodySize()),
		_strand.wrap(boost::bind(&Session::OnReadBody, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred))
	);
}


void
Session::OnReadBody(const boost::system::error_code & error, size_t bytes_transferred)
{
	// Socket Close
	if (0 >= bytes_transferred || error)
	{
		if (_observer)
		{
			_observer->OnClose();
		}

		return;
	}

	// 데이터 처리
	//_readPacketQueue.push_back(_packet);

	if (_observer)
	{
		_readPacketQueue.push_back(_packet);
		_observer->OnMessage();
	}

	DoReadHeader();
}


void
Session::Send(std::shared_ptr<Packet> sendPacket)
{
	bool isEmpty = false;
	{
		std::lock_guard<std::mutex> lock(_sendLock);
		isEmpty = _sendPacketQueue.empty();

		_sendPacketQueue.push_back(sendPacket->CAPTURE());
	}

	if (isEmpty)
		DoWrite();
}


void
Session::DoWrite()
{
//	auto self(shared_from_this());
//	auto function = [this, self](boost::system::error_code ec, std::size_t length)
//	{
//		if (!ec)
//		{
//			bool isEmpty = false;
//			{
//				std::lock_guard<std::mutex> lock(_sendLock);
//				_sendPacketQueue.pop_front();
//
//				if (true == _sendPacketQueue.empty())
//					isEmpty = true;
//			}
//
//			if (false == isEmpty)
//				DoWrite();
//		}
//		else
//		{
//			if (NULL != _observer) _observer->OnClose();
//		}
//	};

	std::shared_ptr<Packet> sendPacket;
	{
		std::lock_guard<std::mutex> lock(_sendLock);
		sendPacket = _sendPacketQueue.front();
	}

	boost::asio::async_write(_socket,
		boost::asio::buffer(sendPacket->GetBuf().data(), sendPacket->GetSize()),
		_strand.wrap(boost::bind(&Session::OnWrite, this, boost::asio::placeholders::error))
	);

	//boost::asio::async_write(_socket, boost::asio::buffer(sendPacket->GetBuf().data(), 8), function);
}

void
Session::OnWrite(const boost::system::error_code & ec)
{
	if (ec)
	{
		LOG(ERROR) << __FUNCTION__ << " : Failure Write! cause(" << ec.message() << ")";

		if (_observer)
		{
			_observer->OnClose();
		}

		return;
	}

	{
		std::lock_guard<std::mutex> lock(_sendLock);
		_sendPacketQueue.pop_front();

		if (_sendPacketQueue.empty())
			return;
	}

	// Queue에 데이터가 있으면 보내기 작업 계속.
	DoWrite();
}

} /*BoostAsioNetwork*/
