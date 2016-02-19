/*
 * Session.cpp
 *
 *  Created on: 2014. 6. 20.
 *      Author: mega
 */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/throw_exception.hpp>

#include "../Session.h"
#include "../Static.h"


namespace BoostAsioNetwork {

Session::Session()
	: m_Socket(Static::Instance().GETIOSERVICE()),
	  m_Strand(Static::Instance().GETIOSERVICE())
{
	m_nRemainSize = MAX_BUFFER_SIZE * 2;
	m_nBufCursor = 0;

	pthread_mutex_init(&m_QueueLock, NULL);
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
	pthread_mutex_lock(&m_QueueLock);

	for (Packet * pPacket : m_PacketQueue)
	{
		Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
	}

	pthread_mutex_unlock(&m_QueueLock);
}

void
Session::Close()
{
	if (true == m_Socket.is_open())
	{
		std::cout << "[Socket Close]" << std::endl;

		//try {
		//	m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		//} catch (std::exception & e) {
		//	std::cout << "Close Exception : " << e.what() << std::endl;
		//}

		m_Socket.close();
	}
}

int
Session::GetHeaderSize() const
{
	// identify(1byte) + size(2byte) + option flag(1byte) + command(2byte)
#ifdef CGCII_BMT
	return 4;
#else
	return 1 + 2 + 1 + 2;
#endif
}

int
Session::GetBodySize(char * pBuf) const
{
#ifdef CGCII_BMT
	int iBodySize = *((int *)(pBuf));
	return iBodySize;
#else
	short nBodySize = *((short *)(pBuf + 1));
	return static_cast<int>(nBodySize);
#endif
}

void
Session::HandleRead()
{
	m_Socket.async_read_some(
			boost::asio::buffer(&m_pBuf[m_nBufCursor], m_nRemainSize),
			m_Strand.wrap(
			boost::bind(&Session::OnRead, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
			)
	);
}

void
Session::OnRead(const boost::system::error_code & error, size_t bytes_transferred)
{
	//std::cout << "OnRead error code : " << error << std::endl;

	// Socket Close
	if (0 >= bytes_transferred || error)
	{
		Packet * pPacket = Static::Instance().GETPACKETPOOL()->READ_NEW();
		pPacket->SetPacketType(Packet::PacketType::CLOSE);
		PacketEnqueue(pPacket);

		IListener * pListener = Static::Instance().GetListener();
		if (NULL != pListener)
		{
			pListener->OnNetworkEvent(this);
		}

		return;
	}

	//std::cout << "Read : " << bytes_transferred << std::endl;

	int nTotalReadSize = bytes_transferred + m_nBufCursor;

	int nHeaderSize = this->GetHeaderSize();

	if (nTotalReadSize < nHeaderSize)
	{
		m_nBufCursor = bytes_transferred;
		m_nRemainSize -= bytes_transferred;
		HandleRead();
		return;
	}

	char * pBuf = m_pBuf.data();

	while (nTotalReadSize > 0)
	{
		int nBodySize = this->GetBodySize(pBuf);

#ifdef CGCII_BMT
		int nReadSize = nHeaderSize + (nBodySize - 4);
#else
		int nReadSize = nHeaderSize + nBodySize;
#endif

		if (nReadSize >= MAX_BUFFER_SIZE)
		{
			std::cout << "Packet size Overflow!!! nReadSize : " << nReadSize << std::endl;
			Close();
			return;
		}

		// 데이터 부족
		if (nReadSize > nTotalReadSize)
		{
			m_nBufCursor = nTotalReadSize;
			m_nRemainSize = MAX_BUFFER_SIZE - nTotalReadSize;
			break;
		}

		Packet * pPacket = Static::Instance().GETPACKETPOOL()->READ_NEW();
		memcpy(pPacket->GetBuf().data(), pBuf, nReadSize);

		PacketEnqueue(pPacket);

		// 이벤트 발생
		IListener * pListener = Static::Instance().GetListener();
		if (NULL != pListener)
		{
			pListener->OnNetworkEvent(this);
		}

		pBuf += nReadSize;
		nTotalReadSize -= nReadSize;

		// 다음 데이터 부족
		if (nTotalReadSize < nHeaderSize)
		{
			// 남은 데이터는 맨 앞으로 이동
			if (nTotalReadSize > 0)
			{
				memmove(m_pBuf.data(), pBuf, nTotalReadSize);
				m_nBufCursor = nTotalReadSize;
				m_nRemainSize = MAX_BUFFER_SIZE - nTotalReadSize;
			}
			else if (nTotalReadSize == 0)
			{
				m_nBufCursor = 0;
				m_nRemainSize = MAX_BUFFER_SIZE;
			}

			break;
		}
	}

	HandleRead();
}

void
Session::HandleWrite(Packet * pPacket)
{
	boost::asio::async_write(m_Socket,
			boost::asio::buffer(pPacket->GetBuf(), pPacket->GetOffSet()),
			boost::bind(&Session::OnWrite, this,
				boost::asio::placeholders::error,
				pPacket)
	);
}

void
Session::OnWrite(const boost::system::error_code & error, Packet * pPacket)
{
	//std::cout << "OnWrite error code : " << error << std::endl;

	Static::Instance().GETPACKETPOOL()->SEND_DEL(pPacket);
}

void
Session::PacketEnqueue(Packet * pPacket)
{
	pthread_mutex_lock(&m_QueueLock);

	m_PacketQueue.push_back(pPacket);

	pthread_mutex_unlock(&m_QueueLock);
}

Packet *
Session::PacketDequeue()
{
	Packet * pPacket = NULL;

	pthread_mutex_lock(&m_QueueLock);

	auto iter = m_PacketQueue.begin();
	if (iter != m_PacketQueue.end())
	{
		pPacket = *iter;
		m_PacketQueue.pop_front();
	}

	pthread_mutex_unlock(&m_QueueLock);

	return pPacket;
}

} /*BoostAsioNetwork*/
