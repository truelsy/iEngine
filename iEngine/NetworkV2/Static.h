/*
 * Static.h
 *
 *  Created on: 2014. 6. 17.
 *      Author: mega
 */

#ifndef NETWORKV2_STATIC_H_
#define NETWORKV2_STATIC_H_

#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>
#include <glog/logging.h>

#include "Session.h"
#include "Packet.h"
#include "NBase/Single.h"
#include "NBase/Pool.h"

#define CGCII_BMT

namespace Network {

class Static : public NBase::Single<Static>
{
public:
	typedef std::function<void(std::shared_ptr<Session>, unsigned short)> ONACCEPTEVENTFUNC;

public:
	Static(int threadCount, int sendPacketCount, int readPacketCount, int sessionCount);
	virtual ~Static();

	void		DoAccept();
	void		OnAccept(const boost::system::error_code & error);

	void		Listen(unsigned short port);

	void		SetAcceptEventFunc(const ONACCEPTEVENTFUNC & func) { _onAcceptEventFunc = func; }


	NBase::Pool<Packet>	*	GETPACKETPOOL_READ() { return _readPacketPool; }
	NBase::Pool<Packet>	*	GETPACKETPOOL_SEND() { return _sendPacketPool; }
	NBase::Pool<Session> *	GETSESSIONPOOL() { return _sessionPool; }

	boost::asio::io_service &	GETIOSERVICE() { return _ioService; }

private:
	boost::asio::io_service			_ioService;
	boost::asio::ip::tcp::acceptor	_acceptor;
	boost::asio::io_service::work	_work;
	boost::asio::ip::tcp::socket	_socket;

	int							_threadCount;

	NBase::Pool<Packet> *		_sendPacketPool;
	NBase::Pool<Packet> *		_readPacketPool;
	NBase::Pool<Session> *		_sessionPool;

	boost::thread_group			_threadGroup;

	ONACCEPTEVENTFUNC			_onAcceptEventFunc;
};

} /*Network*/


#endif /* NETWORKV2_STATIC_H_ */
