# Trailing Zeroes（LightOJ-1138） - Alex_McAvoy的博客 - CSDN博客





2019年03月18日 20:15:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：65








> 
# Problem Description

You task is to find minimal natural number N, so that N! contains exactly Q zeroes on the trail in decimal notation. As you know N! = 1*2*...*N. For example, 5! = 120, 120 contains one zero on the trail.

# Input

Input starts with an integer T (≤ 10000), denoting the number of test cases.

Each case contains an integer Q (1 ≤ Q ≤ 108) in a line.

# Output

For each case, print the case number and N. If no solution is found then print 'impossible'.

# **Sample Input**

**3125**

# Sample Output

**Case 1: 5**

**Case 2: 10**

**Case 3: impossible**


题意：对于某个数的阶乘 n!，其有后缀 0，现在给出一个数字 x，代表后缀 0 的个数，求最小的满足后缀 0 的阶乘数 n

思路：不难发现，要找 n！中 0 的个数，只需要找从 1 到 n 中有几个 5 即可，因此可以使用二分法枚举这个数，判断其是否满足 1 到 n 种 5 的个数 

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
LL calculate(LL x){//计算n中有多少个5
    LL res=0;
    while(x){
        res+=x/5;
        x/=5;
    }
    return res;
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL q;
        scanf("%lld",&q);
        
        LL left=1,right=1000000000;
        LL res=0;
        while(left<=right){
            LL mid=(left+right)/2;
            LL temp=calculate(mid);
            if(temp<q)//小于
                left=mid+1;
            else if(temp>q)//大于
                right=mid-1;
            else{//等于
                res=mid;//记录答案
                right=mid-1;
            }
        }

        if(res==0)
            printf("Case %d: impossible\n",Case++);
        else
            printf("Case %d: %lld\n",Case++,res);
    }
    return 0;
}
```





