/*
 * State.h
 *
 *  Created on: 2014. 9. 3.
 *      Author: mega
 */

#ifndef SERVER_NSTATE_STATE_H_
#define SERVER_NSTATE_STATE_H_

#include <map>
#include "../User.h"

namespace ChatServer { namespace NState {

class State
{
public:
	State();
	virtual ~State();

public:
	typedef void(* Command)(User * pUser, ::BoostAsioNetwork::Packet * pPacket);

public:
	Command		GetCommand(uint16_t msgType);

protected:
	void		AddCommand(uint16_t msgType, Command pCommand);

protected:
	std::map<int, Command>	clsCommandMap;
};

} /* namespace NState */ } /* namespace ChatServer */

#endif /* SERVER_NSTATE_STATE_H_ */
