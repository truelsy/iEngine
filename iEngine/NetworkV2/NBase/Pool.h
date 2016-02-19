/*
 * Pool.h
 *
 *  Created on: 2016. 2. 17.
 *      Author: mega
 */

#ifndef NETWORKDV2_NBASE_POOL_H_
#define NETWORKDV2_NBASE_POOL_H_

#include <mutex>
#include <stddef.h>
#include <glog/logging.h>
#include "PoolBase.h"

namespace Network { namespace NBase {

template<class Type>
class Pool : public PoolBase<Type>
{
public:
	Pool(int poolSize = 1);
	virtual ~Pool();

protected:
	virtual Type * Allocator();
	virtual void Deleter(Type * pType);

private:
	std::deque<Type *> _clsTypeDeque;
	std::mutex _queueLock;
};


template<class Type>
Pool<Type>::Pool(int poolSize) : PoolBase<Type>(poolSize)
{
}


template<class Type>
Pool<Type>::~Pool()
{
}

template<class Type> Type *
Pool<Type>::Allocator ()
{
	Type * pType = NULL;
	{
		std::lock_guard<std::mutex> lock(_queueLock);
		if (false == PoolBase<Type>::_objectQueue.empty())
		{
			pType = PoolBase<Type>::_objectQueue.front();
			PoolBase<Type>::_objectQueue.pop_front();
		}
	}

	if (NULL == pType)
	{
		pType = new Type;
	}

	pType->Initialize();

	return pType;
}

template<class Type> void
Pool<Type>::Deleter(Type * pType)
{
	if (pType)
	{
		pType->Finalize();

		{
			std::lock_guard<std::mutex> lock(_queueLock);
			PoolBase<Type>::_objectQueue.push_back(pType);
		}
	}
}

} /*NBase*/ } /*Network*/



#endif /* NETWORKDV2_NBASE_POOL_H_ */
