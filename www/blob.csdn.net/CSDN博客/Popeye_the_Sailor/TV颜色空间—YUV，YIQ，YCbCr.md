# TV颜色空间—YUV，YIQ，YCbCr - Popeye_the_Sailor - CSDN博客
2017年10月01日 00:20:48[_Sailor_](https://me.csdn.net/lz0499)阅读数：847
## **TV颜色空间—YUV，YIQ，YCbCr**
### 1.前言
     自然界的颜色千变万化，为了给颜色一个量化的衡量标准，就需要建立色彩空间模型来描述各种各样的颜色，由于人对色彩的感知是一个复杂的生理和心理联合作用的过程，所以在不同的应用领域中为了更好更准确的满足各自的需求，就出现了各种各样的色彩空间模型来量化的描述颜色。我们比较常接触到的就包括 RGB / CMYK / YIQ / YUV / HSI等等。
    对于数字电子多媒体领域来说，我们经常接触到的色彩空间的概念，主要是RGB ,YUV这两种（实际上，这两种体系包含了许多种具体的颜色表达方式和模型，如sRGB, Adobe RGB,YUV422, YUV420 …）, RGB是按三基色加光系统的原理来描述颜色，而YUV则是按照 亮度，色差的原理来描述颜色。“Y”表示明亮度（Luminance或Luma），也就是灰度值；而“U”和“V”表示的则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。
### 2.YUV颜色空间的由来
     计算机彩色显示器显示色彩的原理与彩色电视机一样，都是采用R（Red）、G（Green）、B（Blue）相加混色的原理：通过发射出三种不同强度的电子束，使屏幕内侧覆盖的红、绿、蓝磷光材料发光而产生色彩。这种色彩的表示方法称为RGB色彩空间表示（它也是多媒体计算机技术中用得最多的一种色彩空间表示方法）。
根据三基色原理，任意一种色光F都可以用不同分量的R、G、B三色相加混合而成。
F = r [R] + g [G] + b [B]
    其中，r、g、b分别为三基色参与混合的系数。当三基色分量都为0（最弱）时混合为黑色光；而当三基色分量都为k（最强）时混合为白色光。调整r、g、b三个系数的值，可以混合出介于黑色光和白色光之间的各种各样的色光。
   那么YUV又从何而来呢？在现代彩色电视系统中，通常采用三管彩色摄像机或彩色CCD摄像机进行摄像，然后把摄得的彩色图像信号经分色、分别放大校正后得到RGB，再经过矩阵变换电路得到亮度信号Y和两个色差信号R－Y（即U）、B－Y（即V），最后发送端将亮度和色差三个信号分别进行编码，用同一信道发送出去。这种色彩的表示方法就是所谓的YUV色彩空间表示。用这种方式视频数据的传输占用的带宽是极小的。
   采用YUV色彩空间的重要性是它的亮度信号Y和色度信号U、V是分离的。如果只有Y信号分量而没有U、V分量，那么这样表示的图像就是黑白灰度图像。彩色电视采用YUV空间正是为了用亮度信号Y解决彩色电视机与黑白电视机的兼容问题，使黑白电视机也能接收彩色电视信号。此外，在ISP某些模块中需要使用YUV格式，更加方便的对Y分量和U、V分量分别进行不同处理，使得相应的算法更加容易实现。**ISP中常用YUV格式进行相应图像处理的模块有：Y gamma（Y伽马）、Saturation（饱和度调整）、WDR（宽动态）、3DNR（3维降噪）、Sharp（锐化）等。**
### **3.YUV颜色空间**
    YUV颜色空间是北美NTSC和欧洲PAL模拟电视系统颜色编码的基础。亮度分量Y通过下述公式计算出：
![](https://img-blog.csdn.net/20171001002117589)
   上述公式假定的是RGB值是根据TV编码标准进行Gamma校正之后逇值。UV分量是分别对亮度分量与红色分量、亮度分量与蓝色分量进行差值之后再赋予一定的权重：
![](https://img-blog.csdn.net/20171001002145567)
    因此，从RGB转换为YUV的关系为：
![](https://img-blog.csdn.net/20171001002245776)
  相应的，从YUV转换为RGB的关系为：
![](https://img-blog.csdn.net/20171001002309197)
### **4.YIQ颜色空间**
    原始的NTSC系统用了一种YUV的变体进行颜色编码，这种变体称之为YIQ（I for“in-phase”and Qfor “quadrature”）。IQ分量是UV分量进行旋转和镜像之后的数值：
![](https://img-blog.csdn.net/20171001002309197)
  ß=0.576（33度）。Y分量同上述YUV颜色空间计算方式一样。
### **5.YCbCr颜色空**
    YCbCr颜色空间是YUV颜色空间的一种变体，广泛应用于数字电视系统和图像压缩方面（比如JPEG）。色度分量Cb,Cr类似于U,V分量，也是通过亮度值与红色分量和蓝色分量差值之后，在赋予一定的权重计算得出。相比于YUV颜色空间，在RGB转换为YUV颜色空间的时候，其各个分量的权重值稍有不同。如下公式：
![](https://img-blog.csdn.net/20171001002509713)
  相应的从YCbCr转换为RGB颜色空间为：
![](https://img-blog.csdn.net/20171001002536189)
   BT.601中规定WR= 0.299，Wb=0.144,WG = 0.587,因此，上述公式变换为：
![](https://img-blog.csdn.net/20171001002559053)
   YCbCr转换为RGB公式为：
![](https://img-blog.csdn.net/20171001002631776)
**    由于上述U,VI,Q,Cb,Cr可能为负数或正数，为了图像能够显示这些分量的负值，一般会在上述分量计算公式之后加上一个偏移量，使得其颜色分量为正值。比如，对于8bit表示一个分量的像素而言，其颜色分量的偏移量一般设置为128=2的7次方。**
### **6.关键代码：**
```cpp
int RGB2YUV(IMAGE_TYPE *bmp_img, DWORD width, DWORD height,TYPE_YUV type)
{
	T_U32 lineByte,Source_linebyte,source_index,dst_index;
	T_U16 i,j;
	T_U16 k = 0;
	T_U8 *Bmp8_img,*Source_img,R,G,B,*Y_img,*U_img,*V_img;
	int newbiBitCount =8,Y,U,V,I,Q,Cb,Cr;
	int colorTablesize;
	RGBQUAD colortable[256];
	RGBQUAD *pColorTable1 = colortable;
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	
	FILE *BMP2Y_fp = fopen("BMP2Y.bmp","wb");
	FILE *BMP2U_fp = fopen("BMP2U.bmp","wb");
	FILE *BMP2V_fp = fopen("BMP2V.bmp","wb");
	
	if(NULL == BMP2Y_fp)
	{
		printf("Can't open BMP2Y.bmp\n");
		return -1;
	}
	if(NULL == BMP2U_fp)
	{
		printf("Can't open BMP2U.bmp\n");
		return -1;
	}
	if(NULL == BMP2V_fp)
	{
		printf("Can't open BMP2V.bmp\n");
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
	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,BMP2Y_fp);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,BMP2Y_fp);
	fwrite(pColorTable1, sizeof(RGBQUAD), 256, BMP2Y_fp);
	
	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,BMP2U_fp);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,BMP2U_fp);
	fwrite(pColorTable1, sizeof(RGBQUAD), 256, BMP2U_fp);
			
	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,BMP2V_fp);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,BMP2V_fp);
	fwrite(pColorTable1, sizeof(RGBQUAD), 256, BMP2V_fp);
	Source_img = bmp_img+54;
	Y_img = (T_U8*)malloc(width*height);
	U_img = (T_U8*)malloc(width*height);
	V_img = (T_U8*)malloc(width*height);	
	if(NULL == Y_img || NULL == U_img || NULL == V_img)
	{
		printf("Can't malloc YUV_img\n");
		return -1;
	}
	switch (type)
	{
	case YUV:
		{
			for (i = 0; i < height;i++)
			{
				for (j = 0;j < width;j++)
				{
					source_index = Source_linebyte*i+3*j;
					dst_index = lineByte*i+j;
					R = Source_img[source_index+2];
					G = Source_img[source_index+1];
					B = Source_img[source_index];
#if 0
					Y = 0.299*R+0.548*G+0.114*B;
					U = -0.147*R-0.289*G+0.436*B+128;
					V = 0.615*R-0.515*G-0.100*B+128;
#endif 
					Y = (306*R+561*G+117)>>10;
					U = (-150*R-296*G+446*B+131072)>>10;
					V = (630*R-527*G-102*G+131072)>>10;
					Y_img[dst_index] = (T_U8)Y;
					U_img[dst_index] = (T_U8)U;
					V_img[dst_index] = (T_U8)V;
				}
			}
		}
	case YIQ:
		{
			for (i = 0; i < height;i++)
			{
				for (j = 0;j < width;j++)
				{
					source_index = Source_linebyte*i+3*j;
					dst_index = lineByte*i+j;
					R = Source_img[source_index+2];
					G = Source_img[source_index+1];
					B = Source_img[source_index];
#if 1
					Y = 0.299*R+0.548*G+0.114*B;
					U = -0.147*R-0.289*G+0.436*B;
					V = 0.615*R-0.515*G-0.100*B;
#endif 
#if 0
					Y = (306*R+561*G+117)>>10;
					U = (-150*R-296*G+446*B+131072)>>10;
					V = (630*R-527*G-102*G+131072)>>10;
#endif
					I =(int) (-sin(0.576)*U+V*cos(0.576))+128;
					Q = (int)(U*cos(0.576)+V*sin(0.576))+128;
					Y_img[dst_index] = (T_U8)Y;
					U_img[dst_index] = (T_U8)I;
					V_img[dst_index] = (T_U8)Q;
				}
			}
		}
	case YCBCR:
		{
			for (i = 0; i < height;i++)
			{
				for (j = 0;j < width;j++)
				{
					source_index = Source_linebyte*i+3*j;
					dst_index = lineByte*i+j;
					R = Source_img[source_index+2];
					G = Source_img[source_index+1];
					B = Source_img[source_index];
#if 1
					Y = 0.299*R+0.587*G+0.114*B;
					Cb = -0.169*R-0.331*G+0.5*B+128;
					Cr = 0.5*R-0.419*G-0.081*B+128;
#endif 
#if 0
					Y = (306*R+601*G+117)>>10;
					Cb = (-173*R-339*G+512*B+131072)>>10;
					Cr = (512*R-429*G-83*G+131072)>>10;
#endif
					Y_img[dst_index] = (T_U8)Y;
					U_img[dst_index] = (T_U8)Cb;
					V_img[dst_index] = (T_U8)Cr;
				}
			}
		}
		break;
	}
	fwrite(Y_img, lineByte*height, 1, BMP2Y_fp);
	fwrite(U_img, lineByte*height, 1, BMP2U_fp);
	fwrite(V_img, lineByte*height, 1, BMP2V_fp);
	fclose(BMP2Y_fp);  
	fclose(BMP2U_fp); 
	fclose(BMP2V_fp); 
	free(Y_img);
	free(U_img);
	free(V_img);
	
	return 0;
}
```
### **7.图像效果**
![](https://img-blog.csdn.net/20171001003502266)
![](https://img-blog.csdn.net/20171001003622756)![](https://img-blog.csdn.net/20171001003704165)![](https://img-blog.csdn.net/20171001003646966)
YUV颜色空间的Y、U、V分量
![](https://img-blog.csdn.net/20171001003927529)![](https://img-blog.csdn.net/20171001005659470)![](https://img-blog.csdn.net/20171001004150328)
YIQ颜色空间的Y、I、Q分量
![](https://img-blog.csdn.net/20171001004541728)![](https://img-blog.csdn.net/20171001005355740)![](https://img-blog.csdn.net/20171001005507424)

YCbCr颜色空间的Y、Cb、Cr分量

