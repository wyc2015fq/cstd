

///////////////////////////////// Includes //////////////////////////////////
#include <windows.h>
#include <rpc.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
//#include "debug.h"


#pragma comment (lib,"RPCRT4.lib")

#ifdef _DEBUG
#define TRACE printf
#else
void AfxTrace(LPCTSTR lpszFormat, ...) {}
#define TRACE AfxTrace
#endif

//////////////////////////////// implementation /////////////////////////////
//Class which handles UuidCreateSequential call, which
//must be constructed at run time since it is not implemented on NT 4 or Windows 9x,
//
//Note that I could have used VC 6's support for delay loading but this would
//mean that the code would not work on VC 5 which I aim to provide support
//for in my code.

//typedefs of the function pointers
typedef RPC_STATUS (WINAPI UUIDCREATESEQUENTIAL)(UUID*);
typedef UUIDCREATESEQUENTIAL* LPUUIDCREATESEQUENTIAL;

//Member variables
HINSTANCE              m_hRcpt4;  //Instance handle of the "RCPT4.DLL" which houses the UuidCreateSequential
LPUUIDCREATESEQUENTIAL m_lpfnUuidCreateSequential;

static int _UUIDS_init() {
  m_hRcpt4 = GetModuleHandle(_T("RPCRT4.DLL"));
  assert(m_hRcpt4 != NULL);
  m_lpfnUuidCreateSequential = (LPUUIDCREATESEQUENTIAL) GetProcAddress(m_hRcpt4, "UuidCreateSequential");
}

BOOL GetNicAddress(LPTSTR pszNicAddress, UINT nBufSize)
{
  UUID Uuid;
  RPC_STATUS rpcStatus;
  BOOL bSuccess = FALSE;

  //NIC address is 12 character string
  if (nBufSize < 13)
    return FALSE;

  //the way we determine the NIC address is to call the RPC DCE function
  //UuidCreate. The standard format of the GUID returned contains
  //the NIC address in the last 12 characters. The added advantage to
  //this method is that we do not need to rely on a specific network
  //protocol needing to be installed on the client machine to determine
  //the NIC address. You could use this function as the basis for a
  //security scheme for a networked product of yours. Using the NIC address
  //is a guranteed way of uniquely identify a machine throughout the network.
  //
  //One thing to note is that we call UuidCreateSequential if it is available.
  //This is due to a change in the way that UuidCreate works on Windows 2000

  if (m_lpfnUuidCreateSequential)
    rpcStatus = m_lpfnUuidCreateSequential(&Uuid);
  else
    rpcStatus = UuidCreate(&Uuid);

  if (rpcStatus == RPC_S_OK) {
    #ifdef _UNICODE
    unsigned short* pszGuid;
    #else
    unsigned char* pszGuid;
    #endif
    rpcStatus = UuidToString(&Uuid, &pszGuid);
    if (rpcStatus == RPC_S_OK)
    {
      TCHAR* pLastMinus = _tcsrchr((TCHAR*)pszGuid, _T('-'));
      if (pLastMinus)
      {
        _tcscpy(pszNicAddress, pLastMinus+1);
        bSuccess = TRUE;
      }

      //need to pfree created buffer
      RpcStringFree(&pszGuid);
    }
    else
      TRACE(_T("Error calling UuidToString, Error value is %d\n"), rpcStatus);
  }
  else
    TRACE(_T("Error calling UuidCreate, Error value is %d\n"), rpcStatus);

  return bSuccess;
}

BOOL GetCDriveSerialNumber(unsigned long* pdwSerialNumber) {
  //Again as with the NIC address, the serial number of the C drive
  //could be used as the basis of a security scheme for a networked
  //product of yours
  BOOL bSuccess = (GetVolumeInformation(_T("C:\\"), NULL, 0, pdwSerialNumber,
                                        NULL, NULL, NULL, 0) != 0);
  return bSuccess;
}

BOOL GetCPUNum(char* CpuID, int nBufSize)
{
  //»ñÈ¡CPUÐòÁÐºÅ
  unsigned long s1,s2;
  char sel;
  if (nBufSize < 13)
    return FALSE;
  sel='1';
  __asm{
    mov eax,01h;
    xor edx,edx;
    cpuid;
    mov s1,edx;
    mov s2,eax;
  }
  _snprintf(CpuID, nBufSize, "%08X%08X",s1,s2);
#if 0
  int len;
  char* p = CpuID;
  __asm{
    mov eax,03h;
    xor ecx,ecx;
    xor edx,edx;
    cpuid;
    mov s1,edx;
    mov s2,ecx;
  }
  p=CpuID+strlen(CpuID);
  len=nBufSize-strlen(CpuID);
  _snprintf(p, len, "%08X%08X",s1,s2);
#endif
  return TRUE;//CpuID.Mid(5,3);
}
