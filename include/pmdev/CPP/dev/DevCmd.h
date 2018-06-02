// DevCommand.h: interface for the DevCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVCOMMAND_H__F617831D_295D_483C_8290_8511AEB46DB4__INCLUDED_)
#define AFX_DEVCOMMAND_H__F617831D_295D_483C_8290_8511AEB46DB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdFactory.h"

class DevData
{
public:
  DevData() {}
  virtual ~DevData() {}
protected:
  void OnDevCmd(const char* cszDevAddr,
      const char* cszClientAddr,
      DEV_OPER_TYPE eType,
      DEV_OPER_FLAG eFlag,
      void* pvContent,
      int nSerialNo,
      int nResult,
      void* pvClientData
               );
  BOOL InternalSend(const char* cszIP, const char* pbBuf, unsigned int uLen, void* pvClientData);
};

class SuperCmd
{
public:
  SuperCmd(BOOL bmax) : m_bMax(bmax) {}
  virtual ~SuperCmd() {}
public:
  virtual void Recv(const char* cszIP, NCMD* pNCMD, BOOL bServer, void* pvClientData) = 0;
protected:
  BOOL m_bMax;
};

template <class T>
class DevCmd  : public SuperCmd
{
public:
  explicit DevCmd(BOOL bMax) : SuperCmd(bMax) {}
  virtual ~DevCmd() {}
public:
  virtual void Recv(const char* cszIP, NCMD* pNCMD, BOOL  bServer, void* pvClientData) {
    T dev_data(cszIP, pNCMD, bServer, pvClientData);
  }
};

template <class T>
class AutoRegister
{
public:
  AutoRegister(int nIdentify, BOOL bMax): m_Cmd(bMax) {
    ObjectFactory::GetInstance().AddProducer(nIdentify, &m_Cmd);
  }
private:
  DevCmd<T> m_Cmd;
};

#define REG_OBJECT(Identify, Type, bmax) \
  static AutoRegister<Type> DEV_MAKE_UNIQUE_NAME(autoRegisterRegistry__, Type, __LINE__)(Identify, bmax)

#define GET_OBJECT(Identify) (ObjectFactory::GetInstance().GetObj(Identify))

#endif // !defined(AFX_DEVCOMMAND_H__F617831D_295D_483C_8290_8511AEB46DB4__INCLUDED_)
