# (转载)ADO技术 - 深之JohnChen的专栏 - CSDN博客

2005年09月01日 14:47:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：9107


/*########################################################################
 【文件名】:  ado.h
 【名  称】:  ADO 封装类.
 【版  本】:  0.20
 【作  者】:  成真
 【E-mail】:  [anyou@sina.com](mailto:anyou@sina.com)
  ---------------------------------------------------------
 【创建时间】:  20/1/2003 19:08
 【修改时间】:  09/4/2003
  ---------------------------------------------------------
 【版本历史】:
  [0.20]  :   封装了 CAdoCommand 类.
   [-15/4/2003-]

  [0.11]  :   修正了一些 bug.
     删除 oleinit() 函数, 因为觉得不妥。
     增加了记录集存取函数.
   [-09/4/2003-]

  [0.10] : 主要包括 CAdoConnection 和 CAdoRecordSet 两个类, 封装了
   对ado的常用的操作方法.
   [-20/1/2003-]
  ---------------------------------------------------------
 【使用说明】:
  1. 必须在下面指明 msado15.dll 的全路径, 一般在 "C:/Program Files/
     Common Files/System/ADO/" 目录下.
  2. 在使用本类之前必须要初始化 COM 环境, 可以调用 CoInitialize(NULL)  
     来初始化, 用 CoUninitialize() 释放;
  3. 在使用记录集对象前必须先调用 CAdoConnection::Open() 连接数据库,
     连接后可给多个 CAdoRecordSet 对象使用, 用完后须关闭数据库.
  4. 使用记录集执行SQL语句之前, 要使用构建方法或调用 SetAdoConnection() 
     关联到连接对象.
  5. 在编译过程中不用理会下面的编译警告:
     warning: unary minus operator applied to unsigned type, result
     still unsigned
     如果不想此警告出现,可以在 StdAfx.h 文件中加入这样一行代码以禁止
     此警告:  #pragma warning(disable:4146) 
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADO_H)
#define _ANYOU_COOL_ADO_H

#if _MSC_VER > 1000
#pragma once
#endif 

// 导入 ado 库 -----------------------------------------------------------
#pragma warning(disable:4146)
#import "C:/Program Files/Common Files/System/ADO/msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;  

#include <icrsint.h>
class CAdoConnection;
#include "AdoRecordSet.h"
//#include "AdoCommand.h"

// 数值类型转换 -----------------------------------
COleDateTime vartodate(const _variant_t& var);
COleCurrency vartocy(const _variant_t& var);
bool vartobool(const _variant_t& var);
BYTE vartoby(const _variant_t& var);
short vartoi(const _variant_t& var);
long vartol(const _variant_t& var);
double vartof(const _variant_t& var);
CString vartostr(const _variant_t& var);

/*########################################################################
     ------------------------------------------------
          CAdoConnection class
     ------------------------------------------------
  ########################################################################*/
class CAdoConnection
{
// 构建/析构 ------------------------------------------
public:
 CAdoConnection();
 virtual ~CAdoConnection();

protected:
 void Release();

// 属性 -----------------------------------------------
public:
 // 连接对象 ----------------------------------
 _ConnectionPtr& GetConnection() {return m_pConnection;};

 // 异常信息 ----------------------------------
 CString GetLastErrorText();
 ErrorsPtr GetErrors();
 ErrorPtr  GetError(long index);

 // 连接字串 ----------------------------------
 CString GetConnectionText() {return m_strConnect;}

 // 连接信息 ----------------------------------
 CString GetProviderName();
 CString GetVersion();
 CString GetDefaultDatabase();

 // 连接状态 ----------------------------------
 BOOL IsOpen();
 long GetState();

 // 连接模式 ----------------------------------
 ConnectModeEnum GetMode();
 BOOL SetMode(ConnectModeEnum mode);

 // 连接时间 ----------------------------------
 long GetConnectTimeOut();
 BOOL SetConnectTimeOut(long lTime = 5);

 // 数据源信息 -------------------------------
 _RecordsetPtr OpenSchema(SchemaEnum QueryType);

// 操作 -----------------------------------------------
public:
 // 数据库连接 --------------------------------
 BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
 BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
 BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
    BOOL ConnectDefault();
 BOOL OpenUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
 void Close();

// 处理 -----------------------------------------------
public:
 // 事务处理 ----------------------------------
 long BeginTrans();
 BOOL RollbackTrans();
 BOOL CommitTrans();

 // 执行 SQL 语句 ------------------------------
 _RecordsetPtr Execute(LPCTSTR strSQL, long lOptions = adCmdText);
 BOOL Cancel();

// 数据 -----------------------------------------------
protected:
 CString   m_strConnect;
 _ConnectionPtr m_pConnection;
};

#endif // !defined(_ANYOU_COOL_ADO_H)

/*########################################################################
 Filename:  ado.cpp
 ----------------------------------------------------
 Remarks: ...
 ----------------------------------------------------
 Author:  成真
 Email:  [anyou@sina.com](mailto:anyou@sina.com)
[anyou@msn.com](mailto:anyou@msn.com)
 Created: 20/1/2003 19:19
  ########################################################################*/
#include "stdafx.h"
#include "ADO.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*########################################################################
     ------------------------------------------------
           数值类型转换
     ------------------------------------------------
  ########################################################################*/
COleDateTime vartodate(const _variant_t& var)
{
 COleDateTime value;
 switch (var.vt) 
 {
 case VT_DATE:
  {
   value = var.date;
  }
  break;
 case VT_EMPTY:
 case VT_NULL:
  value.SetStatus(COleDateTime::null);
  break;
 default:
  value.SetStatus(COleDateTime::null);
  TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 }
 return value;
}

COleCurrency vartocy(const _variant_t& var)
{
 COleCurrency value;
 switch (var.vt) 
 {
 case VT_CY:
  value = (CURRENCY)var.cyVal;
  break;
 case VT_EMPTY:
 case VT_NULL:
  value.m_status = COleCurrency::null;
  break;
 default:
  value.m_status = COleCurrency::null;
  TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 }
 return value;
}

bool vartobool(const _variant_t& var)
{
 bool value = false;
 switch (var.vt)
 {
 case VT_BOOL:
  value = var.boolVal ? true : false;
 case VT_EMPTY:
 case VT_NULL:
  break;
 default:
  TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 }
 return value;
}

BYTE vartoby(const _variant_t& var)
{
 BYTE value = 0;
 switch (var.vt)
 {
 case VT_I1:
 case VT_UI1:
  value = var.bVal;
  break;
 case VT_NULL:
 case VT_EMPTY:
  value = 0;
  break;
 default:
  TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 } 
 return value;
}

short vartoi(const _variant_t& var)
{
 short value = 0;
 switch (var.vt)
 {
 case VT_BOOL:
  value = var.boolVal;
  break;
 case VT_UI1:
 case VT_I1:
  value = var.bVal;
  break;
 case VT_I2:
 case VT_UI2:
  value = var.iVal;
  break;
 case VT_NULL:
 case VT_EMPTY:
  value = 0;
  break;
 default:
  TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 } 
 return value;
}

long vartol(const _variant_t& var)
{
 long value = 0;
 switch (var.vt)
 {
 case VT_BOOL:
  value = var.boolVal;
  break;
 case VT_UI1:
 case VT_I1:
  value = var.bVal;
  break;
 case VT_UI2:
 case VT_I2:
  value = var.iVal;
  break;
 case VT_I4:
 case VT_UI4:
  value = var.lVal;
  break;
 case VT_INT:
  value = var.intVal;
  break;
 case VT_R4:
  value = (long)(var.fltVal + 0.5);
  break;
 case VT_R8:
  value = (long)(var.dblVal + 0.5);
  break;
 case VT_DECIMAL:
  value = (long)var;
  break;
 case VT_CY:
  value = (long)var;
  break;
 case VT_BSTR://字符串
 case VT_LPSTR://字符串
 case VT_LPWSTR://字符串
  value = atol((LPCTSTR)(_bstr_t)var);
  break;
 case VT_NULL:
 case VT_EMPTY:
  value = 0;
  break;
 default:
  TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 }
 return value;
}

double vartof(const _variant_t& var)
{
 double value = 0;
 switch (var.vt)
 {
 case VT_R4:
  value = var.fltVal;
  break;
 case VT_R8:
  value = var.dblVal;
  break;
 case VT_DECIMAL:
  value = (double)var;
  break;
 case VT_CY:
  value = (double)var;
  break;
 case VT_BOOL:
  value = var.boolVal;
  break;
 case VT_UI1:
 case VT_I1:
  value = var.bVal;
  break;
 case VT_UI2:
 case VT_I2:
  value = var.iVal;
  break;
 case VT_UI4:
 case VT_I4:
  value = var.lVal;
  break;
 case VT_INT:
  value = var.intVal;
  break;
 case VT_BSTR://字符串
 case VT_LPSTR://字符串
 case VT_LPWSTR://字符串
  value = atof((LPCTSTR)(_bstr_t)var);
  break;
 case VT_NULL:
 case VT_EMPTY:
  value = 0;
  break;
 default:
  value = 0;
  TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 }
 return value;
}

CString vartostr(const _variant_t &var)
{
 CString strValue;

 switch (var.vt)
 {
 case VT_BSTR://字符串
 case VT_LPSTR://字符串
 case VT_LPWSTR://字符串
  strValue = (LPCTSTR)(_bstr_t)var;
  break;
 case VT_I1:
 case VT_UI1:
  strValue.Format("%d", var.bVal);
  break;
 case VT_I2://短整型
  strValue.Format("%d", var.iVal);
  break;
 case VT_UI2://无符号短整型
  strValue.Format("%d", var.uiVal);
  break;
 case VT_INT://整型
  strValue.Format("%d", var.intVal);
  break;
 case VT_I4: //整型
  strValue.Format("%d", var.lVal);
  break;
 case VT_I8: //长整型
  strValue.Format("%d", var.lVal);
  break;
 case VT_UINT://无符号整型
  strValue.Format("%d", var.uintVal);
  break;
 case VT_UI4: //无符号整型
  strValue.Format("%d", var.ulVal);
  break;
 case VT_UI8: //无符号长整型
  strValue.Format("%d", var.ulVal);
  break;
 case VT_VOID:
  strValue.Format("%8x", var.byref);
  break;
 case VT_R4://浮点型
  strValue.Format("%.4f", var.fltVal);
  break;
 case VT_R8://双精度型
  strValue.Format("%.8f", var.dblVal);
  break;
 case VT_DECIMAL: //小数
  strValue.Format("%.8f", (double)var);
  break;
 case VT_CY:
  {
   COleCurrency cy = var.cyVal;
   strValue = cy.Format();
  }
  break;
 case VT_BLOB:
 case VT_BLOB_OBJECT:
 case 0x2011:
  strValue = "[BLOB]";
  break;
 case VT_BOOL://布尔型

  strValue = var.boolVal ? "TRUE" : "FALSE";
  break;
 case VT_DATE: //日期型
  {
   DATE dt = var.date;
   COleDateTime da = COleDateTime(dt); 
   strValue = da.Format("%Y-%m-%d %H:%M:%S");
  }
  break;
 case VT_NULL://NULL值
  strValue = "";
  break;
 case VT_EMPTY://空
  strValue = "";
  break;
 case VT_UNKNOWN://未知类型
 default:
  strValue = "UN_KNOW";
  break;
 }
 return strValue;
}

/*########################################################################
    ------------------------------------------------
     CAdoConnection class 构造/析构函数
    ------------------------------------------------
  ########################################################################*/

CAdoConnection::CAdoConnection()
{
 //创建 Connection 对象---------------------------
 m_pConnection.CreateInstance("ADODB.Connection");
 #ifdef _DEBUG
 if (m_pConnection == NULL)
 {
  AfxMessageBox("Connection 对象创建失败! 请确认是否初始化了COM环境/r/n");
 }
 #endif
 ASSERT(m_pConnection != NULL);
}

CAdoConnection::~CAdoConnection()
{
 if (m_pConnection != NULL)
 {
  Release();
 }
}

/*========================================================================
 Name:  连接到数据源.
 -----------------------------------------------------
 Params:  [lpszConnect]: 连接字符串, 包含连接信息.
    [lOptions]: 可选. 决定该方法是以同步还是异步的方式连接数据
      源. 可以是如下某个常量:
  [常量]     [说明] 
  ----------------------------------
  adConnectUnspecified (默认)同步方式打开连接. 
  adAsyncConnect   异步方式打开连接. Ado用 ConnectComplete 事
      件来通知已经完成连接. 
==========================================================================*/
BOOL CAdoConnection::Open(LPCTSTR lpszConnect, long lOptions)
{
 ASSERT(m_pConnection != NULL);
 ASSERT(AfxIsValidString(lpszConnect));

 if (strcmp(lpszConnect, _T("")) != 0)
 {
  m_strConnect = lpszConnect;
 }

 if (m_strConnect.IsEmpty())
 {
  ASSERT(FALSE);
  return FALSE;
 }

 if (IsOpen()) Close();

 try
 {
  // 连接数据库 ---------------------------------------------
  return (m_pConnection->Open(_bstr_t(LPCTSTR(m_strConnect)), "", "", lOptions) == S_OK);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: 连接数据库发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  TRACE(_T("%s/r/n"), GetLastErrorText());
  return FALSE;
 } 
 catch (...)
 {
  TRACE(_T("Warning: 连接数据库时发生未知错误:"));
 }
 return FALSE;
}

/*========================================================================
 Name: 连接 SQL SERVER 数据库. 
 -----------------------------------------------------
 Params:  [dbsrc]: SQL SERVER 服务器名.
    [dbname]: 默认的数据库名.
    [user]:  用户名.
    [pass]:  密码.
==========================================================================*/
BOOL CAdoConnection::ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions)
{
 CString strConnect = _T("Provider=SQLOLEDB.1; Data Source=") + dbsrc + 
       _T("; Initial Catalog=") + dbname  +
       _T("; User ID=") + user + 
       _T("; PWD=") + pass;
 return Open(LPCTSTR(strConnect), lOptions);
}
/*========================================================================
 Name: 连接 ACCESS 数据库. 
 -----------------------------------------------------
 Params:  [dbpath]: MDB 数据库文件路径名.
    [pass]:  访问密码.
===========================================================================*/
BOOL CAdoConnection::ConnectDefault()
{
 CString strConnect = _T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source=");
 strConnect += "DataBase.mdb";
 return Open(LPCTSTR(strConnect),adConnectUnspecified);
}

/*========================================================================
 Name: 连接 ACCESS 数据库. 
 -----------------------------------------------------
 Params:  [dbpath]: MDB 数据库文件路径名.
    [pass]:  访问密码.
===========================================================================*/
BOOL CAdoConnection::ConnectAccess(CString dbpath, CString pass, long lOptions)
{
 CString strConnect = _T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source=") + dbpath;
 if (pass != _T("")) 
 {
  strConnect += _T("Jet OLEDB:Database Password=") + pass + _T(";");
 }
 return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
 Name: 通过 udl 文件连接数据库. 
 -----------------------------------------------------
 Params:  [strFileName]: UDL 数据库连接文件路径名.
==========================================================================*/
BOOL CAdoConnection::OpenUDLFile(LPCTSTR strFileName, long lOptions)
{
 CString strConnect = _T("File Name=");
 strConnect += strFileName;
 return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
 Name: 关闭与数据源的连接.
 -----------------------------------------------------
 Remarks: 使用 Close 方法可关闭 Connection 对象以便释放所有关联的系统资源. 
==========================================================================*/
void CAdoConnection::Close()
{
 try
 {
  if (m_pConnection != NULL && IsOpen()) 
  {
   m_pConnection->Close();
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: 关闭数据库发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 } 
}

/*========================================================================
 Name: 关闭连接并释放对象.
 -----------------------------------------------------
 Remarks: 关闭连接并释放connection对象.
==========================================================================*/
void CAdoConnection::Release()
{
 if (IsOpen()) Close();
 m_pConnection.Release();
}

/*========================================================================
 Name:  执行指定的查询、SQL 语句、存储过程等.
    ----------------------------------------------------------
 Remarks: 请参考 CAdoRecordSet 类的Open方法. 返回的 Recordset 对象始
   终为只读、仅向前的游标.
==========================================================================*/
_RecordsetPtr CAdoConnection::Execute(LPCTSTR lpszSQL, long lOptions)
{
 ASSERT(m_pConnection != NULL);
 ASSERT(AfxIsValidString(lpszSQL));

 try
 {
  return m_pConnection->Execute(_bstr_t(lpszSQL), NULL, lOptions);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Execute 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Remarks: 请参考 CAdoRecordSet 类 Cancel 方法.
==========================================================================*/
BOOL CAdoConnection::Cancel()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return m_pConnection->Cancel();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Cancel 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name: 取得最后发生的错误信息.
==========================================================================*/
CString CAdoConnection::GetLastErrorText()
{
 ASSERT(m_pConnection != NULL);
 CString strErrors = "";
 try
 {
  if (m_pConnection != NULL)
  {
   ErrorsPtr pErrors = m_pConnection->Errors;
   CString strError;
   for (long n = 0; n < pErrors->Count; n++)
   {
    ErrorPtr pError = pErrors->GetItem(n);
    strError.Format(_T("Description: %s/r/nState: %s, Native: %d, Source: %s/r/n"),
        (LPCTSTR)pError->Description,
        (LPCTSTR)pError->SQLState,
           pError->NativeError,
        (LPCTSTR)pError->Source);
    strErrors += strError;
   }
  }
  return strErrors;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetLastError 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return strErrors;
 } 
 return strErrors;
}

ErrorsPtr CAdoConnection::GetErrors()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  if (m_pConnection != NULL)
  {
   return m_pConnection->Errors;
  }
  return NULL;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetErrors 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
 return NULL;
}

ErrorPtr CAdoConnection::GetError(long index)
{
 ASSERT(m_pConnection != NULL);
 try
 {
  if (m_pConnection != NULL)
  {
   ErrorsPtr pErrors =  m_pConnection->Errors;
   if (index >= 0 && index < pErrors->Count)
   {
    return pErrors->GetItem(_variant_t(index));
   }
  }
  return NULL;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetError 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
 return NULL;
}

/*========================================================================
 Name:  取得连接时间.
==========================================================================*/
long CAdoConnection::GetConnectTimeOut()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return m_pConnection->GetConnectionTimeout();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetConnectTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
 return -1;
}

/*========================================================================
 Name:  设置连接时间.
==========================================================================*/
BOOL CAdoConnection::SetConnectTimeOut(long lTime)
{
 ASSERT(m_pConnection != NULL);
 try
 {
  m_pConnection->PutConnectionTimeout(lTime);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetConnectTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

/*========================================================================
 Name:  取得默认数据库的名称.
==========================================================================*/
CString CAdoConnection::GetDefaultDatabase()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return CString(LPCTSTR(_bstr_t(m_pConnection->GetDefaultDatabase())));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetDefaultDatabase 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return CString(_T(""));
 } 
}

/*========================================================================
 Name:  取得 Connection 对象提供者的名称.
==========================================================================*/
CString CAdoConnection::GetProviderName()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return CString(LPCTSTR(_bstr_t(m_pConnection->GetProvider())));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetProviderName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return CString(_T(""));
 } 
}

/*========================================================================
 Name:  取得 ADO 的版本号
==========================================================================*/
CString CAdoConnection::GetVersion()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return CString(LPCTSTR(_bstr_t(m_pConnection->GetVersion())));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetVersion 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return CString(_T(""));
 } 
}

/*========================================================================
 Name:  取得对象的状态(同 Recordset 对象的 GetState 方法).
 -----------------------------------------------------
 returns: 返回下列常量之一的长整型值.
  [常量]    [说明] 
  ----------------------------------
  adStateClosed  指示对象是关闭的. 
  adStateOpen   指示对象是打开的. 
 -----------------------------------------------------
 Remarks:  可以随时使用 State 属性取得指定对象的当前状态.
==========================================================================*/
long CAdoConnection::GetState()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return m_pConnection->GetState();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetState 发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
 return -1;
}

/*========================================================================
 Name: 检测连接对象是否为打开状态.
==========================================================================*/
BOOL CAdoConnection::IsOpen()
{
 try
 {
  return (m_pConnection != NULL && (m_pConnection->State & adStateOpen));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: IsOpen 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name:  取得在 Connection 对象中修改数据的可用权限.
    ----------------------------------------------------------
 returns: 返回以下某个 ConnectModeEnum 的值.
  [常量]     [说明] 
  ----------------------------------
  adModeUnknown   默认值. 表明权限尚未设置或无法确定. 
  adModeRead    表明权限为只读. 
  adModeWrite    表明权限为只写. 
  adModeReadWrite   表明权限为读/写. 
  adModeShareDenyRead  防止其他用户使用读权限打开连接. 
  adModeShareDenyWrite 防止其他用户使用写权限打开连接. 
  adModeShareExclusive 防止其他用户打开连接. 
  adModeShareDenyNone  防止其他用户使用任何权限打开连接.
    ----------------------------------------------------------
 Remarks: 使用 Mode 属性可设置或返回当前连接上提供者正在使用的访问权限.
==========================================================================*/
ConnectModeEnum CAdoConnection::GetMode()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return m_pConnection->GetMode();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetMode 发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adModeUnknown;
 } 
 return adModeUnknown;
}

/*========================================================================
 Name:  设置在 Connection 中修改数据的可用权限. 请参考 GetMode 方法.
    ----------------------------------------------------------
 Remarks: 只能在关闭 Connection 对象时方可设置 Mode 属性.
==========================================================================*/
BOOL CAdoConnection::SetMode(ConnectModeEnum mode)
{
 ASSERT(m_pConnection != NULL);
 ASSERT(!IsOpen());

 try
 {
  m_pConnection->PutMode(mode);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetMode 发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return TRUE;
}

/*========================================================================
    Name:  从数据源获取数据库信息.
 -----------------------------------------------------
 Params:  QueryType  所要运行的模式查询类型.
 -----------------------------------------------------
 returns: 返回包含数据库信息的 Recordset 对象. Recordset 将以只读、静态
   游标打开.
==========================================================================*/
_RecordsetPtr CAdoConnection::OpenSchema(SchemaEnum QueryType)
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return m_pConnection->OpenSchema(QueryType, vtMissing, vtMissing);
 }
 catch(_com_error e)
 {
  TRACE(_T("Warning: OpenSchema方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
 return NULL;
}

/*########################################################################
     ------------------------------------------------
         事务处理
     ------------------------------------------------
  ########################################################################*/

/*========================================================================
    Name:  开始新事务.
 -----------------------------------------------------
 returns: 对于支持嵌套事务的数据库来说, 在已打开的事务中调用 BeginTrans 
 方法将开始新的嵌套事务. 返回值将指示嵌套层次: 返回值为 1 表示已打开顶层
 事务 (即事务不被另一个事务所嵌套), 返回值为 2 表示已打开第二层事务(嵌套
 在顶层事务中的事务), 依次类推.
 -----------------------------------------------------
 Remarks: 一旦调用了 BeginTrans 方法, 在调用 CommitTrans 或 RollbackTrans
 结束事务之前, 数据库将不再立即提交所作的任何更改.

==========================================================================*/
long CAdoConnection::BeginTrans()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return m_pConnection->BeginTrans();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: BeginTrans 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
 return -1;
}

/*========================================================================
    Name:  保存任何更改并结束当前事务.
 -----------------------------------------------------
 Remarks: 调用 CommitTrans 或 RollbackTrans 只影响最新打开的事务; 在
 处理任何更高层事务之前必须关闭或回卷当前事务.
==========================================================================*/
BOOL CAdoConnection::CommitTrans()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return SUCCEEDED(m_pConnection->CommitTrans());
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: CommitTrans 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
    Name:  取消当前事务中所作的任何更改并结束事务.
==========================================================================*/
BOOL CAdoConnection::RollbackTrans()
{
 ASSERT(m_pConnection != NULL);
 try
 {
  return SUCCEEDED(m_pConnection->RollbackTrans());
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: RollbackTrans 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*########################################################################
 【文件名】:  ado.h
 【名  称】:  ADO 封装类.
 【版  本】:  0.20
 【作  者】:  成真
 【E-mail】:  [anyou@sina.com](mailto:anyou@sina.com)
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADORECORDSET_H)
#define _ANYOU_COOL_ADORECORDSET_H
#include "Ado.h"

/*########################################################################
    ------------------------------------------------
        CAdoRecordSet class
    ------------------------------------------------
  ########################################################################*/

class CAdoRecordSet 
{
// 构建/折构 --------------------------------------------
public:
 CAdoRecordSet();
 CAdoRecordSet(CAdoConnection *pConnection);

 virtual ~CAdoRecordSet();

protected:
 void Release();

// 属性 ------------------------------------------------ 
public:
 // 当前编辑状态 ----------------------------
 EditModeEnum GetEditMode();

 // 当前状态 --------------------------------
 BOOL IsEOF();
 BOOL IsBOF();
 BOOL IsOpen();
 long GetState();
 long GetStatus();

 // 充许返回的最大记录数 --------------------
 long GetMaxRecordCount();
 BOOL SetMaxRecordCount(long count);

 // 光标位置 --------------------------------
 CursorLocationEnum GetCursorLocation();
 BOOL SetCursorLocation(CursorLocationEnum CursorLocation = adUseClient);

 // 光标类型 --------------------------------
 CursorTypeEnum GetCursorType();
 BOOL SetCursorType(CursorTypeEnum CursorType = adOpenStatic);

 // 书签 --------------------------------
 _variant_t GetBookmark();
 BOOL SetBookmark(_variant_t varBookMark = _variant_t((long)adBookmarkFirst));

 // 当前记录位置 ------------------------
 long GetAbsolutePosition();
 BOOL SetAbsolutePosition(int nPosition);

 long GetAbsolutePage();
 BOOL SetAbsolutePage(int nPage);

 // 每页的记录数 ------------------------
 long GetPageSize();
 BOOL SetCacheSize(const long& lCacheSize); 

 // 页数 --------------------------------
 long GetPageCount();

 // 记录数及字段数 ----------------------
 long GetRecordCount();
 long GetFieldsCount();

 // 查询字符串 --------------------------
 CString GetSQLText() {return m_strSQL;}
 void SetSQLText(LPCTSTR strSQL) {m_strSQL = strSQL;}

 // 连接对象 ----------------------------
 CAdoConnection* GetConnection() {return m_pConnection;}
 void SetAdoConnection(CAdoConnection *pConnection);

 // 记录集对象 --------------------------
 _RecordsetPtr& GetRecordset();

 CString GetLastError();

// 字段属性 ----------------------------------------------
public:
 // 字段集 -------------------------------
 FieldsPtr GetFields();

 // 字段对象 -----------------------------
 FieldPtr  GetField(long lIndex);
 FieldPtr  GetField(LPCTSTR lpszFieldName);

 // 字段名 -------------------------------
 CString GetFieldName(long lIndex);

 // 字段数据类型 -------------------------
 DataTypeEnum GetFieldType(long lIndex);
 DataTypeEnum GetFieldType(LPCTSTR lpszFieldName);

 // 字段属性 -----------------------------
 long  GetFieldAttributes(long lIndex);
 long  GetFieldAttributes(LPCTSTR lpszFieldName);

 // 字段定义长度 -------------------------
 long  GetFieldDefineSize(long lIndex);
 long  GetFieldDefineSize(LPCTSTR lpszFieldName);

 // 字段实际长度 -------------------------
 long  GetFieldActualSize(long lIndex);
 long  GetFieldActualSize(LPCTSTR lpszFieldName);

 // 字段是否为NULL -----------------------
 BOOL  IsFieldNull(long index);
 BOOL  IsFieldNull(LPCTSTR lpFieldName);

// 记录更改 --------------------------------------------
public:
 BOOL AddNew();
 BOOL Update();
 BOOL UpdateBatch(AffectEnum AffectRecords = adAffectAll); 
 BOOL CancelUpdate();
 BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
 BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);

 // 刷新记录集中的数据 ------------------
 BOOL Requery(long Options = adConnectUnspecified);
 BOOL Resync(AffectEnum AffectRecords = adAffectAll, ResyncEnum ResyncValues = adResyncAllValues);   

 BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
 BOOL AddNew(CADORecordBinding &pAdoRecordBinding);

// 记录集导航操作 --------------------------------------
public:
 BOOL MoveFirst();
 BOOL MovePrevious();
 BOOL MoveNext();
 BOOL MoveLast();
 BOOL Move(long lRecords, _variant_t Start = _variant_t((long)adBookmarkFirst));

 // 查找指定的记录 ----------------------
 BOOL Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection = adSearchForward);
 BOOL FindNext();

// 查询 ------------------------------------------------
public:
 BOOL Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
    void OpenA();
 BOOL Cancel();
 void Close();

 // 保存/载入持久性文件 -----------------
 BOOL Save(LPCTSTR strFileName = _T(""), PersistFormatEnum PersistFormat = adPersistXML);
 BOOL Load(LPCTSTR strFileName);

// 字段存取 --------------------------------------------
public:
 BOOL PutCollect(long index, const _variant_t &value);
 BOOL PutCollect(long index, const CString &value);
 BOOL PutCollect(long index, const double &value);
 BOOL PutCollect(long index, const float  &value);
 BOOL PutCollect(long index, const long   &value);
 BOOL PutCollect(long index, const DWORD  &value);
 BOOL PutCollect(long index, const int    &value);
 BOOL PutCollect(long index, const short  &value);
 BOOL PutCollect(long index, const BYTE   &value);
 BOOL PutCollect(long index, const bool   &value);
 BOOL PutCollect(long index, const COleDateTime &value);
 BOOL PutCollect(long index, const COleCurrency &value);

 BOOL PutCollect(LPCTSTR strFieldName, const _variant_t &value);
 BOOL PutCollect(LPCTSTR strFieldName, const CString &value);
 BOOL PutCollect(LPCTSTR strFieldName, const double &value);
 BOOL PutCollect(LPCTSTR strFieldName, const float  &value);
 BOOL PutCollect(LPCTSTR strFieldName, const long   &value);
 BOOL PutCollect(LPCTSTR strFieldName, const DWORD  &value);
 BOOL PutCollect(LPCTSTR strFieldName, const int    &value);
 BOOL PutCollect(LPCTSTR strFieldName, const short  &value);
 BOOL PutCollect(LPCTSTR strFieldName, const BYTE   &value);
 BOOL PutCollect(LPCTSTR strFieldName, const bool   &value);
 BOOL PutCollect(LPCTSTR strFieldName, const COleDateTime &value);
 BOOL PutCollect(LPCTSTR strFieldName, const COleCurrency &value);

 // ---------------------------------------------------------

 BOOL GetCollect(long index, CString &value);
 BOOL GetCollect(long index, double  &value);
 BOOL GetCollect(long index, float   &value);
 BOOL GetCollect(long index, long    &value);
 BOOL GetCollect(long index, DWORD   &value);
 BOOL GetCollect(long index, int     &value);
 BOOL GetCollect(long index, short   &value);
 BOOL GetCollect(long index, BYTE    &value);
 BOOL GetCollect(long index, bool   &value);
 BOOL GetCollect(long index, COleDateTime &value);
 BOOL GetCollect(long index, COleCurrency &value);

 BOOL GetCollect(LPCSTR strFieldName, CString &strValue);
 BOOL GetCollect(LPCSTR strFieldName, double &value);
 BOOL GetCollect(LPCSTR strFieldName, float  &value);
 BOOL GetCollect(LPCSTR strFieldName, long   &value);
 BOOL GetCollect(LPCSTR strFieldName, DWORD  &value);
 BOOL GetCollect(LPCSTR strFieldName, int    &value);
 BOOL GetCollect(LPCSTR strFieldName, short  &value);
 BOOL GetCollect(LPCSTR strFieldName, BYTE   &value);
 BOOL GetCollect(LPCSTR strFieldName, bool   &value);
 BOOL GetCollect(LPCSTR strFieldName, COleDateTime &value);
 BOOL GetCollect(LPCSTR strFieldName, COleCurrency &value);

 // BLOB 数据存取 ------------------------------------------
 BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
 BOOL AppendChunk(long index, LPVOID lpData, UINT nBytes);
 BOOL AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes);
 BOOL AppendChunk(long index, LPCTSTR lpszFileName);
 BOOL AppendChunk(LPCSTR strFieldName, LPCTSTR lpszFileName);

 BOOL GetChunk(FieldPtr pField, LPVOID lpData);
 BOOL GetChunk(long index, LPVOID lpData);
 BOOL GetChunk(LPCSTR strFieldName, LPVOID lpData);
 BOOL GetChunk(long index, CBitmap &bitmap);
 BOOL GetChunk(LPCSTR strFieldName, CBitmap &bitmap);

// 其他方法 --------------------------------------------
public:
 // 过滤记录 ---------------------------------
 BOOL SetFilter(LPCTSTR lpszFilter);

 // 排序 -------------------------------------
 BOOL SetSort(LPCTSTR lpszCriteria);

 // 测试是否支持某方法 -----------------------
 BOOL Supports(CursorOptionEnum CursorOptions = adAddNew);

 // 克隆 -------------------------------------
 BOOL Clone(CAdoRecordSet &pRecordSet);
 _RecordsetPtr operator = (_RecordsetPtr &pRecordSet);

 // 格式化 _variant_t 类型值 -----------------

//成员变量 --------------------------------------------
 _RecordsetPtr  m_pRecordset;
protected:
 CAdoConnection     *m_pConnection;
 CString    m_strSQL;
 CString    m_strFind;
 CString    m_strFileName;
 IADORecordBinding *m_pAdoRecordBinding;
 SearchDirectionEnum m_SearchDirection;
public:
 _variant_t   m_varBookmark;
};
//________________________________________________________________________

#endif //_ANYOU_COOL_ADORECORDSET_H

/*########################################################################
 Filename:  adorecordset.cpp
 ----------------------------------------------------
 Remarks: ...
 ----------------------------------------------------
 Author:  成真
 Email:  [anyou@sina.com](mailto:anyou@sina.com)
[anyou@msn.com](mailto:anyou@msn.com)
 Created: 29/4/2003 8:15
  ########################################################################*/
#include "stdafx.h"
#include "AdoRecordSet.h"
#include <math.h>
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*########################################################################
    ------------------------------------------------
     CAdoRecordSet class 构造/析构函数
    ------------------------------------------------
  ########################################################################*/

CAdoRecordSet::CAdoRecordSet()
{
 m_pConnection = NULL;
 m_SearchDirection = adSearchForward;
 m_pRecordset.CreateInstance("ADODB.Recordset");
 #ifdef _DEBUG
 if (m_pRecordset == NULL)
 {
  AfxMessageBox("RecordSet 对象创建失败! 请确认是否初始化了COM环境.");
 }
 #endif
 ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::CAdoRecordSet(CAdoConnection *pConnection)
{
 m_SearchDirection = adSearchForward;
 m_pConnection = pConnection;
 ASSERT(m_pConnection != NULL);
 m_pRecordset.CreateInstance("ADODB.Recordset");
 #ifdef _DEBUG
 if (m_pRecordset == NULL)
 {
  AfxMessageBox("RecordSet 对象创建失败! 请确认是否初始化了COM环境.");
 }
 #endif
 ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::~CAdoRecordSet()
{
 Release();
}

/*========================================================================
 Params:  
  - strSQL:  SQL语句, 表名, 存储过程调用或持久 Recordset 文件名.
  - CursorType:   可选. CursorTypeEnum 值, 确定打开 Recordset 时应该
     使用的游标类型. 可为下列常量之一.
  [常量]    [说明] 
  -----------------------------------------------
  adOpenForwardOnly 打开仅向前类型游标. 
  adOpenKeyset  打开键集类型游标. 
  adOpenDynamic  打开动态类型游标. 
  adOpenStatic  打开静态类型游标. 
  -----------------------------------------------
  - LockType:     可选, 确定打开 Recordset 时应该使用的锁定类型(并发)
     的 LockTypeEnum 值, 可为下列常量之一.
  [常量]    [说明] 
  -----------------------------------------------
  adLockReadOnly  只读 - 不能改变数据. 
  adLockPessimistic 保守式锁定 - 通常通过在编辑时立即锁定数据源的记录. 
  adLockOptimistic 开放式锁定 - 只在调用 Update 方法时才锁定记录. 
  adLockBatchOptimistic 开放式批更新 - 用于批更新模式(与立即更新模式
      相对). 
  -----------------------------------------------
  - lOption  可选. 长整型值, 用于指示 strSQL 参数的类型. 可为下
     列常量之一.
  [常量]    [说明] 
  -------------------------------------------------
  adCmdText   指示strSQL为命令文本, 即普通的SQL语句. 
  adCmdTable   指示ADO生成SQL查询返回以 strSQL 命名的表中的
      所有行. 
  adCmdTableDirect 指示所作的更改在strSQL中命名的表中返回所有行. 
  adCmdStoredProc  指示strSQL为存储过程. 
  adCmdUnknown  指示strSQL参数中的命令类型为未知. 
  adCmdFile   指示应从在strSQL中命名的文件中恢复保留(保存的)
      Recordset. 
  adAsyncExecute  指示应异步执行strSQL. 
  adAsyncFetch  指示在提取 Initial Fetch Size 属性中指定的初始
      数量后, 应该异步提取所有剩余的行. 如果所需的行尚未
      提取, 主要的线程将被堵塞直到行重新可用. 
  adAsyncFetchNonBlocking 指示主要线程在提取期间从未堵塞. 如果所请求
      的行尚未提取, 当前行自动移到文件末尾. 
==========================================================================*/
void CAdoRecordSet::OpenA(){}
BOOL CAdoRecordSet::Open(LPCTSTR strSQL, long lOption, CursorTypeEnum CursorType, LockTypeEnum LockType)
{
 ASSERT(m_pConnection != NULL);
 ASSERT(m_pRecordset != NULL);
 ASSERT(AfxIsValidString(strSQL));

 if(strcmp(strSQL, _T("")) != 0)
 {
  m_strSQL = strSQL;
 }
 if (m_pConnection == NULL || m_pRecordset == NULL)
 {
  return FALSE;
 }

 if (m_strSQL.IsEmpty())
 {
  ASSERT(FALSE);
  return FALSE;
 }

 try
 {
  if (IsOpen()) Close();
  return SUCCEEDED(m_pRecordset->Open(_variant_t(LPCTSTR(m_strSQL)), 
           _variant_t((IDispatch*)m_pConnection->GetConnection(), true),
           CursorType, LockType, lOption));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: 打开记录集发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  TRACE(_T("%s/r/n"), GetLastError());
  return FALSE;
 }
}

/*========================================================================
 Name:  通过重新执行对象所基于的查询, 更新 Recordset 对象中的数据.
    ----------------------------------------------------------
 Params:  Options   可选. 指示影响该操作选项的位屏蔽. 如果该参数设置
  为 adAsyncExecute, 则该操作将异步执行并在它结束时产生 
  RecordsetChangeComplete 事件
    ----------------------------------------------------------
 Remarks: 通过重新发出原始命令并再次检索数据, 可使用 Requery 方法刷新
 来自数据源的 Recordset 对象的全部内容. 调用该方法等于相继调用 Close 和 
 Open 方法. 如果正在编辑当前记录或者添加新记录将产生错误.
==========================================================================*/
BOOL CAdoRecordSet::Requery(long Options)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return (m_pRecordset->Requery(Options) == S_OK);
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Requery 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE; 
}

/*========================================================================
 Name:  从基本数据库刷新当前 Recordset 对象中的数据. 
    ----------------------------------------------------------
 Params:  AffectRecords:   可选, AffectEnum 值, 决定 Resync 方法所影
  响的记录数目, 可以为下列常量之一.
  [常量]    [说明]
  ------------------------------------
  adAffectCurrent  只刷新当前记录. 
  adAffectGroup  刷新满足当前 Filter 属性设置的记录.只有将 Filter
      属性设置为有效预定义常量之一才能使用该选项. 
  adAffectAll   默认值.刷新 Recordset 对象中的所有记录, 包括由
      于当前 Filter 属性设置而隐藏的记录. 
  adAffectAllChapters 刷新所有子集记录. 

  ResyncValues:   可选, ResyncEnum 值. 指定是否覆盖基本值. 可为下列
     常量之一.
  [常量]    [说明] 
  ------------------------------------
  adResyncAllValues 默认值. 覆盖数据, 取消挂起的更新. 
  adResyncUnderlyingValues 不覆盖数据, 不取消挂起的更新. 
    ----------------------------------------------------------
 Remarks: 使用 Resync 方法将当前 Recordset 中的记录与基本的数据库重新
 同步. 这在使用静态或仅向前的游标但希望看到基本数据库中的改动时十分有用.
 如果将 CursorLocation 属性设置为 adUseClient, 则 Resync 仅对非只读的 
 Recordset 对象可用.
 与 Requery 方法不同, Resync 方法不重新执行 Recordset 对象的基本的命令, 
 基本的数据库中的新记录将不可见.
==========================================================================*/
BOOL CAdoRecordSet::Resync(AffectEnum AffectRecords, ResyncEnum ResyncValues)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return (m_pRecordset->Resync(AffectRecords, ResyncValues) == S_OK);
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Resync 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE; 
}

/*========================================================================
 Name:  将 Recordset 保存在持久性文件中.
    ----------------------------------------------------------
 Params:  
 [strFileName]:   可选. 文件的完整路径名, 用于保存 Recordset.
 [PersistFormat]:   可选. PersistFormatEnum 值, 指定保存 Recordset 所使
  用的格式. 可以是如下的某个常量: 
  [常量]   [说明] 
  ------------------------------
  adPersistADTG 使用专用的"Advanced Data Tablegram"格式保存. 
  adPersistXML (默认)使用 XML 格式保存. 
    ----------------------------------------------------------
 Remarks: 只能对打开的 Recordset 调用 Save 方法. 随后使用 Load 方法可
 以从文件中恢复 Recordset. 如果 Filter 属性影响 Recordset, 将只保存经过
 筛选的行.
  在第一次保存 Recordset 时指定 FileName. 如果随后调用 Save 时, 应忽
 略 FileName, 否则将产生运行时错误. 如果随后使用新的 FileName 调用 Save, 
 那么 Recordset 将保存到新的文件中, 但新文件和原始文件都是打开的.
==========================================================================*/
BOOL CAdoRecordSet::Save(LPCTSTR strFileName, PersistFormatEnum PersistFormat)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(IsOpen());

 if (m_strFileName == strFileName)
 {
  strFileName = NULL;
 }
 else if(_taccess(strFileName, 0) != -1)
 {
  DeleteFile(strFileName);
  m_strFileName = strFileName;
 }
 else
 {
  m_strFileName = strFileName;
 }

 try
 {
  if (m_pRecordset != NULL) 
  {
   return (m_pRecordset->Save(_bstr_t(strFileName), PersistFormat) == S_OK);
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Save 发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

BOOL CAdoRecordSet::Load(LPCTSTR strFileName)
{
 if (IsOpen()) Close();

 try
 {
  return (m_pRecordset->Open(strFileName, "Provider=MSPersist;", adOpenForwardOnly, adLockOptimistic, adCmdFile) == S_OK);
 }
 catch (_com_error &e)
 {
  TRACE(_T("Warning: Load 发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 Name:  取消执行挂起的异步 Execute 或 Open 方法的调用.
 -----------------------------------------------------
 Remarks: 使用 Cancel 方法终止执行异步 Execute 或 Open 方法调用(即通
  过 adAsyncConnect、adAsyncExecute 或 adAsyncFetch 参数调用的方法).
  如果在试图终止的方法中没有使用 adAsyncExecute, 则 Cancel 将返回运行
  时错误.
==========================================================================*/
BOOL CAdoRecordSet::Cancel()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->Cancel();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Cancel发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name:  关闭打开的对象及任何相关对象.
 -----------------------------------------------------
 Remarks: 使用 Close 方法可关闭 Recordset 对象以便释放所有关联的系统
  资源. 关闭对象并非将它从内存中删除, 可以更改它的属性设置并且在此后
  再次打开. 要将对象从内存中完全删除, 可将对象变量设置为 NULL.
   如果正在立即更新模式下进行编辑, 调用Close方法将产生错误,应首先
  调用 Update 或 CancelUpdat 方法. 如果在批更新期间关闭 Recordset 对
  象, 则自上次 UpdateBatch 调用以来所做的修改将全部丢失.
   如果使用 Clone 方法创建已打开的 Recordset 对象的副本, 关闭原始
  Recordset或其副本将不影响任何其他副本.
==========================================================================*/
void CAdoRecordSet::Close()
{
 try
 {
  if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
  {
   if (GetEditMode() == adEditNone) 
    CancelUpdate();
   m_pRecordset->Close();
  }
 }
 catch (const _com_error& e)
 {
  TRACE(_T("Warning: 关闭记录集发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 }
}

/*========================================================================
 Name: 关闭连接并释放对象.
 -----------------------------------------------------
 Remarks: 关闭连接并释放CAdoRecordSet对象, 这样基本上从内容中完全清除了
  CAdoRecordSet对象.
==========================================================================*/
void CAdoRecordSet::Release()
{
 if (IsOpen()) Close();
 m_pRecordset.Release();
 m_pRecordset = NULL;
}

/*########################################################################
     ------------------------------------------------
          记录集更新操作
     ------------------------------------------------
  ########################################################################*/

/*========================================================================
 Remarks: 开始添加新的纪录. 
在批更新模式（提供者缓存多个更改并只在调用 UpdateBatch 时将其写入基本数据源）下，
调用不带参数的 AddNew 方法可将 EditMode 属性设置为 adEditAdd。提供者将任何字段值
的更改缓存在本地。调用 Update 方法可将新的记录添加到当前记录集并将 EditMode 属性
重置为 adEditNone，但在调用 UpdateBatch 方法之前提供者不将更改传递到基本数据库。
如果传送 Fields 和 Values 参数，ADO 则将新记录发送给提供者以便缓存；需要调用 
UpdateBatch 方法将新记录传递到基本数据库

在立即更新模式（调用 Update 方法时提供者会立即将更改写入基本数据源）下，调用不带参数的 AddNew 
方法可将 EditMode 属性设置为 adEditAdd。提供者将任何字段值的更改缓存在本地。调用 Update 方法可
将新记录传递到数据库并将 EditMode 属性重置为 adEditNone。如果传送了 Fields 和 Values 参数，ADO
 则立即将新记录传递到数据库（无须调用 Update），且 EditMode 属性值没有改变 (adEditNone)。
==========================================================================*/
BOOL CAdoRecordSet::AddNew()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   if (m_pRecordset->AddNew() == S_OK)
   {
    return TRUE;
   }
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: AddNew 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Remarks: 在调用 AddNew 等方法后, 调用此方法完成更新或修改.
==========================================================================*/
BOOL CAdoRecordSet::Update()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   if (m_pRecordset->Update() == S_OK)
   {
    return TRUE;
   }
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Update 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 }

 CancelUpdate();
 return FALSE;
}

/*========================================================================
 Name:  将所有挂起的批更新写入磁盘.
    ----------------------------------------------------------
 Params:  AffectRecords   可选, AffectEnum 值. 决定 UpdateBatch 方法
  所影响的记录数目.可以为如下常量之一.
  [常量]    [说明] 
  ------------------------------------
  adAffectCurrent  只写入当前记录的挂起更改. 
  adAffectGroup  写入满足当前 Filter 属性设置的记录所发生的挂起
   更改. 必须将 Filter 属性设置为某个有效的预定义常量才能使用该选项. 
  adAffectAll   (默认值). 写入 Recordset 对象中所有记录的挂起
      更改, 包括由于当前 Filter 属性设置而隐藏的任何记录. 
  adAffectAllChapters 写入所有子集的挂起更改. 
    ----------------------------------------------------------
 Remarks: 按批更新模式修改 Recordset 对象时, 使用 UpdateBatch 方法可
 将 Recordset 对象中的所有更改传递到基本数据库.
 如果 Recordset 对象支持批更新, 那么可以将一个或多个记录的多重更改缓存在
 本地, 然后再调用 UpdateBatch 方法. 如果在调用 UpdateBatch 方法时正在编
 辑当前记录或者添加新的记录, 那么在将批更新传送到提供者之前, ADO 将自动
 调用 Update 方法保存对当前记录的所有挂起更改.
    只能对键集或静态游标使用批更新.
==========================================================================*/
BOOL CAdoRecordSet::UpdateBatch(AffectEnum AffectRecords)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return (m_pRecordset->UpdateBatch(AffectRecords) == S_OK);
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: UpdateBatch 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE; 
}

/*========================================================================
 Name:  取消在调用 Update 方法前对当前记录或新记录所作的任何更改.
 -----------------------------------------------------
 Remarks: 使用 CancelUpdate 方法可取消对当前记录所作的任何更改或放弃
 新添加的记录. 在调用 Update 方法后将无法撤消对当前记录或新记录所做的更
 改, 除非更改是可以用 RollbackTrans 方法回卷的事务的一部分, 或者是可以
 用 CancelBatch 方法取消的批更新的一部分.
  如果在调用 CancelUpdate 方法时添加新记录, 则调用 AddNew 之前的当前
 记录将再次成为当前记录.
  如果尚未更改当前记录或添加新记录, 调用 CancelUpdate 方法将产生错误.
==========================================================================*/
BOOL CAdoRecordSet::CancelUpdate()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   if (GetEditMode() == adEditNone || m_pRecordset->CancelUpdate() == S_OK)
   {
    return TRUE;
   }
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: CancelUpdate 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name:  取消挂起的批更新.
 -----------------------------------------------------
 Params:  AffectRecords   可选的 AffectEnum 值, 决定CancelBatch 方法
  所影响记录的数目, 可为下列常量之一: 
  [常量]   [说明] 
  -------------------------------------------------
  AdAffectCurrent 仅取消当前记录的挂起更新. 
  AdAffectGroup 对满足当前 Filter 属性设置的记录取消挂起更新.
      使用该选项时,必须将 Filter 属性设置为合法的预
      定义常量之一. 
  AdAffectAll  默认值.取消 Recordset 对象中所有记录的挂起更
      新,包括由当前 Filter 属性设置所隐藏的任何记录. 
==========================================================================*/
BOOL CAdoRecordSet::CancelBatch(AffectEnum AffectRecords)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return (m_pRecordset->CancelBatch(AffectRecords) == S_OK);
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: CancelBatch 发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Params:   AffectRecords:  AffectEnum 值, 确定 Delete 方法所影响的记
  录数目, 该值可以是下列常量之一.
  [常量]    [说明 ]
  -------------------------------------------------
  adAffectCurrent  默认. 仅删除当前记录. 
  adAffectGroup  删除满足当前 Filter 属性设置的记录. 要使用该选
      项, 必须将 Filter 属性设置为有效的预定义常量之一. 
  adAffectAll   删除所有记录. 
  adAffectAllChapters 删除所有子集记录. 
==========================================================================*/
BOOL CAdoRecordSet::Delete(AffectEnum AffectRecords)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return (m_pRecordset->Delete(AffectRecords) == S_OK);
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Delete发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*########################################################################
     ------------------------------------------------
        记录集导航操作
     ------------------------------------------------
  ########################################################################*/

/*========================================================================
 Name:  将当前记录位置移动到 Recordse 中的第一个记录.
==========================================================================*/
BOOL CAdoRecordSet::MoveFirst()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return SUCCEEDED(m_pRecordset->MoveFirst());
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: MoveFirst 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name:  将当前记录位置移动到 Recordset 中的最后一个记录.
 -----------------------------------------------------
 Remarks: Recordset 对象必须支持书签或向后光标移动; 否则调用该方法将
   产生错误.
==========================================================================*/
BOOL CAdoRecordSet::MoveLast()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return SUCCEEDED(m_pRecordset->MoveLast());
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: MoveLast 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name:  将当前记录位置向后移动一个记录(向记录集的顶部).
 -----------------------------------------------------
 Remarks: Recordset 对象必须支持书签或向后游标移动; 否则方法调用将产
 生错误.如果首记录是当前记录并且调用 MovePrevious 方法, 则 ADO 将当前记
 录设置在 Recordset (BOF为True) 的首记录之前. 而BOF属性为 True 时向后移
 动将产生错误. 如果 Recordse 对象不支持书签或向后游标移动, 则 MovePrevious 
 方法将产生错误.
==========================================================================*/
BOOL CAdoRecordSet::MovePrevious()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return SUCCEEDED(m_pRecordset->MovePrevious());
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: MovePrevious 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
 return FALSE; 
}

/*========================================================================
 Name:  将当前记录向前移动一个记录(向 Recordset 的底部).
 -----------------------------------------------------
 Remarks: 如果最后一个记录是当前记录并且调用 MoveNext 方法, 则 ADO 将
 当前记录设置到 Recordset (EOF为 True)的尾记录之后. 当 EOF 属性已经为 
 True 时试图向前移动将产生错误.
==========================================================================*/
BOOL CAdoRecordSet::MoveNext()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return SUCCEEDED(m_pRecordset->MoveNext());
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: MoveNext 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
 return FALSE;
}

/*========================================================================
 Name:  移动 Recordset 对象中当前记录的位置.
    ----------------------------------------------------------
 Params:  
  - lRecords    带符号长整型表达式, 指定当前记录位置移动的记录数.
  - Start    可选, 字符串或变体型, 用于计算书签. 也可为下列 
  BookmarkEnum 值之一: 
  [常量]    [说明] 
  --------------------------------
  adBookmarkCurrent 默认. 从当前记录开始. 
  adBookmarkFirst  从首记录开始. 
  adBookmarkLast  从尾记录开始. 
==========================================================================*/
BOOL CAdoRecordSet::Move(long lRecords, _variant_t Start)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return SUCCEEDED(m_pRecordset->Move(lRecords, _variant_t(Start)));
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Move 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*########################################################################
     ------------------------------------------------
          记录集属性
     ------------------------------------------------
  ########################################################################*/

/*========================================================================
 Name:  取得记录集对象的状态(是打开状态还是关闭状态). 对异步方式执
 行的 Recordset 对象, 则说明当前的对象状态是连接、执行还是获取状态.
 -----------------------------------------------------
 returns: 返回下列常量之一的长整型值.
  [常量]    [说明] 
  ----------------------------------
  adStateClosed  指示对象是关闭的. 
  adStateOpen   指示对象是打开的. 
  adStateConnecting 指示 Recordset 对象正在连接. 
  adStateExecuting 指示 Recordset 对象正在执行命令. 
  adStateFetching  指示 Recordset 对象的行正在被读取. 
 -----------------------------------------------------
 Remarks:  可以随时使用 State 属性确定指定对象的当前状态. 该属性是只
 读的. Recordset 对象的 State 属性可以是组合值. 例如: 如果正在执行语句,
 该属性将是 adStateOpen 和 adStateExecuting 的组合值.
==========================================================================*/
long CAdoRecordSet::GetState()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->GetState();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetState 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
 return -1;
}

BOOL CAdoRecordSet::IsOpen()
{
 try
 {
  return (m_pRecordset != NULL && (GetState() & adStateOpen));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: IsOpen方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name:  指示有关批更新或其他大量操作的当前记录的状态.
 -----------------------------------------------------
 returns: 返回下列一个或多个 RecordStatusEnum 值之和.
  [常量]      [说明]
  -------------------------------------------------
  adRecOK      成功地更新记录. 
  adRecNew     记录是新建的. 
  adRecModified    记录被修改. 
  adRecDeleted    记录被删除. 
  adRecUnmodified    记录没有修改. 
  adRecInvalid    由于书签无效, 记录没有保存. 
  adRecMultipleChanges  由于影响多个记录, 因此记录未被保存. 
  adRecPendingChanges   由于记录引用挂起的插入, 因此未被保存. 
  adRecCanceled    由于操作被取消, 未保存记录. 
  adRecCantRelease   由于现有记录锁定, 没有保存新记录. 
  adRecConcurrencyViolation 由于开放式并发在使用中, 记录未被保存. 
  adRecIntegrityViolation  由于用户违反完整性约束, 记录未被保存. 
  adRecMaxChangesExceeded  由于存在过多挂起更改, 记录未被保存. 
  adRecObjectOpen    由于与打开的储存对象冲突, 记录未被保存. 
  adRecOutOfMemory   由于计算机内存不足, 记录未被保存. 
  adRecPermissionDenied  由于用户没有足够的权限, 记录未被保存. 
  adRecSchemaViolation  由于记录违反基本数据库的结构, 因此未被保存. 
  adRecDBDeleted    记录已经从数据源中删除. 
 -----------------------------------------------------
 Remarks: 使用 Status 属性查看在批更新中被修改的记录有哪些更改被挂起.
 也可使用 Status 属性查看大量操作时失败记录的状态. 例如, 调用 Recordset
 对象的 Resync、UpdateBatch 或 CancelBatch 方法, 或者设置 Recordset 对象
 的 Filter 属性为书签数组. 使用该属性, 可检查指定记录为何失败并将问题解
 决.
==========================================================================*/
long CAdoRecordSet::GetStatus()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->GetStatus();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetStatus 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
 return -1;
}

/*========================================================================
 Name:  获取当前记录集中记录数目
==========================================================================*/
long CAdoRecordSet::GetRecordCount()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  long count = m_pRecordset->GetRecordCount();

  // 如果ado不支持此属性，则手工计算记录数目 --------
  if (count < 0)
  {
   long pos = GetAbsolutePosition();
   MoveFirst();
   count = 0;
   while (!IsEOF()) 
   {
    count++;
    MoveNext();
   }
   SetAbsolutePosition(pos);
  }
  return count;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetRecordCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
}

/*========================================================================
 Name:  获取当前记录集中字段数目
==========================================================================*/
long CAdoRecordSet::GetFieldsCount()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return GetFields()->Count;
 }
 catch(_com_error e)
 {
  TRACE(_T("Warning: GetFieldsCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
}
/*========================================================================
 Name:  指示通过查询返回 Recordset 的记录的最大数目. 
==========================================================================*/
long CAdoRecordSet::GetMaxRecordCount()
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  return m_pRecordset->GetMaxRecords();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetMaxRecordCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}
BOOL CAdoRecordSet::SetMaxRecordCount(long count)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  m_pRecordset->PutMaxRecords(count);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetMaxRecordCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 Name:  指针是否在在记录集头
==========================================================================*/
BOOL CAdoRecordSet::IsBOF()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->adoBOF;
 }
 catch(_com_error e)
 {
  TRACE(_T("Warning: IsBOF 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

/*========================================================================
 Name:  指针是否在在记录集尾
==========================================================================*/
BOOL CAdoRecordSet::IsEOF()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->adoEOF;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: IsEOF 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

EditModeEnum CAdoRecordSet::GetEditMode()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   return m_pRecordset->GetEditMode();
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetEditMode 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adEditNone;
 } 
 return adEditNone; 
}

long CAdoRecordSet::GetPageCount()
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  return m_pRecordset->GetPageCount();
 }
 catch (_com_error &e)
 {
  TRACE(_T("Warning: GetPageCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

BOOL CAdoRecordSet::SetCacheSize(const long &lCacheSize)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL && !(GetState() & adStateExecuting))
  {
   m_pRecordset->PutCacheSize(lCacheSize);
  }
 }
 catch (const _com_error& e)
 {
  TRACE(_T("Warning: SetCacheSize方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
 return TRUE;
}

long CAdoRecordSet::GetPageSize()
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  return m_pRecordset->GetPageSize();
 }
 catch (_com_error &e)
 {
  TRACE(_T("Warning: GetPageCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

/*========================================================================
 name:  指定当前记录所在的页.
    ----------------------------------------------------------
 returns: 置或返回从 1 到 Recordset 对象 (PageCount) 所含页数的长整型
   值，或者返回以下常量. 
 [常量]   [说明]
 ---------------------------------
 adPosUnknown Recordset 为空，当前位置未知，或者提供者不支持 AbsolutePage 属性.  
 adPosBOF  当前记录指针位于 BOF(即 BOF 属性为 True).  
 adPosEOF  当前记录指针位于 EOF(即 EOF 属性为 True).  
==========================================================================*/
BOOL CAdoRecordSet::SetAbsolutePage(int nPage)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);  
  return TRUE;
 }
 catch(_com_error &e)
 {
  TRACE(_T("Warning: SetAbsolutePage 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

long CAdoRecordSet::GetAbsolutePage()
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  return m_pRecordset->GetAbsolutePage();
 }
 catch(_com_error &e)
 {
  TRACE(_T("Warning: GetAbsolutePage 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

/*========================================================================
 name:  指定 Recordset 对象当前记录的序号位置. 
    ----------------------------------------------------------
 returns: 设置或返回从 1 到 Recordset 对象 (PageCount) 所含页数的长整
   型值，或者返回以下常量. 
 [常量]   [说明]
 ---------------------------------
 adPosUnknown Recordset 为空，当前位置未知，或者提供者不支持 AbsolutePage 属性.  
 adPosBOF  当前记录指针位于 BOF(即 BOF 属性为 True).  
 adPosEOF  当前记录指针位于 EOF(即 EOF 属性为 True).  
    ----------------------------------------------------------
 Remarks:  使用 AbsolutePosition 属性可根据其在 Recordset 中的序号
 位置移动到记录，或确定当前记录的序号位置. 提供者必须支持该属性的相应功
 能才能使用该属性. 
  同 AbsolutePage 属性一样，AbsolutePosition 从 1 开始，并在当前记录
 为 Recordset 中的第一个记录时等于 1. 从 RecordCount 属性可获得 Recordset 
 对象的总记录数. 
  设置 AbsolutePosition 属性时，即使该属性指向位于当前缓存中的记录，
 ADO 也将使用以指定的记录开始的新记录组重新加载缓存. CacheSize 属性决定
 该记录组的大小. 
  注意   不能将 AbsolutePosition 属性作为替代的记录编号使用. 删除前面
 的记录时，给定记录的当前位置将发生改变. 如果 Recordset 对象被重新查询或
 重新打开，则无法保证给定记录有相同的 AbsolutePosition. 书签仍然是保持和
 返回给定位置的推荐方式，并且在所有类型的 Recordset 对象的定位时是唯一的
 方式. 
==========================================================================*/
BOOL CAdoRecordSet::SetAbsolutePosition(int nPosition)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);  
  return TRUE;
 }
 catch(_com_error &e)
 {
  TRACE(_T("Warning: SetAbsolutePosition 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

long CAdoRecordSet::GetAbsolutePosition()
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  return m_pRecordset->GetAbsolutePosition();
 }
 catch(_com_error &e)
 {
  TRACE(_T("Warning: GetAbsolutePosition 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

BOOL CAdoRecordSet::SetCursorLocation(CursorLocationEnum CursorLocation)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  m_pRecordset->PutCursorLocation(CursorLocation);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: PutCursorLocation 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

CursorLocationEnum CAdoRecordSet::GetCursorLocation()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->GetCursorLocation();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetCursorLocation 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adUseNone;
 }
}

BOOL CAdoRecordSet::SetCursorType(CursorTypeEnum CursorType)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  m_pRecordset->PutCursorType(CursorType);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetCursorType 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

CursorTypeEnum CAdoRecordSet::GetCursorType()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->GetCursorType();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetCursorType 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adOpenUnspecified;
 }
}

/*========================================================================
 Remarks: Recordset 对象包括 Field 对象组成的 Fields 集合. 每个Field
  对象对应 Recordset 集中的一列.
==========================================================================*/
FieldsPtr CAdoRecordSet::GetFields()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->GetFields();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetFields 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
 return NULL;
}

/*========================================================================
 Remarks: 取得指定列字段的字段名.
==========================================================================*/
CString CAdoRecordSet::GetFieldName(long lIndex)
{
 ASSERT(m_pRecordset != NULL);
 CString strFieldName;
 try
 {
  strFieldName = LPCTSTR(m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetName());
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetFieldName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 }
 return strFieldName;
}

/*========================================================================
 name:  取得 Field 对象一项或多项属性.
    ----------------------------------------------------------
 returns: 对于 Field 对象, Attributes 属性为只读, 其值可能为以下任意
  一个或多个 FieldAttributeEnum 值的和.
   [常量]    [说明] 
   -------------------------------------------
   adFldMayDefer   指示字段被延迟, 即不从拥有整个记录的数据源检索
      字段值, 仅在显式访问这些字段时才进行检索. 
   adFldUpdatable  指示可以写入该字段. 
   adFldUnknownUpdatable 指示提供者无法确定是否可以写入该字段. 
   adFldFixed   指示该字段包含定长数据. 
   adFldIsNullable  指示该字段接受 Null 值. 
   adFldMayBeNull  指示可以从该字段读取 Null 值. 
   adFldLong    指示该字段为长二进制字段. 并指示可以使用 AppendChunk 
      和 GetChunk 方法. 
   adFldRowID   指示字段包含持久的行标识符, 该标识符无法被写入
      并且除了对行进行标识(如记录号、唯一标识符等)外不
      存在有意义的值. 
   adFldRowVersion  指示该字段包含用来跟踪更新的某种时间或日期标记. 
   adFldCacheDeferred 指示提供者缓存了字段值, 并已完成随后对缓存的读取. 
==========================================================================*/
long CAdoRecordSet::GetFieldAttributes(long lIndex)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetAttributes();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetFieldAttributes 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

long CAdoRecordSet::GetFieldAttributes(LPCTSTR lpszFieldName)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->GetAttributes();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetFieldAttributes 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

/*========================================================================
 Name:  指示 Field 对象所定义的长度.
    ----------------------------------------------------------
 returns: 返回某个字段定义的长度(按字节数)的长整型值.
    ----------------------------------------------------------
 Remarks: 使用 DefinedSize 属性可确定 Field 对象的数据容量.
==========================================================================*/
long CAdoRecordSet::GetFieldDefineSize(long lIndex)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->DefinedSize;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetDefineSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

long CAdoRecordSet::GetFieldDefineSize(LPCTSTR lpszFieldName)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->DefinedSize;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetDefineSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

/*========================================================================
 Name: 取得字段的值的实际长度.
    ----------------------------------------------------------
 returns: 返回长整型值.某些提供者允许设置该属性以便为 BLOB 数据预留
   空间, 在此情况下默认值为 0.
    ----------------------------------------------------------
 Remarks: 使用 ActualSize 属性可返回 Field 对象值的实际长度.对于所有
   字段,ActualSize 属性为只读.如果 ADO 无法确定 Field 对象值的实
   际长度, ActualSize 属性将返回 adUnknown.
    如以下范例所示, ActualSize 和  DefinedSize 属性有所不同: 
   adVarChar 声明类型且最大长度为 50 个字符的 Field 对象将返回为 
   50 的 DefinedSize 属性值, 但是返回的 ActualSize 属性值是当前记
   录的字段中存储的数据的长度.
==========================================================================*/
long CAdoRecordSet::GetFieldActualSize(long lIndex)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->ActualSize;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetFieldActualSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
}

long CAdoRecordSet::GetFieldActualSize(LPCTSTR lpszFieldName)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->ActualSize;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetFieldActualSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
}

/*========================================================================
 returns: 返回下列值之一. 相应的 OLE DB 类型标识符在下表的说明栏的括
   号中给出.
   [常量]   [说明] 
   ---------------------------------------------------------
   adArray   与其他类型一起加入逻辑 OR 以指示该数据是那种类型的
     安全数组 (DBTYPE_ARRAY). 
   adBigInt   8 字节带符号的整数 (DBTYPE_I8). 
   adBinary   二进制值 (DBTYPE_BYTES). 
   adBoolean   布尔型值 (DBTYPE_BOOL). 
   adByRef   与其他类型一起加入逻辑 OR 以指示该数据是其他类型数
     据的指针 (DBTYPE_BYREF). 
   adBSTR   以空结尾的字符串 (Unicode) (DBTYPE_BSTR). 
   adChar   字符串值 (DBTYPE_STR). 
   adCurrency  货币值 (DBTYPE_CY).货币数字的小数点位置固定、小数
     点右侧有四位数字.该值保存为 8 字节范围为10,000 的带符
     号整型值. 
   adDate   日期值 (DBTYPE_DATE).日期按双精度型数值来保存, 数
     字全部表示从 1899 年 12 月 30 开始的日期数.小数部分是
     一天当中的片段时间. 
   adDBDate   日期值 (yyyymmdd) (DBTYPE_DBDATE). 
   adDBTime   时间值 (hhmmss) (DBTYPE_DBTIME). 
   adDBTimeStamp  时间戳 (yyyymmddhhmmss 加 10 亿分之一的小数)(DBTYPE_DBTIMESTAMP). 
   adDecimal   具有固定精度和范围的精确数字值 (DBTYPE_DECIMAL). 
   adDouble   双精度浮点值 (DBTYPE_R8). 
   adEmpty   未指定值 (DBTYPE_EMPTY). 
   adError   32 - 位错误代码 (DBTYPE_ERROR). 
   adGUID   全局唯一的标识符 (GUID) (DBTYPE_GUID). 
   adIDispatch  OLE 对象上 Idispatch 接口的指针 (DBTYPE_IDISPATCH). 
   adInteger   4 字节的带符号整型 (DBTYPE_I4). 
   adIUnknown  OLE 对象上 IUnknown 接口的指针 (DBTYPE_IUNKNOWN).
   adLongVarBinary 长二进制值. 
   adLongVarChar  长字符串值. 
   adLongVarWChar 以空结尾的长字符串值. 
   adNumeric   具有固定精度和范围的精确数字值 (DBTYPE_NUMERIC). 
   adSingle   单精度浮点值 (DBTYPE_R4). 
   adSmallInt  2 字节带符号整型 (DBTYPE_I2). 
   adTinyInt   1 字节带符号整型 (DBTYPE_I1). 
   adUnsignedBigInt 8 字节不带符号整型 (DBTYPE_UI8). 
   adUnsignedInt  4 字节不带符号整型 (DBTYPE_UI4). 
   adUnsignedSmallInt 2 字节不带符号整型 (DBTYPE_UI2). 
   adUnsignedTinyInt 1 字节不带符号整型 (DBTYPE_UI1). 
   adUserDefined  用户定义的变量 (DBTYPE_UDT). 
   adVarBinary  二进制值. 
   adVarChar   字符串值. 
   adVariant   自动变体型 (DBTYPE_VARIANT). 
   adVector   与其他类型一起加入逻辑 OR 中, 指示数据是 DBVECTOR 
     结构(由 OLE DB 定义).该结构含有元素的计数和其他类型 
     (DBTYPE_VECTOR) 数据的指针. 
   adVarWChar  以空结尾的 Unicode 字符串. 
   adWChar   以空结尾的 Unicode 字符串 (DBTYPE_WSTR). 
    ----------------------------------------------------------
 Remarks: 返回指定字段的数据类型.
==========================================================================*/
DataTypeEnum CAdoRecordSet::GetFieldType(long lIndex)
{
 ASSERT(m_pRecordset != NULL);
 try 
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetType();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetField 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adEmpty;
 } 
}

DataTypeEnum CAdoRecordSet::GetFieldType(LPCTSTR lpszFieldName)
{
 ASSERT(m_pRecordset != NULL);
 try 
 {
  return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->GetType();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetField发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adEmpty;
 } 
}

BOOL CAdoRecordSet::IsFieldNull(LPCTSTR lpFieldName)
{
 try
 {
  _variant_t vt = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
  return (vt.vt == VT_NULL);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: IsFieldNull 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoRecordSet::IsFieldNull(long index)
{
 try
 {
  _variant_t vt = m_pRecordset->Fields->GetItem(_variant_t(index))->Value;
  return (vt.vt == VT_NULL);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: IsFieldNull 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 Name: 取得指定列的字段对象的指针. 
==========================================================================*/
FieldPtr CAdoRecordSet::GetField(long lIndex)
{
 try
 {
  return GetFields()->GetItem(_variant_t(lIndex));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetField发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 }
}

FieldPtr CAdoRecordSet::GetField(LPCTSTR lpszFieldName)
{
 try
 {
  return GetFields()->GetItem(_variant_t(lpszFieldName));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetField发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 }
}

/*########################################################################
     ------------------------------------------------
          设置字段的值
     ------------------------------------------------
  ########################################################################*/
BOOL CAdoRecordSet::PutCollect(long index, const _variant_t &value)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(index < GetFieldsCount());
 try
 {
  if (m_pRecordset != NULL) 
  {
   m_pRecordset->PutCollect(_variant_t(index), value);
   return TRUE;
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: PutCollect 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
 return FALSE;
}

BOOL CAdoRecordSet::PutCollect(LPCSTR strFieldName, const _variant_t &value)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL) 
  {
   m_pRecordset->put_Collect(_variant_t(strFieldName), value);
   return TRUE;
  }
 }
 catch (_com_error e)
 {
  return FALSE;
  TRACE(_T("Warning: PutCollect 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 } 
 return FALSE;
}

BOOL CAdoRecordSet::PutCollect(long index, const bool &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adBoolean)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const bool &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adBoolean)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const BYTE &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adUnsignedTinyInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const BYTE &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adUnsignedTinyInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const short &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adSmallInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const short &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adSmallInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const int &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adInteger)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(index, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const int &value)
{
 ASSERT(m_pRecordset != NULL);

 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adInteger)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(strFieldName, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(long index, const long &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adBigInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const long &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adBigInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const DWORD &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adUnsignedBigInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 _variant_t vt;
 vt.vt = VT_UI4;
 vt.ulVal = value;
 return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const DWORD &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adUnsignedBigInt)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 _variant_t vt;
 vt.vt = VT_UI4;
 vt.ulVal = value;
 return PutCollect(strFieldName, vt);
}

BOOL CAdoRecordSet::PutCollect(long index, const float &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adSingle)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const float &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adSingle)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const double &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adDouble)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const double &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adDouble)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const COleDateTime &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (   GetFieldType(index) != adDate
  && GetFieldType(index) != adDBDate
  && GetFieldType(index) != adDBTime
  && GetFieldType(index) != adDBTimeStamp)
 {
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 }
 #endif

 _variant_t vt;
 vt.vt = VT_DATE;
 vt.date = value;
 return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const COleDateTime &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (   GetFieldType(strFieldName) != adDate
  && GetFieldType(strFieldName) != adDBDate
  && GetFieldType(strFieldName) != adDBTime
  && GetFieldType(strFieldName) != adDBTimeStamp)
 {
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 }
 #endif

 _variant_t vt;
 vt.vt = VT_DATE;
 vt.date = value;
 return PutCollect(strFieldName, vt);
}

BOOL CAdoRecordSet::PutCollect(long index, const COleCurrency &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(index) != adCurrency)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 if (value.m_status == COleCurrency::invalid) return FALSE;

 _variant_t vt;
 vt.vt = VT_CY;
 vt.cyVal = value.m_cur;
 return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const COleCurrency &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (GetFieldType(strFieldName) != adCurrency)
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 if (value.m_status == COleCurrency::invalid) return FALSE;

 _variant_t vt;
 vt.vt = VT_CY;
 vt.cyVal = value.m_cur;
 return PutCollect(strFieldName, vt);
}

BOOL CAdoRecordSet::PutCollect(long index, const CString &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (! (GetFieldType(index) == adVarChar
  || GetFieldType(index) == adChar
  || GetFieldType(index) == adLongVarChar
  || GetFieldType(index) == adVarWChar
  || GetFieldType(index) == adWChar
  || GetFieldType(index) == adLongVarWChar))
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 _variant_t vt;
 vt.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
 vt.bstrVal = value.AllocSysString();
 return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const CString &value)
{
 ASSERT(m_pRecordset != NULL);
 #ifdef _DEBUG
 if (! (GetFieldType(strFieldName) == adVarChar
  || GetFieldType(strFieldName) == adChar
  || GetFieldType(strFieldName) == adLongVarChar
  || GetFieldType(strFieldName) == adVarWChar
  || GetFieldType(strFieldName) == adWChar
  || GetFieldType(strFieldName) == adLongVarWChar))
  TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
 #endif

 _variant_t vt;
 vt.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
 vt.bstrVal = value.AllocSysString();
 return PutCollect(strFieldName, vt);
}

/*########################################################################
     ------------------------------------------------
           读取字段的值
     ------------------------------------------------
  ########################################################################*/
BOOL CAdoRecordSet::GetCollect(long index, COleDateTime &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartodate(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value.SetStatus(COleDateTime::null);
  return FALSE;
 }
}

BOOL CAdoRecordSet::GetCollect(LPCTSTR strFieldName, COleDateTime &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartodate(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value.SetStatus(COleDateTime::null);
  return FALSE;
 }
}

BOOL CAdoRecordSet::GetCollect(long index, COleCurrency &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartocy(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value.m_status = COleCurrency::null;
  return FALSE;
 }
}

BOOL CAdoRecordSet::GetCollect(LPCTSTR strFieldName, COleCurrency &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartocy(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value.m_status = COleCurrency::null;
  return FALSE;
 }
}

BOOL CAdoRecordSet::GetCollect(long index,  bool &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartobool(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = false;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  bool &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartobool(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = false;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(long index,  BYTE &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartoby(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  BYTE &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartoby(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(long index,  short &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartoi(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  short &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartoi(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(long index,  int &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = (int)vartol(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
 return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  int &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = (int)vartol(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(long index,  long &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartol(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  long &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartol(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
 return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  DWORD &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  _variant_t result = m_pRecordset->GetCollect(_variant_t(index));
  switch (result.vt)
  {
  case VT_UI4:
  case VT_I4:
   value = result.ulVal;
   break;
  case VT_NULL:
  case VT_EMPTY:
   value = 0;
   break;
  default:
   TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
   return FALSE;
  }  
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  DWORD &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
  switch (result.vt)
  {
  case VT_UI4:
  case VT_I4:
   value = result.ulVal;
   break;
  case VT_NULL:
  case VT_EMPTY:
   value = 0;
   break;
  default:
   TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
   return FALSE;
  }  
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(long index,  float &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  _variant_t result = m_pRecordset->GetCollect(_variant_t(index));
  switch (result.vt)
  {
  case VT_R4:
   value = result.fltVal;
   break;
  case VT_UI1:
  case VT_I1:
   value = result.bVal;
   break;
  case VT_UI2:
  case VT_I2:
   value = result.iVal;
   break;
  case VT_NULL:
  case VT_EMPTY:
   value = 0;
   break;
  default:
   TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
   return FALSE;
  }  
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  float &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  _variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
  switch (result.vt)
  {
  case VT_R4:
   value = result.fltVal;
   break;
  case VT_UI1:
  case VT_I1:
   value = result.bVal;
   break;
  case VT_UI2:
  case VT_I2:
   value = result.iVal;
   break;
  case VT_NULL:
  case VT_EMPTY:
   value = 0;
   break;
  default:
   TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d/n"), __FILE__, __LINE__);
   return FALSE;
  }  
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(long index,  double &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartof(m_pRecordset->GetCollect(_variant_t(index)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  double &value)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  value = vartof(m_pRecordset->GetCollect(_variant_t(strFieldName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  value = 0;
  return FALSE;
 }  
}

BOOL CAdoRecordSet::GetCollect(long index, CString& strValue)
{
 ASSERT(m_pRecordset != NULL);
 if (index < 0 || index >= GetFieldsCount())
 {
  return FALSE;
 }

 try
 {
  if (!IsOpen())
  {
   MessageBox(NULL, _T("数据库可能已经断开,/r/n请重新连接、然后重试."), _T("提示"), MB_ICONINFORMATION);
   return FALSE;
  } 
  if (m_pRecordset->adoEOF)
  {
   return FALSE;
  }
  _variant_t value = m_pRecordset->GetCollect(_variant_t(index));
  strValue = vartostr(value);
  return TRUE;
 }
 catch (_com_error e)
 {
  return FALSE;
 }

 return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCTSTR strFieldName, CString &strValue)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (!IsOpen())
  {
   MessageBox(NULL, _T("数据库可能已经断开,/r/n请重新连接、然后重试."), _T("提示"), MB_ICONINFORMATION);
   return FALSE;
  } 
  if (m_pRecordset->adoEOF)
  {
   return FALSE;
  }
  _variant_t value = m_pRecordset->GetCollect(_variant_t(LPCTSTR(strFieldName)));
  strValue = vartostr(value);
  return TRUE; 
 }
 catch (_com_error e)
 {
  #ifdef _DEBUG
     CString sMsg = strFieldName;
     MessageBox(NULL, sMsg+_T(" GetCollect失败，请检查项目名称!"), _T("提示"), MB_ICONINFORMATION);
        #endif
  return FALSE;
 }

 return FALSE;
}

/*########################################################################
     ------------------------------------------------
    将数据追加到大型文本、二进制数据 Field 对象. 
     ------------------------------------------------
  ########################################################################*/
BOOL CAdoRecordSet::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
{
 SAFEARRAY FAR *pSafeArray = NULL;
 SAFEARRAYBOUND rgsabound[1];

 try
 {
  rgsabound[0].lLbound = 0;
  rgsabound[0].cElements = nBytes;
  pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound);

  for (long i = 0; i < (long)nBytes; i++)
  {
   UCHAR &chData = ((UCHAR*)lpData)[i];
   HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
   if (FAILED(hr)) return FALSE;
  }

  _variant_t varChunk;
  varChunk.vt = VT_ARRAY | VT_UI1;
  varChunk.parray = pSafeArray;

  return (pField->AppendChunk(varChunk) == S_OK);
 }
 catch (_com_error &e)
 {
  TRACE(_T("Warning: AppendChunk 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoRecordSet::AppendChunk(long index, LPVOID lpData, UINT nBytes)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(lpData != NULL);
 if (adFldLong & GetFieldAttributes(index))
 {
  return AppendChunk(GetField(index), lpData, nBytes);
 }
 else return FALSE;
}

BOOL CAdoRecordSet::AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(lpData != NULL);
 if (adFldLong & GetFieldAttributes(strFieldName))
 {
  return AppendChunk(GetField(strFieldName), lpData, nBytes);
 }
 else return FALSE;
}

BOOL CAdoRecordSet::AppendChunk(long index, LPCTSTR lpszFileName)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(lpszFileName != NULL);
 BOOL bret = FALSE;
 if (adFldLong & GetFieldAttributes(index))
 {
  CFile file;
  if (file.Open(lpszFileName, CFile::modeRead))
  {
   long length = (long)file.GetLength();
   char *pbuf = new char[length];
   if (pbuf != NULL && file.Read(pbuf, length) == (DWORD)length)
   {
    bret = AppendChunk(GetField(index), pbuf, length);
   }
   if (pbuf != NULL) delete[] pbuf;
  }
  file.Close();
 }
 return bret;
}

BOOL CAdoRecordSet::AppendChunk(LPCSTR strFieldName, LPCTSTR lpszFileName)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(lpszFileName != NULL);
 BOOL bret = FALSE;
 if (adFldLong & GetFieldAttributes(strFieldName))
 {
  CFile file;
  if (file.Open(lpszFileName, CFile::modeRead))
  {
   long length = (long)file.GetLength();
   char *pbuf = new char[length];
   if (pbuf != NULL && file.Read(pbuf, length) == (DWORD)length)
   {
    bret = AppendChunk(GetField(strFieldName), pbuf, length);
   }
   if (pbuf != NULL) delete[] pbuf;
  }
  file.Close();
 }
 return bret;
}

BOOL CAdoRecordSet::GetChunk(FieldPtr pField, LPVOID lpData)
{
 ASSERT(pField != NULL);
 ASSERT(lpData != NULL);

 UCHAR chData;
 long index = 0;

 while (index < pField->ActualSize)
 { 
  try
  {
   _variant_t varChunk = pField->GetChunk(100);
   if (varChunk.vt != (VT_ARRAY | VT_UI1))
   {
    return FALSE;
   }

            for (long i = 0; i < 100; i++)
            {
                if (SUCCEEDED( SafeArrayGetElement(varChunk.parray, &i, &chData) ))
                {
     ((UCHAR*)lpData)[index] = chData;
                    index++;
                }
                else
    {
                    break;
    }
            }
  }
  catch (_com_error e)
  {
   TRACE(_T("Warning: GetChunk 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
   return FALSE;
  }
 }

 return TRUE;
}

BOOL CAdoRecordSet::GetChunk(long index, LPVOID lpData)
{
 if (adFldLong & GetFieldAttributes(index))
  return  GetChunk(GetField(index), lpData);
 else return FALSE;
}

BOOL CAdoRecordSet::GetChunk(LPCSTR strFieldName, LPVOID lpData)
{
 if (adFldLong & GetFieldAttributes(strFieldName))
  return  GetChunk(GetField(strFieldName), lpData);
 else return FALSE;
}

BOOL CAdoRecordSet::GetChunk(long index, CBitmap &bitmap)
{
 return GetChunk(GetFieldName(index), bitmap);
}

BOOL CAdoRecordSet::GetChunk(LPCSTR strFieldName, CBitmap &bitmap)
{  
 BOOL bret = FALSE;
 long size = GetFieldActualSize(strFieldName);
 if ((adFldLong & GetFieldAttributes(strFieldName)) && size > 0)
 {
  BYTE *lpData = new BYTE[size];

  if (GetChunk(GetField(strFieldName), (LPVOID)lpData))
  {
   BITMAPFILEHEADER bmpHeader;
   DWORD bmfHeaderLen = sizeof(bmpHeader);
   strncpy((LPSTR)&bmpHeader, (LPSTR)lpData, bmfHeaderLen);

   // 是否是位图 ----------------------------------------
   if (bmpHeader.bfType == (*(WORD*)"BM"))
   {
    BYTE* lpDIBBits = lpData + bmfHeaderLen;
    BITMAPINFOHEADER &bmpiHeader = *(LPBITMAPINFOHEADER)lpDIBBits;
    BITMAPINFO &bmpInfo = *(LPBITMAPINFO)lpDIBBits;
    lpDIBBits = lpData + ((BITMAPFILEHEADER *)lpData)->bfOffBits;

    // 创建位图 --------------------------------------
    CDC dc;
    HDC hdc = GetDC(NULL);
    dc.Attach(hdc);
    HBITMAP hBmp = CreateDIBitmap(dc.m_hDC, &bmpiHeader, CBM_INIT, lpDIBBits, &bmpInfo, DIB_RGB_COLORS);
    if (bitmap.GetSafeHandle() != NULL) bitmap.DeleteObject();
    bitmap.Attach(hBmp);
    dc.Detach();
    ReleaseDC(NULL, hdc);
    bret = TRUE;
   }
  }
  delete[] lpData;
  lpData = NULL;
 }
 return bret;
}

/*########################################################################
     ------------------------------------------------
           其他方法
     ------------------------------------------------
  ########################################################################*/

_RecordsetPtr CAdoRecordSet::operator =(_RecordsetPtr &pRecordSet)
{
 Close();
 if (pRecordSet != NULL)
 {
  m_pRecordset = NULL;
  m_pRecordset = pRecordSet;
  return m_pRecordset;
 }
 return NULL;
}

/*========================================================================
 Name: 确定指定的 Recordset 对象是否支持特定类型的功能. 
    ----------------------------------------------------------
 Params: CursorOptions   长整型, 包括一个或多个下列 CursorOptionEnum 值.
 [常量]    [说明] 
 ------------------------------------
 adAddNew   可使用 AddNew 方法添加新记录. 
 adApproxPosition 可读取并设置 AbsolutePosition 和 AbsolutePage 的属性. 
 adBookmark   可使用 Bookmark 属性获得对特定记录的访问. 
 adDelete   可以使用 Delete 方法删除记录. 
 adHoldRecords  可以检索多个记录或者更改下一个检索位置而不必提交所
     有挂起的更改. 
 adMovePrevious  可使用 MoveFirst 和 MovePrevious 方法, 以及 Move 或
     GetRows 方法将当前记录位置向后移动而不必使用书签. 
 adResync   通过 Resync 方法, 使用在基本的数据库中可见的数据更
     新游标. 
 adUpdate   可使用 Update 方法修改现有的数据. 
 adUpdateBatch  可以使用批更新(UpdateBatch 和 CancelBatch 方法) 将
     更改组传输给提供者. 
 adIndex    可以使用 Index 属性命名索引. 
 adSeek    可以使用 Seek 方法定位 Recordset 中的行. 
    ----------------------------------------------------------
 returns: 返回布尔型值, 指示是否支持 CursorOptions 参数所标识的所有功能.
    ----------------------------------------------------------
 Remarks: 使用 Supports 方法确定 Recordset 对象所支持的功能类型. 如
 果 Recordset 对象支持其相应常量在 CursorOptions 中的功能, 那么 Supports
 方法返回 True.否则返回 False.
 注意   尽管 Supports 方法可对给定的功能返回 True, 但它不能保证提供者可
 以使功能在所有环境下均有效. Supports 方法只返回提供者是否支持指定的功能
 (假定符合某些条件). 例如, Supports 方法可能指示 Recordset 对象支持更新
 (即使游标基于多个表的合并), 但并且某些列仍然无法更新.
==========================================================================*/
BOOL CAdoRecordSet::Supports(CursorOptionEnum CursorOptions)
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (m_pRecordset != NULL)
  {
   return m_pRecordset->Supports(CursorOptions);
  }
 }
 catch (const _com_error& e)
 {
  TRACE(_T("Warning: Supports方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
 return FALSE;
}

/*========================================================================
 name:  为 Recordset 中的数据指定筛选条件.
==========================================================================*/
BOOL CAdoRecordSet::SetFilter(LPCTSTR lpszFilter)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(IsOpen());

 try
 {
  m_pRecordset->PutFilter(lpszFilter);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetFilter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 name:  为 Recordset 中的数据指定排序条件.
==========================================================================*/
BOOL CAdoRecordSet::SetSort(LPCTSTR lpszCriteria)
{
 ASSERT(IsOpen());

 try
 {
  m_pRecordset->PutSort(lpszCriteria);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetFilter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 name:  返回唯一标识 Recordset 对象中当前记录的书签。
==========================================================================*/
_variant_t CAdoRecordSet::GetBookmark()
{
 ASSERT(m_pRecordset != NULL);
 try
 {
  if (IsOpen())
  {
   m_varBookmark = m_pRecordset->GetBookmark();
   return m_varBookmark;
  }  
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetBookmark 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 }
 return _variant_t((long)adBookmarkFirst);
}

/*========================================================================
 name:  将 Recordset 对象的当前记录设置为由有效书签所标识的记录。
==========================================================================*/
BOOL CAdoRecordSet::SetBookmark(_variant_t varBookMark)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  if (IsOpen() && varBookMark.vt != VT_EMPTY && varBookMark.vt != VT_NULL)
  {
   m_pRecordset->PutBookmark(varBookMark);
   return TRUE;
  } 
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetBookmark 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 }
 return FALSE;
}

void CAdoRecordSet::SetAdoConnection(CAdoConnection *pConnection)
{
 m_pConnection = pConnection;
}

_RecordsetPtr& CAdoRecordSet::GetRecordset()
{
 return m_pRecordset;
}

CString CAdoRecordSet::GetLastError()
{
 ASSERT(m_pConnection != NULL);
 return m_pConnection->GetLastErrorText();
}

/*========================================================================
 name: 创建与现有 Recordset 对象相同的复制 Recordset 对象。可选择指定
 该副本为只读。
==========================================================================*/
BOOL CAdoRecordSet::Clone(CAdoRecordSet &pRecordSet)
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  pRecordSet = m_pRecordset->Clone(adLockUnspecified);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Clone 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 name:  搜索 Recordset 中满足指定标准的记录. 如果满足标准，则记录集
   位置设置在找到的记录上，否则位置将设置在记录集的末尾. 
    ----------------------------------------------------------
 params:  [criteria]:   字符串，包含指定用于搜索的列名、比较操作符和
    值的语句. 
    [searchDirection]:    可选的 SearchDirectionEnum 值，指定搜
 索应从当前行还是下一个有效行开始. 其值可为 adSearchForward 或 adSearchBackward. 
 搜索是在记录集的开始还是末尾结束由 searchDirection 值决定. 
 [常量]   [说明]
 ---------------------------------
 adSearchForward  
 adSearchBackward 
    ----------------------------------------------------------
 Remarks: criteria 中的"比较操作符"可以是">"(大于)、"<"(小于)、"="(等
 于)、">="(大于或等于)、"<="(小于或等于)、"<>"(不等于)或"like"(模式匹配).  
  criteria 中的值可以是字符串、浮点数或者日期. 字符串值以单引号分界(如
 "state = 'WA'"). 日期值以"#"(数字记号)分界(如"start_date > #7/22/97#"). 
  如"比较操作符"为"like"，则字符串"值"可以包含"*"(某字符可出现一次或
 多次)或者"_"(某字符只出现一次). (如"state like M_*"与 Maine 和 Massachusetts 
 匹配.). 
==========================================================================*/
BOOL CAdoRecordSet::Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection)
{
 ASSERT(m_pRecordset != NULL);
 ASSERT(AfxIsValidString(lpszFind));

 try
 {
  if (strcmp(lpszFind, _T("")) != 0)
  {
   m_strFind = lpszFind;
  }

  if (m_strFind.IsEmpty()) return FALSE;

  m_pRecordset->Find(_bstr_t(m_strFind), 0, SearchDirection, "");

  if ((IsEOF() || IsBOF()) )
  {
   return FALSE;
  }
  else
  {
   m_SearchDirection = SearchDirection;
   GetBookmark();
   return TRUE;
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Find 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 name:  查找下一条满足条件的记录.
==========================================================================*/
BOOL CAdoRecordSet::FindNext()
{
 ASSERT(m_pRecordset != NULL);

 try
 {
  if (m_strFind.IsEmpty()) return FALSE;

  m_pRecordset->Find(_bstr_t(m_strFind), 1, m_SearchDirection, m_varBookmark);

  if ((IsEOF() || IsBOF()) )
  {
   return FALSE;
  }
  else
  {
   GetBookmark();
   return TRUE;
  }
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: FindNext 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoRecordSet::RecordBinding(CADORecordBinding &pAdoRecordBinding)
{
 m_pAdoRecordBinding = NULL;

 try
 {
  if (SUCCEEDED(m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&m_pAdoRecordBinding)))
  {
   if (SUCCEEDED(m_pAdoRecordBinding->BindToRecordset(&pAdoRecordBinding)))
   {
    return TRUE;
   } 
  }
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: RecordBinding 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoRecordSet::AddNew(CADORecordBinding &pAdoRecordBinding)
{
 try
 {
  if (m_pAdoRecordBinding->AddNew(&pAdoRecordBinding) == S_OK)
  {
     m_pAdoRecordBinding->Update(&pAdoRecordBinding);
   return TRUE;
  }
  return FALSE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: AddNew 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

/*########################################################################
 【文件名】:  ado.h
 【名  称】:  ADO 封装类.
 【版  本】:  0.20
 【作  者】:  成真
 【E-mail】:  [anyou@sina.com](mailto:anyou@sina.com)
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADOCOMMAND_H)
#define _ANYOU_COOL_ADOCOMMAND_H
#include "Ado.h"

/*########################################################################
   ------------------------------------------------
         CAdoParameter class
   ------------------------------------------------
  ########################################################################*/
class CAdoParameter
{
// 构建/折构 -------------------------------------------
public:
 CAdoParameter();
 CAdoParameter(DataTypeEnum DataType, long lSize, ParameterDirectionEnum Direction, CString strName);
 virtual ~CAdoParameter();
// 属性 ------------------------------------------------
public:
 _ParameterPtr GetParameter() {return m_pParameter;}

 // 参数精度 ---------------------------
 BOOL SetPrecision(char nPrecision);

 // 参数小数位数 -----------------------
 BOOL SetNumericScale(int nScale);

 // 参数类型 ---------------------------
 ParameterDirectionEnum GetDirection();
 BOOL SetDirection(ParameterDirectionEnum Direction);

 // 参数名称 ---------------------------
 CString GetName();
 BOOL SetName(CString strName);

 // 参数长度 ---------------------------
 int GetSize();
 BOOL SetSize(int size);

 // 参数据类型 -------------------------
 DataTypeEnum GetType();
 BOOL SetType(DataTypeEnum DataType);

// 方法 ------------------------------------------------
public: 
 BOOL GetValue(COleDateTime &value);
 BOOL GetValue(CString &value);
 BOOL GetValue(double &value);
 BOOL GetValue(long &value);
 BOOL GetValue(int &value);
 BOOL GetValue(short &value);
 BOOL GetValue(BYTE &value);
 BOOL GetValue(bool &value);

 BOOL SetValue(const float &value);
 BOOL SetValue(const short &value);
 BOOL SetValue(const BYTE &value);
 BOOL SetValue(const COleDateTime &value);
 BOOL SetValue(const CString &value);
 BOOL SetValue(const double &value);
 BOOL SetValue(const long &value);
 BOOL SetValue(const int &value);
 BOOL SetValue(const bool &value);
 BOOL SetValue(const _variant_t &value);

//其他方法 ------------------------------
public:
 _ParameterPtr& operator =(_ParameterPtr& pParameter);

// 数据 ------------------------------------------------
protected:
 _ParameterPtr m_pParameter;
 CString m_strName;
 DataTypeEnum m_nType;
};

/*########################################################################
   ------------------------------------------------
         CAdoCommand class
   ------------------------------------------------
  ########################################################################*/

class CAdoCommand
{
// 构建/折构 -------------------------------------------
public:
 CAdoCommand();
 CAdoCommand(CAdoConnection* pAdoConnection, CString strCommandText = _T(""), CommandTypeEnum CommandType = adCmdStoredProc);
 virtual ~CAdoCommand();

// 属性 ------------------------------------------------
public:
 _variant_t GetValue(LPCTSTR lpstrName);
 _variant_t GetValue(long index);

 _ParameterPtr GetParameter(long index);
 _ParameterPtr GetParamter(LPCTSTR lpstrName);

 BOOL Append(_ParameterPtr param);

 ParametersPtr GetParameters();

 BOOL SetCommandTimeOut(long lTime);

 long GetState();

 BOOL SetCommandType(CommandTypeEnum CommandType);
 BOOL SetCommandText(LPCTSTR lpstrCommand);

 CAdoParameter operator [](int index);
 CAdoParameter operator [](LPCTSTR lpszParamName);

// 实现 ------------------------------------------------
public:
 _ParameterPtr CreateParameter(LPCTSTR lpstrName, DataTypeEnum Type, 
          ParameterDirectionEnum Direction, 
          long Size, _variant_t Value);
 _RecordsetPtr CAdoCommand::Execute(long Options = adCmdStoredProc);
 BOOL Cancel();

// 其他方法 --------------------------------------------
public:
 _CommandPtr& GetCommand();
 BOOL SetConnection(CAdoConnection *pConnect);

// 数据 ------------------------------------------------
protected:
 void Release();
 _CommandPtr  m_pCommand;
 CString   m_strSQL;
};

#endif // !defined(_ANYOU_COOL_ADOCOMMAND_H)
/*########################################################################
 Filename:  ado.cpp
 ----------------------------------------------------
 Remarks: ...
 ----------------------------------------------------
 Author:  成真
 Email:  [anyou@sina.com](mailto:anyou@sina.com)
[anyou@msn.com](mailto:anyou@msn.com)
 Created: 20/1/2003 19:19
  ########################################################################*/
#include "stdafx.h"
#include "../resource.h"
#include "AdoCommand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*########################################################################
   ------------------------------------------------
         CAdoCommand class
   ------------------------------------------------
  ########################################################################*/

CAdoCommand::CAdoCommand()
{
 ///创建 Connection 对象---------------------------
 m_pCommand.CreateInstance("ADODB.Command");
 #ifdef _DEBUG
 if (m_pCommand == NULL)
 {
  AfxMessageBox("Command 对象创建失败! 请确认是否初始化了Com环境.");
 }
 #endif
 ASSERT(m_pCommand != NULL);
}

CAdoCommand::CAdoCommand(CAdoConnection* pAdoConnection, CString strCommandText, CommandTypeEnum CommandType)
{
 ///创建 Connection 对象---------------------------
 m_pCommand.CreateInstance("ADODB.Command");
 #ifdef _DEBUG
 if (m_pCommand == NULL)
 {
  AfxMessageBox("Command 对象创建失败! 请确认是否初始化了Com环境.");
 }
 #endif
 ASSERT(m_pCommand != NULL);
 ASSERT(pAdoConnection != NULL);
 SetConnection(pAdoConnection);
 if (strCommandText != _T(""))
 {
  SetCommandText(LPCTSTR(strCommandText));
 }
 SetCommandType(CommandType);
}

CAdoCommand::~CAdoCommand()
{
 Release();
}

void CAdoCommand::Release()
{
 try
 {
  m_pCommand.Release();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Release方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 } 
}

_RecordsetPtr CAdoCommand::Execute(long Options)
{
 ASSERT(m_pCommand != NULL);
 try
 {
  return m_pCommand->Execute(NULL, NULL, Options);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Execute 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
}

BOOL CAdoCommand::Cancel()
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return (m_pCommand->Cancel() == S_OK);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Cancel 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

_ParameterPtr CAdoCommand::CreateParameter(LPCTSTR lpstrName, 
          DataTypeEnum Type, 
          ParameterDirectionEnum Direction, 
          long Size, 
          _variant_t Value)
{

 ASSERT(m_pCommand != NULL);
 try
 {
  return m_pCommand->CreateParameter(_bstr_t(lpstrName), Type, Direction, Size, Value);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: CreateParameter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
}

BOOL CAdoCommand::SetCommandText(LPCTSTR lpstrCommand)
{
 ASSERT(m_pCommand != NULL);
 try
 {
  m_pCommand->PutCommandText(_bstr_t(lpstrCommand));
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: PutCommandText 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

BOOL CAdoCommand::SetConnection(CAdoConnection *pConnect)
{
 ASSERT(pConnect != NULL);
 ASSERT(pConnect->GetConnection() != NULL);
 ASSERT(m_pCommand != NULL);

 try
 {
  m_pCommand->PutActiveConnection(_variant_t((IDispatch*)pConnect->GetConnection(), true));
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetConnection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}
/*========================================================================
 Name:  指示 Command 对象的类型。
    ----------------------------------------------------------
 returns: 返回以下某个 CommandTypeEnum 的值.
  [常量]     [说明] 
  ----------------------------------
  adCmdText   指示strSQL为命令文本, 即普通的SQL语句. 
  adCmdTable   指示ADO生成SQL查询返回以 strSQL 命名的表中的
      所有行. 
  adCmdTableDirect 指示所作的更改在strSQL中命名的表中返回所有行. 
  adCmdStoredProc  指示strSQL为存储过程. 
  adCmdUnknown  指示strSQL参数中的命令类型为未知. 
  adCmdFile   指示应从在strSQL中命名的文件中恢复保留(保存的)
      Recordset. 
  adAsyncExecute  指示应异步执行strSQL. 
  adAsyncFetch  指示在提取 Initial Fetch Size 属性中指定的初始
      数量后, 应该异步提取所有剩余的行. 如果所需的行尚未
      提取, 主要的线程将被堵塞直到行重新可用. 
  adAsyncFetchNonBlocking 指示主要线程在提取期间从未堵塞. 如果所请求
      的行尚未提取, 当前行自动移到文件末尾. 
   ----------------------------------------------------------
 Remarks: 使用 CommandType 属性可优化 CommandText 属性的计算。
  如果 CommandType 属性的值等于 adCmdUnknown(默认值), 系统的性能将会
 降低, 因为 ADO 必须调用提供者以确定 CommandText 属性是 SQL 语句、还是存
 储过程或表格名称。如果知道正在使用的命令的类型, 可通过设置 CommandType 
 属性指令 ADO 直接转到相关代码。如果 CommandType 属性与 CommandText 属
 性中的命令类型不匹配, 调用 Execute 方法时将产生错误。
==========================================================================*/
BOOL CAdoCommand::SetCommandType(CommandTypeEnum CommandType)
{
 ASSERT(m_pCommand != NULL);

 try
 {
  m_pCommand->PutCommandType(CommandType);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: PutCommandType 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

long CAdoCommand::GetState()
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return m_pCommand->GetState();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetState 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 } 
}

BOOL CAdoCommand::SetCommandTimeOut(long lTime)
{
 ASSERT(m_pCommand != NULL);

 try
 {
  m_pCommand->PutCommandTimeout(lTime);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetCommandTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

ParametersPtr CAdoCommand::GetParameters()
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return m_pCommand->GetParameters();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetCommandTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
}

BOOL CAdoCommand::Append(_ParameterPtr param)
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return m_pCommand->GetParameters()->Append((IDispatch*)param);
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: Append 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

_ParameterPtr CAdoCommand::GetParamter(LPCTSTR lpstrName)
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return m_pCommand->GetParameters()->GetItem(_variant_t(lpstrName));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetParamter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
}

_ParameterPtr CAdoCommand::GetParameter(long index)
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return m_pCommand->GetParameters()->GetItem(_variant_t(index));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetParamter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return NULL;
 } 
}

_variant_t CAdoCommand::GetValue(long index)
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return m_pCommand->GetParameters()->GetItem(_variant_t(index))->Value;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  _variant_t vt;
  vt.vt = VT_NULL;
  return vt;
 } 
}

_variant_t CAdoCommand::GetValue(LPCTSTR lpstrName)
{
 ASSERT(m_pCommand != NULL);

 try
 {
  return m_pCommand->GetParameters()->GetItem(_variant_t(lpstrName))->Value;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  _variant_t vt;
  vt.vt = VT_NULL;
  return vt;
 } 
}

_CommandPtr& CAdoCommand::GetCommand()
{
 return m_pCommand;
}

CAdoParameter CAdoCommand::operator [](int index)
{
 CAdoParameter pParameter;
 ASSERT(m_pCommand != NULL);
 try
 {
  pParameter = m_pCommand->GetParameters()->GetItem(_variant_t(long(index)));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: operator [] 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 }
 return pParameter;
}

CAdoParameter CAdoCommand::operator [](LPCTSTR lpszParamName)
{
 CAdoParameter pParameter;
 ASSERT(m_pCommand != NULL);
 try
 {
  pParameter = m_pCommand->GetParameters()->GetItem(_variant_t(lpszParamName));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: operator [] 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
 }
 return pParameter;
}

/*########################################################################
     ------------------------------------------------
           CAdoParameter class
     ------------------------------------------------
  ########################################################################*/
CAdoParameter::CAdoParameter()
{
 m_pParameter = NULL;
 m_pParameter.CreateInstance("ADODB.Parameter");
 #ifdef _DEBUG
 if (m_pParameter == NULL)
 {
  AfxMessageBox("Parameter 对象创建失败! 请确认是否初始化了Com环境.");
 }
 #endif
 ASSERT(m_pParameter != NULL);
 m_strName = _T("");
}

CAdoParameter::CAdoParameter(DataTypeEnum DataType, long lSize, ParameterDirectionEnum Direction, CString strName)
{
 m_pParameter = NULL;
 m_pParameter.CreateInstance("ADODB.Parameter");
 #ifdef _DEBUG
 if (m_pParameter == NULL)
 {
  AfxMessageBox("Parameter 对象创建失败! 请确认是否初始化了Com环境.");
 }
 #endif
 ASSERT(m_pParameter != NULL);

 m_pParameter->Direction = Direction;
 m_strName = strName;
 m_pParameter->Name = m_strName.AllocSysString();
 m_pParameter->Type = DataType;
 m_pParameter->Size = lSize;
}

_ParameterPtr& CAdoParameter::operator =(_ParameterPtr& pParameter)
{
 if (pParameter != NULL)
 {
  m_pParameter = pParameter;
 }
 else
 {
  return pParameter;
 }
 return m_pParameter;
}

CAdoParameter::~CAdoParameter()
{
 m_pParameter.Release();
 m_pParameter = NULL;
 m_strName = _T("");
}

/*========================================================================
 Name:  指示在 Parameter 对象中数字值或数字 Field 对象的精度。
    ----------------------------------------------------------
 Params:  设置或返回 Byte 值，用来表示值的最大位数。该值在 Parameter
   对象上为读/写，而在 Field 对象上为只读。
    ----------------------------------------------------------
 Remarks: 使用 Precision 属性可确定表示数字 Parameter 或 Field 对象值
   的最大位数
==========================================================================*/
BOOL CAdoParameter::SetPrecision(char nPrecision)
{
 ASSERT(m_pParameter != NULL);
 try
 {
  m_pParameter->PutPrecision(nPrecision);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetPrecision 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

/*========================================================================
 Name:  指出 Parameter 或 Field 对象中数字值的范围。
    ----------------------------------------------------------
 Params:  设置或返回字节值，指示数字值所精确到的小数点位数。
    ----------------------------------------------------------
 Remarks: 使用 NumericScale 属性可确定用于表明数字型 Parameter 或 Field 
  对象的值的小数位数。
  对于 Parameter 对象，NumericScale 属性为读/写。对于 Field 对象，
 NumericScale 属性为只读。

==========================================================================*/
BOOL CAdoParameter::SetNumericScale(int nScale)
{
 ASSERT(m_pParameter != NULL);
 try
 {
  m_pParameter->PutNumericScale(nScale);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetPrecision 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

/*========================================================================
 Name:  指示 Parameter 对象的数据类型.
    ----------------------------------------------------------
 Params:  [DataType]: DataTypeEnum 类型值, 请参考 CRecordSet 类相关
   函数.
==========================================================================*/
BOOL CAdoParameter::SetType(DataTypeEnum DataType)
{
 ASSERT(m_pParameter != NULL);
 try
 {
  m_pParameter->PutType(DataType);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetType 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

DataTypeEnum CAdoParameter::GetType()
{
 ASSERT(m_pParameter != NULL);
 try
 {
  return m_pParameter->GetType();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adEmpty;
 }
}

/*========================================================================
 Name:  表示 Parameter 对象的最大大小（按字节或字符）。
    ----------------------------------------------------------
 Params:  [size]: 表示 Parameter 对象的最大大小（按字节或字符）的长
   整型值。
==========================================================================*/
BOOL CAdoParameter::SetSize(int size)
{
 ASSERT(m_pParameter != NULL);
 try
 {
  m_pParameter->PutSize(long(size));
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

int CAdoParameter::GetSize()
{
 ASSERT(m_pParameter != NULL);
 try
 {
  return (int)m_pParameter->GetSize();

 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return -1;
 }
}

/*========================================================================
 Name:  指示对象的名称。
==========================================================================*/
BOOL CAdoParameter::SetName(CString strName)
{
 ASSERT(m_pParameter != NULL);
 try
 {
  m_pParameter->PutName(_bstr_t(LPCTSTR(strName)));
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

CString CAdoParameter::GetName()
{
 ASSERT(m_pParameter != NULL);
 try
 {
  return CString(LPCTSTR(m_pParameter->GetName()));
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return CString(_T(""));
 }
}

/*========================================================================
 Name:  指示 Parameter 所标明的是输入参数、输出参数还是既是输出又
  是输入参数，或该参数是否为存储过程返回的值。
    ----------------------------------------------------------
 Params:  [Direction]: 设置以下某个 ParameterDirectionEnum 值。
  [常量]    [说明] 
  -------------------------------------------
  AdParamUnknown  指示参数方向未知。 
  AdParamInput  默认值。指示输入参数。 
  AdParamOutput  指示输出参数。 
  AdParamInputOutput 同时指示输入参数和输出参数。 
  AdParamReturnValue 指示返回值。 
==========================================================================*/
BOOL CAdoParameter::SetDirection(ParameterDirectionEnum Direction)
{
 ASSERT(m_pParameter != NULL);
 try
 {
  m_pParameter->PutDirection(Direction);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 } 
}

ParameterDirectionEnum CAdoParameter::GetDirection()
{
 ASSERT(m_pParameter != NULL);
 try
 {
  return m_pParameter->GetDirection();
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return adParamUnknown;
 } 
}

/*########################################################################
     ------------------------------------------------

     ------------------------------------------------
  ########################################################################*/

BOOL CAdoParameter::SetValue(const  _variant_t &value)
{
 ASSERT(m_pParameter != NULL);

 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(VARIANT);
  }
  m_pParameter->Value = value;
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const bool &value)
{
 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(short);
  }
  m_pParameter->Value = _variant_t(value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const int &value)
{
 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(int);
  }
  m_pParameter->Value = _variant_t(long(value));
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const long &value)
{
 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(long);
  }
  m_pParameter->Value = _variant_t(value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const double &value)
{
 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(double);
  }
  m_pParameter->Value = _variant_t(value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const CString &value)
{
 _variant_t var;
 var.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
 var.bstrVal = value.AllocSysString();

 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = value.GetLength();
  }
  m_pParameter->Value = var;
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const COleDateTime &value)
{
 _variant_t var;
 var.vt = VT_DATE;
 var.date = value;

 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(DATE);
  }
  m_pParameter->Value = var;
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
 return TRUE;
}

BOOL CAdoParameter::SetValue(const BYTE &value)
{
 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(BYTE);
  }
  m_pParameter->Value = _variant_t(value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const short &value)
{
 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(short);
  }
  m_pParameter->Value = _variant_t(value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::SetValue(const float &value)
{
 try
 {
  if (m_pParameter->Size == 0)
  {
   m_pParameter->Size = sizeof(float);
  }
  m_pParameter->Value = _variant_t(value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(bool &value)
{
 try
 {
  value = vartobool(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(BYTE &value)
{
 try
 {
  value = vartoby(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(short &value)
{
 try
 {
  value = vartoi(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(int &value)
{
 try
 {
  value = (int)vartol(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(long &value)
{
 try
 {
  value = vartol(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(double &value)
{
 try
 {
  value = vartof(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(CString &value)
{
 try
 {
  value = vartostr(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}

BOOL CAdoParameter::GetValue(COleDateTime &value)
{
 try
 {
  value = vartodate(m_pParameter->Value);
  return TRUE;
 }
 catch (_com_error e)
 {
  TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d/n"), e.ErrorMessage(), __FILE__, __LINE__);
  return FALSE;
 }
}












