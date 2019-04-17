# Ice_cream’s world II（HDU-2121） - Alex_McAvoy的博客 - CSDN博客





2019年01月05日 17:25:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：77








> 
# Problem Description

After awarded lands to ACMers, the queen want to choose a city be her capital. This is an important event in ice_cream world, and it also a very difficult problem, because the world have N cities and M roads, every road was directed. Wiskey is a chief engineer in ice_cream world. The queen asked Wiskey must find a suitable location to establish the capital, beautify the roads which let capital can visit each city and the project’s cost as less as better. If Wiskey can’t fulfill the queen’s require, he will be punishing.

# **Input**

Every case have two integers N and M (N<=1000, M<=10000), the cities numbered 0…N-1, following M lines, each line contain three integers S, T and C, meaning from S to T have a road will cost C.

# Output

If no location satisfy the queen’s require, you must be output “impossible”, otherwise, print the minimum cost in this project and suitable city’s number. May be exist many suitable cities, choose the minimum number city. After every case print one blank.

# Sample Input

**3 10 1 1**

**4 40 1 100 2 101 3 202 3 30**

# Sample Output

**impossible**

**40 0**


题意：n 个点 m 条有向边，现要在 n 个点中选一个点作为首都，要求首都与其他点是可达的，给出 m 条道路修建要花费的价格，求最小花费并给出首都的序号，点的标号从 0 开始。

思路：实质是要求一个最小树形图，最小花费可以直接使用朱刘模版去求，但题目要求首都的序号即根结点的位置，因此可以在朱刘模版的基础上，加一个 0 号结点，让其作为一开始的根结点，在寻找每个点的最小入边过程中寻找出实际的根结点，再令 0 号结点增加 m 条边，并使边到各点的距离均为原来所有边权的加和，这样朱刘算法算出的是以 0 号结点为根结点的最小权值，令这个值减去所有边权的加和就是实际的最小树形图的边权。

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
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 16
#define LL long long
using namespace std;
struct Edge{
    int x,y;
    int w;
}edge[N];
int pos;//实际起点
int vis[N];
int id[N];
int in[N],pre[N];
int zhuLiu(int root,int V,int E){
    int res=0;
    while(true){
        for(int i=0;i<V;i++)
            in[i]=INF;

        for(int i=0;i<E;i++){
            int x=edge[i].x;
            int y=edge[i].y;
            if(edge[i].w<in[y] && x!=y){
                pre[y]=x;
                in[y]=edge[i].w;

                if(x==root)//记录实际位置
                    pos=i;
            }
        }

        for(int i=0;i<V;i++){
            if(i==root)
                continue;
            if(in[i]==INF)
                return -1;
        }

        int cnt=0;
        in[root]=0;
        memset(id,-1,sizeof(id));
        memset(vis,-1,sizeof(vis));
        for(int i=0;i<V;i++){
            res+=in[i];

            int y=i;
            while(vis[y]!=i&&id[y]==-1&&y!=root){
                vis[y]=i;
                y=pre[y];
            }

            if(y!=root&&id[y]==-1){
                for(int x=pre[y];x!=y;x=pre[x])
                    id[x]=cnt;
                id[y]=cnt++;
            }
        }
        if(cnt==0)
                break;
        for(int i=0;i<V;i++)
            if(id[i]==-1)
                id[i]=cnt++;

        for(int i=0;i<E;i++){
            int x=edge[i].x;
            int y=edge[i].y;
            edge[i].x=id[x];
            edge[i].y=id[y];

            if(id[x]!=id[y])
                edge[i].w-=in[y];
        }

        V=cnt;
        root=id[root];
    }
    return res;
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        int sum=0;
        for(int i=0;i<m;i++){
            scanf("%d%d%d",&edge[i].x,&edge[i].y,&edge[i].w);
            edge[i].x++;
            edge[i].y++;
            sum+=edge[i].w;
        }
        for(int i=m;i<m+n;i++){//增加超级结点使其到其余点边权相同
            edge[i].x=0;
            edge[i].y=i-m+1;
            edge[i].w=sum;
        }

        int res=zhuLiu(0,n+1,m+n);//res代表以超级结点0为根的最小树形图的总权值
        if(res==-1)
            printf("impossible\n");
        else if(res-sum>=sum)//若差值大于sum说明结点0的出度不止为1,即原图不是连通图
            printf("impossible\n");
        else
            printf("%d %d\n",res-sum,pos-m);
        printf("\n");
    }
    return 0;
}
```





