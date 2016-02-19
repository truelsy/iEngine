/*
 * Session.h
 *
 *  Created on: 2014. 6. 20.
 *      Author: mega
 */

#ifndef NETWORKV2_SESSION_H_
#define NETWORKV2_SESSION_H_

#include <list>
#include <deque>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Packet.h"

namespace Network {

class Session : public std::enable_shared_from_this<Session>
{
public:
	class Observer
	{
	public:
		enum class E_EVENT {
			OPEN,
			CLOSE,
			MESSAGE,
		};

	public:
		Observer() {}
		virtual ~Observer() {}

		//virtual void Initialize() { LOG(INFO) << __PRETTY_FUNCTION__ ; }
		//virtual void Finalize() { LOG(INFO) << __PRETTY_FUNCTION__ ; }

	public:
		virtual void OnOpen() {}
		virtual void OnClose() {}
		virtual void OnMessage() {}
		virtual void OnConnectFail(boost::system::error_code& ec) {}

		virtual int get_head_length() { return 4; }
		virtual int get_body_length(char * pBuffer) {
			short size = static_cast<short>(* pBuffer);
			return size;
		}
	};

public:
	Session();
	virtual ~Session();

public:
	typedef std::deque<std::shared_ptr<Packet> >	PACKETQUEUE;

public:
	void	Initialize();
	void	Finalize();

	void	Close();

	void	DoStart(boost::asio::ip::tcp::socket socket);
	void	DoReadHeader();
	void	DoReadBody();
	void	DoWrite();

	void	Send(std::shared_ptr<Packet> sendPacket);
	void	SetObserver(std::shared_ptr<Observer> observer) { _observer = observer; }

	PACKETQUEUE	& GetReadPacketQueue() { return _readPacketQueue; }
	std::shared_ptr<Session> CAPTURE() { return shared_from_this(); }

private:
	void	OnReadHeader(const boost::system::error_code & error, size_t bytes_transferred);
	void	OnReadBody(const boost::system::error_code & error, size_t bytes_transferred);
	void	OnWrite(const boost::system::error_code & error);

private:
	boost::asio::ip::tcp::socket	_socket;
	boost::asio::io_service::strand	_strand;

	std::shared_ptr<Packet>		_packet;
	std::shared_ptr<Observer>	_observer;

	std::mutex					_sendLock;
	PACKETQUEUE					_sendPacketQueue;
	PACKETQUEUE					_readPacketQueue;
};

} /*BoostAsioNetwork*/


#endif /* NETWORKV2_SESSION_H_ */
