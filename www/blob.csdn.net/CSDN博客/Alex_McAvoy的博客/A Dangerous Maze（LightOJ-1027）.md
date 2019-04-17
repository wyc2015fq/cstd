# A Dangerous Maze（LightOJ-1027） - Alex_McAvoy的博客 - CSDN博客





2019年04月02日 14:39:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：10
个人分类：[概率与期望——数学期望																[LightOJ](https://blog.csdn.net/u011815404/article/category/8037630)](https://blog.csdn.net/u011815404/article/category/8820534)








> 
# Problem Description

You are in a maze; seeing n doors in front of you in beginning. You can choose any door you like. The probability for choosing a door is equal for all doors.

If you choose the ith door, it can either take you back to the same position where you begun in xi minutes, or can take you out of the maze after xi minutes. If you come back to the same position, you can't remember anything. So, every time you come to the beginning position, you have no past experience.

Now you want to find the expected time to get out of the maze.

# **Input**

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case contains a blank line and an integer n (1 ≤ n ≤ 100) denoting the number of doors. The next line contains n space separated integers. If the ith integer (xi) is positive, you can assume that the ith door will take you out of maze after xi minutes. If it's negative, then the ith door will take you back to the beginning position after abs(xi) minutes. You can safely assume that 1 ≤ abs(xi) ≤ 10000.

# Output

For each case, print the case number and the expected time to get out of the maze. If it's impossible to get out of the maze, print 'inf'. Print the result in p/q format. Where p is the numerator of the result and q is the denominator of the result and they are relatively prime. See the samples for details.

# Sample Input

**7**

**3**

**11**

**2-10 -3**

**33 -6 -9**

# Sample Output

**Case 1: 1/1Case 2: infCase 3: 18/1**


题意：在一个迷宫中，现在有 n 个门，每个门要么将人传送出迷宫，要么传送到原来的位置并将记忆重置，使得不知道之前走的哪个门，现在给出每个门的传送时间，若 t 为正数，说明该门花费 t 时间将人传送出迷宫，若 t 为负数，说明该门花费 t 时间将人传送到原位置，已知每个门选择的概率是相同的，问出迷宫所花费时间的期望，若不能出迷宫，则输出 inf

思路：

每个门只有两种情况：
- 将人传送出去：花费 t 时间，期望为 ![\frac{1}{n}*t](https://private.codecogs.com/gif.latex?%5Cfrac%7B1%7D%7Bn%7D*t)
- 将人传送到原位置：花费 t 时间，期望为 ![\frac{1}{n}*(t+E)](https://private.codecogs.com/gif.latex?%5Cfrac%7B1%7D%7Bn%7D*%28t&plus;E%29)

那么，设出去的门个数为 d1，回去的门个数为 d2，所有出去的门的时间值总和为 s1，所有回去的门个数为 s2

出迷宫的期望为：![E=\frac{1}{n}*s_1+\frac{1}{n}*(s_2+d_2*E)](https://private.codecogs.com/gif.latex?E%3D%5Cfrac%7B1%7D%7Bn%7D*s_1&plus;%5Cfrac%7B1%7D%7Bn%7D*%28s_2&plus;d_2*E%29)

化简得：![E=\frac{s_1+s_2}{n-d_2}](https://private.codecogs.com/gif.latex?E%3D%5Cfrac%7Bs_1&plus;s_2%7D%7Bn-d_2%7D)，显然，当所有门均为传送回的门时，无法走出迷宫

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
int GCD(int a,int b){
    return !b?a:GCD(b,a%b);
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);

        int s1=0,s2=0,d2=0;
        for(int i=1;i<=n;i++){
            int x;
            scanf("%d",&x);
            if(x>0)
                s1+=x;
            else{
                s2+=abs(x);
                d2++;
            }
        }

        int up=s1+s2;
        int down=n-d2;
        int gcd=GCD(up,down);

        if(down==0)
            printf("Case %d: inf\n",Case++);
        else
            printf("Case %d: %d/%d\n",Case++,up/gcd,down/gcd);

    }

    return 0;
}
```






