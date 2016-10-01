#ifndef __READ_CONFIG_H__
#define __READ_CONFIG_H__

#include "../inc/gen_queue.h"
#include <stdio.h>
#include "../inc/HashMap.h"

typedef struct Config Config;

Config* ReadConfig(const char* _configFileName);
HashMap* GetNextConfig(Config* _configs);
void GetPath(Config* _configs, char** _path);
void DestroyConfigs(Config* _configs);

#endif
