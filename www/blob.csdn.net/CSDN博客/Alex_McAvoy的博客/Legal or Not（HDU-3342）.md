# Legal or Not（HDU-3342） - Alex_McAvoy的博客 - CSDN博客





2018年11月04日 15:19:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48








> 
# Problem Description

ACM-DIY is a large QQ group where many excellent acmers get together. It is so harmonious that just like a big family. Every day,many "holy cows" like HH, hh, AC, ZT, lcc, BF, Qinz and so on chat on-line to exchange their ideas. When someone has questions, many warm-hearted cows like Lost will come to help. Then the one being helped will call Lost "master", and Lost will have a nice "prentice". By and by, there are many pairs of "master and prentice". But then problem occurs: there are too many masters and too many prentices, how can we know whether it is legal or not?

We all know a master can have many prentices and a prentice may have a lot of masters too, it's legal. Nevertheless，some cows are not so honest, they hold illegal relationship. Take HH and 3xian for instant, HH is 3xian's master and, at the same time, 3xian is HH's master,which is quite illegal! To avoid this,please help us to judge whether their relationship is legal or not. 

Please note that the "master and prentice" relation is transitive. It means that if A is B's master ans B is C's master, then A is C's master.

# **Input**

The input consists of several test cases. For each case, the first line contains two integers, N (members to be tested) and M (relationships to be tested)(2 <= N, M <= 100). Then M lines follow, each contains a pair of (x, y) which means x is y's master and y is x's prentice. The input is terminated by N = 0.

TO MAKE IT SIMPLE, we give every one a number (0, 1, 2,..., N-1). We use their numbers instead of their names.

# Output

For each test case, print in one line the judgement of the messy relationship.

If it is legal, output "YES", otherwise "NO".

# Sample Input

**3 20 11 22 20 11 00 0**

# Sample Output

**YESNO**


题意：给出一个 n 个点 m 条边的有向图，要求判断图能否进行拓扑排序

思路：套用模版判断图是否具环即可，要注意的是，点的序号为 0 到 n-1

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
vector<int> G[N];//G[i]表示i节点所指向的所有其他点
bool judgeTopsort()//判断该图是否可拓扑排序
{
    stack<int> S;
    int cnt=0;//记录可拆解的点数目
    for(int i=0;i<=n-1;i++)//枚举编号从0到n-1的点
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
    while(scanf("%d%d",&n,&m)==2&&n)
    {
        memset(in,0,sizeof(in));
        for(int i=0;i<=n;i++)
            G[i].clear();

        while(m--) {
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            in[y]++;
        }

        printf("%s\n",judgeTopsort()?"YES":"NO");
    }
    return 0;
}
```





