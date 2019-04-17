# Wireless Network（POJ-2236） - Alex_McAvoy的博客 - CSDN博客





2019年02月18日 18:04:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50
个人分类：[POJ																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

An earthquake takes place in Southeast Asia. The ACM (Asia Cooperated Medical team) have set up a wireless network with the lap computers, but an unexpected aftershock attacked, all computers in the network were all broken. The computers are repaired one by one, and the network gradually began to work again. Because of the hardware restricts, each computer can only directly communicate with the computers that are not farther than d meters from it. But every computer can be regarded as the intermediary of the communication between two other computers, that is to say computer A and computer B can communicate if computer A and computer B can communicate directly or there is a computer C that can communicate with both A and B. 

In the process of repairing the network, workers can take two kinds of operations at every moment, repairing a computer, or testing if two computers can communicate. Your job is to answer all the testing operations. 

# Input

The first line contains two integers N and d (1 <= N <= 1001, 0 <= d <= 20000). Here N is the number of computers, which are numbered from 1 to N, and D is the maximum distance two computers can communicate directly. In the next N lines, each contains two integers xi, yi (0 <= xi, yi <= 10000), which is the coordinate of N computers. From the (N+1)-th line to the end of input, there are operations, which are carried out one by one. Each line contains an operation in one of following two formats: 

1. "O p" (1 <= p <= N), which means repairing computer p. 

2. "S p q" (1 <= p, q <= N), which means testing whether computer p and q can communicate. 

The input will not exceed 300000 lines. 

# Output

For each Testing operation, print "SUCCESS" if the two computers can communicate, or "FAIL" if not.

# **Sample Input**

**4 10 10 20 30 4O 1O 2O 4S 1 4O 3S 1 4**

# Sample Output

**FAILSUCCESS**


题意：有 n 台损坏的电脑，要将他们逐台恢复。若两台电脑能相互通信，有两种情况，一是他们间的距离小于 d，二是他们都能到达第三台已修复的电脑，现在给出全部的电脑位置，对所有电脑进行上述的操作，O x 代表修复第 x 台，S x y 代表判断 x y 能否通信，若能输出 SUCCESS，否则输出 FALL

思路：简单的并查集

用并查集来保存电脑之间的连通情况，每次修好电脑后，就将已经修好的、在他距离范围内的电脑将他连通

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Node{
    int a;
    int b;
}node[N];
int n,d;
int father[N];
bool vis[N];
vector<int> G[N];
double calculate(Node x,Node y){
    return (x.a-y.a)*(x.a-y.a)+(x.b-y.b)*(x.b-y.b);
}
int Find(int x){
    if(father[x]==x)
        return x;
    return father[x]=Find(father[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[x]=y;
}
bool query(int x,int y){
    return Find(x)==Find(y);
}
void update(int x){
    vis[x]=true;
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(vis[y]&&!query(x,y))
            Union(x,y);
    }
}
int main(){
    scanf("%d%d",&n,&d);
    for(int i=1;i<=n;i++)
        scanf("%d%d",&node[i].a,&node[i].b);

    for(int i=1;i<=n;i++)
        father[i]=i;
    for(int i=1;i<=n;i++){
        for(int j=i+1;j<=n;j++){
            if(calculate(node[i],node[j])<=d*d){
                G[i].push_back(j);
                G[j].push_back(i);
            }
        }
    }

    char op[5];
    while(scanf("%s",op)!=EOF){
        if(op[0]=='O'){
            int p;
            scanf("%d",&p);
            update(p);
        }
        else{
            int p,q;
            scanf("%d%d",&p,&q);
            if(query(p,q))
                printf("SUCCESS\n");
            else
                printf("FAIL\n");
        }
    }

    return 0;
}
```






