/*
 * TimerHandler.h
 *
 *  Created on: 2016. 2. 19.
 *      Author: mega
 */

#ifndef TIMER_TIMERHANDLER_H_
#define TIMER_TIMERHANDLER_H_

#include <memory>
#include <iostream>
#include <functional>
#include <thread>
#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <glog/logging.h>
#include "Static.h"

namespace Timer {

class TimerHandler
{
public:
	class Timer
	{
	public:
		Timer() : _deadlineTimer(Static::Instance().GetIoService()), _callback(NULL), _isSetTimer(false)
		{
			_millisecond = 0;
			_timerIndex = 0;
		}

		~Timer()
		{
		}

	public:
		typedef boost::function<void(const boost::system::error_code &, int)> TimerCallbackFunc;

	public:
		boost::asio::deadline_timer _deadlineTimer;
		TimerCallbackFunc			_callback;
		bool						_isSetTimer;
		int							_millisecond;
		int							_timerIndex;
	};

public:
	TimerHandler();
	virtual ~TimerHandler();

	void SetTimer(int milliseconds, int id, Timer::TimerCallbackFunc func);
	void KillTimer();
	void ResumeTimer();

private:
	std::unique_ptr<Timer> _timer;
};

} /* namespace Timer */

#endif /* TIMER_TIMERHANDLER_H_ */
