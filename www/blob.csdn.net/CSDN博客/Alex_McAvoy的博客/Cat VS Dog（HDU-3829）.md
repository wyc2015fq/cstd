# Cat VS Dog（HDU-3829） - Alex_McAvoy的博客 - CSDN博客





2018年11月29日 23:18:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

The zoo have N cats and M dogs, today there are P children visiting the zoo, each child has a like-animal and a dislike-animal, if the child's like-animal is a cat, then his/hers dislike-animal must be a dog, and vice versa.

Now the zoo administrator is removing some animals, if one child's like-animal is not removed and his/hers dislike-animal is removed, he/she will be happy. So the administrator wants to know which animals he should remove to make maximum number of happy children.

# **Input**

The input file contains multiple test cases, for each case, the first line contains three integers N <= 100, M <= 100 and P <= 500.

Next P lines, each line contains a child's like-animal and dislike-animal, C for cat and D for dog. (See sample for details)

# Output

For each case, output a single integer: the maximum number of happy children.

# Sample Input

**1 1 2C1 D1D1 C1**

**1 2 4C1 D1C1 D1C1 D2D2 C1**

# Sample Output

**13**


题意：动物园有 n 只猫 m 条狗 p 个小孩，每个小孩都有自己喜欢和讨厌的动物，如果喜欢狗就讨厌猫，如果讨厌猫就喜欢狗，如果想让一个孩子开心，就需要将他喜欢的动物留在动物园中，将讨厌的动物带走，现在通过工作人员带走某些动物，问最多能让多少个孩子开心

思路：将喜欢狗的小孩放在左边，将喜欢猫的小孩放在右边，如果左边小孩 i 喜欢的狗/猫与右边小孩 j 讨厌的狗/猫是同一只的话，就在左 i 右 j 间连一条无向边，现在要尽量多让小孩开心，因此需要尽量选出多的小孩来将他们讨厌的动物带走把他们喜欢的动物留下，因此只要在新建的二分图中尽量的选择多的点，使得任意两点不存在边，即求最大独立集

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
    int x1,x2,p;
    while(scanf("%d%d%d",&x1,&x2,&p)!=EOF&&(x1+x2+p)){
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






