//#ifdef _DEBUG
#define _OUTPUT_INFO_
#define DebugAssistant DebugAssistant111
#define CMemPool CMemPool111
//#endif
#include "CPMDev.h"
#include "./sock/libsock.inl"
#ifdef _DEBUG
#ifndef DEBUG_NEW
#define DEBUG_NEW new
#endif // DEBUG_NEW
#endif
#if 1
#ifdef WIN32
//#define USE_CPMDB
#else
#define ZeroMemory(p, n) memset(p, 0, n)
#define Sleep usleep
#endif
#ifdef USE_CPMDB
#else
#include "./CPMDev/CPMDB.h"
#define CPM_DBOpen(flag, DB_AbsPath) (false)
#define CPM_DBSetOperCallBack(cb, param) (0)
#define OperDB(buf, len, DB_AbsPath) (0)
#endif
#include "./CPMDev/Base64Convert.inl"
#include "./CPMDev/common.inl"
#include "./CPMDev/CmdFactory.inl"
#include "./CPMDev/CPMDev.inl"
#include "./CPMDev/DevAuth.inl"
#include "./CPMDev/DevBaseInfo.inl"
#include "./CPMDev/DevCImage.inl"
#include "./CPMDev/DevCmd.inl"
#include "./CPMDev/DevCN.inl"
#include "./CPMDev/DevFormat.inl"
#include "./CPMDev/DevHeart.inl"
#include "./CPMDev/DevID.inl"
#include "./CPMDev/DevLog.inl"
#include "./CPMDev/DevOpenDoor.inl"
#include "./CPMDev/DevProUpdate.inl"
#include "./CPMDev/DevRecord.inl"
#include "./CPMDev/DevRgn.inl"
#include "./CPMDev/DevRight.inl"
#include "./CPMDev/DevSet.inl"
#include "./CPMDev/DevStatis.inl"
#include "./CPMDev/DevSysTime.inl"
#include "./CPMDev/DevTimeGroup.inl"
#include "./CPMDev/DevUser.inl"
#include "./CPMDev/DevUserGroup.inl"
#include "./CPMDev/DevVideo.inl"
#include "./CPMDev/DspDevManager.inl"
#include "./CPMDev/Net.inl"
#include "./CPMDev/video.inl"
#include "./CPMDev/yuv2bmp.inl"
#endif

