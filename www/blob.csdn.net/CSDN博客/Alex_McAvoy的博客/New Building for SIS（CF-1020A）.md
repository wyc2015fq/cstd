# New Building for SIS（CF-1020A） - Alex_McAvoy的博客 - CSDN博客





2018年08月13日 16:54:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：49








# Problem Description

You are looking at the floor plan of the Summer Informatics School's new building. You were tasked with SIS logistics, so you really care about travel time between different locations: it is important to know how long it would take to get from the lecture room to the canteen, or from the gym to the server room.

The building consists of n towers, h floors each, where the towers are labeled from 1 to n, the floors are labeled from 1 to h. There is a passage between any two adjacent towers (two towers i and i + 1 for all i: 1 ≤ i ≤ n - 1) on every floor x, where a ≤ x ≤ b. It takes exactly one minute to walk between any two adjacent floors of a tower, as well as between any two adjacent towers, provided that there is a passage on that floor. It is not permitted to leave the building.

![](https://img-blog.csdn.net/20180813165243687?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

The picture illustrates the first example.

You have given k pairs of locations (ta, fa), (tb, fb): floor fa of tower ta and floor fb of tower tb. For each pair you need to determine the minimum walking time between these locations.Input

# Input

The first line of the input contains following integers:

n: the number of towers in the building (1 ≤ n ≤ 108),

h: the number of floors in each tower (1 ≤ h ≤ 108),

a and b: the lowest and highest floor where it's possible to move between adjacent towers (1 ≤ a ≤ b ≤ h),

k: total number of queries (1 ≤ k ≤ 104).

Next k lines contain description of the queries. Each description consists of four integers ta, fa, tb, fb (1 ≤ ta, tb ≤ n, 1 ≤ fa, fb ≤ h). This corresponds to a query to find the minimum travel time between fa-th floor of the ta-th tower and fb-th floor of the tb-th tower.

# Output

For each query print a single integer: the minimum walking time between the locations in minutes.

# Examples

**Input**

3 6 2 3 3

1 2 1 3

1 4 3 4

1 2 2 3

**Output**

1

4

2

————————————————————————————————————————————

题意：给出长度为 n 的字符串1与长度为 m 的字符串2，再给出 q 组 l，r，求在串1区间 [ l , r ] 中有多少个串2

思路：模拟水题，分成四种情况无脑模拟即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int main()
{
    int n,h,a,b,k;
    cin>>n>>h>>a>>b>>k;

    while(k--)
    {
        int ta,fa,tb,fb;
        cin>>ta>>fa>>tb>>fb;

        LL sum=0;
        if(ta==tb)//同一楼移动
            sum+=abs(fa-fb);
        else
        {
            sum+=abs(ta-tb);//楼数差
            if(fa>=a&&fa<=b)//初始位置在两桥之间
                sum+=abs(fa-fb);
            else if(abs(fa-a)>abs(fa-b))//初始位置在最低通道下方
                sum=sum+abs(fa-b)+abs(fb-b);
            else//初始位置在最高通道上方
                sum=sum+abs(fa-a)+abs(fb-a);
        }
        cout<<sum<<endl;
    }

    return 0;
}
```





