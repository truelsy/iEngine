/*
 * User.cpp
 *
 *  Created on: 2014. 6. 27.
 *      Author: mega
 */

#include "../User.h"
#include "../NState/Static.h"
#include "../NLogic/Function.h"


namespace ChatServer {

User::User() : m_pState(NULL)
{
	SetState(NState::Static::Instance().AUTHSTATE());
	m_CGCIIPacketCount = 0;
}

User::~User()
{
}

void
User::OnPacket()
{
	::BoostAsioNetwork::Packet * pPacket = NULL;

	while (true)
	{
		pPacket = this->PacketDequeue();
		if (NULL == pPacket)
			break;

		if (pPacket->GetPacketType() == ::BoostAsioNetwork::Packet::PacketType::OPEN)
		{
			//std::cout << "Session Open ~" << std::endl;

			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}

		if (pPacket->GetPacketType() == ::BoostAsioNetwork::Packet::PacketType::CLOSE)
		{
			//std::cout << "Session Close ~" << std::endl;
			NLogic::DelUser(this);

			this->Close();

			::BoostAsioNetwork::Static::Instance().GETSESSIONPOOL()->DEL(this);
			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}


		if (NULL == m_pState)
		{
			std::cout << "OOPS! m_pState is NULL!" << std::endl;
			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}

#ifdef CGCII_BMT
		::BoostAsioNetwork::Packet * pAck = ::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->SEND_NEW();
		memcpy(pAck->GetBuf().data(), pPacket->GetBuf().data(), pPacket->GetBodySize());
		pAck->MakePacket(0x00, pAck->GetBodySize());
		SendPacket(pAck);

		//m_CGCIIPacketCount++;
		//if (m_CGCIIPacketCount % 100 == 0)
		//{
		//	std::cout << "PROC COUNT : " << m_CGCIIPacketCount << std::endl;
		//}
#else
		uint16_t msgType = pPacket->GetCommand();

		NState::State::Command pCommand = m_pState->GetCommand(msgType);
		if (NULL == pCommand)
		{
			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}

		pCommand(this, pPacket);
#endif

		// 패킷 반환
		::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
	}
}

void
User::SendPacket(::BoostAsioNetwork::Packet * pPacket)
{
	HandleWrite(pPacket);
}

} /*ChatServer*/
