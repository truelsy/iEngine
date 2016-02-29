/*
 * Macro.h
 *
 *  Created on: 2016. 2. 29.
 *      Author: mega
 */

#ifndef MACRO_H_
#define MACRO_H_

#include "stdarg.h"
#include "stdio.h"

enum ConsoleColor
{
	FG_NONE = 0,
	FG_DEFAULT = 39,
	FG_BLACK = 30,
	FG_RED = 31,
	FG_GREEN = 32,
	FG_YELLOW = 33,
	FG_BLUE = 34,
	FG_MAGENTA = 35,
	FG_CYAN = 36,
	FG_LIGHT_GRAY = 37,
	FG_DARK_GRAY = 90,
	FG_LIGHT_RED = 91,
	FG_LIGHT_GREEN = 92,
	FG_LIGHT_YELLOW = 93,
	FG_LIGHT_BLUE = 94,
	FG_LIGHT_MAGENTA = 95,
	FG_LIGHT_CYAN = 96,
	FG_WHITE = 97,
};


inline void SETBUF(char * buf, size_t bufSize, ConsoleColor eColor, const char *fmt, ...)
{
	va_list args;

	int colorLen = 0;

	if (FG_NONE != eColor)
	{
		colorLen = snprintf(buf, bufSize, "\033[%03dm", eColor);
	}

	va_start(args, fmt);

	int messageLen = vsnprintf(buf + colorLen, bufSize - colorLen, fmt, args);

	if (FG_NONE != eColor)
	{
		snprintf(buf + (messageLen + colorLen), bufSize - (messageLen + colorLen), "\033[0m");
	}

	va_end(args);
}

#define INFO(...) 									\
	{ 												\
		char buf[1024];								\
		SETBUF(buf, sizeof(buf), FG_NONE, __VA_ARGS__);		\
		LOG(INFO) << buf;						\
	}

#define INFOC(eColor, ...) 								\
	{ 												\
		char buf[1024];								\
		SETBUF(buf, sizeof(buf), eColor, __VA_ARGS__);		\
		LOG(INFO) << buf;						\
	}

#define WARN(...) 									\
	{ 												\
		char buf[1024];								\
		SETBUF(buf, sizeof(buf), FG_NONE, __VA_ARGS__);		\
		LOG(WARNING) << buf;						\
	}

#define ERROR(...) 								\
	{ 												\
		char buf[1024];								\
		SETBUF(buf, sizeof(buf), FG_RED, __VA_ARGS__);		\
		LOG(ERROR) << buf;							\
	}


#endif /* MACRO_H_ */
