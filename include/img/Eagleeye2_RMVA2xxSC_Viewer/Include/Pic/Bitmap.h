//////////////////////////////////////////////////////////////////
//																//
//		用途 : Bitmap ( DDB ) 操作								//
//		创建 : [Foolish] / 2001-1-18							//
//		更新 : 2002-12-25										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef	 __FOO_BITMAP_H__
#define	 __FOO_BITMAP_H__
#pragma once
#include <windows.h>
#include "..\StdDefine.h"

//===================================================================
//	DDB 操作类
//===================================================================
class FCBitmap
{
public :
	FCBitmap () ;
	FCBitmap (const FCBitmap & bmp) ;
	~FCBitmap () ;
	FCBitmap & operator= (const FCBitmap & bmp) ;
	void	SetCpuSpeed (int iFreq) ; // 设置CPU主频, DrawEx中用
	int		GetCpuSpeed () const ;
	void	FillRect (HBRUSH hBrush, const RECT * rcFill = NULL) ;
	void	FrameRgn (HBRUSH hBrush, HRGN hRgn, int nWidth = 1) ;
	HBITMAP	Clone (HDC hdc = NULL) const ; // 复制出一个HBITMAP

	// 获取一DDB
	BOOL	CreateCompatibleBitmap (int iWidth, int iHeight) ;
	void	Attach (HBITMAP	hBitmap) ; // Attach a DDB, 不删除当前位图
	BOOL	LoadBitmap (PCTSTR szResBitmap) ;
	BOOL	LoadBitmap (UINT uIDBitmap) ;
	BOOL	LoadBitmapFromFile (PCTSTR szFileName) ;
	BOOL	LoadOEMBitmap (UINT uIDBitmap) ; // 在WinUser.h中定义 OBM_*
	HBITMAP	Detach () ;	// 和Attach成对调用, 不删除m_hBitmap, 只把它置NULL。并返回当前位图句柄。
	void	Clear () ; // 删除m_hBitmap

	// 获取DDB信息
	HBITMAP	GetHandle () const ;
	void	GetBitmapInfo (BITMAP * pBitmap) const ;
	int		GetWidth () const ;
	int		GetHeight () const ;
	int		GetColorBit () const ;

	// 缩放系数 (m_nMin to -1) & (1 to m_nMax)
	void	SetZoomScale (int iScale) ;
	int		GetZoomScale () const ;
	void	IncreaseZoomScale () ;
	void	DecreaseZoomScale () ;
	int		GetScaledWidth () const ;
	int		GetScaledHeight () const ;
	void	MapScaledPoint (POINT & pt) ;
	void	MapScaledRect (RECT & rect) ;
	void	MapRealPoint (POINT & pt) ;
	void	MapRealRect (RECT & rect) ;
	void	ScaleDraw (HDC hdc, int xDcPos, int yDcPos, int nWidth, int nHeight) ; // 由缩放系数控制的显示

	// 中断控制
	BOOL	IsContinueShow () const ;
	void	SetContinueFlag (BOOL bContinue = TRUE) ;

	// alpha 为当前位图的权值 [0..100]
	void	AlphaBlendDraw (HDC hdc, int alpha, int x = 0, int y = 0) ;
	void	Draw (HDC hdc, int xDest = 0, int yDest = 0,
						   int xSrc = 0, int ySrc = 0) ;
	void	DrawEx (HDC hdc, SHOWEFFECT ShowParam) ; // 详见StdDefine.h

protected :
	HBITMAP		m_hBitmap ;
	int			m_iScale ;
	int			m_iFreqCount ; // 每微秒的count (frequency / 1M)
	int			m_nMax ; // limit of zoom in == 16
	int			m_nMin ; // limit of zoom out == -16
	BOOL		m_bContinue ;
protected :
	void	ShowDib_SCAN (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_MOVE (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_MOSAIC (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_FADE (HDC hdc, HDC hDCmem, const BITMAP & bmif, const SHOWEFFECT & Param) ;
	void	ShowDib_FADESCAN (HDC hdc, HDC hDCmem, BITMAP bmif, DWORD dwParam) ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCBitmap::FCBitmap () : m_nMax(16), m_nMin (-16)  {
	m_hBitmap = NULL ;	m_iFreqCount = 300 ; m_iScale = 1 ; m_bContinue = TRUE ;
}
inline FCBitmap::FCBitmap (const FCBitmap & bmp) : m_nMax(16), m_nMin (-16) {
	m_hBitmap = NULL ;	m_iFreqCount = 300 ; m_iScale = 1 ; m_bContinue = TRUE ;
	if (bmp.GetHandle () != NULL)
		FCBitmap::operator = (bmp) ;
}
inline FCBitmap::~FCBitmap () {
	this->Clear () ;
}
inline BOOL  FCBitmap::IsContinueShow () const {
	return m_bContinue ;
}
inline void  FCBitmap::SetContinueFlag (BOOL bContinue) {
	::InterlockedExchange ((LONG *)&m_bContinue, bContinue) ;
}
inline void  FCBitmap::SetCpuSpeed (int iFreq) {
	m_iFreqCount = iFreq ;
}
inline int  FCBitmap::GetCpuSpeed () const {
	return m_iFreqCount ;
}
inline void FCBitmap::Clear () {
	if (m_hBitmap != NULL) {
		::DeleteObject (m_hBitmap) ;
		m_hBitmap = NULL ;
	}
}
inline HBITMAP  FCBitmap::GetHandle () const {
	return m_hBitmap ;
}
inline void  FCBitmap::Attach (HBITMAP hBitmap) {
	m_hBitmap = hBitmap ;
}
inline HBITMAP  FCBitmap::Detach () {
	HBITMAP		hRet = m_hBitmap ;
	m_hBitmap = NULL ;
	return hRet ;
}
inline void  FCBitmap::GetBitmapInfo (BITMAP * pBitmap) const {
	if (pBitmap != NULL)
		::GetObject (m_hBitmap, sizeof(BITMAP), pBitmap) ;
}
inline int  FCBitmap::GetWidth () const {
	BITMAP	bmif ;
	return (::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) == 0) ? 0 : bmif.bmWidth ;
}
inline int  FCBitmap::GetHeight () const {
	BITMAP	bmif ;
	return (::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) == 0) ? 0 : bmif.bmHeight ;
}
inline int  FCBitmap::GetColorBit () const {
	BITMAP	bmif ;
	return (::GetObject (m_hBitmap, sizeof(BITMAP), &bmif) == 0) ? 0 : bmif.bmBitsPixel ;
}
inline BOOL  FCBitmap::CreateCompatibleBitmap (int iWidth, int iHeight) {
	this->Clear () ;
	HDC		dcScreen = ::CreateIC (TEXT("display"), NULL, NULL, NULL) ;
	m_hBitmap = ::CreateCompatibleBitmap (dcScreen, iWidth, iHeight) ;
	::ReleaseDC (NULL, dcScreen) ;
	return (m_hBitmap != NULL) ;
}
inline BOOL  FCBitmap::LoadBitmap (UINT uIDBitmap) {
	return this->LoadBitmap (MAKEINTRESOURCE(uIDBitmap)) ;
}
inline BOOL  FCBitmap::LoadBitmap (PCTSTR szResBitmap) {
	this->Clear () ;
	return (BOOL) (m_hBitmap = (HBITMAP)::LoadBitmap (::GetModuleHandle (NULL), szResBitmap)) ;
}
inline BOOL  FCBitmap::LoadBitmapFromFile (PCTSTR szFileName) {
	this->Clear () ;
	return (BOOL) (m_hBitmap = (HBITMAP)::LoadImage (NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) ;
}
inline BOOL  FCBitmap::LoadOEMBitmap (UINT uIDBitmap) {
	this->Clear () ;
	return (BOOL) (m_hBitmap = (HBITMAP)::LoadImage (NULL, MAKEINTRESOURCE (uIDBitmap), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) ;
}
inline void  FCBitmap::SetZoomScale (int iScale) {
	if (iScale > 0)
		m_iScale = max (1, min (m_nMax, iScale)) ;
	else
		m_iScale = max (m_nMin, min (-1, iScale)) ;
}
inline int  FCBitmap::GetZoomScale () const {
	return m_iScale ;
}
inline void  FCBitmap::IncreaseZoomScale () {
	if (m_iScale < m_nMax) {
		if (m_iScale == -1)
			m_iScale = 1 ;
		m_iScale++ ;
	}
}
inline void  FCBitmap::DecreaseZoomScale () {
	if (m_iScale > m_nMin) {
		if (m_iScale == 1)
			m_iScale = -1 ;
		m_iScale-- ;
	}
}
inline int  FCBitmap::GetScaledWidth () const {
	return (m_iScale > 0) ? (this->GetWidth() * m_iScale) : (this->GetWidth() / (-m_iScale)) ;
}
inline int  FCBitmap::GetScaledHeight () const {
	return (m_iScale > 0) ? (this->GetHeight() * m_iScale) : (this->GetHeight() / (-m_iScale)) ;
}

#endif
