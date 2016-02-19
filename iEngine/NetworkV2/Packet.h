/*
 * Packet.h
 *
 *  Created on: 2014. 6. 26.
 *      Author: mega
 */

#ifndef NETWORKV2_PACKET_H_
#define NETWORKV2_PACKET_H_

#include <array>
#include <boost/enable_shared_from_this.hpp>

#define	MAX_BUFFER_SIZE	((1024 * 4) * 10)

namespace Network {

class Packet : public std::enable_shared_from_this<Packet>
{
public:
	Packet();
	virtual ~Packet();

public:
	typedef std::array<char, MAX_BUFFER_SIZE>		PACKETBUF;

	PACKETBUF &	GetBuf();

	void		Initialize(bool isSendPacket=true);
	void		Finalize() {};

	void		SetHeaderSize(int headerSize) { _headerSize = headerSize; }
	int			GetHeaderSize() { return _headerSize; }

	void		SetBodySize(int bodySize) { _bodySize = bodySize; }
	int			GetBodySize() { return _bodySize; }

	int			GetSize() { return _headerSize + _bodySize; }

	std::shared_ptr<Packet> CAPTURE() { return shared_from_this(); }

#if 0
	/* Packet Parse Operation */
	bool	Read1(char * d);
	bool	Read2(short * d);

	bool	Write1(char d);
	bool	Write2(short d);

	inline Packet & operator>>(char & d);
	inline Packet & operator>>(unsigned char & d);
	inline Packet & operator>>(short & d);

	inline Packet &	 operator<<(char d);
	inline Packet &	 operator<<(short d);
#endif

private:
	PACKETBUF	_buf;
	int			_headerSize;
	int			_bodySize;
};

#if 0
inline Packet & Packet::operator >>(char & d)
{
	Read1(&d);
	return *this;
}

inline Packet & Packet::operator >>(unsigned char & d)
{
	Read1((char *)&d);
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
#endif

} /*BoostAsioNetwork*/

#endif /* NETWORKV2_PACKET_H_ */
