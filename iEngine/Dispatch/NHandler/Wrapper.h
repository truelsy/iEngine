/*
 * Wrapper.h
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#ifndef WRAPPER_H_
#define WRAPPER_H_

namespace Dispatch { namespace NHandler {

class Wrapper
{
public:
	enum Event
	{
		__NONE__ = 0,
		__NETWORK__,
	};

public:
	Wrapper() : m_pType(0), m_eKind(__NONE__) {}
	virtual ~Wrapper() {}

	void	SetType(void * pType) { m_pType = pType; }
	void *	GetType() { return m_pType; }

	void	SetKind(Event eKind) { m_eKind = eKind; }
	Event	GetKind() { return m_eKind; }

private:
	void *	m_pType;
	Event	m_eKind;
};

} /*NHandler*/ } /*Dispatch*/


#endif /* WRAPPER_H_ */
