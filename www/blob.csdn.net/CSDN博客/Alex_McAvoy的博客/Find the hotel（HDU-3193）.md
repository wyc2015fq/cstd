# Find the hotel（HDU-3193） - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 18:49:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：85
个人分类：[HDU																[数据结构——RMQ](https://blog.csdn.net/u011815404/article/category/8671578)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Summer again! Flynn is ready for another tour around. Since the tour would take three or more days, it is important to find a hotel that meets for a reasonable price and gets as near as possible! 

  But there are so many of them! Flynn gets tired to look for any. It’s your time now! Given the <p i, d i> for a hotel h i, where p i stands for the price and d i is the distance from the destination of this tour, you are going to find those hotels, that either with a lower price or lower distance. Consider hotel h 1, if there is a hotel h i, with both lower price and lower distance, we would discard h1. To be more specific, you are going to find those hotels, where no other has both lower price and distance than it. And the comparison is strict.

# Input

There are some cases. Process to the end of file. 

Each case begin with N (1 <= N <= 10000), the number of the hotel. 

The next N line gives the (p i, d i) for the i-th hotel. 

The number will be non-negative and less than 10000.

# Output

First, output the number of the hotel you find, and from the next line, print them like the input( two numbers in one line). You should order them ascending first by price and break the same price by distance.

# **Sample Input**

**315 1010 158 9**

# Sample Output

**18 9**


题意：每个房子有两个属性：价格、距离，现给出 n 个房子的价格和距离，定义目标房子：对于当前的房子 i ，若没有其他的任何一个房子的价格和距离同时小于当前房子 i 的价格和距离，要求先输出目标房子的个数，然后按照价格从小到大的顺序输出所有目标房子的价格和距离，若价格相同，则按照距离从小到大的顺序输出

思路：RMQ 模版题

对于一个房子，只有当他的价格、距离属性同时小于另一个房子时，才能判断另一个房子不是目标房子

首先用 sort 将所有房子按照价格排序，然后用 RMQ 求出距离区间的最小值，去枚举每一个点 i，找出 i 这个房子前面价格更小的房子中，距离最小的房子 j，若这个房子 j 的距离比 i 的距离要远，那么说明 i 就符合没有其他旅馆的价格、距离都比 i 小

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
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
const int MOD=10;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
struct Node{
    int price;
    int dis;
    bool operator <(const Node &rhs)const{
        if(price==rhs.price)
            return dis<rhs.dis;
        return price<rhs.price;
    }
}a[N],res[N];
int dmin[N][30];
void initMin(int n){
    for(int i=1;i<=n;i++)
        dmin[i][0]=a[i].dis;
    for(int j=1;(1<<j)<=n;j++)
        for(int i=1;i+(1<<j)-1<=n;i++)
            dmin[i][j]=min(dmin[i][j-1] , dmin[i+(1<<(j-1))][j-1]);
}
int getMin(int l,int r){
    if(l>r)
        return INF;

    //int k = (int)(log10(r-l+1)/log10(2));
    int k=0;
    while((1<<(k+1))<=r-l+1)
       k++;

    return min(dmin[l][k] , dmin[r-(1<<k)+1][k]);
}

int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        for(int i=1;i<=n;i++)
            scanf("%d%d",&a[i].price,&a[i].dis);
        sort(a+1,a+n+1);
        initMin(n);//价格排好序，求区间中距离的最小值

        a[0].price=INF;
        a[0].dis=INF;

        int j=1,cnt=1;
        for(int i=1;i<=n;i++){
            if(a[i].price!=a[j].price)
                j=i;
            int minn=getMin(1,j-1);
            if(minn>=a[i].dis){
                res[cnt].price=a[i].price;
                res[cnt].dis=a[i].dis;
                cnt++;
            }
        }

        printf("%d\n",cnt-1);
        for(int i=1;i<=cnt-1;i++)
            printf("%d %d\n",res[i].price,res[i].dis);
    }
    return 0;
}
```






