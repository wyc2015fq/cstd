# Internet Service Providers（LightOJ-1275） - Alex_McAvoy的博客 - CSDN博客





2019年02月13日 23:28:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[LightOJ																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

A group of N Internet Service Provider companies (ISPs) use a private communication channel that has a maximum capacity of C traffic units per second. Each company transfers T traffic units per second through the channel and gets a profit that is directly proportional to the factor T(C - T*N). The problem is to compute the smallest value of T that maximizes the total profit the N ISPs can get from using the channel. Notice that N, C, T, and the optimal T are integer numbers.

# Input

Input starts with an integer T (≤ 20), denoting the number of test cases.

Each case starts with a line containing two integers N and C (0 ≤ N, C ≤ 109).

# Output

For each case, print the case number and the minimum possible value of T that maximizes the total profit. The result should be an integer.

# Examples

**Input**

6

1 0

0 1

4 3

2 8

3 27

25 1000000000

**Output**

Case 1: 0

Case 2: 0

Case 3: 0

Case 4: 2

Case 5: 4

Case 6: 20000000


题意：t 组数据，每组给出 N、C 两个值，对于函数 T(C - T*N)，现要使函数最大，求 T 的最小值

思路：解一元二次方程

设 x=T，则 ![y = x(C-xN)](https://private.codecogs.com/gif.latex?y%20%3D%20x%28C-xN%29)，当给定 N、C 时，求取 x 的坐标，使得 y 最大

故有：对 ![y=-N*x^2+C*x](https://private.codecogs.com/gif.latex?y%3D-N*x%5E2&plus;C*x)，求最大值，对于一元二次方程，当 ![x=-\frac{b}{2a}=\frac{c}{2N}](https://private.codecogs.com/gif.latex?x%3D-%5Cfrac%7Bb%7D%7B2a%7D%3D%5Cfrac%7Bc%7D%7B2N%7D) 时，有最值

但由于 x 要求是整数，因此需要多计算 x 前、后的值，然后进行比较

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
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=1e9+7;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
LL max(LL a,LL b,LL c){
    LL maxx;
    maxx=a>b?a:b;
    maxx=maxx>c?maxx:c;
    return maxx;
}
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        LL n,c;
        scanf("%lld%lld",&n,&c);

        if(n==0||c==0)
            printf("Case %d: 0\n",Case++);
        else{
            LL x1=c/(2*n);//最值点
            LL x2=x1+1;//后一个值
            LL x3=x1-1;//前一个值

            LL res1=x1*(c-x1*n);
            LL res2=x2*(c-x2*n);
            LL res3=x3*(c-x3*n);

            LL maxx=max(res1,res2,res3);
            if(maxx==res1)
                printf("Case %d: %d\n",Case++,x1);
            else if(maxx==res2)
                printf("Case %d: %d\n",Case++,x2);
            else if(maxx==res3)
                printf("Case %d: %d\n",Case++,x3);
        }
    }
    return 0;
}
```






