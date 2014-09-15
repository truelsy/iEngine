/*
 * Static.h
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#ifndef NHANDLER_STATIC_H_
#define NHANDLER_STATIC_H_

#include <list>
#include <pthread.h>

#include "Wrapper.h"
#include "../NBase/Single.h"

namespace Dispatch { namespace NHandler {

class Static : public NBase::Single<Static>
{
public:
	Static();
	virtual ~Static();

	void	Initialize();
	void	Finalize();

	Wrapper *	NEW();
	void		DEL(Wrapper * pWrapper);

private:
	std::list<Wrapper *>	m_pWrapperPool;
	pthread_mutex_t			m_Lock;
};

} /*NHandler*/ } /*Dispatch*/


#endif /* NHANDLER_STATIC_H_ */
