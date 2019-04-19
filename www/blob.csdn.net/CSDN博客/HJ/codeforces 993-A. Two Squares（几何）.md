# codeforces 993-A. Two Squares（几何） - HJ - CSDN博客
2018年08月09日 14:24:10[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：90
[http://codeforces.com/problemset/problem/993/A](http://codeforces.com/problemset/problem/993/A)
解题思路：
枚举两个正方形包含的点，暴力判断是否重复即可。
```cpp
#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
struct node{
    int x,y;
};
node a[5],b[5];
int vis[205][205];
bool cmp(node p,node q)
{
    if(p.x==q.x)
      return p.y<q.y;
    return p.x<q.x;
}
int main()
{
//  while(true)
//  {
        memset(vis,0,sizeof(vis));
        for(int i=0;i<4;i++)
          cin>>a[i].x>>a[i].y;
        for(int i=0;i<4;i++)
          cin>>b[i].x>>b[i].y;
        sort(a,a+4,cmp);
        sort(b,b+4,cmp);
        for(int i=a[0].x;i<=a[2].x;i++)
          for(int j=a[0].y;j<=a[1].y;j++)
            vis[i+100][j+100]=1;
        int cnt=0,blag=0;
        for(int i=b[0].x;i<b[1].x;i++,cnt++)
          for(int j=b[0].y-cnt;j<=b[0].y+cnt;j++)
            if(vis[i+100][j+100])
              blag=1;
        for(int i=b[1].x;i<=b[3].x;i++,cnt--)
          for(int j=b[0].y-cnt;j<=b[0].y+cnt;j++)
            if(vis[i+100][j+100])
              blag=1;
        if(blag==0)
          cout<<"NO"<<endl;
        else
          cout<<"YES"<<endl;
//  }
    return 0;
}
```
