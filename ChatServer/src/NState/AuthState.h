/*
 * AuthState.h
 *
 *  Created on: 2014. 9. 4.
 *      Author: mega
 */

#ifndef SERVER_NSTATE_AUTHSTATE_H_
#define SERVER_NSTATE_AUTHSTATE_H_

#include "State.h"

namespace ChatServer { namespace NState {

class AuthState : public ::ChatServer::NState::State
{
public:
	AuthState();
	virtual ~AuthState();
};

} /* namespace NState */ } /* namespace ChatServer */

#endif /* SERVER_NSTATE_AUTHSTATE_H_ */
