# Is the Information Reliable?（POJ-2983） - Alex_McAvoy的博客 - CSDN博客





2019年03月01日 16:01:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46
个人分类：[POJ																[图论——差分约束系统](https://blog.csdn.net/u011815404/article/category/8543163)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The galaxy war between the Empire Draco and the Commonwealth of Zibu broke out 3 years ago. Draco established a line of defense called Grot. Grot is a straight line with N defense stations. Because of the cooperation of the stations, Zibu’s Marine Glory cannot march any further but stay outside the line.

A mystery Information Group X benefits form selling information to both sides of the war. Today you the administrator of Zibu’s Intelligence Department got a piece of information about Grot’s defense stations’ arrangement from Information Group X. Your task is to determine whether the information is reliable.

The information consists of M tips. Each tip is either precise or vague.

Precise tip is in the form of P A B X, means defense station A is X light-years north of defense station B.

Vague tip is in the form of V A B, means defense station A is in the north of defense station B, at least 1 light-year, but the precise distance is unknown.

# **Input**

There are several test cases in the input. Each test case starts with two integers N (0 < N ≤ 1000) and M (1 ≤ M ≤ 100000).The next M line each describe a tip, either in precise form or vague form.

# Output

Output one line for each test case in the input. Output “Reliable” if It is possible to arrange N defense stations satisfying all the M tips, otherwise output “Unreliable”.

# Sample Input

**3 4P 1 2 1P 2 3 1V 1 3P 1 3 15 5V 1 2V 2 3V 3 4V 4 5V 3 5**

# Sample Output

**UnreliableReliable**


题意：n 个防御站排成一条线，有 m 个条件共 2 种形式：
- P A B X：表示 A 在 B 的北面 X 光年处
- V A B：表示 A 在 B 的北面至少 1 光年处

问防御系统是否能满足这 m 个条件

思路：

由题意可得：
- P A B X：B+X=A
- V A B：B+1<=A

假设在最北方存在一个超级源点 0 号，那么设 dis[i] 为 0 号站离 i 号防御站的距离，那么：
- P A B X：dis[A]+X=dis[B]，那么：dis[B]<=dis[A]+X 且 dis[B]>=dis[A]+X

	dis[B]<=dis[A]+X：A 到 B 权值为 X 的边

	dis[B]>=dis[A]+X：B 到 A 的权值 - X 的边
- V A B：dis[A]<=dis[B]-1，据此可以添加：B 到 A 的权值为 -1 的边

![](https://img-blog.csdnimg.cn/20190301160118723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

由于边的权值存在负数，因此可以使用 SPFA 算法来判断负环是否存在从而判断该差分约束系统是否有解，将所有点的最短距离都设为 INF，即 dis[i]=INF，再将超级源点 0 号点设为 dis[0]=0，然后求 0 号点到其他所有点的最短距离并判断负环是否存在即可

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
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Edge{
    int from;
    int to;
    int w;
    Edge(){}
    Edge(int from,int to,int w):from(from),to(to),w(w){}
}edge[N];
int head[N],next[N],num;
int dis[N];
int outque[N];
bool vis[N];
void init(){
    num=0;
    memset(head,-1,sizeof(head));
}
void add(int from,int to,int w){
    edge[num]=Edge(from,to,w);
    next[num]=head[from];
    head[from]=num++;
}
int SPFA(int s,int n){
    int res=0;
    memset(vis,false,sizeof(vis));
    memset(outque,0,sizeof(outque));

    for(int i=1;i<n;i++)
        dis[i]=INF;
    dis[s]=0;
    vis[s]=true;

    queue<int> Q;
    Q.push(s);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        vis[x]=false;

        outque[x]++;
        if(outque[x]>n-1){
            res=-1;
            break;
        }

        for(int i=head[x];i!=-1;i=next[i]){
            Edge &e=edge[i];
            if(dis[e.to]>dis[x]+e.w){
                dis[e.to]=dis[x]+e.w;
                if(!vis[e.to]){
                    vis[e.to]=true;
                    Q.push(e.to);
                }
            }
        }
    }

    return res;
}
int main(){
    int n,m;
    while(scanf("%d%d%d",&n,&m)!=EOF){
        init();
        for(int i=0;i<m;i++){
            char ch[10];
            scanf("%s",ch);
            if(ch[0]=='P'){
                int A,B,X;
                scanf("%d%d%d",&A,&B,&X);
                add(A,B,X);
                add(B,A,-X);
            }
            else{
                int A,B;
                scanf("%d%d",&A,&B);
                add(B,A,-1);
            }
        }
        for(int i=1;i<=n;i++)//虚拟节点到各点的边
            add(0,i,0);
        int res=SPFA(0,n+1);
        if(res==-1)
            printf("Unreliable\n");
        else
            printf("Reliable\n");
    }
    return 0;
}
```






