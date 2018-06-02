// DevCommand.cpp: implementation of the DevCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevCmd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void DevCMDNotify(const char* cszDevAddr,
    const char* cszClientAddr,
    DEV_OPER_TYPE eType,
    DEV_OPER_FLAG eFlag,
    void* pvContent,
    int nSerialNo,
    DEV_OPER_RESULT rt ,
    void* pvClientData
                        );

extern BOOL InternalSend(const char* cszIP, const char* pbBuf, unsigned int uLen, void* pvClientData);

void DevData::OnDevCmd(const char* cszDevAddr,
    const char* cszClientAddr,
    DEV_OPER_TYPE eType,
    DEV_OPER_FLAG eFlag,
    void* pvContent,
    int nSerialNo,
    int nResult,
    void* pvClientData
                      )
{
  ::DevCMDNotify(cszDevAddr, cszClientAddr, eType, eFlag, pvContent, nSerialNo, (DEV_OPER_RESULT)nResult, pvClientData);
}

BOOL DevData::InternalSend(const char* cszIP, const char* pbBuf, unsigned int uLen, void* pvClientData)
{
  return ::InternalSend(cszIP, pbBuf, uLen, pvClientData);
}