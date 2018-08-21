
typedef struct
{
	UINT Width, Height, Colors;
	LPBYTE lpBits;
	DWORD dwNumBytes;
	LPBITMAPINFO lpbi;
	LPBYTE lpXOR;
	LPBYTE lpAND;
}ICONIMAGE, *LPICONIMAGE;
typedef struct
{
	BYTE bWidth;
	BYTE bHeight;
	BYTE bColorCount;
	BYTE bReserved;
	WORD wPlanes;
	WORD wBitCount;
	DWORD dwByteslnRes;
	DWORD dwImageOffset;
}ICONDIRENTRY, *LPICONDIRENTRY;


///////////////////////////////以下为实现部分
#define WIDTHBYTES(bits) ((((bits)+31)>>5)<<2)

UINT CheckHead(const LPBYTE pBuff, long lSize, int& _lPos)
{
	_lPos = 0;
	WORD input;
	if (lSize <= sizeof(WORD) * 3)
		return(UINT)-1;
	memcpy(&input, pBuff + _lPos, sizeof(WORD));
	_lPos += sizeof(WORD);
	if (input != 0)
		return(UINT)-1;
	memcpy(&input, pBuff + _lPos, sizeof(WORD));
	_lPos += sizeof(WORD);
	if (input != 1)
		return(UINT)-1;
	memcpy(&input, pBuff + _lPos, sizeof(WORD));
	_lPos += sizeof(WORD);
	return input;//pBuff中包含的图标个数
}
DWORD DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;
	DWORD dwClrUsed;
	dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
	if (dwClrUsed)
		return (WORD)dwClrUsed;
	wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	switch (wBitCount)
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	case 16:
		return 65535;
	case 32:
		return 4294967295;
	default:
		return 0;
	}
	return 0;
}
DWORD PaletteSize(LPSTR lpbi)
{
	return (DIBNumColors(lpbi) * sizeof(RGBQUAD));
}
LPSTR FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
}
DWORD BytesPerLine(LPBITMAPINFOHEADER lpBMIH)
{
	return WIDTHBYTES(lpBMIH->biWidth*lpBMIH->biPlanes*lpBMIH->biBitCount);
}
HICON MakeIconFromResource(LPICONIMAGE lpIcon)
{
	HICON hIcon = NULL;
	if (lpIcon == NULL || lpIcon->lpBits == NULL)
	{
		return NULL;
	}
	hIcon = CreateIconFromResourceEx(lpIcon->lpBits, lpIcon->dwNumBytes, TRUE, 0x00030000,
		(*(LPBITMAPINFOHEADER)(lpIcon->lpBits)).biWidth, (*(LPBITMAPINFOHEADER)(lpIcon->lpBits)).biHeight / 2, 0);
	if (hIcon == NULL)
	{
		if (lpIcon->lpbi->bmiHeader.biBitCount != 16)
		{
			hIcon = CreateIconFromResource(lpIcon->lpBits, lpIcon->dwNumBytes, TRUE, 0x00030000);
		}
	}
	return hIcon;
}
HICON CreateFromBuff(LPBYTE pBuff, long IDataSize)
{
	HICON _hIcon = NULL;
	if (pBuff == NULL || IDataSize == 0)
		return _hIcon;
	int _lPos = 0;
	if (CheckHead(pBuff, IDataSize, _lPos) == (UINT)-1)
		return _hIcon;
	LPICONDIRENTRY lpIDE = (LPICONDIRENTRY)malloc(sizeof(ICONDIRENTRY));
	if (lpIDE == NULL)
		return _hIcon;
	memcpy(lpIDE, pBuff + _lPos, sizeof(ICONDIRENTRY));
	LPICONIMAGE lpIconImage = (LPICONIMAGE)malloc(sizeof(ICONIMAGE));
	if (lpIconImage == NULL)
		return _hIcon;
	lpIconImage->lpBits = (LPBYTE)malloc(lpIDE->dwByteslnRes);
	memcpy(lpIconImage->lpBits, pBuff + lpIDE->dwImageOffset, lpIDE->dwByteslnRes);
	lpIconImage->lpbi = (LPBITMAPINFO)lpIconImage->lpBits;
	lpIconImage->Width = lpIconImage->lpbi->bmiHeader.biWidth;
	lpIconImage->Height = (lpIconImage->lpbi->bmiHeader.biHeight) / 2;
	lpIconImage->Colors = lpIconImage->lpbi->bmiHeader.biPlanes*lpIconImage->lpbi->bmiHeader.biBitCount;
	lpIconImage->lpXOR = (LPBYTE)FindDIBBits((LPSTR)lpIconImage->lpbi);
	lpIconImage->lpAND = lpIconImage->lpXOR + (lpIconImage->Height*BytesPerLine((LPBITMAPINFOHEADER)lpIconImage->lpbi));
	HICON hTemp = NULL;
	hTemp = MakeIconFromResource(lpIconImage);
	if (hTemp != NULL)
	{
		if (_hIcon != NULL)
			DestroyIcon(_hIcon);
		_hIcon = hTemp;
	}
	free(lpIconImage->lpBits);
	free(lpIconImage);
	free(lpIDE);
	return _hIcon;
}
