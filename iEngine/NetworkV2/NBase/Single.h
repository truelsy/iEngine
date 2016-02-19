/*
 * Single.h
 *
 *  Created on: 2014. 6. 26.
 *      Author: mega
 */

#ifndef NETWORKV2_NBASE_SINGLE_H_
#define NETWORKV2_NBASE_SINGLE_H_

namespace Network { namespace NBase {

#ifdef __LP64__
	typedef long long SINGLE_OFFSET;
#else
	typedef int SINGLE_OFFSET;
#endif


template<typename Type>
class Single
{
public:
	Single()
	{
		SINGLE_OFFSET offset = (SINGLE_OFFSET)((Type*)0 + 1) - (SINGLE_OFFSET)(Single <Type>*)((Type*)0+1);
		m_Type = (Type*)((SINGLE_OFFSET)this + offset);
	}
	~Single()
	{
		m_Type = 0;
	}

	static Type & Instance()
	{
		return ( *m_Type);
	}
	static Type * InstancePtr()
	{
		return (m_Type);
	}

private:
	static Type * m_Type;
};

template<typename Type> Type* Single<Type>::m_Type = 0;


} /*NBase*/ } /*Network*/



#endif /* NETWORKV2_NBASE_SINGLE_H_ */
