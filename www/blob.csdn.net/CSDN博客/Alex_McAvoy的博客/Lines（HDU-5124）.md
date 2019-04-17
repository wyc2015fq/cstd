# Lines（HDU-5124） - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 20:26:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：71
个人分类：[HDU																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

John has several lines. The lines are covered on the X axis. Let A is a point which is covered by the most lines. John wants to know how many lines cover A.

# Input

The first line contains a single integer T(1≤T≤100)(the data for N>100 less than 1 cases),indicating the number of test cases. 

Each test case begins with an integer N(1≤N≤105),indicating the number of lines. 

Next N lines contains two integers Xi and Yi(1≤Xi≤Yi≤109),describing a line.

# Output

For each case, output an integer means how many lines cover A.

# **Sample Input**

**251 2 2 22 43 45 100051 12 23 34 45 5**

# Sample Output

**31**


题意：t 组数据，每组给出 n 个区间 [xi,yi]，求这些区间段所覆盖的最多的点

思路： 对于一个区间 [xi,yi]，将其分为两个端点 xi、yi，在 xi 端点时该点会新加入一条新的线段，而对于 yi+1 的点，在该点时会减少一条线段，因此可以将给出的 2n 个端点进行排序，同时，令 xi 价值为 1，yi 价值为 -1，这样一来，问题就由区间覆盖转换你为了最大区间和，由于 1 一定在 -1 之前，因此问题又转换为了最大前缀和，寻找最大值即可

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
using namespace std;
int n;
pair<int,int> a[N*2];//由于将区间转为点，需要进行扩容
int main(){
    int t;
    scanf("%d",&t);

    while(t--){
        scanf("%d%",&n);

        n*=2;
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i].first);//左端点xi
            a[i].second=1;//左端点赋值

            i++;
            scanf("%d",&a[i].first);//右端点yi
            a[i].first++;//右端点后移一位
            a[i].second=-1;//右端点赋值
        }

        sort(a+1,a+1+n);//默认对第一个元素升序排序

        int maxx=-INF;
        int sum=0;
        for(int i=1;i<=n;i++){
            sum+=a[i].second;//求前缀和
            maxx=max(maxx,sum);///记录最大值
        }

        printf("%d\n",maxx);
    }
    return 0;
}
```






