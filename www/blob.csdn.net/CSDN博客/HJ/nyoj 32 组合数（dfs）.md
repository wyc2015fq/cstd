# nyoj  32 组合数（dfs） - HJ - CSDN博客
2017年07月01日 15:51:21[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：188
描述 
    找出从自然数1、2、… 、n（0< n< 10）中任取r(0< r<=n)个数的所有组合。
输入 
    输入n、r。 
输出 
    按特定顺序输出所有组合。 
    特定顺序：每一个组合中的值从大到小排列，组合之间按逆字典序排列。 
样例输入 
    5 3
样例输出 
    543 
    542 
    541 
    532 
    531 
    521 
    432 
    431 
    421 
    321
简单dfs，主要是思维！
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int n,m,vis[15];
void dfs(int x,int k)        //x表示当前最小的数，k表示当前阶段数字的个数 
{
    if(k==m)               //如果当前阶段数字的个数等于m了，便打印结果 
    {
        for(int i=n;i>=1;i--)
          if(vis[i])             //结果用vis[]数组保存，从大到小输出 
            cout<<i;
        cout<<endl;
        return ;
    }
    for(int i=n;i>=1;i--)
    {
        if(vis[i]||i>x)
          continue;
        vis[i]=1;
        dfs(i,k+1);
        vis[i]=0;
    }
}
int main()
{
    while(cin>>n>>m)
    {
        memset(vis,0,sizeof(vis));
        for(int i=n;i>=m;i--)         //从n到m 
        {
            vis[i]=1;
            dfs(i,1);
            vis[i]=0;
        }
    } 
    return 0;
}
```
