#pragma once

#include "obj-cache.h"

ObjCache *
redis_cache_new (const char *host, const char *passwd,
                 int port, int mc_expiry,
                 int max_connections);
