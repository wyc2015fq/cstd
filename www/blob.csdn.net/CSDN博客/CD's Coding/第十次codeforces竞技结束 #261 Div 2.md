# 第十次codeforces竞技结束 #261 Div 2 - CD's Coding - CSDN博客





2014年08月16日 03:14:10[糖果天王](https://me.csdn.net/okcd00)阅读数：632








COUT我恨你……

大家注意哦，printf比cout快很多……

这次原先C题我出的是最快的，除去超强的qwzeng学长我就是第一名了，然后FinalTest跪了，就变成只有2题AC了……就倒数了，哭……无奈了……

当然这里贴给大家的是对的啦，自己在cf上提交ac过了……









## A. Pashmak and Garden



time limit per test

1 second



memory limit per test

256 megabytes



input

standard input



output

standard output


Pashmak has fallen in love with an attractive girl called Parmida since one year ago...

Today, Pashmak set up a meeting with his partner in a romantic garden. Unfortunately, Pashmak has forgotten where the garden is. But he remembers that the garden looks like a square
 with sides parallel to the coordinate axes. He also remembers that there is exactly one tree on each vertex of the square. Now, Pashmak knows the position of only two of the trees. Help him to find the position of two remaining ones.




Input

The first line contains four space-separated*x*1, *y*1, *x*2, *y*2( - 100 ≤ *x*1, *y*1, *x*2, *y*2 ≤ 100)integers,
 where*x*1and*y*1are
 coordinates of the first tree and*x*2and*y*2are
 coordinates of the second tree. It's guaranteed that the given points are distinct.




Output

If there is no solution to the problem, print-1. Otherwise print
 four space-separated integers*x*3, *y*3, *x*4, *y*4that
 correspond to the coordinates of the two other trees. If there are several solutions you can output any of them.

Note that*x*3, *y*3, *x*4, *y*4must
 be in the range( - 1000 ≤ *x*3, *y*3, *x*4, *y*4 ≤ 1000).




Sample test(s)




input
0 0 0 1




output
1 0 1 1




input
0 0 1 1




output
0 1 1 0




input
0 0 1 2




output
-1









A题的意思呢是给你两个点的坐标，问这两个能否作为一个正方形的两个点，如果可以，输出正方形的另两个点

所以我们就这么判断：

1）如果给的两个点连线是竖直的（x1==x2）

2）如果给的两个点连线是水平的（y1==y2）

3）如果两个点是对角点且可以构成正方形（两个差值绝对值相等）

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

int main()
{
	int x1,y1,x2,y2,x3,y3,x4,y4,l;
	cin>>x1>>y1>>x2>>y2;
	if(x1==x2)
	{
		l=abs(y1-y2);
		x3=x4=(x1-l>=-1000?(x1-l):(x1+l));
		if(x3==x1+l && x1+l>1000)cout<<-1<<endl;
		else cout<<x3<<" "<<y1<<" "<<x4<<" "<<y2<<endl;
	}
	else if(y1==y2)
	{
		l=abs(x1-x2);
		y3=y4=(y1-l>=-1000?(y1-l):(y1+l));
		if(y3==y1+l && y1+l>1000)cout<<-1<<endl;
		else cout<<x1<<" "<<y3<<" "<<x2<<" "<<y4<<endl;
	}
	else
	{
		if(abs(x1-x2)!=abs(y1-y2))cout<<-1<<endl;
		else cout<<x1<<" "<<y2<<" "<<x2<<" "<<y1<<endl;
	}
	return 0;
}
```




## B. Pashmak and Flowers



time limit per test

1 second



memory limit per test

256 megabytes



input

standard input



output

standard output


Pashmak decided to give Parmida a pair of flowers from the garden. There are*n*flowers
 in the garden and the*i*-th of them has a beauty number*b**i*.
 Parmida is a very strange girl so she doesn't want to have the two most beautiful flowers necessarily. She wants to have those pairs of flowers that their beauty difference is maximal possible!

Your task is to write a program which calculates two things:
- 
The maximum beauty difference of flowers that Pashmak can give to Parmida.
- 
The number of ways that Pashmak can pick the flowers. Two ways are considered different if and only if there is at least one flower that is chosen in the first way and not chosen in the second way.




Input

The first line of the input contains*n*(2 ≤ *n* ≤ 2·105).
 In the next line there are*n*space-separated integers*b*1,*b*2,
 ...,*b**n*(1 ≤ *b**i* ≤ 109).




Output

The only line of output should contain two integers. The maximum beauty difference and the number of ways this may happen, respectively.




Sample test(s)




input
2
1 2




output
1 1



input
3
1 4 5




output
4 1



input
5
3 1 2 3 1




output
2 4





Note

In the third sample the maximum beauty difference is2and
 there are4ways to do this:
- 
choosing the first and the second flowers;
- 
choosing the first and the fifth flowers;
- 
choosing the fourth and the second flowers;
- 
choosing the fourth and the fifth flowers.





这题的意思是问最大值和最小值的差，以及最大值&最小值的对（Pair）有多少

咱们遍历一遍找出最大值和最小值，同时输出有多少个，然后差就有了，数对的个数不就是两个cnt相乘，多简单~

不好意思，WA at 5

为啥呢？ 哈哈 如果所有值都是一样的咋办呢对不对^_^ ~ 

是C（n,2）~~

所以代码如下

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
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

int main()
{
	int n;	cin>>n;
	int max=0,min=2147483646,max_cnt=0,min_cnt=0,now;
	for(int ni=0;ni<n;ni++)
	{
		cin>>now;
		if(now>max)	max=now,max_cnt=0;
		if(max==now) max_cnt++;
		if(now<min) min=now,min_cnt=0;
		if(min==now) min_cnt++;
	} 
	if(max==min) cout<<0<<" "<<(ll)max_cnt*(ll)(min_cnt-1)/2<<endl;
	else cout<<(max-min)<<" "<<(ll)max_cnt*(ll)min_cnt<<endl;
	return 0;
}
```




## C. Pashmak and Buses



time limit per test

1 second



memory limit per test

256 megabytes



input

standard input



output

standard output


Recently Pashmak has been employed in a transportation company. The company has*k*buses
 and has a contract with a school which has*n*students. The school planned to take the students to*d*different
 places for*d*days (each day in one place). Each day the company provides all the buses for the trip.
 Pashmak has to arrange the students in the buses. He wants to arrange the students in a way that no two students become close friends. In his ridiculous idea, two students will become close friends if and only if they are in the same buses for all*d*days.

Please help Pashmak with his weird idea. Assume that each bus has an unlimited capacity.




Input

The first line of input contains three space-separated integers*n*, *k*, *d*(1 ≤ *n*, *d* ≤ 1000; 1 ≤ *k* ≤ 109).




Output

If there is no valid arrangement just print-1. Otherwise print*d*lines,
 in each of them print*n*integers. The*j*-th
 integer of the*i*-th line shows which bus the*j*-th
 student has to take on the*i*-th day. You can assume that the buses are numbered from1to*k*.




Sample test(s)




input
3 2 2




output
1 1 2 
1 2 1 




input
3 2 1




output
-1






Note

Note that two students become close friends only if they share a bus each day. But the bus they share can differ from day to day.





这里有一个特别不愿意成人之美的人~ 就是说不允许任何俩小盆友每天都在同一辆车上。

问能不能做到，不能输出-1，能的话输出每一天每个人坐的车的编号（1-k）

我们不横着看，咱竖着看，是不是能发现什么呢^_^

比如样例一：

第一个小盆友坐车的编号是 11

第二个小盆友是12

第三个小盆友是21

（其实可以再来个第四个小盆友是22）

竖着看的话是不是分分钟能发现这个问题的等效问题呢？

既然不能有两个小盆友每天的车号都一致，即每两个小盆友竖着看都不能是同一个序列对不对。

这个序列是每一位不超过k的数列的序列，那我们就把这个当成一个k+1（车数加一）进制的d（天数）位数能否表示n个不同的数值呢~

可以的话是不是从1111..111到kkkk...kkk能表示就是可行，不能就是不行哇~

在那之前，咱们通过judge函数（ll来存，一旦d自乘超过n就说明可行，k都用完了还没超过就输出-1）来直接预判剪枝大大降低时间~

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
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
int n,k,d,depth;	
int stu[1008][1008];  //stu(d,n)=no.k

int judge()
{
	ll tmp=k,tmpd=d;
	while(1)
	{
		if(tmp>=n)return (int)d-(int)tmpd+1;
		if(tmpd==1)return -1;
		tmpd--;
		tmp*=k;
	}
}

void init()
{
	memset(stu,0,sizeof stu);
	for(int i=1;i<=depth;i++)		stu[i][1]=1;
	for(int j=2;j<=n;j++)
	{
		stu[depth][j]+=1;
		for(int i=depth;i>=1;i--)
		{
			stu[i][j]+=(stu[i][j-1]);
			if(stu[i][j]>k)	stu[i][j]=stu[i][j]%k,stu[i-1][j]++;
		}
	}
}

int main()
{
	cin>>n>>k>>d;
	depth=judge();
	if(depth==-1){cout<<-1<<endl;	return 0;}
	else init();
	
	for(int i=1;i<=depth;i++)
	{
		cout<<stu[i][1];
		for(int j=2;j<=n;j++)
		{
			cout<<" "<<stu[i][j];
		}
		cout<<endl;
	}
	for(int i=depth+1;i<=d;i++)
	{
		cout<<stu[i][1]+1;
		for(int j=2;j<=n;j++)
		{
			cout<<" "<<stu[i][j]+1;
		}
		cout<<endl;
	}
	return 0;
}
```








﻿﻿



