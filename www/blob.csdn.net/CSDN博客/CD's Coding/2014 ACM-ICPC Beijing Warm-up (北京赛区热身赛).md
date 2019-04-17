# 2014 ACM-ICPC Beijing Warm-up (北京赛区热身赛) - CD's Coding - CSDN博客





2014年11月15日 20:48:38[糖果天王](https://me.csdn.net/okcd00)阅读数：1086标签：[ACM-ICPC 2014																[Beijing																[Warm-up																[北京赛区热身赛](https://so.csdn.net/so/search/s.do?q=北京赛区热身赛&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





ACM-ICPC 2014 Beijing Warm-up (北京赛区热身赛)


这次四道题好萌好萌……萌的裁判都一脸血……

比赛是一次不错的经历，要好好把握锻炼的机会~

这四题太萌了点反而觉得没有比赛的氛围

不过C题做不出来还是深深感受到不足Q^Q

明天要加油呢……







我们比赛的时候问了这么个问题……然后被AllTeam广播回复了……

![](https://img-blog.csdn.net/20141115205343296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


紧接着就有人问

![](https://img-blog.csdn.net/20141115205420391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


也被公共频道回复了……










简述题意：

## A题 ACM

请数出整本题册中出现的所有“ACM”字符串（大小写均可）的个数。

即数ACM在这本题册里出现了多少次，封面里的要算，封面贴图里的acm也要算，页眉页脚的也要算，题目里出现的甚至连这个问题中的ACM也要算，共26个。

因为不知道这个算不算那个算不算诸如此类的问题，赛场中掀起一场提交狂潮，裁判机都哭了…… 裁判表示你们数数都数不对不能怪裁判机慢

什么你要源码？

### Code：



```cpp
#include<iostream>
using namespace std;
int main()
{cout<<26;return 0;}
```





## B题 Brilliant Prim-Number

给一个数字，如果这个数字的质因数个数是质数，则输出Prim，反之输出Kruskal。

先筛法筛1e5的素数（两个99997乘起来肯定大于1e9啦），然后？直接质因数分解数个数呗~ 

这里需要注意的是素数都试完了之后还没有被分解过可能是超过1e5的大素数哦~

### Code:



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
int prime[100050]={0},T,n;
vector <int> pri;

void init()
{
	for(int i=2;i<100001;i++)
	{
		if(prime[i]==0)
		{
			pri.push_back(i);
			for(int j=i;j<=100000;j+=i) prime[j]=1;
		}
	}
} 

int main()
{
	init();
	cin>>T;
	for(int z=1;z<=T;z++)
	{
		cin>>n;
		int ans=0;
		while(n!=1)
		{
			for(int i=0;i<=pri.size()-1;i++)
			{
				bool f=false;
				while(n%pri[i]==0)
				{
					n=n/pri[i];
					if(!f)ans++;
					f=true;
				}
			}
			if(n>pri[pri.size()-1])
			{
				ans++;
				break;
			}
		}
		bool flag=false;
		for(int i=0;i<pri.size();i++)
		{
			if(pri[i]==ans)
			{
				flag=true;
				break;
			}else if(pri[i]>ans) break;
		}
		flag?printf("Case #%d: Prim!\n",z):printf("Case #%d: Kruskal!\n",z);
	}
	return 0;
}
```





## C题 Construction

题意为给定N，P，Q

要求构造一个长度为N的数列，要求每相邻P个的和都为正，相邻Q个的和都为负。

可构造则输出任意一组，不可构造则输出NoSolution




当前想到的是这么几点：

1） N>= (P+Q)+gcd(P,Q) 为NoSolution

2）N>= [P,Q] 为NoSolution

3）答案为PQ中较小的那个作为长度的一个小数组一直循环到N个的数列

### Code:

暂缺




## D题 （题目叫啥我忘了不好意思……）

这道题是一个编译模拟题，给一个GCC的程序问你结果是WA还是TLE……

当时吓呆了简直……




然后发现……HINT：只有一组测试数据，请大家放心大胆的猜答案吧……

呃呃呃呃呃……




因为组数T∈[1,3]，答案只有两种，WA或者TLE，所以最多试验2+4+8=14种必然有一个是结果。

但是因为裁判机可以返回TLE和WA两种（如果可以PE和OLE就可以同时实验4种情况了可惜……），所以我们可以

if(n==1||n==2) while(1)n++;

来判断T的规模是多大。

然后得知是T=3就是实验咯

我们的顺序：

TWT WA

WTW WA

TTT WA

WWW WA

TWW WA


TTW YES




所以这道题也真是萌一脸……

### Code：


```cpp
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int T=0;	cin>>T;
	for(int i=0;i<T;i++)
	{
		int n=0;	cin>>n;
		for(int j=0;j<n;j++)
		{
			string str;
			getline(cin,str);
		} 
	}
	cout<<"Case #1: Time Limit Exceeded!"		<<endl;
		cout<<"Case #2: Time Limit Exceeded!"	<<endl;
			cout<<"Case #3: Wrong Answer!"		<<endl;
	return 0;
}
```







![](https://img-blog.csdn.net/20141115205551812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://so.csdn.net/so/search/s.do?q=Warm-up&t=blog)](https://so.csdn.net/so/search/s.do?q=Beijing&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM-ICPC 2014&t=blog)




