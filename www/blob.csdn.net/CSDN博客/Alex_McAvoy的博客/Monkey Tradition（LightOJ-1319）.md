# Monkey Tradition（LightOJ-1319） - Alex_McAvoy的博客 - CSDN博客





2019年03月23日 15:32:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[LightOJ																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

In 'MonkeyLand', there is a traditional game called "Bamboo Climbing". The rules of the game are as follows:

1)       There are N monkeys who play this game and there are N bamboos of equal heights. Let the height be L meters.

2)       Each monkey stands in front of a bamboo and every monkey is assigned a different bamboo.

3)       When the whistle is blown, the monkeys start climbing the bamboos and they are not allowed to jump to a different bamboo throughout the game.

4)       Since they are monkeys, they usually climb by jumping. And in each jump, the ith monkey can jump exactly pi meters (pi is a prime). After a while when a monkey finds that he cannot jump because one more jump may get him out of the bamboo, he reports the remaining length ri that he is not able to cover.

5)       And before the game, each monkey is assigned a distinct pi.

6)       The monkey, who has the lowest ri, wins.

Now, the organizers have found all the information of the game last year, but unluckily they haven't found the height of the bamboo. To be more exact, they know N, all pi and corresponding ri, but not L. So, you came forward and found the task challenging and so, you want to find L, from the given information.

# Input

Input starts with an integer T (≤ 10000), denoting the number of test cases.

Each case starts with a line containing an integer n (1 ≤ n ≤ 12). Each of the next n lines contains two integers pi (1 < pi < 40, pi is a prime) and ri (0 < ri < pi). All pi will be distinct.

# Output

For each case, print the case number and the minimum possible value of L that satisfies the above conditions. If there is no solution, print 'Impossible'.

# Sample Input

**235 47 611 342 13 25 37 1**

# Sample Output

**Case 1: 69Case 2: 113**


题意：t 组数据，每组给出 n 个数 p、m，代表 k%p=m，其中 p 是质数，求最小的 k

思路：将 n 个数转换为同余方程的形式，于是得到一个线性同余方程组，由于 p 是质数，因此直接用中国剩余定理解方程组即可

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
const int MOD=10007;
const int N=100+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

LL Extended_GCD(LL a,LL b,LL &x,LL &y)
{
    if(b==0)
    {
        x=1;
        y=0;
        return a;
    }

    LL gcd=Extended_GCD(b,a%b,y,x);
    y=y-a/b*x;
    return gcd;
}
LL China(LL W[],LL B[],LL k){//W除数，B余数
    LL mod=1;
    for(int i=0;i<k;i++)//计算mod的大小
        mod*=W[i];

    LL res=0;
    LL x,y,m;
    for(int i=0;i<k;i++){
        m=mod/W[i];
        Extended_GCD(W[i],m,x,y);//求出每一组W[i]与m的解
        res=(res+y*B[i]*mod/W[i]+mod)%mod;//累加所有解
    }
    return (res+mod)%mod;
}
LL p[N],m[N];
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);

        for(int i=0;i<n;i++)
            scanf("%lld%lld",&p[i],&m[i]);


        LL res=China(p,m,n);

        printf("Case %d: %lld\n",Case++,res);
    }
    return 0;
}
```






