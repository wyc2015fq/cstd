# RGB数据保存为BMP图片 - yixianfeng41的专栏 - CSDN博客
2016年09月20日 04:31:08[第2梦](https://me.csdn.net/yixianfeng41)阅读数：8113
### 一、BMP文件由[文件头](http://baike.baidu.com/view/1606305.htm)、位图信息头、颜色信息和[图形数据](http://baike.baidu.com/view/286856.htm)四部分组成。
#### 1、BMP文件头（14字节）
**[cpp]**[view
 plain](http://blog.csdn.net/yixianfeng41/article/details/46712723#)[copy](http://blog.csdn.net/yixianfeng41/article/details/46712723#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1626994/fork)
- typedefstruct/**** BMP file header structure ****/
- {  
-     unsigned int   bfSize;           /* Size of file */
-     unsigned short bfReserved1;      /* Reserved */
-     unsigned short bfReserved2;      /* ... */
-     unsigned int   bfOffBits;        /* Offset to bitmap data */
- } MyBITMAPFILEHEADER;  
#### 2、位图信息头（40字节）
**[cpp]**[view
 plain](http://blog.csdn.net/yixianfeng41/article/details/46712723#)[copy](http://blog.csdn.net/yixianfeng41/article/details/46712723#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1626994/fork)
- typedefstruct/**** BMP file info structure ****/
- {  
-     unsigned int   biSize;           /* Size of info header */
- int            biWidth;          /* Width of image */
- int            biHeight;         /* Height of image */
-     unsigned short biPlanes;         /* Number of color planes */
-     unsigned short biBitCount;       /* Number of bits per pixel */
-     unsigned int   biCompression;    /* Type of compression to use */
-     unsigned int   biSizeImage;      /* Size of image data */
- int            biXPelsPerMeter;  /* X pixels per meter */
- int            biYPelsPerMeter;  /* Y pixels per meter */
-     unsigned int   biClrUsed;        /* Number of colors used */
-     unsigned int   biClrImportant;   /* Number of important colors */
- } MyBITMAPINFOHEADER;  
#### 3、颜色表
颜色表用于说明位图中的颜色，它有若干个表项，每一个表项是一个RGBQUAD类型的结构，定义一种颜色。RGBQUAD结构的定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/yixianfeng41/article/details/46712723#)[copy](http://blog.csdn.net/yixianfeng41/article/details/46712723#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1626994/fork)
- typedefstructtagRGBQUAD{  
-     BYTErgbBlue;//蓝色的亮度（值范围为0-255)
-     BYTErgbGreen;//绿色的亮度（值范围为0-255)
-     BYTErgbRed;//红色的亮度（值范围为0-255)
-     BYTErgbReserved;//保留，必须为0
- }RGBQUAD;  
颜色表中的RGBQUAD结构数据的个数由biBitCount来确定：当biBitCount=1,4,8时，分别为2,16,256个表项；当biBitCount=24时，没有颜色表项。
#### 4、位图数据
位图[数据记录](http://baike.baidu.com/view/1266613.htm)了位图的每一个像素值，记录顺序是在扫描行内是从左到右，扫描行之间是从下到上。位图的一个像素值所占的字节数：
当biBitCount=1时，8个像素占1个字节；
当biBitCount=4时，2个像素占1个字节；
当biBitCount=8时，1个像素占1个字节；
当biBitCount=24时，1个像素占3个字节,按顺序分别为B,G,R；
### 二、将rgb数据保存为bmp图片的方法
**[cpp]**[view
 plain](http://blog.csdn.net/yixianfeng41/article/details/46712723#)[copy](http://blog.csdn.net/yixianfeng41/article/details/46712723#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1626994/fork)
- void CDecVideoFilter::MySaveBmp(constchar *filename,unsigned char *rgbbuf,int width,int height)  
- {  
-     MyBITMAPFILEHEADER bfh;  
-     MyBITMAPINFOHEADER bih;  
- /* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
-     unsigned short bfType=0x4d42;             
-     bfh.bfReserved1 = 0;  
-     bfh.bfReserved2 = 0;  
-     bfh.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+width*height*3;  
-     bfh.bfOffBits = 0x36;  
- 
-     bih.biSize = sizeof(BITMAPINFOHEADER);  
-     bih.biWidth = width;  
-     bih.biHeight = height;  
-     bih.biPlanes = 1;  
-     bih.biBitCount = 24;  
-     bih.biCompression = 0;  
-     bih.biSizeImage = 0;  
-     bih.biXPelsPerMeter = 5000;  
-     bih.biYPelsPerMeter = 5000;  
-     bih.biClrUsed = 0;  
-     bih.biClrImportant = 0;  
- 
- FILE *file = fopen(filename, "wb");  
- if (!file)  
-     {  
-         printf("Could not write file\n");  
- return;  
-     }  
- 
- /*Write headers*/
-     fwrite(&bfType,sizeof(bfType),1,file);  
-     fwrite(&bfh,sizeof(bfh),1, file);  
-     fwrite(&bih,sizeof(bih),1, file);  
- 
-     fwrite(rgbbuf,width*height*3,1,file);  
-     fclose(file);  
- }  
### 三、测试
具体测试代码：[RGB TO BMP TEST](http://download.csdn.net/detail/yixianfeng41/8865639)
