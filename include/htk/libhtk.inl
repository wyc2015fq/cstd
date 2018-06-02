
#define ARCH "win32"
#define _HPUX_SOURCE
//#define HPUX_AUDIO
#define USE_TIMER

#ifndef _WINSOCKAPI_           // 没有包含winsock.h
#define _WINSOCKAPI_           // 避免再包含winsock.h
#ifndef _WINSOCK2API_      // 没有包含winsock2.h
#define _WINSOCK2API_      // 避免再包含winsock2.h
#include <winsock2.h>
//#pragma comment(lib, "ws32_2.lib")
#endif
#endif
#pragma comment(lib,"ws2_32.lib")
#include <windows.h>

#include "HTKLib\LGBase.inl"
#include "HTKLib\LCMap.inl"
#include "HTKLib\LModel.inl"
#include "HTKLib\LPCalc.inl"
#include "HTKLib\LPMerge.inl"
#include "HTKLib\LUtil.inl"
#include "HTKLib\LWMap.inl"

#include "HTKLib\esignal.inl"
#include "HTKLib\esig_asc.inl"
#include "HTKLib\esig_edr.inl"
#include "HTKLib\esig_nat.inl"

#include "HTKLib\HAdapt.inl"
#include "HTKLib\HArc.inl"
#include "HTKLib\HDict.inl"
#include "HTKLib\HExactMPE.inl"
#include "HTKLib\HFB.inl"
#include "HTKLib\HFBLat.inl"
//#include "HTKLib\HGraf.inl"
//#include "HTKLib\HGraf.null.inl"
#include "HTKLib\HGraf_WIN32.inl"
#include "HTKLib\HLabel.inl"
#include "HTKLib\HLat.inl"
#include "HTKLib\HLM.inl"
#include "HTKLib\HMap.inl"
#include "HTKLib\HMath.inl"
#include "HTKLib\HMem.inl"
#include "HTKLib\HModel.inl"
#include "HTKLib\HNet.inl"
#include "HTKLib\HParm.inl"
#include "HTKLib\HRec.inl"
#include "HTKLib\HShell.inl"
#include "HTKLib\HSigP.inl"
#include "HTKLib\HTrain.inl"
#include "HTKLib\HUtil.inl"
#include "HTKLib\HVQ.inl"
#include "HTKLib\HWave.inl"
#include "HTKLib\strarr.inl"
#include "HTKLib\HAudio.inl"
#if 0

#endif
