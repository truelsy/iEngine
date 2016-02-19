/*
 * TimerHandler.cpp
 *
 *  Created on: 2016. 2. 19.
 *      Author: mega
 */

#include "TimerHandler.h"

namespace Timer {

TimerHandler::TimerHandler() : _timer(new Timer)
{
	_timer->_callback = NULL;
	_timer->_isSetTimer = false;
}

TimerHandler::~TimerHandler()
{
	if (_timer->_isSetTimer)
	{
		this->KillTimer();
	}
}

void
TimerHandler::SetTimer(int milliseconds, int id, Timer::TimerCallbackFunc callback)
{
	_timer->_callback = callback;
	_timer->_millisecond = milliseconds;
	_timer->_timerIndex = id;
	_timer->_isSetTimer = true;

	ResumeTimer();
}

void
TimerHandler::KillTimer()
{
	_timer->_isSetTimer = false;
	_timer->_deadlineTimer.cancel();
}

void
TimerHandler::ResumeTimer()
{
	if (_timer->_isSetTimer)
	{
		_timer->_deadlineTimer.expires_from_now(boost::posix_time::milliseconds(_timer->_millisecond));
		_timer->_deadlineTimer.async_wait(boost::bind(_timer->_callback, boost::asio::placeholders::error, _timer->_timerIndex));
	}
}

} /* namespace Timer */
