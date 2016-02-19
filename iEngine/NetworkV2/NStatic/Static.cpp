/*
 * Static.cpp
 *
 *  Created on: 2014. 6. 17.
 *      Author: mega
 */

#include "../Static.h"

#include <boost/bind.hpp>

namespace Network {

Static::Static(int threadCount, int sendPacketCount, int readPacketCount, int sessionCount)
	: _acceptor(_ioService),
	  _work(_ioService),
	  _socket(_ioService),
	  _threadCount(threadCount),
	  _onAcceptEventFunc(NULL)
{
	_sendPacketPool = new NBase::Pool<Packet>(sendPacketCount);
	_readPacketPool = new NBase::Pool<Packet>(readPacketCount);
	_sessionPool	= new NBase::Pool<Session>(sessionCount);
}

Static::~Static()
{
	std::cout << "[Network] Static Finalize" << std::endl;

	_ioService.stop();

	_threadGroup.join_all();

	delete _sendPacketPool;
	delete _readPacketPool;
	delete _sessionPool;
}

void
Static::Listen(unsigned short port)
{
	if (0 >= _threadCount)
	{
		_threadCount = boost::thread::hardware_concurrency();
		LOG(INFO) << __FUNCTION__ << " : Worker ThreadCount(" << _threadCount << ")";
	}

	for (int i = 0; i < _threadCount; i++)
	{
		_threadGroup.create_thread(boost::bind(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &_ioService));
	}

	boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), port);
	_acceptor.open(endPoint.protocol());
	_acceptor.bind(endPoint);

	/// SO_REUSEADDR 옵션을 사용하면, TIME_WAIT 상태에 있는 PORT를 사용할 수 있다.
	_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

	/// socket 옵션을 이용해서 TIME_WAIT이 발생하지 않도록 하는 코드
	_acceptor.set_option(boost::asio::ip::tcp::acceptor::linger(true, 0));

	_acceptor.listen();

	DoAccept();
}

void
Static::DoAccept()
{
	// 재사용하기 전에 Close시킨다.
	if (_socket.is_open())
	{
		boost::system::error_code ec;
		_socket.close(ec);

		if (ec)
		{
			LOG(ERROR) << __FUNCTION__ << ": Failure _socket close. cause(" << ec.message() << ")";
		}
	}

	_acceptor.async_accept(_socket,
			boost::bind(&Static::OnAccept, this, boost::asio::placeholders::error)
	);
}

void
Static::OnAccept(const boost::system::error_code & ec)
{
	if (ec)
	{
		std::cout << "OnAccept Error. cause(" << ec.message() << ") code(" << ec.value() << ")" << std::endl;
		DoAccept();
		return;
	}

	DLOG(INFO) << __FUNCTION__ << ": Session Open!!";

	{
		boost::system::error_code ec;
		boost::asio::ip::tcp::endpoint remoteEndPoint = _socket.remote_endpoint(ec);

		if (ec)
		{
			LOG(ERROR) << __FUNCTION__ << ": remoteEndPoint Error. cause(" << ec.message() << ")";
			DoAccept();
			return;
		}

		boost::asio::ip::address address = remoteEndPoint.address();
		std::string strAddress = address.to_string();
		//std::array<unsigned char, 4> arrAddress = address.to_v4().to_bytes();

		DLOG(INFO) << __FUNCTION__ << ": [remote] endpoint : " << remoteEndPoint;
		DLOG(INFO) << __FUNCTION__ << ": [remote] address  : " << strAddress;
		DLOG(INFO) << __FUNCTION__ << ": [remote] port     : " << remoteEndPoint;
	}

	std::shared_ptr<Session> pSession = GETSESSIONPOOL()->NEW();
	if (nullptr == pSession.get())
	{
		LOG(ERROR) << __FUNCTION__ << ": pSession is NULL";
		DoAccept();
		return;
	}

	if (NULL == _onAcceptEventFunc)
	{
		LOG(ERROR) << __FUNCTION__ << ": _onAcceptEventFunc is NULL";
		DoAccept();
		return;
	}

	_onAcceptEventFunc(pSession->CAPTURE(), _acceptor.local_endpoint().port());
	pSession->DoStart(std::move(_socket));

	DoAccept();
}

} /*BoostAsioNetwork*/
