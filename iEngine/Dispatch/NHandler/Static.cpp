/*
 * Static.cpp
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#include "Static.h"

namespace Dispatch { namespace NHandler {

Static::Static()
{
	pthread_mutex_init(&m_Lock, NULL);
}

Static::~Static()
{
	Finalize();
}

void
Static::Initialize()
{
	for (int i = 0; i < 1024; i++)
	{
		m_pWrapperPool.push_back(new Wrapper);
	}
}

void
Static::Finalize()
{
	pthread_mutex_lock(&m_Lock);

	for (Wrapper * pWrapper : m_pWrapperPool)
	{
		delete pWrapper;
	}

	m_pWrapperPool.clear();

	pthread_mutex_unlock(&m_Lock);
}

Wrapper *
Static::NEW()
{
	Wrapper * pWrapper = NULL;

	pthread_mutex_lock(&m_Lock);

	if (true == m_pWrapperPool.empty())
	{
		pWrapper = new Wrapper;
	}
	else
	{
		pWrapper = m_pWrapperPool.front();
		m_pWrapperPool.pop_front();
	}

	pthread_mutex_unlock(&m_Lock);

	return pWrapper;
}

void
Static::DEL(Wrapper * pWrapper)
{
	pthread_mutex_lock(&m_Lock);

	m_pWrapperPool.push_back(pWrapper);

	pthread_mutex_unlock(&m_Lock);
}

} /*NHandler*/ } /*Dispatch*/
