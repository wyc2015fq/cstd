# LCP Array（HDU-5635） - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 20:08:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：108
个人分类：[HDU																[字符串处理——模拟与暴力](https://blog.csdn.net/u011815404/article/category/8650811)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Peter has a string s=s1s2...sn, let suffi=sisi+1...sn...sn be the suffix start with ii-th character of ss. Peter knows the lcp (longest common prefix) of each two adjacent suffixes which denotes as ai=lcp(suffi,suffi+1)(1≤i<n). Given the lcp array, Peter wants to know how many strings containing lowercase English letters only will satisfy the lcp array. The answer may be too large, just print it modulo 109+7.

# Input

There are multiple test cases. The first line of input contains an integer T indicating the number of test cases. For each test case: 

The first line contains an integer n (2≤n≤105) -- the length of the string. The second line contains n−1 integers: a1,a2,...,an−1 (0≤ai≤n). 

The sum of values of nn in all test cases doesn't exceed 106.

# Output

For each test case output one integer denoting the answer. The answer must be printed modulo 109+7

# **Sample Input**

**330 043 2 131 2**

# Sample Output

**16250260**


题意：假设有一个字符串 S，用 a[i] 表示 Si、Si+1...Sn 与 Si+1...Sn 的最大前缀和，给出 t 组数据，每组给出一个数 n 代表字符串的长度，再给出 n-1 个数字，代表字符串的前缀和 a[i]，问 S 有多少种可能

思路：

对于如下字符串，假如 a[1] = 4，那么 s1 = s2 = s3 = s4，从而有 a[2]=3，a[3]=2，a[4]=1，a[5]=0

s1 s2 s3 s4 s5   

s2 s3 s4 s5      

s3 s4 s5

s4 s5

s5

可以看出，对于 n 个数来说，假如 a[i] 不等于 0，可以说明这个字符串从第 i 位开始一直到第 i+a[i] 位都是同一个字符，而且 a[i+1] 一定等于 a[i]-1，结合前缀的性质，容易得出 a[i]=0 时，字符串的第 i 位与第 i+1 位字符不同

因此，对于数组 a[i] 来说，0 是一个断点，故而统计 0 的个数后再进行字母的判断即可，由于第一位的选择有 26 个，剩余的有25 种，故而最后答案实际就是 26*25^m，其中，m 是 0 的个数

记得模 10E9+7

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
int n;
int a[N];
int main(){
    int t;
    scanf("%d",&t);

    while(t--){
        scanf("%d%",&n);
        for(int i=0;i<n-1;i++)
            scanf("%d",&a[i]);
        a[n-1]=0;

        LL res=26;
        for(int i=0;i<n-1;i++){
            if(!a[i]){
                res=(LL)(res*25)%MOD;
            }
            else if(a[i]!=a[i+1]+1){
                res=0;
                break;
            }
        }
        printf("%d\n",res);
    }
    return 0;
}
```






