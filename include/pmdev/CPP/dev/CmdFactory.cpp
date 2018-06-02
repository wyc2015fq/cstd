// CommandFactory.cpp: implementation of the CommandFactory class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "CmdFactory.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


ObjectFactory& ObjectFactory::GetInstance()
{
  static ObjectFactory ins;
  return ins;
}

ObjectFactory::~ObjectFactory()
{

}

void ObjectFactory::AddProducer(UINT uIdentify, SuperCmd* pObject)
{
  PRODUCER_CONTAINER::iterator it = m_ProducerContainer.find(uIdentify);

  if (it == m_ProducerContainer.end()) {
    m_ProducerContainer.insert(std::pair<UINT, SuperCmd*>(uIdentify, pObject));
  }
}

SuperCmd* ObjectFactory::GetObj(UINT uIdentify)
{
  PRODUCER_CONTAINER::iterator it = m_ProducerContainer.find(uIdentify);

  if (it == m_ProducerContainer.end()) {
    return NULL;
  }

  return it->second ;
}