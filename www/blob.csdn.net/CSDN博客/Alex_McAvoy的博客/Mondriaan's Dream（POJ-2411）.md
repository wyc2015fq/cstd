# Mondriaan's Dream（POJ-2411） - Alex_McAvoy的博客 - CSDN博客





2018年08月21日 11:59:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：102








# Problem Description

Squares and rectangles fascinated the famous Dutch painter Piet Mondriaan. One night, after producing the drawings in his 'toilet series' (where he had to use his toilet paper to draw on, for all of his paper was filled with squares and rectangles), he dreamt of filling a large rectangle with small rectangles of width 2 and height 1 in varying ways. 

![](https://img-blog.csdn.net/2018082112085737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



Expert as he was in this material, he saw at a glance that he'll need a computer to calculate the number of ways to fill the large rectangle whose dimensions were integer values, as well. Help him, so that his dream won't turn into a nightmare!

# **Input**

The input contains several test cases. Each test case is made up of two integer numbers: the height h and the width w of the large rectangle. Input is terminated by h=w=0. Otherwise, 1<=h,w<=11.

# Output

For each test case, output the number of different ways the given rectangle can be filled with small rectangles of size 2 times 1. Assume the given large rectangle is oriented, i.e. count symmetrical tilings multiple times.

# Sample Input

**1 21 31 42 22 32 42 114 110 0**

# Sample Output

**10123514451205**

————————————————————————————————————————————————————

题意：给出两个数 n、m，现在有 1*2、2*1 两种类型的方块，要求将 n*m 的区域填满，问一共有多少种方案

思路：状压DP入门题

思路转自：[点击这里](https://blog.csdn.net/lu597203933/article/details/44137277)、[点击这里](http://https//blog.csdn.net/hopeztm/article/details/7841917)

假设现在在位置（i，j）铺地砖，且之前的位置已铺好，则在这个位置，有如下三种选择：

1. 不用铺砖，在（i-1，j）的时刻已经被竖着铺了，考虑（i，j+1）即可

2. 横着铺砖，将（i，j+1）也铺上了，考虑的是（i，j+2）即可

3. 竖着铺砖，将（i，j）和（i+1，j）铺上一个竖着的砖

因此，可以将铺砖的选择转换为2进制的状态，在位置（i，j）如果选择横铺砖，那么将（i，j）,（i，j+1）都填为1，如果选择竖着铺砖，将（i，j）填为0，将（i+1，j）填为1

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
#define N 1001
#define MOD 1000000007
#define E 1e-6
#define LL long long
using namespace std;

bool testFirstLine(int j, int m)
{
	int i=0;
	while(i<m)
	{
		if(j&(1<<i)){
            if(i==m-1 || (j&(1<<(i+1)))==0)
                return false;
            i+=2;
		}
		else
            i++;
	}
	return true;
}

bool testCompatible(int statesA, int statesB, int m)//判断下一行状态与上一行状态的兼容性
{
	int i=0;
	while(i<m)
	{
		if((statesA & (1<<i))==0)
		{
			if((statesB & (1<<i))==0)
				return false;

			i++;
		}
		else
        {
			if((statesB & (1<<i))==0)
                i++;
			else if(i==m-1 || !((statesA &(1<<(i+1))) && (statesB &(1<<(i+1)))))
				return false;
			else
                i+=2;
		}
	}
	return true;
}

long long F[11][2048];
int main()
{
	int n,m;
	while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){

        if(m>n)
            swap(m,n);

        int allStates=2<<(m-1);

        memset(F,0,sizeof(F));
        for(int j=0;j<allStates;j++)
            if(testFirstLine(j,m))
                F[0][j]=1;


        for(int i=1;i<n;i++)
            for(int j=0;j<allStates;j++)
                for(int k=0;k<allStates;k++)
                    if(testCompatible(j,k,m))
                        F[i][j]+=F[i-1][k];


        cout<<F[n-1][allStates-1]<<endl;
	}

	return 0;
}
```





