#ifndef __ZLOG_H__
#define __ZLOG_H__

#include <sys/types.h>
#include <unistd.h>
#include "zlog.h"
/*
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "­Wvariadic­macros"*/

#define ZLOG_SEND(zlog, level, message,...) do \
										{  \
										if(GetZlogLevel(zlog) >= level) \
										{ \
										fprintf(GetFilePtr(zlog), "%s %s %d %u %s %s %d %s "#message "\n", __DATE__, __TIME__, getpid(), GetZlogLevel(zlog), GetModuleName(zlog), __FUNCTION__,  __LINE__, __FILE__, __VA_ARGS__); \
										fflush(GetFilePtr(zlog)); \
										} \
										} while(0);

typedef struct LogManager LogManager;

void ZlogInit(const char* _configFileName);
Zlog* ZlogGet(const char* _module);
void LogManagerDestroy();

#endif

