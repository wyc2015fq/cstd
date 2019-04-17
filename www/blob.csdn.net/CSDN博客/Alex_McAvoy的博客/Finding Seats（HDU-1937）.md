# Finding Seats（HDU-1937） - Alex_McAvoy的博客 - CSDN博客





2019年02月17日 20:56:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38








> 
# Problem Description

A group of K friends is going to see a movie. However, they are too late to get good tickets, so they are looking for a good way to sit all nearby. Since they are all science students, they decided to come up with an optimization problem instead of going on with informal arguments to decide which tickets to buy. 

The movie theater has R rows of C seats each, and they can see a map with the currently available seats marked. They decided that seating close to each other is all that matters, even if that means seating in the front row where the screen is so big it’s impossible to see it all at once. In order to have a formal criteria, they thought they would buy seats in order to minimize the extension of their group. 

The extension is defined as the area of the smallest rectangle with sides parallel to the seats that contains all bought seats. The area of a rectangle is the number of seats contained in it. 

They’ve taken out a laptop and pointed at you to help them find those desired seats. 

# Input

Each test case will consist on several lines. The first line will contain three positive integers R, C and K as explained above (1 <= R,C <= 300, 1 <= K <= R × C). The next R lines will contain exactly C characters each. The j-th character of the i-th line will be ‘X’ if the j-th seat on the i-th row is taken or ‘.’ if it is available. There will always be at least K available seats in total. 

Input is terminated with R = C = K = 0. 

# Output

For each test case, output a single line containing the minimum extension the group can have.

# **Sample Input**

**3 5 5...XX.X.XXXX...5 6 6..X.X..XXX...XX.X..XXX.X.XX.XX0 0 0**

# Sample Output

**69**


题意：每组数据给出一个 n*m 的矩阵，其中 . 代表空座，X 代表座位有人，要求最小的矩阵面积，使得矩阵中空座的个数大于等于 k 个

思路：尺取法

如果普通的进行枚举，最多会达到 O(N^5)，一定超时，可用尺取法来优化

用一个 sum[i][j] 来记录 (1,1) 到 (i,j) 所围成的矩阵中 . 的个数，两层循环枚举第 i 列到第 j 列，最内层循环使用尺取法枚举行即可

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
#define N 1001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
char str[N];
int sum[N][N];
int G[N][N];
int main(){
    int n,m,k;
    while(scanf("%d%d%d",&n,&m,&k)!=EOF&&(m+n+k)){
        for(int i=1;i<=n;i++){
            scanf("%s",str+1);
            for(int j=1;j<=m;j++){
                if(str[j]=='.')
                    G[i][j]=1;
                else
                    G[i][j]=0;

                sum[i][j]=sum[i][j-1]+G[i][j];//尺取法预处理
            }
        }

        for(int i=2;i<=n;i++)
            for(int j=1;j<=m;j++)
                sum[i][j]+=sum[i-1][j];
        
        int res=INF;
        for(int i=1;i<=m;i++){//第i列
            for(int j=i;j<=m;j++){//第j列
                int l=1,r=1;
                while(r<=n){//尺取法枚举行
                    while(sum[r][j]-sum[r][i-1]-sum[l-1][j]+sum[l-1][i-1]>=k){
                        res=min(res,(r-l+1)*(j-i+1));//更新最优解
                        l++;
                    }
                    r++;
                }
            }
        }

        printf("%d\n",res);

    }
    return 0;
}
```





