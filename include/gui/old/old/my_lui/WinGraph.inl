///////////////////////////////////////////////////////////////////////////
//  Copyright(c) 1999-2015, zyzx, All Rights Reserved
//	Created: 2015/03/14
//
/// @file	 WinGraph.cpp
/// @brief	 GDI绘图基础模块
/// @author  lsl(zyzx),email:zyzx_lsl@163.com
/// @version 0.1
/// @history 修改历史
///  \n lsl	2015/03/13	0.1	创建文件
///////////////////////////////////////////////////////////////////////////
#include "WinGraph.h"


extern CWinGraph *g_pGraph;


CWinGraph::CWinGraph()
{

}

CWinGraph::~CWinGraph()
{

}

int CWinGraph::Initial(HWND hwnd, int cx, int cy)
{
	CreateCanvas(&m_canvas16, cx, cy, 2);
	CreateCanvas(&m_canvas32, cx, cy, 4);

	m_hwnd = hwnd;
	m_hdc = ::GetDC(m_hwnd);
	
	m_bmpDC = ::CreateCompatibleDC(m_hdc);
	m_bmp = ::CreateCompatibleBitmap(m_hdc, cx, cy);
	m_bmpOld = (HBITMAP)::SelectObject(m_bmpDC, m_bmp);

	return 0;
}

void CWinGraph::Quit()
{
	::SelectObject(m_bmpDC, m_bmpOld);

	::DeleteObject(m_bmp);
	::DeleteDC(m_bmpDC);
	::ReleaseDC(m_hwnd, m_hdc);

	DestroyCanvas(&m_canvas32);
	DestroyCanvas(&m_canvas16);

	m_hwnd = NULL;
}

int_t CWinGraph::ScreenSize(int_t *pCx, int_t *pCy)
{
	*pCx = g_pGraph->m_canvas16.cx;
	*pCy = g_pGraph->m_canvas16.cy;

	return 0;
}

int_t CWinGraph::CreateCanvas(LUI_CANVAS *ptr, int_t cx, int_t cy, int_t bpp)
{
	ptr->cx = cx;
	ptr->cy = cy;
	ptr->bpp = bpp;

	ptr->pitch = ptr->cx * ptr->bpp;
	ptr->len = ptr->pitch * ptr->cy;
	ptr->pData = new unsigned char[ptr->len];

	memset(ptr->pData, 0, ptr->len);

	return 0;
}

int_t CWinGraph::DestroyCanvas(LUI_CANVAS *ptr)
{
	delete [] ptr->pData;
	memset(ptr, 0, sizeof(LUI_CANVAS));

	return 0;
}

void CWinGraph::DrawPixel(LUI_CANVAS *ptr, int_t x, int_t y, uint32 color)
{
	unsigned char *pData = ptr->pData + y * ptr->pitch + x * ptr->bpp;
	memcpy(pData, &color, ptr->bpp);
}

void CWinGraph::DrawLine(LUI_CANVAS *ptr, int_t x1, int_t y1, int_t x2, int_t y2, uint32 color, int_t width)
{
  int i, w;
	if(x1 == x2)
	{
		//横线
		int_t y = 0, len = 0;

		if(y1 > y2)
		{
			len = y1 - y2 + 1;
			y = y2;
		}
		else
		{
			len = y2 - y1 + 1;
			y = y1;
		}

		DrawRect(ptr, x1, y, width, len, color, true, 1);
	}
	else if(y1 == y2)
	{
		//竖线
		int_t x = 0, len = 0;

		if(x1 > x2)
		{
			len = x1 - x2 + 1;
			x = x2;
		}
		else
		{
			len = x2 - x1 + 1;
			x = x1;
		}

		DrawRect(ptr, x, y1, len, width, color, true, 1);
	}
	else
	{
		//斜线

		int_t bpp = ptr->bpp;
		int_t pitch = ptr->pitch;
		int_t sx = 0, sy = 0, ex = 0, ey = 0;

		if((abs(y2 - y1)) >= (abs(x2 - x1)))
		{
			//y轴方向变化大, x方向变化小
			//按y轴采样

			//起始点(sx, sy) 终止(ex, ey)
			//保证[sy < ey]
			if(y1 > y2)
			{
				sx = x2; sy = y2; ex = x1; ey = y1;
			}
			else
			{
				sx = x1; sy = y1; ex = x2; ey = y2;
			}

			//改进的Bresenham画线
			int_t dx = ex - sx, dy = ey - sy;
			int_t dr = (dx > 0) ? 1 : (-1);
			int_t e = -dy * dr;

			unsigned char* pData = ptr->pData + sy * pitch + sx * bpp;
			unsigned char* p = pData;

			for(w = 0; w < width; w++)
			{
				memcpy(p, &color, bpp);
				p += bpp;
			}

			p = pData;

			int_t dr_dy_1 = (dy * dr) << 1;
			int_t dr_bpp = bpp * dr;
			int_t width_bpp = width * bpp;

			for(i = 0; i < dy; i++)
			{
				e += (dx << 1);
				p += pitch;

				if(e * dr >= 0)
				{
					e -= dr_dy_1;				// -= ((dy * dr) << 1);
					p += dr_bpp;				// += bpp * dr
				}

				memcpy(p, pData, width_bpp);	// width * bpp
			}
		}
		else
		{
			//y轴方向变化小, x方向变化大
			//按x轴采样

			//起始点(sx, sy) 终止(ex, ey)
			//保证[sx < ex]
			if(x1 > x2)
			{
				sx = x2; sy = y2; ex = x1; ey = y1;
			}
			else
			{
				sx = x1; sy = y1; ex = x2; ey = y2;
			}

			//改进的Bresenham画线
			int_t dx = ex - sx, dy = ey - sy;
			int_t dr = (dy > 0) ? 1 : (-1);
			int_t e = - dr * dx;

			unsigned char* pData = ptr->pData + sy * pitch + sx * bpp;
			unsigned char* p = pData;


			for(w = 0; w < width; w++)
			{
				memcpy(p, &color, bpp);
				p += bpp;
			}

			p = pData;

			int_t dx_dr_1 = (dx * dr) << 1;
			int_t dr_lw = pitch * dr;
			int_t width_bpp = width * bpp;

			for(i = 0; i < dx; i++)
			{
				e += (dy << 1);
				p += bpp;

				if(e * dr >= 0)
				{
					e -= dx_dr_1;
					p += dr_lw;
				}

				memcpy(p, pData, width_bpp);
			}
		}
	}
}

void CWinGraph::DrawRect(LUI_CANVAS *ptr, int_t x, int_t y, int_t cx, int_t cy, uint32 color, bool fill, int_t border)
{
	if((0 >= cx) || (0 >= cy))
	{
		return;
	}

	int_t bpp = ptr->bpp;
	int_t pitch = ptr->pitch;
  int_t h, w;

	unsigned char* pData = ptr->pData + y * pitch + x * bpp;
	unsigned char* p = pData;

	if(fill)
	{
		for(w = 0; w < cx; w++)
		{
			memcpy(p, &color, bpp);
			p += bpp;
		}

		int_t width = cx * bpp;
		p = pData + pitch;

		for(h = 1; h < cy; h++)
		{
			memcpy(p, pData, width);
			p += pitch;
		}
	}
	else
	{
		for(w = 0; w < cx; w++)
		{
			memcpy(p, &color, bpp);
			p += bpp;
		}

		int_t width = cx * bpp;

		//顶
		p = pData + pitch;

		for(h = 1; h < border; h++)
		{
			memcpy(p, pData, width);
			p += pitch;
		}

		//底
		p = pData + pitch * (cy - border);

		for(h = cy - border; h < cy; h++)
		{
			memcpy(p, pData, width);
			p += pitch;
		}

		width = border * bpp;

		//左边
		p = pData + pitch;

		for(h = 1; h < cy; h++)
		{
			memcpy(p, pData, width);
			p += pitch;
		}

		//右边
		p = pData + pitch + bpp * cx - width;

		for(h = 1; h < cy; h++)
		{
			memcpy(p, pData, width);
			p += pitch;
		}
	}
}

void CWinGraph::DrawImage(LUI_CANVAS *ptr, const unsigned char *pImage, int_t x, int_t y, int_t width, int_t xi, int_t yi, int_t cx, int_t cy, bool bAlpha)
{

}

void CWinGraph::DrawImageID(LUI_CANVAS *ptr, int_t bmpid, int_t x, int_t y, int_t xi, int_t yi, int_t cx, int_t cy, bool bAlpha)
{

}

int_t CWinGraph::DrawText(LUI_CANVAS *ptr, int_t zoom, int_t size, const char *pStr, int_t len, int_t x, int_t y, int_t cx, int_t cy, uint32 color, bool border, uint32 color_border)
{
	return 0;
}

bool CWinGraph::TextSize(int_t size, int_t zoom, const char *pStr, int_t len, int_t &cx, int_t &cy)
{
	return false;
}

void CWinGraph::Refresh(int_t x, int_t y, int_t cx, int_t cy)
{
	//test
	DrawRect(&g_pGraph->m_canvas16, 10, 10, 100, 100, LUI_RGB1555(180, 40, 40), true, 1);
	DrawLine(&g_pGraph->m_canvas16, 110, 110, 300, 400, LUI_RGB1555(40, 40, 180), 1);
	DrawLine(&g_pGraph->m_canvas16, 300, 400, 110, 110, LUI_RGB1555(40, 40, 180), 1);

	DrawRect(&g_pGraph->m_canvas16, 400, 100, 100, 100, LUI_RGB1555(40, 180, 40), false, 4);


	//下面的做法比较废材
	//仅仅用于测试,要效率时,还需要优化把绘制的UI图片刷新到windows窗口中
	//在公司采用的是directx9.0的d3d绘图接口,这里为方便理解直接使用gdi

	//rgb155 => rgb8888
	unsigned char *ptrDest = g_pGraph->m_canvas32.pData + y * g_pGraph->m_canvas32.pitch;
	unsigned char *ptrSrc = g_pGraph->m_canvas16.pData + y * g_pGraph->m_canvas16.pitch;
	for(int j = 0; j < cy; j++)
	{
		unsigned char *ptrDestTmp = ptrDest;
		unsigned char *ptrSrcTmp = ptrSrc;

		for(int i = 0; i < cx; i++)
		{
			uint16 *pColor = (uint16*)ptrSrcTmp;
			uint32 *pValue = (uint32*)ptrDestTmp;
			*pValue = LUI_RGBA(0xFF, LUI_R1555(*pColor), LUI_G1555(*pColor), LUI_B1555(*pColor));

			ptrDestTmp += 4;
			ptrSrcTmp += 2;
		}

		ptrDest += g_pGraph->m_canvas32.pitch;
		ptrSrc += g_pGraph->m_canvas16.pitch;
	}


	//拷贝到屏幕
	HDC hdc = g_pGraph->m_hdc;
	HDC bmpDc = g_pGraph->m_bmpDC;
	HBITMAP bmp = g_pGraph->m_bmp;

	::SetBitmapBits(bmp, g_pGraph->m_canvas32.len, g_pGraph->m_canvas32.pData);
	::BitBlt(hdc, x, y, cx, cy, bmpDc, x, y, SRCCOPY);
}

void CWinGraph::Clear(LUI_CANVAS *ptr, int_t x, int_t y, int_t cx, int_t cy)
{
	int_t bpp = ptr->bpp;
	int_t pitch = ptr->pitch;
	long width = cx * bpp;

	unsigned char* p = ptr->pData + y * pitch + x * bpp;

	for(long h = 0; h < cy; h++)
	{
		memset(p, 0, width);
		p += pitch;
	}
}

//end
