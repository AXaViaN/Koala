#ifndef KOALA__UTILITY__PLATFORM
#define KOALA__UTILITY__PLATFORM

#if defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS 1
#else
#error Unknown operating system
#endif

#endif // KOALA__UTILITY__PLATFORM
