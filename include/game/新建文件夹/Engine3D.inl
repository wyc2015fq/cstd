
#pragma comment (lib, "vfw32.lib")


#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <vfw.h>
#include <xmmintrin.h>

#define _BITS_PER_PIXEL	32										// Color depth
#define _BYTE_PER_PIXEL	(_BITS_PER_PIXEL>>3)

#define _PI	3.1415926f											// Value of PI
#define _PIXEL	DWORD											// Pixel
#define _BITS_PER_PIXEL	32										// Color depth
#undef _RGB
#define _RGB(r,g,b)	(((r) << 16) | ((g) << 8) | (b))			// Convert to RGB
#define _GetRValue(c)	((BYTE)(((c) & 0x00FF0000) >> 16))		// Red color component
#define _GetGValue(c)	((BYTE)(((c) & 0x0000FF00) >> 8))		// Green color component
#define _GetBValue(c)	((BYTE)((c) & 0x000000FF))				// Blue color component
#define CAMERA_OFFSET	0.025f									// Camera offset
#define MAX_OBJECTS		4096									// Maximum number of objects
#define MAX_LIGHTS		1										// Maximum number of lights
#define MD2_MAGIC_NUM	('I' + ('D' << 8) + ('P' << 16) + ('2' << 24))	// MD2 file magic number
#define MD2_VERSION		8
#define SWAP(a,b,t)		{t=a; a=b; b=t;}


typedef long fixed;												    // Our new fixed point type
#define itofx(x) ((x) << 8)										// Integer to fixed point
#define ftofx(x) (long)((x) * 256)						// Float to fixed point
#define dtofx(x) (long)((x) * 256)								// Double to fixed point
#define fxtoi(x) ((x) >> 8)										// Fixed point to integer
#define fxtof(x) ((float) (x) / 256)							// Fixed point to float
#define fxtod(x) ((double)(x) / 256)							// Fixed point to double
#define Mulfx(x,y) (((x) * (y)) >> 8)							// Multiply a fixed by a fixed
#define Divfx(x,y) (((x) << 8) / (y))							// Divide a fixed by a fixed


struct DrawImageInfo {
  int x, y, cx, cy, step;
  _PIXEL* pix;
};

struct Engine3D {
  HDRAWDIB m_hDD;
  DrawImageInfo iinfo[100];
  int iinfoLen;
  _PIXEL* m_lpData;
  _PIXEL* m_lpData2;
  int m_iWidth;
  int m_iHeight;
  
  Engine3D() {
    m_iWidth=0;
    m_iHeight=0;
    iinfoLen=0;
    m_lpData = 0;
  }
  int Init() {
    m_hDD = DrawDibOpen();
    return 0;
  }
  int SetSize(int iWidth, int iHeight) {
    DWORD m_dwSize;
    m_iWidth=iWidth;
    m_iHeight=iHeight;
    _PIXEL* lpData=m_lpData;
    m_dwSize = m_iWidth * _BYTE_PER_PIXEL * m_iHeight;
    m_lpData = (_PIXEL*)malloc(m_dwSize*sizeof(BYTE));
    m_lpData2 = (_PIXEL*)malloc(m_dwSize*sizeof(BYTE));
    memset(m_lpData, 0, m_iHeight*m_iWidth*_BYTE_PER_PIXEL);
    if (lpData) free(lpData);
    return 0;
  }
  int Shutdown() {
    DrawDibClose(m_hDD);
    free(m_lpData);
    free(m_lpData2);
    return 0;
  }
  void Draw_Text(long dstX, long dstY, LPSTR lpszText, _PIXEL textColor, long textAlpha,
    LPTSTR lpszFontName, long fontSize, BOOL bBold, BOOL bItalic)
  {
    xDrawText(m_iHeight, m_iWidth, (unsigned char *)m_lpData, m_iWidth*_BYTE_PER_PIXEL, 4,
      dstY, dstX, lpszFontName, fontSize, 0, bItalic, bBold, lpszText,
      CV_RGB(_GetRValue(textColor), _GetGValue(textColor), _GetBValue(textColor)));
  }
  int Draw_Image(int x, int y, int cx, int cy, const _PIXEL* pix, int step, UInt32 col) {
    //MEMCPY2D(cy, cx, pix, step, m_lpData, m_iWidth);
    if (m_lpData)
      TransparentCopy2d_32u(cy, cx, pix, step, m_lpData+y*m_iWidth+x, m_iWidth*_BYTE_PER_PIXEL, col, 0);
    return 0;
  }
  int Rander_Images() {
    if (iinfoLen>0) {
      int i;
      for (i=0; i<iinfoLen; ++i) {
        MEMCPY2D(iinfo[i].h, iinfo[i].w, iinfo[i].pix, iinfo[i].step, m_lpData, m_iWidth);
      }
    }
    return 0;
  }
  int Draw_Frame(HDC hDC)
  {
    BITMAPINFOHEADER m_bih;
    //LPBYTE m_lpZBuffer;
    DWORD m_dwSize;
    int bytes_per_line=m_iWidth*_BYTE_PER_PIXEL;
    
    m_dwSize = m_iWidth * _BYTE_PER_PIXEL * m_iHeight;
    //m_lpZBuffer = (LPBYTE)malloc(m_dwSize*sizeof(BYTE));
    //memset(m_lpData, 120, m_dwSize*sizeof(BYTE));
    
    memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
    m_bih.biSize = sizeof(BITMAPINFOHEADER);
    m_bih.biWidth = m_iWidth;
    m_bih.biHeight = m_iHeight;
    m_bih.biPlanes = 1;
    m_bih.biBitCount = _BITS_PER_PIXEL;
    m_bih.biCompression = BI_RGB;
    m_bih.biSizeImage = m_dwSize;
    memcpy2d((unsigned char*)m_lpData2+(m_iHeight-1)*bytes_per_line,
      -bytes_per_line, m_lpData, bytes_per_line, m_iHeight, bytes_per_line);
    DrawDibDraw(m_hDD, hDC, 0, 0, m_iWidth, m_iHeight, &m_bih, m_lpData2, 0, 0, m_iWidth, -m_iHeight, 0);
    //free(m_lpZBuffer);
    return 0;
  }
  
};