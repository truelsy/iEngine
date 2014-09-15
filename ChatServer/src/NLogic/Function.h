/*
 * Function.h
 *
 *  Created on: 2014. 9. 4.
 *      Author: mega
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

namespace ChatServer { namespace NLogic {

void	AddUser(User * pUser);
void	DelUser(User * pUser);
void	Broadcast(User * pUser, const char * message);

} /* namespace NLogic */ } /* namespace ChatServer */

#endif /* FUNCTION_H_ */
