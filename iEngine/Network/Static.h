/*
 * Static.h
 *
 *  Created on: 2014. 6. 17.
 *      Author: mega
 */

#ifndef BOOSTASIONETWORK_STATIC_H_
#define BOOSTASIONETWORK_STATIC_H_

#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/system/error_code.hpp>

#include "Session.h"
#include "Packet.h"
#include "IListener.h"
#include "NBase/Single.h"

namespace BoostAsioNetwork {

class Static : public NBase::Single<Static>
{
public:
	Static(int n_ThreadCount, int n_SendPacketCount, int n_ReadPacketCount);
	virtual ~Static();

	void		Accept();
	void		Start();

	void		OnSessionOpen(Session * pSession, const boost::system::error_code & error);

	Packet::Pool *	GETPACKETPOOL() { return m_pPacketPool; }
	Session::Pool *	GETSESSIONPOOL() { return m_pSessionPool; }

	boost::asio::io_service &	GETIOSERVICE() { return m_ioService; }

	void		SetListener(IListener * pListener) { m_pListener = pListener; }
	IListener *	GetListener() { return m_pListener; }

private:
	boost::asio::io_service			m_ioService;
	boost::asio::ip::tcp::acceptor	m_Acceptor;
	boost::asio::io_service::work	m_Work;

	int								m_nThreadCount;
	int								m_nSendPacketCount;
	int								m_nReadPacketCount;

	Packet::Pool *					m_pPacketPool;
	Session::Pool *					m_pSessionPool;

	boost::thread_group				m_ThreadGroup;

	IListener *						m_pListener;
};

} /*BoostAsioNetwork*/


#endif /* BOOSTASIONETWORK_STATIC_H_ */
