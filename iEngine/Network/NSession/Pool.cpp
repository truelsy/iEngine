/*
 * Pool.cpp
 *
 *  Created on: 2014. 6. 27.
 *      Author: mega
 */

#include "../Session.h"

namespace BoostAsioNetwork {

Session::Pool::Pool()
{
	pthread_cond_init(&m_Cond, NULL);
	pthread_mutex_init(&m_Lock, NULL);
}

Session::Pool::~Pool()
{
	for (Session * pSession : clsSessionList)
		delete pSession;

	clsSessionList.clear();
}

Session *
Session::Pool::NEW()
{
	std::cout << "NEW Before Session Count : " << clsSessionList.size() << std::endl;

	Session * pSession = NULL;
	{
		pthread_mutex_lock(&m_Lock);

		if (true == clsSessionList.empty())
		{
			pthread_cond_wait(&m_Cond, &m_Lock);
		}

		pSession = clsSessionList.front();
		clsSessionList.pop_front();

		pthread_mutex_unlock(&m_Lock);
	}

	std::cout << "NEW After  Session Count : " << clsSessionList.size() << std::endl;

	return pSession;
}

void
Session::Pool::DEL(Session * pSession)
{
	pSession->Finalize();

	std::cout << "DEL Before Session Count : " << clsSessionList.size() << std::endl;

	pthread_mutex_lock(&m_Lock);
	clsSessionList.push_back(pSession);
	pthread_cond_signal(&m_Cond);
	pthread_mutex_unlock(&m_Lock);

	std::cout << "DEL After  Session Count : " << clsSessionList.size() << std::endl;
}

void
Session::Pool::POST(Session * pSession)
{
	pthread_mutex_lock(&m_Lock);
	clsSessionList.push_back(pSession);
	pthread_mutex_unlock(&m_Lock);
}

} /*BoostAsioNetwork*/
