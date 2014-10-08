/*
 * Command.cpp
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#include "../Command.h"

namespace Memcache {

Command::Command()
{
	m_rc = MEMCACHED_SUCCESS;
	m_errorString.clear();
}

Command::~Command()
{

}

void
Command::Initialize()
{
	m_rc = MEMCACHED_SUCCESS;
	m_errorString.clear();
	clsData.Clear();
	clsResultSet.Init();
}

void
Command::Finalize()
{

}

} /*Memcached*/

