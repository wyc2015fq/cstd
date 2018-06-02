// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H_INCLUDED_)
#define AFX_DIB_H_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vfw.h>
#pragma comment(lib,"vfw32.lib")

class CDib : public CObject  
{
public:
	CDib();
	virtual ~CDib();
	CDib(const CDib& dib);
//Attributes
public:
	BYTE    *GetBits();
	CSize	 GetSize();
	LONG	 GetWidth();
	LONG	 GetHeight();
	LONG     GetWidthStep();
	int      GetBiBitCount();
	CSize GetDibSaveDim();
	BOOL     IsValid()  { return(m_hDrawDib!=NULL); }
	operator HDRAWDIB() { return m_hDrawDib; }

//operations
public:
	BOOL Open(const char *pzFileName);
	BOOL Save(const char *pzFileName);
	void Draw(CDC *pDC,int nWidth, int nHeight);
	void Close();
	void Resize(LONG width, LONG height);
//public parameters
	BYTE		*m_pDibBits;
//implementation
public:
	BYTE		*m_pDib;
	HDRAWDIB	m_hDrawDib;

	BITMAPFILEHEADER bmpFileHeader;

	int Pix_X;
	int Pix_Y;
};
#endif // !defined(AFX_DIB_H_INCLUDED_)
