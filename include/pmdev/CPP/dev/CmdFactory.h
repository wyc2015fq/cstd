// CommandFactory.h: interface for the CommandFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDFACTORY_H__C453EBA7_E1F3_4F90_B2DC_F28EC9F7EDE9__INCLUDED_)
#define AFX_COMMANDFACTORY_H__C453EBA7_E1F3_4F90_B2DC_F28EC9F7EDE9__INCLUDED_

#include "common.h"

#define DEV_MAKE_UNIQUE_NAME(symbol1, symbol2, symbol3) symbol1##symbol2##symbol3

class SuperCmd ;
class ObjectFactory
{
public:
  static ObjectFactory& GetInstance();
  virtual ~ObjectFactory();
public:
  void AddProducer(UINT uIdentify, SuperCmd* pObject);
  SuperCmd* GetObj(UINT uIdentify);
private:
  ObjectFactory() {}
  ObjectFactory(const ObjectFactory& ins);
  ObjectFactory& operator = (const ObjectFactory& ins);
private:
  typedef map<UINT, SuperCmd*> PRODUCER_CONTAINER;
  PRODUCER_CONTAINER m_ProducerContainer;
};



#endif // !defined(AFX_COMMANDFACTORY_H__C453EBA7_E1F3_4F90_B2DC_F28EC9F7EDE9__INCLUDED_)
