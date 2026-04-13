#pragma once

#include <stdbool.h>
#ifdef HAVE_MYSQL
#include <mysql.h>
#endif

#if defined(MYSQL_VERSION_ID) && (MYSQL_VERSION_ID >= 80000)
typedef bool my_bool;
#else
typedef char my_bool;
#endif
