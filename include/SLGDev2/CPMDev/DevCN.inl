// DevCN.cpp: implementation of the DevCN class.
//
#include "DevCN.h"
//REG_OBJECT(C_CNDEV, CDevCN, false );
extern void CNNotify(const CSock* s, E_CONNECT_STATUS eFlag, BOOL bServer, void* pvClientData);
CDevCN::CDevCN(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(s, pCMD, bServer, pvClientData);
}
void CDevCN::ProcessNCMD(const CSock* s, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ::CNNotify(s, E_CONNECT_STATUS(pCMD->m_SCmd), TRUE, pvClientData);
}

