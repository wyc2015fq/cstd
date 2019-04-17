# 50 years, 50 colors（HDU-1498） - Alex_McAvoy的博客 - CSDN博客





2018年12月02日 19:00:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75








> 
# Problem Description

On Octorber 21st, HDU 50-year-celebration, 50-color balloons floating around the campus, it's so nice, isn't it? To celebrate this meaningful day, the ACM team of HDU hold some fuuny games. Especially, there will be a game named "crashing color balloons".

There will be a n*n matrix board on the ground, and each grid will have a color balloon in it.And the color of the ballon will be in the range of [1, 50].After the referee shouts "go!",you can begin to crash the balloons.Every time you can only choose one kind of balloon to crash, we define that the two balloons with the same color belong to the same kind.What's more, each time you can only choose a single row or column of balloon, and crash the balloons that with the color you had chosen. Of course, a lot of students are waiting to play this game, so we just give every student k times to crash the balloons.

Here comes the problem: which kind of balloon is impossible to be all crashed by a student in k times.

![](https://img-blog.csdnimg.cn/20181202183241676.jpg)

# **Input**

There will be multiple input cases.Each test case begins with two integers n, k. n is the number of rows and columns of the balloons (1 <= n <= 100), and k is the times that ginving to each student(0 < k <= n).Follow a matrix A of n*n, where Aij denote the color of the ballon in the i row, j column.Input ends with n = k = 0.

# Output

For each test case, print in ascending order all the colors of which are impossible to be crashed by a student in k times. If there is no choice, print "-1".

# Sample Input

**1 112 11 11 22 11 22 25 41 2 3 4 52 3 4 5 13 4 5 1 24 5 1 2 35 1 2 3 43 350 50 5050 50 5050 50 500 0**

# Sample Output

**-1121 2 3 4 5-1**


题意：给一个 n*n 的图，图中有 s 种颜色的气球，现在能扎破气球 k 次，每次可以扎破一行或一列的同一颜色的气球，问在 k 次操作后，有哪几种颜色的气球是不能被完全扎破的

思路：令行为左点集，列为右点集，对于图中的点（i,j），若存在气球，则建立一条左 i 右 j 的边，只需要对每种颜色判断该气球的颜色最少需要多少次才能全部扎破即可，尽可能的选出小的行号或列号，然后判断是否正好将当前颜色的气球全部覆盖，实质即为求二分图的最小点覆盖数

由于气球颜色最多有50种，因此每次处理一种颜色的气球即可

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
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int n,k;
bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
int G[N][N];//存边
int nowColor;//记录当前的颜色
bool dfs(int x){
    for(int y=1;y<=n;y++){
        if(G[x][y]==nowColor&&!vis[y]){//不在交替路中且当前结点是当前颜色
            vis[y]=true;
            if(link[y]==-1 || dfs(link[y])){//如果是未匹配点，说明交替路是增广路
                link[y]=x;//交换路径
                return true;//返回成功
            }
        }
    }
    return false;
}
int hungarian(int x){
    nowColor=x;//记录当前颜色
    memset(link,-1,sizeof(link));

    int ans=0;//记录最大匹配数
    for(int i=1;i<=n;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}
int main(){
    while(scanf("%d%d",&n,&k)!=EOF&&(n+k)){

        memset(G,0,sizeof(G));

        int color[51];//记录颜色
        memset(color,0,sizeof(color));
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                scanf("%d",&G[i][j]);
                color[G[i][j]]++;
            }
        }

        vector<int> col;//不能被扎破的颜色
        for(int i=1;i<=50;i++){
            if(color[i]>0){
                if(hungarian(i)>k){
                    col.push_back(i);
                }
            }
        }

        if(col.size()==0)
            printf("-1\n");
        else{
            for(int i=0;i<col.size()-1;i++)
                printf("%d ",col[i]);
            printf("%d\n",col[col.size()-1]);
        }
    }
    return 0;
}
```





