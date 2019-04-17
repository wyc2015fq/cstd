# 第三次codeforces竞技结束 #254 Div 2 - CD's Coding - CSDN博客





2014年07月07日 10:56:14[糖果天王](https://me.csdn.net/okcd00)阅读数：567








掉到绿名喽……谁教我这次作死一定要做D也就是Div1的B题……唉叹口气……




DZY loves chessboard, and he enjoys playing with it.


He has a chessboard of *n* rows and *m* columns. Some
 cells of the chessboard are bad, others are good. For every good cell, DZY wants to put a chessman on it. Each chessman is either white or black. After putting all chessmen, DZY wants that no two chessmen with the same color are on two adjacent cells. Two
 cells are adjacent if and only if they share a common edge.


You task is to find any suitable placement of chessmen on the given chessboard.




Input


The first line contains two space-separated integers *n* and *m*(1 ≤ *n*, *m* ≤ 100).


Each of the next *n* lines contains a string of *m* characters:
 the *j*-th character of the *i*-th string is either
 "." or "-". A "."
 means that the corresponding cell (in the *i*-th row and the *j*-th
 column) is good, while a "-" means it is bad.




Output


Output must contain *n* lines, each line must contain a string of *m* characters.
 The *j*-th character of the *i*-th string should be
 either "W", "B" or "-".
 Character "W" means the chessman on the cell is white, "B"
 means it is black, "-" means the cell is a bad cell.


If multiple answers exist, print any of them. It is guaranteed that at least one answer exists.




Sample test(s)




input
1 1
.




output
B




input
2 2
..
..




output
BW
WB




input
3 3
.-.
---
--.



output
B-B
---
--B





Note


In the first sample, DZY puts a single black chessman. Of course putting a white one is also OK.


In the second sample, all 4 cells are good. No two same chessmen share an edge in the sample output.


In the third sample, no good cells are adjacent. So you can just put 3 chessmen, no matter what their colors are.








这是一个水题，要求减号输出减号，点号输出黑白BW，相邻格子不让颜色一致，那么事先做一张国际象棋那样的格子表，遇到点输出，遇到减号输出减号即可。

5Min秒掉，但是……又忘记把Freopen删掉结果TLE At 1 了一次……

### Code：



```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
using namespace std;
char map[101][101];

int main()
{
	//freopen("in.txt","r",stdin);
	int n,m;
	scanf("%d%d",&n,&m);
	char huiche;
	scanf("%c",&huiche);
	//printf("%d %d\n",n,m); 中途改题 ？！！ 
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			char ch;
			scanf("%c",&ch);
			if(ch=='-')printf("-");
			else if(ch=='.')printf((i+j)%2==0?"B":"W");
			else printf("%c",ch);
		}
		if(i==n-1)return 0;
		else 
		{
			scanf("%c",&huiche);
			printf("\n");
		}
	}
	
	return 0;
}
```






DZY loves Fast Fourier Transformation, and he enjoys using it.


Fast Fourier Transformation is an algorithm used to calculate convolution. Specifically, if *a*, *b* and *c* are
 sequences with length *n*, which are indexed from 0 to *n* - 1,
 and
![](http://espresso.codeforces.com/954d85f8c44a768d0814c95b7202d2f2d8b5fc92.png)

We can calculate *c* fast using Fast Fourier Transformation.


DZY made a little change on this formula. Now
![](http://espresso.codeforces.com/09afa7cc3a89ee532dca56e03f335bb766d7af37.png)

To make things easier, *a* is a permutation of integers from 1 to *n*,
 and *b* is a sequence only containing 0 and 1.
 Given *a* and *b*, DZY needs your help to calculate *c*.


Because he is naughty, DZY provides a special way to get *a* and *b*.
 What you need is only three integers *n*, *d*, *x*.
 After getting them, use the code below to generate *a* and *b*.
//x is 64-bit variable;
function getNextX() {
    x = (x * 37 + 10007) % 1000000007;
    return x;
}
function initAB() {
    for(i = 0; i < n; i = i + 1){
        a[i] = i + 1;
    }
    for(i = 0; i < n; i = i + 1){
        swap(a[i], a[getNextX() % (i + 1)]);
    }
    for(i = 0; i < n; i = i + 1){
        if (i < d)
            b[i] = 1;
        else
            b[i] = 0;
    }
    for(i = 0; i < n; i = i + 1){
        swap(b[i], b[getNextX() % (i + 1)]);
    }
}



Operation x % y denotes remainder after division *x* by *y*.
 Function swap(x, y) swaps two values *x* and *y*.




Input


The only line of input contains three space-separated integers *n*, *d*, *x* (1 ≤ *d* ≤ *n* ≤ 100000; 0 ≤ *x* ≤ 1000000006).
 Because DZY is naughty, *x* can't be equal to 27777500.




Output


Output *n* lines, the *i*-th line should contain an integer *c**i* - 1.




Sample test(s)




input
3 1 1




output
1
3
2




input
5 4 2




output
2
2
4
5
5




input
5 4 3




output
5
5
5
5
4






Note


In the first sample, *a* is [1 3 2], *b* is [1
 0 0], so *c*0 = *max*(1·1) = 1, *c*1 = *max*(1·0, 3·1) = 3, *c*2 = *max*(1·0, 3·0, 2·1) = 2.


In the second sample, *a* is [2 1 4 5 3], *b* is [1
 1 1 0 1].


In the third sample, *a* is [5 2 1 4 3], *b* is [1
 1 1 1 0].





然后攻的是D题，以下为题目以及小数据代码（可惜cf没有小数据给半分这种，叹气）：（TLE At 9）

这题一个是要把他给的式子模拟出来以获得初始A和B的数组初值，然后找题求最大，下面模拟了一下，所以为什么说我是作死呢……

### Code1：



```cpp
#include <cmath>   
#include <vector>  
#include <cctype>  
#include <cstdio>  
#include <string>  
#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
#define MAXN 100001  
using namespace std;

typedef __int64 ll;
ll n,d,x,a[MAXN],b[MAXN],lst[MAXN];

ll getNextX() 
{
    x = (x * 37 + 10007) % 1000000007;
    return x;
}

void initAB() 
{
	ll i;
    for(i = 0; i < n; i++)        a[i] = i + 1;
    for(i = 0; i < n; i++)        
	{
		ll t = getNextX() % (i + 1);
		ll temp = a[i];
		a[i] = a[t];
		a[t] = temp;
	}//swap(a[i], a[getNextX() % (i + 1)]);
    for(i = 0; i < n; i++)
        if (i < d)      b[i] = 1;
        else            b[i] = 0;
    for(i = 0; i < n; i++)        
	{
		ll t = getNextX() % (i + 1);
		ll temp = b[i];
		b[i] = b[t];
		b[t] = temp;
	}//swap(b[i], b[getNextX() % (i + 1)]);
}

ll solve(ll k)
{
	memset(lst,0,sizeof lst);
	ll j,ans=0;
	ll p = 0;
	for(j = 0;j <= k;j++)
	{
		if (b[j] == 0)continue;//jianzhi
		else 
		{
			lst[p] = a[k-j];
			//cout<<p<<"\t"<<a[k-j]<<endl;
			p++;
		}
	}
	//sort(lst,lst+p);
	for(int ii=0;ii<=p;ii++)
	if(lst[ii]>ans)ans=lst[ii];
	return ans;
}

int main()
{
	ll k;
	memset(a,0,sizeof a);
	memset(b,0,sizeof b);
	memset(lst,0,sizeof lst);
	scanf("%I64d%I64d%I64d",&n,&d,&x);
	initAB();
	//for(k=0;k<n;k++)	printf("%I64d\t%I64d\n",a[k],b[k]);
	for(k=0;k<n;k++)
	{
		printf("%I64d",solve(k));
		if(k<n-1)printf("\n");
	}
	return 0;
}
```






### Code2：

之前剪枝剪的太少，跟没有一样，后来弄了多组答案之后发现，很多答案都是一片一片一样的，然后去找原因……然后……好吧……

从后向前批量刷c（就是答案数组）的值，算法时间在CF上百毫秒以内……

From [andrei14vl](http://codeforces.com/profile/andrei14vl)@Codeforces



```cpp
#include <iostream>
#include <algorithm>
using namespace std;

int i, n, d, a[100009], b[100009], c[100009], aPoz[100009];
long long x;

long long getNextX() 
{
    x = (x * 37 + 10007) % 1000000007;
    return x;
}

void initAB() 
{
    for(i = 0; i < n; i = i + 1)
        a[i] = i + 1;
    for(i = 0; i < n; i = i + 1)
        swap(a[i], a[getNextX() % (i + 1)]);
    for(i = 0; i < n; i = i + 1)
        if (i < d)      b[i] = 1;
        else            b[i] = 0;
    for(i = 0; i < n; i = i + 1)
        swap(b[i], b[getNextX() % (i + 1)]);
}

int main()
{
    cin >> n >> d >> x;
    initAB();
    
    for (int i = 0; i < n; ++i)
        aPoz[a[i]] = i + 1;
    int bcnt = -1;
    for (int i = 0; i < n; ++i)
        if (b[i])
            b[++bcnt] = i;
        
    int n2 = n; 
    for (int i = 100000; i > 0; --i)
        if (aPoz[i])
        {
            for (int j = 0; j <= bcnt; ++j)
            {
                if (aPoz[i] + b[j] > n2) break;
                if (!c[aPoz[i] + b[j]])
                    c[aPoz[i] + b[j]] = i;
            }
            while (c[n2]) --n2;
        }
    for (int i = 1; i <= n; ++i)
        cout << c[i] << endl;   
        
return 0;
}
```







