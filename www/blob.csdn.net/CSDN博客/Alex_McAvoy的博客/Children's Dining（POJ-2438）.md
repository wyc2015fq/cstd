# Children's Dining（POJ-2438） - Alex_McAvoy的博客 - CSDN博客





2018年11月02日 22:30:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45








> 
# Problem Description

Usually children in kindergarten like to quarrel with each other. This situation annoys the child-care women. For instant, when diner time comes, a fierce conflict may break out when a certain couple of children sitting side by side who are hostile with each other. Although there aren't too many children dining at the same round table, but the relationship of "enemy" or "friend" may be very complex. The child-care women do come across a big problem. Now it is time for you to help them to figure out a proper arrangement of sitting, with which no two "enemy" children is adjacent. 

Now we assume that there are 2 * n children who sit around a big table, and that none has more than n - 1 "enemies".

# **Input**

The input is consisted of several test blocks. For each block, the first line contains two integers n and m (1 <= n <= 200, 0 <= m <= n (n - 1)). We use positive integers from 1 to 2 * n to label the children dining round table. Then m lines followed. Each contains positive integers i and j ( i is not equal to j, 1 <= i, j <= 2 * n), which indicate that child i and child j consider each other as "enemy". In a input block, a same relationship isn't given more than once, which means that if "i j" has been given, "j i" will not be given. 

There will be a blank line between input blocks. And m = n = 0 indicates the end of input and this case shouldn't be processed.

# Output

For each test block, if the proper arrangement exist, you should print a line with a proper one; otherwise, print a line with "No solution!".

# Sample Input

**1 0**

**2 21 23 4**

**3 61 21 32 43 54 65 6**

**4 121 21 31 42 52 63 73 84 84 75 65 76 8**

**0 0**

# Sample Output

**1 24 2 3 11 6 3 2 5 41 6 7 2 3 4 5 8**


题意：有 2*N 个人坐在一张圆桌上吃饭，但是每两个人之间存在一种关系，即敌人或者朋友，然后需要安排一个座位次序，使得相邻的两个人都不会是敌人，假设每个人最多有 N-1 个敌人，如果没有输出"No solution!".

思路：如果按照题意直接建图，每个点表示一个人，人之间的敌对关系表示两个点之间有边，但问题是求人围着桌子的座次，也即求图中的一个环，但是要求这个环不能包含所给出的每条边，所有没给出的边却是可以用的，也就是说本题实际上是在上面建的图的反图上求解一个环，使得该环包含所有点，包含所有点的环一定是一条哈密顿回路，因此题目就是求所给图的反图上的一条哈密顿回路

题中给了一个特殊条件，就是一共有 2*N 个人，但是每个人最多有 N-1 个敌人，也就是说，每个人可以选择坐在身边的人数大于 n+1，这就意味着在建立的反图中，每个点的度数大于 N+1，由 Dirac 定理可知，此图一定存在哈密顿回路，所以答案不会出现 "No solution!"，因此直接套模板构造哈密顿回路即可

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
#define N 1001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
bool G[N][N];
bool vis[N];
int ans[N];

void Reverse(int arv[N],int s,int t){//将数组anv从下标s到t的部分的顺序反向
    int temp;
    while(s<t){
        temp=arv[s];
        arv[s]=arv[t];
        arv[t]=temp;
        s++;
        t--;
    }
}

void Hamilton(int n){

    int t;
    int s=1;//初始化取s为1号点
    for(int i=1;i<=n;i++)
        if(G[s][i]){
            t=i;//取任意邻接与s的点为t
            break;
        }

    memset(vis,false,sizeof(vis));
    vis[s]=true;
    vis[t]=true;
    ans[0]=s;
    ans[1]=t;


    int ansi=2;
    while(true){

        //从t向外扩展
        while(true){
            int i;
            for(i=1;i<=n;i++){
                if(G[t][i] && !vis[i]){
                    ans[ansi++]=i;
                    vis[i]=true;
                    t=i;
                    break;
                }
            }
            if(i>n)
                break;
        }

        //将当前得到的序列倒置
        Reverse(ans,0,ansi-1);

        //s和t互换
        swap(s,t);

        while(true){//从t继续扩展,相当于在原来的序列上从s向外扩展
            int i;
            for(i=1;i<=n;i++){
                if(G[t][i] && !vis[i]){
                    ans[ansi++]=i;
                    vis[i]=true;
                    t=i;
                    break;
                }
            }
            if(i>n)
                break;
        }


        //如果s和t不相邻,进行调整
        if(!G[s][t]){
            //取序列中的一点i,使得ans[i]与t相连,并且ans[i+1]与s相连
            int i;
            for(i=1;i<ansi-2;i++)
                if(G[ans[i]][t]&&G[s][ans[i+1]])
                    break;
            i++;
            t=ans[i];
            Reverse(ans,i,ansi-1);//将从ans[i+1]到ｔ部分的ans[]倒置
        }//此时s和t相连


        //如果当前序列包含n个元素,算法结束
        if(ansi==n)
            return;

        //当前序列中元素的个数小于n,寻找点ans[i],使得ans[i]与ans[]外的一个点相连
        int i,j;
        for(j=1;j<=n;j++){
            if(vis[j])
                continue;
            for(i=1;i<ansi-2;i++)
                if(G[ans[i]][j])
                    break;
            if(G[ans[i]][j])
                break;
        }
        s=ans[i-1];
        t=j;//将新找到的点j赋给t
        Reverse(ans,0,i-1);//将ans[]中s到ans[i-1]的部分倒置
        Reverse(ans,i,ansi-1);//将ans[]中ans[i]到t的部分倒置
        ans[ansi++]=j;//将点j加入到ans[]尾部
        vis[j]=true;
    }
}

int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n||m)){

        n*=2;
        for(int i=0;i<=n;i++){
            for(int j=0;j<=n;j++){
                if(i==j){
                    G[i][j]=false;
                    G[j][i]=false;
                }
                else{
                    G[i][j]=true;
                    G[j][i]=true;
                }
            }
        }

        int ansi=0;
        memset(ans, 0, sizeof(ans));
        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            G[y][x]=false;
            G[x][y]=false;
        }

        Hamilton(n);
        for(int i=0;i<n;i++)
            printf("%d ", ans[i]);
        printf("\n");
    }
    return 0;
}
```





