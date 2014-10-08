/*
 * Command.h
 *
 *  Created on: 2014. 10. 6.
 *      Author: mega
 */

#ifndef MEMCACH_COMMAND_H_
#define MEMCACH_COMMAND_H_

#include <list>
#include <string>
#include <stddef.h>
#include <libmemcached/memcached.h>
#include "ResultSet.h"

namespace Memcache {

class Command
{
public:
	class Pool
	{
	public:
		Pool() {}
		~Pool()
		{
			auto iter = clsCommandPool.begin();
			for(; iter != clsCommandPool.end(); iter++)
			{
				delete (*iter);
			}
			clsCommandPool.clear();
		}

		void	Init(int nPoolSize = 1024)
		{
			for (int i = 0; i < nPoolSize; i++)
			{
				Command * pCommand = new Command;
				if (NULL != pCommand)
					clsCommandPool.push_back(pCommand);
			}
		}

		Command *	NEW()
		{
			Command * pCommand = NULL;
			auto iter = clsCommandPool.begin();

			if (iter != clsCommandPool.end())
			{
				pCommand = *iter;
				clsCommandPool.pop_front();
			}
			else
			{
				pCommand = new Command;
			}

			pCommand->Initialize();
			return pCommand;
		}

		void		DEL(Command * pCommand)
		{
			pCommand->Finalize();
			clsCommandPool.push_back(pCommand);
		}

	private:
		std::list<Command *>	clsCommandPool;
	};

public:
	class Auto
	{
	public:
		Auto(Command::Pool * pPool) :
			pCommand(NULL), pCommandPool(pPool)
		{
			if (NULL != pPool)
				pCommand = pCommandPool->NEW();
		}

		~Auto()
		{
			if (NULL != pCommandPool)
				pCommandPool->DEL(pCommand);
		}

		operator Command * ()
		{
			return pCommand;
		}

		Command * operator -> ()
		{
			return pCommand;
		}

		Command * GetCommand()
		{
			return pCommand;
		}

	private:
		Command * pCommand;
		Command::Pool * pCommandPool;
	};


public:
	enum Action {
		NONE,
		SET,
		GET
	};

	struct Data
	{
		std::string _key;
		char *	_value;
		size_t	_valueLen;
		Action	_eAction;
		time_t	_expire;

		Data()
		{
			_key.clear();
			_value		= NULL;
			_valueLen	= 0;
			_eAction	= NONE;
			_expire		= 0;
		}

		void	Clear()
		{
			_key.clear();
			_value		= NULL;
			_valueLen	= 0;
			_eAction	= NONE;
			_expire		= 0;
		}
	};

public:
	Command();
	virtual ~Command();

	void	Initialize();
	void	Finalize();

	void	SetData(Data & data) { clsData = data; }
	Data &	GetData() { return clsData; }

	void				SetReturnCode(memcached_return_t rc) { m_rc = rc; }
	memcached_return_t	GetReturnCode() { return m_rc; }

	void		SetErrorString(const char * error) { m_errorString = error; }
	const char *	GetErrorString() { return m_errorString.c_str(); }

	ResultSet &	GetResultSet() { return clsResultSet; }

private:
	Data	clsData;
	ResultSet	clsResultSet;
	memcached_return_t	m_rc;
	std::string			m_errorString;
};

} /*Memcached*/


#endif /* MEMCACH_COMMAND_H_ */
