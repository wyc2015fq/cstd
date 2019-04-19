# hdu  1016  Prime Ring Problem（DFS） - HJ - CSDN博客
2016年08月08日 20:45:53[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：299
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Prime Ring Problem
Problem Description 
A ring is compose of n circles as shown in diagram. Put natural number 1, 2, …, n into each circle separately, and the sum of numbers in two adjacent circles should be a prime.
Note: the number of first circle should always be 1.
Input 
n (0 < n < 20).
Output 
The output format is shown as sample below. Each row represents a series of circle numbers in the ring beginning from 1 clockwisely and anticlockwisely. The order of numbers must satisfy the above requirements. Print solutions in lexicographical order.
You are to write a program that completes above process.
Print a blank line after each case.
Sample Input 
6 
8
Sample Output 
Case 1: 
1 4 3 2 5 6 
1 6 5 2 3 4
Case 2: 
1 2 3 8 5 6 7 4 
1 2 5 8 3 4 7 6 
1 4 7 6 5 8 3 2 
1 6 7 4 3 8 5 2
解析：本题在lnj的紫书上面作为例题讲过，不过主要体现的是回溯，在这里，我重点就广搜这方面来讲。
思路：首先需要开一个标记数组和一个判断素数的调用函数，然后从第二个位置开始进行广搜，从2到n，如果找到符合条件的数，则用标记数组进行标记，一直递归搜索，知道第n个位置，如果符合条件跳出边界输出结果，如果中途不符合条件则清除当前节点标记并一层一层返回上一层，不断回溯。
具体解释看如下代码：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
int n,a[22],vis[22];       //a为素数数组，vis为标记数组 
int prime(int a)          //判断素数 
{
    for(int i=2;i*i<=a;i++)
      if(a%i==0)
        return 0;
    return 1;
}
void dfs(int x)
{ 
    if(x==n+1&&prime(a[x-1]+a[1]))     //判断边界条件，如果满足即输出 
    {
        for(int i=1;i<n;i++)
          cout<<a[i]<<" ";
        cout<<a[n]<<endl;
    }
    else
    {
        for(int i=2;i<=n;i++)        //遍历寻找符合的数 
        {
            if(!vis[i]&&prime(i+a[x-1])) 
            {
                vis[i]=1;            //对当前满足的数所对应的标记下标数组进行标记 
                a[x]=i;              //对当前满足的数所对应的素数下标数组进行赋值 
                dfs(x+1);           //递归广度优先搜索 
                vis[i]=0;           //回溯时注意清除标记 
            }
        }
    }
}
int main()
{
    int t=0;
    while(cin>>n)
    {
        t++;
        cout<<"Case "<<t<<":"<<endl;
        memset(vis,0,sizeof(vis));
        a[1]=1;
        dfs(2);      //从第二个位置开始递归搜索 
        cout<<endl;
    }
    return 0;
}
```
