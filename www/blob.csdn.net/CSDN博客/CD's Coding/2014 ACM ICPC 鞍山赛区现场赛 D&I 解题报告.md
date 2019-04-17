# 2014 ACM/ICPC 鞍山赛区现场赛 D&I 解题报告 - CD's Coding - CSDN博客





2014年10月20日 01:58:38[糖果天王](https://me.csdn.net/okcd00)阅读数：1961标签：[2014																[ACMICPC																[鞍山赛区现场赛																[解题报告](https://so.csdn.net/so/search/s.do?q=解题报告&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





鞍山现场赛结束了呢…… 我们出的是D+E+I三道题……

吾辈AC掉的是D和I两道，趁着还记得，先在这里写一写我写的两道水题D&I的解题报告吧^_^。




D题的意思呢是说星云内有一堆排成一条直线的小行星们，给出每个小行星所处在的距离

我们需要让小行星距离重心距离的平方和最小 （Σdi^2）

（啊细节记忆不太清楚就帮大家简化题意了哦^_^）我们最多可以移动k个小行星，问最优状态下上述平方和最小为多少

首先我们知道，k肯定是需要用满的，反证非k次移动一定不优于k次即可。

然后就是如何移动的问题，我们将输入数据按升序排列后，j从0到k枚举，即从左边拿出j个，从右边拿出k-j个，然后把这些都放在剩余的星星们的重心上（即可以无视），只要计算此时的平方和然后找最小值即可

但是——TLE了……

然后立马想到了一个剪枝，就是如果加到某一个值的时候已经比当前minv大了，就可以不继续加来检查下一种情况了

队友们不相信这也能过……于是等到队友们闲下来……交了一下……就AC了……

（后记：哎呀我去……HDU模拟赛出来之后交了一发TLE了……所以下面这个算法是错的……还是说现场赛的机子算得快？）

Code：



```cpp
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
int n,k,d[50055];
ll sumd[50055];

ll getsum(int i,int j) //[i,j)
{
	if(i)return sumd[j-1]-sumd[i-1];
	else return sumd[j-1]; 
}

int main()
{
	int T;	cin>>T;	while(T--)
	{
		memset(d,0,sizeof d);
		memset(sumd,0,sizeof sumd);
		scanf("%d%d",&n,&k);
		for(int i=0;i<n;i++)
			scanf("%d",&d[i]);
		sort(d,d+n);
		for(int i=0;i<n;i++)
			if(i) sumd[i]=sumd[i-1]+(ll)d[i];
			else sumd[i]=(ll)d[i];
		double messc=0.0,minv=0.0,now=0.0;
		for(int j=0;j<=k;j++)
		{
			ll nows=getsum(j,n-(k-j));	//now sum of j ~ (n-k+j)
			messc=(double)nows/(double)(n-k);
			now=0.0;
			for(int i=j;i<=(n-1)-(k-j);i++)// it means move i item at front and (n-(k-j)) items at end
			{
				double dis=(double)d[i]-messc;
				now+=(dis*dis);
				if(j && now>=minv) break; //IMPORTANT! TLE if miss it
			}
			if(!j)minv=now;
			else if(now<minv) minv=now;
		}
		printf("%.10f\n",minv);
	}
}
```

楼上的算法现场赛过了但是HDU过不了了…… 好在现在ACM官网上发出了kuangbin大大的解法，也拿来学习一下：

转载地址：http://www.kuangbin.net/archives/hdu5073



```cpp
/* ***********************************************
Author        :kuangbin
Created Time  :2014/10/22 23:20:07
File Name     :E:\2014ACM\2014现场赛\鞍山\D.cpp
************************************************ */

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
double x[50010];

int main()
{
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    int T;
	int n,k;
	scanf("%d",&T);
	while(T--){
		scanf("%d%d",&n,&k);
		for(int i = 0;i < n;i++)
			scanf("%lf",&x[i]);
		sort(x,x+n);
		if(n == k){
			printf("0\n");
			continue;
		}
		int cnt = n-k;
		double sum = 0;
		double sum2 = 0;
		for(int i = 0;i < cnt;i++){
			sum += x[i];
			sum2 += x[i]*x[i];
		}
		double ans = sum2 - 2*sum*(sum/cnt) + cnt*(sum/cnt)*(sum/cnt);
		for(int i = cnt;i < n;i++){
			sum += x[i];
			sum2 += x[i]*x[i];
			sum -= x[i-cnt];
			sum2 -= x[i-cnt]*x[i-cnt];
			ans = min(ans,sum2-2*sum*(sum/cnt)+cnt*(sum/cnt)*(sum/cnt));
		}
		printf("%.10lf\n",ans);
	}
    return 0;
}
```












接着是神水题I，就是大家都作为签到题的那个

题意转述的话：OSU里有好多个按顺序出现的点，每两个点都有个距离，距离除以时间称作难度，求难度的最大值。

既然都说了时间肯定是升序了……那每次都计算当前dist，然后更新max咯~



```cpp
#include <cmath>
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
ll t,x,y,tt,xx,yy; //present, last 

double dist()
{
	ll asqr=(ll)(x-xx)*(x-xx)+(ll)(y-yy)*(y-yy);
	double ret=sqrt((double)asqr);
	return ret/(double)(t-tt);
}

int main()
{
	int T;	cin>>T;
	while(T--)
	{
		int N;	cin>>N;
		double maxd=-1.0;
		for(int Ncnt=0;Ncnt<N;Ncnt++)
		{
			cin>>t>>x>>y;
			if(Ncnt && dist()>maxd)
			{
				double now=dist();
				if(now>maxd)maxd=now;
			}
			tt=t,xx=x,yy=y;
		}
		printf("%.10f\n",maxd);
	} 
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=鞍山赛区现场赛&t=blog)](https://so.csdn.net/so/search/s.do?q=ACMICPC&t=blog)](https://so.csdn.net/so/search/s.do?q=2014&t=blog)




