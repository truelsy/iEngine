/*
 * Function.cpp
 *
 *  Created on: 2014. 9. 4.
 *      Author: mega
 */

#include "../Static.h"
#include "../Protocol.h"
#include "ChatMessage.pb.h"
#include "Function.h"

namespace ChatServer { namespace NLogic {

void
AddUser(User * pUser)
{
	std::string joinMessage = "# '" + pUser->GetNickName() + "' is join.";

	ChatMessage ack;
	ack.set_msg(joinMessage);

	std::list<User *> & clsUserList = Static::Instance().GetUserList();
	auto iter = clsUserList.begin();

	for (; iter != clsUserList.end(); iter++)
	{
		::BoostAsioNetwork::Packet * pAck = ::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->SEND_NEW();
		ack.SerializeToArray(pAck->GetBodyPtr(), ack.ByteSize());
		pAck->MakePacket(CMD_CHAT, ack.ByteSize());
		(* iter)->SendPacket(pAck);
	}

	clsUserList.push_back(pUser);
}

void
DelUser(User * pUser)
{
	std::string leaveMessage = "# '" + pUser->GetNickName() + "' is leave.";

	ChatMessage ack;
	ack.set_msg(leaveMessage);

	std::list<User *> & clsUserList = Static::Instance().GetUserList();
	auto iter = clsUserList.begin();

	for (; iter != clsUserList.end(); iter++)
	{
		if (*iter == pUser)
			continue;

		::BoostAsioNetwork::Packet * pAck = ::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->SEND_NEW();
		ack.SerializeToArray(pAck->GetBodyPtr(), ack.ByteSize());
		pAck->MakePacket(CMD_CHAT, ack.ByteSize());
		(* iter)->SendPacket(pAck);
	}

	clsUserList.remove(pUser);
}

void
Broadcast(User * pUser, const char * message)
{
	std::string chatMessage = pUser->GetNickName() + " > " + message;

	ChatMessage ack;
	ack.set_msg(chatMessage);

	std::list<User *> & clsUserList = Static::Instance().GetUserList();
	auto iter = clsUserList.begin();

	for (; iter != clsUserList.end(); iter++)
	{
		if (*iter == pUser)
			continue;

		::BoostAsioNetwork::Packet * pAck = ::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->SEND_NEW();
		ack.SerializeToArray(pAck->GetBodyPtr(), ack.ByteSize());
		pAck->MakePacket(CMD_CHAT, ack.ByteSize());
		(* iter)->SendPacket(pAck);
	}
}

} /* namespace NLogic */ } /* namespace ChatServer */
