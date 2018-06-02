
#ifndef __TRANS_H__
#define __TRANS_H__

#ifdef TRANS_EXPORTS
#define TRANS_API  __declspec(dllexport)
#else
#define TRANS_API  __declspec(dllimport)
#endif
#define TRANS_CALL __stdcall

#ifdef __cplusplus
extern "C"
{
#endif

#include "CPMDev.h"

#define CN_TYPE_DEVICE 0
#define CN_TYPE_CLIENT 1
#define CN_TYPE_CLIDEV 2

  typedef BOOL (TRANS_CALL* LinkCb)(const char* ip, unsigned short port, int type, DEV_CONNECT_STATUS state, void* pvContext);

  TRANS_API void TRANS_CALL CPM_RegLinkCb(LinkCb pfn, void* pvContext);
  TRANS_API void TRANS_CALL CPM_RegDataCb(DevOperResultNotify pfn, void* pvContext);
  TRANS_API BOOL TRANS_CALL CPM_StartTrans(BOOL start);
  TRANS_API BOOL TRANS_CALL CPM_SearchDev(BOOL start, const char* cszRgnAddr = DEV_REGION_ADDR);
  TRANS_API BOOL TRANS_CALL CPM_CnDev(const char* ip);
  TRANS_API BOOL TRANS_CALL CPM_DnDev(const char* ip);
  TRANS_API BOOL TRANS_CALL CPM_Verify(const char* ip, const char* name, const char* psw);
  TRANS_API long TRANS_CALL CPM_GetPeer(const char* ip, unsigned short port, PEERADR arry[], int size);
  TRANS_API BOOL TRANS_CALL CPM_UpUser(const char* cszDevAddr, const DEV_USER& cUser, const DEV_USER_REAL_LOAD* cRtLoad);
  TRANS_API BOOL TRANS_CALL CPM_SetTime(const char* cszDevAddr, const DEV_DATETIME& cSysTime);

#ifdef __cplusplus
}
#endif

#endif