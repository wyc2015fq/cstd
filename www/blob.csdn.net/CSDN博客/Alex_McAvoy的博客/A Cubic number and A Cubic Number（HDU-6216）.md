# A Cubic number and A Cubic Number（HDU-6216） - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 18:53:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[HDU																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

A cubic number is the result of using a whole number in a multiplication three times. For example, 3×3×3=27 so 27 is a cubic number. The first few cubic numbers are 1,8,27,64 and 125. Given an prime number pp. Check that if pp is a difference of two cubic numbers.

# Input

The first of input contains an integer T (1≤T≤100) which is the total number of test cases. 

For each test case, a line contains a prime number p (2≤p≤10^12).

# Output

For each test case, output 'YES' if given p is a difference of two cubic numbers, or 'NO' if not.

# **Sample Input**

**102357111317192329**

# Sample Output

**NONONOYESNONONOYESNONO**


题意：t 组数据，每组给出一个素数 p，问这个数是不是两个数的立方的差

思路：

首先，对于**任意的两个数** x、y，他们的立方数的差 ![p=x^3-y^3=(x-y)(x^2+x*y+y^2)](https://private.codecogs.com/gif.latex?p%3Dx%5E3-y%5E3%3D%28x-y%29%28x%5E2&plus;x*y&plus;y%5E2%29)

由于输入的 p 是素数，那么 ![(x-y)=1](https://private.codecogs.com/gif.latex?%28x-y%29%3D1)，![(x^2+x*y+y^2)=p](https://private.codecogs.com/gif.latex?%28x%5E2&plus;x*y&plus;y%5E2%29%3Dp)

由于 ![(x-y)=1](https://private.codecogs.com/gif.latex?%28x-y%29%3D1)，因此只需要打表记录**相邻两个数**的立方差，然后对于每个输入检查是否在数组中即可

100 0000 的立方差差不多在 13 位左右，而 p 的范围最大到 1E12，因此打 100w 的表足够

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
#define INF 0x3f3f3f3f
#define N 1000010
#define LL long long
const int MOD=10;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
LL a[N];
int main(){
    int cnt=0;
    for(LL i=1;i<=1000000;i++)
        a[cnt++]=i*i+i*(i+1)+(i+1)*(i+1);

    int t;
    scanf("%d",&t);
    while(t--){
        LL p;
        scanf("%lld",&p);

        bool flag=false;
        for(int i=0;i<cnt;i++){
            if(a[i]==p){
                flag=true;
                break;
            }
        }

        if(flag)
            printf("YES\n");
        else
            printf("NO\n");

    }
    return 0;
}
```






