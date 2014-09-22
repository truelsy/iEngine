/*
 * Timer.h
 *
 *  Created on: 2014. 9. 22.
 *      Author: mega
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <thread>


namespace NTimer
{

class Timer
{
public:
	Timer();
	virtual ~Timer();

	template <class FUNCTOR>
	void	SetTimer(int interval, FUNCTOR func)
	{
	    std::thread([interval, func]() {
	            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
	            func();
	    }).detach();
	}
};

} /* namespace NTimer */

#endif /* TIMER_H_ */
