/*
 * CHAT.cpp
 *
 *  Created on: 2014. 9. 3.
 *      Author: mega
 */

#include "../User.h"
#include "../Command.h"
#include "../Protocol.h"
#include "../NLogic/Function.h"
#include "ChatMessage.pb.h"

namespace ChatServer { namespace NCommand {

void
Command::CHAT(User * pUser, ::BoostAsioNetwork::Packet * pPacket)
{
	ChatMessage syn;
	syn.ParseFromArray(pPacket->GetBodyPtr(), pPacket->GetBodySize());

	NLogic::Broadcast(pUser, syn.msg().c_str());


#if 0
	ChatMessage ack;
	ack.set_msg("PONG");

	::BoostAsioNetwork::Packet * pAck = ::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->SEND_NEW();
	ack.SerializeToArray(pAck->GetBodyPtr(), ack.ByteSize());
	pAck->MakePacket(CMD_CHAT, ack.ByteSize());
	pUser->SendPacket(pAck);
#endif
}

} /*NCommand*/ } /*ChatServer*/

