# 第十五次codeforces竞技结束 #267 Div 2 - CD's Coding - CSDN博客





2014年09月29日 13:31:58[糖果天王](https://me.csdn.net/okcd00)阅读数：747








267这场打得不怎么样……

个人感觉就是……阶梯性好强……题和题之间跳跃性真的需要这么大么……













George has recently entered the BSUCP (Berland State University for Cool Programmers). George has a friend Alex who has also entered the university. Now they are moving into a dormitory.


George and Alex want to live in the same room. The dormitory has *n* rooms in total. At the moment the *i*-th
 room has *p**i* people
 living in it and the room can accommodate *q**i* people
 in total (*p**i* ≤ *q**i*).
 Your task is to count how many rooms has free place for both George and Alex.




Input


The first line contains a single integer *n*(1 ≤ *n* ≤ 100) —
 the number of rooms.


The *i*-th of the next *n* lines contains two integers *p**i* and *q**i*(0 ≤ *p**i* ≤ *q**i* ≤ 100) —
 the number of people who already live in the *i*-th room and the room's capacity.




Output


Print a single integer — the number of rooms where George and Alex can move in.




Sample test(s)




input
3
1 1
2 2
3 3




output
0




input
3
1 10
0 10
10 10




output
2












好萌好萌好萌的题哇~ 用来刚学会a+b problem的孩子们做最适合不过了~



有n个房间，每行是这个房间现在有几个人，总共能住几个人，问：有几个房间里有2个空位……

23333 代码如下

### Code：



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

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	int n=0;
	cin>>n;
	int cnt=0;
	for(int i=1;i<=n;i++)
	{
		int a,b;
		scanf("%d%d",&a,&b);
		if(b-a>=2)cnt++;
	}
	cout<<cnt;
	return 0;
}
```









After you had helped George and Alex to move in the dorm, they went to help their friend Fedor play a new computer game «Call of Soldiers 3».


The game has (*m* + 1) players and *n* types of soldiers
 in total. Players «Call of Soldiers 3» are numbered form 1 to (*m* + 1).
 Types of soldiers are numbered from 0 to *n* - 1. Each player
 has an army. Army of the *i*-th player can be described by non-negative integer *x**i*.
 Consider binary representation of *x**i*:
 if the *j*-th bit of number *x**i* equal
 to one, then the army of the *i*-th player has soldiers of the *j*-th
 type.


Fedor is the (*m* + 1)-th player of the game. He assume that two players can become friends if their armies differ in at most *k* types
 of soldiers (in other words, binary representations of the corresponding numbers differ in at most *k* bits). Help Fedor and count how many players can become
 his friends.




Input


The first line contains three integers *n*, *m*, *k*(1 ≤ *k* ≤ *n* ≤ 20; 1 ≤ *m* ≤ 1000).


The *i*-th of the next (*m* + 1) lines contains a single
 integer *x**i*(1 ≤ *x**i* ≤ 2*n* - 1),
 that describes the *i*-th player's army. We remind you that Fedor is the (*m* + 1)-th
 player.




Output


Print a single integer — the number of Fedor's potential friends.




Sample test(s)




input
7 3 1
8
5
111
17




output
0




input
3 3 3
1
2
3
4




output
3















不知道孩纸们知不知道这个运算符 :' ^ '

按位异或，如果这一位上的二进制位相同则为0，不同则为1.

这题的意思是有这么多个玩家，他们用二进制表示他们有哪些种类的兵力，最后一行是菲德尔，问有多少个人和他的兵种差距不大于k种。

那么……就直接用菲德尔（人）的兵种那个数字和别人异或一下看看有几个1咯~

代码如下：

### Code



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

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int army[1024];

int cnt_s(int a)
{
	int cnt=0,	tmp=a;
	while(tmp!=0)
	{
		cnt+=tmp%2;
		tmp/=2;
	} 
	return cnt;
}

int main()
{
	int n,m,k;
	cin>>n>>m>>k;
	memset(army,0,sizeof army); 
	for(int i=1;i<=m+1;i++)
		scanf("%d",&army[i]);
	int cnt=0;
	for(int i=1;i<=m;i++)
		if(cnt_s(army[i]^army[m+1])<=k)cnt++;
	cout<<cnt;
	return 0;
}
```









The new ITone 6 has been released recently and George got really keen to buy it. Unfortunately, he didn't have enough money, so George was going to work as a programmer. Now he faced the following problem at the work.


Given a sequence of *n* integers *p*1, *p*2, ..., *p**n*.
 You are to choose *k* pairs of integers:


[*l*1, *r*1], [*l*2, *r*2], ..., [*l**k*, *r**k*] (1 ≤ *l*1 ≤ *r*1 < *l*2 ≤ *r*2 < ... < *l**k* ≤ *r**k* ≤ *n*; *r**i* - *l**i* + 1 = *m*), 

in such a way that the value of sum ![](http://espresso.codeforces.com/56a872d240fd0705aaee49aa3763026a40df8f6c.png) is
 maximal possible. Help George to cope with the task.




Input


The first line contains three integers *n*, *m* and *k*(1 ≤ (*m* × *k*) ≤ *n* ≤ 5000).
 The second line contains *n* integers *p*1, *p*2, ..., *p**n*(0 ≤ *p**i* ≤ 109).




Output


Print an integer in a single line — the maximum possible value of sum.




Sample test(s)




input
5 2 1
1 2 3 4 5




output
9




input
7 1 3
2 10 7 18 5 33 0




output
61












题意是，有这么多数字，每次挑取连续的m个数，挑k次，这么多数字的和要求最大，每个数最多只允许被挑到一次



先获得每一个数的sum数组（它及它之前的所有数字之和）

然后通过sum[i]-sum[i-m]获得每一个m连数组的和

dp获得结果




代码如下：

### Code：



```cpp
#include<stdio.h>
#include<string.h>
typedef __int64 ll;
ll pre[5005],cur[5005],sum[5005],a[5005];
int n,m,k;

int main(void){
	int i,j;
	while(~scanf("%d%d%d",&n,&m,&k)){
		sum[0]=0;
		for(i=1;i<=n;i++){
			scanf("%I64d",&a[i]);
			sum[i]=sum[i-1]+a[i];
		}
		memset(pre,-1,sizeof(pre));
		for(i=m;i<=n;i++)
			pre[i]=cur[i]=sum[i]-sum[i-m];
		for(i=2;i<=k;i++){
			memset(cur,0,sizeof(cur));
			ll temp=0;
			for(j=m;j+m<=n;j++){
				if(pre[j]==-1) continue;
				if(pre[j]>temp) temp=pre[j];
				cur[j+m]=temp+sum[j+m]-sum[j];
			}
			memcpy(pre,cur,sizeof(pre));
		}
		ll ans=0;
		for(i=1;i<=n;i++) if(cur[i]>ans) ans=cur[i];
		printf("%I64d\n",ans);
	}
	return 0;
}
```

















