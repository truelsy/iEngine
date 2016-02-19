/*
 * Static.h
 *
 *  Created on: 2016. 2. 17.
 *      Author: mega
 */

#ifndef STATIC_H_
#define STATIC_H_

namespace Server {

class Static : public ::Network::NBase::Single<Static>
{
public:
	Static(int sessionCount)
	{
		_userPool = new ::Network::NBase::Pool<User>(sessionCount);
	};

	virtual ~Static() {};

	::Network::NBase::Pool<User> *	GETUSERPOOL() { return _userPool; }

private:
	::Network::NBase::Pool<User> * _userPool;
};

} /*Server*/



#endif /* STATIC_H_ */
