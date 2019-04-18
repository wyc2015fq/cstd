# BMP图像文件TO数据区的互换 - 深之JohnChen的专栏 - CSDN博客

2006年04月25日 14:09:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2198标签：[算法																[fp																[bi](https://so.csdn.net/so/search/s.do?q=bi&t=blog)](https://so.csdn.net/so/search/s.do?q=fp&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)


**BMP图像文件TO数据区的互换**

                                                                                                                 cndg                         2006/04/25

       一般为了查看图象的效果,比如写了一个算法,想看看效果怎么样,可以把图像数据（一维或者二维数组）保存到BMP文件看效果。下面写一些代码实现BMP文件TO图象数据，图象数据TOBMP文件。

1、图象数据到BMP文件转换。

/*
*   函数功能：数据到BMP的转换
*  参数说明：unsigned char *p，指向图像数据区域；
                        int width，图像的高度；
                        int height，图像的宽度；
                        int bitcount，每个像数的字节数
* 返回值：VOID
*/

void DataToBmp(unsigned char *p, int width, int height, int bitcount, int flag = 0)
{
 BITMAPFILEHEADER bmfHdr; 
//定义文件头
 BITMAPINFOHEADER bmiHdr; 
//定义信息头
 RGBQUAD rgbQuad[256];    
//定义调色板
 int i;
 int dpl;
 int colorused = 0;
 int nbytes;
 nbytes = bitcount / 8;
    //对信息头进行赋值
 bmiHdr.biSize = sizeof(BITMAPINFOHEADER);
 bmiHdr.biWidth = width;
 bmiHdr.biHeight = height;
 bmiHdr.biPlanes = 1;
 bmiHdr.biBitCount = bitcount;
 bmiHdr.biCompression = BI_RGB;
 bmiHdr.biSizeImage = 0;
 bmiHdr.biXPelsPerMeter = 0;
 bmiHdr.biYPelsPerMeter = 0;
 bmiHdr.biClrUsed = 0;
 bmiHdr.biClrImportant = 0;
 memset(rgbQuad,0,sizeof(rgbQuad));
 if (bitcount == 8)
  for (i=0;i<256;i++){
   rgbQuad[i].rgbBlue =i;
   rgbQuad[i].rgbGreen =i;
   rgbQuad[i].rgbRed =i;
   rgbQuad[i].rgbReserved =0;
  }
  if( bmiHdr.biBitCount<=8 )
  {
   colorused=bmiHdr.biClrUsed;
   if( colorused==0) colorused=1<<bmiHdr.biBitCount;
  }

  //dpl = ((bmiHdr.biWidth * bmiHdr.biBitCount + 31) & ~31) >> nbytes; 
  if (width*nbytes % 4 == 0)
  {
   dpl = width*nbytes;
  }
  else
  {
   dpl = width*nbytes + 4 - width*nbytes%4;
  }
  int actualdpl = dpl;
  if (flag == 1)
  {
   dpl = width*nbytes;
  }
  //对文件头进行赋值
  bmfHdr.bfType = 19778;//;((WORD)('M'<<8) | 'B')
  bmfHdr.bfReserved1 = 0;
  bmfHdr.bfReserved2 = 0;
  bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER)+ bmiHdr.biSize + colorused*sizeof(RGBQUAD);
  bmfHdr.bfSize= (bmfHdr.bfOffBits+dpl*height);
  //保存文件
  CFile fp;
  fp.Open("d://Model.bmp",CFile::modeCreate | CFile::modeWrite);
  fp.Write((LPSTR)&bmfHdr,sizeof(BITMAPFILEHEADER)); 
//写文件头
  fp.Write((LPSTR)&bmiHdr,sizeof(BITMAPINFOHEADER)); 
//写信息头
  fp.Write((LPSTR)rgbQuad,sizeof(RGBQUAD)*colorused);   
//写调色板
  int j;
  int value = 0;
  for (i=0; i<height; i++)
  {
   fp.Write((LPSTR)(p+dpl*i), width*nbytes);
//写数据
   for (j=0; j<actualdpl-width*nbytes; j++)
   {
    fp.Write((LPSTR)(&value), 1);
   }
   // fp.Write((LPSTR)0, dpl - width*3);
  }

  fp.Close();
}

2、BMP文件到图象数据转换。

根据文件的格式包数据读到一片内存中。
请参考：[http://www.vckbase.com/document/viewdoc/?id=674](http://www.vckbase.com/document/viewdoc/?id=674)

