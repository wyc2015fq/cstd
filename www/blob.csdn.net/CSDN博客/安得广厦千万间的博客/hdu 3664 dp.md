# hdu 3664 dp - 安得广厦千万间的博客 - CSDN博客





2018年05月26日 13:47:14[独-](https://me.csdn.net/qq_41713256)阅读数：23








题目网址：http://acm.hdu.edu.cn/showproblem.php?pid=3664

题目大意：

一个全排列的E值等于 a【i】>i的数量，求n的全排列中E值等于k的方案数。

首先要利用好全排列这个约束。

dp【n】【k】表示：n的全排列中E值等于k的方案数。


- 把n放在第n个位置，那么 dp【n】【k】=dp【n-1】【k】；
- n不放在第n个位置，E值就会加一。那么把n放在第m个位置其中（a【m】<=m），把a【m】放在第n个位置。这样的m有 n-k个。那么 dp【n】【k】=dp【n-1】【k-1】*（n-k）；
- 那么把n放在第m个位置其中（a【m】>m）这样的m有 k个。那么 dp【n】【k】=dp【n-1】【k】*k。

所以dp【n】【k】=dp【n-1】【k】+dp【n-1】【k-1】*（n-k）+dp【n-1】【k】*k；

最后就是边界的细节问题。

AC代码：


#include<cstdio>
#include<cmath>
#include<iostream>
#include<string>
#include<cstring>
#include<queue>
#include<stack>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<set>
#include<map>
**using namespace** std**;typedef long long** ll**;const** ll inf**=**1e9**+**7**;**
ll dp**[**1010**][**1010**];int main(){**
    memset**(**dp**,**0**,sizeof(**dp**));    int** i**,**j**,**n**,**k**;**
    dp**[**1**][**0**]=**1**;    for(**i**=**1**;**i**<=**1000**;**i**++)    {**
        dp**[**i**][**0**]=**1**;        for(**j**=**1**;**j**<**i**;**j**++)        {**
            dp**[**i**][**j**]=(**dp**[**i**-**1**][**j**]+**dp**[**i**-**1**][**j**]***j**+**dp**[**i**-**1**][**j**-**1**]*(**i**-**j**))%**inf**;        }    }    while(**cin**>>**n**>>**k**)    {**
        cout**<<**dp**[**n**][**k**]<<**endl**;    }    return** 0**;}**



