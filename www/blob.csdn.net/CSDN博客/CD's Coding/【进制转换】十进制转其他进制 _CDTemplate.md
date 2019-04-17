# 【进制转换】十进制转其他进制 _CDTemplate - CD's Coding - CSDN博客





2015年04月25日 16:20:21[糖果天王](https://me.csdn.net/okcd00)阅读数：644标签：[进制转换																[十进制																[n进制																[十六进制](https://so.csdn.net/so/search/s.do?q=十六进制&t=blog)
个人分类：[模板记忆](https://blog.csdn.net/okcd00/article/category/1650751)





CodeHunt里有个进制转换的36进制转换，以前在清澄做过，但是可惜没放在CSDN过，这个着实是暴力的进制转换没有错，但是为了以后的速度，还是在这里存一份好了……

顺便提一句，特别地，对十六进制的转换时：[http://blog.csdn.net/okcd00/article/details/45196255](http://blog.csdn.net/okcd00/article/details/45196255)




对于任何一个数字，及10-36间的进制基数，接口 TBase(n,m)

【n】 long long 长度的原十进制整数

【m】 进制基数Base

Code：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

string TBase(ll in,int m)
{
	string s="";
	ll n=in;
	while(n)
	{
		ll mod=n%m;
		if(mod) s+= (mod<10)? '0'+mod : 'A'+mod-1 ;
		n/=m;
		if(!n) break;
	}
	return s;
}

int main()
{
	ll n;	int m;
	cin>>n>>m;
	cout<<"Trans "<< n <<" to Base-"<< m <<" ："<<TBase(n,m); 
	return 0;
}
```






几年前清澄上吾辈的C语言代码，怀念~~ 贴一下：


- #include<stdio.h>
- #include<string.h>
- int str[10000];
- int main()
- 
{
- int n,m,len=1;
- 
    scanf("%d %d",&n,&m);
- if(!n) {
- 
    printf("0\n");
- return0;}
- while(n)
- 
    {
- int mod=n%m;
- 
        str[len++]=mod;
- 
        n=(n-mod)/m;
- if(!n) break;
- 
    }
- while(--len)
- 
    {
- if(str[len]<10) printf("%d",str[len]);
- else printf("%c",str[len]-10+'A');
- 
    }
- 
}
-](https://so.csdn.net/so/search/s.do?q=n进制&t=blog)](https://so.csdn.net/so/search/s.do?q=十进制&t=blog)](https://so.csdn.net/so/search/s.do?q=进制转换&t=blog)




