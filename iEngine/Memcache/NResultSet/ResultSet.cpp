/*
 * ResultSet.cpp
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#include "../ResultSet.h"
#include <string.h>
#include <stdlib.h>

namespace Memcache {

ResultSet::ResultSet()
: ROW(0), m_Offset(0), m_Fields(0), m_Rows(0), m_pVal(0), m_pBuff(0), m_CurrentField(0), m_CurrentRow(0), m_bIsFirstMoreRecord(true)
{
	memset( m_Data, 0x00, sizeof(m_Data) );
}

ResultSet::~ResultSet()
{
	if( m_pVal != NULL )
		free(m_pVal);
}

bool ResultSet::Init()
{
	memset(m_Data, 0x00, sizeof(m_Data));

	ROW			= 0;
	m_Offset	= 0;
	m_Fields	= 0;
	m_Rows		= 0;
	m_pVal		= 0;
	m_pBuff		= 0;
	m_CurrentField	= 0;
	m_CurrentRow	= 0;
	m_bIsFirstMoreRecord	= true;

	return true;
}

bool ResultSet::Init(short row, short field)
{
	memset( m_Data, 0x00, sizeof(m_Data) );

	m_Fields = field;
	m_Rows = row;
	m_CurrentField = 0;
	m_CurrentRow = 0;

	m_pBuff = m_Data;
	m_Offset = 0;
	m_bIsFirstMoreRecord = true;

	AddShort(&m_Rows);
	AddShort(&m_Fields);

	return true;
}

bool ResultSet::Init(const char *buff, int len)
{
	memcpy( m_Data, buff, len );
	memcpy( &m_Rows, buff, 2 );
	memcpy( &m_Fields, buff+2, 2 );

	m_Offset = 4;
	m_CurrentField = 0;
	m_CurrentRow = 0;
	m_pBuff = m_Data;
	m_bIsFirstMoreRecord = true;

	return false;
}

bool ResultSet::Add(const char *item)
{
	AddString( item );

	return true;
}

bool ResultSet::MoreRecord()
{
	if( m_bIsFirstMoreRecord == true )
	{
		Allocate();

		m_Offset = 4;
		m_CurrentField = 0;
		m_CurrentRow = 0;
		m_bIsFirstMoreRecord = false;
	}

	int i;

	if( m_CurrentRow >= m_Rows )
	{
		for( i=0 ; i<m_Fields ; i++ )
		{
			free(ROW[i]);
		}

		free(ROW);
		ROW = NULL;

		return false;
	}

	for( i=0 ; i<m_Fields ; i++ )
	{
		memset( ROW[i], 0x00, MAX_RESULTSET_FIELDSIZE );

		short len;
		memcpy( &len, m_pBuff + m_Offset, 2 );
		m_Offset += 2;
		memcpy( ROW[i], m_pBuff + m_Offset, len );
		m_Offset += len;
	}

	m_CurrentRow++;

	return true;
}

bool ResultSet::AddShort(short *value)
{
	memcpy( m_pBuff + m_Offset, value, 2 );
	m_Offset += 2;

	return true;
}

bool ResultSet::AddString(const char *item)
{
	short len = strlen(item);
	AddShort( &len );

	memcpy( m_pBuff + m_Offset, item, len );
	m_Offset += len;

	return true;
}

bool ResultSet::Allocate()
{
	ROW = (char **)malloc(sizeof(char *) * m_Fields);

	for( int i=0 ; i<m_Fields ; i++ )
	{
		ROW[i] = (char *)malloc(MAX_RESULTSET_FIELDSIZE);
	}

	return true;
}

} /*Memcache*/


