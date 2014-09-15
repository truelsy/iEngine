/*
 * AuthState.cpp
 *
 *  Created on: 2014. 9. 4.
 *      Author: mega
 */

#include "../Protocol.h"
#include "../Command.h"
#include "AuthState.h"

namespace ChatServer { namespace NState {

AuthState::AuthState()
{
	AddCommand(CMD_AUTH, NCommand::Command::AUTH);
}

AuthState::~AuthState()
{
}

} /* namespace NState */ } /* namespace ChatServer */
