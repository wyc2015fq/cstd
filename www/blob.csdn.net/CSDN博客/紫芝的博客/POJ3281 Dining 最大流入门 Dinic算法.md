# POJ3281 Dining 最大流入门 Dinic算法 - 紫芝的博客 - CSDN博客





2018年10月08日 10:40:13[紫芝](https://me.csdn.net/qq_40507857)阅读数：19
个人分类：[最大流Dinic算法](https://blog.csdn.net/qq_40507857/article/category/8104974)









# Dining
|**Time Limit:** 2000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 23216||**Accepted:** 10271|

Description

Cows are such finicky eaters. Each cow has a preference for certain foods and drinks, and she will consume no others.

Farmer John has cooked fabulous meals for his cows, but he forgot to check his menu against their preferences. Although he might not be able to stuff everybody, he wants to give a complete meal of both food and drink to as many cows as possible.

Farmer John has cooked *F* (1 ≤ *F* ≤ 100) types of foods and prepared *D* (1 ≤ *D* ≤ 100) types of drinks. Each of his *N* (1 ≤ *N* ≤ 100) cows has decided whether she is willing to eat a particular food or drink a particular drink. Farmer John must assign a food type and a drink type to each cow to maximize the number of cows who get both.

Each dish or drink can only be consumed by one cow (i.e., once food type 2 is assigned to a cow, no other cow can be assigned food type 2).

Input

Line 1: Three space-separated integers: *N*, *F*, and *D*

Lines 2..*N*+1: Each line *i* starts with a two integers *Fi* and *Di*, the number of dishes that cow *i* likes and the number of drinks that cow *i* likes. The next *Fi* integers denote the dishes that cow *i* will eat, and the *Di* integers following that denote the drinks that cow *i* will drink.

Output

Line 1: A single integer that is the maximum number of cows that can be fed both food and drink that conform to their wishes

Sample Input

4 3 3
2 2 1 2 3 1
2 2 2 3 1 2
2 2 1 3 1 2
2 1 1 3 3
Sample Output

3
Hint

One way to satisfy three cows is: 

Cow 1: no meal 

Cow 2: Food #2, Drink #2 

Cow 3: Food #1, Drink #1 

Cow 4: Food #3, Drink #3 

The pigeon-hole principle tells us we can do no better since there are only three kinds of food or drink. Other test data sets are more challenging, of course.

Source

[USACO 2007 Open Gold](http://poj.org/searchproblem?field=source&key=USACO+2007+Open+Gold)

**题意：**
**有N头牛，F种食物可以制作，D种饮料可以制作 然后每行代表一头牛的喜好，开头两个数fi,di表示这头牛喜欢fi种食物，di种饮料，接下来fi个数表示喜欢的食物编号，di个数表示喜欢的饮料的编号现在主人使用最优决策制作出F种食物和D种饮料，问怎么喂才能使尽可能多的牛喂饱(喂饱=一份食物一份饮料，且一头牛最多消耗一份食物和一份饮料) , 输出最多喂饱的牛数**
**思路：**

本题关键是建图，图建完以后就是裸 **dinic**
[***Dinnic算法模板***](https://www.cnblogs.com/SYCstudio/p/7260613.html)

**建图方法：**
**设1+F+D+2N+1个结点，1表示源点，1+F+D+2N+1表示汇点 1+1 ~ 1+F表示食物结点，1+F+1 ~ 1+F+N表示牛的编号，1+F+N+1 ~ 1+F+2N也表示牛的编号**
**建图解释：**
**首先，这幅图被分为几个模块，从左到右依次为：食物制作模块(src-F边集)，喂食物模块(F-N1边集)，牛吃食物模块(N1-N2边集)，喂饮料模块(N2-D边集)和牛喝饮料模块(D-tar边集) 然后解释为什么设权值为1**
**第一：人做的食物和水都是一份；第二：牛最多吃一份食物(水)**，就算有多份食物指向牛，由于下一条边(牛指出的边，即牛吃食物(喝饮料)模块)为1，意味着最后只能流出1的流量，即吃掉其中一份食物，水的指出同理 ，这样就能想到为什么要设两次牛了，就是要通过牛-牛边控制流量为1，即通过食物选择模块后，保证一头牛只吃了一份食物 ， 然后吃完食物流量为1了，就只能选一条路喝饮料，最后饮料流出的也是唯一的一条边，表示这种饮料最终只能被一头牛选择

```cpp
//#include<bits/stdc++.h>
#include<cstdio>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
#define INF 0x3f3f3f3f
const int maxn=1e3+10;

int F,D,N;
int g[maxn][maxn];
int lv[maxn];

//bfs分层图过程
bool bfs(int src,int tar)
{
    memset(lv,0,sizeof lv);
    queue<int>que;
    que.push(src);
    lv[src]=1;//源点的深度为1

    while(!que.empty()){
        int cur=que.front();
        que.pop();
        for(int i=src;i<=tar;i++)
        //若该残量不为0，并未分配深度
            if(!lv[i]&&g[cur][i]){
                lv[i]=lv[cur]+1;
                if(i==tar)  return 1;
                que.push(i);
            }
        }
    return 0;
}

//寻找增广路
//cur是当前节点，src是源点，tar是汇点，totflow当前流量
int dfs(int cur,int src,int tar,int totflow){
    int ret=0;
    if(cur==tar||!totflow)//当已经到达汇点，直接返回
        return totflow;
    for(int i=src;i<=tar;i++){
        if(totflow==0)  break;//若没有增广路
    //满足分层图和残量不为0两个条件
        if(g[cur][i]&&lv[cur]+1==lv[i]){
            int f=min(totflow,g[cur][i]);
            int flowdown=dfs(i,src,tar,f);//向下增广
            ret+=flowdown;
            totflow-=flowdown;
            g[cur][i]-=flowdown;//正向边减
            g[i][cur]+=flowdown;//反向边加
        }
    }
    return ret;
}

int dinic(int src,int tar)
{
    int ret=0;//记录最大流量
    while(bfs(src,tar)){
        int tmp=dfs(src,src,tar,INF);
        if(!tmp)    break;
        ret+=tmp;
    }
    return ret;
}

int main()
{
    while(scanf("%d%d%d",&N,&F,&D)!=EOF){
        int src=1;//源点
        int tar=1+N*2+F+D+1;//汇点

        for(int i=1;i<=F;i++)//src-F边集
            g[src][src+i]=1;

        for(int i=1+N*2+F+1;i<tar;i++)//D-tar边集
            g[i][tar]=1;

        for(int i=1;i<=N;i++){
            g[src+F+i][src+F+i+N]=1;//N1-N2边集
            int fi,di;
            scanf("%d%d",&fi,&di);

            int tmp;
            while(fi--){//F-N1边集
                scanf("%d",&tmp);
                g[src+tmp][src+F+i]=1;
            }
            while(di--){
                scanf("%d",&tmp);
                g[src+F+i+N][src+F+2*N+tmp]=1;
            }
        }
        printf("%d\n",dinic(src,tar));
    }
    return 0;
}
```





