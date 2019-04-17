# Mysterious Bacteria（LightOJ-1220） - Alex_McAvoy的博客 - CSDN博客





2019年01月21日 22:15:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[LightOJ																[数论——唯一分解定理](https://blog.csdn.net/u011815404/article/category/8796922)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Dr. Mob has just discovered a Deathly Bacteria. He named it RC-01. RC-01 has a very strange reproduction system. RC-01 lives exactly x days. Now RC-01 produces exactly p new deadly Bacteria where x = b^p (where b, p are integers). More generally, x is a perfect p^th power. Given the lifetime x of a mother RC-01 you are to determine the maximum number of new RC-01 which can be produced by the mother RC-01.

# Input

Input starts with an integer T (≤ 50), denoting the number of test cases.

Each case starts with a line containing an integer x. You can assume that x will have magnitude at least 2 and be within the range of a 32 bit signed integer.

# Output

For each case, print the case number and the largest integer p such that x is a perfect pth power.

# **Sample Input**

**317107374182425**

# Sample Output

**Case 1: 1Case 2: 30Case 3: 2**


题意：给出一个数 x，求满足 x=p^k 的 k 值，其中 p、k 为整数

思路：根据整数唯一分解定理可以得出 x=p1^k1+p2^k2+...+pn^kn，由于 x=p^k 中的 p 是唯一的，因此可分解完所有的 ki 后，求 GCD(k1,k2,...,kn) 即为答案

例如：当 x=17 时，有 x=2^4+1，由于要让 x=p^k，因此应为 17=17^1，所以 k=GCD(4,1)

此外，需要注意的是，x 可能为负数，如果 x 为负数的话，x = p^k，k 必须是奇数，因此将 x 转换为整数求解后，如果解是偶数的话，必须一直除二转换为奇数

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
#define N 1000001
#define LL long long
using namespace std;
int prime[N],cnt;
bool bprime[N];
void makePrime(){
	memset(bprime,true,sizeof(bprime));
	bprime[0]=false;
	bprime[1]=false;

    for(int i=2;i<=N;i++){
        if(bprime[i]){
            prime[cnt++]=i;
            for(int j=i*2;j<=N;j+=i)
                bprime[j]=false;
        }
    }
}
int GCD(int a, int b){
    return b?GCD(b,a%b):a;
}

int main(){
    makePrime();
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL x;
        scanf("%lld",&x);

        bool flag=false;
        if(x<0){//负数的情况
            x=-x;
            flag=true;
        }

        int res=0;
        for(int i=0;(LL)prime[i]*prime[i]<=x&&i<cnt;i++){
            if(x%prime[i]==0){//枚举每一组的prime^k
                int k=0;
                while(x%prime[i]==0){
                    k++;
                    x/=prime[i];
                }
                //求p=GCD(k1,k2,...,kn)
                if(res==0)
                    res=k;
                else
                    res=GCD(res,k);
            }
    
        }

        if(x>1)
            res=1;

        if(flag){
            while(res%2==0)
                res/=2;
        }

        printf("Case %d: %d\n",Case++,res);
    }

	return 0;
}
```






