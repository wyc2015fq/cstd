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

/*************************************************
  Function:       GetRecordCount
  Description:    获取当前记录集的记录数目
  Calls:          
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         int值（记录数目）
  Others:         
*************************************************/

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


/*************************************************
  Function:       Close
  Description:    关闭当前记录集
  Calls:          ADO方法Close
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       IsBOF
  Description:    判断是否位于记录集的最开始
  Calls:          
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         BOOL值
  Others:         
*************************************************/

BOOL CHCADORecordset::IsBOF() const
{
	ASSERT(m_pRecordset != NULL);
	return m_pRecordset->BOF;
}

/*************************************************
  Function:       IsEOF
  Description:    判断是否位于记录集的最开始
  Calls:          
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         BOOL值
  Others:         
*************************************************/

BOOL CHCADORecordset::IsEOF() const
{
	ASSERT(m_pRecordset != NULL);
	return m_pRecordset->adoEOF;
}

/*************************************************
  Function:       IsEmpty
  Description:    判断记录集是否为空
  Calls:          
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         BOOL值
  Others:         
*************************************************/

BOOL CHCADORecordset:: IsEmpty() const
{
	return IsEOF() && IsBOF();
}


/*************************************************
  Function:       MoveFirst
  Description:    移动到记录集的最开始
  Calls:          ADO方法MoveFirst
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       MovePrevious
  Description:    移动到记录集的最开始
  Calls:          ADO方法MovePrevious 
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       MoveNext
  Description:    移动到当前记录的下一个位置
  Calls:          ADO方法MoveNext
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       MoveLast
  Description:    移动到记录集的最后
  Calls:          ADO方法MoveLast
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       Update
  Description:    对当前记录集进行批更新
  Calls:          ADO方法Update
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       BatchUpdate
  Description:    对当前记录集进行批更新
  Calls:          ADO方法UpdateBatch
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

void CHCADORecordset::BatchUpdate()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->UpdateBatch(adAffectAll);//默认方式
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


/*************************************************
  Function:       SetShort(const CString &strIndex, SHORT nValue)
  Description:    向当前记录strIndex所代表列写SHORT值
  Calls:          ADO方法PutCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          const CString &strIndex:列名
				  SHORT nValue:待设置的值
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       SetLong(const CString &strIndex, LONG nValue)
  Description:    向当前记录strIndex所代表列写LONG值
  Calls:          ADO方法PutCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          const CString &strIndex:列名
				  LONG nValue:待设置的值
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       SetDouble(const CString &strIndex, DOUBLE dbValue)
  Description:    向当前记录strIndex所代表列写Double值
  Calls:          ADO方法PutCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          const CString &strIndex:列名
				  DOUBLE dbValue:待设置的值
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       SetFloat(const CString &strIndex, FLOAT fValue)
  Description:    向当前记录strIndex所代表列写FLOAT值
  Calls:          ADO方法PutCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          const CString &strIndex:列名
				  FLOAT fValue:待设置的值
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       SeCString(const CString &strIndex, const CString &strValue)
  Description:    向当前记录strIndex所代表列写字符串
  Calls:          ADO方法PutCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          const CString &strIndex:列名
				  const CString &strValue:待设置的值
  Output:         无
  Return:         无
  Others:         
*************************************************/

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


/*************************************************
  Function:       SetBool(const CString &strIndex, BOOL bValue)
  Description:    向当前记录strIndex所代表列写BOOL值
  Calls:          ADO方法PutCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          const CString &strIndex:列名
				  BOOL bValue:待设置的值
  Output:         无
  Return:         无
  Others:         
*************************************************/

void CHCADORecordset::SetBool(const CString &strIndex, BOOL bValue)
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


/*************************************************
  Function:       NewRecord
  Description:    向当前记录集增加一个空白记录，必须在
				  以后某个时候设置该记录的内容
  Calls:          ADO方法Supports，AddNew
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

void CHCADORecordset::NewRecord()
{
	ASSERT( m_pRecordset != NULL);
	try
	{
		if (m_pRecordset->Supports(adAddNew))
		{
			m_pRecordset->AddNew();//只产生一条新记录，没有填充值。
		}
		else
		{
			AfxMessageBox(_T("不能插入"));
		}
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       GetShort(const CString &strIndex)
  Description:    从当前记录得到一个BOOL值
  Calls:          ADO方法GetCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          const CString &strIndex:代表要取的列
  Output:         无
  Return:         Short值
  Others:         
*************************************************/

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


/*************************************************
  Function:       GetLong(const CString &strIndex)
  Description:    从当前记录得到一个BOOL值
  Calls:          ADO方法GetCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          const CString &strIndex:代表要取的列
  Output:         无
  Return:         Long值
  Others:         
*************************************************/

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


/*************************************************
  Function:       GetDouble(const CString &strIndex)
  Description:    从当前记录得到一个BOOL值
  Calls:          ADO方法GetCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          const CString &strIndex:代表要取的列
  Output:         无
  Return:         Double值
  Others:         
*************************************************/

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


/*************************************************
  Function:       GetFloat(const CString &strIndex)
  Description:    从当前记录得到一个BOOL值
  Calls:          ADO方法GetCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          const CString &strIndex:代表要取的列
  Output:         无
  Return:         Float值
  Others:         
*************************************************/

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


/*************************************************
  Function:       GeCString(const CString &strIndex)
  Description:    从当前记录得到一个BOOL值
  Calls:          ADO方法GetCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          const CString &strIndex:代表要取的列
  Output:         无
  Return:         字符串
  Others:         
*************************************************/

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


/*************************************************
  Function:       Delete
  Description:    删除当前记录
  Calls:          ADO方法Delete，Update
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/

void CHCADORecordset::Delete()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		m_pRecordset->Delete(adAffectCurrent);//默认。仅删除当前记录。
		m_pRecordset->Update();
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}


/*************************************************
  Function:       GetBool(const CString &strIndex)
  Description:    从当前记录得到一个BOOL值
  Calls:          ADO方法GetCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          const CString &strIndex:代表要取的列
  Output:         无
  Return:         BOOL值
  Others:         
*************************************************/

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
		return BOOL(var);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
	return false;
}

/*************************************************
  Function:       Get_variant_t(const CString &strIndex)
  Description:    从当前记录得到一个_variant_t值
  Calls:          ADO方法GetCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  无
  Input:          const CString &strIndex:代表要取的列
  Output:         无
  Return:         _variant_t值
  Others:         
*************************************************/

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

/*************************************************
  Function:       Set_variant_t(const CString& strIndex,_variant_t vr)
  Description:    向当前记录strIndex所代表列写_variant_t值
  Calls:          ADO方法PutCollect
  Called By:      该类对象
  Table Accessed: 当前记录集
  Table Updated:  当前记录集
  Input:          const CString &strIndex:列名
				  _variant_t vr:待设置的值
  Output:         无
  Return:         无
  Others:         
*************************************************/

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
			if(varBLOB.vt == (VT_ARRAY | VT_UI1)) ///判断数据类型是否正			
			{
				char *pBuf = NULL;
				SafeArrayAccessData(varBLOB.parray,(void **)&pBuf); 
				memcpy( buf, pBuf, len);
				//得到指向数据的指针 
			    /*****在这里我们可以对pBuf中的数据进行处理*****/	
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
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound); ///创建SAFEARRAY对象		
		for (long i = 0; i < (long)len; i++)		
			SafeArrayPutElement (psa, &i, buf++); ///将pBuf指向的二进制数据保存到SAFEARRAY对象psa中 		
		varBLOB.vt = VT_ARRAY | VT_UI1;///将varBLOB的类型设置为BYTE类型的数组	
		varBLOB.parray = psa; ///为varBLOB变量赋值 
		m_pRecordset->GetFields()->GetItem( _variant_t(strIndex) )->AppendChunk(varBLOB); ///加入BLOB类型的数据
		SafeArrayDestroy(psa);
	}
	catch (_com_error e)
	{
		throw new CHCADOException(e);
	}
}