/*
 * Handler.cpp
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#include "Static.h"
#include "../Handler.h"

namespace Dispatch {

Handler::Handler()
{
	pthread_cond_init(&m_Cond, NULL);
	pthread_mutex_init(&m_Lock, NULL);

	pthread_attr_init(&m_Attr);
	pthread_attr_setdetachstate(&m_Attr, PTHREAD_CREATE_DETACHED);

	int ret = pthread_create(&m_pThread, &m_Attr, &Handler::doWork, (void*)this);

	m_bThreadCreate = (ret == 0);

	pthread_attr_destroy(&m_Attr);

	m_bThreadExitWait = false;
	m_bThreadCancel = false;
}

Handler::~Handler()
{
	if (true == m_bThreadCreate)
	{
		m_bThreadCreate = false;

		// wait for thread created
		int count = 10000;
		while (!m_bThreadExitWait && count-- > 0)
			usleep(100);

		m_bThreadCancel = true;
		pthread_cond_signal(&m_Cond);

		// wait for thread exit
		count = 10000;
		while (m_bThreadExitWait && count-- > 0)
			usleep(100);
	}
}

void *
Handler::doWork(void * pParam)
{
#if 0
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_cleanup_push(Handler::doCleanup, pData);
#endif

	Handler * pHandler = (Handler *)pParam;

	pHandler->m_bThreadExitWait = true;

	while (!pHandler->m_bThreadCancel)
	{
#if 0
		pthread_testcancel();
#endif

		pthread_mutex_lock(&pHandler->m_Lock);

		if (true == pHandler->m_pWrapperQueue.empty())
		{
			pthread_cond_wait(&pHandler->m_Cond, &pHandler->m_Lock);
		}

		// when waked up by system broadcast signal, loop again
		if (true == pHandler->m_pWrapperQueue.empty())
		{
			pthread_mutex_unlock (&pHandler->m_Lock);
			continue;
		}

		NHandler::Wrapper * pWrapper = pHandler->m_pWrapperQueue.front();
		pHandler->m_pWrapperQueue.pop();

		pthread_mutex_unlock (&pHandler->m_Lock);

		if (pWrapper->GetKind() == NHandler::Wrapper::Event::__NETWORK__)
		{
			::BoostAsioNetwork::Session * pSession = (::BoostAsioNetwork::Session *) pWrapper->GetType();
			pSession->OnPacket();
		}

		NHandler::Static::Instance().DEL(pWrapper);
	}

#if 0
	pthread_cleanup_pop(0);
#endif

	std::cout << "Thread Cancel!!!" << std::endl;

	doCleanup(pParam);

	return NULL;
}

void
Handler::doCleanup(void * pParam)
{
	Handler * pHandler = (Handler *)pParam;
	pHandler->m_bThreadExitWait = false;
}

void
Handler::OnNetworkEvent(::BoostAsioNetwork::Session * pSession)
{
	NHandler::Wrapper * pWrapper = NHandler::Static::Instance().NEW();
	if (NULL == pWrapper)
		return;

	pWrapper->SetType(pSession);
	pWrapper->SetKind(NHandler::Wrapper::Event::__NETWORK__);

	pthread_mutex_lock(&m_Lock);
	m_pWrapperQueue.push(pWrapper);
	pthread_cond_signal(&m_Cond);
	pthread_mutex_unlock(&m_Lock);
}

} /*Dispatch*/
