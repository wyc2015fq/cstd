# Washing Clothes（POJ-3211） - Alex_McAvoy的博客 - CSDN博客





2019年03月14日 18:42:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：101








> 
# Problem Description

Dearboy was so busy recently that now he has piles of clothes to wash. Luckily, he has a beautiful and hard-working girlfriend to help him. The clothes are in varieties of colors but each piece of them can be seen as of only one color. In order to prevent the clothes from getting dyed in mixed colors, Dearboy and his girlfriend have to finish washing all clothes of one color before going on to those of another color.

From experience Dearboy knows how long each piece of clothes takes one person to wash. Each piece will be washed by either Dearboy or his girlfriend but not both of them. The couple can wash two pieces simultaneously. What is the shortest possible time they need to finish the job?

# **Input**

The input contains several test cases. Each test case begins with a line of two positive integers M and N (M < 10, N < 100), which are the numbers of colors and of clothes. The next line contains M strings which are not longer than 10 characters and do not contain spaces, which the names of the colors. Then follow N lines describing the clothes. Each of these lines contains the time to wash some piece of the clothes (less than 1,000) and its color. Two zeroes follow the last test case.

# Output

For each test case output on a separate line the time the couple needs for washing.

# Sample Input

**3 4red blue yellow2 red3 blue4 blue6 red0 0**

# Sample Output

**10**


题意：n 种颜色 m 件衣服，给出洗每件衣服的时间，现在两个一起洗衣服，要求只有在同一颜色的衣服洗完后才能洗下一种颜色的衣服，问花费的最小时间

思路：

两人每次只能洗同一种衣服，那么就意味着其他颜色的衣服对当前的状态没有影响，因此利用分组背包的思想先将衣服按颜色进行分组

其次，由于每种颜色的衣服都有一个各自的时间，那么对于当前的颜色，可以求出一个人洗衣服时洗完所有衣服的总时间，而由于是两个人洗衣服，那么两个人各自用去总时间的一半，也即理想状态下的最短时间，这个时间也即是**背包中的容量**，

然后，洗每件衣服所需时间就是**物品的重量**，每件衣服所需的时间即为**物品的价值**，每种颜色的衣服件数即为**物品的数量**，做 01 背包求出最大价值就是一半时间内一个人洗衣服的用时 maxTime

由于在洗衣服时，最理想的时间是 **总时间/2**，但实际情况下，一定是**大于** 总时间/2 这个值的，由于在做 01背包是，设定的容量是 总时间/2，那么求出的 maxTime 一定是**小于** 总时间/2 的那个值，因此 总时间-maxTime 就是**大于 **总时间/2 的值，也即最小的花费时间

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
const int N=20000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
struct Node{
    int time[N];//每种衣服第i件所需时间
    string color;//每种衣服颜色
    int num;//每种衣服数量
    int times;//每种衣服洗完的总时间
}a[N];
int dp[N];
void ZeroOnePack(int V,int cost,int weight){
    for(int v=V;v>=weight;v--)
        dp[v]=max(dp[v],dp[v-weight]+cost);
}

int main(){
    int m,n;
    while(scanf("%d%d",&m,&n)!=EOF&&(n+m)){
        for(int i=1;i<=m;i++){
            cin>>a[i].color;
            a[i].num=0;
            a[i].times=0;
        }
        for(int i=1;i<=n;i++){
            int time;
            string color;

            cin>>time>>color;
            for(int j=1;j<=m;j++){//对于m种颜色
                if(a[j].color==color){
                    int num=a[j].num;//当前颜色衣服数量
                    a[j].time[num]=time;//当前衣服花费时间
                    a[j].times+=time;//记录洗衣总时间
                    a[j].num++;//衣服数量+1
                }
            }
        }

        int res=0;
        for(int i=1;i<=m;i++){//分组
            memset(dp,0,sizeof(dp));
            int halfTime=a[i].times/2;//时间的一半
            for(int j=0;j<a[i].num;j++)//每种颜色有num种
                ZeroOnePack(halfTime,a[i].time[j],a[i].time[j]);//洗每件衣服的时间即是价值又是重量
            
            res+=a[i].times-dp[halfTime];
        }
        printf("%d\n",res);
    }
    return 0;
}
```





