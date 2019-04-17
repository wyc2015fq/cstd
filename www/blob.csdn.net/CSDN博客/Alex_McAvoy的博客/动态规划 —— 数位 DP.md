# 动态规划 ——  数位 DP - Alex_McAvoy的博客 - CSDN博客





2018年09月04日 20:00:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：89








# 【概述】

数位 DP 实际是一种计数用的 DP，一般就是统计一个区间 [le,ri] 内满足一些条件数的个数。

所求的限定条件往往与数的位数有关，例如：数位之和、指定数码个数、数的大小顺序分组等。

题目所给的区间范围往往很大，无法通过暴力枚举来求解，一般是要求把某个区间的符合某种特征的数的个数求出来，因此根据根据 “逐位确定” 的基本思想，将最大数按位分解，然后 dfs 依次判断每一位相应的数是否满足要求。 

# 【基本思想】

数位 DP 的实质就是一种暴力枚举的方式，使得新的枚举方式满足 DP 的性质，然后进行记忆化搜索。

对于一个求区间 [le,ri] 满足条件数的个数，最简单的暴力方式为：

```cpp
for(int i=le;i<=ri;i++)
    if(right(i))
        ans++;
```

但这样枚举没有任何状态，不便于进行记忆化，因此，可以控制上界，从最高位向下枚举。

例如：要求比 456 小的数，可以如下考虑

```
4          5             6

4　        5           (0~6)

4        (0~4)         (0~9)

(0~3)    (0~9)         (0~9)
```

此时，这种新的枚举只控制了上界，统计 [1,ri] 和 [1,le-1] 的数量，然后相减即为区间 [le,ri] 的数量，需要注意的是 le 的范围都是大于等于 1 的

```cpp
int main()
{
    long long le,ri;
    while(~scanf("%lld%lld",&le,&ri))
        printf("%lld\n",solve(ri)-solve(le-1));
}
```

# 【模板】

枚举到当前位置 pos，状态为 state 的数量，dp 值保存的是满足条件数的个数。

```cpp
typedef long long LL;
using namespace std;
int a[20];
LL dp[20][state];//不同题目状态不同
/*
    pos 枚举到当前位置
    lead 前导零，不是每个题都要判断
    limit 数位上界变量
*/
LL dfs(int pos,state变量,bool lead,bool limit)
{
    /*
        递归边界，由于按位枚举，最低位是0，此时说明枚举完了
        此时一般返回1，表示枚举的这个数是合法的，
        因此要保证枚举时每一位都要满足题目条件，
        即当前枚举到pos位，
    */
    if(pos==-1)
        return 1;

    if(!limit && !lead && dp[pos][state]!=-1) //记忆化
        return dp[pos][state];

    int up=limit?a[pos]:9;//根据 limit 判断枚举的上界

    LL ans=0;
    for(int i=0;i<=up;i++)//枚举
    {
        /*把不同情况的个数加到ans*/
        if()
            ...
        else if()
            ...
        else
            ...

        ans+=dfs(pos-1,状态转移,lead && i==0,limit && i==a[pos])
    }

    if(!limit && !lead)//计算完，记录状态
        dp[pos][state]=ans;
    return ans;
}
LL solve(LL x)
{
    int pos=0;
    while(x)//分解数位
    {
        a[pos++]=x%10;//存储各数位
        x/=10;
    }
    return dfs(pos-1,一系列状态,true,true);//从最高位开始枚举
}
int main()
{
    LL le,ri;
    while(~scanf("%lld%lld",&le,&ri))
    {
        memset(dp,-1,sizeof(dp));//一般初始化为-1
        printf("%lld\n",solve(ri)-solve(le-1));
    }
}
```

# 【例题】
- Fast Bit Calculations（LightOJ-1032）**(模版题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82429699)
- 不要62（HDU-2089）(**模版题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82389431)
- Amount of Degrees（Ural-1057）**(模版题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82429699)
- 数字0-9的数量（51Nod-1042）**(模版题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/89321086)
- 处女座和小姐姐（三）（2019牛客寒假算法基础集训营 Day3-G）**(模版题)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86659470)
- Bomb（HDU-3555）**(类似 不要62，注意数据精度)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82389619)
- Magic Numbers（CF-628D）**(左闭右闭区间，左端点特判)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86518420)
- Balanced Number（HDU-3709）**(注意以力矩作为状态)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82429013)
- F(x)（HDU-4734）**(先计算F(a)，再找出比其小的即可)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82429265)
- B-number（HDU-3652）**(需要三维数组)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82389894)
- Balanced Numbers（SPOJ-BALNUM）**(三种状态，用三进制表示)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82667772)
- windy数（BZOJ-1026）**(数位+前缀和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82597707)
- K-th Nya Number（HDU-3943）**(数位+二分)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82668338)
- Apocalypse Someday（POJ-3208）**(数位+二分，设置二分上界时要换为长整型极大值0x3f3f3f3f3f3f3f3f)**：[点击这里](https://blog.csdn.net/u011815404/article/details/82533435)



