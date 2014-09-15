/*
 * Handler.h
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#ifndef HANDLER_H_
#define HANDLER_H_

#include <queue>
#include <pthread.h>

#include "NHandler/Wrapper.h"

#include "../Network/IListener.h"
#include "../Network/Session.h"

namespace Dispatch {

class Handler : public ::BoostAsioNetwork::IListener
{
public:
	Handler();
	virtual ~Handler();

	virtual void	OnNetworkEvent(::BoostAsioNetwork::Session * pSession);

private:
	static void *		doWork(void * pData);
	static void		doCleanup(void * pData);

private:
	pthread_cond_t		m_Cond;
	pthread_mutex_t		m_Lock;
	pthread_attr_t		m_Attr;
	pthread_t			m_pThread;

	bool				m_bThreadCreate;
	bool				m_bThreadExitWait;
	bool				m_bThreadCancel;

	std::queue<NHandler::Wrapper *>	m_pWrapperQueue;
};

} /*Dispatch*/

#endif /* HANDLER_H_ */
