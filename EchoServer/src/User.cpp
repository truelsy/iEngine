/*
 * Session.cpp
 *
 *  Created on: 2016. 2. 17.
 *      Author: mega
 */

#include "User.h"

namespace Server {

User::User()
{
}

User::~User()
{
}

void
User::OnOpen()
{
	DLOG(INFO) << __FUNCTION__ << " : Session Connected. PoolSize(" << ::Network::Static::Instance().GETSESSIONPOOL()->GetPoolSize() << ")";
}

void
User::OnClose()
{
	if (_pSession)
	{
		_pSession->Close();
		_pSession = NULL;
	}
}

void
User::OnMessage()
{
	std::shared_ptr<::Network::Packet> readPacket = _pSession->GetReadPacketQueue().front();
	_pSession->GetReadPacketQueue().pop_front();

	if (NULL == readPacket.get())
		return;

	std::shared_ptr<::Network::Packet> sendPacket = ::Network::Static::Instance().GETPACKETPOOL_SEND()->NEW();
	sendPacket->SetHeaderSize(readPacket->GetHeaderSize());
	sendPacket->SetBodySize(readPacket->GetBodySize());
	memcpy(sendPacket->GetBuf().data(), readPacket->GetBuf().data(), readPacket->GetSize());

	_pSession->Send(sendPacket);
}

} /*Server*/


