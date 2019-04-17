# Again Array Queries（LightOJ-1100） - Alex_McAvoy的博客 - CSDN博客





2019年01月21日 22:23:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48
个人分类：[LightOJ																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

Given an array with n integers, and you are given two indices i and j (i ≠ j) in the array. You have to find two integers in the range whose difference is minimum. You have to print this value. The array is indexed from 0 to n-1.

# Input

Input starts with an integer T (≤ 5), denoting the number of test cases.

Each case contains two integers n (2 ≤ n ≤ 105) and q (1 ≤ q ≤ 10000). The next line contains n space separated integers which form the array. These integers range in [1, 1000].

Each of the next q lines contains two integers i and j (0 ≤ i < j < n).

# Output

For each test case, print the case number in a line. Then for each query, print the desired result.

# **Sample Input**

**25 310 2 3 12 70 20 42 42 11 20 1**

# Sample Output

**Case 1:114Case 2:1**


————————————————————————————————————————————

题意：t 组数据，每组 n 个数，给出这 n 个数和 m 组查询，每次查询区间 [i, j] 中所有数两两之间的最小差值

思路：所有数的数据范围在 1~1000，可利用桶排的思想，将每个区间 [i,j] 中的数加入桶中，若某个数出现的次数大于 2，则这个区间内的最小差值一定为 0，若某个数出现次数为 1，则与前一个出现次数为 1 的数计算差值，不断去更新最小值

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
#define N 100001
#define LL long long
using namespace std;
int a[N];
int bucket[N];
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        int n,q;
        scanf("%d%d",&n,&q);
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);

        printf("Case %d:\n",Case++);
        for(int i=0;i<q;i++){
            int x,y;
            scanf("%d%d",&x,&y);

            memset(bucket,0,sizeof(bucket));
            for(int i=x;i<=y;i++)
                bucket[a[i]]++;

            int pre=-INF;
            int minn=INF;

            for(int i=1;i<=1000;i++){
                if(bucket[i]==0)
                    continue;
                else if(bucket[i]>=2)
                    minn=0;
                else
                    minn=min(minn,i-pre);
                pre=i;
            }
            printf("%d\n",minn);
        }

    }
    return 0;
}
```




