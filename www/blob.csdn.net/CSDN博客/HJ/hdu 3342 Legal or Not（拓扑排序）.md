# hdu  3342 Legal or Not（拓扑排序） - HJ - CSDN博客
2017年04月27日 18:46:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：262
Problem Description 
ACM-DIY is a large QQ group where many excellent acmers get together. It is so harmonious that just like a big family. Every day,many “holy cows” like HH, hh, AC, ZT, lcc, BF, Qinz and so on chat on-line to exchange their ideas. When someone has questions, many warm-hearted cows like Lost will come to help. Then the one being helped will call Lost “master”, and Lost will have a nice “prentice”. By and by, there are many pairs of “master and prentice”. But then problem occurs: there are too many masters and too many prentices, how can we know whether it is legal or not?
We all know a master can have many prentices and a prentice may have a lot of masters too, it’s legal. Nevertheless，some cows are not so honest, they hold illegal relationship. Take HH and 3xian for instant, HH is 3xian’s master and, at the same time, 3xian is HH’s master,which is quite illegal! To avoid this,please help us to judge whether their relationship is legal or not.
Please note that the “master and prentice” relation is transitive. It means that if A is B’s master ans B is C’s master, then A is C’s master.
Input 
The input consists of several test cases. For each case, the first line contains two integers, N (members to be tested) and M (relationships to be tested)(2 <= N, M <= 100). Then M lines follow, each contains a pair of (x, y) which means x is y’s master and y is x’s prentice. The input is terminated by N = 0. 
TO MAKE IT SIMPLE, we give every one a number (0, 1, 2,…, N-1). We use their numbers instead of their names.
Output 
For each test case, print in one line the judgement of the messy relationship. 
If it is legal, output “YES”, otherwise “NO”.
Sample Input
3 2 
0 1 
1 2 
2 2 
0 1 
1 0 
0 0
Sample Output
YES 
NO
题目大意： 
a是b的师傅，b是c的师傅，那么a是c的师傅，如果说c是a的师傅则不合法。
解题思路： 
判断图是否存在环。用拓扑排序，两种方法：
方法一：用数组记录
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int vis[105],a[105][105],n,m,u,v;
void topsort()
{
    int blag=0,i,j;
    for(i=0;i<n;i++)   //最多需要寻找n次 
    {
        for(j=0;j<n;j++)    //寻找入度为0的点 
        {
            if(vis[j]==0)
            {
                vis[j]--;
                for(int k=0;k<n;k++)   ///与当前入度为零的点相连的点入度-- 
                  if(a[j][k])
                    vis[k]--;
                break;
            }
        }
        if(j==n)    //如果每次找不到入度为零的点时候，说明有环 
        {
            blag=1;
            break;
        }
    }
    if(blag)
      cout<<"NO"<<endl;
    else
      cout<<"YES"<<endl;
}
int main()
{
    while(cin>>n>>m&&n)
    {
        memset(a,0,sizeof(a));
        memset(vis,0,sizeof(vis));
        for(int i=0;i<m;i++)
        {
            cin>>u>>v;
            if(!a[u][v])
            {
               a[u][v]=1;
               vis[v]++;    //入度++ 
            }
        }
        topsort();
    }
    return 0;
}
```
方法二：邻接表+队列
```cpp
#include<iostream>
#include<cstring>
#include<queue>
#include<vector>
using namespace std;
queue<int> q;
vector<int>t[105];
int in[105],n,m;
void topsort()
{
    for(int i=0;i<n;i++)
      if(in[i]==0)
        q.push(i);
    int cnt=0;
    while(!q.empty())
    {
        cnt++;
        int p=q.front();
        q.pop();
        for(int i=0;i<t[p].size();i++)
        {
            in[t[p][i]]--;
            if(in[t[p][i]]==0)
              q.push(t[p][i]);
        } 
    }
    if(cnt!=n)
      cout<<"NO"<<endl;
    else
      cout<<"YES"<<endl;
}
int main()
{
    while(cin>>n>>m&&n)
    {
        int u,v; 
        memset(in,0,sizeof(in));
        for(int i=0;i<105;i++)
          t[i].clear();
        while(!q.empty())
          q.pop();
        for(int i=0;i<m;i++)
        {
            cin>>u>>v;
            t[u].push_back(v);
            in[v]++;
        }
        topsort();
    }
    return 0;
}
```
