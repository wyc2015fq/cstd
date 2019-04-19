# POJ 2996 Help Me with the Game（模拟） - HJ - CSDN博客
2017年08月19日 18:32:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：189
个人分类：[====ACM====																[---------OJ---------																[POJ](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://poj.org/problem?id=2996](http://poj.org/problem?id=2996)
是在比赛的时候打的，当时想复杂了，题目没有读清楚。其实就是判断，大写为白棋，小写为黑棋。
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<vector>
using namespace std;
struct point{
    int x,y;
};
point a1[105],b1[105];
vector<point> a[30];
vector<point> b[30];
char s1[105],s[105],ss[]={'K','Q','R','B','N','P'};
point k;
void check1(int x,int j)        //白色棋子 
{
    k.x=8-x+1,k.y=j;
    a[s[j]-'A'].push_back(k);
}
void check2(int x,int j)          //黑色棋子 
{
    k.x=8-x+1,k.y=j;
    b[s[j]-'a'].push_back(k);
}
int cmp1(point p,point q)
{
    if(p.x==q.x)
      return p.y<q.y;
    return p.x<q.x;
} 
int cmp2(point p,point q)
{
    if(p.x==q.x)
      return p.y<q.y;
    return p.x>q.x;
}
int main()
{
    cin>>s1;
    for(int q=1;q<=8;q++)
    {
        cin>>s>>s1;
        for(int j=2;j<=30;j+=4)
        {
            if(s[j]!=':'&&s[j]!='.')
            {
                if(s[j]>='A'&&s[j]<='Z')
                  check1(q,j);
                else 
                  check2(q,j);
            }
        }
    }
    printf("White: ");
    for(int i=0;i<6;i++)
    { 
        int cnt=0;
        for(int k=0;k<a[ss[i]-'A'].size();k++)
          a1[cnt++]=a[ss[i]-'A'][k];
        sort(a1,a1+cnt,cmp1);
        for(int j=0;j<cnt;j++)
        {
            if(i!=5)
              printf("%c",ss[i]);
            printf("%c%d",(a1[j].y-2)/4+'a',a1[j].x);
            if(i!=5||j!=cnt-1)
              printf(",");
        }
    }
    printf("\nBlack: ");
    for(int i=0;i<6;i++)
    {
        int cnt=0;
        for(int k=0;k<b[ss[i]-'A'].size();k++)
          b1[cnt++]=b[ss[i]-'A'][k];
        sort(b1,b1+cnt,cmp2);
        for(int j=0;j<cnt;j++)
        {
            if(i!=5)
              printf("%c",ss[i]);
            printf("%c%d",(b1[j].y-2)/4+'a',b1[j].x);
            if(i!=5||j!=cnt-1)
              printf(",");
        }
    }
    printf("\n");
    return 0;
}
```
