# Genealogical tree（POJ-2367） - Alex_McAvoy的博客 - CSDN博客





2018年11月04日 16:00:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：95








> 
# Problem Description

The system of Martians' blood relations is confusing enough. Actually, Martians bud when they want and where they want. They gather together in different groups, so that a Martian can have one parent as well as ten. Nobody will be surprised by a hundred of children. Martians have got used to this and their style of life seems to them natural. 

And in the Planetary Council the confusing genealogical system leads to some embarrassment. There meet the worthiest of Martians, and therefore in order to offend nobody in all of the discussions it is used first to give the floor to the old Martians, than to the younger ones and only than to the most young childless assessors. However, the maintenance of this order really is not a trivial task. Not always Martian knows all of his parents (and there's nothing to tell about his grandparents!). But if by a mistake first speak a grandson and only than his young appearing great-grandfather, this is a real scandal. 

Your task is to write a program, which would define once and for all, an order that would guarantee that every member of the Council takes the floor earlier than each of his descendants.

# **Input**

The first line of the standard input contains an only number N, 1 <= N <= 100 — a number of members of the Martian Planetary Council. According to the centuries-old tradition members of the Council are enumerated with the natural numbers from 1 up to N. Further, there are exactly N lines, moreover, the I-th line contains a list of I-th member's children. The list of children is a sequence of serial numbers of children in a arbitrary order separated by spaces. The list of children may be empty. The list (even if it is empty) ends with 0.

# Output

The standard output should contain in its only line a sequence of speakers' numbers, separated by spaces. If several sequences satisfy the conditions of the problem, you are to write to the standard output any of them. At least one such sequence always exists.

# Sample Input

**504 5 1 01 05 3 03 0**

# Sample Output

**2 4 5 3 1**


题意：给出一个 n 个点 m 条边的有向图，要求输出任意一条拓扑排序结果

思路：套用模版判断图是否具环即可

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
int path[N];//存储路径
vector<int> G[N];//G[i]表示i节点所指向的所有其他点
void Topsort()//拓扑排序
{
    stack<int> S;
    int cnt=0;//记录可拆解的点数目
    for(int i=1;i<=n;i++)//枚举编号从1到n的点
        if(in[i]==0)//入度为0，入栈
            S.push(i);

    while(!S.empty()) {
        int x=S.top();//取栈顶元素
        S.pop();

        path[++cnt]=x;//存储可拆点
        for(int i=0;i<G[x].size();i++){
            int y=G[x][i];
            in[y]--;//入度减一

            if(in[y]==0)//入度为0，出栈
                S.push(y);
        }
    }
}
int main()
{
    while(scanf("%d",&n)!=EOF&&n)
    {
        memset(in,0,sizeof(in));
        for(int i=1;i<=n;i++){
            G[i].clear();
            while(true){
                int y;
                scanf("%d",&y);
                if(y==0)
                    break;
                G[i].push_back(y);
                in[y]++;
            }
        }

        Topsort();
        for(int i=1;i<=n;i++)
            printf("%d ",path[i]);
        printf("\n");
    }
    return 0;
}
```





