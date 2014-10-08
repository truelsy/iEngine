/*
 * Session.cpp
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#include <iostream>
#include <libmemcached/memcached.h>
#include "../Session.h"

namespace Memcache {

Session::Session()
{
	m_pMemc		= NULL;
	m_pServer	= NULL;
}

Session::~Session()
{
	std::cout << "-- Memcached ~Session" << std::endl;
	Finalize();
}

bool
Session::Initialize(const char * address)
{
	memcached_return_t rc;
	m_pMemc		= memcached_create(NULL);
	m_pServer	= memcached_servers_parse(address);

	rc = memcached_server_push(m_pMemc, m_pServer);
	if (rc != MEMCACHED_SUCCESS)
		return false;

	if (1 < m_pServer->number_of_hosts)
		memcached_behavior_set(m_pMemc, MEMCACHED_BEHAVIOR_KETAMA_HASH, 1);

	return true;
}

bool
Session::Finalize()
{
	if (NULL != m_pServer)
		memcached_server_free(m_pServer);
//		memcached_server_list_free(m_pServer);

	if (NULL != m_pMemc)
		memcached_free(m_pMemc);

	m_pMemc		= NULL;
	m_pServer	= NULL;

	return true;
}

bool
Session::Execute(Command * pCommand)
{
	if (pCommand->GetData()._eAction == Command::Action::SET)
	{
		memcached_return_t rc = Set(pCommand->GetData()._key.c_str(), pCommand->GetResultSet().m_Data, pCommand->GetResultSet().m_Offset, pCommand->GetData()._expire);
		pCommand->SetReturnCode(rc);
		pCommand->SetErrorString(GetErrorString(rc));

		if (rc == MEMCACHED_SUCCESS)
			return true;
		else
			return false;
	}

	if (pCommand->GetData()._eAction == Command::Action::GET)
	{
		uint32_t flasg = 0;
		memcached_return_t rc;
		pCommand->GetResultSet().m_pVal = Get(pCommand->GetData()._key.c_str(), &pCommand->GetData()._valueLen, &flasg, &rc);
		pCommand->SetReturnCode(rc);
		pCommand->SetErrorString(GetErrorString(rc));

		if (rc != MEMCACHED_SUCCESS)
			return false;

		pCommand->GetResultSet().Init(pCommand->GetResultSet().m_pVal, pCommand->GetData()._valueLen);
		return true;
	}

	return false;
}

const char *
Session::GetErrorString(memcached_return_t rc)
{
	return memcached_strerror(m_pMemc, rc);
}

memcached_return_t
Session::Set( const char *key, const char *value, const size_t size, time_t expiration )
{
	return memcached_set(m_pMemc, key, strlen(key), value, size, expiration, 0);
}

char *
Session::Get( const char *key, size_t *valueLen, uint32_t *flags, memcached_return_t *error )
{
	return memcached_get(m_pMemc, key, strlen(key), valueLen, flags, error);
}

} /*Memcached*/


