/*
 * Command.h
 *
 *  Created on: 2014. 9. 3.
 *      Author: mega
 */

#ifndef SERVER_COMMAND_H_
#define SERVER_COMMAND_H_

#include "User.h"

namespace ChatServer { namespace NCommand {

class Command
{
public:
	static void AUTH(User * pUser, ::BoostAsioNetwork::Packet * pPacket);
	static void CHAT(User * pUser, ::BoostAsioNetwork::Packet * pPacket);
};

} /*NCommand*/ } /*ChatServer*/


#endif /* SERVER_COMMAND_H_ */
