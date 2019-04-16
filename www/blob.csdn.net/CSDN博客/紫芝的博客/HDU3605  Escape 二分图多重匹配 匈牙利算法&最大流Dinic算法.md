# HDU3605  Escape 二分图多重匹配  匈牙利算法&最大流Dinic算法 - 紫芝的博客 - CSDN博客





2018年10月08日 18:41:28[紫芝](https://me.csdn.net/qq_40507857)阅读数：81








#  Escape

2012 If this is the end of the world how to do? I do not know how. But now scientists have found that some stars, who can live, but some people do not fit to live some of the planet. Now scientists want your help, is to determine what all of people can live in these planets. 

Input

More set of test data, the beginning of each data is n (1 <= n <= 100000), m (1 <= m <= 10) n indicate there n people on the earth, m representatives m planet, planet and people labels are from 0. Here are n lines, each line represents a suitable living conditions of people, each row has m digits, the ith digits is 1, said that a person is fit to live in the ith-planet, or is 0 for this person is not suitable for living in the ith planet. 

The last line has m digits, the ith digit ai indicates the ith planet can contain ai people most.. 

0 <= ai <= 100000 

Output

Determine whether all people can live up to these stars 

If you can output YES, otherwise output NO. 

Sample Input

```
1 1
1
1

2 2
1 0
1 0
1 1
```

Sample Output

```
YES
NO
```

### 题意：

**n个人要移居到m个星球，每个人对每个星球有喜欢（1表示）和不喜欢（0表示），每个星球有最大容量，所有人可以到喜欢的星球，输出YES，否则输出NO**

### 分析：

二分图多重匹配问题，可以用**[匈牙利算法](https://blog.csdn.net/qq_40507857/article/details/82143611)，简单高效，还可以用IO优化一下**

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>
#define INF 1e9
using namespace std;
const int maxn=1e5+10;
int n1,n2;//n1,n2为二分图的顶点集，其中x属于n1，y属于n2
bool mp[maxn][12];
bool vis[12];
int link[12][maxn];
int cap[12];//这个星球最多的人数
int vlink[12];//已经住的人数

//输入数据非常大，输入挂可以优化500ms
template <class T>
inline void scan_d(T &ret)
{
    char c;
    ret = 0;
    while ((c = getchar()) < '0' || c > '9');
    while (c >= '0' && c <= '9')
    {
        ret = ret * 10 + (c - '0'), c = getchar();
    }
}

int dfs(int x)
{
    for(int i=0;i<n2;i++){
        if(mp[x][i]&&!vis[i]){//x->i有边，且未被搜索
            vis[i]=1;//标记已经搜索过
            if(vlink[i]<cap[i])//当前流量小于容量
            {
                link[i][vlink[i]++]=x;//跟新
                return 1;
            }
            for(int j=0;j<vlink[i];j++){
                if(dfs(link[i][j])){//可以寻找到增广路
                    link[i][j]=x;
                    return 1;
                }
            }
        }
    }
    return 0;
}
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF){
        n1=n;n2=m;
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                //scanf("%d",&mp[i][j]);
                scan_d(mp[i][j]);
            }
        }
        for(int i=0;i<m;i++)
            //scanf("%d",&cap[i]);
            scan_d(cap[i]);
        bool flag=true;

        memset(vlink,0,sizeof vlink);//每次清空
        for(int i=0;i<n;i++)
        {
            memset(vis,0,sizeof vis);
            if(!dfs(i))  {
                flag=false;
                break;
            }
        }
        if(flag)
            printf("YES\n");
        else
            printf("NO\n");
    }
}
```

### 网络流->最大流问题

一开始还以为是最大流的裸题，然而N的范围太大，结果建完所有的边就TLE啦。

**首先，N个人选择M个星球（M<=10），一定有许多人的选择是重复的，总方案数不超过![2^{10}](https://private.codecogs.com/gif.latex?2%5E%7B10%7D)（1024），可以对N个人进行缩点，选择一样星球，就把数量累加起来，设为集合SET；**
- **源点连接选择相同的人所组成的集合，容量为人数；**
- **然后连接这个集合所选择的星球，容量为集合人数；**
- **M个星球连接汇点，容量为星球最大容纳人数；**

**若源点到汇点最大流量大于等于N，输出YES，否则，输出NO**

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>
#define INF 1e9
using namespace std;
const int maxn=1e5+10;

struct Edge
{
    int to,cap,next;
}edge[maxn<<1];
int head[maxn];
int cnt=0;
void init()
{
    cnt=0;
    memset(head,-1,sizeof head);
}
void AddEdge(int start,int to,int cap)
{
    edge[cnt].to=to;
    edge[cnt].cap=cap;
    edge[cnt].next=head[start];
    head[start]=cnt++;

    edge[cnt].to=start;
    edge[cnt].cap=0;
    edge[cnt].next=head[to];
    head[to]=cnt++;
}

int src,tar;
int d[maxn];//深度
int q[maxn<<1];

//汇点是否成功标号，即是否找到增广路
bool bfs()
{
    memset(d,-1,sizeof d);
    int front=0,tail=0;
    q[tail++]=src;
    d[src]=0;
    while(front<tail){
        int x=q[front++];

        if(x==tar)  return true;

        for(int i=head[x];i!=-1;i=edge[i].next){
            int temp=edge[i].to;
            //没有标记，且可行流大于0
            if(d[temp]==-1&&edge[i].cap>0){
                d[temp]=d[x]+1;
                q[tail++]=temp;
            }
        }
    }
    return false;
}

int dfs(int x,int cap)
{
    if(x==tar)  return cap;
    int flow=0;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int temp=edge[i].to;
        if(d[temp]==d[x]+1&&edge[i].cap){
            int f=dfs(temp,min(cap-flow,edge[i].cap));
            edge[i].cap-=f;
            edge[i^1].cap+=f;
            flow+=f;
            if(flow==cap) break;
        }
    }
    if(!flow)   d[x]=-2;//防止重搜
    return flow;
}

int maxflow()
{
    int flow=0,f=0;
    while(bfs()){
        while((f=dfs(src,INF))>0)
            flow+=f;
            //printf("%d\n",flow);
    }
    return flow;
}
int num[1200];//n个人最多有(1<<m)种选择
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF){
            init();
        memset(num,0,sizeof num);
        src=0,tar=(1<<m)+m+1;

        for(int i=0;i<n;i++){
                int temp=0;
            for(int j=0;j<m;j++)
            {
                int x;
                scanf("%d",&x);
                if(x==1)
                temp|=(1<<j);
            }
            num[temp]++;//记录选择为temp的人数
        }
        for(int i=1;i<(1<<m);i++){
            if(!num[i])continue;//这种选择没有人
            AddEdge(src,i+1,num[i]);//源点和选择集合建边
            for(int j=0;j<m;j++){
                if(i&(1<<j))
                    AddEdge(i+1,j+(1<<m)+1,num[i]);//选择集合和星球建边
            }
        }

        for(int i=0;i<m;i++){
            int x;
            scanf("%d",&x);
            AddEdge(i+(1<<m)+1,tar,x);//星球和汇点建边
        }
        int flow=maxflow();
        //printf("%d\n",flow);
        if(flow>=n)
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}
```





