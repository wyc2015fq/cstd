# 树形动态规划（树形DP）入门问题—初探 & 训练 - 在思索中前行！ - CSDN博客





2015年04月29日 22:40:03[_Tham](https://me.csdn.net/txl16211)阅读数：15949标签：[树形DP																[树形动态规划																[树状DP																[树状动态规划																[树形DP入门](https://so.csdn.net/so/search/s.do?q=树形DP入门&t=blog)
个人分类：[NOIP 动态规划																[数据结构](https://blog.csdn.net/txl16211/article/category/2409063)](https://blog.csdn.net/txl16211/article/category/2239693)





## 树形DP入门

poj 2342 Anniversary party   先来个题入门一下~

题意：

某公司要举办一次晚会，但是为了使得晚会的气氛更加活跃，每个参加晚会的人都不希望在晚会中见到他的直接上司，现在已知每个人的活跃指数和上司关系（当然不可能存在环），求邀请哪些人（多少人）来能使得晚会的总活跃指数最大。

解题思路：

任何一个点的取舍可以看作一种决策，那么状态就是在某个点取的时候或者不取的时候，以他为根的子树能有的最大活跃总值。分别可以用f[i,1]和f[i,0]表示第i个人来和不来。

当i来的时候，dp[i][1] += dp[j][0];//j为i的下属

当i不来的时候，dp[i][0] +=max(dp[j][1],dp[j][0]);//j为i的下属


```cpp
#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>

using namespace std;

#define maxn 6005

int n;
int dp[maxn][2],father[maxn];//dp[i][0]0表示不去，dp[i][1]1表示去了
bool visited[maxn];

void tree_dp(int node)
{
    int i;
    visited[node] = 1;
    for(i=1; i<=n; i++)
    {
        if(!visited[i]&&father[i] == node)//i为下属
        {
            tree_dp(i);//递归调用孩子结点，从叶子结点开始dp
            //关键
            dp[node][1] += dp[i][0];//上司来,下属不来
            dp[node][0] +=max(dp[i][1],dp[i][0]);//上司不来，下属来、不来
        }
    }
}

int main()
{
    int i;
    int f,c,root;
    while(scanf("%d",&n)!=EOF)
    {
        memset(dp,0,sizeof(dp));
        memset(father,0,sizeof(father));
        memset(visited,0,sizeof(visited));
        for(i=1; i<=n; i++)
        {
            scanf("%d",&dp[i][1]);
        }
        root = 0;//记录父结点
        bool beg = 1;
        while (scanf("%d %d",&c,&f),c||f)
        {
            father[c] = f;
            if( root == c || beg )
            {
                root = f;
            }
        }
        while(father[root])//查找父结点
            root=father[root];
        tree_dp(root);
        int imax=max(dp[root][0],dp[root][1]);
        printf("%d\n",imax);
    }
    return 0;
}
```

### ** dp专辑 A - Rebuilding Roads [ 树形dp]**
第二道树形dp的题，看别人的代码，看了一下午才明白，还没入门呀~

题意：

有n个点组成一棵树，问至少要删除多少条边才能获得一棵有p个结点的子树？

思路：

设dp[i][k]为以i为根，生成节点数为k的子树，所需剪掉的边数。

dp[i][1] = total(i.son) + 1，即剪掉与所有儿子（total(i.son)）的边，还要剪掉与其父亲（+1）的边。

dp[i][k] = min(dp[i][k],dp[i][j - k] + dp[i.son][k] - 2)，即由i.son生成一个节点数为k的子树，再由i生成其他j-k个节点数的子树。

这里要还原i与i.son和其父亲的边所以-2。


大牛动态转移方程


```cpp
dp[root][j] = min(dp[root][j], dp[root][j - k] + dp[tree[root][i]][k] - 2);
```
中的-2，始终想不明白，不是-1就可以了吗？在这里纠结了好久，后来结合



```cpp
for (int i = 1; i <= n; i++)
    {
        //为以i为根，生成节点数为1的子树所需剪掉的边数  每个结点都有个父结点 +1  根结点有个虚拟的父结点，方便统一处理
        dp[i][1] = tree[i].size() + 1;
        for (int j = 2; j <= p; j++)
            dp[i][j] = MAX;
    }
dp[root][p]--;
```
才明白了，顿感大牛们智商太高了，膜拜一会儿~后来仔细想想，这样也是为了方便处理问题~

```cpp
#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
using namespace std;

const int MAX = 10000;

vector <int> tree[160];
//设dp[i][k]为以i为根，生成节点数为k的子树  所需 剪掉 的 边数
int a, b, n, p, dp[160][160];
bool son[160];

void dfs(int root)
{
    int len=tree[root].size();
    for (int i = 0; i < len; i++)
    {
        dfs(tree[root][i]);//递归调用孩子结点（后根遍历）
        for (int j = p; j > 1; j--)//j==1 的情况已经存在 >1 即可
            for (int k = 1; k < j; k++)
                dp[root][j] = min(dp[root][j], dp[root][j - k] + dp[tree[root][i]][k] - 2);
    }
}

int main()
{
    scanf("%d %d",&n,&p);
    memset(son, false, sizeof(son));
    for (int i = 0; i < n - 1; i++)
    {
        scanf("%d %d",&a,&b);
        tree[a].push_back(b);
        son[b] = true;//记录b是否有儿子
    }
    int root = 1;
    while(son[root])//找父结点
        root++;
    for (int i = 1; i <= n; i++)
    {
        //为以i为根，生成节点数为1的子树所需剪掉的边数  每个结点都有个父结点 +1  根结点有个虚拟的父结点，方便统一处理
        dp[i][1] = tree[i].size() + 1;
        for (int j = 2; j <= p; j++)
            dp[i][j] = MAX;
    }
    dfs(root);
    dp[root][p]--;// 与dp方程中+2有关，还原i与其父亲的边，最后i为父节点，则-1
    int ans = MAX;
    for (int i = 1; i <= n; i++)
        ans = min(ans, dp[i][p]);
    printf("%d\n",ans);
    return 0;
}
```

### **hdu 1520（我的第一道树形DP，附详细的讲解）**


关于树形DP，我这今天早上才弄懂了一道题。之前一直觉得这是什么特别高端的东西，但是就这道题而言，无非就是一个数塔的操作放在树上了。


题目大意：

学校要开一个聚会。学校的教职工之间有上下级关系，为了让所有人开心，宴会组织者决定不会同时邀请一个人和他的上级（这让我想起我们昨天晚上聚餐李晔老师不来，她怕她来了我们放不开。。。。），对于每一个人，他能给聚会带来的欢乐度有一个值，问组织者该邀请哪些人能够使宴会的欢乐度达到最大值。

解题思路：

首先是DP的部分（也是很无聊的一部分）：每个参与者都有两种状态，一种是参加，一种是不参加。这个状态的后续影响就是如果他参加了，他的直接上司和直接下属都不能参加。我们可以用一个二维二态的数组来描述：dp[i][1]表示第i个参与者参加了，dp[i][0]表示第i个参与者没有参加。状态转移方程就是dp[i][1]=dp[i][1]+dp[i-1][0]，dp[i][0]=dp[i][0]+Max(dp[i-1][0],dp[i-1][1])。**这要是放在一个线性表上，同志们肯定都直接呵呵了。可所谓的树形DP就是把这个简单的操作放在树上了。**


树的部分：

之前很多大牛说图论的题目，难就难在一个建图。对这道题来说，DP是入门级的，很简单。但是这个建图让我纠结了许久。。。代码中是用静态链表的操作完成了一个父节点下面所有子节点的记录，对于一个子节点的操作完成之后，通过point[now].next指向下一个子节点，一直到point[now].next等于-1的时候，表示这个父节点下面所有的子节点已经遍历完成。返回父节点，再去操作这个父节点的兄弟节点。在这一点上，就是很直白的深搜的操作了。


理解代码的时候，个人建议把图画出来，再跟踪代码的数据，逐个去记录。反正我就是这么理解的。。。。。



```cpp
#include<stdio.h>
#include<string.h>
#define N 6005

struct node
{
    int pa,son;
    int next;
}point[N];
//其实从这个结构体就可以看出很多东西，这就是一个静态链表，在计算过程中遍历一个节点的所有子节点的操作也是和链表完全相同的。
//不过我这都是后知后觉啊。

int dp[N][2],list[N],flag[N],value[N];
int pos;
int Max(int x,int y)
{
    return x>y?x:y;
}
void add(int pa,int son)
{
    point[pos].pa=pa;
    point[pos].son=son;
    point[pos].next=list[pa];  //以静态链表的形式存储一个父节点下面所有的子节点。
    list[pa]=pos++;
    return ;
}
void dfs(int root)   //这道题说起来是树形DP，但是最没有讲的价值的就是这个DP。就是个入门级数塔的操作放在树上了。
{
    if(list[root]==-1)
    {
        dp[root][1]=value[root];
        dp[root][0]=0;
        return ;
    }
    int now=list[root];
    dp[root][0]=0;
    dp[root][1]=value[root];
    while(now!=-1)
    {
        dfs(point[now].son);
        dp[root][1]+=dp[point[now].son][0];  //既然取了父节点的值，子节点的值就不能再取了。
        
        //父节点的值没有取，子节点的值分取和不取两种情况，取其中较大的那种情况。
        dp[root][0]+=Max(dp[point[now].son][1],dp[point[now].son][0]); 
        
        now=point[now].next;//这个子节点计算过了，就要开始计算下一个子节点了。
    }
    return ;
}

int main()
{
    int i,n;
    while(scanf("%d",&n)!=EOF)
    {
        for(i=1;i<=n;i++)
            scanf("%d",&value[i]);//记录每一个点的值
        int a,b;
        pos=0;
        memset(list,-1,sizeof(list));
        memset(flag,0,sizeof(flag));
        while(scanf("%d%d",&a,&b),a+b)
        {
            add(b,a);    //将边加入树中
            flag[a]=1;   //记录a有父节点，不可能是祖节点。
        }
        a=1;
        while(flag[a]==1)
            a++;     //从a往后查，遇到的第一个flag[a]的值是-1的点，这就是大名鼎鼎的祖节点了。
        dfs(a);
        printf("%d\n",Max(dp[a][0],dp[a][1]));
    }
    return 0;
}
```

### **POJ1192 树形DP 最优连通子集**

Description

       众所周知，我们可以通过直角坐标系把平面上的任何一个点P用一个有序数对(x, y)来唯一表示，如果x, y都是整数，我们就把点P称为整点，否则点P称为非整点。我们把平面上所有整点构成的集合记为W。 定义1 两个整点P1(x1, y1), P2(x2, y2)，若|x1-x2| + |y1-y2| = 1，则称P1, P2相邻，记作P1~P2，否则称P1, P2不相邻。 定义 2 设点集S是W的一个有限子集，即S = {P1, P2,..., Pn}(n >= 1)，其中Pi(1 <= i <= n)属于W，我们把S称为整点集。
 定义 3 设S是一个整点集，若点R, T属于S，且存在一个有限的点序列Q1, Q2, ?, Qk满足: 1. Qi属于S（1 <= i <= k）; 2. Q1 = R, Qk = T; 3. Qi~Qi + 1(1 <= i <= k-1)，即Qi与Qi + 1相邻; 4. 对于任何1 <= i < j <= k有Qi ≠ Qj; 

我们则称点R与点T在整点集S上连通，把点序列Q1, Q2,..., Qk称为整点集S中连接点R与点T的一条道路。 定义4 若整点集V满足：对于V中的任何两个整点，V中有且仅有一条连接这两点的道路，则V称为单整点集。 定义5 对于平面上的每一个整点，我们可以赋予它一个整数，作为该点的权，于是我们把一个整点集中所有点的权的总和称为该整点集的权和。 

我们希望对于给定的一个单整点集V，求出一个V的最优连通子集B，满足： 1. B是V的子集 2. 对于B中的任何两个整点，在B中连通； 

3. B是满足条件(1)和(2)的所有整点集中权和最大的。

Input

第1行是一个整数N（2 <= N <= 1000），表示单整点集V中点的个数； 

以下N行中，第i行(1 <= i <= N)有三个整数，Xi, Yi, Ci依次表示第i个点的横坐标，纵坐标和权。同一行相邻两数之间用一个空格分隔。-10^6 <= Xi, Yi <= 10^6；-100 <= Ci <= 100。

Output

仅一个整数，表示所求最优连通集的权和。

Sample Input

5

0 0 -2

0 1 1

1 0 1

0 -1 1

-1 0 1

Sample Output

2


题目很繁琐，该题大意为：

给定一个平面整点集，点与点间在|x1-x2| + |y1-y2| = 1时相邻，且形成的图没有回路， 每个点有一个可正可负的权值，求最大权和连通子图。 


解题思路一：


```cpp
/*
给定的是一颗树，根据题意，我们可以从任意一个节点出发，必能访问到其他所有节点，那么dp的起点可以在任意一个节点。
我们从该起点出发，对以此点为根的树的每个分支进行搜索，采用树的后续遍历法则，对于每个子树来说，dp值首先加上根节点
（因为要保证连通性，所以返回值中必须包含根节点的值，即使为负数也必须加上）先对每个分支dp，然后看分支dp的返回值是不是正数，
如果是正数，那么我们就把该分支的返回值加入到该树中去。
就是每个子树的根节点（包括叶子节点）记录dp[i][0]与dp[i][1]，前一个表示不包含根的最大值，后一个表示包含根的最大值。
那么我们可以得到对于dp[i][0]，必然是所有分支中dp[child][0]与dp[child][1]中大于0的最大值的累加
（因为不包含树根，所以在根节点上的连通性不用保证）,
dp[i][1]必然是所有分支中dp[child][1]中大于0的最大值的累加再加上该树根本身的值（因为要保证连通性）。
最后只要比较dp[root][0]与dp[root][1]，输出较大。
*/
#include <stdio.h>
#include <iostream>
using namespace std;
int Abs(int x)
{
    return x>=0?x:-x;
}
int max(int x,int y,int z)
{
    if(x<y) x=y;
    if(x<z) x=z;
    return x;
}
int max(int x,int y)
{
    return x>y?x:y;
}
struct P
{
    int x,y,c;
    void input()
    {
        scanf("%d%d%d",&x,&y,&c);
    }
    bool isConnect(P & t)
    {
        if(Abs(x-t.x)+Abs(y-t.y)==1) return 1;
        return 0;
    }
}p[1005];
struct Edge
{
    int v,next;
}edge[10005];
int edgeNum,head[1005];
int dp[1005][2];
void addEdge(int u,int v)
{
    edge[edgeNum].v=v;
    edge[edgeNum].next=head[u];
    head[u]=edgeNum++;
}
void dfs(int x,int father)
{
    dp[x][0]=0;
    dp[x][1]=p[x].c;
    for(int i=head[x];i!=-1;i=edge[i].next)
    {
        if(edge[i].v!=father)
        {
            dfs(edge[i].v,x);
            dp[x][0]=max(dp[x][0],dp[edge[i].v][0],dp[edge[i].v][1]);
            if(dp[edge[i].v][1]>0)
            {
                dp[x][1]+=dp[edge[i].v][1];
            }
        }
    }
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=0;i<n;i++)
        {
            head[i]=-1;
            p[i].input();
        }
        edgeNum=0;
        for(int i=0;i<n;i++)
          for(int j=i+1;j<n;j++)
          {
              if(p[i].isConnect(p[j]))
              {
                  addEdge(i,j);
                  addEdge(j,i);
              }
          }
        dfs(0,-1);
        printf("%d\n",max(dp[0][0],dp[0][1]));
    }
}
```
解题思路2：


```cpp
//poj 1192 最优连通子集 (树型DP)
/*
题意：给定一个平面整点集，点与点间在|x1-x2| + |y1-y2| = 1时相邻，且形成的图没有回路，
      每个点有一个可正可负的权值，求最大权和连通子图。
题解：树型DP，dp[i][0]表示以i为根的子树上不包括i的最大权和，dp[i][1]表示包括i的最大权和。
*/
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;
const int inf = 1<<28;
int n,m;
struct Point
{
       int x,y,c;
}p[1010];

vector<int> con[1010];
int dp[1010][2],mark[1010];

void dfs(int v)
{
     mark[v]=1;
     dp[v][0]=0,dp[v][1]=p[v].c;
     int j;
     for (int i=0;i<con[v].size();i++)
     if (!mark[con[v][i]])
     {
         j=con[v][i];
         dfs(j);
         dp[v][0]=max(dp[v][0],max(dp[j][0],dp[j][1]));
         if (dp[j][1]>0) dp[v][1]+=dp[j][1];
     }
}

int main()
{
    scanf("%d",&n);
    for (int i=0;i<n;i++)
        scanf("%d%d%d",&p[i].x,&p[i].y,&p[i].c);
    for (int i=0;i<n;i++)
    for (int j=i+1;j<n;j++)
    if (abs(p[i].x-p[j].x)+abs(p[i].y-p[j].y)==1)
    {
          con[i].push_back(j);
          con[j].push_back(i);
    }
    dfs(0);
    printf("%d/n",max(dp[0][0],dp[0][1]));

    return 0;
}
```
解题思路3:


```cpp
/*
这道题着实花了很长时间才搞明白，对图的知识忘得太多了，看这道题之前最好先把图的邻接表表示方法看下，
有助于对这道题的理解，这道题让我对深度优先遍历又有了进一步的了解

这道题最好画个图，有助于理解，不过你要是画个错误的图此题就无解了

其实就是一个求无向树的所有子树和的最大值
树形dp
dp[i][0]表示以i为根，不包括i结点的子树获得最大权
dp[i][1]表示以i为根，包括i结点的子树获得的最大权
dp[i][0] = max(dp[k][0], dp[k][1]) k为i的所有孩子结点
dp[i][1] = i结点的权 + dp[k][1] 如果dp[k][1] > 0
*/
#include <stdio.h>
#include <cstring>
#include <stdlib.h>

using namespace std;

struct node    //这个名字起得不是很恰当，说是结点，其实是边，本题建的是有向边
{
    int u;     //边所指向的结点
    int next;  //与改变共起点的边，和邻接表表示图的方法差不多，只不过这里用的是一个int型的变量而不是一个指针
};

struct Point
{
    int x;
    int y;
    int c;
};
Point point[1015];
int head[1015];     //head[1]是以1开头的边
node edge[1015*10]; //表示边
int visited[1015];
int n;
int count=0;
int dp[1015][2];

void init()
{
    memset(head,-1,sizeof(head)); //把以任何一个点的开始的边都设置为一个无效的变量
}

void input()
{
    int i;
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%d %d %d",&point[i].x,&point[i].y,&point[i].c);
    }
}

void addEdge(int c ,int d)
{
    edge[count].u=d;         //这是一条有向边
    edge[count].next=head[c];
    head[c]=count++;

    edge[count].u=c;        //这是另一条有向边
    edge[count].next=head[d];
    head[d]=count++;
}

void buildTree()
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=i+1;j<n;j++)
        {
            if((abs(point[i].x - point[j].x) + abs(point[i].y - point[j].y)) == 1)
            {
                addEdge(i,j);    //添加一对有向边
            }
        }
    }
}

int max(int a,int b)
{
    return a>b?a:b;
}

void dfs(int u)
{
    visited[u]=1;
    dp[u][0]=0;
    dp[u][1]=point[u].c;
    for(int i=head[u];i!=-1;i=edge[i].next)
    {
        int v=edge[i].u;
        if(visited[v]==0)
        {
            dfs(v);

            dp[u][0]=max(dp[u][0],max(dp[v][0],dp[v][1]));//注意权值有正有负，这个语句即更新dp[u][0]
            if(dp[v][1]>0)    //当改点不选时就没有必要更新，因为值为1，当选的时候要更新该点，因为父节点要用到子结点的值
            {
            	/*这个语句更新dp[u][i],更新的方法其实差不多，u是父节点,v是子节点,当选u节点时，要加上其儿子，
				  若不选u节点时，比较自己和儿子节点
            	*/
                dp[u][1]=dp[u][1]+dp[v][1];
            }
        }
    }
}

int main()
{
    init();//初始化操作
    input();//解决输入问题
    buildTree();//构造树，采用邻接表的形式
    dfs(0);// 深度优先遍历
    printf("%d\n",max(dp[0][0],dp[0][1]));
    return 0;
}
```

### **hdu1561 树形DP The More The Better**
Problem Description

ACboy很喜欢玩一种战略游戏，在一个地图上，有N座城堡，每座城堡都有一定的宝物，在每次游戏中ACboy允许攻克M个城堡并获得里面的宝物。但由于地理位置原因，有些城堡不能直接攻克，要攻克这些城堡必须先攻克其他某一个特定的城堡。你能帮ACboy算出要获得尽量多的宝物应该攻克哪M个城堡吗？


Input

每个测试实例首先包括2个整数，N,M.(1 <= M <= N <= 200);在接下来的N行里，每行包括2个整数，a,b. 在第 i 行，a 代表要攻克第 i 个城堡必须先攻克第 a 个城堡，如果 a = 0 则代表可以直接攻克第 i 个城堡。b 代表第 i 个城堡的宝物数量, b >= 0。当N = 0, M = 0输入结束。


Output

对于每个测试实例，输出一个整数，代表ACboy攻克M个城堡所获得的最多宝物的数量。

Sample Input

3 2

0 1

0 2

0 3

7 4

2 2

0 1

0 4

2 1

7 1

7 6

2 2

0 0

Sample Output

5

13

思路：自己建立一个根root，使森林的根都成为root的孩子，然后树形dfs+简单背包

![](http://images.cnitblog.com/blog/402333/201301/11174425-f7fd3ff74ab24552ba01c24bd63f327c.png)


0-1背包裸代码：
for i=1..N
    for v=V..0
        f[v]=max{f[v],f[v-c[i]]+w[i]};

状态转移方程：f[root][k]=max(f[root][k],f[root][k-j]+dp[u][j]);

m是个数，j是存几个，f[i][j]表示的是以i为根攻克j个城堡（且这j个城堡必须是它子树上的，不包括它本身），dp[i][j]表示的是是以i为根攻克j个城堡（且这j个城堡必须是它子树上的，一定它本身，ans[i]表示每个城堡的宝物，所以一定有dp[i][1]=ans[i];）。
for(int k=m;k>=0;k--)
      for(int j=0;j<=k;j++)
           f[root][k]=max(f[root][k],f[root][k-j]+dp[u][j]);

更新f[root][0~m]数组，然后全部更新完之后更新dp[root][0~m]。
如图所示样例，先从root即0点访问3，3没有孩子，执行更新dp操作，因为所以叶子都满足dp[i][0~m]=ans[i]，所以dp[3][0~m]都等于ans[3]，以下同理。

返回到root，更新f[0][m~0]。
访问root-->2-->7-->6，访问到叶子，更新dp[6][0~m]。返回7，更新f[7][m~0]，
从7-->5，更新叶子节点dp[5][0~m]，
从5-->7，再次更新f[7][m~0]，
从7-->2，更新dp[7][0~m]，返回2节点，更新f[2][m~0]，
从2-->4，更新叶子节点dp[4][0~m]，
从4-->2，更新f[2][m~0]，
从2-->1，更新dp[1][0~m]，
从1-->2，更新f[2][m~0]，
从2-->root,更新dp[2][0~m]，
更新f[0][m~0]，更新dp[0][0~m]。

```cpp
#include<stdio.h>
#include<string.h>
#define N 205
int n,m,edgeNum=0;
int ans[N],dp[N][N],f[N][N];
int visit[N],head[N];
struct Line{int v,next;}edge[N];
int max(int a,int b){return a>b?a:b;}
void add(int u,int v)
{
    edge[edgeNum].v=v;
    edge[edgeNum].next=head[u];
    head[u]=edgeNum++;
}
void dfs(int root)
{
    visit[root]=1;
    for(int i=head[root];i!=-1;i=edge[i].next)
    {
        int u=edge[i].v;
        if(!visit[u])
        {
            dfs(u);
            for(int k=m;k>=0;k--)
              for(int j=0;j<=k;j++)
              f[root][k]=max(f[root][k],f[root][k-j]+dp[u][j]);
        }
    }
    for(int i=1;i<=m+1;i++)
      dp[root][i]=f[root][i-1]+ans[root];
}
int main()
{
    int a,b;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        if(n==0&&m==0) break;
        edgeNum=ans[0]=0;
        memset(f,0,sizeof(f));
        memset(dp,0,sizeof(dp));
        memset(head,-1,sizeof(head));
        memset(visit,0,sizeof(visit));
        for(int i=1;i<=n;i++)
        {
            scanf("%d%d",&a,&b);
            ans[i]=b;
            add(a,i);
        }
        dfs(0);
        printf("%d\n",dp[0][m+1]);
    }
}
```

## 树形 dp 初探


树形 dp 就是在一颗树上做 dp ，其基本思想是由子节点的信息推出父节点的信息。

由于树都是一般树，可能不只二叉，所以要用到一般树的“左儿子右兄弟”表示法（详见代码中的 first_child 和 next_sibling）。

### **hdu 1520 Anniversary party**
最基本的树形 dp 。

题目大意是，有一群人之间有上下级关系，在一个 party 中，有直接的上下级关系（即树中的父子关系）的人不能同时出席，每个人都有个 rating ，闻如何选择出席的人，使得所有人的 rating 之和最大。
每个节点有两个值，表示这个节点及其子节点所能取得的最大 rating 。max_with 是选择此节点的情况，max_without 不选择此节点。
有状态转移方程：


选择此节点的最大值 = 所有子节点不选择的最大值之和

不选择此节点的最大值 = 每个子节点选择或者不选择的最大值之和


mark：简单的树形dp题，从每个人的状态着手考虑，每个人要么参加，要么不参加，那么状态dp[i][0]代表参加，dp[i][1]代表不参加。

　　　 dp[i][0] += dp[son][1];

　　　 dp[i][1] += max(dp[son][1], dp[son][0]);



```cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int N = 6010;

struct node {

    int v;
    node *next;
}*head[N],tree[2*N];
int n,m,ptr;
int w[N],vst[N];
int dp[N][2];

int max(int a, int b) {return a > b ? a : b;}

void init()
{
    ptr = 1;
    memset(vst, 0, sizeof(vst));
    memset(dp, 0, sizeof(dp));
    memset(head, 0, sizeof(head));
}

void AddEdge(int x,int y)
{
    tree[ptr].v = y;
    tree[ptr].next = head[x],head[x] = &tree[ptr++];
}

void dfs(int v)   //v代表父亲节点
{
    vst[v] = 1;
    node *p = head[v];
    dp[v][0] = w[v];
    while (p != NULL)
    {
        if(vst[p->v]) {p = p->next; continue;}
        dfs(p->v);    //p->v代表v的儿子节点
        dp[v][0] += dp[p->v][1];
        dp[v][1] += max(dp[p->v][0], dp[p->v][1]);
        p = p->next;
    }
}

int main()
{
    int i,j,k;
    while(~scanf("%d", &n))
    {
        init();
        for(i = 1; i <= n; i++)
            scanf("%d", w+i);
        while(scanf("%d%d", &j, &k), j+k)
        {
            AddEdge(j, k);
            AddEdge(k, j);
        }
        dfs(1);
        printf("%d\n", max(dp[1][0], dp[1][1]));
    }
}
```](https://so.csdn.net/so/search/s.do?q=树状动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=树状DP&t=blog)](https://so.csdn.net/so/search/s.do?q=树形动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=树形DP&t=blog)




