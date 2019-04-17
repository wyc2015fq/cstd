# Death Note（CF-1016A） - Alex_McAvoy的博客 - CSDN博客





2018年08月13日 16:41:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63








# Problem Description

You received a notebook which is called Death Note. This notebook has infinite number of pages. A rule is written on the last page (huh) of this notebook. It says: "You have to write names in this notebook during nn consecutive days. During the ii-th day you have to write exactly aiai names.". You got scared (of course you got scared, who wouldn't get scared if he just receive a notebook which is named Death Note with a some strange rule written in it?).

Of course, you decided to follow this rule. When you calmed down, you came up with a strategy how you will write names in the notebook. You have calculated that each page of the notebook can contain exactly mm names. You will start writing names from the first page. You will write names on the current page as long as the limit on the number of names on this page is not exceeded. When the current page is over, you turn the page. Note that you always turn the page when it ends, it doesn't matter if it is the last day or not. If after some day the current page still can hold at least one name, during the next day you will continue writing the names from the current page.

Now you are interested in the following question: how many times will you turn the page during each day? You are interested in the number of pages you will turn each day from 1 to n.

# Input

The first line of the input contains two integers n, m (1≤n≤2⋅10^5, 1≤m≤10^9) — the number of days you will write names in the notebook and the number of names which can be written on each page of the notebook.

The second line contains nn integers a1,a2,…,an (1≤ai≤10^9), where aiai means the number of names you will write in the notebook during the i-th day.

# Output

Print exactly nn integers t1,t2,…,tn, where ti is the number of times you will turn the page during the i-th day.

# Examples

**Input**

3 5

3 7 9

**Output**

0 2 1

**Input**

4 20

10 9 19 2

**Output**

0 0 1 1 

**Input**

1 100

99

**Output**

0

————————————————————————————————————————————

题意：给出 n 个数，从第一个数开始，如果第 i 个数是 m 的倍数，就输出几倍并清零，否则一直累加

思路：模拟水题

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
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=n;i++)
        cin>>a[i];

    int sum=0;
    for(int i=1;i<=n;i++)
    {
        sum+=a[i];
        cout<<sum/m<<" ";
        sum-=sum/m*m;
    }
    cout<<endl;

    return 0;
}
```





