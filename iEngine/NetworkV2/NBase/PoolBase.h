/*
 * PoolBase.h
 *
 *  Created on: 2016. 2. 19.
 *      Author: mega
 */

#ifndef NETWORK_NBASE_POOLBASE_H_
#define NETWORK_NBASE_POOLBASE_H_

#include <memory>
#include <deque>

namespace Network { namespace NBase {

template<class Type>
class PoolBase
{
public:
	PoolBase(int poolSize = 1);
	virtual ~PoolBase();

	PoolBase (const PoolBase & other) = delete;
	PoolBase & operator= (const PoolBase & other) = delete;

public:
	virtual std::shared_ptr<Type> NEW();
	void Post(Type * pType);

	int		GetPoolSize() { return _objectQueue.size(); }

protected:
	virtual Type * Allocator();
	virtual void Deleter(Type * pType);

protected:
	unsigned int _nNewMakeCount;
	std::deque<Type *> _objectQueue;
};

template<class Type>
PoolBase<Type>::PoolBase (int poolSize)
{
	_nNewMakeCount = 0;
	_objectQueue.clear();

	for (int i = 0; i < poolSize; ++i)
	{
		this->Post(new Type);
	}
}

template<class Type>
PoolBase<Type>::~PoolBase ()
{
	for (auto * pType : _objectQueue)
	{
		delete pType;
	}

	_objectQueue.clear();
}

template<class Type>
std::shared_ptr<Type> PoolBase<Type>::NEW()
{
	Type * pType = this->Allocator();

	return std::shared_ptr<Type>(pType, [this](Type * pType) {
		if (NULL != pType)
		{
			this->Deleter(pType);
		}
	} );
}

template<class Type>
void PoolBase<Type>::Post(Type * pType)
{
	if (pType)
	{
		_objectQueue.push_back(pType);
	}
}

template<class Type>
Type * PoolBase<Type>::Allocator()
{
	Type * pType = NULL;
	{
		if (false == _objectQueue.empty())
		{
			pType = _objectQueue.front();
			_objectQueue.pop_front();
		}
	}

	DLOG(INFO) << __FUNCTION__ << "Pool NEW. type(" << typeid(Type).name() << ") poolSize(" << _objectQueue.size() << ")";

	return pType;
}

template<class Type>
void PoolBase<Type>::Deleter(Type * pType)
{
	if (pType)
	{
		_objectQueue.push_back(pType);

		DLOG(INFO) << __FUNCTION__ << "Pool DEL. type(" << typeid(Type).name() << ") poolSize(" << _objectQueue.size() << ")";
	}
}

} /*NBase*/ } /*Network*/



#endif /* NETWORK_NBASE_POOLBASE_H_ */
