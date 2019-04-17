# LCM Walk（HDU-5584） - Alex_McAvoy的博客 - CSDN博客





2019年01月17日 16:12:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43
个人分类：[HDU																[数论——LCD 与 LCM](https://blog.csdn.net/u011815404/article/category/8796915)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

A frog has just learned some number theory, and can't wait to show his ability to his girlfriend. 

Now the frog is sitting on a grid map of infinite rows and columns. Rows are numbered 1,2,⋯ from the bottom, so are the columns. At first the frog is sitting at grid (sx,sy), and begins his journey. 

To show his girlfriend his talents in math, he uses a special way of jump. If currently the frog is at the grid (x,y), first of all, he will find the minimum z that can be divided by both x and y, and jump exactly z steps to the up, or to the right. So the next possible grid will be (x+z,y), or (x,y+z). 

After a finite number of steps (perhaps zero), he finally finishes at grid (ex,ey). However, he is too tired and he forgets the position of his starting grid! It will be too stupid to check each grid one by one, so please tell the frog the number of possible starting grids that can reach (ex,ey)!

# Input

First line contains an integer T, which indicates the number of test cases. 

Every test case contains two integers exex and eyey, which is the destination grid. 

⋅⋅ 1≤T≤1000

⋅⋅ 1≤ex,ey≤109

# Output

For every test case, you should output " Case #x: y", where xx indicates the case number and counts from 1 and y is the number of possible starting grids.

# Examples

**Input**

3

6 10

6 8

2 8

**Output**

Case #1: 1

Case #2: 2

Case #3: 3


题意：从 (x,y) 点出发，可以向上或向右走，每次走的距离为 LCM(x,y)，即移动后的坐标为 (x+LCM(x,y)，y) 或 (x,y+LCM(x,y))，移动若干次后停止在 (ex,ey) 点，问有多少个点可以移动到点 (ex,ey)

思路：一道逆推题

首先设 GCD(x,y)=k，x=km1，y=km2，则 LCM(x,y)=km1m2，从而可得到新的坐标：(km1m2+km1,km2) 或 (km1,km1m2+km2)，再对新的坐标点求 GCD，以 (km1m2+km1,km2) 为例，可得 GCD( (km1m2+km1,km2) )=GCD( km1(m2+1),km2 )=k，由此可知新得到的点与原点具有相同的 GCD，据此可进行逆推，此外在当前点 (x,y) 中，x、y 大的那个，一定是新得到的点

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
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 500001
#define LL long long
using namespace std;
int GCD(int x,int y){
    if(y==0)
        return x;
    else
        return GCD(y,x%y);
}
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        int x,y;
        scanf("%d%d",&x,&y);

        if(x>y)
            swap(x,y);
        int k=GCD(x,y);
        int cnt=1;

        /*
              km1m2+km2=y 
           -> xm2+km2=y
           -> (x+k)m2=y
        */
        while(y%(x+k)==0){
            cnt++;
            
            /*
                  km1m2+km2=y
               -> (x+k)m2=y
               -> m2=y/(x+k)
               -> newY=km2=k*(y/(x+k))
            */
            y=k*(y/(x+k));

            if(x>y)
                swap(x,y);
            k=GCD(x,y);
        }

        printf("Case #%d: %d\n",Case++,cnt);
    }
    return 0;
}
```






