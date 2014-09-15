/*
 * Session.h
 *
 *  Created on: 2014. 6. 20.
 *      Author: mega
 */

#ifndef SESSION_H_
#define SESSION_H_

#include <list>
#include <deque>
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Packet.h"

namespace BoostAsioNetwork {

#define MAX_BUFFER_SIZE	(MAX_PACKET_LEN * 2)

class Session // : public boost::enable_shared_from_this<Session>
{
public:
	class Pool
	{
	public:
		Pool();
		virtual ~Pool();

		Session *	NEW();
		void		DEL(Session * pSession);
		void		POST(Session * pSession);

	private:
		std::list<Session *>	clsSessionList;
		pthread_mutex_t			m_Lock;
		pthread_cond_t  		m_Cond;
	};

public:
	Session();
	virtual ~Session();

public:
	boost::asio::ip::tcp::socket &	GetSocket() { return m_Socket; }

	void	Initialize();
	void	Finalize();

	void	HandleRead();
	void	HandleWrite(Packet * pPacket);

	void	Close();

	virtual void	OnPacket() = 0;

	void			PacketEnqueue(Packet * pPacket);
	Packet *		PacketDequeue();


private:
	int		GetHeaderSize() const;
	int		GetBodySize(char * pBuf) const;

	void	OnRead(const boost::system::error_code & error, size_t bytes_transferred);
	void	OnWrite(const boost::system::error_code & error, Packet * pPacket);

private:
	boost::asio::ip::tcp::socket	m_Socket;
	boost::asio::io_service::strand	m_Strand;

	std::array<char, MAX_PACKET_LEN * 2>	m_pBuf;

	int								m_nBufCursor;
	int								m_nRemainSize;

	std::deque<Packet *>			m_PacketQueue;
	pthread_mutex_t					m_QueueLock;
};

} /*BoostAsioNetwork*/


#endif /* SESSION_H_ */
