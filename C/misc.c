
#include <stdio.h>
#include "headers.h"
/*
#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #include <time.h>
#endif
*/
#include <stdint.h> // portable: uint64_t   MSVC: __int64 
//#include <Windows.h>
//#include "sysinfoapi.h"
//#include <time.h>
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*
int GetTimeMs() {
//#ifdef WIN32
  return GetTickCount();
//#elif __linux__
  //struct timeval t;
  //gettimeofday(&t, NULL);
  //return t.tm_sec*1000 + t.tm_sec/1000000000;
//#endif
}
*/


// MSVC defines this in winsock2.h!?
typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

int GetTimeMs() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec*1000 + t.tv_usec/1000;
}
