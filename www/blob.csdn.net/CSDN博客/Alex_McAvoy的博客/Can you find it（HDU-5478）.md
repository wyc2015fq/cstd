# Can you find it（HDU-5478） - Alex_McAvoy的博客 - CSDN博客





2019年01月17日 16:54:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：77
个人分类：[数学——其他																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/8279050)








> 
# Problem Description

Given a prime number C(1≤C≤2×105), and three integers k1, b1, k2 (1≤k1,k2,b1≤109). Please find all pairs (a, b) which satisfied the equation![a^{k1*n+b1} + b^{k2*n-k2+1} = 0 (mod \:\:C)](https://private.codecogs.com/gif.latex?a%5E%7Bk1*n&plus;b1%7D%20&plus;%20b%5E%7Bk2*n-k2&plus;1%7D%20%3D%200%20%28mod%20%5C%3A%5C%3AC%29)(n = 1, 2, 3, ...).

# Input

There are multiple test cases (no more than 30). For each test, a single line contains four integers C, k1, b1, k2.

# Output

First, please output "Case #k: ", k is the number of test case. See sample output for more detail. 

Please output all pairs (a, b) in lexicographical order. (1≤a,b<C). If there is not a pair (a, b), please output -1.

# Examples

**Input**

23 1 1 2

**Output**

Case #1:

1 22


题意：给出一个质数 c，三个正整数 k1，b1，k2，对于公式 ![a^{k1*n+b1} + b^{k2*n-k2+1} = 0 (mod \:\:C)](https://private.codecogs.com/gif.latex?a%5E%7Bk1*n&plus;b1%7D%20&plus;%20b%5E%7Bk2*n-k2&plus;1%7D%20%3D%200%20%28mod%20%5C%3A%5C%3AC%29)，若能找出最小的 a，b 则输出，若找不出则输出 -1

思路：公式题

对于公式 ![a^{k1*n+b1} + b^{k2*n-k2+1} = 0 (mod \:\:C)](https://private.codecogs.com/gif.latex?a%5E%7Bk1*n&plus;b1%7D%20&plus;%20b%5E%7Bk2*n-k2&plus;1%7D%20%3D%200%20%28mod%20%5C%3A%5C%3AC%29)

当 n=1 时，![a^{k1+b1} + b = 0 \:(mod \:\:C)](https://private.codecogs.com/gif.latex?a%5E%7Bk1&plus;b1%7D%20&plus;%20b%20%3D%200%20%5C%3A%28mod%20%5C%3A%5C%3AC%29) ①

当 n=2 时，![a^{2*k1+b1} + b^{k2+1} = 0 (mod \:\:C)](https://private.codecogs.com/gif.latex?a%5E%7B2*k1&plus;b1%7D%20&plus;%20b%5E%7Bk2&plus;1%7D%20%3D%200%20%28mod%20%5C%3A%5C%3AC%29) ②

令 ![a^{k1}](https://private.codecogs.com/gif.latex?a%5E%7Bk1%7D) * ②，![a^{2*k1+b1} + a^{k1}b= 0 (mod \:\:C)](https://private.codecogs.com/gif.latex?a%5E%7B2*k1&plus;b1%7D%20&plus;%20a%5E%7Bk1%7Db%3D%200%20%28mod%20%5C%3A%5C%3AC%29) ③

令 ③-②，![a^{k1}b+b^{k2+1}= 0 (mod \:\:C)](https://private.codecogs.com/gif.latex?a%5E%7Bk1%7Db&plus;b%5E%7Bk2&plus;1%7D%3D%200%20%28mod%20%5C%3A%5C%3AC%29)

即：![b(a^{k1}+b^{k2})= 0 (mod \:\:C)](https://private.codecogs.com/gif.latex?b%28a%5E%7Bk1%7D&plus;b%5E%7Bk2%7D%29%3D%200%20%28mod%20%5C%3A%5C%3AC%29)

由于 a、b < c，只需要枚举 a 的值(1~c-1)，通过快速幂计算 ![a^{k1}](https://private.codecogs.com/gif.latex?a%5E%7Bk1%7D)、![a^{k1+b1}](https://private.codecogs.com/gif.latex?a%5E%7Bk1&plus;b1%7D)，从而算出 b，再求出 ![b^{k2}](https://private.codecogs.com/gif.latex?b%5E%7Bk2%7D)，比较 ![a^{k1}\:\:(mod\:\:C)](https://private.codecogs.com/gif.latex?a%5E%7Bk1%7D%5C%3A%5C%3A%28mod%5C%3A%5C%3AC%29) 是否等于 ![b^{k2}\:\:(mod\:\:C)](https://private.codecogs.com/gif.latex?b%5E%7Bk2%7D%5C%3A%5C%3A%28mod%5C%3A%5C%3AC%29) 即可

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
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 500001
#define LL long long
using namespace std;
LL Pow_Mod(LL a, LL b, LL m)
{
    LL res=1;
    while(b)
    {
        if(b&1)
            res=(res*a)%m;
        a=(a*a)%m;
        b>>=1;
    }
    return res;
}

int main(){
    LL c,k1,b1,k2;
    int Case=1;
    while(scanf("%lld%lld%lld%lld",&c,&k1,&b1,&k2)!=EOF){
        bool flag=false;
        printf("Case #%d:\n",Case++);

        for(LL i=1;i<=c-1;i++){
            LL a=Pow_Mod(i,k1,c);
            LL b=c-Pow_Mod(i,k1+b1,c);
            LL temp=Pow_Mod(b,k2,c);
            if(a==temp){
                flag=true;
                printf("%lld %lld\n",i,b);
            }
        }
        if(!flag)
            printf("-1\n");

    }
    return 0;
}
```






