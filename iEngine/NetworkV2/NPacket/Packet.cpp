/*
 * Packet.cpp
 *
 *  Created on: 2014. 6. 26.
 *      Author: mega
 */

#include <string.h>

#include "../Packet.h"
#include "../Static.h"

namespace Network {

Packet::Packet()
{
	_buf.fill(0x00);
	_headerSize = 0;
	_bodySize = 0;
}

Packet::~Packet()
{

}

void
Packet::Initialize(bool bSendPacket)
{
	_buf.fill(0x00);
}

Packet::PACKETBUF &
Packet::GetBuf()
{
	return _buf;
}


#if 0
bool
Packet::Read1(char * d)
{
	*d = *(m_Buf.data() + m_nOffSet);
	m_nOffSet++;

	return true;
}

bool
Packet::Read2(short * d)
{
	*d = *((short *)(m_Buf.data() + m_nOffSet));
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
#endif

} /*BoostAsioNetwork*/
