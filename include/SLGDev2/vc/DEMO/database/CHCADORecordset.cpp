// CHCADORecordset.cpp: implementation of the CHCADORecordset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHCADORecordset.h"
#include "CHCADOException.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHCADORecordset::CHCADORecordset()
{
	this->m_pRecordset = NULL;
}

CHCADORecordset::~CHCADORecordset()
{
	Close();
}


int CHCADORecordset::GetRecordCount() const
{
	ASSERT(m_pRecordset != NULL);
	long lcount = 0;
	try
	{
		m_pRecordset->get_RecordCount(&lcount);
		return (int)lcount;
	}
	catch(_com_error e)
	{
		throw new CHCADOException(e);
	} 
}


void CHCADORecordset::Close()
{
	try
	{
		if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
		{
			SUCCEEDED(m_pRecordset->Close());
			m_pRecordset = NULL;
		}
	}
	catch (_com_error& e)
	{
		throw new CHCADOException(e);
	}
}



BOOL CHCADORecordset::IsBOF() const
{
	ASSERT(m_pRecordset != NULL);
	return m_pRecordset->BOF;
}



BOOL CHCADORecordset::IsEOF() const
{
	ASSERT(m_pRecordset != NULL);
	return m_pRecordset->adoEOF;
}



BOOL CHCADORecordset:: IsEmpty() const
{
	return IsEOF() && IsBOF();
}




void CHCADORecordset::MoveFirst()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MoveFirst());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
}




void CHCADORecordset::MovePrevious()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MovePrevious());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
}




void CHCADORecordset::MoveNext()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MoveNext());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}




void CHCADORecordset::MoveLast()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			SUCCEEDED(m_pRecordset->MoveLast());
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
}



BOOL CHCADORecordset::Update()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			HRESULT hr = m_pRecordset->Update();
			if (SUCCEEDED(hr))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	} 
	return FALSE;
}




void CHCADORecordset::BatchUpdate()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->UpdateBatch(adAffectAll);
	}
	catch (_com_error e) 
	{
		throw new CHCADOException(e);
	}
}

CHCADORecordset::CHCADORecordset(_RecordsetPtr m_pRecordset)
{
	this->m_pRecordset = m_pRecordset;
}




void CHCADORecordset::SetShort(const CString &strIndex, SHORT nValue)
{
	ASSERT( m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(nValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}




void CHCADORecordset::SetLong(const CString &strIndex, LONG nValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(nValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}



void CHCADORecordset::SetDouble(const CString &strIndex, DOUBLE dbValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(dbValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}




void CHCADORecordset::SetFloat(const CString &strIndex, FLOAT fValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(fValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}




void CHCADORecordset::SetCString(const CString &strIndex, const CString &strValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(strValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}




void CHCADORecordset::SetBool(const CString &strIndex, bool bValue)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),_variant_t(bValue));
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}



void CHCADORecordset::NewRecord()
{
	ASSERT( m_pRecordset != NULL);
	try
	{
		if (m_pRecordset->Supports(adAddNew))
		{
			m_pRecordset->AddNew();
		}
		else
		{
			AfxMessageBox(_T("Can't Inserted"));
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}




short CHCADORecordset::GetShort(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		short sdefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return sdefault;
		}
		return short(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}


LONG CHCADORecordset::GetLong(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		LONG ldefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return ldefault;
		}
		return long(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}



DOUBLE CHCADORecordset::GetDouble(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		double ddefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return ddefault;
		}
		return double(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}




FLOAT CHCADORecordset::GetFloat(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		float fdefault = 0;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return fdefault;
		}
		return float(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return 0;
}



CString CHCADORecordset::GetCString(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		_variant_t var;
		CString strdefault;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return strdefault;
		}
		return (TCHAR*)_bstr_t(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return _T("");
}




void CHCADORecordset::Delete()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->Delete(adAffectCurrent);
		m_pRecordset->Update();
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}



BOOL CHCADORecordset::GetBool(const CString &strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		BOOL bdefault = FALSE;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return bdefault;
		}
		return bool(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return false;
}



_variant_t CHCADORecordset::Get_variant_t(const CString& strIndex) const
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		_variant_t vdefault;
		_variant_t var;
		var = m_pRecordset->GetCollect(_variant_t(strIndex));
		if (var.vt == VT_NULL)
		{
			return vdefault;
		}
		return var;
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return false;
}



void CHCADORecordset::Set_variant_t(const CString& strIndex,_variant_t vr)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->PutCollect(_variant_t(strIndex),vr);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}

LONG CHCADORecordset::GetChunk( const CString& strIndex, BYTE* buf /*= NULL*/, LONG len /*= 0*/) const
{
	ASSERT(m_pRecordset != NULL);
	LONG ret = 0;
	try
	{
		if ( buf == NULL || len == 0 )
		{
			ret = m_pRecordset->GetFields()->GetItem( _variant_t(strIndex) )->ActualSize;
		}
		else
		{
			_variant_t varBLOB;	
			varBLOB = m_pRecordset->GetFields()->GetItem( _variant_t(strIndex) )->GetChunk(len);		
			if(varBLOB.vt == (VT_ARRAY | VT_UI1)) 			
			{
				char *pBuf = NULL;
				SafeArrayAccessData(varBLOB.parray,(void **)&pBuf); 
				memcpy( buf, pBuf, len);
	
			    SafeArrayUnaccessData (varBLOB.parray);	
				ret = len;
			}
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return ret;

}

void CHCADORecordset::AppendChunk( const CString& strIndex, BYTE* buf, int len)
{
	ASSERT(m_pRecordset != NULL);
	if ( NULL == buf || len <= 0 ) return;
	try
	{
		VARIANT varBLOB;
		SAFEARRAY *psa;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;		
		rgsabound[0].cElements = len;	
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound); 	
		for (long i = 0; i < (long)len; i++)		
			SafeArrayPutElement (psa, &i, buf++); 
		varBLOB.vt = VT_ARRAY | VT_UI1;
		varBLOB.parray = psa; 
		m_pRecordset->GetFields()->GetItem( _variant_t(strIndex) )->AppendChunk(varBLOB); 
		SafeArrayDestroy(psa);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}