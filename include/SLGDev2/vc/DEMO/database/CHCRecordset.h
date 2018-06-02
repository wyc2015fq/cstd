

// CHCRecordset.h: interface for the CHCRecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UERECORDSET_H__09C0D072_B717_4F35_9298_704C992A6CC2__INCLUDED_)
#define AFX_UERECORDSET_H__09C0D072_B717_4F35_9298_704C992A6CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CHCAutoPtr.h"
class CHCRecordset  
{
public:

	CHCRecordset();
	virtual ~CHCRecordset();
public:


	virtual void Delete() = 0;

	virtual void Close() = 0;


	virtual BOOL IsBOF() const = 0;

	virtual BOOL IsEmpty() const = 0;


	virtual BOOL IsEOF() const = 0;


	virtual INT GetRecordCount() const= 0;
	virtual void MoveNext() = 0;
	virtual void MovePrevious() = 0;
	virtual void MoveLast() = 0;
	virtual void MoveFirst() = 0;
	virtual void SetShort(const CString& strIndex, SHORT nValue) = 0;
	virtual void SetLong(const CString& strIndex, LONG nValue) = 0;
	virtual void SetDouble(const CString& strIndex, DOUBLE dbValue) = 0;
	virtual void SetFloat(const CString& strIndex, FLOAT fValue) = 0;
	virtual void SetCString(const CString& strIndex, const CString& strValue) = 0;
	virtual void Set_variant_t(const CString& strIndex,_variant_t vr) = 0;
	virtual void SetBool(const CString& strIndex, bool bValue) = 0;
	virtual void AppendChunk( const CString& strIndex, BYTE* buf, int len) = 0;


	virtual void BatchUpdate() = 0;
	virtual BOOL Update() = 0;
	
	virtual void NewRecord() = 0;
	virtual BOOL GetBool(const CString& strIndex) const= 0;
	virtual short GetShort(const CString& strIndex) const= 0;
	virtual LONG GetLong(const CString& strIndex) const= 0;
	virtual DOUBLE GetDouble(const CString& strIndex) const= 0;
	virtual FLOAT GetFloat(const CString& strIndex) const= 0;
	virtual CString GetCString(const CString& strIndex) const= 0;
	virtual _variant_t Get_variant_t(const CString& strIndex) const = 0;
	virtual LONG GetChunk( const CString& strIndex, BYTE* buf = NULL, LONG len = 0) const = 0;

};

typedef CHCAutoPtr<CHCRecordset> CHCRecordsetPtr;
#endif // !defined(AFX_UERECORDSET_H__09C0D072_B717_4F35_9298_704C992A6CC2__INCLUDED_)
