# 剪贴板操作——设置获取文本UNICODE-HTML-BITMAP等等 监视剪贴板 保存BITMAP到文件

2012年06月15日 15:05:20 [章志强](https://me.csdn.net/i7thTool) 阅读数：1226



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/i7thTool/article/details/7666538

```cpp
/*
CloseClipboard 关闭剪贴板.
CountClipboardFormats 统计在当前剪贴板里有多少剪贴板格式.
EmptyClipboard 清除剪贴板【必须在OpenClipboard之后调用】.
EnumClipboardFormats 枚举当前剪贴板里可用的剪贴板格式.
GetClipboardData 取当前剪贴板里德指定剪贴板格式的剪贴板内容.
GetClipboardDataAlloc 取当前剪贴板里德指定剪贴板格式的剪贴板内容【当前进程使用，LOCALFREE释放】.
GetClipboardFormatName 得到注册的剪贴板格式的名字.
GetClipboardOwner 得到拥有剪贴板的窗口.
GetOpenClipboardWindow 得到当前打开剪贴板的窗口.
GetPriorityClipboardFormat 得到剪贴板列表里的第一个可用的剪贴板格式.
IsClipboardFormatAvailable 判断剪贴板里是不是有指定格式的剪贴板数据.
OpenClipboard 打开检查剪贴板，这样其他程序就不可以修改剪贴板.
RegisterClipboardFormat 注册新的剪贴板格式，返回有效的剪贴板格式.
SetClipboardData 放置剪贴板数据到指定剪贴板格式的剪贴板上.
*/


class Clipboard
{
public:
	static HWND GetOwner();
protected:
	HWND m_hOwner;
public:
	Clipboard(HWND hOwner = NULL) : m_hOwner(NULL) { if (hOwner) { Open(hOwner); } }
	~Clipboard() { Close(); }

	BOOL Open(HWND hOwner);
	void Close();

	BOOL IsOpen();
	BOOL IsFormatAvailable(UINT uCF);
	//UINT CountFormats();
	//UINT EnumFormats(UINT uCF);

	BOOL SetData(UINT uCF, LPBYTE lpData, UINT DataLen = 0);
	BOOL GetData(UINT uCF, LPBYTE lpData, LPUINT lpDataLen = 0);

	BOOL SetText(LPCSTR lpszText, UINT TextLen);
	BOOL SetText(LPCWSTR lpszText, UINT TextLen);
	BOOL GetText(LPCSTR lpBuffer, LPUINT lpBufferLen);
	BOOL GetText(LPCWSTR lpBuffer, UINT lpBufferLen);

	BOOL SetHtml(LPCSTR lpszHtml, UINT HtmlLen);
	BOOL SetHtml(LPCWSTR lpszHtml, UINT HtmlLen);
};

HWND Clipboard::GetOwner()
{
	return GetClipboardOwner();
}

BOOL Clipboard::Open(HWND hOwner)
{
	ASSERT(!m_hOwner);
	if (OpenClipboard(hOwner)) {
		m_hOwner = hOwner;
		return TRUE;
	}
	return FALSE;
}

void Clipboard::Close()
{
	if (m_hOwner) {
		CloseClipboard();
		m_hOwner = NULL;
	}
}

BOOL Clipboard::IsOpen()
{
	return m_hOwner;
}

BOOL Clipboard::IsFormatAvailable(UINT uCF)
{
	return IsClipboardFormatAvailable(uCF);
}

BOOL Clipboard::SetData(UINT uCF, LPBYTE lpData, UINT DataLen)
{
	ASSERT(!IsOpen());
	BOOL bOk = FALSE;
	if (!lpData && !DataLen) {
		//情况剪贴板
		EmptyClipboard();
		bOk = TRUE;
	}
	else {
		HANDLE hClip = NULL;
		if (DataLen) {
			//有数据
			hClip = GlobalAlloc(GMEM_DDESHARE, DataLen);
			if (hClip) {
				LPBYTE lpClipBuffer = (char *)GlobalLock(hClip);
				memcpy(lpClipBuffer, lpData, DataLen);
				GlobalUnlock(hClip);
			}
		}
		else {
			//只是句柄
			hClip = (HANDLE)lpData;
		}
		if (hClip) {
			EmptyClipboard();
			SetClipboardData(uCF, hClip);
			bOk = TRUE;
		}
	}
	return bOk;
}

BOOL Clipboard::GetData(UINT uCF, LPBYTE lpData, LPUINT lpDataLen)
{
	ASSERT(!IsOpen());
	BOOL bOk = FALSE;
	//IsClipboardFormatAvailable(uCF);
	HANDLE hClip = GetClipboardData(uCF);
	if (hClip) {
		if (lpData && !lpDataLen) {
			((HANDLE)lpData) = hClip;
		}
		else {
			if (!lpData) {
				//返回数据大小
				*lpDataLen = GlobalSize(hClip);
			}
			else {
				//返回数据
				LPBYTE lpClipData = (LPBYTE)::GlobalLock(hClip);
				DWORD dwClipSize = GlobalSize(hClip);
				memcpy(lpData, lpClipData, min(dwClipSize, *lpDataLen));
				GlobalUnlock(hClip);
			}
		}
		bOk = hClip;
	}
	return bOk;
}

BOOL Clipboard::SetText(LPCSTR lpszText, UINT TextLen)
{
	SetData(CF_TEXT, lpszText, TextLen + 1);
}

BOOL Clipboard::SetText(LPCWSTR lpszText, UINT TextLen)
{
	SetData(CF_UNICODETEXT, lpszText, TextLen + 1);
}

BOOL Clipboard::GetText(LPCSTR lpBuffer, LPUINT lpBufferLen)
{
	GetData(CF_TEXT, lpBuffer, lpBufferLen);
}

BOOL Clipboard::GetText(LPCWSTR lpBuffer, UINT lpBufferLen)
{
	GetData(CF_UNICODETEXT, lpBuffer, lpBufferLen);
}

BOOL Clipboard::SetHtml(LPCSTR lpszHtml, UINT HtmlLen)
{//UTF-8
	BOOL bOk = FALSE;
	CHAR* pszHtmlClipboard = new CHAR[HtmlLen + 512];
	memset(pszHtmlClipboard, 0, HtmlLen + 512);
	sprintf(pszHtmlClipboard,
		"Version:0.9\r\n"
		"StartHTML:%08u\r\n"
		"EndHTML:%08u\r\n"
		"StartFragment:%08u\r\n"
		"EndFragment:%08u\r\n"
		"<html><body>\r\n"
		"<!--StartFragment -->\r\n"
		"%s\r\n"
		"<!--EndFragment-->\r\n"
		"</body></html>",
		97, 172 + HtmlLen, 111, 136 + HtmlLen, lpszHtml);
	bOk = SetData(RegisterClipboardFormatA("HTML Format"), pszHtmlClipboard, strlen(pszHtmlClipboard) + 1);
	delete pszHtmlClipboard;
	return bOk;
}

BOOL Clipboard::SetHtml(LPCWSTR lpszHtml, UINT HtmlLen)
{
	BOOL bOk = FALSE;
	int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszHtml, HtmlLen, NULL, 0, NULL, NULL);
	if (nUtf8Len > 0) {
		CHAR* pszUtf8 = new CHAR[nUtf8Len + 1];
		memset(pszUtf8, 0, nUtf8Len + 1);
		WideCharToMultiByte(CP_UTF8, 0, lpszHtml, HtmlLen, pszUtf8, nUtf8Len, NULL, NULL);
		bOk = SetHtml(pszUtf8, nUtf8Len);
		delete[]pszUtf8;
	}
	return bOk;
}



//监视剪贴板变化通知
/*
1、设置监视剪贴板：hwndNextViewer = SetClipboardViewer(hwnd);
2、响应WM_DRAWCLIPBOARD，剪贴板内容变化通知
3、ChangeClipboardChain，修改剪贴板的监听链 ，ChangeClipboardChain(hwnd, hwndNextViewer);从链中移除自己
4、处理剪贴板监听链的变化：
case WM_CHANGECBCHAIN:
// If the next window is closing, repair the chain.
if ((HWND) wParam == hwndNextViewer)
hwndNextViewer = (HWND) lParam;
// Otherwise, pass the message to the next link.
else if (hwndNextViewer != NULL)
SendMessage(hwndNextViewer, uMsg, wParam, lParam);
break;

*/

/ 下面的代码演示了从剪贴板获得图元文件并将其绘制到client DC上：
//绘制剪贴板里的图元
if (OpenClipboard())
{
	//获得剪贴板数据
	HENMETAFILE handle = (HENMETAFILE)GetClipboardData(CF_ENHMETAFILE);
	//显示
	CClientDC dc(this);
	CRect client(0, 0, 200, 200);
	dc.PlayMetaFile(handle, client);
	//关闭剪贴板
	CloseClipboard();
} /

//位图的操作

//设置BITMAP到剪贴板
if (OpenClipboard())
{
	EmptyClipboard();
	CBitmap * junk = new CBitmap();
	CClientDC cdc(this);
	CDC dc;
	dc.CreateCompatibleDC(&cdc);
	CRect client(0, 0, 200, 200);
	junk->CreateCompatibleBitmap(&cdc, client.Width(), client.Height());
	dc.SelectObject(junk);
	DrawImage(&dc, CString("Bitmap"));
	//复制数据到剪贴板
	SetClipboardData(CF_BITMAP, junk->m_hObject);
	CloseClipboard();
	delete junk;
} /
/
//绘制剪贴板里的BITMAP
if (OpenClipboard())
{
	//获得剪贴板数据
	HBITMAP handle = (HBITMAP)GetClipboardData(CF_BITMAP);
	CBitmap * bm = CBitmap::FromHandle(handle);
	CClientDC cdc(this);
	CDC dc;
	dc.CreateCompatibleDC(&cdc);
	dc.SelectObject(bm);
	cdc.BitBlt(0, 0, 200, 200, &dc, 0, 0, SRCCOPY);
	CloseClipboard();
} /
/
//保存BITMAP到文件
BOOL SaveBitmap(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	//设备描述表
	HDC hDC;
	//当前分辨率下每象素所占字节数
	int iBits;
	//位图中每象素所占字节数
	WORD wBitCount;
	//定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构
	BITMAP Bitmap;
	//位图文件头结构
	BITMAPFILEHEADER bmfHdr;
	//位图信息头结构
	BITMAPINFOHEADER bi;
	//指向位图信息头结构
	LPBITMAPINFOHEADER lpbi;
	//定义文件，分配内存句柄，调色板句柄
	HANDLE hFile, hDib, hPal, hOldPal = NULL;
	//计算位图文件每个像素所占字节数
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1) wBitCount = 1;
	else if (iBits <= 4)  wBitCount = 4;
	else if (iBits <= 8)  wBitCount = 8;
	else wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	//为位图内容分配内存
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hDC = ::GetDC(NULL);
	// 处理调色板
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal) {
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
	//恢复调色板
	if (hOldPal) {
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
	}
	::ReleaseDC(NULL, hDC);

	//创建位图文件

	hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		// 设置位图文件头
		bmfHdr.bfType = 0x4D42; // "BM"
		dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
		bmfHdr.bfSize = dwDIBSize;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
		// 写入位图文件头
		WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		// 写入位图文件其余内容
		WriteFile(hFile, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
		CloseHandle(hFile);
	}
	//清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	return TRUE;
} /
///使用GDI+保存BITMAP到文件
//CLSID encoderClsid
//GetEncoderClsid(L"image/png", &encoderClsid);    //png
//GetEncoderClsid(L"image/bmp", &encoderClsid);
//GetEncoderClsid(L"image/gif", &encoderClsid);
//GetEncoderClsid(L"image/jpeg",&encoderClsid);
//GetEncoderClsid(L"image/tiff",&encoderClsid);
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0) {
		return -1;
	}
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL) {
		return -1;
	}
	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j< num; ++j) {
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}
BOOL SaveBitmap(HBITMAP hBitmap, LPCTSTR lpszFileName, LPCTSTR lpszFormat)
{
	Bitmap bmp(hBitmap, NULL);
	CLSID encoderclsid;
	GetEncoderClsid(lpszFormat, &encoderclsid);
	if (Status::Ok == bmp.Save(lpszFileName, encoderclsid, NULL)) {
		return TRUE;
	}
	return FALSE;
}
//SaveBitmap(hBitmap, L"C:\123.jpeg", L"image/jpeg");

```

