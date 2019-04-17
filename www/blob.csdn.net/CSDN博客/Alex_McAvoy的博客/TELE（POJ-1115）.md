# TELE（POJ-1115） - Alex_McAvoy的博客 - CSDN博客





2018年10月08日 10:53:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[POJ																[动态规划——树形 DP](https://blog.csdn.net/u011815404/article/category/8104361)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

A TV-network plans to broadcast an important football match. Their network of transmitters and users can be represented as a tree. The root of the tree is a transmitter that emits the football match, the leaves of the tree are the potential users and other vertices in the tree are relays (transmitters). 

The price of transmission of a signal from one transmitter to another or to the user is given. A price of the entire broadcast is the sum of prices of all individual signal transmissions. 

Every user is ready to pay a certain amount of money to watch the match and the TV-network then decides whether or not to provide the user with the signal. 

Write a program that will find the maximal number of users able to watch the match so that the TV-network's doesn't lose money from broadcasting the match.

# **Input**

The first line of the input file contains two integers N and M, 2 <= N <= 3000, 1 <= M <= N-1, the number of vertices in the tree and the number of potential users. 

The root of the tree is marked with the number 1, while other transmitters are numbered 2 to N-M and potential users are numbered N-M+1 to N. 

The following N-M lines contain data about the transmitters in the following form: 

K A1 C1 A2 C2 ... AK CK 

Means that a transmitter transmits the signal to K transmitters or users, every one of them described by the pair of numbers A and C, the transmitter or user's number and the cost of transmitting the signal to them. 

The last line contains the data about users, containing M integers representing respectively the price every one of them is willing to pay to watch the match.

# Output

The first and the only line of the output file should contain the maximal number of users described in the above text.

# Sample Input

**9 63 2 2 3 2 9 32 4 2 5 23 6 2 7 2 8 24 3 3 3 1 1**

# Sample Output

**5**

————————————————————————————————————————————————————

跟 有线电视网（洛谷-P1273）是一个题，题意&&思路：[点击这里](https://blog.csdn.net/u011815404/article/details/82953081)

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
#define N 3001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
struct Edge{
    int to;
    int next;
    int dis;
}edge[N*2];
int n,m;
int head[N],dis[N];
int num[N];
int dp[N][N];
int cnt;
void addEdge(int x,int y,int dis){
    edge[cnt].to=y;
    edge[cnt].next=head[x];
    edge[cnt].dis=dis;
    head[x]=cnt;
    cnt++;
}
void treeDP(int x,int father){
    dp[x][0]=0;
    if(x>n-m){
        dp[x][1]=dis[x];
        num[x]=1;
        return;
    }

    for(int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;
        if(father==y)//下一个相邻结点是父节点，到达底层，剪枝
            continue;
        treeDP(y,x);
        num[x]+=num[y];
        for(int j=num[x];j>=0;j--)//容量
            for(int k=1;k<=num[y];k++)//用户
                dp[x][j]=max(dp[x][j],dp[y][k]+dp[x][j-k]-edge[i].dis);
    }
}
int main()
{
    memset(num,0,sizeof(num));
    memset(dp,~0x3f,sizeof(dp));
    memset(head,-1,sizeof(head));

    scanf("%d%d",&n,&m);
    for(int x=1;x<=n-m;x++){
        int k;
        scanf("%d",&k);
        for(int j=1;j<=k;j++){
            int y,w;
            scanf("%d%d",&y,&w);
            addEdge(x,y,w);
            addEdge(y,x,w);
        }
    }
    for(int i=n-m+1;i<=n;i++)
        scanf("%d",&dis[i]);

    treeDP(1,-1);
    for(int i=n;i>=1;i--){
        if(dp[1][i]>=0){
            printf("%d\n",i);
            break;
        }
    }

	return 0;
}
```






