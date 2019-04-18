# 图像句柄TO文件 - 深之JohnChen的专栏 - CSDN博客

2007年11月05日 12:42:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1446


void BMPHandle2File(HANDLE hBmp,CString strFileName)
{
 if (hBmp == NULL || strFileName == "")
 {
  return;
 }
 BITMAPFILEHEADER bmfHdr; 
 PBITMAPINFOHEADER pbmiHdr; 
 PBYTE pbBmp = (PBYTE)GlobalLock(hBmp);
 pbmiHdr = (PBITMAPINFOHEADER)pbBmp;
int nColors;
 if(pbmiHdr->biBitCount ==1) // nColors 表示颜色表数量,biBitCount 为1,4,8时,颜色表数为:2,16,256.
{
    nColors =pbmiHdr->biBitCount <<1; //需要定义颜色表,来显示
}
else if(pbmiHdr->biBitCount ==4)
{
    nColors =pbmiHdr->biBitCount <<1;
}
else if(pbmiHdr->biBitCount ==8)
{
    nColors =pbmiHdr->biBitCount <<1;
}
else
{
    pbmiHdr->biBitCount  = 24;
}
 int iLineWidth = (pbmiHdr->biBitCount * pbmiHdr->biWidth + 31) / 32 * 4;
 int iBmpLength = iLineWidth * pbmiHdr->biHeight;

 bmfHdr.bfType = 19778;//;((WORD)('M'<<8) | 'B')
 bmfHdr.bfReserved1 = 0;
 bmfHdr.bfReserved2 = 0;
 bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER)  + sizeof(BITMAPINFOHEADER);
 bmfHdr.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iBmpLength;
 CFile fp;
 if(!fp.Open(strFileName,CFile::modeCreate | CFile::modeWrite))
 {
  GlobalUnlock(hBmp);
  return;
 }
 fp.Write(&bmfHdr,sizeof(BITMAPFILEHEADER));
 fp.Write(pbBmp, sizeof(BITMAPINFOHEADER) + iBmpLength);
 fp.Close();

 GlobalUnlock(hBmp);
} 

