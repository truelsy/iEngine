/*
 * ChatState.h
 *
 *  Created on: 2014. 9. 3.
 *      Author: mega
 */

#ifndef SERVER_NSTATE_CHATSTATE_H_
#define SERVER_NSTATE_CHATSTATE_H_

#include "State.h"

namespace ChatServer { namespace NState {

class ChatState : public ::ChatServer::NState::State
{
public:
	ChatState();
	virtual ~ChatState();
};

} /* namespace NState */ } /* namespace ChatServer */

#endif /* SERVER_NSTATE_CHATSTATE_H_ */
