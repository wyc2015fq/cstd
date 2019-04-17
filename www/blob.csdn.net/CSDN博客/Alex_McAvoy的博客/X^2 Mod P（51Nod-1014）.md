# X^2 Mod P（51Nod-1014） - Alex_McAvoy的博客 - CSDN博客





2019年04月16日 22:09:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：4
个人分类：[51Nod																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/8862267)








> 
# 题目

X*X mod P = A，其中P为质数。给出P和A，求<=P的所有X。

# 输入

两个数P A，中间用空格隔开。(1 <= A < P <= 1000000, P为质数)

# 输出

输出符合条件的X，且0 <= X <= P，如果有多个，按照升序排列，中间用空格隔开。

如果没有符合条件的X，输出：No Solution

# 输入样例

**13 3**

# 输出样例

**4 9**


思路：

由于给定 p 和 a，假设 x 存在，那么 x 的个数为 2 个，即：x1、x2

由于 x1*x1-x2*x2=n*p，那么：x1+x2=p，即 x1、x2 互补

考虑 a 是否完全平方数即可，当 a 不为完全平方数时，那么就可以排除 0~p 取整后的数据

因此需要枚举（p 的开方数取整后+1）~（p/2 取整）

故暴力即可

# 源程序

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
#define E 1e-9
#define PI acos(-1.0)
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=7;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
LL a[N];
int main(){
    LL p,a;
    scanf("%lld%lld",&p,&a);

    int i=0;
    bool flag=true;
    while(true){
        double x1=sqrt(i*p*1.0+a*1.0);
        int x2=(int)x1;
        if(x2>p)
            break;

        if(x1==x2){
            printf("%d ",x2);
            flag=false;
        }
        i++;
    }
    if(flag)
        printf("No Solution");
    return 0;
}
```






