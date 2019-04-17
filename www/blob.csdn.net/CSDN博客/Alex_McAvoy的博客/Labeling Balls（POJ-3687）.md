# Labeling Balls（POJ-3687） - Alex_McAvoy的博客 - CSDN博客





2019年02月24日 15:35:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：20
个人分类：[POJ																[图论——AOV网与拓扑排序](https://blog.csdn.net/u011815404/article/category/7813285)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Windy has N balls of distinct weights from 1 unit to N units. Now he tries to label them with 1 to N in such a way that:
- No two balls share the same label.
- The labeling satisfies several constrains like "The ball labeled with a is lighter than the one labeled with b".

Can you help windy to find a solution?

# **Input**

The first line of input is the number of test case. The first line of each test case contains two integers, N (1 ≤ N ≤ 200) and M (0 ≤ M ≤ 40,000). The next M line each contain two integers a and b indicating the ball labeled with a must be lighter than the one labeled with b. (1 ≤ a, b ≤ N) There is a blank line before each test case.

# Output

For each test case output on a single line the balls' weights from label 1 to label N. If several solutions exist, you should output the one with the smallest weight for label 1, then with the smallest weight for label 2, then with the smallest weight for label 3 and so on... If no solution exists, output -1 instead.

# Sample Input

**5**

**4 0**

**4 11 1**

**4 21 22 1**

**4 12 1**

**4 13 2**

# Sample Output

**1 2 3 4-1-12 1 3 41 3 2 4**


题意：t 组数据，每组给出 n 个质量从 1 到 n 的球，要求对这些球进行编号，使得所有球的编号都不一样且满足 m 个约束条件，这 m 个约束条件为 a b，代表编号 a 的球比编号 b 的球轻，最后要求输出这 n 个球的重量，若不能满足约束条件，则输出 -1

思路：拓扑排序

按照轻重将 m 个约束条件进行建图，然后按编号从大到小进行遍历，对于球 i 若没有比他更重的，那么这个球就是最重的，从图中去除相应关系即可，以此类推，直到约束关系全部被满足或不被满足

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
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int n,m;
vector<int> G[N];
int in[N];
int res[N];
bool vis[N];
bool topSort(){
    for(int i=n;i>=1;i--){
        int pos=-1;
        for(int j=n;j>=1;j--){
            if(!vis[j]&&in[j]==0){
                pos=j;
                break;
            }
        }

        if(pos==-1)
            return false;

        for(int j=0;j<G[pos].size();j++)
            in[G[pos][j]]--;

        vis[pos]=true;
        res[pos]=i;
    }
	return true;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(vis,false,sizeof(vis));
        memset(in,0,sizeof(in));
        for(int i=0;i<N;i++)
            G[i].clear();

        scanf("%d%d",&n,&m);
        while(m--){
            int a,b;
            scanf("%d%d",&a,&b);
            G[b].push_back(a);
            in[a]++;
        }

        if(topSort()){
            for(int i=1;i<=n;i++)
                printf("%d ",res[i]);
            printf("\n");
        }
        else
            printf("-1\n");

    }
    return 0;
}
```






