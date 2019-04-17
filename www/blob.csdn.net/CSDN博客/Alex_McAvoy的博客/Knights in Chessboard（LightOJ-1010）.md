# Knights in Chessboard（LightOJ-1010） - Alex_McAvoy的博客 - CSDN博客





2019年03月22日 17:29:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：147
个人分类：[LightOJ																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Given an m x n chessboard where you want to place chess knights. You have to find the number of maximum knights that can be placed in the chessboard such that no two knights attack each other.

Those who are not familiar with chess knights, note that a chess knight can attack 8 positions in the board as shown in the picture below.

![](https://img-blog.csdnimg.cn/20190322163953621.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

# Input

Input starts with an integer T (≤ 41000), denoting the number of test cases.

Each case contains two integers m, n (1 ≤ m, n ≤ 200). Here m and n corresponds to the number of rows and the number of columns of the board respectively.

# Output

For each case, print the case number and maximum number of knights that can be placed in the board considering the above restrictions.

# **Sample Input**

**38 83 74 10**

# Sample Output

**Case 1: 32Case 2: 11Case 3: 20**


题意：t 组数据，每组给出一个 m*n 大小的棋盘，现要在图中放国际象棋的马，其攻击范围如上图，问最多能放多少个

思路：

当 n、m 均大于 2 时，讨论奇偶性： 
- 若 n、m 中有一个是偶数，则答案是：n*m/2
- 若 n、 m 都是奇数，那么在同一行的一半放 m/2+1 个，另一半放 m/2 个，则答案是：(m/2+1)*(n/2+1)+(n/2)*(m/2)

当 n、m 均小于等于 2 时，讨论特例： 
- 若 n 为 1 时，则答案是 m
- 若 m 为 1 时，则答案是 n
- 若 m、n 中有一个为 2，能够多分出 2*2 个格子，则答案是 4
- 其他的情况，设有 x 个格子，则有：(x/2+x(x/2+x % 2)∗42)∗4 

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
int main(){
    //freopen("G:\\Visual Studio\\C++\\Test\\test.txt","r",stdin);
    //freopen("G:\\Visual Studio\\C++\\Test\\test.txt","w",stdout);

    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);

        int res=0;
        if(n>=3&&m>=3){
            if(n%2==0||m%2==0)//n、m中有一个是偶数
                res=n*m/2;
            else//若n、 m都是奇数
                res=(m/2+1)*(n/2+1)+(n/2)*(m/2);
        }
        else{//特殊情况
            if(n==1)
                res=m;
            else if(m==1)
                res=n;
            else{
                if(m==2)
                    swap(n,m);
                    
                if(m<=3)
                    res=4;
                else{
                    int x=m/2;
                    res=(x/2+x%2)*4;
                    if(m%2&&x%2==0)
                        res+=2;
                }
            }
        }
        printf("Case %d: %d\n",Case++,res);
    }
    return 0;
}
```






