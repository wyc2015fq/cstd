# Shifts（CF-230C） - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 20:05:08[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：53
个人分类：[CodeForces																[基础算法——分治](https://blog.csdn.net/u011815404/article/category/8118604)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given a table consisting of n rows and m columns. Each cell of the table contains a number, 0 or 1. In one move we can choose some row of the table and cyclically shift its values either one cell to the left, or one cell to the right.

To cyclically shift a table row one cell to the right means to move the value of each cell, except for the last one, to the right neighboring cell, and to move the value of the last cell to the first cell. A cyclical shift of a row to the left is performed similarly, but in the other direction. For example, if we cyclically shift a row "00110" one cell to the right, we get a row "00011", but if we shift a row "00110" one cell to the left, we get a row "01100".

Determine the minimum number of moves needed to make some table column consist only of numbers 1.

# Input

The first line contains two space-separated integers: n (1 ≤ n ≤ 100) — the number of rows in the table and m (1 ≤ m ≤ 104) — the number of columns in the table. Then n lines follow, each of them contains m characters "0" or "1": the j-th character of the i-th line describes the contents of the cell in the i-th row and in the j-th column of the table.

It is guaranteed that the description of the table contains no other characters besides "0" and "1".

# Output

Print a single number: the minimum number of moves needed to get only numbers 1 in some column of the table. If this is impossible, print -1.

# Examples

**Input**

3 6

101010

000100

100000

**Output**

3

**Input**

2 3

111

000

**Output**

-1

# Note

In the first sample one way to achieve the goal with the least number of moves is as follows: cyclically shift the second row to the right once, then shift the third row to the left twice. Then the table column before the last one will contain only 1s.

In the second sample one can't shift the rows to get a column containing only 1s.


题意：给出 n 行 m 列的一个 0 1矩阵，每一行可以进行滚动，每滚动一次步数记录一次，现在每一行都可以进行滚动，问使得某一列全是 1 时的最少滚动次数，若无法使得某一列全是 1，那么输出 -1

思路：

首先，如果有一行全是 0，那么直接输出 -1即可

然后可以去枚举每行两个 1 之间的位置到两个 1 的最小距离，然后将每列上的结果相加，取每列中的最小值

要注意的是，由于每一行是一个循环，因此最后一个 1 也是第一个 1，第一个 1 也是最后一个 1

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
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
char G[N][N];
int pos[N];//记录每一行中1的位置
int res[N];
int main(){
    int n,m;
    cin>>n>>m;

    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            cin>>G[i][j];

    for(int i=1;i<=n;i++){
        int num=0;//每一行中1的个数
        memset(pos,0,sizeof(pos));

        for(int j=1;j<=m;j++)
            if(G[i][j]=='1')
                pos[++num]=j;

        if(num==0){//某一行全是0
            cout<<-1<<endl;
            return 0;
        }

        pos[++num]=pos[1]+m;//最后一个1也是第一个1
        for(int j=1;j<num;j++){
            int mid=(pos[j]+pos[j+1])/2;//两个1中间的点
            for(int k=pos[j];k<=pos[j+1];k++){//枚举两个1中间的点
                if(k<=mid)
                    res[k%m]+=k-pos[j];
                else
                    res[k%m]+=pos[j+1]-k;
            }
        }
    }

    int minn=INF;
    for(int i=0;i<m;i++)
        minn=min(minn,res[i]);
    cout<<minn<<endl;

    return 0;
}
```






