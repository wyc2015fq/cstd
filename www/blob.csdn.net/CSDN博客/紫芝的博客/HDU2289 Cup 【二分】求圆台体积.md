# HDU2289 Cup 【二分】求圆台体积 - 紫芝的博客 - CSDN博客





2018年11月10日 11:49:47[紫芝](https://me.csdn.net/qq_40507857)阅读数：305








# Cup

****Time Limit: 3000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 11390    Accepted Submission(s): 3500****

**Problem Description**

The WHU ACM Team has a big cup, with which every member drinks water. Now, we know the volume of the water in the cup, can you tell us it height? 


The radius of the cup's top and bottom circle is known, the cup's height is also known.

**Input**

The input consists of several test cases. The first line of input contains an integer T, indicating the num of test cases.

Each test case is on a single line, and it consists of four floating point numbers: r, R, H, V, representing the bottom radius, the top radius, the height and the volume of the hot water.


Technical Specification


1. T ≤ 20.

2. 1 ≤ r, R, H ≤ 100; 0 ≤ V ≤ 1000,000,000.

3. r ≤ R.

4. r, R, H, V are separated by ONE whitespace.

5. There is NO empty line between two neighboring cases.

**Output**

For each test case, output the height of hot water on a single line. Please round it to six fractional digits.

**Sample Input**

1 100 100 100 3141562

**Sample Output**

99.999024

**Source**

[The 4th Baidu Cup final](http://acm.hdu.edu.cn/search.php?field=problem&key=The+4th+Baidu+Cup+final&source=1&searchmode=source)

**Recommend**

lcy

 题目分析：给你一个倒置的圆台（上圆半径R>=下圆半径r）,给你一定体积的水，问将水倒入圆台后，水的高度。

如下图
![](https://img-blog.csdn.net/20130821093611828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb194X21pc3M=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20130821093621687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb194X21pc3M=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如右图设装得水高为h1，则此时上圆半径

r1= r +  r2 

在三角形ABC中由三角形相似得

![\frac{h1}{h}=\frac{r2}{r}](https://private.codecogs.com/gif.latex?%5Cfrac%7Bh1%7D%7Bh%7D%3D%5Cfrac%7Br2%7D%7Br%7D)

![r1=r+\frac{r*h1}{h}](https://private.codecogs.com/gif.latex?r1%3Dr&plus;%5Cfrac%7Br*h1%7D%7Bh%7D)

由圆台的体积公式可得：

![V=\frac{1}{3}*PI*h1*(r1*r1+r*r+r1*r)](https://private.codecogs.com/gif.latex?V%3D%5Cfrac%7B1%7D%7B3%7D*PI*h1*%28r1*r1&plus;r*r&plus;r1*r%29)

接着二分h可得解


```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)2e7+10;
	static double EPS=1e-8;
	static double r,R,H,V;
	static double PI=Math.acos(-1);
	
	static boolean check(double mid) {
		double r1=r+mid*(R-r)/H;//当前水面半径
		double volume=PI*(r*r+r1*r+r1*r1)*mid/3.0;//求水的体积
		if(volume<V)
			return true;
		return false;
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int T=cin.nextInt();
		int ca=1;
		while((T--)!=0) {
			r=cin.nextDouble();
			R=cin.nextDouble();
			H=cin.nextDouble();
			V=cin.nextDouble();
			double left=0,right=H;
			while(left<=right) {
				if(right-left<EPS)	break;
				double mid=(left+right)/2.0;
				if(check(mid))
					left=mid;
				else
					right=mid;
			}
			System.out.println(String.format("%.6f", right));
		}
		cin.close();
	}
}
```





