# Network（POJ-1144） - Alex_McAvoy的博客 - CSDN博客





2018年10月28日 00:11:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36
个人分类：[POJ																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

A Telephone Line Company (TLC) is establishing a new telephone cable network. They are connecting several places numbered by integers from 1 to N . No two places have the same number. The lines are bidirectional and always connect together two places and in each place the lines end in a telephone exchange. There is one telephone exchange in each place. From each place it is 

possible to reach through lines every other place, however it need not be a direct connection, it can go through several exchanges. From time to time the power supply fails at a place and then the exchange does not operate. The officials from TLC realized that in such a case it can happen that besides the fact that the place with the failure is unreachable, this can also cause that some other places cannot connect to each other. In such a case we will say the place (where the failure 

occured) is critical. Now the officials are trying to write a program for finding the number of all such critical places. Help them.

# Input

The input file consists of several blocks of lines. Each block describes one network. In the first line of each block there is the number of places N < 100. Each of the next at most N lines contains the number of a place followed by the numbers of some places to which there is a direct line from this place. These at most N lines completely describe the network, i.e., each direct connection of two places in the network is contained at least in one row. All numbers in one line are separated by one space. Each block ends with a line containing just 0. The last block has only one line with N = 0;

# Output

The output contains for each block except the last in the input file one line containing the number of critical places.

# Sample Input

5

5 1 2 3 4

0

6

2 1 3

5 4 6 2

0

0

# Sample Output

1

2


题意：给出一个无向图，每个实例第一行是 n 表示节点数，接下来最多有 n 行表述边的信息，第一个数表示主顶点 u，接着所有的数字表示副点 vi，即 u 与 vi 之间都有一条边，每行以及每组实例都以 0 表示输入结束，求这个无向图的割点数

思路：直接求割点，Tarjan 模版即可，唯一的难点在于数据的读入

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
int n;
vector<int> G[N];
int dfn[N],low[N];
bool iscut[N];
int block_cnt;
int Tarjan(int x,int father){
    int lowx=dfn[x]=++block_cnt;
    int child=0;

    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(dfn[y]==0){
            child++;

            int lowy=Tarjan(y,x);
            lowx=min(lowx,lowy);
            
            if(lowy>=dfn[x])
                iscut[x]=true;
        }
        else if(dfn[y]<dfn[x] && y!=father)
            lowx=min(lowx,dfn[y]);
    }

    if(father<0 && child==1)
        iscut[x]=false;

    return low[x]=lowx;
}
int main()
{
    while(scanf("%d",&n)!=EOF&&n){
        block_cnt=0;
        memset(dfn,0,sizeof(dfn));
        memset(iscut,0,sizeof(iscut));

        for(int i=1;i<=n;i++)
            G[i].clear();

        char str[10];
        while(scanf("%s",str)!=EOF){
            if(str[0]=='0')
                break;

            int x=0;
            for(int i=0;str[i];i++)
                x=x*10+str[i]-'0';
                
            while(scanf("%s",str)!=EOF){
                int y=0;
                for(int i=0;str[i];i++)
                    y=y*10+str[i]-'0';

                G[x].push_back(y);
                G[y].push_back(x);
                if(getchar()=='\n')
                    break;
            }
        }

        Tarjan(1,-1);

        int res=0;
        for(int i=1;i<=n;i++)
            if(iscut[i])
                res++;
        printf("%d\n",res);
    }
    return 0;
}
```






