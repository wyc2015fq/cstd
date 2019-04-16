# HDU 2255 奔小康赚大钱 带权二分图匹配 KM算法 - 紫芝的博客 - CSDN博客





2018年08月24日 16:06:29[紫芝](https://me.csdn.net/qq_40507857)阅读数：35








# 奔小康赚大钱

****Time Limit: 1000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 13984    Accepted Submission(s): 6102****

**Problem Description**

传说在遥远的地方有一个非常富裕的村落,有一天,村长决定进行制度改革：重新分配房子。

这可是一件大事,关系到人民的住房问题啊。村里共有n间房间,刚好有n家老百姓,考虑到每家都要有房住（如果有老百姓没房子住的话，容易引起不安定因素），每家必须分配到一间房子且只能得到一间房子。

另一方面,村长和另外的村领导希望得到最大的效益,这样村里的机构才会有钱.由于老百姓都比较富裕,他们都能对每一间房子在他们的经济范围内出一定的价格,比如有3间房子,一家老百姓可以对第一间出10万,对第2间出2万,对第3间出20万.(当然是在他们的经济范围内).现在这个问题就是村领导怎样分配房子才能使收入最大.(村民即使有钱购买一间房子但不一定能买到,要看村领导分配的).

**Input**

输入数据包含多组测试用例，每组数据的第一行输入n,表示房子的数量(也是老百姓家的数量)，接下来有n行,每行n个数表示第i个村名对第j间房出的价格(n<=300)。

**Output**

请对每组数据输出最大的收入值，每组的输出占一行。


**Sample Input**

```
2 
100 10 
15 23
```

**Sample Output**
`123`
**Source**

[HDOJ 2008 Summer Exercise（4）- Buffet Dinner](http://acm.hdu.edu.cn/search.php?field=problem&key=HDOJ+2008+Summer+Exercise%A3%A84%A3%A9-+Buffet+Dinner&source=1&searchmode=source)

**Recommend**

lcy

### 分析：

KM算法就是在**[匈牙利算法](https://blog.csdn.net/qq_40507857/article/details/82143611)**的基础上加了权值的束缚，为了达到权值和最大或者最小，就不能简单的去算边数

1.首先找到所有居民愿意花钱最多的那个房子，同时调节 lx，ly数组，使得权值和最大，或者当要松弛的时候，使得本来最大的矛盾权值和，尽可能的损失少一些，来得到满足条件的最大权值和

2.在 lx[x]+ly[y]=w[x][y]条件下进行匈牙利算法，既可以让居民住他花钱最多的房子，又可以在多居民抢一个房子的时候，用它来得到该居民到其他房子的松弛量（即该居民到其它房子 比 到这个用钱最多的房子 愿意花的钱数上差的值。）

那么我们就要把 发生矛盾的居民到 其它房子的松弛量  的最小值求出来。再用它去松弛，就可以让原本矛盾的最大权值和，损失最小而得到满足条件的最大权值和！

**对于每个居民有4个基本问题：**

1.这个房子访问过没有？

2.这个房子能不能满足他的条件

3.这个房子是否被别人住了

4.被别人住了能不能得到调配

```cpp
//HDU2255
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
#define maxn 310
#define INF 0x3f3f3f3f
#define clr(x)  memset(x,0,sizeof(x))
int w[maxn][maxn];//w[i][j]表示i到j的权值
int lx[maxn],ly[maxn];//同时调节两个数组，使得权值和最大
int n;
//n1,n2为二分图的顶点集，其中x属于n1,y属于n2
//link记录n2中的点y在n1中所匹配的x点的编号
int link[maxn];
int slack[maxn];//松弛操作
int visx[maxn],visy[maxn];
bool dfs(int x)
{
    visx[x]=1;//得到发生矛盾的居民集合
    //对于这个居民，每个房子都试一下，找到就退出
    for(int y=1;y<=n;y++){
        if(visy[y]) continue;//不需要重复访问
        int t=lx[x]+ly[y]-w[x][y];//这个条件下使用匈牙利算法
        if(t==0)//标志这个房子可以给这个居民
        {
            visy[y]=1;
//这个房子没人住或者可以让住着个房子的人去找另外的房子住
            if(link[y]==0||dfs(link[y]))
            {
                link[y]=x;
                return 1;//可以让这位居民住进来
            }
        }
        else if(slack[y]>t)//否则这个房子不能给这位居民
            slack[y]=t;
    }
    return 0;
}
int KM()
{
    clr(lx);
    clr(ly);
    clr(link);
    //首先把每个居民出的钱最多的那个房子给他
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
        if(lx[i]<w[i][j])
        lx[i]=w[i][j];

    //在满足上述条件之后，给第i位居民分配房子
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        slack[j]=INF;//松弛量
        while(1)//直到给这个居民找到房子为止
        {
            clr(visx);
            clr(visy);
            if(dfs(i))  break;//找到房子，就跳出循环
            int d=INF;
            for(int k=1;k<=n;k++)
                if(!visy[k]&&d>slack[k])
                d=slack[k];//找到最小松弛量
            for(int k=1;k<=n;k++)//松弛操作，使发生矛盾的居民有更多选择
            {
                if(visx[k]) lx[k]-=d;
                //将矛盾居民的要求降低，使发生矛盾的居民有更多

                if(visy[k]) ly[k]+=d;
                //使发生矛盾的房子在下一个子图，保持矛盾
            }
        }
    }
    int ans=0;
    for(int i=1;i<=n;i++)
        ans+=w[link[i]][i];
    return ans;
}
int main()
{
    while(~scanf("%d",&n)){
        clr(w);//每个案例都重置为0
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
            scanf("%d",&w[i][j]);//输入每条边的权值
        printf("%d\n",KM());
    }
    return 0;
}
```





