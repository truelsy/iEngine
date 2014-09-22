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
		SetUserCountTimer();
	}

	~Checker()
	{
		delete pTimer;
	}

public:
	void	OnTime()
	{
		auto & clsUserList = Static::Instance().GetUserList();
		std::cout << "User Count : " << clsUserList.size() << std::endl;

		SetUserCountTimer();
	}

private:
	void	SetUserCountTimer()
	{
		pTimer->SetTimer(1 * 1000, std::bind(&Checker::OnTime, this));
	}

private:
	::NTimer::Timer * pTimer;
};

} /*ChatServer*/



#endif /* CHECKER_H_ */
