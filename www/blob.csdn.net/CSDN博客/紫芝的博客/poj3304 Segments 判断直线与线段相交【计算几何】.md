# poj3304 Segments 判断直线与线段相交【计算几何】 - 紫芝的博客 - CSDN博客





2018年05月01日 19:20:58[紫芝](https://me.csdn.net/qq_40507857)阅读数：57








# Segments
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 17786||**Accepted:** 5596|

Description

Given *n* segments in the two dimensional space, write a program, which determines if there exists a line such that after projecting these segments on it, all projected segments have at least one point in common.

Input

Input begins with a number *T* showing the number of test cases and then, *T* test cases follow. Each test case begins with a line containing a positive integer *n* ≤ 100 showing the number of segments. After that, *n* lines containing four real numbers *x*1 *y*1 *x*2 *y*2 follow, in which (*x*1, *y*1) and (*x*2, *y*2) are the coordinates of the two endpoints for one of the segments.

Output

For each test case, your program must output "Yes!", if a line with desired property exists and must output "No!" otherwise. You must assume that two floating point numbers *a* and *b* are equal if |*a* - *b*| < 10-8.

Sample Input

```
3
2
1.0 2.0 3.0 4.0
4.0 5.0 6.0 7.0
3
0.0 0.0 0.0 1.0
0.0 1.0 0.0 2.0
1.0 1.0 2.0 1.0
3
0.0 0.0 0.0 1.0
0.0 2.0 0.0 3.0
1.0 1.0 2.0 1.0
```

Sample Output

```
Yes!
Yes!
No!
```

Source

[Amirkabir University of Technology Local Contest 2006](http://poj.org/searchproblem?field=source&key=Amirkabir+University+of+Technology+Local+Contest+2006)

## 题目大意：

给出n条线段两个端点的坐标，问所有线段投影到一条直线上，

如果这些所有投影至少相交于一点就输出Yes！，否则输出No！。


## 解题思路：

如果有存在这样的直线，过投影相交区域作直线的垂线，该垂线必定与每条线段相交，

问题转化为问是否存在一条线和所有线段相交


若存在一条直线与所有线段相交，此时该直线必定经过这些线段的某两个端点，所以枚举任意两个端点即可。

这里要主要的地方就是，题目说如果两个点的距离小于1e-8就等价于一点，所以要考虑重点 


### 关于coross函数：

1.若函数的返回值为0，即 (y-y1)/(x-x1)=(y2-y1)/(x2-x1); （直线斜率相等,k1=k2）

注意：（浮点数除法在计算的过程中会影响精度） 

即 过(x,y),(x1,y1)的直线(直线斜率为k1)和过(x1,y1),(x2,y2)的直线(直线斜率为k2)平行

说明 (x,y)在过 (x1,y1),(x2,y2)的直线上


2.若返回值不为0，说明 (x,y)不在过 (x1,y1),(x2,y2)的直线上

①返回值小于0，(y-y1)/(x-x1)<(y2-y1)/(x2-x1);即 k1<k2

![](https://img-blog.csdn.net/20180501191719864)

②返回值大于0，(y-y1)/(x-x1)>(y2-y1)/(x2-x1);即 k1>k2 


![](https://img-blog.csdn.net/20180501191759582)

### 关于judge函数：

若(x1,y1),(x2,y2)不重合，判断过(x1,y1),(x2,y2)的直线是否和线段seg[i]相交 

1.coross(x1,y1,x2,y2,seg[i].x1,seg[i].y1)*coross(x1,y1,x2,y2,seg[i].x2,seg[i].y2)

若乘积的符号为正，则说明 过(x1,y1),(x2,y2)的直线和线段seg[i]不相交 


2.coross(x1,y1,x2,y2,seg[i].x1,seg[i].y1)*coross(x1,y1,x2,y2,seg[i].x2,seg[i].y2)

若乘积的符号为负，则说明 过(x1,y1),(x2,y2)的直线和线段seg[i]相交 

![](https://img-blog.csdn.net/20180501192029159)

```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
const int MAX=105;
const double EPS=1e-8;//EPS相当于0
struct node{
	double x1,y1,x2,y2;
}seg[MAX];
int n;
double distance(double x1,double y1,double x2,double y2)
{
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

//若两条直线斜率相等，则不相交 
double coross(double x1,double y1,double x2,double y2,double x,double y)
{
	return (x2-x1)*(y-y1)-(x-x1)*(y2-y1); //叉积 
}

//判断经过点(x1,y1),(x2,y2)的直线是否和所有的线段相交 
bool judge(double x1,double y1,double x2,double y2)
{
	if(distance(x1,y1,x2,y2)<EPS)	return 0;//两点重合 
	for(int i=0;i<n;i++)
	{
	if(coross(x1,y1,x2,y2,seg[i].x1,seg[i].y1)*
	coross(x1,y1,x2,y2,seg[i].x2,seg[i].y2)>EPS)
	return 0;
	}
	return 1;
}
int main()
{
	int t;
	scanf("%d",&t);
	while(t--){
		scanf("%d",&n);
		for(int i=0;i<n;i++)
	scanf("%lf%lf%lf%lf",&seg[i].x1,&seg[i].y1,&seg[i].x2,&seg[i].y2);						
	if(n==1){
		printf("Yes!\n");continue;
	}
		bool ans=0;
		for(int i=0;i<n&&!ans;i++){
			for(int j=i+1;j<n&&!ans;j++){
				if(
				judge(seg[i].x1,seg[i].y1,seg[j].x1,seg[j].y1)||
				judge(seg[i].x1,seg[i].y1,seg[j].x2,seg[j].y2)||
				judge(seg[i].x2,seg[i].y2,seg[j].x1,seg[j].y1)||
				judge(seg[i].x2,seg[i].y2,seg[j].x2,seg[j].y2)
				)
				ans=1;
			}
		}
		if(ans) printf("Yes!\n");  
        else printf("No!\n");  
	}
	return 0;
}
```

修改于10.25

我们先来看一下只有1条线段的情况，一条线段把平面分成两个区间，如果点在线段的左侧，那么这个点在左区间（0号），否则在右区间（1号）；

![](https://img-blog.csdn.net/20181024190042209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点(u,y1)和点(x,y)连线的斜率为k1= ![\frac{y1-y}{u-x1}](https://private.codecogs.com/gif.latex?%5Cfrac%7By1-y%7D%7Bu-x1%7D)，点(l,y2)和点(x,y)连线的斜率为k2= ![\frac{y2-y}{l-x2}](https://private.codecogs.com/gif.latex?%5Cfrac%7By2-y%7D%7Bl-x2%7D)
- k1=k2,三点共线
- k1>k2,点在直线的右侧
- k1<k2,点在直线的左侧

**如果线段两个端点在直线的两侧，则线段和直线相交；**

**如果线段两个端点在直线的一侧，则线段和直线不相交；**

```cpp
import java.util.*;
import java.math.*;
class point{
	double x,y;
	public void point(double x,double y) {
		this.x=x;
		this.y=y;
	}
}
class segment{
	point a,b;
	public void segment(point a,point b) {
		this.a=a;
		this.b=b;
	}
}
public class Main{
	static int maxn=110;
	static double EPS=1e-8;
	static int n;
	static segment[] seg=new segment[110];
	
	static double dist(point a,point b) {
		double dis=Math.sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
		return dis;
	}
	//判断点是否在过a，b两点的直线上
	//若两条直线斜率相等，则点在直线上
	static double cross(point a,point b,point p) {
		return (a.x-p.x)*(b.y-p.y)-(b.x-p.x)*(a.y-p.y);//叉积
	}
	//判断经过a，b两点的直线是否与所有的线段相交
	static boolean judge(point a,point b) {
		if(Math.abs(dist(a,b))<EPS)
			return false;
		for(int i=0;i<n;i++) {
			if(cross(a,b,seg[i].a)*cross(a,b,seg[i].b)>EPS)
				return false;//线段两点在直线的同一侧，直线和线段一定不相交
		}
		return true;
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int T=cin.nextInt();
		while((T--)!=0) {
			n=cin.nextInt();
			for(int i=0;i<n;i++) {
				point a=new point();
				point b=new point();
				a.x=cin.nextDouble();
				a.y=cin.nextDouble();
				b.x=cin.nextDouble();
				b.y=cin.nextDouble();
				segment tmp=new segment();
				tmp.segment(a, b);
				seg[i]=tmp;
			}
			
			if(n==1) {//特判只有一条线段
				System.out.println("Yes!");
				continue;
			}
				
			boolean flag=false;
			for(int i=0;i<n&&!flag;i++) {
				for(int j=i+1;j<n&&!flag;j++) {
					if(judge(seg[i].a,seg[j].a)||judge(seg[i].a,seg[j].b)
					   ||judge(seg[i].b,seg[j].a)||judge(seg[i].b,seg[j].b))
						flag=true;
				}
			}
			if(flag)
				System.out.println("Yes!");
			else
				System.out.println("No!");
		}
		cin.close();
	}
}
```





