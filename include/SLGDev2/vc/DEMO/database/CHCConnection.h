// CHCConnection.h: interface for the CHCConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UECONNECTION_H__68839F94_AD6A_4297_9A9E_326CFDB8B331__INCLUDED_)
#define AFX_UECONNECTION_H__68839F94_AD6A_4297_9A9E_326CFDB8B331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CHCRecordset.h"

#include "CHCAutoPtr.h"
class CHCConnection  
{
public:

	CHCConnection();
	virtual ~CHCConnection();
public:

	virtual void Close() = 0;
	virtual BOOL IsOpen() = 0;
	virtual BOOL ExecuteNoSelectSQL(const CString& strSQL) = 0;
	virtual CHCRecordsetPtr ExecuteSelectSQL(const CString& strSQL) = 0;
	virtual void Open(const CString& strCource) =0;
	virtual LONG BeginTrans() = 0;
	virtual LONG CommitTrans() = 0;
	virtual LONG RollbackTrans() = 0;
	

};

typedef CHCAutoPtr<CHCConnection> CHCConnectPtr;
#endif // !defined(AFX_UECONNECTION_H__68839F94_AD6A_4297_9A9E_326CFDB8B331__INCLUDED_)
