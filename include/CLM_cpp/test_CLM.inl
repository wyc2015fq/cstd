
#define C_DBG
#include "cstd.h"
#undef CC_SSE2
#define CC_SSE2 0
#define CC_SSE3 0
#define CC_SSE4_1 0
#if 0
#define test_cap main
#include "cap/test_cap.inl"
#else
#define ACCESS_MASK ACCESS_MASK1
#include "CLM_cpp/drawkonhon.inl"
#include "img/imgio.inl"
#include "libclm.inl"
#undef ACCESS_MASK
#endif

#if 0
#define test_MultiTrack main
#include "test_MultiTrack.inl"
#endif

#if 1
#ifdef _WIN32
#define test_CLM_check main
#include "test_MultiTrack.inl"
#include "test_CLM_check.inl"
#else
#define test_MultiTrack main
#include "test_MultiTrack.inl"
#endif
#endif

#if 0
#define MultiTrackCLM main
#include "MultiTrackCLM.inl"
#endif

#if 0
#define SimpleCLMImg main
#include "SimpleCLMImg.inl"
#endif


#if 0
#define test_cap_tcp_ser main
#include "cap/test_cap_tcp.inl"
#endif


#include "cap.h"
#include "net.h"
#include "lib.inl"
