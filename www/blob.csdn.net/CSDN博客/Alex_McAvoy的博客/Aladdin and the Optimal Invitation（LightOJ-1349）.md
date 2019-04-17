# Aladdin and the Optimal Invitation（LightOJ-1349） - Alex_McAvoy的博客 - CSDN博客





2019年02月13日 23:24:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：18
个人分类：[LightOJ																[计算几何——二维几何基础](https://blog.csdn.net/u011815404/article/category/8630484)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Finally Aladdin reached home, with the great magical lamp. He was happier than ever. As he was a nice boy, he wanted to share the happiness with all people in the town. So, he wanted to invite all people in town in some place such that they can meet there easily. As Aladdin became really wealthy, so, number of people was not an issue. Here you are given a similar problem.

Assume that the town can be modeled as an m x n 2D grid. People live in the cells. Aladdin wants to select a cell such that all people can gather here with optimal overall cost. Here, cost for a person is the distance he has to travel to reach the selected cell. If a person lives in cell (x, y) and he wants to go to cell (p, q), then the cost is |x-p|+|y-q|. So, distance between (5, 2) and (1, 3) is |5-1|+|2-3| which is 5. And the overall cost is the summation of costs for all people.

So, you are given the information of the town and the people, your task to report a cell which should be selected by Aladdin as the gathering point and the overall cost should be as low as possible.

# Input

Input starts with an integer T (≤ 20), denoting the number of test cases.

Each case starts with a blank line. Next line contains three integers: m, n and q (1 ≤ m, n, q ≤ 50000), m and n denote the number of rows and columns of the grid respectively. Each of the next q lines contains three integers u v w (1 ≤ u ≤ m, 1 ≤ v ≤ n, 1 ≤ w ≤ 10000), meaning that there are w persons who live in cell (u, v). You can assume that there are no people in the cells which are not listed. You can also assume that each of the q lines contains a distinct cell.

# Output

For each case, print the case number and the row and column position of the cell where the people should be invited. There can be multiple solutions, any valid one will do.

# Examples

**Input**

2

5 1 1

2 1 10

5 5 4

1 1 1

2 2 1

4 4 1

5 5 1

**Output**

Case 1: 2 1

Case 2: 3 3


题意：t 组数据，每组给出一个 m*n 的平面以及 q 个位置，每个位置以 u、v、w 的形式给出，代表在坐标 (u,v) 处有 w 个人，求一个点在平面内使所有人都到这个点的曼哈顿距离之和最小

思路：要使所有人到一个点的曼哈顿距离最小，找行、列的中位数即可

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
#define N 100001
#define LL long long
const int MOD=1e9+7;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int row[N],col[N];
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int m,n,q;
        scanf("%d%d%d",&m,&n,&q);
        memset(row,0,sizeof(row));
        memset(col,0,sizeof(col));

        int sum=0;//总和
        while(q--){
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            row[u]+=w;
            col[v]+=w;
            sum+=w;
        }

        int mid=(sum+1)/2;//中位数
        int resRow,resCol;
        int numRow=0,numCol=0;
        for(int i=1;i<=m;i++){
            numRow+=row[i];
            if(numRow>=mid){
                resRow=i;
                break;
            }
        }
        for(int i=1;i<=n;i++){
            numCol+=col[i];
            if(numCol>=mid){
                resCol=i;
                break;
            }
        }

        printf("Case %d: %d %d\n",Case++,resRow,resCol);
    }
    return 0;
}
```






