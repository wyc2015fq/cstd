# HBITMAP位图句柄转换图像 - 深之JohnChen的专栏 - CSDN博客

2009年02月03日 10:58:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3800


HBITMAP位图句柄转换图像

//位图句柄转换图像

void WriteBMPFile(HBITMAP hBitMap, LPTSTR filename, HDC hDC) 

{ 

BITMAP bmp;

PBITMAPINFO pbmi;

WORD cClrBits;

HANDLE hf; // file handle

BITMAPFILEHEADER hdr; // bitmap file-header

PBITMAPINFOHEADER pbih; // bitmap info-header

LPBYTE lpBits; // memory pointer

DWORD dwTotal; // total count of bytes

DWORD cb; // incremental count of bytes

BYTE *hp; // byte pointer

DWORD dwTmp;

// create the bitmapinfo header information 

if (!GetObject( hBitMap, sizeof(BITMAP), (LPTSTR)&bmp)) 

{ 

return; 

} 

// Convert the color format to a count of bits.

cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

if (cClrBits == 1)

cClrBits = 1; 

else if (cClrBits <= 4)

cClrBits = 4;

else if (cClrBits <= 8)

cClrBits = 8;

else if (cClrBits <= 16)

cClrBits = 16;

else if (cClrBits <= 24)

cClrBits = 24;

else cClrBits = 32;

// Allocate memory for the BITMAPINFO structure. 

if (cClrBits != 24)

pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits));

else

pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

// Initialize the fields in the BITMAPINFO structure.

pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

pbmi->bmiHeader.biWidth = bmp.bmWidth;

pbmi->bmiHeader.biHeight = bmp.bmHeight;

pbmi->bmiHeader.biPlanes = bmp.bmPlanes;

pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

if (cClrBits < 24)

pbmi->bmiHeader.biClrUsed = (1<<cClrBits);

// If the bitmap is not compressed, set the BI_RGB flag.

pbmi->bmiHeader.biCompression = BI_RGB;

// Compute the number of bytes in the array of color

// indices and store the result in biSizeImage.

pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8 * pbmi->bmiHeader.biHeight * cClrBits;

// Set biClrImportant to 0, indicating that all of the

// device colors are important.

pbmi->bmiHeader.biClrImportant = 0;

// now open file and save the data 

pbih = (PBITMAPINFOHEADER) pbmi;

lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage); 

if (!lpBits)

{ 

return; 

} 

// Retrieve the color table (RGBQUAD array) and the bits

if (!GetDIBits(hDC, HBITMAP(hBitMap), 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS))

{ 

return; 

} 

// Create the .BMP file.

hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, (DWORD) 0,

NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,

(HANDLE) NULL);

if (hf == INVALID_HANDLE_VALUE) 

{ 

return;

} 

hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"

// Compute the size of the entire file.

hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed

* sizeof(RGBQUAD) + pbih->biSizeImage);

hdr.bfReserved1 = 0;

hdr.bfReserved2 = 0;

// Compute the offset to the array of color indices.

hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +

pbih->biSize + pbih->biClrUsed* sizeof (RGBQUAD);

// Copy the BITMAPFILEHEADER into the .BMP file.

if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp, NULL))

{ 

return; 

} 

// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.

if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD),(LPDWORD) &dwTmp, ( NULL))) 

{ 

return; 

} 

// Copy the array of color indices into the .BMP file.

dwTotal = cb = pbih->biSizeImage;

hp = lpBits;

if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 

{ 

return; 

} 

// Close the .BMP file.

if (!CloseHandle(hf)) 

{ 

return; 

} 

// Free memory.

GlobalFree((HGLOBAL)lpBits); 

}

实例：

void CTESTView::OnDraw(CDC* pDC)

{

CTESTDoc* pDoc = GetDocument();

ASSERT_VALID(pDoc);

// TODO: add draw code for native data here

CRect rect;

GetClientRect(&rect);

CDC ImageDC;

ImageDC.CreateCompatibleDC(pDC);

CBitmap bmp,*oldbmp;

bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());

oldbmp = ImageDC.SelectObject(&bmp);

ImageDC.FillSolidRect(rect, pDC->GetBkColor());

ImageDC.MoveTo(10, 10); 

ImageDC.LineTo(300, 300);

WriteBMPFile((HBITMAP)bmp.GetSafeHandle(),"C://1.bmp",ImageDC.m_hDC); 

pDC->BitBlt(0,0,rect.Width(),rect.Height(),&ImageDC,0,0,SRCCOPY); 

ImageDC.SelectObject(oldbmp);

ImageDC.DeleteDC();

}

