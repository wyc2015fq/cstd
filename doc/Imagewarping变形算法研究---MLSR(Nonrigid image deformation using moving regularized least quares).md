# Imagewarping变形算法研究---MLSR(Nonrigid image deformation using moving regularized least quares)

2018年03月19日 12:05:17

 

Trent1985

 

阅读数：1818

 

标签： [MLSR变形算法实现](http://so.csdn.net/so/search/s.do?q=MLSR%E5%8F%98%E5%BD%A2%E7%AE%97%E6%B3%95%E5%AE%9E%E7%8E%B0&t=blog) 更多

个人分类： [照片美妆](https://blog.csdn.net/Trent1985/article/category/6887373)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/79566052

# **Nonrigid image deformation using moving regularized least quares**

MLS算法是基于最小二乘法的移动网格变形算法，该算法依赖于网格大小的选择，而本文介绍的这篇论文，就是对网格变形的改进，使他不依赖于网格的选择。

为了方便更多的人看懂，本文主要讲算法实现的流程，不侧重于数学公式的推导之类。所以这里直接给出算法实现过程：

![img](https://img-blog.csdn.net/20180315115957997)

其中：

![img](https://img-blog.csdn.net/20180315120224799)

![img](https://img-blog.csdn.net/20180315120720986)

![img](https://img-blog.csdn.net/20180315120843256)

上面的算法流程经作者实践，比较耗时，于是作者做了优化，对公式8做了改进，如下：

![img](https://img-blog.csdn.net/20180315121217621)

论文效果图如下：![img](https://img-blog.csdn.net/20180319103754766)

本人代码实现这个算法如下：

```cpp
// 对矩阵求逆，结果保存在a中  
int f_GaussInverseMatrix(double A[], double B[], int nColumns)  
{  
	memcpy(B, A, sizeof(double) * nColumns * nColumns);
    int *is,*js,i,j,k,l,u,v;  
    double d,p;  
    is = new int[nColumns];  
    js = new int[nColumns];  
    for (k=0; k<=nColumns-1; k++)  
    {  
        d=0.0;  
        for (i=k; i<=nColumns-1; i++)  
        for (j=k; j<=nColumns-1; j++)  
        {  
            l=i*nColumns+j; p=fabs(B[l]);  
            if (p>d) { d=p; is[k]=i; js[k]=j;}  
        }  
        if (d+1.0==1.0)  
        {  
            free(is); free(js); printf("err**not inv\n");  
            return(1);  
        }  
        if (is[k]!=k)  
            for (j=0; j<=nColumns-1; j++)  
            {  
                u=k*nColumns+j; v=is[k]*nColumns+j;  
                p=B[u]; B[u]=B[v]; B[v]=p;  
            }  
        if (js[k]!=k)  
            for (i=0; i<=nColumns-1; i++)  
            {  
                u=i*nColumns+k; v=i*nColumns+js[k];  
                p=B[u]; B[u]=B[v]; B[v]=p;  
            }  
        l=k*nColumns+k;  
        B[l]=1.0f/B[l];  
        for (j=0; j<=nColumns-1; j++)  
            if (j!=k)  
            { u=k*nColumns+j; B[u]=B[u]*B[l];}  
        for (i=0; i<=nColumns-1; i++)  
            if (i!=k)  
                for (j=0; j<=nColumns-1; j++)  
        if (j!=k)  
        {  
            u=i*nColumns+j;  
            B[u] -= B[i*nColumns+k]*B[k*nColumns+j];  
        }  
        for (i=0; i<=nColumns-1; i++)  
            if (i!=k)  
            {  
                u=i*nColumns+k;  
                B[u] = -B[u]*B[l];  
            }  
    }  
    for (k=nColumns-1; k>=0; k--)  
    {  
        if (js[k]!=k)  
        for (j=0; j<=nColumns-1; j++)  
        {  
            u=k*nColumns+j; v=js[k]*nColumns+j;  
        p=B[u]; B[u]=B[v]; B[v]=p;  
        }  
        if (is[k]!=k)  
        for (i=0; i<=nColumns-1; i++)  
        {   
            u=i*nColumns+k; v=i*nColumns+is[k];  
            p=B[u]; B[u]=B[v]; B[v]=p;  
        }  
    }  
    free(is); 
	free(js);  
    return 0;  
}  
 
 
// 求矩阵的乘积 C = A.*B  
void f_matrixMul(double a[], double b[], int m, int n, int k, double c[])  
{  
    int i, j, l, u;  
    for (i = 0; i < m; i++)  
	{
        for (j = 0; j < k; j++)  
        {  
            u = i * k + j; 
			c[u] = 0.0;  
            for (l = 0; l < n; l++)  
            {
				c[u] += a[i * n + l] * b[l * k + j];
			}
        }  
	}
}

#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)
//------------------------------------------------------------------    
//Function: MRLS 
//Params:   
//return: 0 or false    
//Reference: Nonrigid image deformation using moving regularized least squares. Jiayi Ma,2013
//-------------------------------------------------------------------    
int f_MLSR(unsigned char *srcData, int width, int height, int stride, int srcPoint[], int dstPoint[], int pointNum)
{
	int ret = 0;
	unsigned char* tempData = (unsigned char*)malloc(sizeof(unsigned char)* height * stride);
	memcpy(tempData, srcData, sizeof(unsigned char) * height * stride);
	//Process
	double *srcPx = (double*)malloc(sizeof(double) * pointNum);
	double *srcPy = (double*)malloc(sizeof(double) * pointNum);
	double *dstPx = (double*)malloc(sizeof(double) * pointNum);
	double *dstPy = (double*)malloc(sizeof(double) * pointNum);
	double* GammaIJ= (double*)malloc(sizeof(double) * pointNum * pointNum); 
	double* GammaIJT= (double*)malloc(sizeof(double) * pointNum * pointNum); 
	double* GammaP = (double*)malloc(sizeof(double) * pointNum); 
	double* S      = (double*)malloc(sizeof(double) * pointNum); 
	double* W      = (double*)malloc(sizeof(double) * pointNum * pointNum); 
	double* WInv   = (double*)malloc(sizeof(double) * pointNum * pointNum); 
	double* tempIJ = (double*)malloc(sizeof(double) * pointNum * pointNum); 
	double belta = 2.0;
	double alpha = 2.0;
	double lambda = 1.0;
	double fbelta = 1.0 / (belta * belta);
	for(int i = 0; i < pointNum; i++)
	{
		srcPx[i] = (double)srcPoint[2 * i] / width;
		srcPy[i] = (double)srcPoint[2 * i + 1] / height;
		dstPx[i] = (double)dstPoint[2 * i] / width;
		dstPy[i] = (double)dstPoint[2 * i + 1] / height;
	}
	int w = pointNum;
	for(int j = 0; j < pointNum; j++)
	{
	    for(int i = 0; i < pointNum; i++)
	    {
			GammaIJ[i + j * w] = exp(-((double)(srcPx[i] - srcPx[j]) * (srcPx[i] - srcPx[j]) + (srcPy[i] - srcPy[j]) * (srcPy[i] - srcPy[j])) * fbelta);
	    }
	}
	unsigned char* pSrc = srcData;
	int aa, bb, cc, dd, pos, pos1;
    double r1, r2, r3;
	unsigned char *pSrcL1;
    unsigned char *pSrcL2;
	unsigned char* p = tempData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			double px = (double)i / width, py = (double)j / height;
			for(int n = 0; n < pointNum; n++)
	        {
	            for(int m = 0; m < pointNum; m++)
	            {
					if(n == m)
					    W[m + n * w] = pow(((double)(px - srcPx[m]) * (px - srcPx[m]) + (py - srcPy[m]) * (py - srcPy[m])), -alpha);
					else
						W[m + n * w] = 0;
		        }
			}
			//compute inverse matrix of W
			f_GaussInverseMatrix(W, WInv, pointNum);
			//compute Gamma + lambda * WInv
			for(int n = 0; n < pointNum; n++)
	        {
	            for(int m = 0; m < pointNum; m++)
	            {
					//W is a diagonal matrix with the i-th entry Wi(p),other's 0
					GammaIJT[m + n * w] = GammaIJ[m + n * w] + lambda * WInv[m + n * w];
		        }
			}
			//compute inverse matrix of (Gamma + lambda * WInv)
			f_GaussInverseMatrix(GammaIJT, tempIJ, pointNum);
			//compute GammaP   exp(-(p-xi) * (p - xi) / (belta * belta))
			for(int m = 0; m < pointNum; m++)
				GammaP[m] = exp((-((double)(px - srcPx[m]) * (px - srcPx[m]) + (py - srcPy[m]) * (py - srcPy[m])) * fbelta));
			//compute S
			f_matrixMul(GammaP, tempIJ, 1, pointNum, pointNum, S);
			//compute fp   T + SY
			double sumx = 0, sumy = 0;
			for(int m = 0; m < pointNum; m++)
			{
				sumx += S[m] * srcPx[m];
				sumy += S[m] * srcPy[m];
			}
			px = px - sumx;
			py = py - sumy;
			sumx = 0, sumy = 0;
			for(int m = 0; m < pointNum; m++)
			{
				sumx += S[m] * dstPx[m];
				sumy += S[m] * dstPy[m];
			}
			px = px + sumx;
			py = py + sumy;
			double x_in = CLIP3(px * width, 0, width - 1);
			double y_in = CLIP3(py * height, 0, height - 1);
            //please change interpolation to get better effects.
			int xx = (int)x_in;
            int yy = (int)y_in;     
			pSrcL1 = p + yy * stride;
            pSrcL2 = pSrcL1 + stride;
			pos = (xx << 2);
            aa = pSrcL1[pos];
            bb = pSrcL1[pos + 4];
            cc = pSrcL2[pos];
            dd = pSrcL2[pos + 4];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[0]=(unsigned char)(CLIP3(r3, 0, 255));//B分量
            aa = pSrcL1[pos + 1];
            bb = pSrcL1[pos + 4 +1];
            cc = pSrcL2[pos + 1];
            dd = pSrcL2[pos + 4 + 1];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[1]=(unsigned char)(CLIP3(r3, 0, 255));//G分量
            aa = pSrcL1[pos + 2];
            bb = pSrcL1[pos + 4 + 2];
            cc = pSrcL2[pos + 2];
            dd = pSrcL2[pos + 4 + 2];
            r1 = aa + (bb - aa) * (x_in - xx);
            r2 = cc + (dd - cc) * (x_in - xx);
            r3 = r1 + (r2 - r1) * (y_in - yy);
            pSrc[2]=(unsigned char)(CLIP3(r3, 0, 255));//R分量
            aa = pSrcL1[pos + 3];
            bb = pSrcL1[pos + 4 + 3];
            cc = pSrcL2[pos + 3];
            dd = pSrcL2[pos + 4 + 3];
            r1=aa + (bb - aa) * (x_in - xx);
            r2=cc + (dd - cc) * (x_in - xx);
            r3=r1 + (r2 - r1) * (y_in - yy);
            pSrc[3]=(unsigned char)(CLIP3(r3, 0, 255));//A分量
			pSrc += 4;
		}
	}
	free(tempData);
	free(srcPx);
	free(srcPy);
	free(dstPx);
	free(dstPy);
	free(GammaIJ);
	free(GammaP);
	free(S     );
	free(W     );
	free(WInv  );
	free(tempIJ);
	free(GammaIJT);
	return ret;
};

```

具体效果，本人与上一篇博文保持一致仍然以瘦脸效果为例，原图和效果图如下：

![img](https://img-blog.csdn.net/20180319104144481)

原图(蓝色点为原始点，红色点为拖拽点)

![img](https://img-blog.csdn.net/201803191042271)

瘦脸效果图

图片来自网络，如有侵权敬请告知

该算法优缺点：

算法变形效果非常好，但是在10个点位个数以上，速度会随点位增加而变慢，对于全脸100+个特征点变形而言，速度太慢；算法适用于10个点位左右的人脸五官变形，比如瘦脸，胖脸，小脸，嘴巴变形等等，当然也可以做其他变形功能需求；

本人QQ1358009172