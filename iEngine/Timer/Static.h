/*
 * Static.h
 *
 *  Created on: 2016. 2. 19.
 *      Author: mega
 */

#ifndef TIMER_STATIC_H_
#define TIMER_STATIC_H_

#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../NetworkV2/NBase/Single.h"

namespace Timer {

class Static : public ::Network::NBase::Single<Static>
{
public:
	Static() : _work(_ioService)
	{
		_thread = std::thread(boost::bind(static_cast<size_t (boost::asio::io_service::*)()>(&boost::asio::io_service::run), &_ioService));
	}

	virtual ~Static()
	{
		_ioService.stop();
		_thread.join();
	};

	boost::asio::io_service &	GetIoService() { return _ioService; }

private:
	boost::asio::io_service			_ioService;
	boost::asio::io_service::work	_work;
	std::thread						_thread;
};

} /*Timer*/

#endif /* TIMER_STATIC_H_ */
