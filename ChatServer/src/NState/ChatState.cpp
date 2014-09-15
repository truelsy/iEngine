/*
 * ChatState.cpp
 *
 *  Created on: 2014. 9. 3.
 *      Author: mega
 */

#include "../Protocol.h"
#include "../Command.h"
#include "ChatState.h"

namespace ChatServer { namespace NState {

ChatState::ChatState()
{
	AddCommand(CMD_CHAT, NCommand::Command::CHAT);
}

ChatState::~ChatState()
{
}

} /* namespace NState */ } /* namespace ChatServer */
