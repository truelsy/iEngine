/*
 * Static.cpp
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#include "../Static.h"
#include <iostream>

namespace Memcache {

Static::Static() : pSession(new Session)
{
	pCommandPool = new Command::Pool;
	pCommandPool->Init();
}

Static::~Static()
{
	std::cout << "-- Memcached ~Static" << std::endl;
	delete pSession;
}

Session *
Static::GetSession()
{
	return pSession;
}

Command::Pool *
Static::GetCommandPool()
{
	return pCommandPool;
}

} /*Memcached*/


