# 图像特效之随机滤波（Randomblue） - Popeye_the_Sailor - CSDN博客
2017年09月10日 21:26:40[_Sailor_](https://me.csdn.net/lz0499)阅读数：155
## 图像特效之随机滤波（Randomblue）
RandomBlue----图像中随机滤波，即在图像范围内，随机获取某一范围，在该范围内以一定半径进行平均滤波处理。
### 关键代码：
```cpp
int Randblur(IMAGE_TYPE *bmp_img,int m_iAmount,int m_iRadius,int m_iSize)
{
	DWORD width,height,dst_index;
	WORD  biBitCount;
	T_U8 *bmp_data,*Result_img;
	T_U32 line_byte;
	DWORD i,j,k,l;
	DWORD x,y;		// beginning coord (random)
	DWORD ex,ey; // end coord (from beginning)
	int rad;	// blur radius (bluriness)
	int RAD; 	// total num pix processed per block 
	int rad2; // half of the radius
	int numBlurs,BytePerPixel = 3;
	float R,G,B,Tmp;
	
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	FILE *Random_Blue_fp = fopen("Random_Blue_BMP.bmp","wb");
	
	if(NULL == Random_Blue_fp)
	{
		printf("Can't open Random_Blue_BMP.bmp\n");
		return -1;
	}
	
	memset(&bf, 0, sizeof(bf));
	memset(&bi, 0, sizeof(bi));
	memcpy(&bf,bmp_img,14);
	memcpy(&bi,&bmp_img[14],40);
	height = bi.biHeight;
	width  = bi.biWidth;
	biBitCount = bi.biBitCount;//每一个像素由24 bits表示，即RGB分量每一个分量用8 bits表示
	line_byte =(width*3+3) & ~0x3;
	fwrite(&bf,sizeof(BITMAPFILEHEADER),1,Random_Blue_fp);
	fwrite(&bi,sizeof(BITMAPINFOHEADER),1,Random_Blue_fp);
	rad = m_iRadius;
	RAD = rad*rad;
	rad2 = rad/2;
	Result_img = (T_U8*)malloc(height*line_byte);
	if(Result_img == NULL)
	{
		printf("Can't malloc for result ingage.\n");
		return -1;
	}
	bmp_data = bmp_img + BMPHEADSIZE;
	memset(Result_img,0,height*line_byte);
	memcpy(Result_img,bmp_data,height*line_byte);
	
	for(numBlurs= 0;numBlurs <  m_iAmount;numBlurs++)
	{
		do
		{
			y = (int)(rand() * ((double)height / RAND_MAX));
			x = (int)(rand() * ((double)width / RAND_MAX));
			ex = x + m_iSize - 1;
			ey = y + m_iSize - 1;
		} while (ex >= width || ey >= height); 
		for (i = y + rad2; i < ey - rad2; i++)
		{
			
			for (j = x + rad2; j < ex - rad2; j++)
			{
				dst_index = i*line_byte+3*j;
				R = G = B = 0;
				for (k = 0; k < rad; k++)
				{
					for (l = 0; l < rad; l++)
					{
						R += (float)bmp_data[(i+k-rad2)*line_byte+(j+l-rad2)*3+2];
						G += (float)bmp_data[(i+k-rad2)*line_byte+(j+l-rad2)*3+1];
						B += (float)bmp_data[(i+k-rad2)*line_byte+(j+l-rad2)*3];
					}
				}
				
				Result_img[dst_index] = CLIP255((int)(B/RAD));
				Result_img[dst_index+1] = CLIP255((int)(G/RAD));
				Result_img[dst_index+2] = CLIP255((int)(R/RAD));
				
			}
		}
	}
	
	fwrite(Result_img, line_byte*height*sizeof(T_U8), 1, Random_Blue_fp);
    fclose(Random_Blue_fp);  
    free(Result_img);
	return 0;
}
```
### 实际结果：
![](https://img-blog.csdn.net/20170910211305214)![](https://img-blog.csdn.net/20170910212407061)
左侧为原始图，右侧围随机滤波之后的图像 
