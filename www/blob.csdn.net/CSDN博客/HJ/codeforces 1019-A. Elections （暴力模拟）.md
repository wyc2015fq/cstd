# codeforces 1019-A. Elections （暴力模拟） - HJ - CSDN博客
2018年08月12日 20:05:04[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：193
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/1019/A](http://codeforces.com/problemset/problem/1019/A)
题目大意：
某个国家有n位候选人以及每位候选人的投票数，现在要求编号为1的候选人票数最高（即比其他任何票数都要至少多1）。你可以花一些钱收买那些投票的人，改投编号为1的党派。问最少需要多少钱收买，才能让编号为1的党派候选人当选（即票数最高）
解题思路：
刚开始做的时候一脸懵逼，其实是因为不确定投多少票才是最高的。因此只要求出最少需要的投的票数和最多只需要投的票数，从小到大进行枚举。用vector进行排序贪心取值，用visited进行标记。详细的过程处理代码会有注释。
比较恶心的一个模拟吧，思路要清楚。
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<map>
#include<set>
#include<cstdio>
#include<cmath>
using namespace std;
typedef long long ll;
#define mod 1000000007
const int maxn=3e3+10;
struct node{
    int id;
    ll c;
}a[maxn];
vector<node> vec[maxn];   //vec[i]表示投票选举i的人需要收买的金钱列表 
int vis[maxn],visited[maxn];
bool cmp(node a,node b)
{
    return a.c<b.c;
}
int main()
{
    int n,m;
    while(cin>>n>>m)
    {
        for(int i=0;i<=n;i++)
          vec[i].clear();
        memset(vis,0,sizeof(vis));   //vis[i]表示投票选举i的票数 
        for(int i=0;i<n;i++)
        {
            cin>>a[i].id>>a[i].c;
            vis[a[i].id]++;   
            vec[a[i].id].push_back(node{i,a[i].c});
        }
        sort(a,a+n,cmp);
        int pos=0,blag=0;;
        for(int i=2;i<=m;i++)   //找到票数的最高值 
          pos=max(pos,vis[i]);
        ll res=0;
        for(int i=0,j=0;i<pos-vis[1]+1;j++)
          if(a[j].id!=1)
            res+=a[j].c,i++;
        ll ans=res;
        while(true)
        {
            if(blag)   //blag表示是否退出的判断变量 
              break;
            pos--;      //pos表示i=1之外的每个选择的投票数的最高值 
            res=0;      //res表示投pp票需要的最少收买金钱数 
            memset(visited,0,sizeof(visited));
            int pp=vis[1];    //pp代表需要的票数,开始的时候值为vis[1] 
            for(int i=2;i<=m;i++)
              if(vis[i]>pos)     //假如i的投票数超过当前最高值，那么就需要先收买投i的一些人 
                for(int j=0;j<vec[i].size()&&j<vis[i]-pos;pp++,j++)
                  res+=vec[i][j].c,visited[vec[i][j].id]=1;
            if(pp>pos)   //如果所有需要提前收买的人收买了之后的票数已经超过了最高值，那么就不必要进行下一轮了 
              blag=1;
            for(int i=0;i<n&&pp<=pos;i++)  //如果没有超过最高值，那么就需要收买其他人，让票数达到最高值 
              if(!visited[i]&&a[i].id!=1)
                res+=a[i].c,pp++;
            ans=min(ans,res);
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
