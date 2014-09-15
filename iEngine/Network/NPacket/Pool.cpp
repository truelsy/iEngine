/*
 * Pool.cpp
 *
 *  Created on: 2014. 6. 27.
 *      Author: mega
 */

#include "../Packet.h"

namespace BoostAsioNetwork {

Packet::Pool::Pool()
{
	pthread_mutex_init(&m_SendLock, NULL);
	pthread_mutex_init(&m_ReadLock, NULL);
}

Packet::Pool::~Pool()
{
	for (Packet * pPacket : clsSendPacketPool)
		delete pPacket;
	clsSendPacketPool.clear();

	for (Packet * pPacket : clsReadPacketPool)
		delete pPacket;
	clsReadPacketPool.clear();
}

void
Packet::Pool::Init(int nSendPacketCount, int nReadPacketCount)
{
	Packet * pPacket = NULL;
	{
		pthread_mutex_lock(&m_SendLock);

		for (int i = 0; i < nSendPacketCount; i++)
		{
			pPacket = new Packet;
			clsSendPacketPool.push_back(pPacket);
		}

		pthread_mutex_unlock(&m_SendLock);
	}

	{
		pthread_mutex_lock(&m_ReadLock);

		for (int i = 0; i < nReadPacketCount; i++)
		{
			pPacket = new Packet;
			clsReadPacketPool.push_back(pPacket);
		}

		pthread_mutex_unlock(&m_ReadLock);
	}
}

Packet *
Packet::Pool::READ_NEW()
{
	Packet * pPacket = NULL;
	{
		pthread_mutex_lock(&m_ReadLock);

		if (true == clsReadPacketPool.empty())
		{
			pPacket = new Packet;
		}
		else
		{
			pPacket = clsReadPacketPool.front();
			clsReadPacketPool.pop_front();
		}

		pPacket->Initialize(false);

		pthread_mutex_unlock(&m_ReadLock);
	}

	return pPacket;
}

void
Packet::Pool::READ_DEL(Packet * pPacket)
{
	pthread_mutex_lock(&m_ReadLock);
	clsReadPacketPool.push_back(pPacket);
	pthread_mutex_unlock(&m_ReadLock);
}

Packet *
Packet::Pool::SEND_NEW()
{
	Packet * pPacket = NULL;
	{
		pthread_mutex_lock(&m_SendLock);

		if (true == clsSendPacketPool.empty())
		{
			pPacket = new Packet;
		}
		else
		{
			pPacket = clsSendPacketPool.front();
			clsSendPacketPool.pop_front();
		}

		pPacket->Initialize();

		pthread_mutex_unlock(&m_SendLock);
	}

	return pPacket;
}

void
Packet::Pool::SEND_DEL(Packet * pPacket)
{
	pthread_mutex_lock(&m_SendLock);
	clsSendPacketPool.push_back(pPacket);
	pthread_mutex_unlock(&m_SendLock);
}

} /*BoostAsioNetwork*/
