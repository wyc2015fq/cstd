# Pupu（HDU-3003） - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 18:52:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[HDU																[数论——快速幂与同余式定理](https://blog.csdn.net/u011815404/article/category/8796909)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

There is an island called PiLiPaLa.In the island there is a wild animal living in it, and you can call them PuPu. PuPu is a kind of special animal, infant PuPus play under the sunshine, and adult PuPus hunt near the seaside. They fell happy every day. 

But there is a question, when does an infant PuPu become an adult PuPu? 

Aha, we already said, PuPu is a special animal. There are several skins wraping PuPu's body, and PuPu's skins are special also, they have two states, clarity and opacity. The opacity skin will become clarity skin if it absorbs sunlight a whole day, and sunshine can pass through the clarity skin and shine the inside skin; The clarity skin will become opacity, if it absorbs sunlight a whole day, and opacity skin will keep sunshine out. 

when an infant PuPu was born, all of its skins were opacity, and since the day that all of a PuPu's skins has been changed from opacity to clarity, PuPu is an adult PuPu. 

For example, a PuPu who has only 3 skins will become an adult PuPu after it born 5 days(What a pity! The little guy will sustain the pressure from life only 5 days old) 

Now give you the number of skins belongs to a new-laid PuPu, tell me how many days later it will become an adult PuPu?

# Input

There are many testcase, each testcase only contains one integer N, the number of skins, process until N equals 0

# Output

Maybe an infant PuPu with 20 skins need a million days to become an adult PuPu, so you should output the result mod N

# **Sample Input**

**230**

# Sample Output

**12**


题意：每组数据给出一个数 n，代表 pupu 有 n 层皮肤，每层皮肤有两种状态：透明、非透明，当阳光照射 1 天后，会改变状态，pupu 有一开始所有的皮肤都是不透明的，若某天他的皮肤都曾变透明过，那么说明他长大了，求长大的天数，最后结果 mod n

思路：

首先模拟一下 4 层皮肤的照射过程，1 代表皮肤不透明，0 代表透明

![](https://img-blog.csdnimg.cn/20190216160845772.png)

可以看到，对于每一层皮肤，透明的可以继续向下照射直到遇到不透明为止，经过照射后，所有的皮肤都会改变状态

由于一开始所有皮肤都是不透明的，要想所有皮肤都曾透明过，只需让前 n-1 层皮肤透明，最后一天照射第 n 层后，总天数 +1 即可

那么，仅需考虑前 n-1 层从不透明全变为透明即可，由于每层有两个状态，所有皮肤初始都是不透明的，要让所有都变为透明的，则需要 2^(n-1) 天

因此结果为 2^(n-1)+1，最后需要再 mod n，即：(2^(n-1)+1)%n，使用快速幂取模即可

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
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=10;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
LL quickPow(LL a,LL b,LL mod){
    LL res=1;
    while(b>0){
        if(b&1)
            res=(res%mod*a%mod)%mod;
        b>>=1;
        a=(a%mod*a%mod)%mod;
    }
    return (res+1)%mod;
}
int main(){
    LL n;
    while(scanf("%lld",&n)!=EOF&&n){
        cout<<quickPow(2,n-1,n)<<endl;
    }
    return 0;
}
```






