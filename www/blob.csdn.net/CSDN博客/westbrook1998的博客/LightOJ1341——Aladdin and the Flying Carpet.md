# LightOJ1341——Aladdin and the Flying Carpet - westbrook1998的博客 - CSDN博客





2018年08月20日 16:18:03[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34








> 
It’s said that Aladdin had to solve seven mysteries before getting the Magical Lamp which summons a powerful Genie. Here we are concerned about the first mystery.  

  Aladdin was about to enter to a magical cave, led by the evil sorcerer who disguised himself as Aladdin’s uncle, found a strange magical flying carpet at the entrance. There were some strange creatures guarding the entrance of the cave. Aladdin could run, but he knew that there was a high chance of getting caught. So, he decided to use the magical flying carpet. The carpet was rectangular shaped, but not square shaped. Aladdin took the carpet and with the help of it he passed the entrance.  

  Now you are given the area of the carpet and the length of the minimum possible side of the carpet, your task is to find how many types of carpets are possible. For example, the area of the carpet 12, and the minimum possible side of the carpet is 2, then there can be two types of carpets and their sides are: {2, 6} and {3, 4}.  

  Input  

  Input starts with an integer T (≤ 4000), denoting the number of test cases.  

  Each case starts with a line containing two integers: a b (1 ≤ b ≤ a ≤ 1012) where a denotes the area of the carpet and b denotes the minimum possible side of the carpet.  

  Output  

  For each case, print the case number and the number of possible carpets.  

  Sample Input  

  2  

  10 2  

  12 2  

  Sample Output  

  Case 1: 1  

  Case 2: 2
这题是要将一个数a分解为因数，且因数不能小于b，求有多少对  

用到的是唯一分解定理+分解质因数  

首先是将素数打表，然后分解计算每个素数的幂，然后利用  

s=(1+a1)*(1+a2)*…*(1+an) 计算得到，然后注意除以2
代码：

```cpp
/算术基本定理
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1000050;
bool prime[N];
int p[N];
int k;
void get_prime(){
    k=0;
    memset(prime,true,sizeof(prime));
    for(int i=2;i<N;i++){
        if(prime[i]){
            p[k++]=i;
            for(int j=2*i;j<N;j+=i){
                prime[j]=false;
            }
        }
    }
}
long long get_factor(long long n){
    long long i;
    long long ans=1;
    for(i=0;i<k && p[i]*p[i]<=n;i++){
        int cnt=0;
        if(n%p[i]==0){
            while(n%p[i]==0){
                cnt++;
                n/=p[i];
            }
        }
        ans=ans*(1+cnt);
    }
    if(n>1){
        ans*=2;
    }
    return ans;
}
int main(void){
    //printf("%lld\n",get_factor(12));
    get_prime();
    int t;
    long long a,b;
    scanf("%d",&t);
    int c=1;
    while(t--){
        scanf("%lld%lld",&a,&b);
        if(b*b>=a){
            printf("Case %d: %d\n",c++,0);
            continue;
        }
        long long ans=get_factor(a);
        for(int i=1;i<b;i++){
            if(a%i==0){
                ans-=2;
            }
        }
        printf("Case %d: %lld\n",c++,ans/2);
    }
}
```






