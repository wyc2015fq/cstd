// video.cpp: implementation of the video class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../dev/common.h"
#include "video.h"
#include "yuv2bmp.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVideo::CVideo( CNet& net ) :
m_Net(net)
{
	InitialTable();
}

CVideo::~CVideo()
{

}

bool CVideo::StartVideo( const char* cszIP, long hwnd )
{
	AoutLock lock( &m_syn );
	unsigned long l = DotIpToLong(cszIP);
	SOCK_MAP::iterator it = m_map.find( l );
	if ( it == m_map.end() )
	{
		SOCK_ADDRESS addr = {0};
		HANDLE hVideo = m_Net.CreateVideoHandle();
		if ( !m_Net.GetVideoHandleAddr( hVideo, addr ) )
		{
			m_Net.ReleaseVideoHanle( hVideo );
			return false;
		}
		VideoPro* pro = new VideoPro;
		pro->m_hVideoHandle = hVideo;
		pro->m_Addr         = addr;
		pro->m_pRGBTMP      = NULL;
		pro->m_pRGBTRANS    = NULL;
		pro->m_pBitmapInfo  = NULL;
		pro->m_hWnd         = (HWND)hwnd;
		::GetWindowRect( (HWND)hwnd, &(pro->m_Rect) );
		m_map.insert( std::pair<unsigned long, VideoPro*>(l, pro) );
		DEBUG_INFO3( "StartVideo HWND = %d , W = %d , H = %d" , (int)pro->m_hWnd , pro->m_Rect.right-pro->m_Rect.left , pro->m_Rect.bottom-pro->m_Rect.top );
	}
	else
	{
		it->second->m_hWnd = (HWND)hwnd;
		::GetWindowRect( (HWND)hwnd, &(it->second->m_Rect) );
	}
	return true;
}

void CVideo::StopVideo( const char* cszIP )
{
	AoutLock lock( &m_syn );
	unsigned long l = DotIpToLong(cszIP);
	SOCK_MAP::iterator it = m_map.find( l );
	if ( it != m_map.end() )
	{
		m_Net.ReleaseVideoHanle( it->second->m_hVideoHandle );
		delete [] it->second->m_pRGBTMP;
		it->second->m_pRGBTMP = NULL;
		delete [] it->second->m_pRGBTRANS;
		it->second->m_pRGBTRANS = NULL;
		free(it->second->m_pBitmapInfo);
		it->second->m_pBitmapInfo = NULL;
		//::ReleaseDC( it->second->m_hWnd, it->second->m_hDC );
		delete it->second;
		m_map.erase( it );
	}
}

void CVideo::GetVideoTransAddr( const char* cszIP,  SOCK_ADDRESS& Addr )
{
	unsigned long l = DotIpToLong(cszIP);
	SOCK_MAP::iterator it = m_map.find( l );
	if ( it != m_map.end() )
	{
		Addr = it->second->m_Addr;
	}
}

void CVideo::ShowVideo( const char* cszIP, const char* cpbImage, int w, int h, int step, int nImageType )
{
	AoutLock lock( &m_syn );

	VideoPro* pro   = NULL;
	unsigned long l = DotIpToLong(cszIP);

	SOCK_MAP::iterator it = m_map.find( l );

	if ( it != m_map.end() )
	{
		pro = it->second;
	}

	if ( NULL == pro ) return;
	
	if ( NULL ==  pro->m_pRGBTMP || w != pro->m_pBitmapInfo->bmiHeader.biWidth || h != pro->m_pBitmapInfo->bmiHeader.biHeight )
	{
		delete [] pro->m_pRGBTMP;
		delete [] pro->m_pRGBTRANS;
		free( pro->m_pBitmapInfo );

		pro->m_pRGBTMP     = new BYTE[w*h*3];
		pro->m_pRGBTRANS   = new BYTE[w*h*3];
		pro->m_pBitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));	

		pro->m_pBitmapInfo->bmiHeader.biSize          = sizeof(BITMAPINFO);
		pro->m_pBitmapInfo->bmiHeader.biPlanes        = 1;
		pro->m_pBitmapInfo->bmiHeader.biBitCount      = 24;
		pro->m_pBitmapInfo->bmiHeader.biCompression   = BI_RGB;
		pro->m_pBitmapInfo->bmiHeader.biSizeImage     = 0;
		pro->m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 11811;
		pro->m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 11811;
		pro->m_pBitmapInfo->bmiHeader.biClrUsed       = 0;
		pro->m_pBitmapInfo->bmiHeader.biClrImportant  = 0;
	}
	
	if ( IMAGE_UYVY == nImageType )
	{
		pro->m_pBitmapInfo->bmiHeader.biWidth  = h;
		pro->m_pBitmapInfo->bmiHeader.biHeight = w;
		YUV422_C_RGB( (BYTE*)cpbImage, pro->m_pRGBTMP, h, w );
		RGB_EDDY( pro->m_pRGBTMP, h, w, pro->m_pRGBTRANS );
	}
	else if ( IMAGE_YUYV == nImageType )
	{
		pro->m_pBitmapInfo->bmiHeader.biWidth  = w;
		pro->m_pBitmapInfo->bmiHeader.biHeight = h;
		YUYV_C_RGB( (BYTE*)cpbImage, pro->m_pRGBTRANS, h, w );
	}
	
	long lStillWidth  = pro->m_Rect.right  - pro->m_Rect.left;
	long lStillHeight = pro->m_Rect.bottom - pro->m_Rect.top;
	
	HDC hdc = ::GetDC( (HWND)(pro->m_hWnd) );

	::SetStretchBltMode( hdc, COLORONCOLOR);

	::StretchDIBits(
		hdc, 0, 0,  
		lStillWidth, lStillHeight,  
		0, 0, 
		pro->m_pBitmapInfo->bmiHeader.biWidth, 
		pro->m_pBitmapInfo->bmiHeader.biHeight,  
		pro->m_pRGBTRANS,  
		pro->m_pBitmapInfo,
		DIB_RGB_COLORS,  
		SRCCOPY );

	::ReleaseDC( pro->m_hWnd, hdc );
}