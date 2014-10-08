/*
 * ResultSet.h
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#ifndef MEMCACHE_RESULTSET_H_
#define MEMCACHE_RESULTSET_H_

#include <stddef.h>
#include <string>

#define MAX_RESULTSET				1048576 //1Mbyte
#define MAX_RESULTSET_FIELDSIZE	128

namespace Memcache {

class ResultSet
{
public:
	ResultSet();
	virtual ~ResultSet();

	bool	Init();
	bool	Init(short row, short field);
	bool	Init(const char *buff, int len);
	bool	Add(const char *item);
	bool	MoreRecord();

	template <typename T>
	inline ResultSet & operator<<(T d);

	inline ResultSet & operator<<(const char * d);

private:
	bool	AddShort(short *value);
	bool	AddString(const char *item);
	bool	Allocate();

public:
	char	m_Data[MAX_RESULTSET];
	char	**ROW;
	int		m_Offset;
	short	m_Fields;
	short	m_Rows;
	char	*m_pVal;

private:
	char	*m_pBuff;
	short	m_CurrentField;
	short	m_CurrentRow;
	bool	m_bIsFirstMoreRecord;
};

inline ResultSet & ResultSet::operator <<(const char * d)
{
	Add(d);
	return *this;
}

template <typename T>
inline ResultSet & ResultSet::operator <<(T d)
{
	Add(std::to_string(d).c_str());
	return *this;
}

} /*Memcache*/



#endif /* MEMCACHE_RESULTSET_H_ */
