# Chip Factory（HDU-5536） - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 16:04:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：72
个人分类：[HDU																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

John is a manager of a CPU chip factory, the factory produces lots of chips everyday. To manage large amounts of products, every processor has a serial number. More specifically, the factory produces nn chips today, the i-th chip produced this day has a serial number si. 

At the end of the day, he packages all the chips produced this day, and send it to wholesalers. More specially, he writes a checksum number on the package, this checksum is defined as below: 

![max_{i,j,k}(si+sj)](https://private.codecogs.com/gif.latex?max_%7Bi%2Cj%2Ck%7D%28si&plus;sj%29)  ⊕ ![sk](https://private.codecogs.com/gif.latex?sk)

which i,j,ki,j,k are three different integers between 1 and nn. And ⊕ is symbol of bitwise XOR. Can you help John calculate the checksum number of today?

# Input

The first line of input contains an integer TT indicating the total number of test cases. 

The first line of each test case is an integer nn, indicating the number of chips produced today. The next line has nn integers s1,s2,..,sn, separated with single space, indicating serial number of each chip. 

1≤T≤1000

3≤n≤1000

0≤si≤109

There are at most 1010 testcases with n>100

# Output

For each test case, please output an integer indicating the checksum number in a line.

# Sample Input

**231 2 33100 200 300**

# **Sample Output**

**6400**

————————————————————————————————————————————

题意：t 组数据，每组给出 n 个数 si，求 ![max_{i,j,k}(si+sj)](https://private.codecogs.com/gif.latex?max_%7Bi%2Cj%2Ck%7D%28si&plus;sj%29)  ⊕ ![sk](https://private.codecogs.com/gif.latex?sk)，其中 1<=i<=n

思路：01字典树的题，但暴搜也可以做，需要注意暴搜的技巧，不然会 TLE

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
#define MOD 1000003
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n;
int s[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(s,0,sizeof(s));

        scanf("%d",&n);
        for(int i=0;i<n;i++)
            scanf("%d",&s[i]);

        int maxx=-INF;
        for(int i=0;i<n;i++){
            for(int j=i+1;j<n;j++){
                for(int k=j+1;k<n;k++){
                    maxx=max(maxx,((s[i]+s[j])^s[k]));
                    maxx=max(maxx,((s[i]+s[k])^s[j]));
                    maxx=max(maxx,((s[j]+s[k])^s[i]));
                }
            }
        }

        printf("%d\n",maxx);
    }
    return 0;
}
```






