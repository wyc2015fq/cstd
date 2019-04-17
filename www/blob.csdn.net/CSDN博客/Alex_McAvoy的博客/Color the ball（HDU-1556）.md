# Color the ball（HDU-1556） - Alex_McAvoy的博客 - CSDN博客





2019年03月25日 18:51:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：7
个人分类：[HDU																[数据结构——差分数组](https://blog.csdn.net/u011815404/article/category/8715535)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

N个气球排成一排，从左到右依次编号为1,2,3....N.每次给定2个整数a b(a <= b),lele便为骑上他的“小飞鸽"牌电动车从气球a开始到气球b依次给每个气球涂一次颜色。但是N次以后lele已经忘记了第I个气球已经涂过几次颜色了，你能帮他算出每个气球被涂过几次颜色吗？

# **Input**

每个测试实例第一行为一个整数N,(N <= 100000).接下来的N行，每行包括2个整数a b(1 <= a <= b <= N)。

当N = 0，输入结束。

# Output

每个测试实例输出一行，包括N个整数，第I个数代表第I个气球总共被涂色的次数。

# Sample Input

**31 12 23 331 11 21 30**

# Sample Output

**1 1 13 2 1**


思路：差分数组

直接记录各次操作，并求差分数组，然后对差分数组进行对应修改，即加 1，最后直接求各项的值即可

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
#define LL long long
const int MOD=1000000007;
const int N=100000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int n;
int f[N];
int a[N];
int main(){

    while(scanf("%d",&n)!=EOF&&n){
        memset(f,0,sizeof(f));
        for(int i=1;i<=n;i++){//对差分数组修改
            int l,r;
            scanf("%d%d",&l,&r);
            f[l]+=1;
            f[r+1]-=1;
        }
        for(int i=1;i<=n;i++)//单点查询每个值
            a[i]=a[i-1]+f[i];

        for(int i=1;i<=n;i++)
            printf("%d ",a[i]);
        printf("\n");

    }
    return 0;
}
```






