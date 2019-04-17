# 图像形状特征（七）--Zernike矩 - 工作笔记 - CSDN博客





2016年12月12日 09:11:00[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：9512
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：http://blog.csdn.net/wrj19860202/article/details/6334275



Zernike在1934年引入了一组定义在单位圆![](http://hi.csdn.net/attachment/201104/19/0_1303212176MEUM.gif) 上的复值函数集{![](http://hi.csdn.net/attachment/201104/19/0_1303212176qJnN.gif) }，{![](http://hi.csdn.net/attachment/201104/19/0_1303212180i63a.gif) }具有完备性和正交性，使得它可以表示定义在单位圆盘内的任何平方可积函数。其定义为：

![](http://hi.csdn.net/attachment/201104/19/0_1303212181Ikn3.gif)

![](http://hi.csdn.net/attachment/201104/19/0_13032121819tHh.gif) 表示原点到点![](http://hi.csdn.net/attachment/201104/19/0_1303212181Ss1x.gif) 的矢量长度；![](http://hi.csdn.net/attachment/201104/19/0_1303212181Q024.gif) 表示矢量![](http://hi.csdn.net/attachment/201104/19/0_1303212181wawz.gif) 与![](http://hi.csdn.net/attachment/201104/19/0_13032121825rVE.gif) 轴逆时针方向的夹角。

![](http://hi.csdn.net/attachment/201104/19/0_1303212182gal6.gif) 是实值径向多项式：

![](http://hi.csdn.net/attachment/201104/19/0_1303212182CDDl.gif)

称为Zernike多项式。

Zernike多项式满足正交性：

![](http://hi.csdn.net/attachment/201104/19/0_1303212182g6Xx.gif)

其中

![](http://hi.csdn.net/attachment/201104/19/0_1303212182HLJ6.gif) 为克罗内克符号，![](http://hi.csdn.net/attachment/201104/19/0_1303212183MkZo.gif)![](http://hi.csdn.net/attachment/201104/19/0_1303212183u72x.gif)

![](http://hi.csdn.net/attachment/201104/19/0_1303212183ZQgo.gif) 是![](http://hi.csdn.net/attachment/201104/19/0_1303212183rfRA.gif) 的共轭多项式。

由于Zernike多项式的正交完备性，所以在单位圆内的任何图像![](http://hi.csdn.net/attachment/201104/19/0_13032121838SKy.gif) 都可以唯一的用下面式子来展开：

![](http://hi.csdn.net/attachment/201104/19/0_130321218433GG.gif)

式子中![](http://hi.csdn.net/attachment/201104/19/0_1303212184ETxH.gif) 就是Zernike矩，其定义为：

![](http://hi.csdn.net/attachment/201104/19/0_1303212184G23M.gif)

注意式子中![](http://hi.csdn.net/attachment/201104/19/0_1303212184gpvZ.gif) 和![](http://hi.csdn.net/attachment/201104/19/0_1303212184fnNk.gif) 采用的是不同的坐标系（![](http://hi.csdn.net/attachment/201104/19/0_1303212185cX3f.gif) 采用直角坐标，而![](http://hi.csdn.net/attachment/201104/19/0_1303212185u17M.gif) 采用的极坐标系，在计算的时候要进行坐标转换）

对于离散的数字图像，可将积分形式改为累加形式：

![](http://hi.csdn.net/attachment/201104/19/0_13032121856x5r.gif)![](http://hi.csdn.net/attachment/201104/19/0_1303212185AoCr.gif)

我们在计算一副图像的Zernike矩时，必须将图像的中心移到坐标的原点，将图像的像素点映射到单位圆内，由于Zernike矩具有旋转不变性，我们可以将![](http://hi.csdn.net/attachment/201104/19/0_13032121859Muz.gif) 作为图像的不变特征，其中图像的低频特征有p值小的![](http://hi.csdn.net/attachment/201104/19/0_1303212186c13v.gif) 提取，高频特征由p值高的![](http://hi.csdn.net/attachment/201104/19/0_130321218645Gm.gif) 提取。从上面可以看出，Zernike矩可以构造任意高阶矩。

由于Zernike矩只具有旋转不变性，不具有平移和尺度不变性，所以要提前对图像进行归一化，我们采用标准矩的方法来归一化一副图像，标准矩定义为：

![](http://hi.csdn.net/attachment/201104/19/0_1303212186NX9l.gif) ，

由标准矩我们可以得到图像的"重心"，

![](http://hi.csdn.net/attachment/201104/19/0_1303212186C40C.gif)

![](http://hi.csdn.net/attachment/201104/19/0_1303212186PfPn.gif)

我们将图像的"重心"移动到单位圆的圆心（即坐标的原点），便解决了平移问题。

我们知道![](http://hi.csdn.net/attachment/201104/19/0_1303212187bteB.gif) 表征了图像的"面积"，归一图像的尺度无非就是把他们的大小变为一致的，（这里的大小指的是图像目标物的大小，不是整幅图像的大小，"面积"也是目标物的"面积"）。

所以，对图像进行变换![](http://hi.csdn.net/attachment/201104/19/0_1303212187lf2L.gif) 就可以达到图像尺寸一致的目的。

综合上面结果，对图像进行![](http://hi.csdn.net/attachment/201104/19/0_1303212187BCKk.gif) 变换，最终图像![](http://hi.csdn.net/attachment/201104/19/0_1303212187X6X8.gif) 的Zernike矩就是平移，尺寸和旋转不变的。

Zernike 不变矩相比 Hu 不变矩识别效果会好一些，因为他描述了图像更多的细节内容，特别是高阶矩，但是由于 Zernike 不变矩计算时间比较长，所以出现了很多快速的[算法](http://lib.csdn.net/base/datastructure)，大家可以 google 一下。

用 Zernike 不变矩来识别手势轮廓，识别率大约在 40%~50% 之间，跟 Hu 不变矩一样， Zernike 不变矩一般用来描述目标物形状占优势的图像，不适合用来描述纹理丰富的图像，对于纹理图像，识别率一般在 20%~30% 左右，很不占优势。



```cpp
C++代码如下：

/*计算一行的像素个数
imwidth:图像宽度
deep:图像深度（8位灰度图为1，24位彩色图为3）
*/
#define  bpl(imwidth, deep) ((imwidth*deep*8+31)/32*4)
/*获取像素值
psrcBmp:图像数据指针
nsrcBmpWidth:图像宽度，以像素为单位
x,y:像素点
deep:图像的位数深度，（1表示8位的灰度图，3表示24位的RGB位图）
*/
COLORREF J_getpixel( const BYTE *psrcBmp, const int nsrcBmpWidth, const int x, const int y, int deep = 3)
{
	if (deep == 3)
	{
		return RGB(*(psrcBmp + x*3 + y*bpl(nsrcBmpWidth, deep) + 2 ) , 
			*(psrcBmp + x*3 + y*bpl(nsrcBmpWidth, deep) + 1 ) , 
			*(psrcBmp + x*3 + y*bpl(nsrcBmpWidth, deep) +0 ));
	}
	else if (deep == 1)
	{
		return *(psrcBmp + x + y*bpl(nsrcBmpWidth, deep));
	}
}

//获取标准矩(只支持8位灰度图)
void GetStdMoment(BYTE *psrcBmp , 
				 int nsrcBmpWidth,
				 int nsrcBmpHeight,
				 double *m)
{
	for ( int p = 0 ; p < 2 ; p++ )
		for ( int q = 0 ; q < 2 ; q++ )
		{
			if( p == 1 && q == 1)
				break;
			for ( int y = 0 ; y < nsrcBmpHeight ; y++ )
				for ( int x = 0 ; x < nsrcBmpWidth ; x++ )
					m[p*2+q] += (pow( (double)x , p ) * pow( (double)y , q ) * J_getpixel(psrcBmp , nsrcBmpWidth , x ,y, 1));
		}
}

//阶乘
double Factorial( int n )
{
	if( n < 0 )
		return -1;

	double m = 1;
	for(int i = 2 ; i <= n ; i++)
	{
		m *= i;
	}
	return m;
}

//阶乘数，计算好方便用，提高速度
double factorials[11] = {1 , 1 , 2 , 6 , 24 , 120 , 720 , 5040 , 40320 , 362880 , 39916800};

//把图像映射到单位圆，获取像素极坐标半径
double GetRadii(int nsrcBmpWidth,
	   int nsrcBmpHeight,
	   int x0,
	   int y0,
	   int x,
	   int y)
{
	double lefttop = sqrt(((double)0 - x0)*(0 - x0) + (0 - y0)*(0 - y0));
	double righttop = sqrt(((double)nsrcBmpWidth - 1 - x0)*(nsrcBmpWidth - 1 - x0) + (0 - y0)*(0 - y0));
	double leftbottom = sqrt(((double)0 - x0)*(0 - x0) + (nsrcBmpHeight - 1 - y0)*(nsrcBmpHeight - 1 - y0));
	double rightbottom = sqrt(((double)nsrcBmpWidth - 1 - x0)*(nsrcBmpWidth - 1 - x0) + (nsrcBmpHeight - 1 - y0)*(nsrcBmpHeight - 1 - y0));

	double maxRadii = lefttop;
	maxRadii < righttop ? righttop : maxRadii;
	maxRadii < leftbottom ? leftbottom : maxRadii;
	maxRadii < rightbottom ? rightbottom : maxRadii;

	double Radii = sqrt(((double)x - x0)*(x - x0) + (y - y0)*(y - y0))/maxRadii;
	if(Radii > 1)
	{
		Radii = 1;
	}
	return Radii;
}

//把图像映射到单位圆，获取像素极坐标角度
double GetAngle(int nsrcBmpWidth,
				int nsrcBmpHeight,
				int x,
				int y)
{
	double o;

	double dia = sqrt((double)nsrcBmpWidth*nsrcBmpWidth + nsrcBmpHeight*nsrcBmpHeight);
	int x0 = nsrcBmpWidth / 2;
	int y0 = nsrcBmpHeight / 2;
	double x_unity = (x - x0)/(dia/2); 
	double y_unity = (y - y0)/(dia/2);

	if( x_unity == 0 && y_unity >= 0 )
		o=pi/2;
	else if( x_unity ==0 && y_unity <0)
		o=1.5*pi;
	else
		o=atan( y_unity / x_unity );
	if(o*y<0)    //第三象限
		o=o+pi;

	return o;
}

//Zernike不变矩
J_GetZernikeMoment(BYTE *psrcBmp , 
				int nsrcBmpWidth,
				int nsrcBmpHeight,
				double *Ze )
{
	double R[count][count] = {0.0};
	double V[count][count] = {0.0};

	double M[4] = {0.0};
	GetStdMoment(psrcBmp , nsrcBmpWidth , nsrcBmpHeight , M);
	int x0 = (int)(M[2]/M[0]+0.5);
	int y0 = (int)(M[1]/M[0]+0.5);

	for(int n = 0 ; n < count ; n++)
	{
		for (int m = 0 ; m < count ; m++)
		{
			//优化算法，只计算以下介数

			if( (n == 1 && m == 0) ||
				(n == 1 && m == 1) ||
				(n == 2 && m == 0) ||
				(n == 2 && m == 1) ||
				(n == 2 && m == 2) ||
				(n == 3 && m == 0) ||
				(n == 3 && m == 1) ||
				(n == 3 && m == 2) ||
				(n == 3 && m == 3) ||
				(n == 4 && m == 0) ||
				(n == 4 && m == 1) ||
				(n == 4 && m == 2) ||
				(n == 4 && m == 3) ||
				(n == 4 && m == 4))

			{
				for(int y = 0 ; y < nsrcBmpHeight ; y++)
				{
					for (int x = 0 ; x < nsrcBmpWidth ; x++)
					{
						for(int s = 0 ; (s <= (n - m)/2 ) && n >= m ; s++)
						{
							R[n][m] += pow( -1.0, s )
									* ( n - s > 10 ? Factorial( n - s ) : factorials[ n - s ] )
									* pow( GetRadii( nsrcBmpWidth, nsrcBmpHeight, x0, y0, x, y ), n - 2 * s )
									/ ( ( s > 10 ? Factorial( s ) : factorials[ s ] )
									* ( ( n + m ) / 2 - s > 10 ? Factorial( ( n + m ) / 2 - s ) : factorials[ ( n + m ) / 2 - s ] )
									* ( ( n - m ) / 2 - s > 10 ? Factorial( ( n - m ) / 2 - s ) : factorials[ ( n - m ) / 2 - s ] ) );
						}
						Ze[ n * count + m ] += R[ n ][ m ]
											* J_getpixel( psrcBmp, nsrcBmpWidth, x ,y, 1)
											* cos( m * GetAngle( nsrcBmpWidth, nsrcBmpHeight, x, y) );//实部

						V[n][m] += R[ n ][ m ] 
								* J_getpixel( psrcBmp, nsrcBmpWidth, x, y, 1)
								* sin( m * GetAngle( nsrcBmpWidth, nsrcBmpHeight, x, y ) );//虚部

						R[n][m] = 0.0;
					}
				}
				*(Ze+n*count + m) = sqrt( (*(Ze+n*count + m))*(*(Ze+n*count + m)) + V[n][m]*V[n][m] )*(n+1)/pi/M[0];
			}
		}
	}
}
```









