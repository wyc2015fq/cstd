# Color Transfer Between Image - Popeye_the_Sailor - CSDN博客
2017年08月25日 20:58:59[_Sailor_](https://me.csdn.net/lz0499)阅读数：764
## Color Transfer Between Image
      ColorTransfer Between Images算法中，给定两张图片source_img和target_img。该算法可以把source_img中的颜色迁移到target_img中。
算法主要思路为：
- 给定输入图像source_img和target_img
- 把source_img和target_img转换到Lab颜色空间，转换后图像为source_lab和target_lab
- 分别计算source_Lab和target_lab三个通道的均值和标准差
- 根据论文中的公式，分别计算三通道的变换值：
![](https://img-blog.csdn.net/20170825202636895)
         5. 把Lab空间的结果图像再转换到RGB空间，并显示
        Lab转RGB空间和RGB转LAB空间，可以参考之前写的博文：[RGB与Lab颜色空间互相转换](http://blog.csdn.net/lz0499/article/details/77345166)
算法关键代码实现：
```cpp
RGB2Lab(source_img,source_lab);
	RGB2Lab(target_img,target_lab);
	
	source_Mean = ConputerMean(source_lab,source_height,source_width,source_line_byte);
	target_Mean = ConputerMean(target_lab,target_height,target_width,target_line_byte);
	source_Labvar = ConputerVar(source_height,source_width,source_Mean);
	target_Labvar = ConputerVar(target_height,target_width,target_Mean);
	Color_Transform_Func(target_lab,source_Labvar,target_Labvar,source_Mean,target_Mean,target_height,target_width,target_line_byte);
	bmp =  Lab2RGB(target_img,target_lab);
	fwrite(bmp, target_line_byte*target_height, 1, Color_Transform_BMP_fp);
        fclose(Color_Transform_BMP_fp);  
        free(Color_Transform_BMP);
	free(bmp);
    return 0;
```
计算均值和方差关键代码：
```cpp
LABMEAN ConputerMean(float *lab_img,DWORD height,DWORD width,T_U32 line_byte)
{
	double LMean,aMean,bMean,SumL,SumA,SumB,SumL2,SumA2,SumB2,TmpL,TmpA,TmpB,size,l,a,b;
	DWORD i,j,index;
	LABMEAN labMean = {0};
	LMean = 0.0;
	aMean =0;
	bMean=0.0;
	SumL = 0;
	SumA = 0;
	SumB = 0;
	SumL2 = 0.0;
	SumA2 = 0.0;
	SumB2 = 0.0;
	TmpL = 0;
	TmpA = 0.0;
	TmpB = 0;
	size = width*height;
	for(i = 0; i < height;i++)
	{
		for(j = 0;j < width;j++)
		{
			index = i*line_byte+3*j;
			TmpL = lab_img[index+0];
			SumL  += TmpL;
			SumL2 += TmpL*TmpL;
			TmpA = lab_img[index+1];
			SumA  += TmpA;
			SumA2 += TmpA*TmpA;
			TmpB = lab_img[index+2];
			SumB  += TmpB;
			SumB2 += TmpB*TmpB;
		}
	}
	LMean = SumL/(size);
	aMean = SumA/(size);
	bMean = SumB/(size);
	labMean.MeanL = LMean;
	labMean.MeanA = aMean;
	labMean.MeanB = bMean;
	labMean.SumL = SumL;
	labMean.SumA = SumA;
	labMean.SumB = SumB;
	labMean.SumL2 = SumL2;
	labMean.SumA2 = SumA2;
	labMean.SumB2 = SumB2;
	
	return labMean;
}
LABVAR ConputerVar(DWORD height,DWORD width,LABMEAN labMean)
{
	double LVar,aVar,bVar,size;
	LABVAR labvar = {0};
    LVar = 0.0;
	aVar = 0.;
	bVar = 0.0;
	size = width*height;
	LVar = (labMean.SumL2-(labMean.SumL*labMean.SumL)/size)/size;
	aVar = (labMean.SumA2-(labMean.SumA*labMean.SumA)/size)/size;
	bVar = (labMean.SumB2-(labMean.SumB*labMean.SumB)/size)/size;
	LVar = sqrt(LVar);
	aVar = sqrt(aVar);
	bVar = sqrt(bVar);
	labvar.VarL = LVar;
	labvar.VarA = aVar;
	labvar.VarB = bVar;
	return labvar;
}
```
颜色转换关键代码实现：
```cpp
void Color_Transform_Func(float *lab_img,LABVAR source_var,LABVAR target_var,LABMEAN source_mean,LABMEAN target_mean,DWORD height,DWORD width,T_U32 line_byte)
{
	int i,j,index;
	for(i = 0; i < height;i++)
	{
		for(j = 0;j < width;j++)
		{
			 index = i*line_byte+3*j;
			 lab_img[index+0] -= target_mean.MeanL;
			 lab_img[index+1] -= target_mean.MeanA;
			 lab_img[index+2] -= target_mean.MeanB;
			 lab_img[index+0] = (target_var.VarL/source_var.VarL)*lab_img[index+0];
			 lab_img[index+1] = (target_var.VarA/source_var.VarA)*lab_img[index+1];
			 lab_img[index+2] = (target_var.VarB/source_var.VarB)*lab_img[index+2];
			 lab_img[index+0] += source_mean.MeanL;
			 lab_img[index+1] += source_mean.MeanA;
			 lab_img[index+2] += source_mean.MeanB;
		}
	}
}
```
图像结果：
![](https://img-blog.csdn.net/20170825205523434)![](https://img-blog.csdn.net/20170825205504436)
     左图为source_img，又图为target_img
![](https://img-blog.csdn.net/20170825205622893)
结果图像
