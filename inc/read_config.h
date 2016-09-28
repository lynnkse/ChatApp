#ifndef __READ_CONFIG_H__
#define __READ_CONFIG_H__

#include "../generic_queue/gen_queue.h"
#include <stdio.h>
#include "../generic_hash/HashMap.h"

typedef struct Config Config;

Config* ReadConfig(const char* _configFileName);
HashMap* GetNextConfig(Config* _configs);
void GetPath(Config* _configs, char** _path);
void DestroyConfigs(Config* _configs);

#endif
