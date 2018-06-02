// UDisk.cpp: implementation of the CUDisk class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "UDisk.h"
#include "../DspDevManager.h"


#include "CPMDB.h"

BOOL   CPM_DBOpen(BOOL flag , const char* DB_AbsPath)
{
  return false;
}

void   CPM_DBSetOperCallBack(OperCallBack cb , void* param)
{
  return ;
}
void   OperDB(const char* buf, unsigned int len, const char* DB_AbsPath)
{
  return ;
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void UDiskRecvCallBack(void* cszPath, const char* pbBuf, unsigned int uLen, void* pvContext)
{
  CUDisk* pNet = (CUDisk*)pvContext;
  pNet->RecvNotify((char*)cszPath, pbBuf, uLen);
}

BOOL CUDisk::UDiskCN(BOOL bFlag, const char* cszPath)
{
  assert(cszPath);

  if (bFlag) {
    if (CPM_DBOpen(true, cszPath)) {
      m_Manager->OnCNNotify(cszPath, DEV_CONNECT_SUCCESS);
    }
    else {
      m_Manager->OnCNNotify(cszPath, DEV_CONNECT_FAILUE);
    }
  }
  else {
    CPM_DBOpen(false, cszPath);
    m_Manager->OnCNNotify(cszPath, DEV_CONNECT_CUT);
  }

  return true;

}

BOOL CUDisk::Send(const char* cszPath, const char* pbBuf, unsigned int* puLen)
{
  OperDB(pbBuf, *puLen, cszPath);
  return true;
}


void CUDisk::RecvNotify(const char* cszPath, const char* pbBuf, unsigned int nLen)
{
  m_Manager->OnRecvNotify(cszPath, pbBuf, nLen);
}

void CUDisk::StartWork()
{
  CPM_DBSetOperCallBack(::UDiskRecvCallBack, this);
}

