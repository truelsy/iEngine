/*
 * Packet.cpp
 *
 *  Created on: 2014. 6. 26.
 *      Author: mega
 */

#include <string.h>

#include "../Packet.h"

namespace BoostAsioNetwork {

Packet::Packet()
	: m_nOffSet(0),
	  m_PacketType(PacketType::NONE)
{
	m_Buf.fill(0x00);
}

Packet::~Packet()
{

}

void
Packet::Initialize(bool bSendPacket)
{
	m_nOffSet = 0;
	m_Buf.fill(0x00);
	m_PacketType = PacketType::NONE;

	if (true == bSendPacket)
		m_nOffSet = 6;	// HEADER SIZE
}

void
Packet::MakePacket(uint16_t command)
{
	m_Buf[0] = 0xff;
	memcpy(&m_Buf[1], &m_nOffSet, 2);
	m_Buf[3] = 0x01;
	memcpy(&m_Buf[4], &command, 2);
}

Packet::PACKETBUF &
Packet::GetBuf()
{
	return m_Buf;
}

bool
Packet::Read1(char * d)
{
	*d = m_Buf[m_nOffSet];
	m_nOffSet++;

	return true;
}

bool
Packet::Read2(short * d)
{
	*d = (short)m_Buf[m_nOffSet];
	m_nOffSet += 2;

	return true;
}

bool
Packet::Write1(char d)
{
	m_Buf[m_nOffSet] = d;
	m_nOffSet++;

	return true;
}

bool
Packet::Write2(short d)
{
	memcpy(&m_Buf[m_nOffSet], &d, 2);
	m_nOffSet += 2;

	return true;
}

} /*BoostAsioNetwork*/
