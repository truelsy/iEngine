/*
 * Static.h
 *
 *  Created on: 2014. 9. 3.
 *      Author: mega
 */

#ifndef SERVER_NSTATE_STATIC_H_
#define SERVER_NSTATE_STATIC_H_

#include "ChatState.h"
#include "AuthState.h"

namespace ChatServer { namespace NState {

class Static : public ::BoostAsioNetwork::NBase::Single<Static>
{
public:
	Static()
	{
		m_pChatState	= new ChatState;
		m_pAuthState	= new AuthState;
	}

	~Static()
	{
		delete m_pChatState;
		delete m_pAuthState;
	}

public:
	ChatState *	CHATSTATE() { return m_pChatState; }
	AuthState * AUTHSTATE() { return m_pAuthState; }

private:
	ChatState *	m_pChatState;
	AuthState * m_pAuthState;
};

} /*NState*/ } /*ChatServer*/


#endif /* SERVER_NSTATE_STATIC_H_ */
