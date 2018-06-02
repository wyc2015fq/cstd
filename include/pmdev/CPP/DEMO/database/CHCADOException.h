/*************************************************
  Description:    CHCADOException封装ADO中的_com_error异常
  Others:         
  Function List:  
    1. Delete:删除被抛出的异常（它自己）
	2. Description:获取异常的描述信息
	3. ReportError:向用户报告错误，以警告对话框的形式
*************************************************/


// CHCADOException.h: interface for the CHCADOException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UEADOEXCEPTION_H__08C141D4_7CFB_4323_B4EC_9A433E5C8F96__INCLUDED_)
#define AFX_UEADOEXCEPTION_H__08C141D4_7CFB_4323_B4EC_9A433E5C8F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CHCException.h"


class CHCADOException : public CHCException  
{
public:

	CHCADOException();
	virtual ~CHCADOException();
public:
	CHCADOException( _com_error& er );
	CHCADOException( HRESULT hr );
	void ReportError();
	CString Description();
	void Delete();


private:
	_com_error m_Error;
};

#endif // !defined(AFX_UEADOEXCEPTION_H__08C141D4_7CFB_4323_B4EC_9A433E5C8F96__INCLUDED_)
