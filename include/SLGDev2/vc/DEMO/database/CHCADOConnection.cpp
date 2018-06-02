// CHCADOConnection.cpp: implementation of the CHCADOConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHCADOConnection.h"
#include <io.h> 
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCADOConnection::CHCADOConnection():m_pConnection(*m_pConn),m_pConn(new _ConnectionPtr)

{
	//m_pConnection = *m_pConn;
	//m_pConn = new _ConnectionPtr;
}

CHCADOConnection::~CHCADOConnection()
{
	this->Close();
}
BOOL CHCADOConnection::Reopen()
{
   CString str = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	str += m_strSource;
	HRESULT hr = this->m_pConnection.CreateInstance("ADODB.Connection"/*__uuidof(Connection)*/);
	HRESULT hRLt = m_pConnection->Open(_bstr_t( str ),"","",adConnectUnspecified);
	if ( hRLt == 0 )
		return TRUE;
	else 
		return FALSE;
}
void CHCADOConnection::Open(const CString &strCource)
{
	
	HRESULT hr = 0;
	try
	{
		if ( _taccess( strCource, 6 ) != 0 ) //06	Read and write permission.
		{
			_tchmod( strCource, _S_IREAD | _S_IWRITE );
		}

		CoInitialize(NULL); 
		hr = this->m_pConnection.CreateInstance("ADODB.Connection"/*__uuidof(Connection)*/);
		if (SUCCEEDED(hr))
		{
			CString str = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
			str += strCource;
			m_strSource = strCource;
			hr = m_pConnection->Open(_bstr_t( str ),"","",adConnectUnspecified);
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}

void CHCADOConnection::Open(const CString &strCource, enum CursorLocationEnum eCursorLocation )
{
	
	HRESULT hr = 0;
	try
	{
		if ( _taccess( strCource, 6 ) != 0 ) //06	Read and write permission.
		{
			_tchmod( strCource, _S_IREAD | _S_IWRITE );
		}

		CoInitialize(NULL); 

		hr = this->m_pConnection.CreateInstance("ADODB.Connection"/*__uuidof(Connection)*/);
		if(adStateConnecting!=( m_pConnection->GetState()& adStateConnecting))
		{
			m_pConnection->CursorLocation = eCursorLocation ;
		}
		if (SUCCEEDED(hr))
		{
			CString str = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
			str += strCource;
			hr = m_pConnection->Open(_bstr_t( str ),"","",adConnectUnspecified);
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
//		AfxMessageBox( e.Description() ) ;
	}
}


void CHCADOConnection::Close()
{
	try
	{
		if( m_pConnection != NULL && m_pConnection->State != adStateClosed )
		{
			m_pConnection->Close();
			CoUninitialize();
		}
		m_pConnection = NULL; 
	//	delete m_pConn;
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


BOOL CHCADOConnection::IsOpen()
{
	if( m_pConnection == NULL || m_pConnection->State == adStateClosed )
	{
		return FALSE;
	}
	return TRUE;
}




BOOL CHCADOConnection::ExecuteNoSelectSQL(const CString &strSQL)
{
	try
	{
		_variant_t RecordsAffected;

#if 0
		//afxDump<<strSQL.c_str()<<_T("\n");
#endif

		m_pConnection->Execute(_bstr_t(strSQL),NULL,adCmdText);
		ULONG iInfluence = (long)RecordsAffected;	
		
		return TRUE;	
	}
	catch (_com_error e)
	{		
		throw new CHCADOException(e);
		return FALSE;			
	}	
}




CHCRecordsetPtr CHCADOConnection::ExecuteSelectSQL(const CString &strSQL)
{
	try
	{
		_RecordsetPtr m_pRecordset;
		//DEBUG_INFO1(_T("%s"), strSQL.c_str());
		HRESULT hRet = m_pRecordset.CreateInstance(__uuidof(Recordset));
		//AfxMessageBox(strSQL);
		//CString str;
		//str.Format("we%d",m_pConnection->State);
		//AfxMessageBox(str);
		m_pRecordset->Open(_bstr_t(strSQL)
							,(IDispatch*)m_pConnection
							,adOpenDynamic
							,adLockOptimistic
							,adCmdText);
		//AfxMessageBox("125");
		return CHCRecordsetPtr(new CHCADORecordset(m_pRecordset));
	}
	catch (_com_error e)
	{	
		// Print Com errors.
		AfxMessageBox(e.ErrorMessage());
		AfxMessageBox(e.Source());
		AfxMessageBox(e.Description());
		throw new CHCADOException(e);
	}

	return CHCRecordsetPtr( NULL );
}

_RecordsetPtr CHCADOConnection::ExecuteSelectSQL(const CString &strSQL,short nMode)
{
	try
	{	
		_RecordsetPtr m_pRecordset;
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(strSQL)
			,(IDispatch*)m_pConnection
			,adOpenDynamic
			,adLockOptimistic
			,adCmdText);
		return m_pRecordset;
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return NULL;
}



_ConnectionPtr* CHCADOConnection::GetADOSmartpConnection()
{
	return m_pConn;
}




LONG CHCADOConnection::BeginTrans()
{
	LONG lResult = 0;
	try
	{
		m_pConnection->BeginTrans();
	}
	catch ( _com_error e ) 
	{
		throw new CHCADOException( e );
	}
	return lResult;
}




LONG CHCADOConnection::CommitTrans()
{
	LONG lResult = 0;
	try
	{
		m_pConnection->CommitTrans();
	}
	catch ( _com_error e ) 
	{
		throw new CHCADOException( e );
	}
	return lResult;
}





LONG CHCADOConnection::RollbackTrans()
{
	LONG lResult = 0;
	try
	{
		m_pConnection->RollbackTrans();
	}
	catch ( _com_error e ) 
	{
		throw new CHCADOException( e );
	}
	return lResult;
}