
#ifndef _SYS_INL_
#define _SYS_INL_

#ifdef _NET_H_
#include "net/net.inl"
#endif

#ifdef _CSTD_H_
#include "sys/sys.inl"
#endif

#ifdef USESYS
#endif

#ifdef _CAP_H_
#include "cap/cap.inl"
#endif

#ifdef _CPU_H_
#include "sys/cpu.inl"
#endif


#ifdef __CURL_CURL_H
//#include "lib/curl/curl.inl"
#endif

#ifdef PTHREAD_H
#include "lib\pthread-win32-master\pthread.c"
#endif

#endif // _SYS_INL_
