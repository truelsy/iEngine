/*
 * AUTH.cpp
 *
 *  Created on: 2014. 9. 4.
 *      Author: mega
 */

#include "../User.h"
#include "../Command.h"
#include "../NState/Static.h"
#include "../NLogic/Function.h"

#include "ChatMessage.pb.h"

namespace ChatServer { namespace NCommand {

void
Command::AUTH(User * pUser, ::BoostAsioNetwork::Packet * pPacket)
{
	AuthRequest syn;
	syn.ParseFromArray(pPacket->GetBodyPtr(), pPacket->GetBodySize());

	// 대화명 설정
	pUser->SetNickName(syn.nickname().c_str());

	// 채팅 가능 상태로 변경
	pUser->SetState(NState::Static::Instance().CHATSTATE());

	NLogic::AddUser(pUser);
}

} /*NCommand*/ } /*ChatServer*/


