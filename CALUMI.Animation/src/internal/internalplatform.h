#pragma once

#ifdef _WIN32
	#include <cstring>
	#define SCOMPARE _stricmp
#else
	#include <strings.h>
    #define SCOMPARE strcasecmp
#endif 

#if __has_include(<format>)
	#include <format>
	namespace fmt_lib = std;
#else
	#include <fmt/core.h>
	namespace fmt_lib = fmt;
#endif