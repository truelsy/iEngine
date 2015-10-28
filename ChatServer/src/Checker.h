/*
 * Checker.h
 *
 *  Created on: 2014. 9. 22.
 *      Author: mega
 */

#ifndef CHECKER_H_
#define CHECKER_H_

#include "Static.h"
#include <functional>
#include <Timer/Timer.h>

namespace ChatServer {

class Checker
{
public:
	Checker()
	{
		pTimer = new ::NTimer::Timer;
		SetPacketCountCheckTimer();
	}

	~Checker()
	{
		delete pTimer;
	}

public:
	void	OnTime()
	{
		std::cout << "SEND PACKET : " << ::BoostAsioNetwork::Static::Instance().GETPACKETPOOL()->GetSendPacketCount() << std::endl;
		SetPacketCountCheckTimer();
	}

private:
	void	SetPacketCountCheckTimer()
	{
		pTimer->SetTimer(60 * 1000, std::bind(&Checker::OnTime, this));
	}

private:
	::NTimer::Timer * pTimer;
};

} /*ChatServer*/



#endif /* CHECKER_H_ */
