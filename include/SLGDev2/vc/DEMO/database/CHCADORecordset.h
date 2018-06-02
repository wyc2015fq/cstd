

// CHCADORecordset.h: interface for the CHCADORecordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UEADORECORDSET_H__2CE0052C_1060_423F_9513_7FF5AC552648__INCLUDED_)
#define AFX_UEADORECORDSET_H__2CE0052C_1060_423F_9513_7FF5AC552648__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CHCRecordset.h"

class CHCADORecordset : public CHCRecordset  
{
public:
	CHCADORecordset(_RecordsetPtr pRecordset);
	CHCADORecordset();
	virtual ~CHCADORecordset();
public:
	void Delete();

	BOOL GetBool(const CString& strIndex) const;
	CString GetCString(const CString& strIndex) const;
	FLOAT GetFloat(const CString& strIndex) const;
	DOUBLE GetDouble(const CString& strIndex) const;
	LONG GetLong(const CString& strIndex) const;
	short GetShort(const CString& strIndex) const;
	_variant_t Get_variant_t(const CString& strIndex) const;
	LONG GetChunk( const CString& strIndex, BYTE* buf = NULL, LONG len = 0) const;
	void NewRecord();
	
	void SetBool(const CString& strIndex, bool bValue);
	void SetCString(const CString& strIndex, const CString& strValue);
	void SetFloat(const CString& strIndex, FLOAT fValue);
	void SetDouble(const CString& strIndex, DOUBLE dbValue);
	void SetLong(const CString& strIndex, LONG nValue);
	void SetShort(const CString& strIndex, SHORT nValue);
	void Set_variant_t(const CString& strIndex,_variant_t vr);
	void AppendChunk( const CString& strIndex, BYTE* buf, int len);
	void BatchUpdate();
	BOOL Update();

	void MovePrevious();
	void MoveLast();
	void MoveNext();
	void MoveFirst();

	BOOL IsEOF() const;
	BOOL IsBOF() const;
	BOOL IsEmpty() const;
	void Close();
	
	INT GetRecordCount() const;

private:
	_RecordsetPtr m_pRecordset;
};

#endif // !defined(AFX_UEADORECORDSET_H__2CE0052C_1060_423F_9513_7FF5AC552648__INCLUDED_)
