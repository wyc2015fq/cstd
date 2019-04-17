# Leading and Trailing（LightOJ-1282） - Alex_McAvoy的博客 - CSDN博客





2019年03月19日 18:37:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[LightOJ																[数论——快速幂与同余式定理](https://blog.csdn.net/u011815404/article/category/8796909)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

You are given two integers: n and k, your task is to find the most significant three digits, and least significant three digits of nk.

# Input

Input starts with an integer T (≤ 1000), denoting the number of test cases.

Each case starts with a line containing two integers: n (2 ≤ n < 231) and k (1 ≤ k ≤ 107).

# Output

For each case, print the case number and the three leading digits (most significant) and three trailing digits (least significant). You can assume that the input is given such that nk contains at least six digits.

# **Sample Input**

**5123456 1123456 22 312 3229 8751919**

# Sample Output

**Case 1: 123 456Case 2: 152 936Case 3: 214 648Case 4: 429 296Case 5: 665 669**


题意：t 组数据，每组给出两个数 n、k，求 n^k 的前 3 位数和后 3 位数

思路：

对于后 3 位数，直接快速幂取模就行了

对于前 3 位数，同样快速幂，只不过不是取模，而是如果大于 1000 就除以 10

此外，注意后三位数要补足 3 位输出

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
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
LL quickPowMod(LL a,LL b,LL mod){
    LL res=1;
    while(b){
        if(b%2)
            res=res*a%mod;
        a=a*a%mod;
        b/=2;
    }
    return res;
}
double getNumber(double x){
    while(x>=1000.0)
        x/=10.0;
    return x;
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n,k;
        scanf("%lld%lld",&n,&k);

        int behind=(int)quickPowMod(n,k,1000);//后三位

        //前三位
        double temp=1.0,a=n*1.0;
        while(k){
            if(k%2){
                temp=temp*a;
                temp=getNumber(temp);//获取前三位
            }
            a=a*a;
            a=getNumber(a);//获取前三位
            k/=2;
        }
        int above=(int)temp;//转为整型

        printf("Case %d: %d %03d\n",Case++,above,behind);
    }
    return 0;
}
```






