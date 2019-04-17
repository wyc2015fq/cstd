# Triangle LOVE（HDU-4324） - Alex_McAvoy的博客 - CSDN博客





2018年11月04日 15:46:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[HDU																[图论——AOV网与拓扑排序](https://blog.csdn.net/u011815404/article/category/7813285)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Recently, scientists find that there is love between any of two people. For example, between A and B, if A don’t love B, then B must love A, vice versa. And there is no possibility that two people love each other, what a crazy world!

Now, scientists want to know whether or not there is a “Triangle Love” among N people. “Triangle Love” means that among any three people (A,B and C) , A loves B, B loves C and C loves A.

  Your problem is writing a program to read the relationship among N people firstly, and return whether or not there is a “Triangle Love”.

# **Input**

The first line contains a single integer t (1 <= t <= 15), the number of test cases.

For each case, the first line contains one integer N (0 < N <= 2000).

In the next N lines contain the adjacency matrix A of the relationship (without spaces). Ai,j = 1 means i-th people loves j-th people, otherwise Ai,j = 0.

It is guaranteed that the given relationship is a tournament, that is, Ai,i= 0, Ai,j ≠ Aj,i(1<=i, j<=n,i≠j).

# Output

For each case, output the case number as shown and then print “Yes”, if there is a “Triangle Love” among these N people, otherwise print “No”.

Take the sample output for more details.

# Sample Input

**25001001000001001111011100050111100000010000110001110**

# Sample Output

**Case #1: YesCase #2: No**


题意：给出一任意两点间有且仅有一条单向边的有向图，要求判断这个图中是否存在仅有3个结点构成的环

思路：本质是一拓扑排序题

如果存在三个结点的环，则不能进行拓扑排序，

假设有一个 a->b->c->d ... 构成的环，由于任意两点之间一定存在边，对于 a、c 来说，若边为 c->a 则构成一个 3 结点的环，否则 a->c->d-> ... 构成一个 n-1 结点的环，以此类推，必然有 n-1、n-2、...、3 结点的环，因此，若能拓扑排序，则必定存在三节点的环

故对图进行拓扑排序判断即可

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
#define N 10001
#define LL long long
using namespace std;
int n,m;
int in[N];//节点入度
char str[N];
vector<int> G[N];//G[i]表示i节点所指向的所有其他点
bool judgeTopsort()//判断该图是否可拓扑排序
{
    stack<int> S;
    int cnt=0;//记录可拆解的点数目
    for(int i=0;i<n;i++)//枚举编号从1到n的点
        if(in[i]==0)//入度为0，入栈
            S.push(i);

    while(!S.empty()) {
        int x=S.top();//取栈顶元素
        S.pop();

        cnt++;//可拆点数+1
        for(int i=0;i<G[x].size();i++){
            int y=G[x][i];
            in[y]--;//入度减一

            if(in[y]==0)//入度为0，出栈
                S.push(y);
        }
    }

    if(cnt==n)//AOV网点数等于图的点数，不存在环，可进行拓扑排序
        return true;
    else//AOV网点数等于图的点数，存在环，不可进行拓扑排序
        return false;
}
int main()
{
    int t;
    int Case=1;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        memset(in,0,sizeof(in));
        for(int i=0;i<n;i++){
            G[i].clear();

            scanf("%s",str);
            for(int j=0;j<n;j++){
                if(str[j]=='1'){
                    G[i].push_back(j);
                    in[j]++;
                }
            }
        }

        printf("Case #%d: %s\n",Case++,judgeTopsort()?"No":"Yes");
    }
    return 0;
}
```






