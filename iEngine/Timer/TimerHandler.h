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
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <glog/logging.h>

namespace Timer {

class TimerHandler
{
public:
	class Timer
	{
	public:
		Timer() : _work(_ioService), _deadlineTimer(_ioService), _callback(NULL), _isSetTimer(false)
		{
			_millisecond = 0;
			_timerIndex = 0;

			Run();
		}

		~Timer()
		{
			_ioService.stop();
			_thread.join();
		}

		void Run()
		{
			_thread = std::thread(boost::bind(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &_ioService));
		}

	public:
		typedef boost::function<void(const boost::system::error_code &, int)> TimerCallbackFunc;

	public:
		boost::asio::io_service			_ioService;
		boost::asio::io_service::work	_work;
		boost::asio::deadline_timer 	_deadlineTimer;

		TimerCallbackFunc	_callback;
		std::thread			_thread;
		bool				_isSetTimer;
		int					_millisecond;
		int					_timerIndex;
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
