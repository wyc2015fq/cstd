# 网易校招笔试题 - Fibonacci数列 - curryche的博客 - CSDN博客





2018年08月10日 22:10:52[curryche](https://me.csdn.net/whwan11)阅读数：57








## 题目

Fibonacci数列 

时间限制：1秒 空间限制：32768K 热度指数：22342 

 算法知识视频讲解 

校招时部分企业笔试将禁止编程题跳出页面，为提前适应，练习时请使用在线自测，而非本地IDE。 

题目描述 

Fibonacci数列是这样定义的： 

F[0] = 0 

F[1] = 1 

for each i ≥ 2: F[i] = F[i-1] + F[i-2] 

因此，Fibonacci数列就形如：0, 1, 1, 2, 3, 5, 8, 13, …，在Fibonacci数列中的数我们称为Fibonacci数。给你一个N，你想让其变为一个Fibonacci数，每一步你可以把当前数字X变为X-1或者X+1，现在给你一个数N求最少需要多少步可以变为Fibonacci数。 

输入描述: 

输入为一个正整数N(1 ≤ N ≤ 1,000,000) 

输出描述: 

输出一个最小的步数变为Fibonacci数” 

示例1 

输入 

复制 

15 

输出 

复制 

2
## 解法

代码

```cpp
#include<iostream>

using namespace std;

int main()
{
    int N,f_n_1,f_n_2,f_n,result;
    cin>>N;
    if(N<1||N>1000000)
        return 0;
    if(N<2)
        result=N;

    f_n_1=1;
    f_n_2=1;
    f_n=-1;

    while(f_n<N)
    {
        f_n=f_n_1+f_n_2;
        f_n_2=f_n_1;
        f_n_1=f_n;
    }

    if(f_n==N)
        result=0;
    if(f_n-N<=N-f_n_2)
    {
        result=f_n-N;
    }
    else
    {
        result=N-f_n_2;
    }

    cout<<result<<endl;

    return 0;
}
```




