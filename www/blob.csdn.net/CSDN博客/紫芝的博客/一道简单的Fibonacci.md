# 一道简单的Fibonacci - 紫芝的博客 - CSDN博客





2018年07月23日 22:40:54[紫芝](https://me.csdn.net/qq_40507857)阅读数：30
个人分类：[数论](https://blog.csdn.net/qq_40507857/article/category/7647322)









## 一道简单的Fibonacci

时间限制: 1 Sec  内存限制: 32 MB

## 题目描述

有如下数列：F(0) = 7, F(1) = 11, F(n) = F(n - 1) + F(n - 2)  (n >= 2)。



## 输入

输入由一系列的行构成，每一行包含一个正数 n (n < 1000000)。



## 输出

如果 F(n) 能被 3 整除，则输出 “Yes”，否则输出 “No”。



## 样例输入

0
1
2
3
4
5

## 样例输出

No
No
Yes
No
No
No
解法一：打表+找规律

```cpp
#include<bits/stdc++.h>
using namespace std;
int f(int n)
{
    if(n==0)    return 7;
    if(n==1)    return 11;
    return f(n-1)+f(n-2);
}
int main()
{
    for(int i=0;i<50;i++)
        if(f(i)%3==0)
        printf("%d:Yes\n",i);
    else    printf("%d:No\n",i);
    return 0;
}
```

![](https://img-blog.csdn.net/20180723220622526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

AC代码

```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    while(cin>>n){
        if((n-2)%4==0)
        printf("Yes\n");
    else    printf("No\n");
    }
    return 0;
}
```

解法2：同余公式：(a+b)%mod=a%mod+b%mod

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int f(int n)
{
    if(n==0)    return 7;
    if(n==1)    return 11;
    int f0=7,f1=11,f2;
    //同余公式：(a+b)%mod=a%mod+b%mod
    for(int i=1;i<n;i++)
    {
        f2=f0+f1;
        f0=f1%3;
        f1=f2%3;
    }
    return f2;
}
int main()
{
    int n;
    while(cin>>n){
        if(f(n)%3==0)
        printf("Yes\n");
    else    printf("No\n");
    }
    return 0;
}
```

解法三：打表

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn=1e6+10;
int a[maxn];
int main()
{
    a[0]=7;a[1]=11;
    for(int i=2;i<maxn;i++)
    {
        a[i]=a[i-1]+a[i-2];
        a[i-1]%=3;a[i-2]%=3;
    }
    int n;
    while(cin>>n){
        if(a[n]%3==0)
        printf("Yes\n");
    else    printf("No\n");
    }
    return 0;
}
```





