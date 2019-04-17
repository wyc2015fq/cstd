# Cat vs. Dog（HDU-2768） - Alex_McAvoy的博客 - CSDN博客





2019年03月13日 09:31:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

The latest reality show has hit the TV: ``Cat vs. Dog''. In this show, a bunch of cats and dogs compete for the very prestigious Best Pet Ever title. In each episode, the cats and dogs get to show themselves off, after which the viewers vote on which pets should stay and which should be forced to leave the show. 

Each viewer gets to cast a vote on two things: one pet which should be kept on the show, and one pet which should be thrown out. Also, based on the universal fact that everyone is either a cat lover (i.e. a dog hater) or a dog lover (i.e. a cat hater), it has been decided that each vote must name exactly one cat and exactly one dog. 

Ingenious as they are, the producers have decided to use an advancement procedure which guarantees that as many viewers as possible will continue watching the show: the pets that get to stay will be chosen so as to maximize the number of viewers who get both their opinions satisfied. Write a program to calculate this maximum number of viewers.

# **Input**

On the first line one positive number: the number of testcases, at most 100. After that per testcase: 

* One line with three integers c, d, v (1 ≤ c, d ≤ 100 and 0 ≤ v ≤ 500): the number of cats, dogs, and voters. 

* v lines with two pet identifiers each. The first is the pet that this voter wants to keep, the second is the pet that this voter wants to throw out. A pet identifier starts with one of the characters `C' or `D', indicating whether the pet is a cat or dog, respectively. The remaining part of the identifier is an integer giving the number of the pet (between 1 and c for cats, and between 1 and d for dogs). So for instance, ``D42'' indicates dog number 42.

# Output

Per testcase: 

* One line with the maximum possible number of satisfied voters for the show.

# Sample Input

**21 1 2C1 D1D1 C11 2 4C1 D1C1 D1C1 D2D2 C1**

# Sample Output

**13**


题意&&思路：与 [Cat VS Dog（HDU-3829）](https://blog.csdn.net/u011815404/article/details/84641710)一样。。只是输入不同。。

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
const int MOD=1E9+7;
const int N=1000+5;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int n,m;
bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
int G[N][N];//存边
bool dfs(int x){
    for(int y=1;y<=m;y++){//对x的每个邻接点
        if(G[x][y]==1&&!vis[y]){//不在交替路中
            vis[y]=true;//放入交替路
            if(link[y]==-1 || dfs(link[y])){//如果是未匹配点，说明交替路是增广路
                link[y]=x;//交换路径
                return true;//返回成功
            }
        }
    }
    return false;//不存在增广路，返回失败
}
int hungarian(){
    int ans=0;//记录最大匹配数
    memset(link,-1,sizeof(link));
    for(int i=1;i<=n;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}
struct Node{
    int like;
    int dislike;
    Node(){}
    Node(int v1,int v2):like(v1),dislike(v2){}
}P1[N],P2[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int x1,x2,p;
        scanf("%d%d%d",&x1,&x2,&p);
        memset(G,false,sizeof(G));

        n=0;
        m=0;
        for(int i=1;i<=p;i++){
            char str1,str2;
            int v1,v2;
            scanf(" %c %d %c %d",&str1,&v1,&str2,&v2);
            if(str1=='D')//狗
                P1[++n]=Node(v1,v2);
            else if(str1=='C')//猫
                P2[++m]=Node(v1,v2);
        }

        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                if(P1[i].like==P2[j].dislike || P1[i].dislike==P2[j].like)//左边小孩i喜欢与右边小孩j讨厌的是同一只
                    G[i][j]=true;

        printf("%d\n",p-hungarian());
    }
    return 0;
}
```






