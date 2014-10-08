/*
 * Session.h
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#ifndef MEMCACHE_SESSION_H_
#define MEMCACHE_SESSION_H_

#include "Command.h"

namespace Memcache {

class Session
{
public:
	Session();
	virtual ~Session();

	bool	Initialize(const char * address);
	bool	Finalize();

	bool	Execute(Command * pCommand);

private:
	memcached_return_t	Set( const char *key, const char *value, const size_t size, time_t expiration );
	char *	Get( const char *key, size_t *valueLen, uint32_t *flags, memcached_return_t *error );
	const char *	GetErrorString(memcached_return_t rc);

private:
	memcached_st		* m_pMemc;
	memcached_server_st	* m_pServer;

};

} /*Memcached*/


#endif /* MEMCACHE_SESSION_H_ */
