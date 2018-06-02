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

void CHCADOConnection::Open(const CString &strCource)
{
	
	HRESULT hr = 0;
	try
	{
		//没有读写属性则设置文件具有读写属性
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
			hr = m_pConnection->Open(_bstr_t( str ),"","",-1);
		//	if ()
			/*******************************************
			HRESULT Connection15::Open ( _bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options )
			ConnectionString为连接字串,UserID是用户名, Password是登陆密码,Options是连接选项,用于指定Connection对象对数据的更新许可权,
			Options可以是如下几个常量:
			adModeUnknown:缺省。当前的许可权未设置
			adModeRead:只读
			adModeWrite:只写
			adModeReadWrite:可以读写
			adModeShareDenyRead:阻止其它Connection对象以读权限打开连接
			adModeShareDenyWrite:阻止其它Connection对象以写权限打开连接
			adModeShareExclusive:阻止其它Connection对象以读写权限打开连接
			adModeShareDenyNone:阻止其它Connection对象以任何权限打开连接
			
			  我们给出一些常用的连接方式供大家参考:
			  (1)通过JET数据库引擎对ACCESS2000数据库的连接
			  
				m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\\test.mdb","","",adModeUnknown);
				
			**********************************************/
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
		//没有读写属性则设置文件具有读写属性
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
			hr = m_pConnection->Open(_bstr_t( str ),"","",-1);
			/*******************************************
			HRESULT Connection15::Open ( _bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options )
			ConnectionString为连接字串,UserID是用户名, Password是登陆密码,Options是连接选项,用于指定Connection对象对数据的更新许可权,
			Options可以是如下几个常量:
			adModeUnknown:缺省。当前的许可权未设置
			adModeRead:只读
			adModeWrite:只写
			adModeReadWrite:可以读写
			adModeShareDenyRead:阻止其它Connection对象以读权限打开连接
			adModeShareDenyWrite:阻止其它Connection对象以写权限打开连接
			adModeShareExclusive:阻止其它Connection对象以读写权限打开连接
			adModeShareDenyNone:阻止其它Connection对象以任何权限打开连接
			
			  我们给出一些常用的连接方式供大家参考:
			  (1)通过JET数据库引擎对ACCESS2000数据库的连接
			  
				m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\\test.mdb","","",adModeUnknown);
				
			**********************************************/
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
//		AfxMessageBox( e.Description() ) ;
	}
}

/*************************************************
Function:       Close
Description:    关闭与ADO数据库的连接
Calls:          ADO方法Close
Called By:      该类对象
Table Accessed: 无
Table Updated:  无
Input:          无
Output:         无
Return:         无
Others:         
*************************************************/

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


/*************************************************
Function:       ExecuteNoSelectSQL
Description:    执行不需要返回记录集的操作
Calls:          ADO方法Execute
Called By:      该类对象
Table Accessed: 当前打开表
Table Updated:  当前打开表
Input:          无
Output:         无
Return:         无
Others:         
*************************************************/

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


/*************************************************
  Function:       ExecuteSelectSQL
  Description:    执行需要返回记录集的操作
  Calls:          ADO方法CreateInstance，Open
  Called By:      该类对象
  Table Accessed: 当前打开表
  Table Updated:  当前打开表
  Input:          无
  Output:         无
  Return:         
  Others:         
*************************************************/

CHCRecordsetPtr CHCADOConnection::ExecuteSelectSQL(const CString &strSQL)
{
	try
	{	
		_RecordsetPtr m_pRecordset;
		//DEBUG_INFO1(_T("%s"), strSQL.c_str());
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(strSQL)
							,(IDispatch*)m_pConnection
							,adOpenDynamic
							,adLockOptimistic
							,adCmdText);
		return CHCRecordsetPtr(new CHCADORecordset(m_pRecordset));
	}
	catch (_com_error e)
	{	
		throw new CHCADOException(e);
	}

	return CHCRecordsetPtr( NULL );//暂时代码 
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
	return NULL;//暂时代码 
}


/*************************************************
  Function:       GetADOSmartpConnection
  Description:    返回ADO连接指针。
  Calls:          
  Called By:      该类对象
  Table Accessed: 当前打开表
  Table Updated:  当前打开表
  Input:          无
  Output:         无
  Return:         _ConnectionPtr*
  Others:         
*************************************************/

_ConnectionPtr* CHCADOConnection::GetADOSmartpConnection()
{
	return m_pConn;
}


/*************************************************
  Function:       BeginTrans
  Description:    启动新事务
  Calls:          
  Called By:      该类对象
  Input:          无
  Output:         无
  Return:         LONG
  Others:         
*************************************************/

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


/*************************************************
  Function:       BeginTrans
  Description:    确认当前事务
  Calls:          
  Called By:      该类对象
  Input:          无
  Output:         无
  Return:         LONG
  Others:         
*************************************************/

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



/*************************************************
  Function:       BeginTrans
  Description:    撤消当前事务
  Calls:          
  Called By:      该类对象
  Input:          无
  Output:         无
  Return:         LONG
  Others:         
*************************************************/

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