# Cube Stacking（POJ-1988） - Alex_McAvoy的博客 - CSDN博客





2018年11月17日 15:51:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[POJ																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Farmer John and Betsy are playing a game with N (1 <= N <= 30,000)identical cubes labeled 1 through N. They start with N stacks, each containing a single cube. Farmer John asks Betsy to perform P (1<= P <= 100,000) operation. There are two types of operations: 

moves and counts. 

* In a move operation, Farmer John asks Bessie to move the stack containing cube X on top of the stack containing cube Y. 

* In a count operation, Farmer John asks Bessie to count the number of cubes on the stack with cube X that are under the cube X and report that value. 

Write a program that can verify the results of the game. 

# **Input**

* Line 1: A single integer, P 

* Lines 2..P+1: Each of these lines describes a legal operation. Line 2 describes the first operation, etc. Each line begins with a 'M' for a move operation or a 'C' for a count operation. For move operations, the line also contains two integers: X and Y.For count operations, the line also contains a single integer: X. 

Note that the value for N does not appear in the input file. No move operation will request a move a stack onto itself. 

# Output

Print the output from each of the count operations in the same order as the input file.

# Sample Input

**6M 1 6C 1M 2 4M 2 6C 3C 4**

# Sample Output

**102**


———————————————————————————————————————————————————————

题意：有N个立方体和N个格子，1~N编号，一开始i立方体在i号格子上，每个格子刚好1个立方体。现在m组操作，M a b表示将a号立方体所在的格子的全部立方体放在b号立方体所在的格子的全部立方体上面。C x表示询问x号立方体下面的立方体的个数。

思路：我们需要新增两种属性cnt[i]与s[i]，分别表示i之下的块数和i所在堆的数量。在路径压缩时，cnt[i] += cnt[f[i]] ，另外在连接操作时，需要动态更新cnt[find(u)]和s[find(v)]的信息。

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
#define N 100001
#define LL long long
using namespace std;
int f[N], cnt[N], s[N];
int find(int x){
    if(x != f[x]){
        int fa = f[x];
        f[x] = find(f[x]);
        cnt[x] += cnt[fa];
    }
    return f[x];
}
int main()
{
    for(int i = 0; i < N; i++) {
        f[i] = i;
        s[i] = 1;
    }
    int n;
    scanf("%d", &n);
    char ch;
    int u, v;
    for(int i = 0; i < n; i++){
        getchar();
        scanf("%c", &ch);
        if(ch == 'M'){
            scanf("%d%d", &u, &v);
            int fa = find(u), fb = find(v);
            if(fa != fb){
                f[fa] = fb;
                cnt[fa] = s[fb];
                s[fb] += s[fa];
            }
        }
        else{
            scanf("%d", &u);
            find(u);
            printf("%d\n", cnt[u]);
        }
    }
    return 0;
}
```




