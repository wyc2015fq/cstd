# Square Number（HDU-2281） - Alex_McAvoy的博客 - CSDN博客





2019年03月21日 20:06:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：22
个人分类：[HDU																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Find the biggest integer n (1 <= n <= N) and an integer x to make them satisfy 

![](https://img-blog.csdnimg.cn/20190321191939320.jpg)

# Input

The input consists of several test cases. Each test case contains a integer N, 1<=N<=10^18.The input ends with N = 0.

# Output

In one line for each case, output two integers n and x you have found.

# Sample Input

1

2

0

# Sample **Output**

1 1

1 1


题意：输入一个数 N，找到满足上式中的 n、x，且 n<=N

思路：

将右边式子的分子求和化简，有：![x^2=\frac{(n+1)(2*n+1)}{6}](https://private.codecogs.com/gif.latex?x%5E2%3D%5Cfrac%7B%28n&plus;1%29%282*n&plus;1%29%7D%7B6%7D)

移项化简，有：![(4n+3)^2 - 48*x^2 = 1](https://private.codecogs.com/gif.latex?%284n&plus;3%29%5E2%20-%2048*x%5E2%20%3D%201)

满足佩尔方程的形式，套入公式求解即可。

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
#define E 1e-9
#define INF 0x3f3f3f3f
//#define LL long long
const int MOD=10007;
const int N=100+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

long long n[N],x[N];
vector<long long> nn,xx;
void init(){
    n[0]=7LL,x[0]=1LL;
    nn.push_back(1LL),xx.push_back(1LL);
    for(int i=1;;i++){
        n[i]=7LL*n[i-1]+48LL*x[i-1];
        x[i]=n[i-1]+7LL*x[i-1];
        if(n[i]<0)
            break;
        if((n[i]-3)%4==0){
            nn.push_back((n[i]-3)/4);
            xx.push_back(x[i]);
        }
    }
    nn.push_back((long long)1e18+5);
}
int main(){
    init();
    long long k;
    while(scanf("%lld",&k)!=EOF&&k){
        for(int i=0;i<nn.size();i++){
            if(k<nn[i]){
                printf("%lld %lld\n",nn[i-1],xx[i-1]);
                break;
            }
        }
    }
    return 0;
}
```






