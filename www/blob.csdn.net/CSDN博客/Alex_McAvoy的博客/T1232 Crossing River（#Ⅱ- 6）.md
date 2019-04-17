# T1232 Crossing River（#Ⅱ- 6） - Alex_McAvoy的博客 - CSDN博客





2018年05月13日 14:36:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：774








### 【题目描述】

几个人过河，每次过两人一人回，速度由慢者决定，问过河所需最短时间。

### 【输入】

输入t组数据，每组数据第1行输入n，第2行输入n个数，表示每个人过河的时间。



### 【输出】

输出t行数据，每行1个数，表示每组过河最少时间。

### 【输入样例】

```
1
4
1 2 5 10
```

### 【输出样例】
`17`
### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#define INF 999999999
#define N 1001
using namespace std;
int a[N],dp[N];
int main()
{
    int t;
    int n;
    cin>>t;
    while(t--)
    {
        cin>>n;
        for(int i=0;i<n;i++)
            cin>>a[i];
        sort(a,a+n);
        dp[0]=a[0];
        dp[1]=a[1];
        for(int i=2;i<n;i++)
            dp[i]=min(dp[i-1]+a[0]+a[i],dp[i-2]+a[0]+a[i]+a[1]*2);
        cout<<dp[n-1]<<endl;
    }
    return 0;
}
```





