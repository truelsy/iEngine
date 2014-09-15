/*
 * Static.cpp
 *
 *  Created on: 2014. 6. 17.
 *      Author: mega
 */

#include "../Static.h"

#include <boost/bind.hpp>

namespace BoostAsioNetwork {

Static::Static(int n_ThreadCount, int n_SendPacketCount, int n_ReadPacketCount)
	: m_Acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 54486)),
	  m_Work(m_ioService),
	  m_nThreadCount(n_ThreadCount),
	  m_nSendPacketCount(n_SendPacketCount),
	  m_nReadPacketCount(n_ReadPacketCount)

{
	m_pPacketPool = new Packet::Pool;
	m_pPacketPool->Init(m_nSendPacketCount, m_nReadPacketCount);

	m_pSessionPool = new Session::Pool;

	m_pListener = NULL;
}

Static::~Static()
{
	std::cout << "+++ Static Finalize" << std::endl;

	m_ioService.stop();

	m_ThreadGroup.join_all();

	delete m_pPacketPool;
	delete m_pSessionPool;

	std::cout << "--- Static Finalize" << std::endl;

}

void
Static::Start()
{
	Accept();

	for (int i = 0; i < m_nThreadCount; i++)
	{
		m_ThreadGroup.create_thread(boost::bind(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &m_ioService));
	}
}

void
Static::Accept()
{
	Session * pSession = GETSESSIONPOOL()->NEW();

	m_Acceptor.async_accept(pSession->GetSocket(),
			boost::bind(&Static::OnSessionOpen, this, pSession, boost::asio::placeholders::error)
	);
}

void
Static::OnSessionOpen(Session * pSession, const boost::system::error_code & error)
{
	// Client Connect Success
	Packet * pPacket = Static::Instance().GETPACKETPOOL()->READ_NEW();
	pPacket->SetPacketType(Packet::PacketType::OPEN);
	pSession->PacketEnqueue(pPacket);

	// 이벤트 발생
	IListener * pListener = Static::Instance().GetListener();
	if (NULL != pListener)
	{
		pListener->OnNetworkEvent(pSession);
	}

	pSession->HandleRead();
	Accept();
}

} /*BoostAsioNetwork*/
