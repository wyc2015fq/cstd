# 【浮点取整】#39 A.Find Color - CD's Coding - CSDN博客





2014年07月28日 11:09:37[糖果天王](https://me.csdn.net/okcd00)阅读数：642标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)



















这一题呢在Problemset里是40A，在Contest里是39A，我着实没辙了一下……这居然还有偏差的。。。

我既然都是#39这样来的，而且看到后面分Div1和Div2之后一个contest会有2个problemset的编号……就一直按照#Round的顺序写下去好了～




这个题本来想用直角坐标系转角坐标来写的。突然发现用不着……第一象限和第三象限是xy>0，第二象限和第四象限是xy<0

然后用sqrt(x*x+y*y)获得ρ得知距离远点距离，通过(int)强制转换获得【向0取整】的整型，从而可以判断奇偶

两者结合起来判断方位，别忘了在这之前特判一下落在分界上的情况~

Code：



```cpp
#include <queue>
#include <cmath> 
#include <memory> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int main()
{
	double x,y;
	cin>>x>>y;
	double raw=sqrt(x*x+y*y);
	int p_sign=(x*y>0?1:0);
	int d_raw=(int)sqrt(x*x+y*y);
	int color=(d_raw%2)+p_sign;
	if(raw-(int)raw==0)cout<<"black"<<endl;
	else cout<<((color%2==0)?"white":"black")<<endl;
	
	return 0;
}
```






