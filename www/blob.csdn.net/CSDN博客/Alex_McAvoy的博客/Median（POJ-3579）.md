# Median（POJ-3579） - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 20:03:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：64








> 
# Problem Description

Given N numbers, X1, X2, ... , XN, let us calculate the difference of every pair of numbers: ∣Xi - Xj∣ (1 ≤ i ＜ j ≤ N). We can get C(N,2) differences through this work, and now your task is to find the median of the differences as quickly as you can!

Note in this problem, the median is defined as the (m/2)-th  smallest number if m,the amount of the differences, is even. For example, you have to find the third smallest one in the case of m = 6.

# Input

The input consists of several test cases.

In each test case, N will be given in the first line. Then N numbers are given, representing X1, X2, ... , XN, ( Xi ≤ 1,000,000,000  3 ≤ N ≤ 1,00,000 )

# Output

For each test case, output the median in a separate line.

# Examples

**Input**

4

1 3 2 4

3

1 10 2

**Output**

1

8


题意：每组数据给出 n 个数，任意两个数之间的差的绝对值有 C(N,2)=(n-1)*n/2 种，要求输出这些数中间的那一个，若 C(N,2) 为偶数，中间那个数即为第 C(N,2)/2 小的数，若 C(N,2) 为奇数，中间那个数即为 第 (C(N,2)+1)/2 小的数

思路：n 是 100000，暴力的话时间复杂度能到 O(n^2) 左右，一定会超时，因此需要用二分来找

根据题意，两个数的差取了绝对值，可看作数组 a[n] 中，任意两个数 a[i]、a[j] 一定是大的减小的，故可以对数组先进行排序，然后根据差值来确定二分的范围：0~a[n]-a[1]，从而进行二分

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
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int n;
int a[N];
bool check(int x){
    int cnt=0;
    for(int i=0;i<n;i++){
        int left=i,right=n-1;
        while(left<=right){
            int mid=(left+right)>>1;
            if(a[mid]<a[i]+x)
                left=mid+1;
            else
                right=mid-1;
        }
        cnt+=left-i-1;
        //cnt+=upper_bound(a,a+n,a[i]+x)-a-i-1;
    }
    int m=n*(n-1)/2;//C(n,2)
    m=(m+1)/2;
    return cnt>=m;
}
int main(){
    while(scanf("%d",&n)!=EOF&&n){
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);
        sort(a,a+n);

        int left=0,right=a[n-1];
        while(left<=right){
            int mid=(left+right)/2;
            if(check(mid))
                right=mid-1;
            else
                left=mid+1;
        }

        printf("%d\n",right);
    }
    return 0;
}
```





