# Codeforces1009E——Intercity Travelling - westbrook1998的博客 - CSDN博客





2018年08月19日 19:39:00[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29








> 
Leha is planning his journey from Moscow to Saratov. He hates trains, so he has decided to get from one city to another by car. 

  The path from Moscow to Saratov can be represented as a straight line (well, it’s not that straight in reality, but in this problem we will consider it to be straight), and the distance between Moscow and Saratov is n km. Let’s say that Moscow is situated at the point with coordinate 0 km, and Saratov — at coordinate n km. 

  Driving for a long time may be really difficult. Formally, if Leha has already covered i kilometers since he stopped to have a rest, he considers the difficulty of covering (i+1)-th kilometer as ai+1. It is guaranteed that for every i∈[1,n−1] ai≤ai+1. The difficulty of the journey is denoted as the sum of difficulties of each kilometer in the journey. 

  Fortunately, there may be some rest sites between Moscow and Saratov. Every integer point from 1 to n−1 may contain a rest site. When Leha enters a rest site, he may have a rest, and the next kilometer will have difficulty a1, the kilometer after it — difficulty a2, and so on. 

  For example, if n=5 and there is a rest site in coordinate 2, the difficulty of journey will be 2a1+2a2+a3: the first kilometer will have difficulty a1, the second one — a2, then Leha will have a rest, and the third kilometer will have difficulty a1, the fourth — a2, and the last one — a3. Another example: if n=7 and there are rest sites in coordinates 1 and 5, the difficulty of Leha’s journey is 3a1+2a2+a3+a4. 

  Leha doesn’t know which integer points contain rest sites. So he has to consider every possible situation. Obviously, there are 2n−1 different distributions of rest sites (two distributions are different if there exists some point x such that it contains a rest site in exactly one of these distributions). Leha considers all these distributions to be equiprobable. He wants to calculate p — the expected value of difficulty of his journey. 

  Obviously, p⋅2n−1 is an integer number. You have to calculate it modulo 998244353. 

  Input 

  The first line contains one number n (1≤n≤106) — the distance from Moscow to Saratov. 

  The second line contains n integer numbers a1, a2, …, an (1≤a1≤a2≤⋯≤an≤106), where ai is the difficulty of i-th kilometer after Leha has rested. 

  Output 

  Print one number — p⋅2n−1, taken modulo 998244353. 

  Examples 

  Input 

  2 

  1 2 

  Output 

  5 

  Input 

  4 

  1 3 3 7 

  Output 

  60
这题题意是一段n长的路 有n-1个休息点  同时还给了a1…an 代表休息后第1到第n段的疲劳度 

最后要求所有休息的可能情况的疲劳度总和 

比如n=2 然后a1=1 a2=2 

就是有一个休息点 所有可能只有两种 休息或不休息 休息的话疲劳度1+1 不休息的话1+2 最后答案就是5 

但是因为n长的路有2^(n-1)的可能休息情况 枚举肯定不可能过 只能找公式或规律 

通过简单枚举我们会发现其实最后就是`系数*a1+系数*a2+...`这样的情况，我们就是要找到这个系数
a4 

a3                      1/4 

a2              1/2   1/4 

a1          1  1/2   1/2 

休息点   1    2    3 
比如上表，1表示从路的0到1，这一段无论如何都无法休息，而且疲劳度肯定是a1 概率为1 

到了2 这里表示的是经过1休息点，还没到达2休息点，这里1休息点有两种选择，休息或不休息，各1/2，若是休息，则a1的系数为1/2 否则就是a2的系数为1/2 来到3 也就是经过2休息点，这时候情况也是类似，可以选择休息，休息之后就是疲劳程度为a1 概率为1/2，如果不休息，对应的前面有可能是休息也有可能是不休息，如果前面是休息，这里就是a2的系数，如果前面不休息，这里就是a3的系数 

然后最后要把这些概率乘以2^(n-1) 情况的种数 再加起来 
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
const int MOD=998244353;
const int N=1e6+10;
const int INF=0x3f3f3f3f;
int a[N];
long long Pow(long long a,int n){
    long long ans=1;
    while(n){
        if(n%2==1){
            ans=(ans*a)%MOD;
        }
        n/=2;
        a=(a*a)%MOD;
    }
    return ans;
}
int main(void){
    int n;
    scanf("%d",&n);
    long long ans=0;
    long long t;
    long long xi;
    for(int i=1;i<=n;i++){
        scanf("%lld",&t);
        xi=(Pow(2,n-i)+(Pow(2,n-i-1)*(n-i)))%MOD;
        ans=(ans+(t*xi)%MOD)%MOD;
    }
    printf("%lld\n",ans);
    return 0;
}
```








