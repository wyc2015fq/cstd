# 图论 —— 2-SAT 问题 - Alex_McAvoy的博客 - CSDN博客





2019年03月15日 20:59:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：253








# 【问题概述】

2-SAT问题是这样的：有n个布尔变量xi，另有m个需要满足的条件，每个条件的形式都是“xi为真/假或者xj为真/假“

SAT 是适定性(Satisfiability)问题的简称，一般形式为：k-适定性问题，简称：k-SAT。

当 k>2 时，k-SAT 是 NP 完全的，因此一般讨论的是 k=2 的情况，即：2-SAT 问题。

关于 2-SAT 问题，简单的来说就是给出 n 个集合，每个集合中有两个元素，然后从每个集合中选出一个元素，一共选 n 个两两不矛盾的元素， 显然可能有多种选择方案，一般题目只需要求任意输出一种即可。

简单来说，就是给出一个由 n 个布尔值组成的序列 A，再给出 m 个限制关系，每个条件的形式都是 Xi 为真/假 或 Xj 为真/假（比如：A[x] AND A[y]=0、A[x] OR A[y] OR A[z]=1 等），来确定 A[0..n-1] 的值，使得其满足所有限制关系，这样的问题就是 SAT 问题，特别的，若每种限制关系中最多只对两个元素进行限制，则称为 2-SAT 问题。

# 【基本原理】

由于在 2-SAT 问题中，最多只对两个元素进行限制，所以可能的限制关系共有 11 种：
- A[x]：A[x]
- NOT A[x]：非 A[x]
- A[x] AND A[y]：A[x] 与 A[y]
- A[x] OR A[y]：A[x] 或 A[y]
- A[x] XOR A[y]：A[x] 异或 A[y]
- A[x] AND NOT A[y]：A[x] 与 非A[y]
- A[x] OR NOT A[y]：A[x] 或 非A[y]
- A[x] XOR NOT A[y]：A[x] 异或 非A[y]
- NOT (A[x] AND A[y])：非(A[x] 与 A[y])
- NOT (A[x] OR A[y])：非(A[x] 或 A[y])
- NOT (A[x] XOR A[y])：非(A[x] 异或 A[y])

进一步来说，**A[x] AND A[y]** 相当于 (A[x]) AND (A[y])，也就是可以拆分成 A[x] 与 A[y] 两个限制关系；**NOT (A[x] OR A[y])** 相当于 NOT A[x] AND NOT A[y]，也就是可以拆分成 NOT A[x] 与 NOT A[y] 两个限制关系。因此，可能的限制关系最多只有9种。

在实际问题中，2-SAT 问题大多数表现为以下形式：给出 n 对物品，每对物品必须选取一个且只能选一个，而且给出它们之间存在的某些限制关系，如：某两个物品不能都选、某两个物品不能都不选等等，这时可以将每对物品当成一个布尔值（选取第一个取 0，选取第二个取 1），如果所有与的限制关系最多只对两个物品进行限制，则它们都可以转换成 9 种基本限制模型，从而转换为 2-SAT 问题模型。

# 【问题模型】

2-SAT 的解法有多种方式，这里采用一种比较容易理解的，且效率较高的模型。

构造一个 2*n 阶的有向图，将开始的 n 个变量的每个变量 Xi 拆分成两个结点 2i、2i+1，从 0 开始编号到 2n-1 号，其中原始第 i 个物品对应 2i 与 2i+1这样就将 n 个点分成了 n 对。

再设一标志数组，vis[2i]、vis[2i+1] 分别表示 Xi 为假和 Xi 为真，显然每对点必须且只能取一个，且每对点对应了布尔序列 A 的一个元素的 0、1 取值，然后再为每个变量选其中一个点进行标记（比如：标记了 vis[2i]，表示 Xi 为假，标记了 vis[2i+1]，表示 Xi 为真）。 

标记完成后，根据题设要求加入存在 i 假或 j 假的论述，即在图中构造有向边：若图中存在边 <i,j>，则表示若选了 i 则必须选 j，将代表 A[i] 取 0 的点记为 i，取 1 的点记为 i'，可以发现，上述的 9 种限制关系中，前 2 种一元关系可通过连一条边来实现，后 7 种二元关系可通过连两条边来实现。

比如：对于 A[x]，可以通过连边 <x',x> 实现，NOT A[x]，可以通过连边 <x,x'> 来实现，对于 NOT(A[x] AND A[y]) 需要连两条边 <x, y'> 和 <y, x'> 来实现，对于 A[x] OR A[y] 需要连两条边 <x', y> 和 <y', x> 来实现。

因此，对于 “Xi 为假或者 Xj 为假“ 这种二元关系条件，就连一条有向边，<2i+1,2j>，表示若标记了 Xi 为真，那么 Xj 就必须要为假，同理，还得另外一条有向边 <2j+1,2i>，表明如果 Xj 为真，Xi 必须为假。

接下来考虑每个没有赋值的变量，将其设为 Xi，先假设其为假标记结点 2i，接着沿着有向边标记所有能标记的点，若标记过程中发现某个变量对应的两个结点都被标记，则 Xi 为假这个假设是不成立的，需要改为 Xi 为真，然后重新标记，在这个过程中，是没有回溯的。比如要设定 i 为假，那么不是 vis[2i]=true，而是添加一条边 <2i+1,2i>，即只要 i 设为真了，那么就会使得到处矛盾，因为每个节点只有两种选择，因此添边的思路更加直观。

在这个多次 dfs 来设置每个节点可能的值的过程中，只需要判断是否所有可能取值情况是否会引起冲突，若当前考虑的变量无论赋值为真还是假都会引起矛盾的话，则可说明整个 2-SAT 问题无解。

# 【模版】

使用方法：先将题目中所给的条件添加进 G[N] 中（若没有可省略该步），根据约束条件添加新的条件，再使用 twoSAT() 判断该 2-SAT 问题有无解即可

```cpp
#include<cstdio>
#include<algorithm>
#include<vector>
#define N 1001
bool vis[N*2];//标记数组,vis[2*x]被标记代表x为假,vis[2*x+1]被标记代表x为真
int Stack[N*2],top;//记录一次dfs遍历的所有点的编号
vector<int> G[N*2];//G[i]=j表示如果vis[i]=true,那么vis[j]=true
void init(int n){
    memset(vis,false,sizeof(vis));
    for(int i=0;i<2*n;i++)
        G[i].clear();
}
//x=xVal OR y=yVal,表或的关系
void addOrClause(int x,int xVal,int y,int yVal){
    x=x*2+xVal;
    y=y*2+yVal;
    G[x^1].push_back(y);
    G[y^1].push_back(x);
}
//x=xVal AND y=yVal,表且的关系
void addAndClause(int x,int xval,int y,int yval) {
    x=x*2+xval;
    y=y*2+yval;
    G[x].push_back(y);
}
bool dfs(int x){//从x开始dfs,标记途径的所有点

    //当前需要标记点是x,若x^1这个结点已被标记,就表示两个点都要被标记掉
    //若冲突,则返回false
    if(vis[x^1])
        return false;

    if(vis[x])
        return true;
    vis[x]=true;

    //模拟栈
    Stack[top++]=x;

    //与x相邻的点都进行标记,以判断是否出错
    for(int i=0;i<G[x].size();i++)
        if(!dfs(G[x][i]))
            return false;

    return true;
}
bool twoSAT(int n){//判断当前2-SAT问题是否有解
    for(int i=0;i<2*n;i+=2){
    //若在其他点标志过程中,其中一个被标记,就无需再进行枚举
    //因为在dfs的过程中,和其相连的点也被标记,较少了重复枚举
        if(!vis[i] && !vis[i+1]){
            top=0;

            if(!dfs(i)){//假设xi为假是错误的,此时要进行回溯
                while(top>0)//将dfs过程中标记过的点还原
                    vis[Stack[--top]]=false;

                if(!dfs(i+1))//再次判断对错
                    return false;
            }
        }
    }
    return true;
}
int main(){
    int n;//原始图的点数
    while(scanf("%d",&n)!=EOF&&(n)){
        int m;//限制关系数
        scanf("%d",&m);
        
        init(n);
        
        for(int i=0;i<m;i++){
            int a,b;//限制关系的相应编号1~n
            int aVal,bVal;//限制关系的值0或1
            scanf("%d%d",&a,&b);
            scanf("%d%d",&aVal,&bVal);
            
            //处理编号,将由1~n改为由0~n-1
            a=abs(a)-1;
            b=abs(b)-1;

            //根据题目限制关系添加或关系、与关系
            addOrClause(a,aVal,b,bVal);
            addAndClause(a,aVal,b,bVal);
            
        }
        
        bool res=twoSAT(n);//进行结果判断
        if(res)
            printf("1\n");
        else
            printf("0\n");
    }
    return 0;
}
```

# 【例题】
- Party（HDU-3062）**(入门题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84978198)
- Let's go home（HDU-1824）**(入门题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84977494)
- Wedding（POJ-3648）**(入门题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84978884)
- Perfect Election（POJ-3905）**(入门题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84958813)
- Katu Puzzle（POJ-3678）**(操作符的分析)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84978199)
- Ikki's Story IV - Panda's Trick（POJ-3207）**(建图+范围判断)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84978885)
- Priest John's Busiest Day（POJ-3683）**(或关系的应用)**：[点击这里](https://blog.csdn.net/u011815404/article/details/85017622)
- Go Deeper（HDU-3715）**(2SAT+二分)**：[点击这里](https://blog.csdn.net/u011815404/article/details/85064327)
- Building roads（POJ-2749）**(2SAT+二分)**：[点击这里](https://blog.csdn.net/u011815404/article/details/85050767)
- Eliminate the Conflict（HDU-4115）**(三元且关系)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84962882)
- Get Luffy Out（POJ-2723）**(且关系的枚举)**：[点击这里](https://blog.csdn.net/u011815404/article/details/84978886)
- Peaceful Commission（HDU-1814）**(最小字典序)**：[点击这里](https://blog.csdn.net/u011815404/article/details/85017621)
- 处女座与宝藏（2019牛客寒假算法基础集训营 Day2-F）**(Tarjan 求解 2SAT)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86636204)



