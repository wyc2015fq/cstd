// DevCommand.cpp: implementation of the DevCommand class.
//
#include "DevCmd.h"
extern void DevCMDNotify(const CSock* s,
    const char* cszClientAddr,
    DEV_OPER_TYPE eType,
    DEV_OPER_FLAG eFlag,
    void* pvContent,
    int nSerialNo,
    DEV_OPER_RESULT rt ,
    void* pvClientData
                        );
extern BOOL InternalSend(const CSock* addr, const char* pbBuf, unsigned int uLen, void* pvClientData);
void DevData::OnDevCmd(const CSock* s,
    const char* cszClientAddr,
    DEV_OPER_TYPE eType,
    DEV_OPER_FLAG eFlag,
    void* pvContent,
    int nSerialNo,
    int nResult,
    void* pvClientData
                      )
{
  //ASSERT(0);
  ::DevCMDNotify(s, cszClientAddr, eType, eFlag, pvContent, nSerialNo, (DEV_OPER_RESULT)nResult, pvClientData);
}
BOOL DevData::InternalSend(const CSock* s, const char* pbBuf, unsigned int uLen, void* pvClientData)
{
  return ::InternalSend(s, pbBuf, uLen, pvClientData);
}

