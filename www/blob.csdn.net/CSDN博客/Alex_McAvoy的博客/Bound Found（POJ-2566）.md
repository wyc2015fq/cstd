# Bound Found（POJ-2566） - Alex_McAvoy的博客 - CSDN博客





2019年02月17日 19:22:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[POJ																[基础算法——尺取法](https://blog.csdn.net/u011815404/article/category/8676200)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Signals of most probably extra-terrestrial origin have been received and digitalized by The Aeronautic and Space Administration (that must be going through a defiant phase: "But I want to use feet, not meters!"). Each signal seems to come in two parts: a sequence of n integer values and a non-negative integer t. We'll not go into details, but researchers found out that a signal encodes two integer values. These can be found as the lower and upper bound of a subrange of the sequence whose absolute value of its sum is closest to t. 

You are given the sequence of n integers and the non-negative target t. You are to find a non-empty range of the sequence (i.e. a continuous subsequence) and output its lower index l and its upper index u. The absolute value of the sum of the values of the sequence from the l-th to the u-th element (inclusive) must be at least as close to t as the absolute value of the sum of any other non-empty range.

# Input

The input file contains several test cases. Each test case starts with two numbers n and k. Input is terminated by n=k=0. Otherwise, 1<=n<=100000 and there follow n integers with absolute values <=10000 which constitute the sequence. Then follow k queries for this sequence. Each query is a target t with 0<=t<=1000000000.

# Output

For each query output 3 numbers on a line: some closest absolute sum and the lower and upper indices of some range where this absolute sum is achieved. Possible indices start with 1 and go up to n.

# **Sample Input**

**5 1-10 -5 0 5 10310 2-9 8 -7 6 -5 4 -3 2 -1 05 1115 2-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -115 1000 0**

# Sample Output

**5 4 45 2 89 1 115 1 1515 1 15**


题意：给出 n 个数字以及 k 个 s 值，求区间总和的绝对值中最接近 s 的一个区间，要求输出总和的绝对值与左、右端点

思路：尺取法

使用尺取法要求有单调性，但总和的绝对值与单调性并没有什么联系，因此可以人为的创造出一个单调性来使用尺取法

若对原数组直接进行排序，无法得到原来区间的和，当若将原数组求前缀和再进行排序，那么任选两个数的差都对应着一个区间和的绝对值，此外，由于要输出左右端点，因此可将前缀和与下标封装成结构体

于是，原问题就转化为了在一个升序区间中，取两个数作差，求最接近 s 的值

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
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Node{
    int sum;
    int pos;
    bool operator <(const Node &rhs)const{
        return sum<rhs.sum;
    }
}node[N];
int a[N];
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(m+n)){
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        node[0].sum=0;
        node[0].pos=0;
        for(int i=1;i<=n;i++){
            node[i].sum=node[i-1].sum+a[i];
            node[i].pos=i;
        }

        sort(node,node+1+n);
        while(m--){

            int t;
            scanf("%d",&t);

            int l=0,r=1;
            int ans=INF;
            int res,nodeL,nodeR;
            int temp;
            while(r<=n&&ans){
                temp=node[r].sum-node[l].sum;
                if(abs(temp-t)<ans){
                    nodeL=node[l].pos;
                    nodeR=node[r].pos;
                    res=temp;
                    ans=abs(temp-t);
                }
                if(temp>t)
                    l++;
                if(temp<t)
                    r++;
                if(l==r)
                    r++;
            }
            if(nodeL>nodeR)
                swap(nodeL,nodeR);

            printf("%d %d %d\n",res,nodeL+1,nodeR);
        }
    }
    return 0;
}
```






