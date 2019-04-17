# 第二次codeforces竞技结束 #249 Div 2 - CD's Coding - CSDN博客





2014年05月31日 01:45:35[糖果天王](https://me.csdn.net/okcd00)阅读数：907

















It's that time of the year when the Russians flood their countryside summer cottages (dachas) and the bus stop has a lot of people. People rarely go to the dacha on their own, it's usually a group, so the people stand in queue by groups.


The bus stop queue has *n* groups of people. The *i*-th
 group from the beginning has *a**i* people.
 Every 30minutes an empty bus arrives at the bus stop, it can carry at most *m* people.
 Naturally, the people from the first group enter the bus first. Then go the people from the second group and so on. Note that the order of groups in the queue never changes. Moreover, if some group cannot fit all of its members into the current bus, it waits
 for the next bus together with other groups standing after it in the queue.


Your task is to determine how many buses is needed to transport all *n* groups to the dacha countryside.




Input


The first line contains two integers *n* and *m*(1 ≤ *n*, *m* ≤ 100).
 The next line contains *n* integers:*a*1, *a*2, ..., *a**n*(1 ≤ *a**i* ≤ *m*).




Output


Print a single integer — the number of buses that is needed to transport all *n* groups to the dacha countryside.




Sample test(s)




input
4 3
2 3 2 1




output
3




input
3 4
1 2 1




output
1

















A大约是个水题：

一个一个装车，一旦这group人比车上空位少，车你就走吧，别管后面的人~



```cpp
#include <map>
#include <stack>
#include <queue>
#include <cmath> 
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

int cmp(const void *a, const void *b)
{
	return(*(int *)a-*(int *)b);
}

int que[101];

int main()
{
	int n=0,m=0;
	scanf("%d%d",&n,&m);
	memset(que,0,sizeof(que));
	//queue<int> q;
	//while(!q.empty()) q.pop();
	for(int i=0;i<n;i++)
	{
		int tmp=0;
		scanf("%d",&tmp);
		que[i]=tmp;
	}
	int rest=m,cnt=0,now=0;
	while(que[n-1]!=0)
	{
		if(rest<que[now])
		{
			cnt++;
			rest=m;
		}
		else
		{
			rest-=que[now];
			que[now]=0;
			now++;
		}
	}
	if(rest!=m)cnt++; 
	printf("%d",cnt);
	return 0;
}
```


官解：



```cpp
int result = 1;
int people = 0;

for(int i = 0; i < n; i++)
{
    if (people + a[i] <= m)
        people += a[i];
    else
    {
        result++;
        people = a[i];
    }
}

cout << result << endl;
```















Pasha has a positive integer *a* without leading zeroes. Today he decided that the number is too small and he should make it larger. Unfortunately, the only
 operation Pasha can do is to swap two adjacent decimal digits of the integer.


Help Pasha count the maximum number he can get if he has the time to make at most *k* swaps.




Input


The single line contains two integers *a* and *k*(1 ≤ *a* ≤ 1018; 0 ≤ *k* ≤ 100).




Output


Print the maximum number that Pasha can get if he makes at most *k* swaps.




Sample test(s)




input
1990 1




output
9190




input
300 0




output
300




input
1034 2




output
3104




input
9090000078001234 6




output
9907000008001234















B：我刚刚结束的时候才想到是这么个意思……不过来不及了敲不完就不管啦，就写个解题报告好了

把数组num排个序获得理应是最大的数组序列sup，然后

1、如果k比排序完所需的总步数多，直接输出降序序列

2、num[i]从左往右一个一个数字的找它右边离他最近的sup[i]，距离比k大的话，直接移（这一段最后边那个数扔最前边，即last->first，其余后移一位）

2.2、如果距离比k小的话，从num[i]开始k位找最大的移到最前，重复直到k用完为止

3、输出

官解：




The problem could solved by greedy algorithm. We will try to pull maximum digits to the beginning. The algorithm could be described in this way:


1) Consider every position in the number from the first, assume that the current position is *i*

2) Find the maximum digit from the next *k* digits of the number, assume that this digit is on position *j*

3) If this maximum digit is greater than current digit on position *i*, then make series of swaps, push this digit to position *i*,
 also decrease *k*, that is do *k* = *k* - (*j* - *i)*



```cpp
#include <map>
#include <stack>
#include <queue>
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

int cmp(const void *a, const void *b)
{
	return(*(int *)a-*(int *)b);
}

int num[20],sup[20];
int away[20],vis[20];

int main()
{
	//loop varieties
	int i=0,j=0,m=0;
	memset(num,0,sizeof(num));
	memset(sup,0,sizeof(sup)); 
	memset(away,0,sizeof(away));
	memset(vis,0,sizeof(vis));
	ll _num=0;
	int k=0,_i=0,len=0;
	scanf("%lld%d",&_num,&k);
	while(_num!=0)
	{
		num[_i]=_num%10;
		sup[_i]=_num%10;
		_num/=10;
		len++,_i++;
	}
	sort(sup,sup+len,greater<int>()); 	//最大数 sup 
	for(i=0;i<len/2;i++)			//当前数 num 
	{
		int temp=num[i];
		num[i]=num[len-1-i];
		num[len-1-i]=temp;
	}
	int position=0;
	while(k>0)
	{
		if(num[position]==sup[position])
		{
			position++;
			continue;
		}
		else for(i=position;i<len;i++)
		{
			if(num[i]==sup[position])
			{
				if(k>position-i)
				{
					k-=(position-i);
					int temp_1=num[i];
					for(j=i;j>position;j--)
					{
						num[j]=num[j-1];
					}
					num[position]=temp_1;
				}
				else break;
			}
		}
		int maxInRest=0;
		//Calc Rest..
		for(m=i;m<=i+k;m++)
		{
			if(num[m]>maxInRest)maxInRest=num[m];
		}
		//Time's Up .Goodgame GG
	}
	return 0;
}
```

AC CODE: From:[hcbbt](http://my.csdn.net/hcbbt)@CSDN



```cpp
/*
*  Author:      illuz <iilluzen[at]gmail.com>
*  File:        b.cpp
*  Create Date: 2014-05-30 23:40:23
*  Descripton:  Bug fixed.
*/

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 20;
int k, len;
char a[N];

void boo() {
	for (int i = 0; i < len; i++) {
		int p = i;	// p是后面能换的那些数中最大的位置
		for (int j = 1; j <= k && j + i < len; j++) {
			if (a[i + j] > a[p]) {
				p = i + j;
			}
		}
		if (a[p] == a[i]) {	// 如果一样大就没意义了
			continue;
		}
		for (int j = p; j > i; j--)
			swap(a[j], a[j - 1]);
		k -= p - i;
		if (k <= 0)
			return;
	}
}

int main()
{
	scanf("%s%d", a, &k);
	len = strlen(a);
	boo();
	printf("%s\n", a);
	return 0;
}
```













In this problem, your task is to use ASCII graphics to paint a cardiogram.


A cardiogram is a polyline with the following corners:
![](http://espresso.codeforces.com/00a323dc6114dc5ef39cb045f4fee905267d842a.png)

That is, a cardiogram is fully defined by a sequence of positive integers *a*1, *a*2, ..., *a**n*.


Your task is to paint a cardiogram by given sequence *a**i*.




Input


The first line contains integer *n*(2 ≤ *n* ≤ 1000).
 The next line contains the sequence of integers*a*1, *a*2, ..., *a**n*(1 ≤ *a**i* ≤ 1000).
 It is guaranteed that the sum of all *a**i* doesn't
 exceed 1000.




Output


Print *max* |*y**i* - *y**j*| lines
 (where *y**k* is
 the *y* coordinate of the *k*-th point of the polyline),
 in each line print ![](http://espresso.codeforces.com/1f1a961d8dec58f3b3d6959618fb497e20ebb1cb.png) characters.
 Each character must equal either « / » (slash), « \ » (backslash),
 «» (space). The printed image must be the image of the given polyline. Please study the test samples for better understanding of how to print a cardiogram.

Note that in this problem the checker checks your answer taking spaces into consideration. Do not print any extra characters. Remember that the wrong answer to the first pretest doesn't give you
 a penalty.




Sample test(s)




input
5
3 1 2 5 1




output
 / \ / \ / \ / \ / \\ / 



input
3
1 5 1




output
 / \\\\\ / 





C:这题超萌的~~~ 

就是让你画一个折线图，每ai个数方向转一次

知道宽度是ai的求和，高度是max-min，以及转向的时候y坐标是不动的，就行了。



```cpp
#include <map>
#include <stack>
#include <queue>
#include <cmath> 
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

int cmp(const void *a, const void *b)
{
	return(*(int *)a-*(int *)b);
}

int rec[1001];
char graph[1001][1001];

int main()
{
	memset(rec,0,sizeof(rec));
	memset(graph,' ',sizeof(graph));
	int cases=0,max=0,min=0,now=0,width=0;
	scanf("%d",&cases);
	for(int _case=0;_case<cases;_case++)
	{
		scanf("%d",&rec[_case]);
			if(_case%2==0) now+=rec[_case];
			else now-=rec[_case];
		width+=rec[_case];
			if(now>max) max=now;
			else if(now<min) min=now;
	}
	int height=max-min;
	//graph is a width_Row x height_Col picture
	
	int record=0,count=rec[0],ypos=max-1,dir=0;
	char sig;
	for(int x=0;x<width;x++)
	{
		if(record%2==0)sig='/';
		else sig='\\';
		graph[ypos][x]=sig;
		count--;
		if(count==0)
		{
			record++;
			count=rec[record];
		}
		else
		{
			if(record%2==0) ypos--;
			else ypos++;
		}
	}
	
	for(int _i=0;_i<height;_i++)
	{
		for(int _j=0;_j<width;_j++)
		{
			printf("%c",graph[_i][_j]);
		}
		if(_i!=height-1)printf("\n");
	}
	
	return 0;
}
```










