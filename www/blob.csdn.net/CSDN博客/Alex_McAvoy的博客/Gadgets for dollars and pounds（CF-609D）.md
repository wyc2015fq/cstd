# Gadgets for dollars and pounds（CF-609D） - Alex_McAvoy的博客 - CSDN博客





2019年01月15日 19:29:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：72
个人分类：[CodeForces																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Nura wants to buy k gadgets. She has only s burles for that. She can buy each gadget for dollars or for pounds. So each gadget is selling only for some type of currency. The type of currency and the cost in that currency are not changing.

Nura can buy gadgets for n days. For each day you know the exchange rates of dollar and pound, so you know the cost of conversion burles to dollars or to pounds.

Each day (from 1 to n) Nura can buy some gadgets by current exchange rate. Each day she can buy any gadgets she wants, but each gadget can be bought no more than once during n days.

Help Nura to find the minimum day index when she will have k gadgets. Nura always pays with burles, which are converted according to the exchange rate of the purchase day. Nura can't buy dollars or pounds, she always stores only burles. Gadgets are numbered with integers from 1 to m in order of their appearing in input.

# Input

First line contains four integers n, m, k, s (1 ≤ n ≤ 2·105, 1 ≤ k ≤ m ≤ 2·105, 1 ≤ s ≤ 109) — number of days, total number and required number of gadgets, number of burles Nura has.

Second line contains n integers ai (1 ≤ ai ≤ 106) — the cost of one dollar in burles on i-th day.

Third line contains n integers bi (1 ≤ bi ≤ 106) — the cost of one pound in burles on i-th day.

Each of the next m lines contains two integers ti, ci (1 ≤ ti ≤ 2, 1 ≤ ci ≤ 106) — type of the gadget and it's cost. For the gadgets of the first type cost is specified in dollars. For the gadgets of the second type cost is specified in pounds.

# Output

If Nura can't buy k gadgets print the only line with the number -1.

Otherwise the first line should contain integer d — the minimum day index, when Nura will have k gadgets. On each of the next k lines print two integers qi, di — the number of gadget and the day gadget should be bought. All values qi should be different, but the valuesdi can coincide (so Nura can buy several gadgets at one day). The days are numbered from 1 to n.

In case there are multiple possible solutions, print any of them.

# Examples

**Input**

5 4 2 2

1 2 3 2 1

3 2 1 2 3

1 1

2 1

1 2

2 2

**Output**

3

1 1

2 3

**Input**

4 3 2 200

69 70 71 72

104 105 106 107

1 1

2 2

1 2

**Output**

-1 

**Input**

4 3 1 1000000000

900000 910000 940000 990000

990000 999000 999900 999990

1 87654

2 76543

1 65432

**Output**

-1

————————————————————————————————————————————

题意：n 天 m 种商品，要求花费在某种货币的价格 s 元内用美元或英镑买 k 种商品，用美元或英镑购买时，商品的价格固定不变，首先给出 n、m、k、s，再给出 n 天这种货币兑换美元、英镑的每天的汇率，然后分别给出每种商品用美元(用 1 代表)或英镑(用 2 代表)的价格，如果可以购买的话输出能够购买完全时的天数，并且依次输出这 k 种商品的序号以及其在购买的天数，如果无法购买，则输出-1

思路：二分+贪心

先将所有的商品分为两类，美元与英镑，然后用贪心的思想枚举这两类在 s 元内能否购买的 k 种商品，若不能购买则直接输出 -1，若可以购买则需要用二分去枚举最小天数，然后再根据可购买的 k 种商品去枚举在哪天买的

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
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 500001
#define LL long long
using namespace std;
struct Node{
    int pos;//物品实际编号
    int cost;//物品的价值
    bool operator <(const Node &rhs) const {
        return cost<rhs.cost;
    }
}nodeA[N],nodeB[N];//存储某种物品时根据货币种类A、B来分类
int numNodeA,numNodeB;
int n,m,k,s;
LL a[N],b[N];//第i天的两种货币汇率
int minA[N],minB[N];//前i天两种货币汇率最小的那一天
LL sumA[N],sumB[N];//前i个物品需要多少钱
bool judge(int x){
    LL sum=0;
    for(int i=0;i<numNodeA&&i<=k;i++)
    {
        if(k<i+numNodeB)
        {
            sum=sumA[i]*(LL)a[minA[x]]+sumB[k-i]*(LL)b[minB[x]];//按汇率最少的时候算需要多少货币
            if(sum<=s)
                return false;
        }
    }
    return true;
}
int twoFind(){
    int left=1,right=n;
    while(left<=right){
        int mid=(left+right)>>1;
        if(judge(mid))
            left=mid+1;//全都不符合天数，需要向后
        else
            right=mid-1;//符合天数，需要向前找
    }

    return left;//left-1是最后一个不满足的天数，left天就是第一个满足最小的天数
}
int main()
{
    while(scanf("%d%d%d%d",&n,&m,&k,&s)!=EOF){
        memset(minA,0,sizeof(minA));
        memset(minB,0,sizeof(minB));
        a[0]=INF,b[0]=INF;

        for(int i=1;i<=n;i++){
            scanf("%lld",&a[i]);
            if(a[minA[i-1]]>a[i])//如果第i-1天的货币A种类的汇率大于第i天的
                minA[i]=i;//则前i天的A汇率最小的天数是第i天
            else
                minA[i]=minA[i-1];//前i天的A汇率最小的天数是第minA[i-1]的天数
        }
        for(int i=1;i<=n;i++){
            scanf("%lld",&b[i]);
            if(b[minB[i-1]]>b[i])//如果第i-1天的货币B种类的汇率大于第i天的
                minB[i]=i;//则前i天的B汇率最小的天数是第i天
            else
                minB[i]=minB[i-1];//前i天的B汇率最小的天数是第minB[i-1]的天数
        }

        numNodeA=1,numNodeB=1;
        for(int i=1;i<=m;i++){
            int type,cost;
            scanf("%d%d",&type,&cost);
            if(type==1){//当物品用货币A来购买时
                nodeA[numNodeA].pos=i;
                nodeA[numNodeA].cost=cost;
                numNodeA++;
            }
            else{//当物品用货币B来购买时
                nodeB[numNodeB].pos=i;
                nodeB[numNodeB].cost=cost;
                numNodeB++;
            }
        }

        //依照物品价值排序
        sort(nodeA+1,nodeA+numNodeA);
        sort(nodeB+1,nodeB+numNodeB);

        sumA[0]=0;
        for(int i=1;i<numNodeA;i++)//用货币A购买时，购买前i个物品所花费的钱数
            sumA[i]=sumA[i-1]+(LL)nodeA[i].cost;
        sumB[0]=0;
        for(int i=1;i<numNodeB;i++)//用货币B购买时，购买前i个物品所花费的钱数
            sumB[i]=sumB[i-1]+(LL)nodeB[i].cost;


        int minDay=twoFind();
        if(minDay>n)//所需天数大于n天
            printf("-1\n");
        else{
            printf("%d\n",minDay);
            LLsum=0;
            int res;
            for(int i=0;i<numNodeA&&i<=k;i++){
                if(k<i+numNodeB){
                    sum=sumA[i]*(LL)a[minA[minDay]]+sumB[k-i]*(LL)b[minB[minDay]];
                    if(sum<=s){
                        res=i;
                        break;
                    }
                }
            }

            for(int i=1;i<=res;i++)
                printf("%d %d\n",nodeA[i].pos,minA[minDay]);
            for(int i=1;i<=k-res;i++)
                printf("%d %d\n",nodeB[i].pos,minB[minDay]);
        }
    }
    return 0;
}
```






