# By Recognizing These Guys, We Find Social Networks Useful（HDU-3849） - Alex_McAvoy的博客 - CSDN博客





2018年10月28日 16:19:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Social Network is popular these days.The Network helps us know about those guys who we are following intensely and makes us keep up our pace with the trend of modern times.

But how?

By what method can we know the infomation we wanna?In some websites,maybe Renren,based on social network,we mostly get the infomation by some relations with those "popular leaders".It seems that they know every lately news and are always online.They are alway publishing breaking news and by our relations with them we are informed of "almost everything".

(Aha,"almost everything",what an impulsive society!)

Now,it's time to know what our problem is.We want to know which are the key relations make us related with other ones in the social network.

Well,what is the so-called key relation?

It means if the relation is cancelled or does not exist anymore,we will permanently lose the relations with some guys in the social network.Apparently,we don't wanna lose relations with those guys.We must know which are these key relations so that we can maintain these relations better.

We will give you a relation description map and you should find the key relations in it.

We all know that the relation bewteen two guys is mutual,because this relation description map doesn't describe the relations in twitter or google+.For example,in the situation of this problem,if I know you,you know me,too.

# Input

The input is a relation description map.

In the first line,an integer t,represents the number of cases(t <= 5).

In the second line,an integer n,represents the number of guys(1 <= n <= 10000) and an integer m,represents the number of relations between those guys(0 <= m <= 100000).

From the second to the (m + 1)the line,in each line,there are two strings A and B(1 <= length[a],length[b] <= 15,assuming that only lowercase letters exist).

We guanrantee that in the relation description map,no one has relations with himself(herself),and there won't be identical relations(namely,if "aaa bbb" has already exists in one line,in the following lines,there won't be any more "aaa bbb" or "bbb aaa").

We won't guarantee that all these guys have relations with each other(no matter directly or indirectly),so of course,maybe there are no key relations in the relation description map.

# Output

In the first line,output an integer n,represents the number of key relations in the relation description map.

From the second line to the (n + 1)th line,output these key relations according to the order and format of the input.

# Sample Input

1

4 4

saerdna aswmtjdsj

aswmtjdsj mabodx

mabodx biribiri

aswmtjdsj biribiri

# Sample Output

1

saerdna aswmtjdsj


题意：给出一个 n 个点 m 条边的无向图，求这个图中桥的个数，并按顺序输出桥

思路：由于图可能不连通，因此首先需要判断是否存在 dfn[i]=0，如果存在则说明图不连通，直接输出 0 即可

求桥可以直接套 Tarjan 算法模版，难点在于按输入顺序求桥，因此不能在求桥的过程中输出，必须在求完所有桥后按顺序判断每条边是否是桥，因此可用 map 来实现一个字符串到点编号的映射，则一个输入边即为两个字符串结点 Node 组成

对于边 x-y 来说，只要满足 low[x]>dfn[y] 或 low[y]>dnf[x] 则该边一定是桥，因而先求完所有节点的 low 值，然后在退出 Tarjan 算法后，重新扫描每一条边的两个节点 x、y 的 low值、dfn 值，再判断该边是否为桥

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 20001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
struct Node{
    char str[20];
    bool operator <(const Node s)const{
        return strcmp(str,s.str)<0;
    }
};
struct Edge{
    Node x;
    Node y;
    bool flag;//判断该边是否为割边
}edge[N];
map<Node,int> mp;//将 字符串Node映射成结点编号
vector<int> G[N];
int n,m;
int dfn[N],low[N];
int block_cnt;
void Tarjan(int x,int father){
    low[x]=dfn[x]=++block_cnt;

    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(y==father)
            continue;

        if(dfn[y]==0){
            Tarjan(y,x);
            low[x]=min(low[x],low[y]);
        }
        else
            low[x]=min(low[x],dfn[y]);
    }
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        mp.clear();
        for(int i=0;i<n;i++)
            G[i].clear();
        block_cnt=0;
        memset(dfn,0,sizeof(dfn));

        int num=0;//记录编号
        for(int i=0;i<m;i++){
            edge[i].flag=false;;
            scanf("%s%s",edge[i].x.str,edge[i].y.str);

            if(mp.find(edge[i].x)==mp.end())
                mp[edge[i].x]=++num;
            if(mp.find(edge[i].y)==mp.end())
                mp[edge[i].y]=++num;

            int x=mp[edge[i].x];
            int y=mp[edge[i].y];
            G[x].push_back(y);
            G[y].push_back(x);
        }

        Tarjan(1,-1);
        bool connection=true;//判断是否连通
        for(int i=1;i<=n;i++){
            if(dfn[i]==0){
                connection=false;
                break;
            }
        }

        if(connection==false)
            printf("0\n");
        else{
            int res=0;
            for(int i=0;i<m;i++){
                int x=mp[edge[i].x];
                int y=mp[edge[i].y];
                if(low[x]>dfn[y] || low[y]>dfn[x]){
                     edge[i].flag=true;
                     res++;
                }
            }

            printf("%d\n",res);
            for(int i=0;i<m;i++)
                if(edge[i].flag)
                    printf("%s %s\n",edge[i].x.str,edge[i].y.str);
        }
    }
    return 0;
}
```






