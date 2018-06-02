// ShowImage.cpp : implementation file
//

#include "stdafx.h"
#include "ShowImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowImage

CShowImage::CShowImage()
{	
	m_path="";
	m_hGlobal = NULL;
	m_nReadLen=0;
	m_controlID=0;
	bLoad = FALSE;
	iPos = 0;
}

CShowImage::~CShowImage()
{
	/*
	if (pBuffer!=NULL)
	{
		delete [] pBuffer;
		pBuffer = NULL;
		m_nReadLen=0;
	}
	*/
}


BEGIN_MESSAGE_MAP(CShowImage, CStatic)
	//{{AFX_MSG_MAP(CShowImage)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowImage message handlers

void CShowImage::OnPaint() 
{
	//AoutLock lock( &m_Lock );

	CPaintDC dc(this); // device context for painting
	RECT rect; 
	GetClientRect(&rect);
	if ( m_hGlobal != NULL && m_nReadLen > 0 )
	{
		m_Pic.LoadPictureData( m_hGlobal, m_nReadLen );
		m_Pic.UpdateSizeOnDC(&dc);
		m_Pic.Show(&dc, rect );
		m_Pic.FreePictureData();
	}
    else
	{
		dc.SetBkMode(TRANSPARENT);
		dc.SelectStockObject(NULL);
		dc.Rectangle(&rect); 
	}
	// Do not call CStatic::OnPaint() for painting messages
}
CString CShowImage::getpicPath()
{
	return m_path;
}


//分配内存长度
BOOL CShowImage::AllocMemory( int nMemoryLen )
{
	//AoutLock lock( &m_Lock );

	m_hGlobal = GlobalAlloc(GMEM_MOVEABLE, nMemoryLen );
	
	if( m_hGlobal == NULL )
	{
		m_nMemoryLen = 0;
		return FALSE;	
	}
	else
	{
		m_nMemoryLen = 	nMemoryLen;
		return TRUE;	
	}

}


//释放内存
void CShowImage::FreeMemory()
{
	//AoutLock lock( &m_Lock );

	if( m_hGlobal != NULL )
	{
		GlobalFree( m_hGlobal );
		m_nMemoryLen = 0;
	}
	
	m_Pic.FreePictureData();
}



void CShowImage::SetPicPath( CString path )
{
	//AoutLock lock( &m_Lock );
	
	//没有给控件分配内存
	if( m_hGlobal == NULL )
	{
		Invalidate( TRUE );
		return;
	}
		

	CFile f;
	m_path=path;
	m_controlID=GetDlgCtrlID();
	CFileException e;
	CDC *pDC=GetDC(); 

	//清空内存
	m_nReadLen = 0;

	//文件存在
	if ( m_path != "" && _taccess( m_path.GetBuffer( 0 ), 0 ) != -1  )
	{
		
		//读取文件信息
		void* pData = GlobalLock(m_hGlobal);
		m_nReadLen = ch_readfile( (BYTE*)pData, m_nMemoryLen, m_path.GetBuffer( 0 ) );
		GlobalUnlock(m_hGlobal);

		DEBUG_INFO1( _T("ch_readfile:%d"), m_nReadLen );

		if( m_nReadLen <= 0 )
		{
			m_nReadLen = 0;
			Invalidate( TRUE );
			return;
		}
		else
		{
			RECT rect; 
			GetClientRect(&rect);
			m_Pic.LoadPictureData( m_hGlobal, m_nReadLen );
			m_Pic.UpdateSizeOnDC(pDC);
			m_Pic.Show( pDC, rect );
			bLoad = TRUE;	
		}
	}
	else
	{ 
		RECT rect; 
		GetClientRect(&rect);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectStockObject(NULL);
		pDC->Rectangle(&rect); 
		
	}

	ReleaseDC(pDC);
	m_Pic.FreePictureData();
	
	 
	return;

}

//得新刷新显示
void CShowImage::ResetPic()
{
	Invalidate( TRUE );	
}


int CShowImage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


BOOL CShowImage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CStatic::PreTranslateMessage(pMsg);
}

void CShowImage::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if( pBuffer && nSize>0 )
	//	::PostMessage(GetParent()->GetSafeHwnd(),DELETE_PIC,m_controlID,iPos - 1 );		
	
	CStatic::OnLButtonDblClk(nFlags, point);
}


void CShowImage::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	/*
	if (pBuffer!=NULL)
	{
		delete[] pBuffer;
		pBuffer = NULL;
		nPicSize = 0;
	}
	m_Pic.FreePictureData();
	*/
}

