/*
 * Packet.h
 *
 *  Created on: 2014. 6. 26.
 *      Author: mega
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <array>
#include <list>
#include <pthread.h>

namespace BoostAsioNetwork {

#define	MAX_PACKET_LEN	(1024 * 4)

class Packet
{
public:
	class Pool
	{
	public:
		Pool();
		virtual ~Pool();

		void		Init();

	public:
		Packet *	READ_NEW();
		void		READ_DEL(Packet * pPacket);

		Packet *	SEND_NEW();
		void		SEND_DEL(Packet * pPacket);

	private:
		std::list<Packet *>		clsSendPacketPool;
		std::list<Packet *>		clsReadPacketPool;

		pthread_mutex_t			m_SendLock;
		pthread_mutex_t			m_ReadLock;
	};

public:
	enum PacketType
	{
		NONE	= 0,
		OPEN,
		CLOSE,
	};

public:
	Packet();
	virtual ~Packet();

public:
	typedef std::array<char, MAX_PACKET_LEN>		PACKETBUF;

	PACKETBUF &	GetBuf();

	void		Initialize(bool bSendPacket=true);

	void		SetPacketType(PacketType type) { m_PacketType = type; }
	PacketType	GetPacketType() { return m_PacketType; }

	void		MakePacket(uint16_t command);
	int			GetOffSet() { return m_nOffSet; }

	/* Packet Parse Operation */
	bool	Read1(char * d);
	bool	Read2(short * d);

	bool	Write1(char d);
	bool	Write2(short d);

	inline Packet & operator>>(char & d);
	inline Packet & operator>>(short & d);

	inline Packet &	 operator<<(char d);
	inline Packet &	 operator<<(short d);

private:
	int			m_nOffSet;
	PACKETBUF	m_Buf;

	PacketType	m_PacketType;
};

inline Packet & Packet::operator >>(char & d)
{
	Read1(&d);
	return *this;
}

inline Packet & Packet::operator >>(short & d)
{
	Read2(&d);
	return *this;
}

inline Packet & Packet::operator <<(char d)
{
	Write1(d);
	return *this;
}

inline Packet & Packet::operator <<(short d)
{
	Write2(d);
	return *this;
}

} /*BoostAsioNetwork*/

#endif /* PACKET_H_ */
