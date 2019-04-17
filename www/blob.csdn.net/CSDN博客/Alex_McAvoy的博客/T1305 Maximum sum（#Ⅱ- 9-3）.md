# T1305 Maximum sum（#Ⅱ- 9 - 3） - Alex_McAvoy的博客 - CSDN博客





2018年06月07日 15:14:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：396








### 【题目描述】

对于给定的整数序列A={a1,a2,...,an}，找出两个不重合连续子段，使得两子段中所有数字的和最大。我们如下定义函数 d(A)：

![](https://img-blog.csdn.net/20180607223546279)

我们的目标就是求出d(A)。

### 【输入】

第一行是一个整数T(≤30)，代表一共有多少组数据。

接下来是T组数据。

每组数据的第一行是一个整数，代表数据个数据n(2≤n≤50000) ,第二行是nn个整数a1,a2,...,an(|ai|≤10000)。

### 【输出】

输出一个整数，就是d(A)的值。

### 【输入样例】

```
1
10
1 -1 2 2 3 -3 4 -4 5 -5
```

### 【输出样例】
`13`
### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 50001
#define MOD 50001
#define E 1e-12
using namespace std;
int a[N];
int leftt[N],rightt[N];
int leftmax[N],rightmax[N];
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        int n;
        cin>>n;
        for(int i=1;i<=n;i++)
            cin>>a[i];

        leftt[1]=a[1];
        leftmax[1]=a[1];
        rightt[n]=a[n];
        rightmax[n]=a[n];

        for(int i=2;i<=n;i++)
            leftt[i]=max(a[i],leftt[i-1]+a[i]);
        for(int i=n-1;i>=1;i--)
            rightt[i]=max(a[i],rightt[i+1]+a[i]);
        for(int i=2;i<=n;i++)
            leftmax[i]=max(leftmax[i-1],leftt[i]);
        for(int i=n-1;i>=1;i--)
            rightmax[i]=max(rightmax[i+1],rightt[i]);

        int ans=a[1];
        for(int i=2;i<=n;i++)
            ans=max(ans,leftmax[i-1]+rightmax[i]);

        cout<<ans<<endl;
    }
    return 0;
}
```













