# Songs Compression（CF-1015C） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 08:43:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：82
个人分类：[基础算法——贪心																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7628972)








# Problem Description

Ivan has nn songs on his phone. The size of the ii-th song is ai bytes. Ivan also has a flash drive which can hold at most m bytes in total. Initially, his flash drive is empty.

Ivan wants to copy all n songs to the flash drive. He can compress the songs. If he compresses the i-th song, the size of the i-th song reduces from aiai to bi bytes (bi<ai).

Ivan can compress any subset of the songs (possibly empty) and copy all the songs to his flash drive if the sum of their sizes is at most m. He can compress any subset of the songs (not necessarily contiguous).

Ivan wants to find the minimum number of songs he needs to compress in such a way that all his songs fit on the drive (i.e. the sum of their sizes is less than or equal to m).

If it is impossible to copy all the songs (even if Ivan compresses all the songs), print "-1". Otherwise print the minimum number of songs Ivan needs to compress.

# Input

The first line of the input contains two integers nn and mm (1≤n≤10^5,1≤m≤10^9) — the number of the songs on Ivan's phone and the capacity of Ivan's flash drive.

The next nn lines contain two integers each: the ii-th line contains two integers aiai and bibi (1≤ai,bi≤10^9, ai>bi) — the initial size of the ii-th song and the size of the ii-th song after compression.

# Output

If it is impossible to compress a subset of the songs in such a way that all songs fit on the flash drive, print "-1". Otherwise print the minimum number of the songs to compress.

# Examples

**Input**

4 21

10 8

7 4

3 1

5 4

**Output**

2

**Input**

4 16

10 8

7 4

3 1

5 4

**Output**

-1

# Note

In the first example Ivan can compress the first and the third songs so after these moves the sum of sizes will be equal to 8+7+1+5=21≤21. Also Ivan can compress the first and the second songs, then the sum of sizes will be equal 8+4+3+5=20≤21. Note that compressing any single song is not sufficient to copy all the songs on the flash drive (for example, after compressing the second song the sum of sizes will be equal to 10+4+3+5=22>21).

In the second example even if Ivan compresses all the songs the sum of sizes will be equal 8+4+1+4=17>16.

————————————————————————————————————————————

题意：给出 n 首歌曲压缩前与压缩后的大小，以及优盘的空间 m，求最少压缩几首歌能将所有歌放入优盘，如果放不进去输出 -1

思路：贪心，先判断所有歌曲压缩后的大小是否小于m，如果大于，则说明放不进去，输出 -1，然后再将所有歌曲按照可压缩空间的大小排序，不断计算压缩值统计压缩数量即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
struct Node{
    int a;
    int b;
    int com;
}song[N];
int cmp(Node x,Node y)
{
    return x.com>y.com;
}
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        long long sum_a=0,sum_b=0;
        for(int i=1;i<=n;i++)
        {
            scanf("%d%d",&song[i].a,&song[i].b);
            song[i].com=song[i].a-song[i].b;
            sum_a+=song[i].a;
            sum_b+=song[i].b;
        }

        sort(song+1,song+n+1,cmp);

        long long cnt=0,ans=0;
        long long temp=sum_a-m;
        for(int i=1;i<=n;i++)
        {
            if(ans>=temp)
                break;
            ans+=song[i].com;
            cnt++;
        }

        if(sum_b>m)//压缩后大小大于优盘空间
            printf("-1\n");
        else
            printf("%d\n",cnt);
    }

    return 0;
}
```






