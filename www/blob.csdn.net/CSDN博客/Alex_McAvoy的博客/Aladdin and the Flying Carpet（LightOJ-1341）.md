# Aladdin and the Flying Carpet（LightOJ-1341） - Alex_McAvoy的博客 - CSDN博客





2019年03月18日 21:34:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：15
个人分类：[LightOJ																[数论——唯一分解定理](https://blog.csdn.net/u011815404/article/category/8796922)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

It's said that Aladdin had to solve seven mysteries before getting the Magical Lamp which summons a powerful Genie. Here we are concerned about the first mystery.

Aladdin was about to enter to a magical cave, led by the evil sorcerer who disguised himself as Aladdin's uncle, found a strange magical flying carpet at the entrance. There were some strange creatures guarding the entrance of the cave. Aladdin could run, but he knew that there was a high chance of getting caught. So, he decided to use the magical flying carpet. The carpet was rectangular shaped, but not square shaped. Aladdin took the carpet and with the help of it he passed the entrance.

Now you are given the area of the carpet and the length of the minimum possible side of the carpet, your task is to find how many types of carpets are possible. For example, the area of the carpet 12, and the minimum possible side of the carpet is 2, then there can be two types of carpets and their sides are: {2, 6} and {3, 4}.

# Input

Input starts with an integer T (≤ 4000), denoting the number of test cases.

Each case starts with a line containing two integers: a b (1 ≤ b ≤ a ≤ 1012) where a denotes the area of the carpet and b denotes the minimum possible side of the carpet.

# Output

For each case, print the case number and the number of possible carpets.

# **Sample Input**

**210 212 2**

# Sample Output

**Case 1: 1Case 2: 2**


题意：t 组数据，每组给出 2 个数 a、b，求满足 c*d=a 且 c>=b、d>=b 的二元组数，其中 (c,d) 与 (d,c) 视为同一种情况

思路：

根据唯一分解定理，先将 a 进行分解，那么可以得到 a 的所有个数  cnt=(1+a1)*(1+a2)*...*(1+an)，由于不会出现 c=d 的情况，因此将 cnt 除以 2，去掉重复情况，最后进行枚举，从中减去小于 b 的 a 的约数即可

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

LL prime[N],cnt;
bool bprime[N];
void make_prime(){
	memset(bprime,false,sizeof(bprime));

    for(LL i=2;i<=N;i++){
        if(!bprime[i]){
            prime[cnt++]=i;
            for(LL j=i*2;j<=N;j+=i)
                bprime[j]=true;
        }
    }
}
LL calculate(LL n){
    LL res=1;
    for(LL i=0;i<cnt&&prime[i]<=sqrt(n);i++){
        LL num=0;
        while(n%prime[i]==0){
            num++;
            n/=prime[i];
        }
        res*=num+1;
    }

    if(n!=1)//不为1时，考虑有序元组
        res=res*2;
    return res;//返回素因子个数
}

int main(){
    make_prime();

    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL a,b;
        scanf("%lld%lld",&a,&b);

        if(a<b*b)
            printf("Case %d: %d\n",Case++,0);
        else{
            LL cnt=calculate(a)/2;//除以2，去重
            for(LL j=1;j<b;j++)//枚举小于b的约数
                if(a%j==0)
                    cnt--;
            printf("Case %d: %lld\n",Case++,cnt);
        }

    }
    return 0;
}
```






