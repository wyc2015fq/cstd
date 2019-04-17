# Petya and Origami（CF-1080A） - Alex_McAvoy的博客 - CSDN博客





2018年12月04日 17:35:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43
个人分类：[CodeForces																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Petya is having a party soon, and he has decided to invite his nn friends.

He wants to make invitations in the form of origami. For each invitation, he needs two red sheets, five green sheets, and eight blue sheets. The store sells an infinite number of notebooks of each color, but each notebook consists of only one color with kk sheets. That is, each notebook contains kk sheets of either red, green, or blue.

Find the minimum number of notebooks that Petya needs to buy to invite all nn of his friends.

# Input

The first line contains two integers nn and kk (1≤n,k≤1081≤n,k≤108) — the number of Petya's friends and the number of sheets in each notebook respectively.

# Output

Print one number — the minimum number of notebooks that Petya needs to buy.

# Examples

**Input**

3 5

**Output**

10

**Input**

15 6

**Output**

38

————————————————————————————————————————————

题意：n 个人，每个人要 2 张红纸，5 张绿纸，8 张蓝纸，现在去买本子，每个本子由同一种颜色的 k 张纸构成，问最少要买多少个本子

思路：简单模拟，注意向上取整即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
int main(){
    int n,k;
    while(scanf("%d%d",&n,&k)!=EOF&&(n+k)){
        int red=2*n;
        int green=5*n;
        int blue=8*n;

        int res=0;
        if(red%k)
            res+=(red/k)+1;
        else
            res+=(red/k);

        if(green%k)
            res+=(green/k)+1;
        else
            res+=(green/k);

        if(blue%k)
            res+=(blue/k)+1;
        else
            res+=(blue/k);

        printf("%d\n",res);
    }
    return 0;
}
```






