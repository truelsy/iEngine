/*
 * State.cpp
 *
 *  Created on: 2014. 9. 3.
 *      Author: mega
 */

#include "State.h"

namespace ChatServer { namespace NState {

State::State()
{
}

State::~State()
{
}

State::Command
State::GetCommand(uint16_t msgType)
{
	auto iter = clsCommandMap.find(msgType);
	if (iter == clsCommandMap.end())
		return NULL;

	return iter->second;
}

void
State::AddCommand(uint16_t msgType, State::Command pCommand)
{
	auto iter = clsCommandMap.find(msgType);
	if (iter != clsCommandMap.end())
		return;

	clsCommandMap[msgType] = pCommand;
}

} /* namespace NState */ } /* namespace ChatServer */
