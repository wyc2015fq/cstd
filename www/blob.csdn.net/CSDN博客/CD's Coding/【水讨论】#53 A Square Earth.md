# 【水讨论】#53 A. Square Earth? - CD's Coding - CSDN博客





2014年08月08日 20:43:47[糖果天王](https://me.csdn.net/okcd00)阅读数：671
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Meg the Rabbit decided to do something nice, specifically — to determine the shortest distance between two points on the surface of our planet. But Meg... what can you say, she wants everything simple. So, she already regards our planet as a two-dimensional
 circle. No, wait, it's even worse — as a square of side *n*. Thus, the task has been reduced to finding the shortest path between two dots on a square (the
 path should go through the square sides). To simplify the task let us consider the vertices of the square to lie at points whose coordinates are: (0, 0), (*n*, 0), (0, *n*) and (*n*, *n*).




Input


The single line contains 5 space-separated integers: *n*, *x*1, *y*1, *x*2, *y*2 (1 ≤ *n* ≤ 1000, 0 ≤ *x*1, *y*1, *x*2, *y*2 ≤ *n*)
 which correspondingly represent a side of the square, the coordinates of the first point and the coordinates of the second point. It is guaranteed that the points lie on the sides of the square.




Output


You must print on a single line the shortest distance between the points.




Sample test(s)




input
2 0 0 1 0




output
1




input
2 0 1 2 1




output
4




input
100 0 0 100 100




output
200















有一个n为边长的正方形边上有两个点，问只能走正方形边界的情况下最短路程是多长（顺时针A到B或者逆时针A到B咯~）。

好吧……分情况讨论好了……

我们把这个正方形从左下到右上分为两半，然后——

1) 两个点在同侧的话，就坐标(x+y)的差即可，记住abs取绝对值

2) 在不同侧的话，就两个方向看看哪个小咯~一侧为x1+y1+x2+y2,另一侧就直接正方形周长4n来减去前面那个就好。

Code：



```cpp
#include <cstdio>
#include <memory>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int n,x1,y1,x2,y2;
	cin>>n>>x1>>y1>>x2>>y2;
	int len1=x1+y1,len2=x2+y2; 
	//if((x1-y1)*(x2-y2)>=0) dist=abs(x1+y1-x2-y2); 	// 两点在 y=x 同侧 
	//else dist=x1+y1+x2+y2;				// 两点在 y=x 异侧 
	int dist=abs( x1+y1-( (x1-y1)*(x2-y2)>=0 ? (x2+y2):(-x2-y2) ) );
	cout<<min(dist,4*n-dist)<<endl;
	return 0;
}
```














