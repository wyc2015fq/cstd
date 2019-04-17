# Find The Multiple（POJ-1426） - Alex_McAvoy的博客 - CSDN博客





2018年11月14日 20:45:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[POJ																[搜索——广度优先搜索 BFS](https://blog.csdn.net/u011815404/article/category/8115771)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Given a positive integer n, write a program to find out a nonzero multiple m of n whose decimal representation contains only the digits 0 and 1. You may assume that n is not greater than 200 and there is a corresponding m containing no more than 100 decimal digits.

# **Input**

The input file may contain multiple test cases. Each line contains a value of n (1 <= n <= 200). A line containing a zero terminates the input.

# Output

For each value of n in the input print a line containing the corresponding value of m. The decimal representation of m must not contain more than 100 digits. If there are multiple solutions for a given value of n, any one of them is acceptable.

# Sample Input

**26190**

# Sample Output

**10100100100100100100111111111111111111**

——————————————————————————————————————————————————————

题意：给出一个数 n ，要求输出一个二进制串，要求该串能整除 n

思路：bfs 搜索，不断取队首元素，判断能否整除，若能直接输出，若不能则让 当前元素*10 与 当前元素*10+1 存入队列，不断判断，直到有解为止。

# **Source Program**

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
int n;
int main(){
    while(scanf("%d",&n)!=EOF&&n){
        queue<LL> Q;
        Q.push(1);
        while(!Q.empty()){
            LL temp=Q.front();
            Q.pop();
            if(temp%n==0){
                printf("%lld\n",temp);
                break;
            }

            Q.push(temp*10);
            Q.push(temp*10+1);
        }
    }
    return 0;
}
```






