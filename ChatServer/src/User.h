/*
 * User.h
 *
 *  Created on: 2014. 6. 27.
 *      Author: mega
 */

#ifndef USER_H_
#define USER_H_

#include <Network/Static.h>
#include <Network/Session.h>
#include <Network/Packet.h>

namespace ChatServer { namespace NState {
class State;
} /*NState*/ } /*ChatServer*/

namespace ChatServer {

class User : public ::BoostAsioNetwork::Session
{
public:
	User();
	virtual ~User();

	virtual void	OnPacket();

	void	SendPacket(::BoostAsioNetwork::Packet * pPacket);

	void	SetState(NState::State * pState) { m_pState = pState; }
	NState::State *	GetState() { return m_pState; }

	void	SetNickName(const char * nickName) { m_nickName = nickName; }
	std::string & GetNickName() { return m_nickName; }

private:
	NState::State *	m_pState;
	std::string		m_nickName;
	int				m_CGCIIPacketCount;
};

} /*ChatServer*/

#endif /* USER_H_ */
