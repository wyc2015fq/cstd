# vc中通过MSXML操作XML文件 - 深之JohnChen的专栏 - CSDN博客

2006年04月17日 11:16:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3411


原文:http://www.codeproject.com/cpp/C___XML_wrapper.asp

对XML文件操作的时候,先安装XML解析器,并要到进行初始话,然后就可以用下面的封装好的类对XML文件进行读和写的操作了.

初始话很简单(和ADO的初始话工作类似),下边以基于对话框的工程(TEST)为例,进行初始话

只要在TEST.CPP的InitInstance()方法中加入

AfxOleInit();

就可以了.

// XmlNodeWrapper.h: interface for the CXmlNodeWrapper class.

//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLNODEWRAPPER_H__43622334_FDEB_4175_9E6D_19BBAA3992A5__INCLUDED_)

#define AFX_XMLNODEWRAPPER_H__43622334_FDEB_4175_9E6D_19BBAA3992A5__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

#import "MSXML3.dll" named_guids

using namespace MSXML2;

class CXmlNodeWrapper

{

public:

CString NodeType();

CString GetAttribVal(int index);

CString GetAttribName(int index);

int NumAttributes();

void ReplaceNode(MSXML2::IXMLDOMNode* pOldNode,MSXML2::IXMLDOMNode* pNewNode);

CString GetText();

void SetText(LPCTSTR text);

MSXML2::IXMLDOMNode* InsertAfter(MSXML2::IXMLDOMNode *refNode, MSXML2::IXMLDOMNode *pNode);

CString Name();

MSXML2::IXMLDOMNodeList* FindNodes(LPCTSTR searchStr);

MSXML2::IXMLDOMNode* Parent();

void RemoveNodes(LPCTSTR searchStr);

MSXML2::IXMLDOMNode* InsertAfter(MSXML2::IXMLDOMNode* refNode, LPCTSTR nodeName);

MSXML2::IXMLDOMNode* InsertBefore(MSXML2::IXMLDOMNode* refNode, LPCTSTR nodeName);

MSXML2::IXMLDOMNode* Interface();

MSXML2::IXMLDOMDocument* ParentDocument();

CString GetXML();

MSXML2::IXMLDOMNode* RemoveNode(MSXML2::IXMLDOMNodePtr pNode);

MSXML2::IXMLDOMNode* InsertNode(int index,LPCTSTR nodeName);

MSXML2::IXMLDOMNode* InsertNode(int index,MSXML2::IXMLDOMNodePtr pNode);

long NumNodes();

MSXML2::IXMLDOMNode* Detach();

MSXML2::IXMLDOMNode* GetNode(LPCTSTR nodeName);

MSXML2::IXMLDOMNode* GetNode(int nodeIndex);

MSXML2::IXMLDOMNode* FindNode(LPCTSTR searchString);

MSXML2::IXMLDOMNode* GetPrevSibling();

MSXML2::IXMLDOMNode* GetNextSibling();

void SetValue(LPCTSTR valueName,LPCTSTR value);

void SetValue(LPCTSTR valueName,int value);

void SetValue(LPCTSTR valueName,short value);

void SetValue(LPCTSTR valueName,double value);

void SetValue(LPCTSTR valueName,float value);

void SetValue(LPCTSTR valueName,bool value);

BOOL IsValid();

CString GetValue(LPCTSTR valueName);

CXmlNodeWrapper();

CXmlNodeWrapper(MSXML2::IXMLDOMNodePtr pNode,BOOL bAutoRelease = TRUE);

void operator=(MSXML2::IXMLDOMNodePtr pNode);

virtual ~CXmlNodeWrapper();

private:

BOOL m_bAutoRelease;

MSXML2::IXMLDOMNodePtr m_xmlnode;

};

class CXmlDocumentWrapper

{

public:

CString GetUrl();

CString GetXML();

BOOL IsValid();

BOOL Load(LPCTSTR path);

BOOL LoadXML(LPCTSTR xml);

BOOL Save(LPCTSTR path = "");

MSXML2::IXMLDOMDocument* Detach();

MSXML2::IXMLDOMDocument* Clone();

CXmlDocumentWrapper();

CXmlDocumentWrapper(MSXML2::IXMLDOMDocumentPtr pDoc);

MSXML2::IXMLDOMNode* AsNode();

virtual ~CXmlDocumentWrapper();

private:

MSXML2::IXMLDOMDocumentPtr m_xmldoc;

};

class CXmlNodeListWrapper

{

public:

MSXML2::IXMLDOMDocument* AsDocument();

MSXML2::IXMLDOMNode* Node(int index);

void Start();

MSXML2::IXMLDOMNode* Next();

BOOL IsValid();

int Count();

CXmlNodeListWrapper();

CXmlNodeListWrapper(MSXML2::IXMLDOMNodeListPtr pList);

void operator=(MSXML2::IXMLDOMNodeListPtr pList);

virtual ~CXmlNodeListWrapper();

private:

MSXML2::IXMLDOMNodeListPtr m_xmlnodelist;

};

#endif // !defined(AFX_XMLNODEWRAPPER_H__43622334_FDEB_4175_9E6D_19BBAA3992A5__INCLUDED_)

// XmlNodeWrapper.cpp: implementation of the CXmlNodeWrapper class.

//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XmlNodeWrapper.h"

#ifdef _DEBUG

#undef THIS_FILE

static char THIS_FILE[]=__FILE__;

#define new DEBUG_NEW

#endif

//////////////////////////////////////////////////////////////////////

// Construction/Destruction

//////////////////////////////////////////////////////////////////////

CXmlNodeWrapper::CXmlNodeWrapper()

{

}

CXmlNodeWrapper::CXmlNodeWrapper(MSXML2::IXMLDOMNodePtr pNode,BOOL bAutoRelease)

{

m_xmlnode = pNode;

m_bAutoRelease = bAutoRelease;

}

void CXmlNodeWrapper::operator=(MSXML2::IXMLDOMNodePtr pNode)

{

if (IsValid())

m_xmlnode.Release();// = 号的重载,优化内存,释放不用的资源

m_xmlnode = pNode;

}

CXmlNodeWrapper::~CXmlNodeWrapper()

{

if (!m_bAutoRelease)

m_xmlnode.Detach();

}

CString CXmlNodeWrapper::GetValue(LPCTSTR valueName)

{

if (!IsValid())

return "";

MSXML2::IXMLDOMNodePtr attribute = m_xmlnode->Getattributes()->getNamedItem(valueName);

if (attribute)

{

return (LPCSTR)attribute->Gettext();

}

return "";

}

BOOL CXmlNodeWrapper::IsValid()

{

if (m_xmlnode == NULL)

return FALSE;

if (m_xmlnode.GetInterfacePtr() == NULL)

return FALSE;

return TRUE;

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::GetPrevSibling()

{

if (!IsValid())

return NULL;

return m_xmlnode->GetpreviousSibling().Detach();

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::GetNextSibling()

{

if (!IsValid())

return NULL;

return m_xmlnode->GetnextSibling().Detach();

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::GetNode(LPCTSTR nodeName)

{

if (!IsValid())

return NULL;

try{

return m_xmlnode->selectSingleNode(nodeName).Detach();

}

catch (_com_error e)

{

CString err = e.ErrorMessage();

}

return NULL;

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::GetNode(int nodeIndex)

{

if (!IsValid())

return NULL;

return m_xmlnode->GetchildNodes()->Getitem(nodeIndex).Detach();

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::FindNode(LPCTSTR searchString)

{

if (!IsValid())

return NULL;

try{

return m_xmlnode->selectSingleNode(searchString).Detach();

}

catch (_com_error e)

{

CString err = e.ErrorMessage();

}

return NULL;

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::Detach()

{

if (IsValid())

{

return m_xmlnode.Detach();

}

else

return NULL;

}

long CXmlNodeWrapper::NumNodes()

{

if (IsValid())

{

return m_xmlnode->GetchildNodes()->Getlength();

}

else

return 0;

}

void CXmlNodeWrapper::SetValue(LPCTSTR valueName,LPCTSTR value)

{

MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();

if (xmlDocument)

{

MSXML2::IXMLDOMNamedNodeMapPtr attributes = m_xmlnode->Getattributes();

if (attributes)

{

MSXML2::IXMLDOMAttributePtr attribute = xmlDocument->createAttribute(valueName);

if (attribute)

{

attribute->Puttext(value);

attributes->setNamedItem(attribute);

}

}

}

}

void CXmlNodeWrapper::SetValue(LPCTSTR valueName,int value)

{

CString str;

str.Format("%ld",value);

SetValue(valueName,str);

}

void CXmlNodeWrapper::SetValue(LPCTSTR valueName,short value)

{

CString str;

str.Format("%hd",value);

SetValue(valueName,str);

}

void CXmlNodeWrapper::SetValue(LPCTSTR valueName,double value)

{

CString str;

str.Format("%f",value);

SetValue(valueName,str);

}

void CXmlNodeWrapper::SetValue(LPCTSTR valueName,float value)

{

CString str;

str.Format("%f",value);

SetValue(valueName,str);

}

void CXmlNodeWrapper::SetValue(LPCTSTR valueName,bool value)

{

CString str;

if (value)

str = "True";

else

str = "False";

SetValue(valueName,str);

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::InsertNode(int index,LPCTSTR nodeName)

{

MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();

if (xmlDocument)

{

MSXML2::IXMLDOMNodePtr newNode = xmlDocument->createNode(_variant_t((short)MSXML2::NODE_ELEMENT),nodeName,"");

MSXML2::IXMLDOMNode* refNode = GetNode(index);

if (refNode)

newNode = m_xmlnode->insertBefore(newNode.Detach(),refNode);

else

newNode = m_xmlnode->appendChild(newNode.Detach());

return newNode.Detach();

}

return NULL;

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::InsertNode(int index,MSXML2::IXMLDOMNodePtr pNode)

{

MSXML2::IXMLDOMNodePtr newNode = pNode->cloneNode(VARIANT_TRUE);

if (newNode)

{

MSXML2::IXMLDOMNode* refNode = GetNode(index);

if (refNode)

newNode = m_xmlnode->insertBefore(newNode.Detach(),refNode);

else

newNode = m_xmlnode->appendChild(newNode.Detach());

return newNode.Detach();

}

else

return NULL;

}

CString CXmlNodeWrapper::GetXML()

{

if (IsValid())

return (LPCSTR)m_xmlnode->Getxml();

else

return "";

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::RemoveNode(MSXML2::IXMLDOMNodePtr pNode)

{

if (!IsValid())

return NULL;

return m_xmlnode->removeChild(pNode).Detach();

}

/* ********************************************************************* */

CXmlDocumentWrapper::CXmlDocumentWrapper()

{

m_xmldoc.CreateInstance(MSXML2::CLSID_DOMDocument);

}

CXmlDocumentWrapper::CXmlDocumentWrapper(MSXML2::IXMLDOMDocumentPtr pDoc)

{

m_xmldoc = pDoc;

}

CXmlDocumentWrapper::~CXmlDocumentWrapper()

{

}

BOOL CXmlDocumentWrapper::IsValid()

{

if (m_xmldoc == NULL)

return FALSE;

if (m_xmldoc.GetInterfacePtr() == NULL)

return FALSE;

return TRUE;

}

MSXML2::IXMLDOMDocument* CXmlDocumentWrapper::Detach()

{

if (!IsValid())

return NULL;

return m_xmldoc.Detach();

}

MSXML2::IXMLDOMDocument* CXmlDocumentWrapper::Clone()

{

if (!IsValid())

return NULL;

MSXML2::IXMLDOMDocumentPtr xmldoc;

xmldoc.CreateInstance(MSXML2::CLSID_DOMDocument);

_variant_t v(xmldoc.GetInterfacePtr());

m_xmldoc->save(v);

return xmldoc.Detach();

}

BOOL CXmlDocumentWrapper::Load(LPCTSTR path)

{

if (!IsValid())

return FALSE;

_variant_t v(path);

m_xmldoc->put_async(VARIANT_FALSE);

VARIANT_BOOL success = m_xmldoc->load(v);

if (success == VARIANT_TRUE)

return TRUE;

else

return FALSE;

}

BOOL CXmlDocumentWrapper::LoadXML(LPCTSTR xml)

{

if (!IsValid())

return FALSE;

VARIANT_BOOL success = m_xmldoc->loadXML(xml);

if (success == VARIANT_TRUE)

return TRUE;

else

return FALSE;

}

BOOL CXmlDocumentWrapper::Save(LPCTSTR path)

{

try

{

if (!IsValid())

return FALSE;

CString szPath(path);

if (szPath == "")

{

_bstr_t curPath = m_xmldoc->Geturl();

szPath = (LPSTR)curPath;

}

_variant_t v(szPath);

if (FAILED(m_xmldoc->save(v)))

return FALSE;

else

return TRUE;

}

catch(...)

{

return FALSE;

}

}

MSXML2::IXMLDOMNode* CXmlDocumentWrapper::AsNode()

{

if (!IsValid())

return NULL;

return m_xmldoc->GetdocumentElement().Detach();

}

CString CXmlDocumentWrapper::GetXML()

{

if (IsValid())

return (LPCSTR)m_xmldoc->Getxml();

else

return "";

}

CString CXmlDocumentWrapper::GetUrl()

{

return (LPSTR)m_xmldoc->Geturl();

}

MSXML2::IXMLDOMDocument* CXmlNodeWrapper::ParentDocument()

{

return m_xmlnode->GetownerDocument().Detach();

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::Interface()

{

if (IsValid())

return m_xmlnode;

return NULL;

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::InsertBefore(MSXML2::IXMLDOMNode *refNode, LPCTSTR nodeName)

{

MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();

if (xmlDocument)

{

MSXML2::IXMLDOMNodePtr newNode = xmlDocument->createNode(_variant_t((short)MSXML2::NODE_ELEMENT),nodeName,"");

newNode = m_xmlnode->insertBefore(newNode.Detach(),(IUnknown*)refNode);

return newNode.Detach();

}

return NULL;

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::InsertAfter(MSXML2::IXMLDOMNode *refNode, LPCTSTR nodeName)

{

MSXML2::IXMLDOMDocumentPtr xmlDocument = m_xmlnode->GetownerDocument();

if (xmlDocument)

{

MSXML2::IXMLDOMNodePtr newNode = xmlDocument->createNode(_variant_t((short)MSXML2::NODE_ELEMENT),nodeName,"");

MSXML2::IXMLDOMNodePtr nextNode = refNode->GetnextSibling();

if (nextNode.GetInterfacePtr() != NULL)

newNode = m_xmlnode->insertBefore(newNode.Detach(),(IUnknown*)nextNode);

else

newNode = m_xmlnode->appendChild(newNode.Detach());

return newNode.Detach();

}

return NULL;

}

void CXmlNodeWrapper::RemoveNodes(LPCTSTR searchStr)

{

if (!IsValid())

return;

MSXML2::IXMLDOMNodeListPtr nodeList = m_xmlnode->selectNodes(searchStr);

for (int i = 0; i < nodeList->Getlength(); i++)

{

try

{

MSXML2::IXMLDOMNode* pNode = nodeList->Getitem(i).Detach();

pNode->GetparentNode()->removeChild(pNode);

}

catch (_com_error er)

{

AfxMessageBox(er.ErrorMessage());

}

}

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::Parent()

{

if (IsValid())

return m_xmlnode->GetparentNode().Detach();

return NULL;

}

CXmlNodeListWrapper::CXmlNodeListWrapper()

{

}

CXmlNodeListWrapper::CXmlNodeListWrapper(MSXML2::IXMLDOMNodeListPtr pList)

{

m_xmlnodelist = pList;

}

void CXmlNodeListWrapper::operator=(MSXML2::IXMLDOMNodeListPtr pList)

{

if (IsValid())

m_xmlnodelist.Release();

m_xmlnodelist = pList;

}

CXmlNodeListWrapper::~CXmlNodeListWrapper()

{

}

int CXmlNodeListWrapper::Count()

{

if (IsValid())

return m_xmlnodelist->Getlength();

else

return 0;

}

BOOL CXmlNodeListWrapper::IsValid()

{

if (m_xmlnodelist == NULL)

return FALSE;

if (m_xmlnodelist.GetInterfacePtr() == NULL)

return FALSE;

return TRUE;

}

MSXML2::IXMLDOMNode* CXmlNodeListWrapper::Next()

{

if (IsValid())

return m_xmlnodelist->nextNode().Detach();

else

return NULL;

}

void CXmlNodeListWrapper::Start()

{

if (IsValid())

m_xmlnodelist->reset();

}

MSXML2::IXMLDOMNode* CXmlNodeListWrapper::Node(int index)

{

if (IsValid())

return m_xmlnodelist->Getitem(index).Detach();

else

return NULL;

}

MSXML2::IXMLDOMDocument* CXmlNodeListWrapper::AsDocument()

{

if (IsValid())

{

CXmlDocumentWrapper doc;

doc.LoadXML("<NodeList></NodeList>");

CXmlNodeWrapper root(doc.AsNode());

for (int i = 0; i < m_xmlnodelist->Getlength(); i++)

{

root.InsertNode(root.NumNodes(),m_xmlnodelist->Getitem(i)->cloneNode(VARIANT_TRUE));

}

return doc.Detach();

}

else

return NULL;

}

MSXML2::IXMLDOMNodeList* CXmlNodeWrapper::FindNodes(LPCTSTR searchStr)

{

if(IsValid())

{

try{

return m_xmlnode->selectNodes(searchStr).Detach();

}

catch (_com_error e)

{

CString err = e.ErrorMessage();

return NULL;

}

}

else

return NULL;

}

CString CXmlNodeWrapper::Name()

{

if (IsValid())

return (LPCSTR)m_xmlnode->GetbaseName();

return "";

}

MSXML2::IXMLDOMNode* CXmlNodeWrapper::InsertAfter(MSXML2::IXMLDOMNode *refNode, MSXML2::IXMLDOMNode *pNode)

{

MSXML2::IXMLDOMNodePtr nextNode = refNode->GetnextSibling();

MSXML2::IXMLDOMNodePtr newNode;

if (nextNode.GetInterfacePtr() != NULL)

newNode = m_xmlnode->insertBefore(pNode,(IUnknown*)nextNode);

else

newNode = m_xmlnode->appendChild(pNode);

return newNode.Detach();

}

void CXmlNodeWrapper::SetText(LPCTSTR text)

{

if (IsValid())

m_xmlnode->Puttext(text);

}

CString CXmlNodeWrapper::GetText()

{

if (IsValid())

return (LPCSTR)m_xmlnode->Gettext();

else

return "";

}

void CXmlNodeWrapper::ReplaceNode(MSXML2::IXMLDOMNode *pOldNode, MSXML2::IXMLDOMNode *pNewNode)

{

if (IsValid())

{

m_xmlnode->replaceChild(pNewNode,pOldNode);

}

}

int CXmlNodeWrapper::NumAttributes()

{

if (IsValid())

{

MSXML2::IXMLDOMNamedNodeMapPtr attribs = m_xmlnode->Getattributes();

if (attribs)

return attribs->Getlength();

}

return 0;

}

CString CXmlNodeWrapper::GetAttribName(int index)

{

if (IsValid())

{

MSXML2::IXMLDOMNamedNodeMapPtr attribs = m_xmlnode->Getattributes();

if (attribs)

{

MSXML2::IXMLDOMAttributePtr attrib = attribs->Getitem(index);

if (attrib)

return (LPCSTR)attrib->Getname();

}

}

return "";

}

CString CXmlNodeWrapper::GetAttribVal(int index)

{

if (IsValid())

{

MSXML2::IXMLDOMNamedNodeMapPtr attribs = m_xmlnode->Getattributes();

if (attribs)

{

MSXML2::IXMLDOMAttributePtr attrib = attribs->Getitem(index);

if (attrib)

return (LPCSTR)attrib->Gettext();

}

}

return "";

}

CString CXmlNodeWrapper::NodeType()

{

if (IsValid())

return (LPCSTR)m_xmlnode->GetnodeTypeString();

return "";

}

