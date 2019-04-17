# Just another Robbery（LightOJ-1079） - Alex_McAvoy的博客 - CSDN博客





2019年04月02日 22:20:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：16
个人分类：[LightOJ																[动态规划——概率 DP 与期望 DP](https://blog.csdn.net/u011815404/article/category/8820787)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

As Harry Potter series is over, Harry has no job. Since he wants to make quick money, (he wants everything quick!) so he decided to rob banks. He wants to make a calculated risk, and grab as much money as possible. But his friends - Hermione and Ron have decided upon a tolerable probability P of getting caught. They feel that he is safe enough if the banks he robs together give a probability less than P.

# **Input**

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case contains a real number P, the probability Harry needs to be below, and an integer N (0 < N ≤ 100), the number of banks he has plans for. Then follow N lines, where line j gives an integer Mj (0 < Mj ≤ 100) and a real number Pj . Bank j contains Mj millions, and the probability of getting caught from robbing it is Pj. A bank goes bankrupt if it is robbed, and you may assume that all probabilities are independent as the police have very low funds.

# Output

For each case, print the case number and the maximum number of millions he can expect to get while the probability of getting caught is less than P.

# Sample Input

**30.04 31 0.022 0.033 0.050.06 32 0.032 0.033 0.050.10 31 0.032 0.023 0.05**

# Sample Output

**Case 1:  2Case 2:  4Case 3:  6**


题意：t 组数据，每组给出一个危险率 p 与 n 个银行数，只有当危险率低于 p 的时候才会行动，现在给出没家银行的金钱数 mi 与危险率 pi，问最多能获得多少金钱

思路：

对于 n 个银行，一个个抢过去，抢到的钱随着银行数量的增加而增加或者不变，被捕的概率随着银行数量的增加而增加或不变

对于第 i 个银行，抢到的钱与被捕概率增加的情况，就是抢劫该银行，抢到的钱与被捕概率不变的情况，就是不抢劫该银行

危险率是 p，那么安全率就是 1-p，那么问题就转换为在金钱数最大的情况下求安全率最大为 1-p 的情况时的金钱，也即概率 DP下的 01 背包问题

设 f[i] 表示最大钱数为 i 的情况下安全的概率，将数据读入后，首先将所有的危险率转为安全率，然后统计最大钱数作为背包容量，再以安全率作为价值，进行 01 背包，最后找第一个大于等于总安全率 1-p 的状态 f[i]，输出 i 即可

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
int money[N];//每个银行的金钱
double pBank[N];//每个银行的安全率
double f[N];//f[i]为最大钱数为i的情况下的安全率
void oneZeroPack(double cost,int weight,int V){
    for(int v=V;v>=weight;v--)
        f[v]=max(f[v],f[v-weight]*cost);
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        double p;
        int n;
        scanf("%lf%d",&p,&n);
        p=1-p;//转为安全率

        int sum=0;
        for(int i=1;i<=n;i++){
            scanf("%d%lf",&money[i],&pBank[i]);
            pBank[i]=1.0-pBank[i];//转为安全率
            sum+=money[i];//统计总钱数
        }

        memset(f,0,sizeof(f));
        f[0]=1;//没有抢时安全率为1
        for(int i=1;i<=n;i++)
            oneZeroPack(pBank[i],money[i],sum);


        for(int i=sum;i>=0;i--){
            if(f[i]>=p){
                printf("Case %d: %d\n",Case++,i);
                break;
            }
        }
    }

    return 0;
}
```






