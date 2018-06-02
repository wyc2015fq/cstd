// DevCN.cpp: implementation of the DevCN class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevCN.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//REG_OBJECT(C_CNDEV, CDevCN, false );

extern void CNNotify(const char* cszIP, E_CONNECT_STATUS eFlag, BOOL bServer, void* pvClientData);

CDevCN::CDevCN(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

void CDevCN::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  char cDstIP[16] = {0};
  LongToDotIp(cDstIP, pCMD->m_Param2);
  ::CNNotify(cDstIP, E_CONNECT_STATUS(pCMD->m_SCmd), TRUE, pvClientData);
}