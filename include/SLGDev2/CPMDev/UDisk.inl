// UDisk.cpp: implementation of the DspDevManager class.
//
#include "UDisk.h"
#include "DspDevManager.h"
#include "CPMDB.h"
void UDiskRecvCallBack(void* cszPath, const char* pbBuf, unsigned int uLen, void* pvContext)
{
  DspDevManager* pNet = (DspDevManager*)pvContext;
  pNet->UDisk_RecvNotify((char*)cszPath, pbBuf, uLen);
}
BOOL DspDevManager::UDisk_UDiskCN(BOOL bFlag, const char* cszPath)
{
  assert(cszPath);

  if (bFlag) {
    if (CPM_DBOpen(true, cszPath)) {
      OnCNNotify(cszPath, DEV_CONNECT_SUCCESS);
    }
    else {
      OnCNNotify(cszPath, DEV_CONNECT_FAILUE);
    }
  }
  else {
    CPM_DBOpen(false, cszPath);
    OnCNNotify(cszPath, DEV_CONNECT_CUT);
  }

  return true;
}
BOOL DspDevManager::UDisk_Send(const char* cszPath, const char* pbBuf, unsigned int* puLen)
{
  OperDB(pbBuf, *puLen, cszPath);
  return true;
}
void DspDevManager::UDisk_RecvNotify(const char* cszPath, const char* pbBuf, unsigned int nLen)
{
  OnRecvNotify(cszPath, pbBuf, nLen);
}
void DspDevManager::UDisk_StartWork()
{
  CPM_DBSetOperCallBack(::UDiskRecvCallBack, this);
}

