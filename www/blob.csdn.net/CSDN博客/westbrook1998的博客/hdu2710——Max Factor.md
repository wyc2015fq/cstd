# hdu2710——Max Factor - westbrook1998的博客 - CSDN博客





2018年05月25日 21:50:52[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：102








> 
To improve the organization of his farm, Farmer John labels each of his N (1 <= N <= 5,000) cows with a distinct serial number in the range 1..20,000. Unfortunately, he is unaware that the cows interpret some serial numbers as better than others. In particular, a cow whose serial number has the highest prime factor enjoys the highest social standing among all the other cows.  

  (Recall that a prime number is just a number that has no divisors except for 1 and itself. The number 7 is prime while the number 6, being divisible by 2 and 3, is not).  

  Given a set of N (1 <= N <= 5,000) serial numbers in the range 1..20,000, determine the one that has the largest prime factor.  

  Input 

  * Line 1: A single integer, N  

  * Lines 2..N+1: The serial numbers to be tested, one per line 

  Output 

  * Line 1: The integer with the largest prime factor. If there are more than one, output the one that appears earliest in the input file. 

  Sample Input 

  4 

  36 

  38 

  40 

  42 

  Sample Output 

  38
有毒，一道水题我WA了12次![这里写图片描述](https://img-blog.csdn.net/20180525215003184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlc3Ricm9vazE5OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后发现居然是多组测试样例，妈呀有毒
代码：

```cpp
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int MAXN=20050;
int p[MAXN];
int getMaxPrimeFactor(int x){
    int i;
    int m=1;
    for(i=2;i*i<=x;i++){
        if(x%i==0){
            if(i>m){
                m=i;
            }
            while(x%i==0){
                x/=i;
            }
        }
    }
    if(x>1){
        if(x>m){
            m=x;
        }
    }
    return m;
}
int main(void){
    /*
    freopen("data.out","w",stdout);
    for(int i=1;i<=20000;i++){
        printf("%d\n",getMaxPrimeFactor(i));
    }
    */
    int n;
    while(~scanf("%d",&n)){
        int a,mf=-1,tmp,res=0;
        for(int i=0;i<n;i++){
            scanf("%d",&a);
            tmp=getMaxPrimeFactor(a);
            if(tmp>mf){
                mf=tmp;
                res=a;
            }
        }
        printf("%d\n",res);
    }
    return 0;   
}
```






