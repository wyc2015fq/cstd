// TimeGroup.cpp: implementation of the CTimeGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeGroup::CTimeGroup()
{
	m_lTimeGroupLst.clear();
	m_Pool.FreePool();
}

CTimeGroup::~CTimeGroup()
{

}

//反初始化函数
void CTimeGroup::UnInitData()
{

}
//初始化函数
BOOL CTimeGroup::InitData( DBManager& DB  )
{
	int nRlt = 0;
	//清空老数据
	m_lTimeGroupLst.clear();
	
	//重置内存池
	m_Pool.ResetPool();
	
	if( m_Pool.InitPool( MAX_TIMEGROUP_NUM,sizeof(TIMEGROUP) ) == FALSE )
		return FALSE;
	
	//加载操作员
	nRlt = DB.TimeGroup_GetGroupInfo( m_Pool, m_lTimeGroupLst );
	
	//加载失败
	if( nRlt != DB_OK && nRlt != DB_NODATA )
	{
		//清空老数据
		m_lTimeGroupLst.clear();
		
		//重置内存池
		m_Pool.ResetPool();	
		
		return FALSE;
	}
	
	//加载成功在保存新的数据
	return TRUE;	
}
//时间组ID
BOOL CTimeGroup::TimeGroup_NewID( DBManager& DB, int& nTimeGroupID )
{
	return DB.TimeGroup_NewID( nTimeGroupID ) == DB_OK?TRUE:FALSE;
}
//添加一个时间组
BOOL CTimeGroup::TimeGroup_Add( DBManager& DB, TIMEGROUP& stTimeGroupData )
{
	PTIMEGROUP pTimeGroup = (PTIMEGROUP)m_Pool.AllocBlock();
	if( pTimeGroup == NULL )
		return FALSE;
	
	DB_RESULT Rlt = DB.TimeGroup_Add( stTimeGroupData );
	if( Rlt != DB_OK )
	{
		m_Pool.FreeBlock( pTimeGroup );
		return FALSE;
	}
	
	CopyMemory( pTimeGroup, &stTimeGroupData, sizeof( TIMEGROUP ) );
	m_lTimeGroupLst.push_back( pTimeGroup );
	return TRUE;
}
//修改一个时间组信息
BOOL CTimeGroup::TimeGroup_Alter( DBManager& DB, TIMEGROUP& stTimeGroup )
{
	DB_RESULT Rlt = DB.TimeGroup_Alter( stTimeGroup );
	if( Rlt != DB_OK )
		return FALSE;
	
	for (list<PTIMEGROUP>::iterator iter = m_lTimeGroupLst.begin(); iter != m_lTimeGroupLst.end();iter++ )
	{
		if( (*iter)->m_nTimeGroupID == stTimeGroup.m_nTimeGroupID )
		{
			CopyMemory( *iter, &stTimeGroup, sizeof( TIMEGROUP ) );
			break;
		}
	}
	return TRUE;
}
//删除一个时间组信息
BOOL CTimeGroup::TimeGroup_Del( DBManager& DB, int nTimgGroupID )
{
	DB_RESULT Rlt = DB.TimeGroup_Del( nTimgGroupID );
	if( Rlt != DB_OK )
		return FALSE;
	
	for (list<PTIMEGROUP>::iterator iter = m_lTimeGroupLst.begin(); iter != m_lTimeGroupLst.end();iter++ )
	{
		if( (*iter)->m_nTimeGroupID == nTimgGroupID )
		{		
			m_Pool.FreeBlock( *iter );
			m_lTimeGroupLst.erase( iter );
			break;
		}
	}
	return TRUE;
}
//部门是否存在
BOOL CTimeGroup::TimeGroup_IsExist( DBManager& DB, int nTimgGroupID, BOOL& bExist )
{
	return DB.TimeGroup_IsExist( nTimgGroupID, bExist ) == DB_OK?TRUE:FALSE;
}
BOOL CTimeGroup::TimeGroup_IsExist( DBManager& DB, TCHAR* szTimeGroupName,int nTimeGroupID,BOOL& bExist )
{
	return DB.TimeGroup_IsExist( szTimeGroupName, nTimeGroupID,bExist ) == DB_OK?TRUE:FALSE;
}
//查找一个部门的信息
PTIMEGROUP CTimeGroup::Find( int nTimeGroupID )
{
	for (list<PTIMEGROUP>::iterator iter = m_lTimeGroupLst.begin(); iter != m_lTimeGroupLst.end();iter++ )
	{
		if( (*iter)->m_nTimeGroupID == nTimeGroupID )
		{		
			return *iter;
			break;
		}
	}
	return NULL;
}