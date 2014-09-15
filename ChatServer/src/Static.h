/*
 * Static.h
 *
 *  Created on: 2014. 9. 4.
 *      Author: mega
 */

#ifndef STATIC_H_
#define STATIC_H_

#include <list>
#include "User.h"

namespace ChatServer {

class Static : public ::BoostAsioNetwork::NBase::Single<Static>
{
public:
	Static() {};
	virtual ~Static() {};

	std::list<User *> &	GetUserList() { return clsUserList; }

private:
	std::list<User *>	clsUserList;
};

} /*ChatServer*/


#endif /* STATIC_H_ */
