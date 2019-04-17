# Turn the Rectangles（CF-1008B） - Alex_McAvoy的博客 - CSDN博客





2018年08月23日 19:57:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：66








# Problem Description

There are n rectangles in a row. You can either turn each rectangle by 90 degrees or leave it as it is. If you turn a rectangle, its width will be height, and its height will be width. Notice that you can turn any number of rectangles, you also can turn all or none of them. You can not change the order of the rectangles.

Find out if there is a way to make the rectangles go in order of non-ascending height. In other words, after all the turns, a height of every rectangle has to be not greater than the height of the previous rectangle (if it is such).

# Input

The first line contains a single integer nn (1≤n≤1051≤n≤105) — the number of rectangles.

Each of the next nn lines contains two integers wi and hi (1≤wi,hi≤109) — the width and the height of the i-th rectangle.

# Output

Print "YES" (without quotes) if there is a way to make the rectangles go in order of non-ascending height, otherwise print "NO".

You can print each letter in any case (upper or lower).

# Examples

**Input**

3

3 4

4 6

3 5

**Output**

YES

**Input**

2

3 4

5 5

**Output**

NO

————————————————————————————————————————————

题意：给出 n 个长方形，问能否通过任意多次旋转，在不改变他们的相对顺序的情况下让他们的高度形成一个不上升序列

思路：贪心，每次使长方形的 w 较大即可

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
#define N 1001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
bool vis[27];
int x[100005],y[100005];
int main()
{
    int n;
    int flag=1;
    cin>>n;
    for(int i=1;i<=n;i++)
    {

        cin>>x[i]>>y[i];
        if(x[i]>y[i]) swap(x[i],y[i]);
    }
    int t=y[1];
    for(int i=2;i<=n;i++)
    {
       if(y[i]<=t)
            t=y[i];
       else if(x[i]<=t)
          t=x[i];
       else
       {
           flag=0;
           break;
       }
    }
    if(flag==1)
        cout<<"YES"<<endl;
    else
        cout<<"NO"<<endl;
	return 0;
}
```





