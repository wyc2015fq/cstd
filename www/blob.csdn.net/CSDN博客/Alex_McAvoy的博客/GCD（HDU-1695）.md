# GCD（HDU-1695） - Alex_McAvoy的博客 - CSDN博客





2018年08月12日 21:31:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：96








> 
# Problem Description

Given 5 integers: a, b, c, d, k, you're to find x in a...b, y in c...d that GCD(x, y) = k. GCD(x, y) means the greatest common divisor of x and y. Since the number of choices may be very large, you're only required to output the total number of different number pairs.

Please notice that, (x=5, y=7) and (x=7, y=5) are considered to be the same.

Yoiu can assume that a = c = 1 in all test cases.

# **Input**

The input consists of several test cases. The first line of the input is the number of the cases. There are no more than 3,000 cases. Each case contains five integers: a, b, c, d, k, 0 < a <= b <= 100,000, 0 < c <= d <= 100,000, 0 <= k <= 100,000, as described above.

# Output

For each test case, print the number of choices. Use the format in the example.

# Sample Input

**21 3 1 5 11 11014 1 14409 9**

# Sample Output

**Case 1: 9Case 2: 736427**


题意：给出 a , b , c , d , k 五个数，设 x 属于 [a,b]，y 属于[c,d] ，求有多少对（x,y）满足 GCD(x,y)=k

思路：

由于 GCD(x,y)=k，故 x、y 除了 k 外没有其他公因数，因此 GCD(x/k,y/k)=1，此时问题可转化为： x 属于 [1,b/k] ，y属于[1,d/k]，求有多少对（x,y）满足 GCD(x,y)=1，即有多少 x、y 是互质的

求有多少对（x,y）满足 GCD(x,y)=1 十分麻烦，可以换个思路，去求有多少对这样的（x,y）满足 GCD(x,y)=1 的倍数

F(d) 为有多少对（x,y）满足 GCD(x,y)=d 的倍数，f(d) 为有多少对（x,y）满足 GCD(x,y)=d

根据 F(d) 的定义可知：![](https://img-blog.csdn.net/20140417201441703)，![n=b / k,m=d / k](https://private.codecogs.com/gif.latex?n%3Db%20/%20k%2Cm%3Dd%20/%20k)

根据莫比乌斯反演的定义，要想用莫比乌斯反演去解决一个问题，最关键的就是设出 f(d) 和 g(d) 这两个方程，且这两个方程要有倍数关系或约数关系

因此，可得：f(1)=mu(1)*g(1) + mu(2)*g(2) + …，在所限定的区间内，d 最大为 min(m,n)

即：f(1)=mu(1)*g(1) + mu(2)*g(2) + … + mu(d)*g(d)，d=min(m,n)

另外，此题还应注意一下去重，即结果除以 2

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
#define E 1e-9
#define PI acos(-1.0)
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=7;
const int N=100000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int mu[N];
int prime[N];
bool bprime[N];
void getMu(int n){//线性筛求莫比乌斯函数

    mu[1]=1;//根据定义，μ(1)=1

    int cnt=0;
    memset(bprime,false,sizeof(bprime));
    for(int i=2;i<=n;i++){//求2~n的莫比乌斯函数
        if(!bprime[i]){
            prime[cnt++]=i;//存储质数
            mu[i]=-1;//i为质数时，μ(1)=-1
        }
        for(int j=0;j<cnt;j++){//枚举i之前的素数个数
            int k=i*prime[j];//素数的乘积
            if(k>n)//剪枝
                break;
            bprime[k]=true;//不是质数
            if(i%prime[j])//i不是primes[j]的整数倍时，i*primes[j]就不会包含相同质因子
                mu[k]=-mu[i];//mu[k]=mu[i]*mu[primes[j]]，因为primes[j]是质数，mu值为-1
            else{
                mu[k]=0;
                break;
            }
        }
    }
}
int main(){
    getMu(N);

    int t;
    int Case=1;
    scanf("%d",&t);
    while(t--){
        int a,b,c,d,k;
        scanf("%d%d%d%d%d",&a,&b,&c,&d,&k);

        printf("Case %d: ",Case++);
        if(k==0){
            printf("0\n");
            continue;
        }

        b/=k;
        d/=k;

        LL ans=0,res=0;
        int minn=min(b,d);
        for(int i=1;i<=minn;i++){
            ans+=(LL)mu[i]*(b/i)*(d/i);
            res+=(LL)mu[i]*(minn/i)*(minn/i);
        }

        printf("%lld\n",ans-res/2);
    }
    return 0;
}
```





