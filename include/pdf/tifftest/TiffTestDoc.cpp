// TiffTestDoc.cpp : implementation of the CTiffTestDoc class
//

#include "stdafx.h"
#include "TiffTest.h"

#include "TiffTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc

IMPLEMENT_DYNCREATE(CTiffTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CTiffTestDoc, CDocument)
	//{{AFX_MSG_MAP(CTiffTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc construction/destruction

CTiffTestDoc::CTiffTestDoc()
{
	// TODO: add one-time construction code here
	m_hDIB = NULL;
	m_sizeDoc = 0;
	m_dwBmSize = 0;
}

CTiffTestDoc::~CTiffTestDoc()
{
	m_sizeDoc = 0;
	if(NULL != m_hDIB)
	{
		GlobalFree(m_hDIB);
		m_hDIB = NULL;
	}
}

BOOL CTiffTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc serialization

void CTiffTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc diagnostics

#ifdef _DEBUG
void CTiffTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTiffTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTiffTestDoc commands

BOOL CTiffTestDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	DWORD dwFileLength = 0;
	CString strTemp = _T("");
	WORD wDECount = 0;
	BYTE* pDIB = NULL;
	int i = 0;

	IFH ifh;
	ZeroMemory(&ifh, sizeof(IFH));

	CFile file;
	CFileException fe;

	if(0 == file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		AfxMessageBox("打开文件失败");
		return FALSE;
	}
	dwFileLength = file.GetLength();

	if(sizeof(IFH) != file.Read(&ifh, sizeof(IFH)))
	{
		AfxMessageBox("读TIF文件头失败");
		return FALSE;
	}

	if(0x2a != ifh.version)
	{
		AfxMessageBox("该文件不是TIF格式，读文件失败");
		return FALSE;
	}

	if(0x4949 != ifh.byteOrder)
	{
		AfxMessageBox("该TIF文件不是IBMPC字节序，读文件失败");
		return FALSE;
	}

	file.Seek(ifh.offsetToIFD, CFile::begin);
	
	if(2 != file.Read(&wDECount, 2))
	{
		AfxMessageBox("无法获得TIF文件目录入口数量");
		return FALSE;
	}
	strTemp.Format("该TIF文件有%d个目录入口", wDECount);
	AfxMessageBox(strTemp);

	DE* pde = new DE[wDECount];
	DE* pTemp = pde;
	memset(pde, 0, sizeof(DE)*wDECount);
	if(sizeof(DE)*wDECount != file.ReadHuge(pde, sizeof(DE)*wDECount))
	{
		AfxMessageBox("读图象文件目录失败");
		delete []pde;
		return FALSE;
	}

	//显示图象目录入口信息--测试用
	for(i=0; i<wDECount; i++)
	{
		pTemp = pde + i;
		strTemp.Format("DE[%d].tag = %d\r\nDE[%d].type = %d\r\nDE[%d].length = %d\r\nDE[%d].valueOffset = %d",
			i, pTemp->tag, i, pTemp->type, i, pTemp->length, i, pTemp->valueOffset);
		AfxMessageBox(strTemp);
	}

	for(i=0; i<wDECount; i++)
	{
		pTemp = pde + i;
		if(256 == pTemp->tag)	//图象宽度
		{
			m_sizeDoc.cx = pTemp->valueOffset;
		}
		if(257 == pTemp->tag)	//图象高度
		{
			m_sizeDoc.cy = pTemp->valueOffset;
		}
		if(273 == pTemp->tag)	//计算图象数据占用字节数
		{
			//m_dwBmSize = pTemp->valueOffset - sizeof(IFH);
			//或者把tag=256的valueOffset乘以tag=257的valueOffset
			m_dwBmSize = m_sizeDoc.cx * m_sizeDoc.cy;
		}
	}

	//构造BMP图象内存块
	if(!CreateBmpBuffer())
	{
		AfxMessageBox("构造BMP图象内存块失败");
		delete []pde;
		return FALSE;
	}

	//获得BMP内存块指针
	pDIB = (BYTE*)GlobalLock(m_hDIB);
	if(NULL == pDIB)
	{
		AfxMessageBox("获得BMP内存块指针失败");
		GlobalUnlock(m_hDIB);
		delete []pde;
		return FALSE;
	}

	//构造BITMAPFILEHEADER并复制到BMP内存块
	BITMAPFILEHEADER bmfHdr;
	memset(&bmfHdr, 0, sizeof(BITMAPFILEHEADER));
	bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
					   256*sizeof(RGBQUAD);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfSize = bmfHdr.bfOffBits + m_dwBmSize;
	bmfHdr.bfType = 0x4d42;

	memmove(pDIB, &bmfHdr, sizeof(BITMAPFILEHEADER));
	
	//构造BITMAPINFOHEADER并复制到BMP内存块
	BITMAPINFOHEADER bmiHdr;
	memset(&bmiHdr, 0, sizeof(BITMAPINFOHEADER));
	bmiHdr.biBitCount = 8;
	bmiHdr.biClrImportant = 0;
	bmiHdr.biClrUsed = 0;
	bmiHdr.biCompression = 0;
	bmiHdr.biHeight = m_sizeDoc.cy;
	bmiHdr.biPlanes = 1;
	bmiHdr.biSize = sizeof(BITMAPINFOHEADER);
	bmiHdr.biSizeImage = 0;
	bmiHdr.biWidth = m_sizeDoc.cx;
	bmiHdr.biXPelsPerMeter = 2834;
	bmiHdr.biYPelsPerMeter = 2834;
	
	memmove((BITMAPFILEHEADER*)pDIB + 1, &bmiHdr, sizeof(BITMAPINFOHEADER));

	//构造256个RGBQUAD并复制到BMP内存块
	RGBQUAD* pRgbQuad = (RGBQUAD*)(pDIB + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	RGBQUAD* pOldQuad = pRgbQuad;
	RGBQUAD rgbQuad;
	memset(&rgbQuad, 0, sizeof(RGBQUAD));
	for(i=0; i<256; i++)
	{
		rgbQuad.rgbBlue = i;
		rgbQuad.rgbGreen = i;
		rgbQuad.rgbRed = i;
		rgbQuad.rgbReserved = 0;
		pRgbQuad = pOldQuad + i;
		memmove(pRgbQuad, &rgbQuad, sizeof(RGBQUAD));
	}

	//填充所有像素数据, 颠倒图象数据从最后一行开始读起
	int j = 0;
	for(i=m_sizeDoc.cy-1; i>=0; i--)
	{
		file.Seek(sizeof(IFH) + i*m_sizeDoc.cx, CFile::begin);
		file.Read((BYTE*)(pRgbQuad + 1) + j*m_sizeDoc.cx, m_sizeDoc.cx);
		j++;
	}

	//初始化专用调色板
	BYTE buf[2+2+4*256];
	LOGPALETTE* pPal = (LOGPALETTE*)buf;
	pPal->palVersion = 0x300;
	pPal->palNumEntries = 256;
	for(i=0; i<255; i++)
	{
		pPal->palPalEntry[i].peBlue = i;
		pPal->palPalEntry[i].peFlags = 0;
		pPal->palPalEntry[i].peGreen = i;
		pPal->palPalEntry[i].peRed = i;
	}
	m_palDIB.CreatePalette(pPal);

	//测试BMP图象
/*	CFile fileWrite("mytiff.bmp", CFile::modeCreate | CFile::modeWrite);
	fileWrite.WriteHuge(pDIB, sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + m_dwBmSize);
*/
	GlobalUnlock(m_hDIB);
	delete []pde;
	return TRUE;
}

BOOL CTiffTestDoc::CreateBmpBuffer()
{
	//申请BMP内存块
	m_hDIB = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + m_dwBmSize);
	if(NULL == m_hDIB)
	{
		AfxMessageBox("申请BMP内存块失败");
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
