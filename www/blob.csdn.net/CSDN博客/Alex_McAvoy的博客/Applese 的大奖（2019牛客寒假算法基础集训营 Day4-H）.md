# Applese 的大奖（2019牛客寒假算法基础集训营 Day4-H） - Alex_McAvoy的博客 - CSDN博客





2019年02月03日 13:57:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：111








> 
# 【题目描述】

Applese 和它的小伙伴参加了一个促销的抽奖活动，活动的规则如下：有一个随机数生成器，能等概率生成 0∼99 之间的整数，每个参与活动的人都要通过它获取一个随机数。最后得到数字最小的 k 个人可以获得大奖。如果有相同的数，那么后选随机数的人中奖。

Applese 自然是最心急的一个，它会抢在第一个去按随机数。请你帮忙计算一下它能够中奖的概率。

# 【输入描述】

仅一行三个正整数 n, k, x，分别表示参与抽奖的总人数（包括Applese）,中奖的人数和 Applese 获得的随机数。

1≤n≤109

1≤k≤min{n,105}

0≤x≤99

# 【输出描述】

输出一个正整数表示 Applese 中奖的概率 mod10^9+7。

即如果概率等于ab，a,b∈N 且 gcd(a,b)=1，你需要输出一个自然数 c<109+7 满足 bc≡a(mod10^9+7)

# 【样例】

示例1

输入

1 1 99

输出

1

示例2

输入

2 1 38

输出

770000006

示例3

输入

6 2 49

输出

687500005


思路：

枚举 Applese 的名次，分别计算概率，设 p 为随机到的数不大于 Applese 的概率，有：![\sum_{i=0}^{k-1}p_i(1-p_i)^{n-i-1}](https://private.codecogs.com/gif.latex?%5Csum_%7Bi%3D0%7D%5E%7Bk-1%7Dp_i%281-p_i%29%5E%7Bn-i-1%7D)


由于数据范围，因此需要预处理逆元递推组合数

# 【源代码】

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
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=1e9+7;
using namespace std;
LL powMod(LL a,LL b){
    LL res=1;
    while(b){
        if(b&1)
            res=(res*a)%MOD;
        b>>=1;
        a=(a*a)%MOD;
    }
    return res;
}
LL inv(LL x){
    return powMod(x,MOD-2);
}
LL c[N];
int main(){
    LL n,k,x;
    cin>>n>>k>>x;

    if(n==k)
        cout<<1<<endl;
    else{
        //预处理逆元
        LL A=(x+1)*inv(100)%MOD;
        LL B=(99-x)*inv(100)%MOD;
        LL invB=inv(B);



        //计算组合数
        c[0]=1;
        for(int i=1;i<=k;i++)
            c[i]=c[i-1]*inv(i)%MOD*((n-1)+1-i)%MOD;

        LL res=0;
        LL a=1;
        LL b=powMod(B,n-1);
        for(int i=0;i<k;i++){
            res+=c[i]*a%MOD*b%MOD;
            if(res>=MOD)
                res%=MOD;

            a=a*A%MOD;
            b=b*invB%MOD;
        }

        cout<<res<<endl;
    }

    return 0;
}
```





