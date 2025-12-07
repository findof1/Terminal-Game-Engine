#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef ENGINECORE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
// GCC / Clang on Linux/Mac
#define ENGINE_API __attribute__((visibility("default")))
#endif
