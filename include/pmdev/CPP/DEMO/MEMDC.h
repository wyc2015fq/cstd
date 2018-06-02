#ifndef _MEMORYDC_H_
#define _MEMORYDC_H_
#include <Windows.h>
class CHMemDC
{
public:
	CHMemDC(HDC hDC, const RECT& rect, int biWidth, int biHeight, HBITMAP bitMap = NULL){
		m_hSourceDC = hDC;
		m_hDstWidth = rect.right-rect.left;
		m_hDstHeight = rect.bottom-rect.top;
		m_hSrcWidth = biWidth;
		m_hSrcHeight = biHeight;
		m_hMemDC = ::CreateCompatibleDC(hDC);
		m_hBMP = bitMap;
		m_rt = rect;
		if ( NULL == bitMap){
			m_hBMP = ::CreateCompatibleBitmap(hDC, m_hSrcWidth, m_hSrcHeight);
		}
		m_old = ::SelectObject(m_hMemDC, m_hBMP);
	}
	~CHMemDC(){
		::SetStretchBltMode(m_hSourceDC, HALFTONE); 
		::SetBrushOrgEx(m_hSourceDC, 0, 0, NULL); 
		BOOL rt = ::StretchBlt(m_hSourceDC, m_rt.left, m_rt.top, m_hDstWidth, m_hDstHeight, m_hMemDC, 0, 0, m_hSrcWidth, m_hSrcHeight, SRCCOPY);
		::DeleteObject(m_hBMP);
		::SelectObject(m_hMemDC, m_old);
		::DeleteDC(m_hMemDC);
	}
	CHMemDC* operator->(){
		return this;
	}
	HDC GetDC(){
		return m_hMemDC;
	}
private:
	HDC m_hSourceDC;
	HDC m_hMemDC;
	HBITMAP m_hBMP;
	int   m_hDstWidth;
	int   m_hDstHeight;
	int   m_hSrcWidth;
	int   m_hSrcHeight;
	RECT m_rt;

	HGDIOBJ m_old;
};
#endif