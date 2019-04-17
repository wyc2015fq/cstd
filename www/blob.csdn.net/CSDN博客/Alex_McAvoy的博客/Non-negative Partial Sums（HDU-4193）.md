# Non-negative Partial Sums（HDU-4193） - Alex_McAvoy的博客 - CSDN博客





2019年02月10日 19:19:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49
个人分类：[HDU																[数据结构——单调栈与单调队列](https://blog.csdn.net/u011815404/article/category/8661014)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

You are given a sequence of n numbers a 0,..., a n-1. A cyclic shift by k positions (0<=k<=n-1) results in the following sequence: a k a k+1,..., a n-1, a 0, a 1,..., a k-1. How many of the n cyclic shifts satisfy the condition that the sum of the fi rst i numbers is greater than or equal to zero for all i with 1<=i<=n?

# **Input**

Each test case consists of two lines. The first contains the number n (1<=n<=10 6), the number of integers in the sequence. The second contains n integers a 0,..., a n-1 (-1000<=a i<=1000) representing the sequence of numbers. The input will finish with a line containing 0.

# Output

For each test case, print one line with the number of cyclic shifts of the given sequence which satisfy the condition stated above.

# Sample Input

**32 2 13-1 1 11-10**

# Sample Output

**320**


题意：对于每组数据，每组给一个长度为 n 的循环序列，从 n 个不同位置开始，问有多少个序列的任意前 i 项和均大于或等于 0

思路：单调队列

首先要处理循环序列的问题，直接将原序列复制一遍接到末尾，这样扩大一倍即可，其次再处理求一段区间的和就有 sum[x~y] = sum[0~y] - sum(0~x)，因此只需要预处理 sum 数组就可得到任意一段区间的和，题目要找满足所有 Sum(i)>=0 的 n 长子序列，那么只要找出最小的 sum ，若他的值大于等于 0 的话，那么该序列就满足条件，个数加 1 即可，因此问题就转化成了用双向队列求区间内的 sum 的最小值

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
#define N 2000001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
struct Node{
    int num;//值
    int pos;//位置
}sum[N];//前缀和
int a[N];
int main(){
    for(int i=0;i<N;i++)//初始化位置
        sum[i].pos=i;

    int n;
    while(scanf("%d",&n)!=EOF&&n){
        deque<Node> D;

        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
            a[i+n]=a[i];//复制序列
        }

        //计算前缀和
        sum[0].num=a[0];
        for(int i=1;i<2*n-1;i++)
            sum[i].num=sum[i-1].num+a[i];

        for(int i=0;i<n-1;i++){//维护从0到n-1的序列
            //维护单调递减队列
            while(!D.empty()&&D.back().num>=sum[i].num)
                D.pop_back();
            D.push_back(sum[i]);

            //维护队首元素，删掉对求下一段最值无影响的元素
            while(!D.empty()&&sum[i].pos-D.front().pos>=n)
                D.pop_front();
        }

        int res=0;
        for(int i=n-1;i<2*n-1;i++){//维护从n-1到2n-1的序列
            //维护单调递减队列
            while(!D.empty()&&D.back().num>=sum[i].num)
                D.pop_back();
            D.push_back(sum[i]);

            //维护队首元素，删掉对求下一段最值无影响的元素
            while(!D.empty()&&sum[i].pos-D.front().pos>=n)
                D.pop_front();

            if(D.front().num-sum[i-n].num>=0)//满足区间和大于等于0
                res++;
        }

        printf("%d\n",res);
    }
    return 0;
}
```






