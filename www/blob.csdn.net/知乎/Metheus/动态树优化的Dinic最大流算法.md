# 动态树优化的Dinic最大流算法 - 知乎
# 

**概述**

![Dinic's Algorithm](https://www.zhihu.com/equation?tex=Dinic%27s+Algorithm) 是一个用于解决网路流最大流问题的多项式算法，它的时间上界为 ![O(V^2E)](https://www.zhihu.com/equation?tex=O%28V%5E2E%29) ，在一般图上表现十分优秀。

Dinic 的实现基于增广路算法,在此基础上，加上了距离标号优化。

给定图 ![G](https://www.zhihu.com/equation?tex=G) 以及源点 ![s](https://www.zhihu.com/equation?tex=s) ，汇点 ![t](https://www.zhihu.com/equation?tex=t) 。Dinic的算法流程如下：

- 对 ![G](https://www.zhihu.com/equation?tex=G) 采用广度优先搜索给每个节点标号，标号的内容为到源点 ![s](https://www.zhihu.com/equation?tex=s) 的经过的最短边数，这样就把原图构建成了一个分层图 ![G_L](https://www.zhihu.com/equation?tex=G_L) ，其中 ![G_L](https://www.zhihu.com/equation?tex=G_L) 的每条边均从第 ![i](https://www.zhihu.com/equation?tex=i) 层连向第 ![i+1](https://www.zhihu.com/equation?tex=i%2B1) 层。
- 在 ![G_L](https://www.zhihu.com/equation?tex=G_L) 上不断寻找增广路，删除零流边。
- 如果遇到了点 ![v](https://www.zhihu.com/equation?tex=v) ，which没有任何出边，那么从图中删除点 ![v](https://www.zhihu.com/equation?tex=v) 的所有入边。
- 如果源点没有任何出边，算法结束，输出流；否则再次将残余图再次分层，重复步骤2。

按此步骤得出的最终流一定是最大流。

## ** Dinic算法的时间复杂度**

Dinic算法分为两部分，分层以及增广。

每一次分层，需要执行一次BFS操作，时间复杂度为 ![O(V+E)](https://www.zhihu.com/equation?tex=O%28V%2BE%29) 。

BFS之后还包含若干次增广。根据增广路的性质，每一次增广都至少会产生一个零边，所以增广次数上界为 ![O(E)](https://www.zhihu.com/equation?tex=O%28E%29) 。每一次增广都会搜寻到一条从 ![s](https://www.zhihu.com/equation?tex=s) 到 ![t](https://www.zhihu.com/equation?tex=t) 的路径，路径的长度上界为 ![O(V)](https://www.zhihu.com/equation?tex=O%28V%29) ，所以每一次分层的时间复杂度为 ![O(VE)+O(V+E)=O(VE)](https://www.zhihu.com/equation?tex=O%28VE%29%2BO%28V%2BE%29%3DO%28VE%29) 。

因为最多会进行 ![O(V)](https://www.zhihu.com/equation?tex=O%28V%29) 次分层，所以总的时间上界为 ![O(V^2E)](https://www.zhihu.com/equation?tex=O%28V%5E2E%29) 。

## **优化**

我们发现，在寻找增广路时，我们会重复经过之前已经走过的路径，如下图所示：
![](https://pic2.zhimg.com/v2-82e7c8963078e5d8e78ca6913d229cdd_b.jpg)将要进行增广的图G![](https://pic3.zhimg.com/v2-9e49d574c86516a1f3f3f9788ab117fa_b.jpg)橙色为增广路径
在图中进行了一次增广，并删除了一条零边。
![](https://pic1.zhimg.com/v2-064e67c621aa326a2d2ca9bba66c82c0_b.jpg)蓝色为第二次增广路径，紫色为两次增广的重复路径
在第二次增广时，我们发现这一次增广的路径与上一次增广路径有重合。

在绝大多数的图中，这样重复的路径不会太长，故Dinic算法不会在这上面浪费太多的时间。这使得Dinic算法虽然运行时间上界为 ![O(V^2E)](https://www.zhihu.com/equation?tex=O%28V%5E2E%29) ，但是在实际运行中跑得飞快（雾）。

正如有特殊数据可以卡掉普通二叉搜索树一般，也有如下特殊构造的图可以使Dinic算法运行时间达到上界：
![](https://pic1.zhimg.com/v2-0b30162ab30595e4e86890964506f954_b.jpg)总共n个点，左右各分一半。总共会增广n/2次，每一次增广都会遍历n/2个点
我们考虑记下已经遍历过的路径，在下一次增广的时候使用上一次增广留下的信息而不是重新遍历。这里我们利用寻找路径的顺序，采用动态树来维护。

我们需要一个能够实现如下功能的动态树：
- ![make\_tree(v)](https://www.zhihu.com/equation?tex=make%5C_tree%28v%29)  返回一个由 ![v](https://www.zhihu.com/equation?tex=v) 组成的树。
- ![find\_root(v)](https://www.zhihu.com/equation?tex=find%5C_root%28v%29) 返回 ![v](https://www.zhihu.com/equation?tex=v) 所在树的根。
- ![link(u,v)](https://www.zhihu.com/equation?tex=link%28u%2Cv%29) 连接以 ![u](https://www.zhihu.com/equation?tex=u) 为根的树与 ![v](https://www.zhihu.com/equation?tex=v) 。
- ![cut(v)](https://www.zhihu.com/equation?tex=cut%28v%29) 断开 ![v](https://www.zhihu.com/equation?tex=v) 与父亲的连边。
- ![find\_min(v)](https://www.zhihu.com/equation?tex=find%5C_min%28v%29) 返回 ![v](https://www.zhihu.com/equation?tex=v) 到根路径上权值的最小值以及深度最小的最小值所在的位置。
- ![add(v,value)](https://www.zhihu.com/equation?tex=add%28v%2Cvalue%29) 将 ![v](https://www.zhihu.com/equation?tex=v) 到根路径上所有点的权值增加 ![value ](https://www.zhihu.com/equation?tex=value+) 。

Link Cut Tree支持在均摊 ![O(logV)](https://www.zhihu.com/equation?tex=O%28logV%29) 内完成以上所有操作。

## **具体实现**

采用动态树优化后的Dinic的分层阶段不变。得到分层图 ![G_L](https://www.zhihu.com/equation?tex=G_L) 之后，寻找增广路经增广的步骤如下：
- 对 ![G_L](https://www.zhihu.com/equation?tex=G_L) 上的每一个点建立一棵包含自己一个点的树。
- 设跟踪的节点为 ![v](https://www.zhihu.com/equation?tex=v) ，初始时置 ![v](https://www.zhihu.com/equation?tex=v) 为源点 ![s](https://www.zhihu.com/equation?tex=s) 。
- 选择一条出边 ![(v,u)](https://www.zhihu.com/equation?tex=%28v%2Cu%29) ，将 ![v](https://www.zhihu.com/equation?tex=v) 所在的树连接到 ![u](https://www.zhihu.com/equation?tex=u) 上，设 ![v](https://www.zhihu.com/equation?tex=v) 的权值为出边的边权，最后将 ![v](https://www.zhihu.com/equation?tex=v) 移动到当前树的根。
- 如果 ![v](https://www.zhihu.com/equation?tex=v) 没有出边，那么删除 ![v](https://www.zhihu.com/equation?tex=v) 的所有入边，如果有入边是树边，则断开树上相应节点。
- 如果 ![v=t](https://www.zhihu.com/equation?tex=v%3Dt) ， 找到树上对应的 ![s-t](https://www.zhihu.com/equation?tex=s-t) 路径，将总流量加上 ![find\_min(s)](https://www.zhihu.com/equation?tex=find%5C_min%28s%29) ，再将路径上所有点的权值减去增广流量，删除路径上的所有零边。重新置 ![v](https://www.zhihu.com/equation?tex=v) 为源点 ![s](https://www.zhihu.com/equation?tex=s) ，重复步骤3。
- 如果源点 ![s](https://www.zhihu.com/equation?tex=s) 没有出边，所有增广结束，进行下一次分层。

下面给出了一个例子：
![](https://pic4.zhimg.com/v2-f0f2fdd0195862e7da4af089fb5f2363_b.jpg)已经进行过几轮增广过后的图GL，加粗的是树边。![](https://pic2.zhimg.com/v2-2ea873a335c39c349ce3480a379faf65_b.jpg)从源点出发![](https://pic1.zhimg.com/v2-508b3020f4ed73459be66f62abe04a14_b.jpg)选择边(s,3)，连接s,3，并移动到根

![](https://pic2.zhimg.com/v2-a390e70c6a5b5e99e63e5686feadf905_b.jpg)选择边(3,6)，连接3,6，并移动到根![](https://pic3.zhimg.com/v2-40075587fba9dd3fb437f75bc8c4b6b2_b.jpg)选择边(6,8），连接6,8，直接移动到根。![](https://pic1.zhimg.com/v2-b0941d9c7985c69062bf54ae8e652bd4_b.jpg)橙色的即为增广路径![](https://pic4.zhimg.com/v2-28d87e3a30dafb5b71bac2bdf93cd1b3_b.jpg)删除零边（虚线边）![](https://pic1.zhimg.com/v2-bad303523596c78321eff21665b9e2e0_b.jpg)本次增广完成

## **优化后的时间复杂度**

建树时间复杂度 ![O(V)](https://www.zhihu.com/equation?tex=O%28V%29) 。

Link Cut Tree由Splay实现，Splay的操作由势能分析可得是 ![O(logn)](https://www.zhihu.com/equation?tex=O%28logn%29) ,整棵树由多个Splay树由虚边相连，Access时可以 ![O(1)](https://www.zhihu.com/equation?tex=O%281%29) 进行虚/实边的转换，所以势能分析在整颗树上仍然成立。

在这里整张图可以看做是一个动态树森林，树与树之间由虚边相连，我们同样可以 ![O(1)](https://www.zhihu.com/equation?tex=O%281%29) 进行虚/实边转换，所以势能分析可以扩展到整张图上。根据动态树的时间复杂度分析，在进行了 ![O(E)](https://www.zhihu.com/equation?tex=O%28E%29) 次操作后，总时间复杂度不会超过 ![O(ElogV)](https://www.zhihu.com/equation?tex=O%28ElogV%29) 。

删除零边操作也可以稍稍改动Link Cut Tree达到 ![O(logV)](https://www.zhihu.com/equation?tex=O%28logV%29) ，并且注意到我们发现某一个点没有出边时是遍历所有入边然后删除，但是因为每一条边最多只会被删除一次，所以总的删边时间复杂度 ![O(ElogV)](https://www.zhihu.com/equation?tex=O%28ElogV%29)。

由于并未改动分层阶段，所以分层需要 ![O(V)](https://www.zhihu.com/equation?tex=O%28V%29) 。

总时间复杂度 ![O(VElogV)](https://www.zhihu.com/equation?tex=O%28VElogV%29) 。

## **代码实现**

```cpp
#include<iostream>
#include<cstdio>
#include<deque>
#define MAX 1000009
#define inf 0x7fffffffffffffffll
using namespace std;
/*Dinic's Algorithm with Dynamic Tree*/
typedef long long ll;

struct Edge{ll link,next,flow,deleted;}E[MAX];
int head[MAX],last[MAX],cnt=;
inline void AddEdge(int a,int b,int c){
    ++cnt,E[cnt].link=b,E[cnt].flow=c,E[cnt].next=head[a],last[a]=head[a]=cnt;swap(a,b);
    ++cnt,E[cnt].link=b,E[cnt].flow=,E[cnt].next=head[a],last[a]=head[a]=cnt;
}

int n,m,s,t;
int level[MAX];
deque<int> Q;
inline bool BFS(){
    for(register int i=;i<=n;++i) level[i]=;
    level[s]=;Q.push_back(s);
    while(!Q.empty()){
        int u=Q.front();Q.pop_front();
        for(register int i=head[u],v;v=E[i].link,i;i=E[i].next){
            E[i].deleted=;//清除上一次的删边记号 
            if(E[i].flow&&!level[v]){
                level[v]=level[u]+;//分层 
                Q.push_back(v);
            }
        }
    }return level[t]>;
}

int c[MAX][],f[MAX],id[MAX];
ll val[MAX],add[MAX];
struct MinCmpType{
    ll value,who;
    MinCmpType(ll v=,int w=):value(v),who(w){}
    bool operator <(const MinCmpType& A)const{return value<=A.value;}//权值一样优先选择深度较浅的点 
}minn[MAX];

namespace LinkCutTree{
    inline bool IsRoot(int x){return (c[f[x]][]^x)&&(c[f[x]][]^x);}
    inline void download(int x){
        if(add[x]){
            add[c[x][]]+=add[x];
            val[c[x][]]+=add[x];
            minn[c[x][]].value+=add[x];
            E[id[c[x][]]].flow+=add[x];
            E[id[c[x][]]^].flow-=add[x];
            add[c[x][]]+=add[x];
            val[c[x][]]+=add[x];
            minn[c[x][]].value+=add[x];
            E[id[c[x][]]].flow+=add[x];
            E[id[c[x][]]^].flow-=add[x];
            add[x]=;
        }
    }
    inline void upload(int x){
        minn[x]=min(min(minn[c[x][]],MinCmpType(val[x],x)),minn[c[x][]]);
    }
    inline void Rot(int x){
        int fa=f[x],gf=f[fa],s=c[fa][]==x,sn=c[x][s^];
        if(!IsRoot(fa)) c[gf][c[gf][]==fa]=x;f[x]=gf;
        f[c[fa][s]=sn]=fa;
        f[c[x][s^]=fa]=x;
        upload(fa);
    }
    inline void Splay(int x){
        download(x);
        while(!IsRoot(x)){
            int fa=f[x],gf=f[fa];
            download(gf),download(fa),download(x);
            if(!IsRoot(fa)) (c[gf][]==fa)==(c[fa][]==x)?Rot(fa):Rot(x);
            Rot(x);
        }
        upload(x);
    }
    inline void Access(int x){
        int y=;
        while(x){
            Splay(x);
            c[x][]=y;
            upload(x);
            y=x,x=f[x];
        }
    }
    inline void DeleteZero(int x){
        download(x);
        while(c[x][]&&minn[c[x][]].value==){
            x=minn[c[x][]].who;
            Splay(x);
            c[x][]=f[c[x][]]=;upload(x);
        }
    }
    inline int GetRoot(int x){Access(x),Splay(x);while(c[x][]) x=c[x][];return x;}
    inline void Cut(int x){Access(x),Splay(x);c[x][]=f[c[x][]]=;upload(x);}
};
using namespace LinkCutTree;

ll MaximumFlow;
bool DFS(int u){
    if(u==t){
        Access(s),Splay(t);//得到源汇s-t路径 
        int delta=minn[t].value;//增广路径 
        MaximumFlow+=delta;
        add[t]-=delta;//路径上所有点减去delta 
        DeleteZero(t);//删除所有零边 
        return ;
    }
    for(int& i=head[u],v;v=E[i].link,i;i=E[i].next){//当前弧优化 
        if(E[i].flow&&!E[i].deleted&&level[v]==level[u]+){
            Splay(u),f[u]=v,val[u]=E[i].flow,id[u]=i,upload(u);//连边 
            if(DFS(GetRoot(u)))return ;//直接跳到根，如果已经到了汇点就返回 
        }
    }
    //如果没有出边 
    for(int i=last[u],v;v=E[i].link,i;i=E[i].next){
        if(level[v]==level[u]-){
            if(GetRoot(v)==u) Cut(v);//如果是树边就断开 
            E[i^].deleted=;
        }
    }
    return ;
}

main(){
    scanf("%d%d%d%d",&n,&m,&s,&t);
    while(m--){
        int a,b,c;
        scanf("%d%d%d",&a,&b,&c);
        AddEdge(a,b,c);
    }
    for(register int i=;i<=n;++i) val[i]=minn[i].value=inf;
    while(BFS()){
        while(DFS(GetRoot(s)));
        for(register int x=;x<=n;++x){
            Splay(x);//一定要释放所有的lazy标记 
            c[x][]=c[x][]=f[x]=add[x]=id[x]=;//清除树边信息 
            val[x]=minn[x].value=inf;
            head[x]=last[x];
        }
        val[]=minn[].value=inf;
    }
    cout<<MaximumFlow;
}
```

## **参考资料**

[Dinic's algorithm -Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Dinic%2527s_algorithm)

[Blocking flows, Dinic's algorithm, and applications of dynamic trees-Anderson Wang](https://link.zhihu.com/?target=https%3A//www.cc.gatech.edu/~rpeng/18434_S15/blockingFlows.pdf)

[Dinic's Algorithm with Dynamic Trees-Jonathan Turner](https://link.zhihu.com/?target=https%3A//www.arl.wustl.edu/~jst/cse/542/text/sec19.pdf)

