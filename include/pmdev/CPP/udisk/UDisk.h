// UDisk.h: interface for the CUDisk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDISK_H__0DD8D73F_CF3A_49AE_822C_7B122E58CEC7__INCLUDED_)
#define AFX_UDISK_H__0DD8D73F_CF3A_49AE_822C_7B122E58CEC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DspDevManager;

class CUDisk
{
public:
  CUDisk() : m_Manager(NULL) {}
  virtual ~CUDisk() {}
  int SetManager(DspDevManager* Manager) {
    m_Manager = (Manager);
    ASSERT(Manager);
    return 0;
  }
public:
  void StartWork();
  BOOL UDiskCN(BOOL bFlag, const char* cszPath);
  BOOL Send(const char* cszPath, const char* pbBuf, unsigned int* puLen);
public:
  void RecvNotify(const char* cszPath, const char* pbBuf, unsigned int nLen);
private:
  DspDevManager* m_Manager;
};

#endif // !defined(AFX_UDISK_H__0DD8D73F_CF3A_49AE_822C_7B122E58CEC7__INCLUDED_)
