/*
 * IListener.h
 *
 *  Created on: 2014. 7. 7.
 *      Author: mega
 */

#ifndef ILISTENER_H_
#define ILISTENER_H_

namespace Network {

class Session;

class IListener
{
public:
	virtual ~IListener() {}

public:
	virtual void	OnNetworkEvent(Session * pSession) = 0;
};

} /*BoostAsioNetwork*/



#endif /* ILISTENER_H_ */
