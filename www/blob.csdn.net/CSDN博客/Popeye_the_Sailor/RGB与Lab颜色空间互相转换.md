# RGB与Lab颜色空间互相转换 - Popeye_the_Sailor - CSDN博客
2017年08月17日 23:25:17[_Sailor_](https://me.csdn.net/lz0499)阅读数：18688
## **RGB与Lab颜色空间互相转换**
### **1.Lab颜色空间**
       同RGB颜色空间相比（见博客《[光与色的故事--颜色空间浅析](http://www.cnblogs.com/Imageshop/archive/2013/02/02/2889897.html)》），Lab是一种不常用的色彩空间。它是在1931年国际照明委员会（CIE）制定的颜色度量国际标准的基础上建立起来的。1976年，经修改后被正式命名为CIELab。它是一种设备无关的颜色系统，也是一种基于生理特征的颜色系统。这也就意味着，它是用数字化的方法来描述人的视觉感应。Lab颜色空间中的L分量用于表示像素的亮度，取值范围是[0,100],表示从纯黑到纯白；a表示从红色到绿色的范围，取值范围是[127,-128]；b表示从黄色到蓝色的范围，取值范围是[127,-128]。下图所示为Lab颜色空间的图示；
![](https://img-blog.csdn.net/20170817231645904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
      需要提醒的是，Lab颜色空间比计算机显示器、打印机甚至比人类视觉的色域都要大，表示为 Lab 的位图比RGB 或CMYK 位图获得同样的精度要求更多的每像素数据。虽然我们在生活中使用RGB颜色空间更多一些，但也并非Lab颜色空间真的一无所有。例如，在Adobe Photoshop图像处理软件中，TIFF格式文件中，PDF文档中，都可以见到Lab颜色空间的身影。而在[**计算机视觉**](http://lib.csdn.net/base/computervison)中，尤其是颜色识别相关的[**算法**](http://lib.csdn.net/base/datastructure)设计中，rgb,hsv,lab颜色空间混用更是常用的方法。
**两者的区别：**
      RGB的是由红色通道（R）、绿色通道（G）、蓝色通道（B）组成的，最亮的红色+最亮的绿色+最亮的蓝色=白色；最暗的红色+最暗的绿色+最暗的蓝色=黑色；而在最亮和最暗之间，相同明暗度的红色+相同明暗度的绿色+相同明暗度的蓝色=灰色。在RGB的任意一个通道内，白和黑表示这个颜色的明暗度。所以，有白色或者灰白色的地方，R、G、B三个通道都不可能是黑色的，因为必须要有R、G、B三个通道来构成这些颜色。
而LAB不一样，LAB中的明度通道（L）专门负责整张图的明暗度，简单的说就是整幅图的黑白版。a通道和b通道只负责颜色的多少。a通道表示从洋红色（通道里的白色）至深绿色（通道里的黑色）的范围；b表示从焦黄色（通道里的白色）至袅蓝色（通道里的黑色）的范围；a、b通道里的50%中性灰色表示没有颜色，所以越接近灰色说明颜色越少，而且a通道和b通道的颜色没有亮度。这就说明了为什么在a、b通道中红色T恤的轮廓是那么的清晰！因为红色是洋红色+焦黄色组成的。
总的来说：
      1、适合RGB通道抠的图大部分LAB模式能完成，反之不成立。
      2、任何单一色调背景下，用通道抠有明显颜色区别的部分，用LAB模式很快能完成
      3、LAB模式下对明度（L）通道做任何操作（如锐化、模糊等）不会影响到色相。
### **2.RGB转Lab颜色空间**
      RGB颜色空间不能直接转换为Lab颜色空间，需要借助XYZ颜色空间，把RGB颜色空间转换到XYZ颜色空间，之后再把XYZ颜色空间转换到Lab颜色空间。
     RGB与XYZ颜色空间有如下关系：
![](https://img-blog.csdn.net/20170817231806394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
      仔细观察式（1），其中 X = 0.412453 * R +  0.412453 *G+ 0.412453B ;  各系数相加之和为0.950456，非常接近于1，我们知道R/G/B的取值范围为[ 0,255 ]，如果系数和等于1，则X的取值范围也必然在[ 0,255 ]之间，因此我们可以考虑等比修改各系数，使其之和等于1，这样就做到了XYZ和RGB在同等范围的映射。这也就是为什么代码里X,Y,Z会分别除以0.950456、1.0、1.088754。
![](https://img-blog.csdn.net/20170817232123204?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170817232134530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
RGB2Lab关键代码实现：
```cpp
//RGB2Lab Lab2RGB
const float param_13 = 1.0f / 3.0f;
const float param_16116 = 16.0f / 116.0f;
const float Xn = 0.950456f;
const float Yn = 1.0f;
const float Zn = 1.088754f;
void RGB2XYZ(T_U8 R, T_U8 G, T_U8 B, float *X, float *Y, float *Z)
{
	 *X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
	 *Y = 0.212671f * R + 0.715160f * G + 0.072169f * B;
	 *Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;
}
void XYZ2Lab(float X, float Y, float Z, float *L, float *a, float *b)
{
	float fX, fY, fZ;
#if 1
	X /= (255.0*Xn);
	Y /= (255.0*Yn);
	Z /= (255.0*Zn);
#endif
	if (Y > 0.008856f)
	{
		fY = pow(Y, param_13);
		*L = 116.0f * fY -16.0f;
	}
	else
	{
		fY = 7.787f * Y + param_16116;
		*L = 903.3*fY;
	}
	
	
	*L = *L > 0.0f ? *L : 0.0f;
	
	if (X > 0.008856f)
		fX = pow(X, param_13);
	else
		fX = 7.787f * X + param_16116;
	
	if (Z > 0.008856)
		fZ = pow(Z, param_13);
	else
		fZ = 7.787f * Z + param_16116;
	*a = 500.0f * (fX - fY);
	*b = 200.0f * (fY - fZ);
}
int RGB2Lab(IMAGE_TYPE *bmp_img,float *lab_img)
{
	DWORD width,height,index;
	WORD  biBitCount;
	T_U8 *dst,*bmp,R,G,B;
	float X,Y,Z,L,a,b;
	
	T_U32 line_byte;
	T_U16 i,j;
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	
	memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));
	
	bmp = bmp_img;
	memcpy(&bf,bmp,14);
	memcpy(&bi,&bmp[14],40);
	
	height = bi.biHeight;
	width  = bi.biWidth;
	biBitCount = bi.biBitCount;//每一个像素由24 bits表示，即RGB分量每一个分量用8 bits表示
	line_byte = WIDTHBYTES(width*bi.biBitCount);
	dst = bmp_img+BMPHEADSIZE;
	
	for (i = 0; i <height;i++)
	{
		for (j = 0;j < width;j++)
		{
			index = i*line_byte+3*j;
			B = dst[index];
			G = dst[index+1];
			R = dst[index+2];
			RGB2XYZ(R,G,B,&X,&Y,&Z);
			XYZ2Lab(X,Y,Z,&L,&a,&b);
			lab_img[index] = L;
			lab_img[index+1] = a;
			lab_img[index+2] = b;
		}
	}
    
	return 0;
}
```
### **3.Lab转RGB颜色空间**
![](https://img-blog.csdn.net/20170817232237694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170817232242720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Lab2RGB关键代码实现：
```cpp
extern const float param_13;
extern const float param_16116;
extern const float Xn;
extern const float Yn;
extern const float Zn;
void XYZ2RGB(float X, float Y, float Z, T_U8 *R, T_U8 *G, T_U8 *B)
{
	float RR, GG, BB;
	RR = 3.240479f * X - 1.537150f * Y - 0.498535f * Z;
	GG = -0.969256f * X + 1.875992f * Y + 0.041556f * Z;
	BB = 0.055648f * X - 0.204043f * Y + 1.057311f * Z;
	if(RR<0.0)
	{
		RR = 0;
	}else if(RR>255.0)
	{
		RR = 255;
	}else
	{
		RR = (T_U8)RR;
	}
	if(GG<0.0)
	{
		GG = 0;
	}else if(GG>255.0)
	{
		GG = 255;
	}
	else
	{
		GG = (T_U8)GG;
	}
	if(BB<0.0)
	{
		BB = 0;
	}else if(BB>255.0)
	{
		BB = 255;
	}
	else
	{
		BB = (T_U8)BB;
	}
	
	*R = (T_U8)RR;
	*G = (T_U8)GG;
	*B = (T_U8)BB;
 }
void Lab2XYZ(float L, float a, float b, float *X, float *Y, float *Z)
{
	float fX, fY, fZ;
	
	fY = (L + 16.0f) / 116.0f;
	if (fY > 0.206893f)
		*Y = fY * fY * fY;
	else
		*Y = (fY-param_16116) / 7.787f;
	
	fX = a / 500.0f + fY;
	if (fX > 0.206893f)
		*X = fX * fX * fX;
	else
		*X = (fX-param_16116) / 7.787f;
	
		fZ = fY - b / 200.0f;
	if (fZ > 0.206893f)
		*Z = fZ * fZ * fZ;
	else
		*Z = (fZ -param_16116) / 7.787f;
#if 1
	(*X) *= (0.950456f*255.0);
	(*Y) *= (255.0*1.0);
	(*Z) *= (1.088754f*255.0);
#endif
}
int Lab2RGB(IMAGE_TYPE *bmp_img,float *lab_img)
{
	DWORD width,height,index;
	WORD  biBitCount;
	T_U8 *bmp,R,G,B,*Lab2BMP;
	float X,Y,Z,L,a,b;
	
	T_U32 line_byte;
	T_U16 i,j;
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	FILE *Lab2BMP_fp = fopen("Lab2BMP.bmp","wb");
	
	if(NULL == Lab2BMP_fp)
	{
		printf("Can't open Lab2BMP.bmp\n");
		return -1;
	}
	
	memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));
	bmp = bmp_img;
	memcpy(&bf,bmp,14);
	memcpy(&bi,&bmp[14],40);
	height = bi.biHeight;
	width  = bi.biWidth;
	biBitCount = bi.biBitCount;//每一个像素由24 bits表示，即RGB分量每一个分量用8 bits表示
	line_byte = WIDTHBYTES(width*bi.biBitCount);
	
	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,Lab2BMP_fp);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,Lab2BMP_fp);
	Lab2BMP = (T_U8*)malloc(height*line_byte);
	if (Lab2BMP == NULL)
	{
		printf("Can't malloc LabBMP image.\n");
		return 0;
	}
	memset(Lab2BMP,0,height*line_byte);
	
	for (i = 0; i <height;i++)
	{
		for (j = 0;j < width;j++)
		{
			index = i*line_byte+3*j;
			L = lab_img[index];
			a = lab_img[index+1];
			b = lab_img[index+2];
			Lab2XYZ(L,a,b,&X,&Y,&Z);
			XYZ2RGB(X,Y,Z,&R,&G,&B);
		
			Lab2BMP[index] = B;
			Lab2BMP[index+1] = G;
			Lab2BMP[index+2] = R;
		}
	}
	fwrite(Lab2BMP, line_byte*height, 1, Lab2BMP_fp);
	fclose(Lab2BMP_fp);  
    free(Lab2BMP);
    
	return 0;
}
```
### 4.结果实例
![](https://img-blog.csdn.net/20170817233224094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170817233448643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHowNDk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
左侧图像是原始图像，右侧图像经过RGB->XYZ->LAB->XYZ->RGB的转换结果图。
参考资料：
[http://www.cnblogs.com/Imageshop/archive/2013/02/02/2889897.html](http://www.cnblogs.com/Imageshop/archive/2013/02/02/2889897.html)
[http://blog.csdn.net/grafx/article/details/59482320](http://blog.csdn.net/grafx/article/details/59482320)
[http://blog.csdn.net/real_myth/article/details/50828171](http://blog.csdn.net/real_myth/article/details/50828171)
[http://blog.sina.com.cn/s/blog_46e3af5b0100u7wt.html](http://blog.sina.com.cn/s/blog_46e3af5b0100u7wt.html)
