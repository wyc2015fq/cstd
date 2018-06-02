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
	//反初始化函数
	void UnInitData();
	//初始化函数
	BOOL InitData( DBManager& DB  );
	
public:
	//获取部门信息
	list< PTIMEGROUP >& Get()
	{
		return m_lTimeGroupLst;
	}
	//获取部门个数
	int	Size()
	{
		return m_lTimeGroupLst.size();	
	}
	
public:
	//时间组ID
	BOOL TimeGroup_NewID( DBManager& DB, int& nTimeGroupID );
	//添加一个时间组
	BOOL TimeGroup_Add( DBManager& DB, TIMEGROUP& stTimeGroupData );
	//修改一个时间组信息
	BOOL TimeGroup_Alter( DBManager& DB, TIMEGROUP& DeptData );
	//删除一个时间组信息
	BOOL TimeGroup_Del( DBManager& DB, int nTimgGroupID );
	//部门是否存在
	BOOL TimeGroup_IsExist( DBManager& DB, int nTimgGroupID, BOOL& bExist );
	//部门是否存在
	BOOL TimeGroup_IsExist( DBManager& DB, TCHAR* szTimeGroupName,int nTimeGroupID, BOOL& bExist );
	//查找一个部门的信息
	PTIMEGROUP Find( int TimeGroupID );
	
protected:
	MemoryPool				m_Pool;		//内存池
	list< PTIMEGROUP >		m_lTimeGroupLst;	//时间组信息
};

#endif // !defined(AFX_TIMEGROUP_H__5EB16F76_CCFA_4C2A_B9C4_EDAAE9DB1031__INCLUDED_)
