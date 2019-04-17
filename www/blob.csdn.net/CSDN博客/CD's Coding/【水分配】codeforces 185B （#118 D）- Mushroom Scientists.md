# 【水分配】codeforces 185B （#118 D）- Mushroom Scientists - CD's Coding - CSDN博客





2014年08月03日 15:56:14[糖果天王](https://me.csdn.net/okcd00)阅读数：743标签：[codeforces																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)







这是我有史以来过的第一道最不明所以的D题

我居然也能给这道题打上水题的标签……简直把自己帅到了~

所以专门给这道题写一个题解：







As you very well know, the whole Universe traditionally uses three-dimensional Cartesian system of coordinates. In this system each point corresponds to three real coordinates(*x*, *y*, *z*). In this coordinate
 system, the distance between the center of the Universe and the point is calculated by the following formula:![](http://espresso.codeforces.com/22cac0bdb059f63ba2e16e3aabb38be91c9a9e6a.png). Mushroom scientists
 that work for the Great Mushroom King think that the Universe isn't exactly right and the distance from the center of the Universe to a point equals
*x**a*·*y**b*·*z**c*.

To test the metric of mushroom scientists, the usual scientists offered them a task: find such*x*, *y*, *z*(0 ≤ *x*, *y*, *z*; *x* + *y* + *z* ≤ *S*), that the distance between the center of the Universe and the point(*x*, *y*, *z*) is
 maximum possible in the metric of mushroom scientists. The mushroom scientists aren't good at maths, so they commissioned you to do the task.

Note that in this problem, it is considered that 00 = 1.



Input

The first line contains a single integer *S*(1 ≤ *S* ≤ 103) — the maximum sum of coordinates of the sought point.

The second line contains three space-separated integers *a*, *b*, *c*(0 ≤ *a*, *b*, *c* ≤ 103) — the numbers that describe the metric of mushroom scientists.



Output

Print three real numbers — the coordinates of the point that reaches maximum value in the metrics of mushroom scientists. If there are multiple answers, print any of them that meets the limitations.

A natural logarithm of distance from the center of the Universe to the given point in the metric of mushroom scientists shouldn't differ from the natural logarithm of the maximum distance by more than10 - 6.
 We think that*ln*(0) =  - ∞.



Sample test(s)



Input
3
1 1 1



Output
1.0 1.0 1.0



Input
3
2 0 0



Output
3.0 0.0 0.0









我写的时候真心不知道这题题意是怎么个意思！！！

有能三遍之内读懂的大神请让我膜拜下……

当时我们几个人有几个不同的解释

1）令这两个式子相等的，和不超过S的x,y,z分别为多少

2）令第一个式子最大的，和不超过S的x,y,z分别为多少

3）令第二个式子最大的，和不超过S的x,y,z分别为多少 （我是这个理解）

4）令第二个式子的log值最大的，和不超过S的x,y,z分别为多少




吾辈为了枚举题意，尝试了一发按照比例分配S的代码，过了你敢信？……

如果a,b,c都为0，把S全都给a，然后bc为0，反之按比例给abc分配即可。




实际上——

若a,b,c中有一个为0，0的0次方为1，非0次方为0，那肯定得给他分配0啊。

非零的情况下，当指数按照底数的比例分配的时候，可以获得最大的乘积，这个可以通过斜率啊、微分啊什么的都可以证明的出来




Code：



```cpp
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;
int main()
{
    int S,a,b,c;    cin>>S>>a>>b>>c;
    if( a+b+c==0 ){ cout<<S<<" 0 0"; return 0;}
    	double x = double(a*S)/(a+b+c);
    	double y = double(b*S)/(a+b+c);
		double z = double(c*S)/(a+b+c);
    printf("%.18lf %.18lf %.18lf", x, y, z);
    return 0;
}
```

























