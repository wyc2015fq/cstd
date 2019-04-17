# Street Numbers（POJ-1320） - Alex_McAvoy的博客 - CSDN博客





2019年03月21日 19:16:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：116
个人分类：[POJ																[数论——方程](https://blog.csdn.net/u011815404/article/category/8796916)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

A computer programmer lives in a street with houses numbered consecutively (from 1) down one side of the street. Every evening she walks her dog by leaving her house and randomly turning left or right and walking to the end of the street and back. One night she adds up the street numbers of the houses she passes (excluding her own). The next time she walks the other way she repeats this and finds, to her astonishment, that the two sums are the same. Although this is determined in part by her house number and in part by the number of houses in the street, she nevertheless feels that this is a desirable property for her house to have and decides that all her subsequent houses should exhibit it. 

Write a program to find pairs of numbers that satisfy this condition. To start your list the first two pairs are: (house number, last number): 

         6         8

        35        49

# Input

There is no input for this program.

# Output

Output will consist of 10 lines each containing a pair of numbers, in increasing order with the last number, each printed right justified in a field of width 10 (as shown above).

# Sample Input

There is no input for this program.

# Sample **Output**

         6         8

        35        49


题意：有 m 个编号从 1 到 m 的房子，问是否存在 1+2+3+...+ (N-1)=(N+1)+(N+2)+...+(M)，求出前 10 个 n、m

思路：

将左右两端的等差数列求和，有：（2M+1)^2 - 8N^2 = 1

满足佩尔方程的形式：x=2m+1，y=n

可以得到最小的一组解为 x=3，y=1，然后套模版求前 10 个佩尔方程的解即可

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
const int N=100000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int main(){
    int x0=3,y0=1;
    for(int i=1;i<=10;i++){
        int x=x0*3+8*y0;
        int y=x0+y0*3;
        printf("%10d%10d\n",y,(x-1)/2);
        x0=x,y0=y;
    }
    return 0;
}
```






