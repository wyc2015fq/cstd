#ifndef _LIBSOCK_INL_
#define _LIBSOCK_INL_
#ifdef WIN32
#pragma warning( disable : 4786 )
// Insert your headers here
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdint.h>
#endif
#include "utime.inl"
//#include "thread.h"
#include "sftime.inl"
#include "DebugAssistant.inl"
#include "MemPool.inl"
#include "Socket.inl"
#include "NetManager.inl"
#include "NetProcessor.inl"
#include "SelectProcessor.inl"
#include "OverlappedProcessor.inl"
#include "SockHandle.inl"
#include "ThreadPool.inl"
#endif // _LIBSOCK_INL_

