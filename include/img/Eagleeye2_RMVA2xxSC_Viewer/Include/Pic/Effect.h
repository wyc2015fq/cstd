//////////////////////////////////////////////////////////////////
//																//
//		用途 : 32 位 DIB 特效处理								//
//		创建 : [Foolish] / 2001-4-14							//
//		更新 : 2002-11-6										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_DIBEFFECT_H__
#define		__FOO_DIBEFFECT_H__
#include "Pic.h"
#include <list>
#pragma once
using namespace std ;

//====================================================================
//	DIB效果
//====================================================================
/************************************************************************/
/*	1 . 所有函数如非特别指明则只对 32 位色有效, 必须先ConvertTo32Bit.	*/
/*	2 . 对于rect, 右和下边界与图形学里一样为开区间 (即不包含)			*/
/*		如选全图为: (0, 0, 800, 600) 而不是 (0, 0, 799, 599)			*/
/*	3 . 打开一幅新图像前要调用->ClearAllList () ;						*/
/*	4 . 如果注明 : (支持 AUTO-Undo/Redo)								*/
/*		则当m_UndoFlag == true时, 函数自动保存当前DIB					*/
/************************************************************************/
class FCDibEffect : public FCDib
{
public :
	FCDibEffect (bool bUndo = false) ;
	FCDibEffect (FCDibEffect & dib) ;
	virtual ~FCDibEffect () ;
	void	operator= (const FCDibEffect & pic) ;

public :
	// Region selection support
	enum SelectType {SEL_CREATE, SEL_ADD, SEL_SUB} ;
	void	SetRectRgn (int nLeft, int nTop, int nRight, int nBottom, SelectType nType = FCDibEffect::SEL_CREATE) ;
	void	SetRoundRectRgn (int nLeft, int nTop, int nRight, int nBottom, int nWidthEllipse, int nHeightEllipse, SelectType nType = FCDibEffect::SEL_CREATE) ;
	void	SetEllipticRgn (int nLeft, int nTop, int nRight, int nBottom, SelectType nType = FCDibEffect::SEL_CREATE) ;
	void	SetPolygonRgn (const POINT * ppt, int cNumPoint, int nFillMode = ALTERNATE, SelectType nType = FCDibEffect::SEL_CREATE) ;
//	void	MagicStickRgn (int x, int y, RGBQUAD rgb, SelectType nType = FCDibEffect::SEL_CREATE) ; // 魔术棒选取
	void	OffsetRgn (int xOffset, int yOffset) ;
	void	ClearSelection () ; // ClearSelection等于选全图
	bool	HasSelection () ;
	bool	IsInSelection (int x, int y) ;
	void	GetBoundBox (RECT * rect) ;
	HRGN	GetSelectHRgn () ;
	void	FillRgn (HBRUSH hBrush) ; // 填充
	void	FrameRgn (HBRUSH hBrush) ; // 画框
	void	MoveRgn (int xOffset, int yOffset, bool bHold = false) ;
	void	CopySelectionDib (FCDib * RgnDib) ;
	BOOL	MakeThumbnail (FCDib * RgnDib, int nThumbW, int nThumbH) ; // 指定RgnDib的大小

	// Multi level Undo/Redo support
	void	Undo () ;	// 撤销一步操作
	void	Redo () ;	// 重作一步操作
	void	AddToUndoList () ; // 当前选择区域
	void	AddImageToUndoList () ;
	void	AddToUndoList (const RECT & rect) ; // 指定区域
	bool	IsUndoEnable () ;
	bool	IsRedoEnable () ;
	void	SetUndoEnable (bool bEnable = false) ; // Undo/Redo flag
	BOOL	GetUndoFlag () ;
//	void	SetUndoSize(int nUndoSize);
//	int		GetUndoSize() const { return m_nUndoSize; };
	void	ClearRedoList () ;
	void	ClearUndoList () ;
	void	ClearAllList () ; // 清空Undo/Redo队列

	//	平移变换, 用边界色填充空白区域 (AUTO-Undo/Redo)
	void	operator << (int iWidth) ;
	void	operator >> (int iWidth) ;
	void	ShiftUp (int iHeight) ;
	void	ShiftDown (int iHeight) ;
	void	ColShiftUp (int iCol, int iHeight) ;	// 列上移
	void	ColShiftDown (int iCol, int iHeight) ;	// 列下移

	// 方位 (支持 Undo/Redo，不支持区域)
		// 加/减边框 (用黑色填充边框)
	void	AddFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	EraseFrame (int iLeft, int iTop, int iRight, int iBottom) ;
	void	DuplicateFrame (int iLeft, int iTop, int iRight, int iBottom) ;
		// 顺时针旋转 90'
	void	Rotate90 () ;
		// 顺时针旋转 270'
	void	Rotate270 () ;
		// 顺时针旋转给定角度 iAngle在[1..89]
	void	Rotate (int iAngle) ;
		// iType == TRUE 为宽高成比例缩放
	void	Stretch (DWORD dwWidth, DWORD dwHeight, int iType = TRUE) ;

	//	特效处理 (支持 Undo/Redo)
		// 上下翻转
	void	Flip () ;
		// 左右镜像
	void	Mirror () ;
		//	调节RGB值
	void	AdjustRGB (int iValueR, int iValueG, int iValueB) ;
		//	亮度	bFlag == TRUE, 则iValue为按增量
		//			bFlag == FALSE, 则iValue为按百分比, 如150 (means 150%)
	void	AdjustBrightness (int iValue, BOOL bFlag = TRUE) ;
	void	AdjustBrightnessPixel (int x, int y, int iValue, BOOL bFlag = TRUE) ;
		//	对比度 (iPercent为按百分比的增量)
	void	AdjustContrast (int iPercent) ;
	void	AdjustContrastPixel (int x, int y, int iPercent) ;
		//	色调
	void	AdjustHue (int iPercent) ;
		//	饱和度
	void	AdjustSaturation (int iPercent) ;
		//	色调 && 饱和度
	void	AdjustHS (int iPercentHue, int iPercentSat) ;
		//	颜色反转
	void	Negate () ;
		//	浮雕效果
	void	Emboss (int nStep = 2, int nDirection = DIRECT_RIGHT) ;
		//  风化效果
		//	nStep 为移动象素 : [1..x]
		//	nDirection 为风化方向
	void	Windy (int nStep, int nDirection = DIRECT_RIGHT, bool bSmooth = true) ;
	void	WindySoftLine (int nLine, int nStep, int nDirection) ;
		//	模糊效果, 使用平均算法
		//	iBlockLen为小方块边长, 在[2..32]
	void	SmoothAverage (int iBlockLen) ;
	void	SmoothAverageNoEdge (int iBlockLen) ;
		//	锐化效果
	void	Sharp (int nStep = 1) ;
		//	马赛克效果
		//	iBlockLen为小方块边长 ( >=2 )
	void	Mosaic (int iBlockLen) ;
		//	玻璃效果 (飞溅)
		//	iBlockLen为小方块边长, 在[2..100]
	void	Glass (int iBlockLen) ;
		//	球化效果
	void	Sphere () ;
		//	柱面效果
	void	Column () ;
		//	锥(凹)缩，nStep : [1..100]
	void	ConeSunken (int nStep = 50) ;
		//	带状效果 (iSwing : 振幅[0..100]		iFrequency : 频率)
	void	Stripe (int iSwing, int iFrequency) ;
		//	卷积运算, iElement从矩阵左上角开始, iLength为矩阵宽度
	void	ConvoluteDIB (int * iElement, int iLength, int iDivisor) ;
	void	ConvoluteDIBNoEdge (int * iElement, int iLength, int iDivisor) ;
		//	基于卷积的特效算法
	void	Algorithm (int iAlgorithm) ; // iAlgorithm见StdDefine.h

public :
	void	__CopyCol (FCDib & dibDest, int iColD, const FCDib & dibSrc, int iCol) ; // 列copy
	void	__BltBlock (const FCDib & Dib, int x, int y) ; // Cover block on current DIB, only the region specified by m_hSelRgn
	void	__SetPixelData (BYTE * pDest, int x, BYTE * pSrc) ;
protected :
	// Undo/Redo used
	enum UNDOTYPE {UNDO_COVER, UNDO_REPLACE} ;
	typedef struct tagUNDO_NODE
	{
		UNDOTYPE	UndoMode ;
		FCDib		* pPrior ;
		POINT		point ;
	} UNDO_NODE, * PUNDO_NODE ;
	void	__AddToUndoList (const RECT & rect, bool bClearRedo) ;
	void	__AddImageToUndoList (bool bClearRedo) ;
	void	__AddToUndoList (const UNDO_NODE & UndoNode) ;
	void	__AddToRedoList (const RECT & rect) ;
	void	__AddImageToRedoList () ;
	void	__AddToRedoList (const UNDO_NODE & UndoNode) ;
	void	__BoundSelectedRgn () ;
private :
	void	__ConeSunken (int nStep, UINT32 * nPercent) ;
	void	__Stripe (int iSwing, int iFrequency, UINT32 * nPercent) ;
	void	__Flip (UINT32 * nPercent) ;
	void	__Mirror (UINT32 * nPercent) ;
	void	__Negate (UINT32 * nPercent) ;
	void	__AdjustRGB (int iValueR, int iValueG, int iValueB, UINT32 * nPercent) ;
	void	__AdjustBrightness (int iValue, BOOL bFlag, UINT32 * nPercent) ;
	void	__AdjustContrast (int iPercent, UINT32 * nPercent) ;
	void	__AdjustHS (int iPercentHue, int iPercentSat, UINT32 * nPercent) ;
	void	__Emboss (int nStep, int nDirection, UINT32 * nPercent) ;
	void	__Windy (int nStep, int nDirection, bool bSmooth, UINT32 * nPercent) ;
	void	__Mosaic (int iBlockLen, UINT32 * nPercent) ;
	void	__Sharp (int nStep, UINT32 * nPercent) ;
	void	__SmoothAverageNoEdge (int iBlockLen, UINT32 * nPercent) ;
protected :
	BOOL				m_UndoFlag ;	// 是否支持Undo/Redo
	RECT				m_SelRect ;		// 区域的外围矩形
	HRGN				m_hSelRgn ;		// 选择区域
	list<UNDO_NODE *>	m_UndoList, m_RedoList ; // Undo/Redo队列
} ;

//===================================================================
//	Implement
//===================================================================
inline void  FCDibEffect::SetUndoEnable (bool bEnable) {
	m_UndoFlag = bEnable ;
}
inline bool  FCDibEffect::IsRedoEnable () {
	return !m_RedoList.empty () ;
}
inline bool  FCDibEffect::IsUndoEnable () {
	return !m_UndoList.empty () ;
}
inline BOOL  FCDibEffect::GetUndoFlag () {
	return m_UndoFlag ;
}
inline void  FCDibEffect::AdjustBrightnessPixel (int x, int y, int iPercent, BOOL bFlag) {
	RGBQUAD		* pPixel = (RGBQUAD *) this->GetBits (x, y) ;
	if (!bFlag)
		iPercent = 128 * iPercent / 100 ; // 转换成x/128
	pPixel->rgbBlue = max (min (bFlag ? (pPixel->rgbBlue + iPercent)
									  : (pPixel->rgbBlue * iPercent / 128), 255), 0) ;
	pPixel->rgbGreen = max (min (bFlag ? (pPixel->rgbGreen + iPercent)
									   : (pPixel->rgbGreen * iPercent / 128), 255), 0) ;
	pPixel->rgbRed = max (min (bFlag ? (pPixel->rgbRed + iPercent)
									 : (pPixel->rgbRed * iPercent / 128), 255), 0) ;
}
inline void  FCDibEffect::AdjustContrastPixel (int x, int y, int iPercent) {
	RGBQUAD		* pPixel = (RGBQUAD *)this->GetBits (x, y) ;
	iPercent = 128 * iPercent / 100 ; // 转换成x/128
	pPixel->rgbBlue = max (min (128 + (pPixel->rgbBlue - 128) * iPercent / 128, 255), 0) ;
	pPixel->rgbGreen = max (min (128 + (pPixel->rgbGreen - 128) * iPercent / 128, 255), 0) ;
	pPixel->rgbRed   = max (min (128 + (pPixel->rgbRed - 128) * iPercent / 128, 255), 0) ;
}
inline void  FCDibEffect::__CopyCol (FCDib & dibDt, int iColD, const FCDib & dibSr, int iCol) {
	BYTE		* pDest = dibDt.GetBits (iColD, 0),
				* pSrc = dibSr.GetBits (iCol, 0) ;
	int			nSpan = dibDt.ColorBits() / 8 ; // 每象素字节数2, 3, 4
	DWORD		dwOldPitch = dibSr.GetPitch (),
				dwPitch = dibDt.GetPitch () ;
	for (int y=0 ; y < dibDt.Height() ; y++, pDest -= dwPitch, pSrc -= dwOldPitch)
		CopyMemory (pDest, pSrc, nSpan) ;
}
__forceinline void  FCDibEffect::__SetPixelData (BYTE * pDest, int x, BYTE * pSrc) {
	switch (this->ColorBits())
	{
		case 8  :	*(pDest + x) = *pSrc ; break ;
		case 16 :	CopyMemory ((WORD *)pDest + x, pSrc, 2) ; break ;
		case 24 :	CopyMemory ((RGBTRIPLE *)pDest + x, pSrc, 3) ; break ;
		case 32 :	CopyMemory ((DWORD *)pDest + x, pSrc, 4) ; break ;
	}
}
inline void  FCDibEffect::ColShiftUp (int iCol, int iHeight) {
	if ((this->ColorBits() >= 8) && (iCol > 0) && (iCol < this->Width()) && (iHeight > 0) && (iHeight < this->Height()))
	{
		BYTE		* pSrc = this->GetBits (iCol, iHeight),
					* pDest = this->GetBits (iCol, 0) ;
		int			iMax = Height() - iHeight,
					nSpan = this->ColorBits() / 8 ;
		DWORD		dwPitch = this->GetPitch () ;
		for (int i=0 ; i < iMax ; i++, pSrc -= dwPitch, pDest -= dwPitch)
			CopyMemory (pDest, pSrc, nSpan) ;
		for (int m=0 ; m < iHeight ; m++, pDest -= dwPitch)
			FillMemory (pDest, nSpan, 0xFF) ; // 填充白色
	}
}
inline void  FCDibEffect::ColShiftDown (int iCol, int iHeight) {
	if ((this->ColorBits() >= 8) && (iCol > 0) && (iCol < this->Width()) && (iHeight > 0) && (iHeight < this->Height()))
	{
		BYTE		* pSrc = this->GetBits (iCol, Height() - 1 - iHeight),
					* pDest = this->GetBits (iCol, Height() - 1) ;
		int			iMax = Height() - iHeight,
					nSpan = this->ColorBits() / 8 ;
		DWORD		dwPitch = this->GetPitch () ;
		for (int i=0 ; i < iMax ; i++, pSrc += dwPitch, pDest += dwPitch)
			CopyMemory (pDest, pSrc, nSpan) ;
		for (int m=0 ; m < iHeight ; m++, pDest += dwPitch)
			FillMemory (pDest, nSpan, 0xFF) ; // 填充白色
	}
}
inline void  FCDibEffect::__BoundSelectedRgn () {
	HRGN	hBound = ::CreateRectRgn (0, 0, Width(), Height()) ;
	::CombineRgn (m_hSelRgn, m_hSelRgn, hBound, RGN_AND) ;
	::DeleteObject (hBound) ;
}
inline void  FCDibEffect::GetBoundBox (RECT * rect) {
	if (rect != NULL)
		if (this->HasSelection())
			::CopyRect (rect, &m_SelRect) ;
		else
			::SetRect (rect, 0, 0, this->Width(), this->Height()) ;
}
inline bool  FCDibEffect::HasSelection () {
	return ((m_hSelRgn != NULL) && !::IsRectEmpty (&m_SelRect)) ;
}
inline bool  FCDibEffect::IsInSelection (int x, int y) {
	return (::PtInRegion (m_hSelRgn, x, y) != 0) ;
}
inline void  FCDibEffect::ClearSelection () {
	if (HasSelection ())
		::DeleteObject (m_hSelRgn) ;
	m_hSelRgn = NULL ;
	::SetRectEmpty (&m_SelRect) ;
}
inline void  FCDibEffect::ConeSunken (int nStep) {
	this->__ConeSunken (nStep, &m_FinishedPer) ;
}
inline void  FCDibEffect::CopySelectionDib (FCDib * RgnDib) {
	if (!this->HasSelection ())
		* RgnDib = * (FCDib *) this ;
	else
		FCDibEffect::MakeThumbnail (RgnDib, m_SelRect.right-m_SelRect.left, m_SelRect.bottom-m_SelRect.top) ;
}
inline HRGN  FCDibEffect::GetSelectHRgn () {
	return m_hSelRgn ;
}
inline void  FCDibEffect::OffsetRgn (int xOffset, int yOffset) {
	if (this->HasSelection ()) {
		::OffsetRgn (m_hSelRgn, xOffset, yOffset) ;
		this->__BoundSelectedRgn () ;
		::GetRgnBox (m_hSelRgn, &m_SelRect) ;
	}
}
inline void  FCDibEffect::AddToUndoList () {
	if (this->HasSelection ()) {
		RECT	rc ;
		this->GetBoundBox (&rc) ;
		this->AddToUndoList (rc) ;
	}
	else
		this->AddImageToUndoList () ;
}
inline void  FCDibEffect::AddImageToUndoList () {
	this->__AddImageToUndoList (true) ;
}
inline void  FCDibEffect::AddToUndoList (const RECT & rect) {
	this->__AddToUndoList (rect, true) ;
}
inline void  FCDibEffect::ClearAllList () {
	this->ClearRedoList () ;
	this->ClearUndoList () ;
}
inline void  FCDibEffect::Stripe (int iSwing, int iFrequency) {
	this->__Stripe (iSwing, iFrequency, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Flip () {
	this->__Flip (&this->m_FinishedPer) ;
}
inline void  FCDibEffect::Mirror () {
	this->__Mirror (&this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustRGB (int iValueR, int iValueG, int iValueB) {
	this->__AdjustRGB (iValueR, iValueG, iValueB, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustBrightness (int iPercent, BOOL bFlag) {
	this->__AdjustBrightness (iPercent, bFlag, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustContrast (int iPercent) {
	this->__AdjustContrast (iPercent, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustHue (int iPercentHue) {
	this->__AdjustHS (iPercentHue, 100, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustSaturation (int iPercentSat) {
	this->__AdjustHS (100, iPercentSat, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::AdjustHS (int iPercentHue, int iPercentSat) {
	this->__AdjustHS (iPercentHue, iPercentSat, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Negate () {
	this->__Negate (&this->m_FinishedPer) ;
}
inline void  FCDibEffect::Emboss (int nStep, int nDirection) {
	this->__Emboss (nStep, nDirection, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Windy (int nStep, int nDirection, bool bSmooth) {
	this->__Windy (nStep, nDirection, bSmooth, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Sharp (int nStep) {
	this->__Sharp (nStep, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::Mosaic (int iBlockLen) {
	this->__Mosaic (iBlockLen, &this->m_FinishedPer) ;
}
inline void  FCDibEffect::SmoothAverageNoEdge (int iBlockLen) {
	this->__SmoothAverageNoEdge (iBlockLen, &this->m_FinishedPer) ;
}

#endif