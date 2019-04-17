# Problem b（BZOJ-2301/HAOI-2011） - Alex_McAvoy的博客 - CSDN博客





2019年04月03日 19:51:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36








> 
# Problem Description

对于给出的 n 个询问，每次求有多少个数对 (x,y) ，满足 a ≤ x ≤ b ， c ≤ y ≤ d ，且 gcd(x,y) = k ， gcd(x,y) 函数为 x 和 y 的最大公约数。

# **Input**

第一行一个整数n，接下来n行每行五个整数，分别表示a、b、c、d、k

# Output

共n行，每行一个整数表示满足要求的数对(x,y)的个数

# Sample Input

**22 5 1 5 11 5 1 5 2**

# Sample Output

**143**


思路：

与[GCD（HDU-1695）](https://blog.csdn.net/u011815404/article/details/81610973) 相似，但 x、y 不是从 1 开始，因此需要使用容斥原理

即：res=cal(b,d)-cal(a-1,d)-cal(b,c-1)+cal(a-1,b-1)

此外，由于是分块计算，还需要预处理莫比乌斯函数的前缀和

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
LL mu[N];
LL prime[N];
bool bprime[N];
LL sum[N];
void getMu(int n){//线性筛求莫比乌斯函数

    mu[1]=1;//根据定义，μ(1)=1

    int cnt=0;
    memset(bprime,false,sizeof(bprime));
    for(int i=2;i<n;i++){//求2~n的莫比乌斯函数
        if(!bprime[i]){
            prime[cnt++]=i;//存储质数
            mu[i]=-1;//i为质数时，μ(1)=-1
        }
        for(int j=0;j<cnt;j++){//枚举i之前的素数个数
            int k=i*prime[j];//素数的乘积
            if(k>n)//剪枝
                break;
            bprime[k]=true;//标记合数
            if(i%prime[j])//i不是primes[j]的整数倍时，i*primes[j]就不会包含相同质因子
                mu[k]=-mu[i];//mu[k]=mu[i]*mu[primes[j]]，因为primes[j]是质数，mu值为-1
            else{
                mu[k]=0;
                break;
            }
        }
    }
    //构造前缀和
    for(int i=1;i<n;i++)
        sum[i]=sum[i-1]+mu[i];
}
LL cal(LL n,LL m,LL k){//计算a在[1,n]，b在[1,m]中gcd(a,b)=1的个数
    n/=k;
    m/=k;
    if(n>m)
        swap(n,m);
    if(n==0)
        return 0;

    LL res=0;
    LL next;
    for(LL i=1;i<=n;i=next){
        LL next1=n/(n/i);
        LL next2=m/(m/i);
        next=min(next1,next2);
        res+=(sum[next]-sum[i-1])*(m/i)*(n/i);
        next++;
    }
    return res;
}
int main(){
    getMu(N);

    int t;
    scanf("%d",&t);
    while(t--){
        LL a,b,c,d,k;
        scanf("%lld%lld%lld%lld%lld",&a,&b,&c,&d,&k);
        LL res=cal(b,d,k)-cal(a-1,d,k)-cal(b,c-1,k)+cal(a-1,c-1,k);
        printf("%lld\n",res);
    }
    return 0;
}
```





