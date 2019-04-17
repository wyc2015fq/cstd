# 4 Values whose Sum is 0（POJ-2785） - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 19:59:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：186








> 
# Problem Description

The SUM problem can be formulated as follows: given four lists A, B, C, D of integer values, compute how many quadruplet (a, b, c, d ) ∈ A x B x C x D are such that a + b + c + d = 0 . In the following, we assume that all lists have the same size n .

# Input

The first line of the input file contains the size of the lists n (this value can be as large as 4000). We then have n lines containing four integer values (with absolute value as large as 2 28 ) that belong respectively to A, B, C and D .

# Output

For each input file, your program has to write the number quadruplets whose sum is zero.

# Examples

**Input**

6

-45 22 42 -16

-41 -27 56 30

-36 53 -37 77

-36 30 -75 -46

26 -38 -10 62

-32 -54 -6 45

**Output**

5

**Hint**

Sample Explanation: Indeed, the sum of the five following quadruplets is zero: (-45, -27, 42, 30), (26, 30, -10, -46), (-32, 22, 56, -46),(-32, 30, -75, 77), (-32, -54, 56, 30).


题意：给出 n 行，每行有 4 个数，每一列看做一个组，现在在每个组中选出一个数，问有多少种组合使得选出的 4 个数和为 0

思路：分治法

由于计算 4 个数的和并不好计算，因此可以使用分治的思想，将 4 组数分为两组，然后每组再分别计算和，最后对两组合进行排序，让正数与负数相加判断是否为 0 即可

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
int a[N],b[N],c[N],d[N];
int sumBe[N*N],sumLa[N*N];
int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
        cin>>a[i]>>b[i]>>c[i]>>d[i];

    //分组求和
    int cnt=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sumBe[cnt]=a[i]+b[j];
            sumLa[cnt]=c[i]+d[j];
            cnt++;
        }
    }

    //对两组和进行排序
    sort(sumBe,sumBe+cnt);
    sort(sumLa,sumLa+cnt);

    int res=0;
    int right=cnt-1;
    for(int left=0;left<cnt;left++){
        //和太大，调整右边界
        while(right>=0&&(sumBe[left]+sumLa[right])>0)
            right--;

        if(right<0)
            break;

        //一个left可能对应多组解
        int newRight=right;
        while(newRight>=0&&(sumBe[left]+sumLa[newRight])==0){
            res++;
            newRight--;
        }
    }
    cout<<res<<endl;

    return 0;
}
```





