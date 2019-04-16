# 超纯超美的曲线（Peter De Jong Attractor） - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年01月13日 13:56:46[gloomyfish](https://me.csdn.net/jia20003)阅读数：5758标签：[float																[算法																[variables																[image																[html5																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[图像处理](https://blog.csdn.net/jia20003/article/category/879324)






无意之中在网上看到一篇基于HTML5 Demo的Peter De Jong　吸引子生成图像的Demo，没Hold住，


感觉太漂亮了。于是想研究一下，发现有个专业术语 - 分形。一位大师这样评价分形学科 - “it is not about 


math, it is about art” 显然有时候程序员也是有点艺术细胞。





效果图一

![](http://hi.csdn.net/attachment/201201/13/0_13264336830Yl9.gif)


效果图二

![](http://hi.csdn.net/attachment/201201/13/0_1326433720trJj.gif)


效果图三：

![](http://hi.csdn.net/attachment/201201/13/0_1326433741CvL6.gif)


**产生原理**



基于Peter De Jong公式：Peter De Jong的数学公式如下：

Xn+1 = sin(a*yn) - cos(b*xn)

Yn+1 = sin(c*xn) - cos(d*yn)

从一个随机的点P(x0, y0)开始得到下一个P（xn,yn）然后令 Xn = Xn+1, Yn = Yn+1

迭代产生更多数据。



**算法实现**

算法实现基于分形火焰算法，首先根据迭代的出来的数据点，使用直方图统计算法得到指定大小直方图数据点,

然后再根据直方图的结果实现像素绘制和颜色，透明度生产。关于分形火焰算法可以参考这里 ：

[http://en.wikipedia.org/wiki/Fractal_flame](http://en.wikipedia.org/wiki/Fractal_flame)

** 完成随机点产生与直方图统计代码：**

****

```java
private void getPoint() {

		la = a;
		lb = b;
		lc = c;
		ld = d; 
		
		// Add Noise to Coefficients for Smoothness
		if (noise) {
			la += getRandom();
			lb += getRandom();
			lc += getRandom();
			ld += getRandom();
		}
		
		// *************************************************
		// ** Update Temp Variables -- Magic
		xn = (float)(Math.sin(la * y) - Math.cos(lb * x)); 
		yn = (float)(Math.sin(lc * x) - Math.cos(ld * y)); 
		
		// color here
		zn = (float)(Math.sin(e * x) - Math.cos(f * z)); 
		
		// take current result as x0,y0,z0 in order to calculate next xn,yn,zn
		x = xn;
		y = yn;
		z = zn;
		
		// Convert to 2D Image Space for Plotting
		u = (int) ((x + 2.5) * K); 
		v = (int) ((y + 2.5) * K);

		tr = (float)(z * 0.9 + (1.0 - z) * 0.6); // Map Z-Coordinate to Color
		tg = (float)(z * 0.2 + (1.0 - z) * 0.4);
		tb = (float)(z * 0.5 + (1.0 - z) * 0.9);
		
    	pList[u][v][0] += 1.0f; // alpha value, normalization factor, must have it
    	pList[u][v][1] += tr; //Add New Point to Total
    	pList[u][v][2] += tg;  
    	pList[u][v][3] += tb;
	}
```
****


**结果归一化与像素绘制代码：**



```java

```

```java
private void renderPoints(int[] rgbData) {
		float max = -1.0f;
	    for (int i=0; i < nSize; i++) {
	      for (int j=0; j < nSize; j++) {
	          if (pList[i][j][3] > max){
	            max = pList[i][j][3];
	            }
	          }
	      }

	    //Adjust Values and Fill Image
	    float logval, logmax = (float)Math.log(max);
	    float M = (float)((logmax * logmax) / 255.0f);
	    int[] color ={0,0,0,0};
	    for (int i=0; i < nSize; i++) {
	        for (int j=0; j < nSize; j++) {
	          for (int k=0; k < 4; k++) {
	            logval = (float)Math.log(pList[i][j][k]);
	            color[k] = (int) (logval * logval / M);
	          }
	          // rgbData[j*nSize + i] = (color[0] << 24) | (color[1] << 16) | (color[2] << 8) | color[3];
	          rgbData[j*nSize + i] = (255 << 24) | (color[1] << 16) | (color[2] << 8) | color[3];
	      }
	    }
	}
```






随机产生参数 a, b,c,d,e,f值区间在[-3,3]之间，可以得到你认为很cool的效果。

算是送给大家的2012新年礼物吧，程序完全是基于Java语言实现。](https://so.csdn.net/so/search/s.do?q=html5&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=variables&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)




