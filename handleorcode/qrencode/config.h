#pragma once


#define MAJOR_VERSION 4
#define MINOR_VERSION 4
#define MICRO_VERSION 1
#define VERSION "4.1.1"

#define STATIC_IN_RELEASE static

#if defined(_WIN32) || defined(_WIN64)
//#define snprintf _snprintf
//#define vsnprintf _vsnprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strdup _strdup
#define _CRT_SECURE_NO_WARNINGS
#define HAVE_STRDUP 1
#endif
