# Bi-shoe and Phi-shoe（LightOJ-1370） - Alex_McAvoy的博客 - CSDN博客





2019年03月18日 20:37:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：24
个人分类：[LightOJ																[数论——素性测试](https://blog.csdn.net/u011815404/article/category/8796920)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Bamboo Pole-vault is a massively popular sport in Xzhiland. And Master Phi-shoe is a very popular coach for his success. He needs some bamboos for his students, so he asked his assistant Bi-Shoe to go to the market and buy them. Plenty of Bamboos of all possible integer lengths (yes!) are available in the market. According to Xzhila tradition,

Score of a bamboo = Φ (bamboo's length)

(Xzhilans are really fond of number theory). For your information, Φ (n) = numbers less than n which are relatively prime (having no common divisor other than 1) to n. So, score of a bamboo of length 9 is 6 as 1, 2, 4, 5, 7, 8 are relatively prime to 9.

The assistant Bi-shoe has to buy one bamboo for each student. As a twist, each pole-vault student of Phi-shoe has a lucky number. Bi-shoe wants to buy bamboos such that each of them gets a bamboo with a score greater than or equal to his/her lucky number. Bi-shoe wants to minimize the total amount of money spent for buying the bamboos. One unit of bamboo costs 1 Xukha. Help him.

# Input

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case starts with a line containing an integer n (1 ≤ n ≤ 10000) denoting the number of students of Phi-shoe. The next line contains n space separated integers denoting the lucky numbers for the students. Each lucky number will lie in the range [1, 106].

# Output

For each case, print the case number and the minimum possible money spent for buying the bamboos. See the samples for details.

# **Sample Input**

**351 2 3 4 5610 11 12 13 14 1521 1**

# Sample Output

**Case 1: 22 XukhaCase 2: 88 XukhaCase 3: 4 Xukha**


题意：t 组数据，每组给出 n 个数，对于这些数，第 i 个数代表的是某个数的欧拉函数的值，现在要求出这些数的和，且要这些数的和最小，问最小和是多少

思路：

由于一些数的欧拉函数值相同，因此才会求最小和，根据欧拉函数的性质，当 x 是素数时，有 Φ(x)=x-1

假设给出了一个数 x，那么如果 x+1 是素数，那么最小的值就是 x+1，如果不是素数，那么就向后找一个最近的素数，因此可以打一个素数表，然后判断就行了

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
bool bprime[N];
void makePrime(){//筛素数
    memset(bprime,false,sizeof(bprime));
    bprime[1]=true;

    for(int i=2;i<=N;i++){
        if(!bprime[i]){
            for(int j=2*i;j<=N;j+=i)
                bprime[j]=true;
        }
    }
}
int main(){
    makePrime();

    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);

        LL res=0;
        for(int i=1;i<=n;i++){
            int x;
            scanf("%d",&x);
            for(int j=x+1;;j++){
                if(!bprime[j]){//判断
                    res+=j;//累计结果
                    break;
                }
            }
        }

        printf("Case %d: %lld Xukha\n",Case++,res);
    }
    return 0;
}
```






