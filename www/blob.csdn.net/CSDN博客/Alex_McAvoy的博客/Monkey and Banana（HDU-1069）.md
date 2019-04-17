# Monkey and Banana（HDU-1069） - Alex_McAvoy的博客 - CSDN博客





2018年03月26日 20:44:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：77








# Problem Description

    A group of researchers are designing an experiment to test the IQ of a monkey. They will hang a banana at the roof of a building, and at the mean time, provide the monkey with some blocks. If the monkey is clever enough, it shall be able to reach the banana by placing one block on the top another to build a tower and climb up to get its favorite food.

    The researchers have n types of blocks, and an unlimited supply of blocks of each type. Each type-i block was a rectangular solid with linear dimensions (xi, yi, zi). A block could be reoriented so that any two of its three dimensions determined the dimensions of the base and the other dimension was the height. 

    They want to make sure that the tallest tower possible by stacking blocks can reach the roof. The problem is that, in building a tower, one block could only be placed on top of another block as long as the two base dimensions of the upper block were both strictly smaller than the corresponding base dimensions of the lower block because there has to be some space for the monkey to step on. This meant, for example, that blocks oriented to have equal-sized bases couldn't be stacked. 

    Your job is to write a program that determines the height of the tallest tower the monkey can build with a given set of blocks.

# Input

    The first line of each test case contains an integer n,representing the number of different blocks in the following data set. The maximum value for n is 30.

    Each of the next n lines contains three integers representing the values xi, yi and zi.

    Input is terminated by a value of zero (0) for n.

# Output

    For each test case, print one line containing the case number (they are numbered sequentially starting from 1) and the height of the tallest possible tower in the format "Case case: maximum height = height".

# Sample Input

**1**

**10 20 3026 8 105 5 571 1 12 2 23 3 34 4 45 5 56 6 67 7 7531 41 5926 53 5897 93 2384 62 6433 83 270**

# Sample Output

**Case 1: maximum height = 40Case 2: maximum height = 21Case 3: maximum height = 28Case 4: maximum height = 342**

————————————————————————————————————————————

题目大意：有n个小块，每个块能摆成3种方案，每个小块提供无数个，上边的长和宽要小于下边的长和宽。问最高能够叠多高。

思路：

叠加的条件是，上面一个长方体的长和宽都比下面长方体的长和宽短，其实就是求最长的单调递增序列。在长和宽的递增下，求最大即可。



状态转移方程：dp[i]=max(dp[0~j] , dp[i])+num[i].h


边界条件：dp[0]=0

# Source Program

```cpp
#include<iostream>
#include<cstring>
#include<algorithm>
#include<cstdio>
#define N 500
using namespace std;

int dp[N];
struct block
{
    int l;
    int w;
    int h;
    int area;
}num[N];

int cmp(block a,block b)
{
    if(a.area==b.area)//先按面积排序，再按长度排序
        return a.l>b.l;
    else
        return a.area>b.area;
}

int main()
{
    int n;
    int lengh[3];
    int max_h;
    int i,j,k;
    int t=1;

    while(cin>>n && n)
    {
        memset(dp,0,sizeof(dp));
        max_h=0;

        for(i=0,k=0;i<n;i++,k+=3)
        {
            cin>>lengh[0]>>lengh[1]>>lengh[2];//每种箱子的长宽高
            sort(lengh,lengh+3);//对长方形三边从小到大排进行排序

            /*箱子的三种摆放方式*/
            num[k].l=lengh[2];
            num[k].w=lengh[1];
            num[k].h=lengh[0];
            num[k].area=lengh[2]*lengh[1];

            num[k+1].l=lengh[2];
            num[k+1].w=lengh[0];
            num[k+1].h=lengh[1];
            num[k+1].area=lengh[2]*lengh[0];

            num[k+2].l=lengh[1];
            num[k+2].w=lengh[0];
            num[k+2].h=lengh[2];
            num[k+2].area=lengh[1]*lengh[0];
        }

        sort(num,num+k,cmp);//排序

        max_h=0;
        for(i=0;i<3*n;i++)
        {
            for(j=0;j<i;j++)
                if(num[i].l<num[j].l&&num[i].w<num[j].w)
                    dp[i]=max(dp[i],dp[j]);
            dp[i]+=num[i].h;
            max_h=max(dp[i],max_h);
        }

        printf("Case %d: maximum height = %d\n",t,max_h);
        t++;
    }
    return 0;
}
```





