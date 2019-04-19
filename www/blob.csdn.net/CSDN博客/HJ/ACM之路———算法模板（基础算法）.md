# ACM之路———算法模板（基础算法） - HJ - CSDN博客
2016年08月20日 11:40:22[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：853
算法模板
一、DP 
二、博弈 
三、最小生成树 
四、最短路 
五、二分图匹配 
六、字符串匹配 
七、动态数据查询（线段树、树状数组） 
八、字典树 
九、强连通（tarjan） 
十、LCA倍增法 
十一、STL（map、优先队列、栈、堆排序） 
十二、高斯消元（整型）
一、基础DP类型
（1）**01背包**
```
for(int i=0;i<n;i++)
          for(int j=m;j>=w[i];j--)         //注意01背包用一维数组开的时候第二重循环一定要逆序！！！ 
              dp[j]=max(dp[j],dp[j-w[i]]+v[i]);       //每次两种决策：选择当前物品换来价值牺牲体积；或者不选择当前物品，毫无损失。
          cout<<dp[m]<<endl;    //输出最优解
```
（2）**多重背包**
```cpp
memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
          for(int j=1;j<=k[i];j++)       //多重背包，最简单的做法就是插入一个for循环 
            for(int p=m;p>=v[i];p--)         //多重背包化成01背包，开一维数组就一定要逆序循环！ 
              dp[p]=max(dp[p],dp[p-v[i]]+w[i]); 
        cout<<dp[m]<<endl;        //输出最优解
```
（3）**混合背包**
```cpp
memset(dp,0,sizeof(dp));        //初始化为0 
        for(int i=1;i<=k;i++)
          cin>>a[i]>>b[i];          //a存储经验值，b存储忍耐度 
        int i,ans=-1;            //如果最优解不存在，直接输出ans的值-1 
        for(int i=1;i<=m;i++)
          for(int j=1;j<=k;j++)
          {
              for(int p=1;p<=s;p++)
              {
                  if(i>=b[j])
                    dp[i][p]=max(dp[i][p],dp[i-b[j]][p-1]+a[j]);     //如果决策杀当前的怪，则消耗忍耐度b[j]，还剩下p-1个怪，但是增加了a[j]的经验值 
              }
              if(dp[i][s]>=n)   //如果存在第一个经验值不小于n，则这第一个存在的数据剩下的忍耐度一定是最大，保存数据并输出 
              {
                  ans=m-i;
                  break;
              }
          }
```
（4）**最大连续子序列**
```
memset(d,0,sizeof(d));
        for(int i=1;i<=n;i++) 
          d[i]=max(d[i-1]+a[i],a[i]);            //寻找最大子段和 
        int sum=d[1],y;
        for(int i=2;i<=n;i++)                 //寻找最大值并赋值给sum 
          if(sum<d[i])
            sum=d[i];
```
（5）**最大连续子序列和**
```
memset(d,0,sizeof(d));  // 来自 hdu Jumping
        for(int i=1;i<=n;i++)
        {
            int sum=a[i],k=a[i];
            for(int j=i-1;j>=0;j--)
              if(a[i]>a[j])
                 d[i]=max(d[j]+a[i],d[i]);        //每次决策取出当前前段递增序列和的最大值 
        }
        int max=d[0];
        for(int i=1;i<=n;i++)
          if(max<d[i])
            max=d[i];          //循环取出所有前段递增序列和的最大值
```
（6）**最长公共子串**
```cpp
len1=strlen(a);     
        len2=strlen(b);
        memset(d,0,sizeof(d));      //注意因为在输入的时候串是从下标0开始的，因此数组d在写状态跟转移方程的时候自动往后面加一位 
        for(int i=0;i<len1;i++)
          for(int j=0;j<len2;j++)
          {
              if(a[i]==b[j])            //如果两个串中存在相同字符 
                d[i+1][j+1]=d[i][j]+1;
              else                      //如果两个串中的字符不相同 
                d[i+1][j+1]=max(d[i][j+1],d[i+1][j]);      
          }
```
（7）**概率DP**
```
dp[0]=1.0;       //赋初值为概率1.0 
    for(int i=1;i<=m;i++)  
      for(int j=n;j>=a[i];j--)  
      {  
         dp[j]=min(dp[j],dp[j-a[i]]*b[i]);       //计算出不能得到offer的最小概率，用对立事件概率解 
      }
```
（8）**树形DP**
```cpp
vector<int>aa[6005];
    int indeg[6005],val[6005],f[6005][2],vis[6005],a,b;
    void dfs(int rr)
    {
        vis[rr]=1;       //标记数组 
        f[rr][0]=0;
        f[rr][1]=val[rr];
        for(int i=0;i<aa[rr].size();i++)
        {
            int v=aa[rr][i];       //寻找rr的孩子结点，v为下属 
            if(!vis[v])
            {
                dfs(v);     //递归调用孩子结点 
                f[rr][0]+=max(f[v][1],f[v][0]);      //上司不来，下属来或者不来 
                f[rr][1]+=f[v][0];                 //上司来，下属不来，计算所有孙子结点的个数 
            } 
        }
    }
```
（9）**数位DP**
```
//数位dp适合在一段数的区间内找出满足某些条件的数的个数，这个时候往往不能之间遍历，肯定会超时，则一般使用数位dp来解决
//数位dp的常见形式是dp[i][j],表示开头是j的i位数满足条件的有多少个，当然也有其他dp[i][j][k]等等，
//但i,j,k都很小，不会像直接遍历那么耗时
int dp[10][10];  
void init()  
{  
    memset(dp,0,sizeof(dp));  
    dp[0][0] = 1;  
    for(int i=1;i<=7;i++)  
      for(int j=0;j<10;j++)             //枚举第i位可能出现的数  
        for(int k=0;k<10;k++)        //枚举第i-1位可能出现的数   
          if(j!=4&&!(j==6&&k==2))  
            dp[i][j]  += dp[i-1][k];  
}  
int solve(int n)  
{  
    init();  
    int digit[10];  
    int len = 0;  
    while(n>0)  
    {  
        digit[++len] = n%10;  
        n/=10;  
    }  
    digit[len+1]=0;  
    int ans = 0;  
    for(int i=len;i;i--)  
    {  
        for(int j=0;j<digit[i];j++)  
          if(j!=4&&!(digit[i+1]==6&&j==2))  
            ans+=dp[i][j];  
        if(digit[i]==4||(digit[i]==2&&digit[i+1]==6))  
            break;  
    }  
    return  ans;  
}
```
二、博弈
（1）**巴什博弈**
```cpp
if(n<=m)
          cout<<"Grass"<<endl;
        else if(n%(m+1)==0)
          cout<<"Rabbit"<<endl;
        else
          cout<<"Grass"<<endl;
```
（2）**威佐夫博弈**
```cpp
bool solve(int a,int b)//判断(a,b)是否为奇异态  
{  
    double x=(1+sqrt(5))/2;  
    int n=b-a;  
    if(a==(int)(x*n))  
        return 1;  
    return 0;  
}  
int main()  
{  
    int a,b;  
    while(cin>>a>>b&&(a||b))  
    {  
        if(solve(a,b))  
        {  
            cout<<0<<endl;        //奇异态，先手必败  
            continue;  
        }  
        cout<<1<<endl;  
        for(int i=1;i<=a;i++)  
        {  
            int n=a-i;int m=b-i;  
            if(solve(n,m))  
                cout<<n<<" "<<m<<endl;  
        }  
        if(a!=b)              //相同时，可能会出现相同的状态，比如取第一堆会出现2 ，3 取第二堆也会出现 2 ，3  
        {  
            for(int i=1;i<=a;i++)  
            {  
                int n=a-i;int m=b;  
                if(solve(n,m))  
                    cout<<n<<" "<<m<<endl;  
            }  
           for(int i=1;i<=b;i++)  
            {  
                int n=a;int m=b-i;  
                if(n>m)  
                    swap(n,m);  
                if(solve(n,m))  
                    cout<<n<<" "<<m<<endl;  
            }  
        }  
    }  
    return 0;  
}
```
（3）**Nim博弈**
```cpp
int a,t=0;
        for(int i=0;i<n;i++)
        {
            scanf("%d",&a);
            t=t^a;       //其本质还是位运算
        }
        if(t==0)
          printf("Grass Win!\n");
        else
          printf("Rabbit Win!\n");
```
（4）**SG函数**
```cpp
const int N=10008;          //N为所有堆最多石子的数量  
int f[108],sg[N];           //f[]用来保存只能拿多少个，sg[]来保存SG值  
bool hash[N];               //mex{}  
void sg_solve(int t,int N)  
{  
    int i,j;  
    memset(sg,0,sizeof(sg));  
    for(i=1;i<=N;i++)  
    {  
        memset(hash,0,sizeof(hash));  
        for(j=1;j<=t;j++)  
          if(i - f[j] >= 0)  
            hash[sg[i-f[j]]] = 1;  
        for(j=0;j<=N;j++)  
          if(!hash[j])  
            break;  
        sg[i] = j;  
    }  
}  
int main()  
{  
    int k,m,l,num,i,j;  
    while(scanf("%d",&k),k)  
    {  
        for(i=1;i<=k;i++)  
            scanf("%d",&f[i]);  
        sg_solve(k,N);  
        scanf("%d",&m);  
        string ans="";  
        for( i=1;i<=m;i++)  
        {  
            int sum=0;  
            scanf("%d",&l);  
            for( j=1;j<=l;j++)  
            {  
                scanf("%d",&num);  
                sum^=sg[num];  
            }  
            if(sum==0)  
              ans+="L";  
            else  
              ans+="W";  
        }  
        cout<<ans<<endl;  
    }  
    return 0;  
}
```
（5）**对称博弈**
```cpp
//对于n<=2，先手必胜。n>3时，无论先手怎么取，
//后手都可以取一定的数目来构造出中心对称，这样后手一定是最后一次取光的。
        if(n>2)       //后手胜
          cout<<"Bob"<<endl;  
        else          //否则先手胜
          cout<<"Alice"<<endl;
```
三、最小生成树
（1）**Prim**
```
int visited[105],map[105][105],lowcost[105],n,m;
int prime(int v)
{
    int i,j,minn,k,sum=0;
    for(i=1;i<=n;i++)
     lowcost[i]=map[v][i];
    visited[v]=1;
    for(i=2;i<=n;i++)
    {
        minn=100000000;
        for(j=1;j<=n;j++)
          if(lowcost[j]<minn&&visited[j]==0)
          {
             minn=lowcost[j];
             k=j;
          }
        sum=sum+minn;
        visited[k]=1;
        for(j=1;j<=n;j++)
          if(lowcost[j]>map[k][j]&&visited[j]==0)
            lowcost[j]=map[k][j];
    }
    return sum;
}
```
（2）**Kruskal**
```cpp
const int N=105;
const int INF=1<<30;
struct edge
{
     int fr,to,w,nxt;
     bool operator < (const edge &a) const 
     {
        return w<a.w;
     }
}e[N*(N-1)+5];
int pre[N],n,head[N],cnt;
void add(int fr,int to,int w)
{
     e[cnt].fr=fr;
     e[cnt].to=to;
     e[cnt].w=w;
     e[cnt].nxt=head[fr];
     head[fr]=cnt++;
}
int fin(int x)
{
    if(x==pre[x])
      return x;
    return pre[x]=fin(pre[x]);
}
void Kruskal()
{
    for(int i=1;i<=n;++i)
      pre[i]=i;
    sort(e,e+cnt);
    int ans=0;
    for(int i=0;i<cnt;++i)
    {
         int u=fin(e[i].fr);
         int v=fin(e[i].to);
         if(u!=v)
         {
             ans+=e[i].w;
             pre[u]=v;
         }
    }
    printf("%d\n",ans);
}
```
四、最短路
（1）**Floyed**
```
//原理是动态规划
void floyed()  
{  
    for(int k=1;k<=nodeNum;k++)  
      for(int i=1;i<=nodeNum;i++)  
        for(int j=1;j<=nodeNum;j++)  
          if(mp[i][k]+mp[k][j]<mp[i][j])  
            mp[i][j]=mp[i][k]+mp[k][j];  
}
```
（2）**Djikstra**
```
// dijkstra算法求最短路，单源最短路，不能处理带有负权的图，思想为单源点加入集合，更新dis[]数组，
//每次取dis[]最小的那个点,贪心思想，加入集合，再次更新dis[]数组，取点加入集合，直到所有的点都加入集合中.
#define INF 0x7ffffff
int d[1005],vis[1005],g[1005][1005];
int n,m,a,b,len;
void Dijkstra()
{
    for(int i=1;i<=n;i++)
      d[i]=g[1][i];
    memset(vis,0,sizeof(vis));
    vis[1]=1;
    for(int i=1;i<=n;i++)
    {
        int mind=INF,mark;
        for(int j=1;j<=n;j++)       //从未选取顶点集中选取距离源点最小的顶点mark 
        {
            if(!vis[j]&&d[j]<mind)
            {
                mind=d[j];
                mark=j;
            }
        }
        vis[mark]=1;       //加入顶点集 
        for(int j=1;j<=n;j++)          //通过mark顶点修改源点到其他顶点的距离，松弛操作 
          if(!vis[j]&&d[mark]+g[mark][j]<d[j])
            d[j]=d[mark]+g[mark][j];
    }
}
```
（3）**Bellman_Ford**
```
//求单源到其它节点的最短路，可以处理含有负权的边，并且能判断图中是否存在负权回路（这一条在一些题中也有应用） ，
//无向图转化为有向图，边数加倍，构造边结构体，没用到邻接矩阵 
    const int maxNodeNum=110;           //最多节点个数    
    const int maxEdgeNum=10010;         //最多边条数    
    int nodeNum,edgeNum;                //节点，有向边个数    
    int dis[maxNodeNum];                //从单源点到各个点的距离    
    const int inf=0x3f3f3f3f;           //边的权重无穷大数    
    bool loop;                          //判断是否存在负权回路    
    struct Edge    
    {    
        int s,e,w;    
    }edge[maxEdgeNum*2];               //构造边，这里因为是无向图，要看成有向处理。    
    void bellman_ford(int start)    
    {    
        for(int i=1;i<=nodeNum;i++)    //第一步：赋初值    
            dis[i]=inf;    
        dis[start]=0;                   //第二步，对边进行松弛更新操作
        for(int i=1;i<=nodeNum-1;i++)   //最短路径为简单路径不可能含有环，图中最多有nodeNum-1条边    
        {    
            bool ok=0;    
            for(int j=1;j<=edgeNum;j++)    
            {    
                if(dis[edge[j].s]+edge[j].w<dis[edge[j].e])   //松弛    
                {    
                    dis[edge[j].e]=dis[edge[j].s]+edge[j].w;    
                    ok=1;    
                }    
            }    
            if(ok==0)    
                break;    
        }    
        loop=0;    
        for(int i=1;i<=edgeNum;i++)     //第三步，判断图中是否存在负权环  
          if(dis[edge[i].s]+edge[i].w<dis[edge[i].e])    
            loop=1;    
    }    
    int main()    
    {    
        while(cin>>nodeNum>>edgeNum&&(nodeNum||edgeNum))    
        {    
            int from,to,w;    
            int cnt=1;    
            for(int i=1;i<=edgeNum;i++)      //无向图，一条无向边看为两条有向边    
            {    
                cin>>from>>to>>w;    
                edge[cnt].s=edge[cnt+1].e=from;    
                edge[cnt].e=edge[cnt+1].s=to;    
                edge[cnt++].w=w;    
                edge[cnt++].w=w;           //切记，不能写成 edge[cnt++]=edge[cnt++].w；    
            }    
            edgeNum*=2;                    //无向图    
            bellman_ford(1);    
            cout<<dis[nodeNum]<<endl;    
        }    
        return 0;    
    }
```
（4）**SPFA**
```cpp
//对bellman_ford算法的优化，采用队列，在队列中取点对其相邻的点进行松弛操作。
//如果松弛成功且相邻的点不在队列中，就把相邻的点加入队列，被取的点出队，并把它的状态(是否在队列中) 
// 改为否，vis[i]=0,同一个节点可以多次进入队列进行松弛操作。
//dis[]数组初始化为inf,源点dis[start]=0  
    const int maxNodeNum=110;             //最多节点个数    
    const int maxEdgeNum=10010;           //最多边条数    
    const int inf=0x3f3f3f3f;             //边的权重无穷大数    
    int nodeNum,edgeNum;                  //节点，有向边个数    
    int dis[maxNodeNum];                  //从单源点到各个点的距离    
    bool vis[maxNodeNum];                 //某个节点是否已经在队列中    
    int mp[maxNodeNum][maxNodeNum];       //建立邻接矩阵    
    void SPFA(int start)    
    {    
        queue<int>q;    //第一步：建立队列，初始化vis,dis数组，并把源点加入队列中，修改其vis[]状态
        memset(vis,0,sizeof(vis));    //第二步：在队列中取点，把其vis状态设为0，对该点相邻的点（连接二者的边）进行松弛操作，修改相邻点的dis[] 
        for(int i=1;i<=nodeNum;i++)    
            dis[i]=inf;    
        dis[start]=0;     //并判断相邻的点vis[]状态是否为0(不存在于队列中),如果是，将其加入到队列中  
        q.push(start);    
        vis[start]=1;    
        while(!q.empty())    
        {    
            int from=q.front();    
            q.pop();    
            vis[from]=0;             //别忘了这一句   
            for(int i=1;i<=nodeNum;i++)    
            {    
                if(dis[from]+mp[from][i]<dis[i])    
                {    
                    dis[i]=dis[from]+mp[from][i];    
                    if(!vis[i])     //要写在松弛成功的里面    
                    {    
                        q.push(i);    
                        vis[i]=1;    
                    }    
                }    
            }    
        }    
    }    
    int main()   
    {    
        while(cin>>nodeNum>>edgeNum&&(nodeNum||edgeNum))    
        {    
            int from,to,w;    
            memset(mp,inf,sizeof(mp));          //初始化    
            for(int i=1;i<=edgeNum;i++)         //无向图，一条无向边看为两条有向边    
            {    
                cin>>from>>to>>w;    
                if(w<mp[from][to])    
                    mp[from][to]=mp[to][from]=w;    
            }    
            SPFA(1);    
            cout<<dis[nodeNum]<<endl;    
        }    
        return 0;    
    }
```
五、二分图匹配（匈牙利算法）
```cpp
const int maxn=502;         //来自 hdu 2063  坐过山车
int un,vn;              //左边顶点数，右边顶点数  
int g[maxn][maxn];      //邻接矩阵来存储边  
int linked[maxn];       //右边的点和左边的哪个点匹配  
bool vis[maxn];  
bool dfs(int u)  
{  
    for(int v=1;v<=vn;v++)     //遍历右边顶点  
    {  
        if(g[u][v]&&!vis[v])  
        {  
            vis[v]=1;  
            if(!linked[v]||dfs(linked[v]))  //右边顶点还没有被匹配，或者已经匹配的前面左边顶点可以去寻找另一个匹配而把该右边顶点“腾出空位”让给当前左边顶点u  
            {  
                linked[v]=u;  
                return true;  
            }  
        }  
    }  
    return false;  
}  
int hungary()  
{  
    int ans=0;  
    memset(linked,0,sizeof(linked));  
    for(int u=1;u<=un;u++)        //遍历左边顶点，去寻找与之匹配的右边顶点  
    {  
        memset(vis,0,sizeof(vis));  
        if(dfs(u))     //去找u能不能匹配,如果可以匹配的话,ans++  
            ans++;  
    }  
    return ans;  
}  
int k,m,n;    //m为题目中要求输入的左边顶点数,n为题目中要求输入的右边顶点数  
int main()  
{  
    while(scanf("%d",&k)!=EOF&&k)  
    {  
        scanf("%d%d",&m,&n);  
        un=m;vn=n;  
        memset(g,0,sizeof(g));  
        int u,v;  
        for(int i=1;i<=k;i++)  
        {  
            scanf("%d%d",&u,&v);//u和v有边  
            g[u][v]=1;  
        }  
        printf("%d\n",hungary());  
    }  
    return 0;  
}
```
六、字符串匹配
（1）**KMP**
```cpp
int cnt,n,m,last,nxt[1005];
char S[1005],T[1005];
void getNext()
{
    nxt[0]=nxt[1]=0;
    for(int i=1;i<m;i++)
    {
        int j=nxt[i];
        while(j&&T[i]!=T[j])
          j=nxt[j];
        nxt[i+1]=(T[i]==T[j])?j+1:0;
    }
}
void KMP()
{
    n=strlen(S);
    m=strlen(T);
    getNext();
    int j=0;
    for(int i=0;i<n;i++)
    {
        while(j&&S[i]!=T[j])
          j=nxt[j];
        if(S[i]==T[j])
          j++;
        if(j==m)
        {
            if(cnt==0)
            {
                cnt++;
                last=i;
            }
            else if(i-last>=m)   //保证这一次在母串中匹配成功的位置跟上一次在母串中匹配成功的位置不重复
            {
                cnt++;
                last=i;    //把这个一次在母串中匹配成功的最后一个字母位置记录下来，以作下一次比较
            }
        }
    }
}
```
（2）**Manacher**
```
const int maxn=1000010;     //来自 POJ Palindrome  求最长回文子串
char MA[maxn<<1];  
int MP[maxn<<1]; 
void Manacher(char s[],int len)  
{  
    int l=0;  
    MA[l++]='$';  
    MA[l++]='#';  
    for(int i=0;i<len;i++)  
    {  
        MA[l++]=s[i];  
        MA[l++]='#';  
    }  
    MA[l]=0;  
    int mx=0,id=0;  
    for(int i=0;i<l;i++)  
    {  
        MP[i]=mx>i?min(MP[2*id-i],mx-i):1;  
        while(MA[i+MP[i]]==MA[i-MP[i]])  
            MP[i]++;  
        if(i+MP[i]>mx)  
        {  
            mx=i+MP[i];  
            id=i;  
        }  
    }  
}  
char s[maxn]; 
int main()  
{  
    int c=1;  
    while(scanf("%s",s)==1)  
    {  
        if(s[0]=='E')  
            break;  
        int len=strlen(s);  
        Manacher(s,len);  
        int ans=0;  
        for(int i=0;i<2*len+2;i++)  
            ans=max(ans,MP[i]-1);  
        printf("Case %d: %d\n",c++,ans);  
    }  
    return 0;  
}
```
七、动态数据查询
（1）**树状数组（单点）**
```
int c[maxn],n;
int lowbit(int x)         //构造树状数组
{
    return x&(-x);
}
void Add(int x,int y)     //给树状数组更新值
{
    while(x<=n)
    {
        c[x]+=y;
        x+=lowbit(x);
    }
}
int sum(int x)           //求两个点之间的连续数值和
{
    int ans=0;
    while(x>0)
    {
        ans+=c[x];
        x-=lowbit(x);
    }
    return ans;
}
```
（2）**线段树（单点更新）**
```
int a,b;
struct node
{
    int ll,rr,max;          //ll表示左子节点，rr表示右子节点，max存储当前下标的最大值 
}ss[1200005];
int find(int l,int r,int root)      //查找 
{
    if(ss[root].ll==l&&ss[root].rr==r)
      return ss[root].max;
    int mid=(ss[root].ll+ss[root].rr)/2;
    if(r<=mid)
      find(l,r,root*2);
    else if(l>=mid+1)
      find(l,r,root*2+1);
    else
      return max(find(l,mid,root*2),find(mid+1,r,root*2+1));
}
void build(int l,int r,int root)    //建树 
{
    ss[root].ll=l;
    ss[root].rr=r;
    ss[root].max=0;
    if(l==r) return ;
    int mid=(l+r)/2;
    build(l,mid,root*2);
    build(mid+1,r,root*2+1);
}
void insert(int l,int r,int a,int root)     //插入数值（更新） 
{
    if(ss[root].ll==l&&ss[root].rr==r)
    {
        ss[root].max=a;
        return ;
    }
    else if(a>ss[root].max)
      ss[root].max=a;
    int mid=(ss[root].ll+ss[root].rr)/2;
    if(r<=mid)
      insert(l,r,a,2*root);
    else if(l>=mid+1)
      insert(l,r,a,2*root+1);
}
```
（3）**线段树（区间更新）**
```
#define imax 100005
int n,m,num[imax];
struct node
{
    int l,r;
    __int64 add,sum;
}f[imax*3];
void build(int t,int l,int r)    //建树
{
    f[t].l=l;
    f[t].r=r;
    f[t].add=0;
    if(l==r)
    {
        f[t].sum=num[l];
        return ;
    }
    int mid=(l+r)/2;
    build(t*2,l,mid);
    build(t*2+1,mid+1,r);
    f[t].sum=f[t*2].sum+f[t*2+1].sum;
} 
void Add(int t,int l,int r,int c)       //更新
{
    if(f[t].l==l&&f[t].r==r)
    {
        f[t].add+=c;
        return ;
    }
    f[t].sum+=c*(r-l+1);       //区间求和
    int mid=(f[t].l+f[t].r)/2;
    if(r<=mid)
      Add(t*2,l,r,c);
    else
    {
        if(l>mid)
          Add(t*2+1,l,r,c);
        else
        {
            Add(t*2,l,mid,c);
            Add(t*2+1,mid+1,r,c); 
        }
    }
}
__int64 query(int t,int l,int r)
{
    if(f[t].l==l&&f[t].r==r)
      return f[t].sum+f[t].add*(f[t].r-f[t].l+1);  
    int mid=(f[t].l+f[t].r)/2;
    f[2*t].add+=f[t].add;
    f[2*t+1].add+=f[t].add;
    f[t].sum+=f[t].add*(f[t].r-f[t].l+1);    //区间更新
    f[t].add=0;
    if(r<=mid)
      return query(2*t,l,r);
    else
    {
        if(l>mid)
          return query(2*t+1,l,r);
        else
          return query(2*t,l,mid)+query(2*t+1,mid+1,r);
    } 
}
```
八、字典树
```
#define Word_Len 50500  
#define Sigma_size 95  
struct Trie{  
    int ch[Word_Len][Sigma_size], sz;     //Word_Len是字典树的节点数 若都是小写字母Sigma_size=26 sz为当前节点数  
    int Have_word[Word_Len];      //这个节点下有几个单词  
    int val[Word_Len];                // 这个节点附带的信息，初始化为0表示这个节点不存在单词，所以节点带的信息必须!=0  
    int pre[Word_Len];    
    char he[Word_Len];    
    int Newnode()      //初始化字典树 
    {
        memset(ch[sz], 0, sizeof(ch[sz])); 
        val[sz]=Have_word[sz]=0; 
        return sz++;
    }  
    void init()          //初始化   
    { 
        sz = 0;
        Newnode();
    }
    int idx(char c)     //字符串编号 
    {
        return c-32;
    }      
    int insert(char *s){     //把v数字加给 s单词最后一个字母  
        int u = 0;  
        for(int i = 0; s[i]; i++)
        {  
            int c = idx(s[i]);  
            if(!ch[u][c])            //节点不存在就新建后附加  
            {  
                ch[u][c] = Newnode();         
                he[sz-1] = s[i];  
                val[sz-1] = val[u]+1;  
                pre[sz-1] = u;  
            }  
            u = ch[u][c];     
        }  
        Have_word[u]++;  
        return u;  
    }  
    int find_word(char *s){  
        int u = 0;  
        for(int i = 0; s[i]; i++)
        {  
            int c = idx(s[i]);  
            if(!ch[u][c])return 0;      //节点不存在  
            u = ch[u][c];  
        }  
        return Have_word[u];  
    }  
    void Have_name(char *s, ll now)
    {    
        int len = val[now], cc = now;    
        s[len--] = 0;    
        while(cc)    
        {    
            s[len--] = he[cc];    
            cc = pre[cc];    
        }    
    }  
}ac;
```
九、强连通（tarjan）
```cpp
#define N 30100  //N为最大点数  
    #define M 150100  //M为最大边数  
    int n, m;   //n m 为点数和边数  
    struct Edge{  
        int from, to, nex;  
        bool sign;      //是否为桥  
    }edge[M<<1];
    int head[N], edgenum; 
    void add(int u, int v)   //边的起点和终点  
    {
        Edge E={u, v, head[u], false};  
        edge[edgenum] = E;  
        head[u] = edgenum++;  
    }  
    int DFN[N], Low[N], Stack[N], top, Time;   //Low[u]是点集{u点及以u点为根的子树} 中(所有反向弧)能指向的(离根最近的祖先v) 的DFN[v]值(即v点时间戳)  
    int taj;                    //连通分支标号，从1开始  
    int Belong[N];              //Belong[i] 表示i点属于的连通分支  
    bool Instack[N];  
    vector<int> bcc[N];           //标号从1开始  
    void tarjan(int u ,int fa)
    {    
        DFN[u] = Low[u] = ++ Time ;    
        Stack[top ++ ] = u ;    
        Instack[u] = 1 ;    
        for (int i = head[u] ; ~i ; i = edge[i].nex )
        {    
            int v = edge[i].to ;    
            if(DFN[v] == -1)  
            {    
                tarjan(v , u) ;    
                Low[u] = min(Low[u] ,Low[v]) ;  
                if(DFN[u] < Low[v])  
                {  
                    edge[i].sign = 1;       //为割桥  
                }  
            }    
            else if(Instack[v]) 
              Low[u] = min(Low[u] ,DFN[v]) ;        
        }    
        if(Low[u] == DFN[u])
        {    
            int now;  
            taj ++ ; bcc[taj].clear();  
            do{  
                now = Stack[-- top] ;    
                Instack[now] = 0 ;   
                Belong [now] = taj ;  
                bcc[taj].push_back(now);  
            }while(now != u) ;  
        }  
    }  
    void tarjan_init(int all)
    {  
        memset(DFN, -1, sizeof(DFN));  
        memset(Instack, 0, sizeof(Instack));  
        top = Time = taj = 0;  
        for(int i=1;i<=all;i++)
          if(DFN[i]==-1 )
            tarjan(i, i);            //注意开始点标！！！  
    }  
    vector<int>G[N];  
    int du[N]; 
    void suodian()
    {  
        memset(du, 0, sizeof(du));  
        for(int i = 1; i <= taj; i++)
          G[i].clear();  
        for(int i = 0; i < edgenum; i++)
        {  
            int u = Belong[edge[i].from], v = Belong[edge[i].to];  
            if(u!=v)
            {
                G[u].push_back(v);
                du[v]++;  
            }
        }  
    }  
    void init()
    {
        memset(head, -1, sizeof(head));
        edgenum=0;
    }
```
十、LCA倍增法
```
#define N 10050  
struct Edge
{    
    int from, to, dis, nex;    
}edge[5*N];  
int head[N],edgenum,dis[N],fa[N][20],dep[N];     //fa[i][x] 是i的第2^x个父亲（如果超过范围就是根）  
void add(int u,int v,int w)
{    
    Edge E={u,v,w,head[u]};  
    edge[edgenum] = E;  
    head[u]=edgenum++;    
} 
void bfs(int root)
{    
    queue<int> q;    
    fa[root][0]=root;dep[root]=0;dis[root]=0;    
    q.push(root);    
    while(!q.empty())
    {    
        int u=q.front();q.pop();    
        for(int i=1;i<20;i++)fa[u][i]=fa[fa[u][i-1]][i-1];    
        for(int i=head[u]; ~i;i=edge[i].nex)
        {    
            int v=edge[i].to;
            if(v==fa[u][0])
              continue;       
            dep[v]=dep[u]+1;
            dis[v]=dis[u]+edge[i].dis;
            fa[v][0]=u;    
            q.push(v);  
        }    
    }    
} 
int Lca(int x,int y)
{    
    if(dep[x]<dep[y])
      swap(x,y);    
    for(int i=0;i<20;i++)
      if((dep[x]-dep[y])&(1<<i))
        x=fa[x][i];    
    if(x==y)
      return x;    
    for(int i=19;i>=0;i--)
      if(fa[x][i]!=fa[y][i])
      {
          x=fa[x][i];
          y=fa[y][i];
      }
    return fa[x][0];  
} 
void init()
{
    memset(head, -1, sizeof head);
    edgenum = 0;
}
```
十一、STL
（1）**map**
```cpp
map< string,map<string,int> > k;
map< string,map<string,int> >::iterator it;
map<string,int>::iterator its;
int main()
{
    int t,n,count;
    cin>>t;
    string name,place;
    while(t--)
    {
        cin>>n;
        while(n--)
        {
            cin>>name>>place>>count;
            k[place][name]+=count;
        }
        for(it=k.begin();it!=k.end();it++)
        {
            cout<<(*it).first<<endl;
            for(its=(*it).second.begin();its!=(*it).second.end();its++)
              cout<<"   |----"<<(*its).first<<"("<<(*its).second<<")"<<endl;
        }
        if(t)
          cout<<endl;
        k.clear();
    }
    return 0;
}
```
（2）**优先队列**
```cpp
priority_queue<int,vector<int>,greater<int> >q;
        int n,sum,x,y,m,k;
        cin>>n;
        for(int i=0;i<n;i++)
        {
            int k;
            cin>>k;
            q.push(k);
        } 
        sum=0;
        while(!q.empty())
        {
            x=q.top();
            q.pop();
            if(q.empty())
              break;
            y=q.top();
            q.pop();
            m=x+y;
            sum=sum+m;
            q.push(m);
        }
        cout<<sum<<endl;
```
（3）**栈**
```
char a[1005];
stack<int> s;
while(cin>>a)
{
    int len=strlen(a);
    for(int i=0;i<len;i++)
    {
        if(a[i]=='(')
          s.push(1);
        else if(a[i]==')')
          s.pop();
        else
          break;
    }
    int i=0;
    while(!s.empty())
    {
        s.pop();
        i++;
    }
    cout<<i<<endl;
}
```
（4）**堆排序**
```
void HeapAdjust(int *a,int i,int size)   //调整堆 
{
    int lchild=2*i;       //i的左孩子节点序号 
    int rchild=2*i+1;     //i的右孩子节点序号 
    int max=i;            //临时变量 
    if(i<=size/2)         //如果i不是叶节点就不用进行调整 
    {
        if(lchild<=size&&a[lchild]>a[max])
          max=lchild;
        if(rchild<=size&&a[rchild]>a[max])
          max=rchild;
        if(max!=i)
        {
            swap(a[i],a[max]);
            HeapAdjust(a,max,size);      //避免调整之后以max为父节点的子树不是堆 
        }
    }
}
void BuildHeap(int *a,int size)      //构建堆 
{
    for(int i=size/2;i>=1;i--)     //非叶节点最大序号值为size/2 
      HeapAdjust(a,i,size);
}
void HeapSort(int *a,int size)      //堆排序 
{
    BuildHeap(a,size);
    for(int i=size;i>=1;i--)
    {
        swap(a[1],a[i]);      //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
        BuildHeap(a,i-1);      //将余下元素重新建立为大顶堆 
        HeapAdjust(a,1,i-1);   //重新调整堆顶节点成为大顶堆 
    }
}
```
十二、高斯消元（整型）
```cpp
const int maxn=105;    
int equ,var;             // 有equ个方程，var个变元。增广阵行数为equ, 分别为0到equ - 1，列数为var + 1，分别为0到var.    
int a[maxn][maxn];    
int x[maxn];             // 解集.    
bool free_x[maxn];       // 判断是否是不确定的变元.    
int free_num;   
void Debug(void)    
{    
    int i,j;    
    for(i=0;i<equ;i++)    
    {    
        for(j=0;j<var+1;j++)      
          cout<<a[i][j]<<" ";       
        cout<<endl;    
    }    
    cout<<endl;    
}   
inline int gcd(int a, int b)    
{    
    int t;    
    while (b!=0)    
    {    
        t=b;    
        b=a%b;    
        a=t;    
    }    
    return a;    
}    
inline int lcm(int a, int b)    
{    
    return a*b/gcd(a,b);    
}  
// 高斯消元法解方程组(Gauss-Jordan elimination).(-2表示有浮点数解，但无整数解，-1表示无解，0表示唯一解，大于0表示无穷解，并返回自由变元的个数)    
int Gauss(void)    
{    
    int i,j,k;    
    int max_r;            // 当前这列绝对值最大的行.    
    int col;              // 当前处理的列.    
    int ta,tb;    
    int LCM;    
    int temp;    
    int free_x_num;    
    int free_index;        // 转换为阶梯阵.    
    col=0;                  // 当前处理的列.    
    for(k=0;k<equ&&col<var;k++,col++)              // 枚举当前处理的行.
    {                          
        max_r=k;    
        for(i=k+1;i<equ;i++)                      // 找到该col列元素绝对值最大的那行与第k行交换.(为了在除法时减小误差)    
          if(abs(a[i][col])>abs(a[max_r][col]))    
            max_r=i;    
        if(max_r!=k)                              // 与第k行交换   
          for(j=k;j<var+1;j++)    
            swap(a[k][j],a[max_r][j]);      
        if(a[k][col]==0)                          // 说明该col列第k行以下全是0了，则处理当前行的下一列. 
        {   
            k--;
            continue;    
        }    
        for(i=k+1;i<equ;i++)                      // 枚举要删去的行.    
          if (a[i][col]!=0)    
          {    
                LCM=lcm(abs(a[i][col]),abs(a[k][col]));    
                ta=LCM/abs(a[i][col]);
                tb=LCM/abs(a[k][col]);    
                if(a[i][col]*a[k][col]<0)         // 异号的情况是两个数相加.
                  tb=-tb;     
                for(j=col;j<var+1;j++)       
                  a[i][j]=a[i][j]*ta-a[k][j]*tb;      
          }      
    }     
    // 1. 无解的情况: 化简的增广阵中存在(0, 0, ..., a)这样的行(a != 0).    
    for(i=k;i<equ;i++)       // 对于无穷解来说，如果要判断哪些是自由变元，那么初等行变换中的交换就会影响，则要记录交换.    
      if (a[i][col]!=0)    
        return -1;       
    // 2. 无穷解的情况: 在var * (var + 1)的增广阵中出现(0, 0, ..., 0)这样的行，即说明没有形成严格的上三角阵.    
    if(k<var)                        // 且出现的行数即为自由变元的个数.
    {    
        for (i=k-1;i>=0;i--)         // 首先，自由变元有var - k个，即不确定的变元至少有var - k个. 
        {    
            // 第i行一定不会是(0, 0, ..., 0)的情况，因为这样的行是在第k行到第equ行.    
            // 同样，第i行一定不会是(0, 0, ..., a), a != 0的情况，这样的无解的.    
            free_x_num=0;     // 用于判断该行中的不确定的变元的个数，如果超过1个，则无法求解，它们仍然为不确定的变元.    
            for(j=0;j<var;j++)      
              if(a[i][j]!=0&&free_x[j]) 
              {
                 free_x_num++;
                 free_index = j;  
              }
            if(free_x_num>1)                   // 无法求解出确定的变元. 
              continue;    
            // 说明就只有一个不确定的变元free_index，那么可以求解出该变元，且该变元是确定的.    
            temp=a[i][var];    
            for(j=0;j<var;j++)     
              if(a[i][j]!=0&&j!=free_index)    
                temp-=a[i][j]*x[j];       
            x[free_index]=temp/a[i][free_index]; // 求出该变元.    
            free_x[free_index]=0;          // 该变元是确定的.    
        }    
        return var-k;                     // 自由变元有var - k个.    
    }    
    // 3. 唯一解的情况: 在var * (var + 1)的增广阵中形成严格的上三角阵.    
    for (i=var-1;i>=0;i--)               // 计算出Xn-1, Xn-2 ... X0.   
    {    
        temp=a[i][var];    
        for(j=i+1;j<var;j++)       
          if(a[i][j]!=0)     
            temp-=a[i][j]*x[j];      
        if(temp%a[i][i]!=0)     
            return -2;                  // 说明有浮点数解，但无整数解.    
        x[i]=temp/a[i][i];    
    }    
    return 0;    
}    
int main()    
{    
    int i, j;    
    while (scanf("%d %d",&equ,&var)!=EOF)    
    {    
        memset(a,0,sizeof(a));    
        memset(x,0,sizeof(x));    
        memset(free_x,1,sizeof(free_x));   // 一开始全是不确定的变元    
        for(i=0;i<equ;i++)                 //构造增广矩阵    
          for(j=0;j<var+1;j++)    
            scanf("%d",&a[i][j]);        
        free_num=Gauss();    
        if(free_num==-1)
           printf("无解!\n");    
        else if(free_num==-2) 
          printf("有浮点数解，无整数解!\n");    
        else if(free_num>0)    
        {    
            printf("无穷多解! 自由变元个数为%d\n",free_num);    
            for(i=0;i<var;i++)    
            {    
                if(free_x[i])
                  printf("x%d 是不确定的\n",i+1);    
                else 
                  printf("x%d: %d\n",i+1,x[i]);    
            }    
        }    
        else    
        {    
            for(i=0;i<var;i++)    
              printf("x%d: %d\n",i+1,x[i]);    
        }    
        printf("\n");    
    }    
    return 0;    
}
```
