# HDU 1069 Monkey and Banana 最长上升子序列进阶（动态规划） - 紫芝的博客 - CSDN博客





2018年11月15日 14:10:18[紫芝](https://me.csdn.net/qq_40507857)阅读数：474








# HDU 1069（动态规划）

# [Monkey and Banana](http://acm.hdu.edu.cn/showproblem.php?pid=1069)

Time Limit: 2000/1000 MS (Java/Others) Memory Limit: 65536/32768 K (Java/Others)

# Problem Description

A group of researchers are designing an experiment to test the IQ of a monkey. They will hang a banana at the roof of a building, and at the mean time, provide the monkey with some blocks. If the monkey is clever enough, it shall be able to reach the banana by placing one block on the top another to build a tower and climb up to get its favorite food.

The researchers have n types of blocks, and an unlimited supply of blocks of each type. Each type-i block was a rectangular solid with linear dimensions (xi, yi, zi). A block could be reoriented so that any two of its three dimensions determined the dimensions of the base and the other dimension was the height.

They want to make sure that the tallest tower possible by stacking blocks can reach the roof. The problem is that, in building a tower, one block could only be placed on top of another block as long as the two base dimensions of the upper block were both strictly smaller than the corresponding base dimensions of the lower block because there has to be some space for the monkey to step on. This meant, for example, that blocks oriented to have equal-sized bases couldn’t be stacked.

Your job is to write a program that determines the height of the tallest tower the monkey can build with a given set of blocks.

# Input

The input file will contain one or more test cases. The first line of each test case contains an integer n, 

representing the number of different blocks in the following data set. The maximum value for n is 30. 

Each of the next n lines contains three integers representing the values xi, yi and zi. 

Input is terminated by a value of zero (0) for n.

# Output

For each test case, print one line containing the case number (they are numbered sequentially starting from 1) and the height of the tallest possible tower in the format “Case case: maximum height = height”.

# Sample Input

1 

10 20 30 

2 

6 8 10 

5 5 5 

7 

1 1 1 

2 2 2 

3 3 3 

4 4 4 

5 5 5 

6 6 6 

7 7 7 

5 

31 41 59 

26 53 58 

97 93 23 

84 62 64 

33 83 27 

0

# Sample Output

Case 1: maximum height = 40 

Case 2: maximum height = 21 

Case 3: maximum height = 28 

Case 4: maximum height = 342

### 题意

**一群研究人员要测一下猴子的智商，他们把香蕉放到房顶上，给猴子一些方块，让猴子垒方块爬上去，处于上面的方块长和宽都严格小于下面的方块，要不然猴子怎么落脚啊？**

**给出N个方块的长、宽、高，每种方块有无数种，求这些方块组合起来的最大高度？**

分析

首先，这些方块的长和宽从下到上都是递减的，只是高度不同而已；想一想**[最长上升子序列](https://blog.csdn.net/qq_40507857/article/details/81198662)**，每个元素的值都是依次递增的，最后求最长的子序列长度；

**如果把方块当作数字来看的话，同样，满足最长上升子序列的条件，只不过每个元素的长度是方块的高度，元素大小的比较变成了方块长和宽的比较，然后就变成了最长上升子序列的升级版**

这里每个方块可以使用多次，只要满足长和宽递减即可，所以要按长和宽排序，一个方块有6种使用方法，长、宽、高排列组合一下就好了

### 状态转移方程：dp[i]=max(dp[i],dp[j]+a[i].z)  (j<i,a[j].x>a[i].x&&a[j].y>a[i].y)

```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
typedef long long ll;
using namespace std;
const int maxn=200;
int dp[maxn];//表示以a[i]结尾的最长上升子序列的长度
struct Block{
    int x,y,z;
    //按长宽递减的方式来比较方块
    bool friend operator > (Block a,Block b){
        if(a.x>b.x&&a.y>b.y)
            return true;
        return false;
    }
}a[maxn];
//给方块排序
bool cmp(Block a,Block b)
{
    if(a.x>b.x)
        return true;
    else if(a.x==b.x)
        return a.y>b.y;
    return false;
}
int k,n;
void turn(int x,int y,int z){
    k++;
    a[k].x=x;
    a[k].y=y;
    a[k].z=z;
}
int main(){
    int ca=1;
    while(scanf("%d",&n)!=EOF&&n){
            k=0;
        for(int i=0;i<n;i++){
            Block b;
            scanf("%d%d%d",&b.x,&b.y,&b.z);
            turn(b.x,b.y,b.z);
            turn(b.x,b.z,b.y);

            turn(b.y,b.x,b.z);
            turn(b.y,b.z,b.x);

            turn(b.z,b.x,b.y);
            turn(b.z,b.y,b.x);
        }
        sort(a+1,a+k+1,cmp);
        
        int ans=-1;
        for(int i=1;i<k;i++){//枚举子序列的终点
            dp[i]=a[i].z;//长度最短为自身
            for(int j=1;j<i;j++)//从头向终点检查每一个元素
                if(a[j]>a[i])//递减
                dp[i]=max(dp[i],dp[j]+a[i].z);//状态转移
            ans=max(ans,dp[i]);
        }
         printf("Case %d: maximum height = %d\n",ca++,ans);
    }
}
```





