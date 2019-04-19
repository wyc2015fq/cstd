# UVALive 7267 Mysterious Antiques in Sackler Museum（dfs） - HJ - CSDN博客
2017年10月27日 13:07:25[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：197
[https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5279](https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5279)
题目大意： 
给出四个矩形的长和宽，要你判断任意选取其中的三个能不能组成一个新的矩形。
简单dfs搜一下就行了，注意要判断两种情况（如果是两个一起判断就是四种）。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
int blag,vis[4];
struct node{
    int x,y;
}s[5];
void dfs(int a,int b,int cur)
{
    if(cur==3)
    {
        blag=1;
        return ;
    }
    for(int i=0;i<4;i++)
    {
        if(!vis[i])
        {
            vis[i]=1;
            if(s[i].x==a)
              dfs(b+s[i].y,a,cur+1);
            if(s[i].x==b)
              dfs(a+s[i].x,b,cur+1);
            if(s[i].y==a)
              dfs(b+s[i].x,a,cur+1);
            if(s[i].y==b)
              dfs(a+s[i].x,b,cur+1);
            vis[i]=0;
        } 
    }
} 
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        for(int i=0;i<4;i++)
          cin>>s[i].x>>s[i].y;
        blag=0;
        memset(vis,0,sizeof(vis));
        for(int i=0;i<4;i++)
        {
            vis[i]=1;
            dfs(s[i].x,s[i].y,1);
            vis[i]=0;
        }
        printf("%s\n",blag?"Yes":"No");
    }
    return 0;
}
```
