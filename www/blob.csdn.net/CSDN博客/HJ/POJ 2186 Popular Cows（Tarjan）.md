# POJ 2186  Popular Cows（Tarjan） - HJ - CSDN博客
2017年01月18日 02:21:47[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：313
Description 
Every cow’s dream is to become the most popular cow in the herd. In a herd of N (1 <= N <= 10,000) cows, you are given up to M (1 <= M <= 50,000) ordered pairs of the form (A, B) that tell you that cow A thinks that cow B is popular. Since popularity is transitive, if A thinks B is popular and B thinks C is popular, then A will also think that C is 
popular, even if this is not explicitly specified by an ordered pair in the input. Your task is to compute the number of cows that are considered popular by every other cow.
Input 
* Line 1: Two space-separated integers, N and M
- Lines 2..1+M: Two space-separated numbers A and B, meaning that A thinks B is popular.
Output 
* Line 1: A single integer that is the number of cows who are considered popular by every other cow.
Sample Input
3 3 
1 2 
2 1 
2 3
Sample Output
1
Hint 
Cow 3 is the only cow of high popularity.
Source 
USACO 2003 Fall
题目大意：
有N头牛，都想成为most poluler的牛，给出M个关系，如(1,2)代表1欢迎2，关系可以传递，但是不可以相互，即1欢迎2不代表2欢迎1，但是如果2也欢迎3那么1也欢迎3. 
给出N,M和M个欢迎关系，求被所有牛都欢迎的牛的数量。
思路：
1、用Tarjan求双连通分量然后缩成点。这些点会形成一棵树。
2、求树上的节点有多少个出度为零，如果有一个就输出那个点里包含的所有点（因为是缩点出来的树）。
注意：
1、给出的图会有不连通的可能，如果那样肯定输出零。因为不连通肯定不会有所有其他牛认为某只牛很牛的情况出现。
2、如果缩点后有多个出度为零的点，那么输出零。因为这样图虽然联通了，但是还是不会出现所有其他牛认为某只牛很牛的情况。
Tarjan算法也可收藏作为模板，如果没有弄懂，推荐找篇博客仔细理解。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 10001
int n,m;
struct node
{
    int to,next;
}edge[maxn*5];
int head[maxn],cnt,time,Bcnt,top;
void add(int u,int v)
{
    edge[cnt].to=v;
    edge[cnt].next=head[u];
    head[u]=cnt++;
}
int Belong[maxn],dfn[maxn],low[maxn],stack[maxn];
int out[maxn];                                      //记录出度 
bool instack[maxn];
void tarjan(int u)
{
    int v;
    dfn[u]=low[u]=++time;           //标记点u的DFS遍历序号 
    stack[top++]=u;                 //压入栈中 
    instack[u]=true;               //标记u点已经在栈中 
    for(int j=head[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].to;
        if(!dfn[v])                 //如果u的邻接点没有入过栈 
        {
            tarjan(v);
            low[u]=min(low[u],low[v]);     //如果u能直接到达的这个点没在栈中，u的最早祖先为他们中的较小值 
        }
        else if(instack[v])         //如果在栈中 
          low[u]=min(low[u],dfn[v]);    //则u的最早祖先是他的序号和那个点序号中的较小的那个 
    }
    if(dfn[u]==low[u])    //如果dfn[u]和low[u]相等，则说明u点是其所属强连通分支DFS遍历起点，这个强连通分支所有点都在u点之上 
    {
        Bcnt++;
        do{
            v=stack[--top];
            instack[v]=false;
            Belong[v]=Bcnt;
        }while(u!=v);          //弹栈，保存属于同一强连通分支的点，Belong[v]的值表示顶点v所属强连通分支的序号 
    }
}
int main()
{
    int n,m,a,b,u,v,ans;
    while(cin>>n>>m)
    {
        cnt=0,ans=0;
        memset(head,-1,sizeof(head));
        while(m--)
        {
            cin>>a>>b;
            add(a,b);                             //建图 
        } 
        memset(dfn,0,sizeof(dfn));
        Bcnt=top=time=0;;
        for(int u=1;u<=n;u++)                     //taijan
          if(!dfn[u])
            tarjan(u);                         
        memset(out,0,sizeof(out));
        for(int u=1;u<=n;u++)                   //求taijan之后缩点的出度 
        {
            for(int j=head[u];j!=-1;j=edge[j].next)
            {
                v=edge[j].to;
                if(Belong[u]!=Belong[v])        //如果u的邻接点与u不属于同一连通分支 
                  out[Belong[u]]++;             //u所属的连通分支序号的总出度增加一个 
            }
        }
        int num=0;                         //num记录缩点后出度为0的个数 
        int flag;                          //flag记录入度为0的缩点标号(也就是第几个连通分量) 
        for(int i=1;i<=Bcnt;i++)           //在Bcnt个连通分支中统计出度为0的连通分支的个数 , 并用flag保存连通分支的序号 
          if(out[i]==0)
          {
              num++;
              flag=i;
          }
        if(num==1)                        //只有一个连通分量的出度为0 
          for(int i=1;i<=n;i++)
            if(Belong[i]==flag)          //计算属于这个连通分量的所有牛 
              ans++;
        cout<<ans<<endl;   
    }
    return 0;
}
```
