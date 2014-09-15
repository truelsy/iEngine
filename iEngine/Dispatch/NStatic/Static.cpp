/*
 * Static.cpp
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#include "../Static.h"

namespace Dispatch {

Static::Static()
{
	m_pHandler = new Handler;
	m_pHandlerStatic = new NHandler::Static;
}

Static::~Static()
{
	delete m_pHandler;
	delete m_pHandlerStatic;
}

void
Static::Initialize()
{
	m_pHandlerStatic->Initialize();
}

void
Static::Finalize()
{
	m_pHandlerStatic->Finalize();
}

Handler *
Static::GetHandler()
{
	return m_pHandler;
}

} /*Dispatch*/
