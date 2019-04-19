# Kuwahara滤波 - Popeye_the_Sailor - CSDN博客
2017年01月21日 18:37:43[_Sailor_](https://me.csdn.net/lz0499)阅读数：806
## 1.Kuwahara_type 滤波原理
         Kuwahara类型的滤波思想是由Kuwahara等人提出来的。其基本原理是：计算图像模板中邻域内的均值和方差，选择图像灰度值较为均匀的区域的均值替代模板中心像素灰度值。而图像模板中较为均匀的区域所对应的方差是最小的。为了获取图像模板中较为均匀区域的均值，滤波区域R被划分为K个重叠的子区域R1，R2,…, Rk。位于图像中位置为（u，v）处的每一个像素，其所对应的每一个模板子区域的均值和方差都将被计算：
![](https://img-blog.csdn.net/20170121194243107)
![](https://img-blog.csdn.net/20170121194324511)
![](https://img-blog.csdn.net/20170121194343562)
     对于k=1，……，K，有如下表达式成立：
![](https://img-blog.csdn.net/20170121194509548)
![](https://img-blog.csdn.net/20170121194528220)
    对应方差最小的子区域的均值替代模板中心像素的灰度值。即：
![](https://img-blog.csdn.net/20170121194534481)
   其伪代码实现如下图所示：
![](https://img-blog.csdn.net/20170121194828098)
         Kuwahara等人提出的3*3（r=1）模板子区域如Fig1所示。3*3的滤波模板中，其子区域由重叠与中心像素的(r+1)*(r+1)的正方形区域构成。一般而言，滤波模板的大小应该为（2*r+1）*（2*r+1）。
![](https://img-blog.csdn.net/20170121194955381)
Fig1 .Subregionstructures for Kuwahara filters
![](https://img-blog.csdn.net/20170121195228245)
Fig2. Subregion structures for *Tomita-Tsuji’s *filters
![](https://img-blog.csdn.net/20170121195253268)
Fig3. Subregion structures for Nagao-Matsuyama’s filters
## 2.Kuwahara_type 滤波代码实现
```
typedef struct Area_Pix
{
	T_U8 Model_Pix1;
	T_U8 Model_Pix2;
	T_U8 Model_Pix3;
	T_U8 Model_Pix4;
	T_U8 Model_Pix5;
	T_U8 Model_Pix6;
	T_U8 Model_Pix7;
	T_U8 Model_Pix8;
	T_U8 Model_Pix9;
}AREA_PIX;
AREA_PIX Arae[9] = {0};
int CalaMeanMin(AREA_PIX *area)
{
	int i,flag = 0;
	double MeanMin = 9999,Mean7 = 0,Mean8 = 0;
	double VarianceMin = 99999,Variance7=0,Variance8 = 0;
	double Pow_0,Pow_1,Pow_2,Pow_3,Pow_4,Pow_5,Pow_6,Pow_7=0,Pow_8=0;
	for (i= 0;i< 8;i++)
	{
		Mean7 =(area[i].Model_Pix1+area[i].Model_Pix2+area[i].Model_Pix3\
			  +area[i].Model_Pix4+area[i].Model_Pix5+area[i].Model_Pix6+area[i].Model_Pix7)/7;
		Pow_0 = pow((area[i].Model_Pix1-Mean7),2);
		Pow_1 = pow((area[i].Model_Pix2-Mean7),2);
		Pow_2 = pow((area[i].Model_Pix3-Mean7),2);
		Pow_3 = pow((area[i].Model_Pix4-Mean7),2);
		Pow_4 = pow((area[i].Model_Pix5-Mean7),2);
		Pow_5 = pow((area[i].Model_Pix6-Mean7),2);
		Pow_6 = pow((area[i].Model_Pix7-Mean7),2);
		
		Variance7 = (Pow_0+Pow_1+Pow_2+Pow_3+Pow_4+Pow_5+Pow_6)/7;
		if(VarianceMin > Variance7)
		{
				VarianceMin = Variance7;
				MeanMin = Mean7;
		}
	}
	Mean8 = (area[8].Model_Pix1+area[8].Model_Pix2+area[8].Model_Pix3+area[8].Model_Pix4+area[8].Model_Pix5\
		    +area[8].Model_Pix6+area[8].Model_Pix7+area[8].Model_Pix8+area[8].Model_Pix9)/9;
	Pow_0 = pow((area[8].Model_Pix1-Mean8),2);
	Pow_1 = pow((area[8].Model_Pix2-Mean8),2);
	Pow_2 = pow((area[8].Model_Pix3-Mean8),2);
	Pow_3 = pow((area[8].Model_Pix4-Mean8),2);
	Pow_4 = pow((area[8].Model_Pix5-Mean8),2);
	Pow_5 = pow((area[8].Model_Pix6-Mean8),2);
	Pow_6 = pow((area[8].Model_Pix7-Mean8),2);
	Pow_6 = pow((area[8].Model_Pix8-Mean8),2);
	Pow_6 = pow((area[8].Model_Pix9-Mean8),2);
	Variance8 = (Pow_0+Pow_1+Pow_2+Pow_3+Pow_4+Pow_5+Pow_6+Pow_7+Pow_8)/9;
	if(VarianceMin > Variance8)
	{
			VarianceMin = Variance8;
			MeanMin = Mean8;
	}
	if (MeanMin > 255)
		MeanMin = 255;
	if (MeanMin < 0)
		MeanMin = 0;
	return (T_U8)MeanMin;
}
int AddArea(AREA_PIX *area,T_U8 p1,T_U8 p2,T_U8 p3,T_U8 p4,T_U8 p5,T_U8 p6,T_U8 p7,T_U8 p8,T_U8 p9)
{
	area->Model_Pix1 = p1;
	area->Model_Pix2 = p2;
	area->Model_Pix3 = p3;
	area->Model_Pix4 = p4;
	area->Model_Pix5 = p5;
	area->Model_Pix6 = p6;
	area->Model_Pix7 = p7;
	area->Model_Pix8 = p8;
	area->Model_Pix9 = p9;
	return 0;
}
T_U8 KuwaharaType_Filter_TempCala(DWORD width,DWORD height,T_U8* Graybmp_img,T_U8* Temp_dst,DWORD x,DWORD y,T_U32 line_byte)
{
	
	T_U32 index = x*line_byte+y*3;
	T_U8  p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24;
	p0 =  Graybmp_img[index+0-2*line_byte-6];
	p1 =  Graybmp_img[index+0-2*line_byte-3];
	p2 =  Graybmp_img[index+0-2*line_byte];
	p3 =  Graybmp_img[index+0-2*line_byte+3];
	p4 =  Graybmp_img[index+0-2*line_byte+6];
	p5 =  Graybmp_img[index+0-line_byte-6];
	p6 =  Graybmp_img[index+0-line_byte-3];
	p7 =  Graybmp_img[index+0-line_byte];
	p8 =  Graybmp_img[index+0-line_byte+3];
	p9 =  Graybmp_img[index+0-line_byte+6];
	p10 = Graybmp_img[index+0-6];
	p11 = Graybmp_img[index+0-3];
	p12 = Graybmp_img[index+0];
	p13 = Graybmp_img[index+0+3];
	p14 = Graybmp_img[index+0+6];
	p15 = Graybmp_img[index+0+line_byte-6];
	p16 = Graybmp_img[index+0+line_byte-3];
	p17 = Graybmp_img[index+0+line_byte];
	p18 = Graybmp_img[index+0+line_byte+3];
	p19 = Graybmp_img[index+0+line_byte+6];
	p20 = Graybmp_img[index+0+2*line_byte-6];
	p21 = Graybmp_img[index+0+2*line_byte-3];
	p22 = Graybmp_img[index+0+2*line_byte];
	p23 = Graybmp_img[index+0+2*line_byte+3];
	p24 = Graybmp_img[index+0+2*line_byte+6];
	AddArea(&Arae[0],p1,p2,p3,p6,p7,p8,p12,0,0);
	AddArea(&Arae[1],p3,p4,p7,p8,p9,p12,p13,0,0);
	AddArea(&Arae[2],p8,p9,p12,p13,p14,p18,p19,0,0);
	AddArea(&Arae[3],p12,p13,p17,p18,p19,p23,p24,0,0);
	AddArea(&Arae[4],p12,p16,p17,p18,p21,p22,p23,0,0);
	AddArea(&Arae[5],p11,p12,p15,p16,p17,p20,p21,0,0);
	AddArea(&Arae[6],p5,p6,p11,p12,p10,p15,p16,0,0);
	AddArea(&Arae[7],p0,p1,p5,p6,p7,p11,p12,0,0);
	AddArea(&Arae[8],p6,p7,p8,p11,p12,p13,p16,p17,p18);
	
	Temp_dst[index+0] = CalaMeanMin(Arae);
	Temp_dst[index+1] = Temp_dst[index+0];
	Temp_dst[index+2] = Temp_dst[index+0];
	return 0;
}
int KuwaharaType_Filter(IMAGE_TYPE *Graybmp_img)
{
	DWORD width,height,x,y,bfsize;
	WORD  biBitCount;
	T_U8 *dst,*bmp,*Temp,*Tempdst;
	T_U32 line_byte,index;
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	FILE *KuwaharaType_FilterBMP_fp = fopen("KuwaharaType_Filter.bmp","wb");
	if(NULL == KuwaharaType_FilterBMP_fp)
	{
		printf("Can't open KuwaharaType_Filter.bmp\n");
		return -1;
	}
	memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));
	bmp = Graybmp_img;
	memcpy(&bf,bmp,14);
	memcpy(&bi,&bmp[14],40);
	height = bi.biHeight;
	width  = bi.biWidth;
	bfsize = bf.bfSize;
	biBitCount = bi.biBitCount;
	line_byte = WIDTHBYTES(width*bi.biBitCount);
	dst = bmp+54;
	Temp = (T_U8 *)malloc(bfsize);
	memset(Temp,0,bfsize);
	memcpy(Temp,bmp,bfsize);
	Tempdst = Temp+54;
	for (x = SIZE/2;x < height-SIZE/2;x++)
	{
		for (y = SIZE/2;y < width-SIZE/2;y++)
		{
			KuwaharaType_Filter_TempCala(width,height,dst,Tempdst,x,y,line_byte);
		}
	}
	fwrite(Temp,bfsize,1,KuwaharaType_FilterBMP_fp);
	fclose(KuwaharaType_FilterBMP_fp);
	KuwaharaType_FilterBMP_fp = NULL;
	free(Temp);
	return 0;
}
```
滤波效果如下所示：
![](https://img-blog.csdn.net/20170121195650048)![](https://img-blog.csdn.net/20170121195706974)

