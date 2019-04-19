# RGB颜色空间肤色检测 - Popeye_the_Sailor - CSDN博客
2017年10月14日 23:12:34[_Sailor_](https://me.csdn.net/lz0499)阅读数：1151
## RGB颜色空间肤色检测
### 1.算法原理
       基于RGB颜色空间的简单阈值肤色识别
       在[human skin color clustering for face detection](http://scholar.google.com.hk/scholar?q=human+skin+color+clustering+for+face+detection&btnG=&hl=zh-CN&as_sdt=0%2C5&as_vis=1)一文中提出如下简单的判别算式：
　　　　　　R>95 And G>40 And B>20 And R>G And R>B And Max(R,G,B)-Min(R,G,B)>15 And Abs(R-G)>15 
　　  算法非常之简单，同样主要把复杂的判断条件放到后面去判断，能有效的降低程序的执行时间。
### 2.参考代码：
```cpp
int DetectFace(IMAGE_TYPE *bmp_img,DWORD width,DWORD height)
{
	DWORD i,j,bfsize,Source_linebyte,lineByte,SourceSpeed,dstSpeed;
	T_U8 *dst_img,*img_data,*Source_img;
	int R,G,B,Max,Min;
	int colorTablesize;
	int newbiBitCount =8;
	RGBQUAD colortable[256];
	RGBQUAD *pColorTable1 = colortable;
	
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	
	FILE *DetectFace_fp = fopen("DetectFace.bmp","wb");
	
	if(NULL == DetectFace)
	{
		printf("Can't open Fragment.bmp\n");
		return -1;
	}
	
	memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));
	
	bf.bfType = (WORD)0x4d42;
	
    //8位图像数据每行字节数为4的倍数  
    lineByte = (width * newbiBitCount / 8 + 3) / 4 * 4; 
	colorTablesize = 1024; 
	bf.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+colorTablesize+lineByte*height;//图像数据加上调色板信息和前两部分BMP文件信息
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	bf.bfOffbits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+colorTablesize;//注意此offset是BMP前三部分数据，也就是真正图像数据的前面几部分之和
	
	
	bi.biSize   = 40;
	bi.biHeight = height;
	bi.biWidth  = width;
	bi.biPlanes = 1;
	bi.biCompression = 0;
	bi.biBitCount = newbiBitCount;//每一个像素由8 bits表示
	bi.biSizeImage = lineByte*height;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	Source_linebyte = WIDTHBYTES(width*24);
	Source_img = bmp_img+54;
	
	for (i = 0; i<256; ++i)  
    {  
        colortable[i].rgbBlue = i;  
        colortable[i].rgbGreen = i;  
        colortable[i].rgbRed = i;  
        colortable[i].rgbReserved = 0;  
    }  
	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,DetectFace_fp);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,DetectFace_fp);
	fwrite(pColorTable1, sizeof(RGBQUAD), 256, DetectFace_fp);
	img_data = bmp_img+54;
	dst_img = (T_U8*)malloc(height*lineByte);
	memset(dst_img,0,height*lineByte);
	for(i = 0;i < height;i++)
	{
		SourceSpeed = Source_linebyte*i;
		dstSpeed = lineByte*i;
		for(j = 0;j < width;j++)
		{
			B = img_data[SourceSpeed];
			G = img_data[SourceSpeed+1];
			R = img_data[SourceSpeed+2];
			
			if(R > 95 && G > 40 && B > 20 && R > B && R > G && abs(R - G) > 15)
			{
				 if (B >= G)     
				{
					Max = B;
					Min = G;
				}
				else
				{
					Max = G;
					Min = B;
				}
				if (R > Max)
					Max = R;
				else if (R < Min)
					Min = R;
				if (Max - Min > 15) dst_img[dstSpeed] = 255;
			}
			dstSpeed++;
			SourceSpeed += 3;
		}
	}
	fwrite(dst_img,lineByte*height,1,DetectFace_fp);
	fclose(DetectFace_fp);
	DetectFace_fp = NULL;
	free(dst_img);
	return 0;
}
```
### 3.算法结果
![](https://img-blog.csdn.net/20171014233017239)![](https://img-blog.csdn.net/20171014233053996)
![](https://img-blog.csdn.net/20171014233157872)![](https://img-blog.csdn.net/20171014233228573)
有很多误判的区域，还是不够准确！
参考文献：
[肤色检测算法 - 基于不同颜色空间简单区域划分的皮肤检测算法](http://www.cnblogs.com/Imageshop/p/3265353.html)
