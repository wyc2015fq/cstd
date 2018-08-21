///////////////////////////////////////////////////////////////////////////
//  Copyright(c) 1999-2015, zyzx, All Rights Reserved
//	Created: 2015/03/14
//
/// @file	 WinGraph.h
/// @brief	 GDI绘图基础模块
/// @author  李绍良[lsl](zyzx),email:zyzx_lsl@163.com
/// @version 0.1
/// @history 修改历史
///  \n lsl	2015/03/13	0.1	创建文件
///////////////////////////////////////////////////////////////////////////
#ifndef __WINGRAPH_H__
#define __WINGRAPH_H__

#include "windows.h"
#include "lui.h"

class CWinGraph
{
public:
	CWinGraph();
	~CWinGraph();

	int Initial(HWND hwnd, int cx, int cy);
	void Quit();

	static int_t ScreenSize(int_t *pCx, int_t *pCy);
	static int_t CreateCanvas(LUI_CANVAS *ptr, int_t cx, int_t cy, int_t bpp);
	static int_t DestroyCanvas(LUI_CANVAS *ptr);

	static void DrawPixel(LUI_CANVAS *ptr, int_t x, int_t y, uint32 color);
	static void DrawLine(LUI_CANVAS *ptr, int_t x1, int_t y1, int_t x2, int_t y2, uint32 color, int_t width);
	static void DrawRect(LUI_CANVAS *ptr, int_t x, int_t y, int_t cx, int_t cy, uint32 color, bool fill, int_t border);
	static void DrawImage(LUI_CANVAS *ptr, const unsigned char *pImage, int_t x, int_t y, int_t width, int_t xi, int_t yi, int_t cx, int_t cy, bool bAlpha);
	static void DrawImageID(LUI_CANVAS *ptr, int_t bmpid, int_t x, int_t y, int_t xi, int_t yi, int_t cx, int_t cy, bool bAlpha);
	static int_t DrawText(LUI_CANVAS *ptr, int_t zoom, int_t size, const char *pStr, int_t len, int_t x, int_t y, int_t cx, int_t cy, uint32 color, bool border, uint32 color_border);
	static bool TextSize(int_t size, int_t zoom, const char *pStr, int_t len, int_t &cx, int_t &cy);


	static void Refresh(int_t x, int_t y, int_t cx, int_t cy);
	static void Clear(LUI_CANVAS *ptr, int_t x, int_t y, int_t cx, int_t cy);

protected:
	LUI_CANVAS	m_canvas32;
	LUI_CANVAS	m_canvas16;

	HWND		m_hwnd;
	HDC			m_hdc;

	HDC			m_bmpDC;
	HBITMAP		m_bmpOld;
	HBITMAP		m_bmp;
};

#endif // __WINGRAPH_H__
//end

