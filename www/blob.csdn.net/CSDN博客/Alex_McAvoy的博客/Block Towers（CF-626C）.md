# Block Towers（CF-626C） - Alex_McAvoy的博客 - CSDN博客





2019年01月16日 16:41:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：158
个人分类：[CodeForces																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Students in a class are making towers of blocks. Each student makes a (non-zero) tower by stacking pieces lengthwise on top of each other. n of the students use pieces made of two blocks and m of the students use pieces made of three blocks.

The students don’t want to use too many blocks, but they also want to be unique, so no two students’ towers may contain the same number of blocks. Find the minimum height necessary for the tallest of the students' towers.

# Input

The first line of the input contains two space-separated integers n and m (0 ≤ n, m ≤ 1 000 000, n + m > 0) — the number of students using two-block pieces and the number of students using three-block pieces, respectively.

# Output

Print a single integer, denoting the minimum possible height of the tallest tower.

# Examples

**Input**

3 2

**Output**

8

**Input**

5 0

**Output**

10

**Input**

1 3

**Output**

9

————————————————————————————————————————————

题意：若干个学生用砖摞塔，每次摞塔时每个学生只能用两块或三块砖来摞，给出两个数 n、m，n 代表有多少个学生用两块砖，m 代表有多少个学生用 三块砖，每个学生用砖搭成的塔高度都不同，问最大高度的最小值

思路：二分，每次二分枚举高度

根据题设给出的 n、m 的值，可算出初始的高度区间。由于 0 ≤ n, m ≤ 1 000 000，m+n > 1 所以最小的情况是左区间 left=2，最大的情况是右区间 right=3 000 000

每次二分时，根据 mid 的值去判断，mid 高度的情况下，有几个用 2 摞的，有几个用 3 摞的，以及用 2 摞、3 摞垒成的公倍数的个数，然后根据得出的值去比较 n、m 的值，如果大于 n、m 说明高度范围大，需要减少右区间，否则说明高度范围小，需要增加左区间，最后记录满足条件时的 mid 值即可

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
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n,m;
bool judge(int x){
    if( (n<=x/2) && (m<=x/3) &&((n+m)<=(x/2+x/3-x/6)) )
        return true;
    else
        return false;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        int left=2,right=3E7;
        int res;
        while(left<=right){
            int mid=(left+right)>>1;
            if(judge(mid)){
                right=mid-1;
                res=mid;
            }
            else
                left=mid+1;
        }

        printf("%d\n",res);
    }
    return 0;
}
```






