// MemPool.cpp: implementation of the CMemPool class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MemPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemPool::CMemPool():
m_pHead(NULL),
m_pCur(NULL),
m_pLast(NULL),
m_pbMemPool(NULL)
{
}

CMemPool::~CMemPool()
{
	FreeAllMem();
}

CMemPool& CMemPool::GetInstance()
{
	static CMemPool ins;
	return ins;
}

/********************************************************************  
/*函数名称:	CreateMemPool		
/*创建时间: 2011/07/08 16:22 
/*参	数:	nPoolSize  内存池大小
nUnitSize  内存池基本单元
/*文件作者: lcy  
/*功能说明: 申请一块nPoolSize大小的内存，并将此内存分解为若干nUnitSize
大小基本单元，通过链表将基本单元给串联起来  
*********************************************************************/
void CMemPool::CreateMemPool( uint32 nPoolSize, uint32 nUnitSize )
{
	FreeAllMem();
	m_nUnitsize = ALIGN(nUnitSize, 4);
	int32 blocks= CalcueMemBlock( ALIGN(nPoolSize,4) );
	m_nPoolSize = blocks*m_nUnitsize;
	m_pHead = new MEMBLOCK[blocks+1];
	m_pLast = m_pHead+blocks;
	memset(m_pHead, 0x0, sizeof(MEMBLOCK)*(blocks+1));
	m_pbMemPool = (int8*)(new int32[m_nPoolSize>>2]);
	AssignMemToBlock(m_pHead, m_nPoolSize, m_pbMemPool);
	m_pCur = m_pHead;
}

/********************************************************************  
/*函数名称:	AssinMemToBlock		
/*创建时间: 2011/07/08 16:31 
/*参	数:	pFirst 链表首地址
nPoolSize 内存池大小
pRawMem 内存池首地址
/*文件作者: lcy  
/*功能说明: 将内存池分解为若干个基本块，并将基本块绑定到链表节点   
*********************************************************************/ 
void CMemPool::AssignMemToBlock(PMEMBLOCK pFirst, uint32 nPoolSize, int8* pRawMem)
{
	int32 nChunks = CalcueMemBlock(nPoolSize);
	pFirst[0].m_bFirst = true;
	for ( int32 i = 0; i < nChunks; ++i )
	{
		pFirst[i].m_nSize = nPoolSize - i*m_nUnitsize;
		pFirst[i].m_nUsed = 0;
		pFirst[i].m_pbBlock = pRawMem + i*m_nUnitsize;
		pFirst[i].m_pNext = &(pFirst[i+1]);
	}
}

/********************************************************************  
/*函数名称:	CalcueMemBlock		
/*创建时间: 2011/07/08 16:34 
/*参	数:	size 内存池大小
/*文件作者: lcy  
/*功能说明: 根据内存池基本单元大小计算出实际将要申请的内存池大小  
*********************************************************************/ 
int32 CMemPool::CalcueMemBlock( uint32 size )
{
	int32 blocks = size / m_nUnitsize;
	if ( size % m_nUnitsize ) blocks += 1;
	return blocks;
}

/********************************************************************  
/*函数名称:	GetMem		
/*创建时间: 2011/07/08 16:36 
/*参	数:	nSize 内存大小
/*文件作者: lcy  
/*功能说明: 从内存池中申请nSize大小的内存  
*********************************************************************/ 
int8* CMemPool::GetMem( uint32 nSize, bool bCreate )
{
	if ( nSize <= 0 ) return NULL;
	
	AoutLock lock( &m_syn );
	PMEMBLOCK pRet = FindSuitbleBlock( ALIGN(nSize,4) );
	if ( !pRet && bCreate ) //当内存池中剩余空闲的内存大小小雨nsize时重新申请一块内存
	{
		int32 nNeedChunks = CalcueMemBlock( ALIGN(nSize,4) );
		int nNeedSize = nNeedChunks*m_nUnitsize;
		int8* pNewMem = (int8*)(new int32[nNeedSize>>2]);
		PMEMBLOCK pNewBlock = new MEMBLOCK[nNeedChunks+1];
		memset( pNewBlock, 0x0, sizeof(MEMBLOCK)*(nNeedChunks+1));
		m_pLast->m_pNext = pNewBlock;
		m_pLast = pNewBlock+nNeedChunks;
		AssignMemToBlock(pNewBlock, nNeedSize, pNewMem);
		m_nPoolSize += nNeedSize;
		pRet = pNewBlock;
		pRet->m_nUsed = nNeedSize;
		DEBUG_INFO1( _T("CreateMemPool=%d"), nNeedSize );
	} 
	if ( pRet == NULL ) return NULL;
	m_pCur = SkipBlock( pRet );
	return pRet->m_pbBlock;
}

/********************************************************************  
/*函数名称:	FindSuitbleBlock		
/*创建时间: 2011/07/08 16:37 
/*参	数:	nSize 内存大小
/*文件作者: lcy  
/*功能说明: 从内存池中找一块大于或等于nszie的内存  
*********************************************************************/ 
PMEMBLOCK CMemPool::FindSuitbleBlock( uint32 nSize )
{
	int32 nChunks = CalcueMemBlock(m_nPoolSize);
	int32 nNeedChunks = CalcueMemBlock( nSize );
	int32 nNeedSize = nNeedChunks*m_nUnitsize;
	
	PMEMBLOCK pRet = NULL;
	PMEMBLOCK pCur = m_pCur;
	for ( int32 i = 0; (i < nChunks) && (NULL != pCur); ++i )
	{
		if ( pCur == m_pLast )
		{
			pCur = m_pHead;
		}
		if ( pCur->m_nSize >= nNeedSize )
		{
			pRet = pCur;
			bool bFind = true;
			for ( int32 j = 0; j < nNeedChunks; ++j )
			{
				assert( m_pLast != pCur );
				assert( pCur->m_nSize );
				if ( pCur->m_nUsed )
				{
					bFind = false;
					break;
				}
				pCur = pCur->m_pNext;
			}
			if ( bFind )
			{	
				break;
			}
			else
			{
				pRet = NULL;
			}
		}
		pCur = SkipBlock( pCur );
	}
	if ( pRet )
	{
		pRet->m_nUsed = nNeedSize;
	}
	return pRet;
}

/********************************************************************  
/*函数名称:	SkipBlock		
/*创建时间: 2011/07/08 16:39 
/*参	数:	pCur   链表节点
nSkips 链表节点跳跃个数
/*文件作者: lcy  
/*功能说明: 从pCur跳跃nSkips个节点  
*********************************************************************/ 
PMEMBLOCK CMemPool::SkipBlock( PMEMBLOCK pCur )
{
	int nSkips = CalcueMemBlock( pCur->m_nUsed );
	if( nSkips <= 0 ) nSkips = 1;
	int32 i = 0;
	for ( ; i < nSkips; ++i )
	{
		pCur = pCur->m_pNext;
	}
	return pCur;
}

/********************************************************************  
/*函数名称:	FreeMem		
/*创建时间: 2011/07/08 16:42 
/*参	数:	pMem 内存首地址
nMemSize 内存大小
/*文件作者: lcy  
/*功能说明: 释放内存到内存池，以供后续使用  
*********************************************************************/ 
void CMemPool::FreeMem( int8* pMem )
{
	if ( pMem == NULL ) return ;
	AoutLock lock( &m_syn );
	PMEMBLOCK tmp = m_pHead;
	while ( tmp != m_pLast )
	{
		if ( tmp->m_pbBlock == pMem )
		{		
			tmp->m_nUsed = 0;
			m_pCur = tmp;
			break;
		}
		tmp = tmp->m_pNext;
	}
	assert( m_pCur == tmp );
}

/********************************************************************
/*函数名称:	FreeAllMem		
/*创建时间: 2011/07/08 16:42 
/*参	数:	
/*文件作者: lcy  
/*功能说明: 释放内存池 
*********************************************************************/ 
void CMemPool::FreeAllMem()
{
	PMEMBLOCK tmp  = NULL;
	PMEMBLOCK pCur = m_pHead;
	while ( pCur )
	{	
		tmp  = pCur;
		pCur = pCur->m_pNext;
		while ( pCur != NULL && !(pCur->m_bFirst) )
		{
			pCur = pCur->m_pNext;
		}
		assert( tmp->m_bFirst  );
		delete [] tmp->m_pbBlock;
		delete [] tmp;
	}
	m_pHead     = NULL;
	m_pCur      = NULL;
	m_pLast     = NULL;
	m_pbMemPool = NULL;
}
