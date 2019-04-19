# hdu 1546  Idiomatic Phrases Game （SPFA或者Dijkstra） - HJ - CSDN博客
2017年10月10日 17:52:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：182
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1546](http://acm.split.hdu.edu.cn/showproblem.php?pid=1546)
题目大意： 
给出n个“成语”， 这写成语至少由3个“汉字”组成，所谓的“汉字”，是指4个连续的16进制数字（1～9， A～F）。 以第一个成语作为起点，最后一个作为终点， 需要找出一个序列，这个序列的前一个成语的最后一个“汉字”与后一个成语的第一个“汉字”是相同的，求最少花费时间。
Dijkstra优先队列优化：
```cpp
#include<iostream>
#include<cstring>
#include<queue>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1005;
struct node{
    int v,len;
    bool operator < (const node&a)const{
        return len>a.len;
    }
};
priority_queue<node>q;
int n,cnt,d[maxn],head[maxn],cost[maxn];
struct edge{
    int v,next,w;
}E[maxn*maxn];
struct Idiom{
    char s[10],t[10];
}a[maxn];
void Init()           //初始化 
{
    cnt=0;
    memset(head,-1,sizeof(head));
    while(!q.empty())
      q.pop();
}
void addedge(int u,int v,int w)      //邻接表构图 
{
    E[cnt].v=v,E[cnt].w=w;
    E[cnt].next=head[u];
    head[u]=cnt++;
}
void Dijkstra(int s)        //优先队列优化的Dijkstra 
{
    for(int i=0;i<n;i++)
      d[i]=INF;
    d[s]=0;
    q.push(node{s,d[s]});
    while(!q.empty())
    {
        node x=q.top();
        q.pop();
        int u=x.v;
        if(d[u]!=x.len)
          continue;
        for(int e=head[u];~e;e=E[e].next)
        {
            int tmp=d[u]+E[e].w;
            if(d[E[e].v]>tmp)
            {
                d[E[e].v]=tmp;
                q.push(node{E[e].v,d[E[e].v]});
            }
        }
    }
}
int main()
{
    char str[105];
    while(cin>>n&&n)
    {
        Init();
        for(int i=0;i<n;i++)
        {
            cin>>cost[i]>>str;
            for(int j=0;j<5;j++)      //取前四位 
              a[i].s[j]=str[j];
            int len=strlen(str);
            for(int j=len-4,k=0;j<len;k++,j++)        //取后四位 
              a[i].t[k]=str[j];
             a[i].s[4]=a[i].s[4]='\0';            //每个a[i]保存两个"汉字" 
        }
        for(int i=0;i<n;i++)
          for(int j=0;j<n;j++)
            if(strcmp(a[i].t,a[j].s)==0)   //如果i的尾部与j的首部相同  
              addedge(i,j,cost[i]);   //代表i->j连通  连通的权值大小为 cost[i] 
        Dijkstra(0);     
        if(d[n-1]==INF)
          d[n-1]=-1;
        cout<<d[n-1]<<endl;
    } 
    return 0;
}
```
SPFA
```cpp
#include<iostream>
#include<cstring>
#include<queue>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1005;
char str[maxn][maxn];
int n,d[maxn],cost[maxn],vis[maxn];
vector<int>E[maxn];
void SPFA(int s)
{
    queue<int>q;
    memset(vis,0,sizeof(vis));
    d[s]=0;
    vis[s]=1;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        vis[u]=0;
        for(int i=0;i<E[u].size();i++)
        {
            if(d[E[u][i]]>d[u]+cost[u])
            {
                d[E[u][i]]=d[u]+cost[u];
                if(!vis[E[u][i]])
                {
                    vis[E[u][i]]=1;
                    q.push(E[u][i]);
                }
            }
        } 
    }
} 
void Init()
{
    for(int i=0;i<n;i++)
    {
        d[i]=INF;
        E[i].clear();
    }
}
int main()
{
    while(cin>>n&&n)
    {
        Init();
        for(int i=0;i<n;i++)
          cin>>cost[i]>>str[i];
        for(int i=0;i<n;i++)
        {
            int len2=strlen(str[i]);
            for(int j=0;j<n;j++)
              if(str[i][len2-4]==str[j][0]&&str[i][len2-3]==str[j][1]&&str[i][len2-2]==str[j][2]&&str[i][len2-1]==str[j][3])   
                E[i].push_back(j);  
        }
        SPFA(0);
        if(d[n-1]==INF)
          d[n-1]=-1;
        cout<<d[n-1]<<endl;
    } 
    return 0;
}
```
