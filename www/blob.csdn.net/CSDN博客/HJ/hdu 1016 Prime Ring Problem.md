# hdu 1016  Prime Ring Problem - HJ - CSDN博客
2016年07月18日 14:46:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：177
hdu 1016  Prime Ring Problem
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
解析：简单来说，如果用暴力把每一种结果都测试一遍，那么排列的总数会很大，明显会超时。因此，需要用到回溯法，详细见以下代码：
```cpp
#include<iostream>
using namespace std;
int visit[20],a[21],n;
int _prime(int p)           //判断素数
{
    int i;
    for(i=2;i*i<=p;i++)
      if(p%i==0)
        return 0;
    return 1;
}
void dfs(int k,int num,int n)    //递归广度优先搜索
{
    a[k]=num;
    visit[num]=1;
    if(k==n&&_prime(a[1]+a[k]))      //边界条件
    {
        cout<<"1";
        for(int i=2;i<=n;i++)
          cout<<" "<<a[i];
        cout<<endl;
    }
    else for(int i=1;i<=n;i++)
    {
        if(!visit[i]&&_prime(i+a[k]))
        {
            dfs(k+1,i,n);
            visit[i]=0;        //清除标志
        } 
    }
}
int main()
{
    int t=1;
    while(cin>>n)
    {
        a[1]=1;
        cout<<"Case "<<t<<":"<<endl;
        memset(visit,0,sizeof(visit));
        dfs(1,1,n);
        cout<<endl;
        t++;
    }
    return 0;
}
```
