# HDU5532 Almost Sorted Array 最长上升子序列、暴力 - 紫芝的博客 - CSDN博客





2018年10月12日 14:37:22[紫芝](https://me.csdn.net/qq_40507857)阅读数：39








# Almost Sorted Array

We are all familiar with sorting algorithms: quick sort, merge sort, heap sort, insertion sort, selection sort, bubble sort, etc. But sometimes it is an overkill to use these algorithms for an almost sorted array. 


We say an array is sorted if its elements are in non-decreasing order or non-increasing order. We say an array is almost sorted if we can remove exactly one element from it, and the remaining array is sorted. Now you are given an array a1,a2,…,ana1,a2,…,an, is it almost sorted?

Input

The first line contains an integer TT indicating the total number of test cases. Each test case starts with an integer nn in one line, then one line with nn integers a1,a2,…,ana1,a2,…,an. 


1≤T≤20001≤T≤2000 

2≤n≤1052≤n≤105 

1≤ai≤1051≤ai≤105 

There are at most 20 test cases with n>1000n>1000.

Output

For each test case, please output "`YES`" if it is almost sorted. Otherwise, output "`NO`" (both without quotes).

Sample Input

```
3
3
2 1 7
3
3 2 1
5
3 1 4 1 5
```

Sample Output

```
YES
YES
NO
```

### 题意：

**给一个n个数的序列，问去掉一个数字，能否构成不上升或者不下降序列**

### 分析：

**判断原序列的最长不上升子序列长度，和最长不下降子序列长度，若len>=n-1,输出YES，否在NO。**

### [最长上升子序列（LIS）算法](https://blog.csdn.net/qq_40507857/article/details/81198662)

```cpp
#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<vector>
#include<cmath>
#include<queue>
#include<stack>
#include<map>
#include<set>
#include<algorithm>
using namespace std;
#define INF 0x3f3f3f3f
template <class T>
inline void scan(T &ret)
{
    char c;
    ret = 0;
    while ((c = getchar()) < '0' || c > '9');
    while (c >= '0' && c <= '9')
    {
        ret = ret * 10 + (c - '0'), c = getchar();
    }
}
int n;
const int maxn=1e5+10;
int a[maxn];
int dp[maxn];
//最长不下降子序列，时间复杂度：n*log(n)
bool LIS()
{
    memset(dp,0,sizeof dp);
    dp[1]=a[1];
    int len=1;
    for(int i=2;i<=n;i++){
        if(a[i]>=dp[len])
            dp[++len]=a[i];
        else{
          //找到第一个大于a[i]的位置，用a[i]替换
            int pos=upper_bound(dp+1,dp+len+1,a[i])-dp;
            dp[pos]=a[i];
        }
    }
    if(len>=n-1)
        return true;
    else
        return false;
}
//最长不上升子序列
bool LDS()
{
    int i=1,j=n;
    while(i<j){
        swap(a[i],a[j]);
        i++,j--;
    }
    return LIS();
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
           scanf("%d",&a[i]);
        if(LIS()){
            printf("YES\n");
            continue;
        }

        if(LDS()){
             printf("YES\n");
            continue;
        }

        printf("NO\n");
    }
    return 0;
}
```

**还有一种是O(N) 对于每一个数，要么呈现V形或者^形，或者单调，记录上升下降次数，我们对删除某个数所造成影响记录下来，假如删除它致使最后上升次数或者下降次数满足n-2次就行了**

```cpp
#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<vector>
#include<cmath>
#include<queue>
#include<stack>
#include<map>
#include<set>
#include<algorithm>
using namespace std;
#define INF 0x3f3f3f3f
template <class T>
inline void scan(T &ret)
{
    char c;
    ret = 0;
    while ((c = getchar()) < '0' || c > '9');
    while (c >= '0' && c <= '9')
    {
        ret = ret * 10 + (c - '0'), c = getchar();
    }
}
int n;
const int maxn=1e5+10;
int a[maxn];
bool judge()
{
  int res=0,fall=0;
  for(int i=1;i<=n;i++){
    if(i-1>=1&&a[i]>a[i-1]) res++;
    else if(i-1>=1&&a[i]<a[i-1])  fall++;
  }
  for(int i=1;i<=n;i++){
    int t1=res,t2=fall;

    if(i-1>=1&&a[i]>a[i-1]) res--;
    else if(i-1>=1&&a[i]<a[i-1])  fall--;

    if(i+1<=n&&a[i]>a[i+1]) fall--;
    else if(i+1<=n&&a[i]<a[i+1])  res--;

    if(i-1>=1&&i+1<=n){
      if(a[i-1]<a[i+1])  res++;
      else if(a[i-1]>a[i+1])  fall++;
    }
    if(fall==0||res==0)
    return true;
    res=t1;fall=t2;
  }
  return false;
}

int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
           scanf("%d",&a[i]);
        if(judge()){
            printf("YES\n");
            continue;
        }
        printf("NO\n");
    }
    return 0;
}
```





