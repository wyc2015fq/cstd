# Rightmost Digit（HDU-1061） - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 18:51:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[HDU																[数论——快速幂与同余式定理](https://blog.csdn.net/u011815404/article/category/8796909)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Given a positive integer N, you should output the most right digit of N^N. 

# Input

The input contains several test cases. The first line of the input is a single integer T which is the number of test cases. T test cases follow. 

Each test case contains a single positive integer N(1<=N<=1,000,000,000). 

# Output

For each test case, you should output the rightmost digit of N^N. 

# **Sample Input**

**234**

# Sample Output

**76**


题意：t 组数据，每组给出一个数 n，求 n^n 的个位数

思路：n^n 相当于 n 个 n 相乘，直接使用快速幂即可，而求 n^n 的个位数相当于在进行快速幂的 mod 10 

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
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=10;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int quickPow(int a,int b){
    int res=1;
    a%=MOD;
    while(b>0){
        if(b&1)
            res=(res*a)%MOD;
        b>>=1;
        a=(a*a)%MOD;
    }
    return res;
}
int main(){
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        cout<<quickPow(n,n)<<endl;
    }
    return 0;
}
```






