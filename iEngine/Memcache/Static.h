/*
 * Static.h
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#ifndef MEMCACHE_STATIC_H_
#define MEMCACHE_STATIC_H_

#include "NBase/Single.h"
#include "Session.h"
#include "Command.h"

namespace Memcache {

//class Command;

class Static : public NBase::Single<Static>
{
public:
	Static();
	virtual ~Static();

public:
	Session *	GetSession();
	Command::Pool *	GetCommandPool();

private:
	Session * pSession;
	Command::Pool * pCommandPool;
};


static inline Session * GETSESSION()
{
	return Static::Instance().GetSession();
}

static inline Command::Pool * GETCOMMANDPOOL()
{
	return Static::Instance().GetCommandPool();
}

} /*Memcached*/

#endif /* MEMCACHE_STATIC_H_ */
