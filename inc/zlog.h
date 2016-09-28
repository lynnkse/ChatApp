#ifndef __PRIVATE_LOG_H__
#define __PRIVATE_LOG_H__

#include "zlog.h"
#include "stdio.h"
#include "../generic_hash/HashMap.h" 

typedef struct Zlog Zlog;

typedef enum Level {LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_CRITICAL, LOG_SEVERE, LOG_FATAL, LOG_NONE} Level;

Zlog* ZlogCreate(HashMap* _config);

void ZlogDestroy(Zlog* _zlog);
char* GetModuleName(Zlog* _zlog);
FILE* GetFilePtr(Zlog* _zlog);
Level GetZlogLevel(Zlog* _zlog);


#endif
