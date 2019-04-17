# Mad Counting（LightOJ-1148） - Alex_McAvoy的博客 - CSDN博客





2019年03月22日 19:28:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：11
个人分类：[LightOJ																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Mob was hijacked by the mayor of the Town "TruthTown". Mayor wants Mob to count the total population of the town. Now the naive approach to this problem will be counting people one by one. But as we all know Mob is a bit lazy, so he is finding some other approach so that the time will be minimized. Suddenly he found a poll result of that town where N people were asked "How many people in this town other than yourself support the same team as you in the FIFA world CUP 2010?" Now Mob wants to know if he can find the minimum possible population of the town from this statistics. Note that no people were asked the question more than once.

# Input

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case starts with an integer N (1 ≤ N ≤ 50). The next line will contain N integers denoting the replies (0 to 106) of the people.

# Output

For each case, print the case number and the minimum possible population of the town.

# **Sample Input**

**241 1 2 210**

# Sample Output

**Case 1:  5Case 2:  1**


题意：t 组数据，每组给出 n 个数，第 i 个数的含义是这个城市中除第 i 个人之外还有多少个人支持第 i 个人支持的队伍，问最小人口数

思路：

要使人尽量的少，那么可以让具有相同支持人数的人凑在一起

因此，问题就转化为统计具相同支持人数的人数，再对每一个队伍遍历，累加出除了被问问题的人之外的人数

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
const int N=100+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int a[N];
int main(){
    //freopen("G:\\Visual Studio\\C++\\Test\\test.txt","r",stdin);
    //freopen("G:\\Visual Studio\\C++\\Test\\test.txt","w",stdout);

    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);

        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        sort(a+1,a+1+n);

        int sum=0;
        for(int i=1;i<=n;i++){
            int num=1;
            for(int j=i+1;j<=n;j++){
                if(a[j]==a[i])
                    num++;
                else
                    break;
            }
            i+=num-1;
            if(num%(a[i]+1)!=0)
                sum+=a[i]+1-num%(a[i]+1);
        }

        printf("Case %d: %d\n",Case++,sum+n);
    }
    return 0;
}
```






