# Helping Cicada（LightOJ-1117） - Alex_McAvoy的博客 - CSDN博客





2019年03月26日 15:46:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30
个人分类：[LightOJ																[—————组合数学—————](https://blog.csdn.net/u011815404/article/category/7900800)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Cicada is an insect with large transparent eyes and well-veined wings similar to the "jar flies". The insects are thought to have evolved 1.8 million years ago during the Pleistocene epoch. There are about 2,500 species of cicada around the world which live in temperate tropical climates.

These are all sucking insects, which pierce plants with their pointy mouthparts and suck out the juices. But there are some predators (like birds, the Cicada Killer Wasp) that attack cicadas. Each of the predators has a periodic cycle of attacking Cicadas. For example, birds attack them every three years; wasps attack them every 2 years. So, if Cicadas come in the 12th year, then birds or wasps can attack them. If they come out in the 7th year then no one will attack them.

So, at first they will choose a number N which represents possible life-time. Then there will be an integer M indicating the total number of predators. The next M integers represent the life-cycle of each predator. The numbers in the range from 1 to N which are not divisible by any of those M life-cycles numbers will be considered for cicada's safe-emerge year. And you want to help them.

# **Input**

Input starts with an integer T (≤ 125), denoting the number of test cases.

Each case contains two integers N (1 ≤ N < 2^31) and M (1 ≤ M ≤ 15). The next line contains M positive integers (fits into 32 bit signed integer) denoting the life cycles of the predators.

# Output

For each test case, print the case number and the number of safe-emerge days for cicada.

# Sample Input

**215 32 3 510 42 4 5 7**

# Sample Output

**Case 1: 4Case 2: 3**


题意：t 组数据，每组给出一个数 n 和 m 个数，问从这 m 个数中，1-n 里不能被这 m 个数中任意一个整除的数有多少个

思路：

对于任意一个数 a[i] 来说，我们能知道在 1-n 中有 n/a[i] 个数是 a[i] 的倍数，但这样将 m 个数扫一遍一定会用重复的数，因此需要用到容斥原理

对于任意两个数来说，就要减去他们最小公倍数能组成的数，一共是 2^m 种情况，由于 m 很小，因此可以压缩成 1~2^m 种状态，对于每种状态，依次判断当前状态由多少种数组成

根据容斥原理：AUBUC=A+B+C-A∩B-A∩C-B∩C-A∩B∩C（奇加偶减）

有：sum=从 m 中选 1 个数得到的倍数的个数-从 m 中选 2 个数得到的倍数的个数 + 从 m 中选 3 个数得到的倍数的个数 - 从 m 中选 4 个数得到的倍数的个数……

那么最后的答案就是 n-sum

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
const int MOD=10056;
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
LL GCD(LL a,LL b){
    return !b?a:GCD(b,a%b);
}
LL LCM(LL a,LL b){
    return a/GCD(a,b)*b;
}
LL a[N];
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;

    while(t--){
        LL n;
        int m;
    	scanf("%lld%d",&n,&m);
	    for(int i=0;i<m;i++)
			scanf("%lld",&a[i]);

        LL sum=0;
        for(int i=0;i<(1<<m);i++){//2^m种状态
            LL temp=1;
            LL cnt=0;
            for(int j=0;j<m;j++){
                if(i&(1<<j)){//从m中选出j个数
                    temp=LCM(temp,a[j]);
                    cnt++;
                }
            }
            if(cnt!=0){
                if(cnt&1)//奇加
                    sum+=n/temp;
                else//偶减
                    sum-=n/temp;
            }
        }

        printf("Case %d: %lld\n",Case++,n-sum);
    }
    return 0;
}
```






