# PIGS（POJ-1149） - Alex_McAvoy的博客 - CSDN博客





2019年01月01日 14:45:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：76
个人分类：[POJ																[图论——网络流](https://blog.csdn.net/u011815404/article/category/8562971)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Mirko works on a pig farm that consists of M locked pig-houses and Mirko can't unlock any pighouse because he doesn't have the keys. Customers come to the farm one after another. Each of them has keys to some pig-houses and wants to buy a certain number of pigs. 

All data concerning customers planning to visit the farm on that particular day are available to Mirko early in the morning so that he can make a sales-plan in order to maximize the number of pigs sold. 

More precisely, the procedure is as following: the customer arrives, opens all pig-houses to which he has the key, Mirko sells a certain number of pigs from all the unlocked pig-houses to him, and, if Mirko wants, he can redistribute the remaining pigs across the unlocked pig-houses. 

An unlimited number of pigs can be placed in every pig-house. 

Write a program that will find the maximum number of pigs that he can sell on that day.

# **Input**

The first line of input contains two integers M and N, 1 <= M <= 1000, 1 <= N <= 100, number of pighouses and number of customers. Pig houses are numbered from 1 to M and customers are numbered from 1 to N. 

The next line contains M integeres, for each pig-house initial number of pigs. The number of pigs in each pig-house is greater or equal to 0 and less or equal to 1000. 

The next N lines contains records about the customers in the following form ( record about the i-th customer is written in the (i+2)-th line): 

A K1 K2 ... KA B It means that this customer has key to the pig-houses marked with the numbers K1, K2, ..., KA (sorted nondecreasingly ) and that he wants to buy B pigs. Numbers A and B can be equal to 0.

# Output

The first and only line of the output should contain the number of sold pigs.

# Sample Input

**3 33 1 102 1 2 22 1 3 31 2 6**

# Sample Output

**7**


题意：m 个猪圈 n 个顾客，第二行给出初始时每个猪圈中猪的个数，之后 n 行首先给出每个顾客会打开猪圈的个数，再给出会打开的猪圈的编号，以及其所能购买的上限。当每个顾客走后，他打开的那些猪圈中的剩下的猪可以任意地被调换到其它开着的猪圈中，然后所有猪圈重新关上，等待下一顾客，问最多能卖多少头猪

思路：

以样例来说，有 3 个猪圈，初始时 1 号有 3 头猪，2 号有 1 头猪，3 号有 10 头猪，第一个顾客打开 1、2 号猪圈最多买 2 头猪，第二个顾客打开 1、3 号猪圈最多买 3 头猪，第三个顾客打开第 2 号猪圈最多买 6 头猪。最理想的情况是，第一个顾客打开 1、2 号门，买第 1 号的 2 头猪，再将剩下的猪都赶入 2 号圈，第二个顾客打开 1、3 猪圈买第 3 号猪圈的 3 头猪，第三个顾客打开第 2 号猪圈买 2 头猪，因此最理想的情况就是买 2+3+2=7 头猪。

先假设图中有一源点与汇点，根据样例可看出，n 个顾客就有 n 轮交易，每一轮都有 m 个猪圈和 1 个顾客结点，可将每个顾客看做一个结点，对于每个猪圈的第一个顾客，从源点向他连一条边，容量就是猪圈中的初始数量，从顾客到汇点之间各有一条边，容量是各顾客能买的数量上限，建图后直接套用 EK 模版即可。

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
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n,m;
int cap[N][N],flow[N][N];
int EK(int s,int t){
    int res=0;
    int dis[N];
    int p[N];
    memset(p,0,sizeof(p));

    queue<int> Q;
    while(true){
        memset(dis,0,sizeof(dis));
        dis[s]=INF;
        Q.push(s);

        while(!Q.empty()){
            int x=Q.front();
            Q.pop();
            for(int y=0;y<=n+1;y++){
                if(!dis[y] && flow[x][y]<cap[x][y]){
                    p[y]=x;
                    Q.push(y);
                    dis[y]=min(dis[x],cap[x][y]-flow[x][y]);
                }
            }
        }

       if(dis[t]==0)
            break;

       for(int x=t;x!=s;x=p[x]){
           flow[p[x]][x]+=dis[t];
           flow[x][p[x]]-=dis[t];
       }
       res+=dis[t];
   }
   return res;
}
int pigNum[N];
int last[N];//last[i]存储上一个开猪圈i的人
int main(){
    while(scanf("%d%d",&m,&n)!=EOF&&(n+m)){
        memset(last,0,sizeof(last));
        memset(cap,0,sizeof(cap));
        memset(flow,0,sizeof(flow));
        int s=0,t=n+1;

        for(int i=1;i<=m;i++)
            scanf("%d",&pigNum[i]);
        for(int i=1;i<=n;i++){
            int num;
            scanf("%d",&num);
            while(num--){
                int temp;
                scanf("%d",&temp);
                if(last[temp]==0)//两人有重合的猪圈才能转移猪
                    cap[s][i]+=pigNum[temp];
                else
                    cap[last[temp]][i]=INF;
                last[temp]=i;
            }
            scanf("%d",&cap[i][t]);
        }

        printf("%d\n",EK(s,t));
    }
    return 0;
}
```






