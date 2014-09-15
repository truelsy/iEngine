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
}

User::~User()
{
}

void
User::OnPacket()
{
	uint16_t msgType;
	::BoostAsioNetwork::Packet * pPacket = NULL;

	while (true)
	{
		pPacket = this->PacketDequeue();
		if (NULL == pPacket)
			break;

		if (pPacket->GetPacketType() == ::BoostAsioNetwork::Packet::PacketType::OPEN)
		{
			std::cout << "Session Open ~" << std::endl;

			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}

		if (pPacket->GetPacketType() == ::BoostAsioNetwork::Packet::PacketType::CLOSE)
		{
			std::cout << "Session Close ~" << std::endl;
			NLogic::DelUser(this);

			this->Close();

			::BoostAsioNetwork::Static::Instance().GETSESSIONPOOL()->DEL(this);
			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}


		if (NULL == m_pState)
		{
			std::cout << "WOW. m_pState is NULL!" << std::endl;
			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}

		msgType = pPacket->GetCommand();

		NState::State::Command pCommand = m_pState->GetCommand(msgType);
		if (NULL == pCommand)
		{
			::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->READ_DEL(pPacket);
			continue;
		}

		pCommand(this, pPacket);

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
