// TimeGroup.h: interface for the CTimeGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEGROUP_H__5EB16F76_CCFA_4C2A_B9C4_EDAAE9DB1031__INCLUDED_)
#define AFX_TIMEGROUP_H__5EB16F76_CCFA_4C2A_B9C4_EDAAE9DB1031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimeGroup  
{
public:
	CTimeGroup();
	virtual ~CTimeGroup();

public:
	//
	void UnInitData();
	//
	BOOL InitData( DBManager& DB  );
	
public:
	//
	list< PTIMEGROUP >& Get()
	{
		return m_lTimeGroupLst;
	}
	//
	int	Size()
	{
		return m_lTimeGroupLst.size();	
	}
	
public:
	//
	BOOL TimeGroup_NewID( DBManager& DB, int& nTimeGroupID );
	//
	BOOL TimeGroup_Add( DBManager& DB, TIMEGROUP& stTimeGroupData );
	//
	BOOL TimeGroup_Alter( DBManager& DB, TIMEGROUP& DeptData );
	//
	BOOL TimeGroup_Del( DBManager& DB, int nTimgGroupID );
	//
	BOOL TimeGroup_IsExist( DBManager& DB, int nTimgGroupID, BOOL& bExist );
	//
	BOOL TimeGroup_IsExist( DBManager& DB, TCHAR* szTimeGroupName,int nTimeGroupID, BOOL& bExist );
	//
	PTIMEGROUP Find( int TimeGroupID );
	
protected:
	MemoryPool				m_Pool;		//
	list< PTIMEGROUP >		m_lTimeGroupLst;	//
};

#endif // !defined(AFX_TIMEGROUP_H__5EB16F76_CCFA_4C2A_B9C4_EDAAE9DB1031__INCLUDED_)
