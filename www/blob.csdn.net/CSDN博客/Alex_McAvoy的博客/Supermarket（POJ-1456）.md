# Supermarket（POJ-1456） - Alex_McAvoy的博客 - CSDN博客





2019年02月20日 18:11:38[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[POJ																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

A supermarket has a set Prod of products on sale. It earns a profit px for each product x∈Prod sold by a deadline dx that is measured as an integral number of time units starting from the moment the sale begins. Each product takes precisely one unit of time for being sold. A selling schedule is an ordered subset of products Sell ≤ Prod such that the selling of each product x∈Sell, according to the ordering of Sell, completes before the deadline dx or just when dx expires. The profit of the selling schedule is Profit(Sell)=Σ x∈Sellpx. An optimal selling schedule is a schedule with a maximum profit. 

For example, consider the products Prod={a,b,c,d} with (pa,da)=(50,2), (pb,db)=(10,1), (pc,dc)=(20,2), and (pd,dd)=(30,1). The possible selling schedules are listed in table 1. For instance, the schedule Sell={d,a} shows that the selling of product d starts at time 0 and ends at time 1, while the selling of product a starts at time 1 and ends at time 2. Each of these products is sold by its deadline. Sell is the optimal schedule and its profit is 80. 

![](https://img-blog.csdnimg.cn/20190220181340815.jpg)

Write a program that reads sets of products from an input text file and computes the profit of an optimal selling schedule for each set of products. 

# Input

A set of products starts with an integer 0 <= n <= 10000, which is the number of products in the set, and continues with n pairs pi di of integers, 1 <= pi <= 10000 and 1 <= di <= 10000, that designate the profit and the selling deadline of the i-th product. White spaces can occur freely in input. Input data terminate with an end of file and are guaranteed correct.

# Output

For each set of products, the program prints on the standard output the profit of an optimal selling schedule for the set. Each result is printed from the beginning of a separate line.

# **Sample Input**

**4  50 2  10 1   20 2   30 1**

**7  20 1   2 1   10 3  100 2   8 2   5 20  50 10**

# Sample Output

**80185**


题意：有 n 个产品要卖，每个产品都有一个从开始卖时算起的截止时间 di，只有在这个截止时间前卖出且活动利润 pi，每卖一个产品要用 1 个单位的时间，问最大利润

思路：贪心

由于每次卖出的产品要占用 1 个时间单位，因此可以以单位时间来决定每个时间卖什么产品，这样来保证每个单位时间卖出的产品都是利润最大的

由于将时间从小到大枚举的话十分麻烦，因此可以从最后一个截止时间开始向前枚举，只要将截止时间大于当前时间段的产品放入优先队列，然后不断取出最大的利润即可

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
#define N 50001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Node{
    int d;
    int p;
    bool operator <(const Node &rhs)const{
        return d>rhs.d;
    }
}a[N];
int main(){
    int n;
    while(scanf("%d",&n)!=EOF){
        int maxx=-INF;//记录最大时间
        for(int i=1;i<=n;i++){
            scanf("%d%d",&a[i].p,&a[i].d);
            maxx=max(maxx,a[i].d);
        }
        sort(a+1,a+n+1);

        int res=0,pos=1;
        priority_queue<int,vector<int>,less<int> > Q;
        for(int time=maxx;time>=1;time--){//倒序枚举时间
            while(pos<=n&&a[pos].d>=time)//满足条件的进队
                Q.push(a[pos++].p);

            if(!Q.empty()){//记录最大利润
                int temp=Q.top();
                Q.pop();
                res+=temp;
            }
        }
        printf("%d\n",res);
    }
    return 0;
}
```






