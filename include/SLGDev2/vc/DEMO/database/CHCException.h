
// CHCException.h: interface for the CHCException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UEEXCEPTION_H__D8F112C6_C529_43C4_9BCD_89638140AC07__INCLUDED_)
#define AFX_UEEXCEPTION_H__D8F112C6_C529_43C4_9BCD_89638140AC07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHCException  
{
public:

	CHCException();
	virtual ~CHCException();
public:

	virtual void ReportError() = 0;
	virtual void Delete() = 0;
	virtual CString Description() = 0;
	

};

#endif // !defined(AFX_UEEXCEPTION_H__D8F112C6_C529_43C4_9BCD_89638140AC07__INCLUDED_)
