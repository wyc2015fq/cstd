// CHCADOException.cpp: implementation of the CHCADOException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHCADOException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCADOException::CHCADOException():m_Error(0)
{
	
}

CHCADOException::~CHCADOException()
{

}


/*************************************************
  Function:       Delete
  Description:    删除异常对象本身
  Calls:          操作符delete
  Called By:      该类对象
  Table Accessed: 无
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

void CHCADOException::Delete()
{
	delete this;
}


/*************************************************
  Function:       Description
  Description:    获取对该异常的描述信息
  Calls:          _com_error方法Description
  Called By:      该类对象
  Table Accessed: 无
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         描述信息字符串
  Others:         
*************************************************/

CString CHCADOException::Description()
{
	_bstr_t tmp = this->m_Error.Description();
	TCHAR* strtmp = tmp;
	return strtmp;
}


/*************************************************
  Function:       ReportError
  Description:    向用户报告异常。
  Calls:          全局函数AfxMessageBox
  Called By:      该类对象
  Table Accessed: 无
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

void CHCADOException::ReportError()
{
	AfxMessageBox(this->Description());	
}

CHCADOException::CHCADOException(HRESULT hr):m_Error(hr)
{
	
}

CHCADOException::CHCADOException(_com_error& er):m_Error(er)
{

}
