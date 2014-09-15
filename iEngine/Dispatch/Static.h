/*
 * Static.h
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#ifndef DISPATCH_STATIC_H_
#define DISPATCH_STATIC_H_

#include "NHandler/Static.h"

#include "Handler.h"
#include "NBase/Single.h"


namespace Dispatch {


class Static : public NBase::Single<Static>
{
public:
	Static();
	virtual ~Static();

	void		Initialize();
	void		Finalize();

	Handler *	GetHandler();

private:
	Handler * 	m_pHandler;
	NHandler::Static *	m_pHandlerStatic;
};

inline Handler * GETHANDLER()
{
	return Static::Instance().GetHandler();
}

} /*Dispatch*/


#endif /* DISPATCH_STATIC_H_ */
