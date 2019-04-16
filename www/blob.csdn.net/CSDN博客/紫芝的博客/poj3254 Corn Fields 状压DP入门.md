# poj3254 Corn Fields 状压DP入门 - 紫芝的博客 - CSDN博客





2018年07月28日 23:03:50[紫芝](https://me.csdn.net/qq_40507857)阅读数：20
个人分类：[动态规划](https://blog.csdn.net/qq_40507857/article/category/7565832)









# Corn Fields
|**Time Limit:** 2000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 19368||**Accepted:** 10169|

Description

Farmer John has purchased a lush new rectangular pasture composed of *M* by *N* (1 ≤ *M* ≤ 12; 1 ≤ *N* ≤ 12) square parcels. He wants to grow some yummy corn for the cows on a number of squares. Regrettably, some of the squares are infertile and can't be planted. Canny FJ knows that the cows dislike eating close to each other, so when choosing which squares to plant, he avoids choosing squares that are adjacent; no two chosen squares share an edge. He has not yet made the final choice as to which squares to plant.

Being a very open-minded man, Farmer John wants to consider all possible options for how to choose the squares for planting. He is so open-minded that he considers choosing no squares as a valid option! Please help Farmer John determine the number of ways he can choose the squares to plant.

Input

Line 1: Two space-separated integers: *M* and *N*

Lines 2..*M*+1: Line *i*+1 describes row *i* of the pasture with *N* space-separated integers indicating whether a square is fertile (1 for fertile, 0 for infertile)

Output

Line 1: One integer: the number of ways that FJ can choose the squares modulo 100,000,000.

Sample Input

2 3
1 1 1
0 1 0
Sample Output

9
Hint

Number the squares as follows:

1 2 3
  4  


There are four ways to plant only on one squares (1, 2, 3, or 4), three ways to plant on two squares (13, 14, or 34), 1 way to plant on three squares (134), and one way to plant on no squares. 4+3+1+1=9.

Source

[USACO 2006 November Gold](http://poj.org/searchproblem?field=source&key=USACO+2006+November+Gold)

### 题意：

农夫有一块地，被划分为m行n列大小相等的格子，农夫要在地里种草。但是有些地土壤不适合种草(0)，有些可以种草(1)。

并且不能再相邻的两块地种草。问农夫最多有多少种方法种草。

任何格子都不放也是一种选择，不要忘记考虑！

### 分析：

以样例数据第一行为例，三个格子都可以放牧，即每个格子都可以选择放，或不放。再考虑附加条件“相邻格子不可同时放牧”，那么我们可以列出单看第一行时的所有可行状态如下（1代表放牧，0代表不放牧）
|编号|状态|
|----|----|
|1|0 0 0|
|2|0 0 1|
|3|0 1 0|
|4|1 0 0|
|5|1 0 1|

由此，可将表中的状态看作二进制表示，那么，只需将每种状态转化为相应的十进制数，即可只用一个数字，

就能表示某一种状态，如下表：
|编号|二进制|十进制|
|----|----|----|
|1|0 0 0|0|
|2|0 0 1|1|
|3|0 1 0|2|
|4|1 0 0|4|
|5|1 0 1|5|

这种用一个数来表示一组数，以降低表示状态所需的维数的解题手段，就叫做状态压缩。

至此我们看到，在只考虑第一行的时候，有5种可行的放牧方案，但这只是我们要做的第一步。接下来要将第二行纳入考虑：

首先思考：纳入第二行后，会对当前问题造成什么样的影响？

答案还是那句话：“相邻格子不可同时放牧”！

也就是说，不止左右相邻不可以，上下之间也不能存在相邻的情况。

首先观察第二行，只有中间的格子可以放牧，那么我们的状态表格就可以相对简单些了～如下：
|编号|二进制|十进制|
|----|----|----|
|1|0 0 0|0|
|2|0 1 0|2|


只有两种可行状态，那么我们不妨一个一个来考察：

1、当第二行的状态为编号1时，第二行的三个格子都没有放牧，那么就不会与第一行的任何情况有冲突，

第一行的5种方案都可行，即：第二行选用编号1的状态时，结合第一行，可得到5种可行的放牧方案；

2、当第二行的状态为编号2时，第二行中间的格子已经放牧了，那么第一行中间的格子就不可以放牧。

看表2，发现其中第3种状态与当前第二行冲突，那么第一行只有4种方案是可行的，

即：第二行选用编号2的状态时，结合第一行，可得到4种可行的放牧方案；

那么，在样例数据给出的情况下，我们的最终答案即为5+4=9；



通过对样例数据的分析即可以发现不同状态之间的关系：

以dp[i][state(j)]来表示对于前i行，第i行采用第j种状态时可以得到的可行方案总数！

例如：回头看样例数据，dp[2][1]即代表第二行使用第2中状态（0 1 0）时可得的方案数，即为4；

那么，可得出状态转移方程为：

**dp[i][state(j)]=dp[i-1][state(k1)]+dp[i-1][state(k2)]+......+dp[i-1][state(kn)]**

(kn即为上一行可行状态的编号，上一行共有n种可行状态）

最终**ans=dp[m][state(k1)]+dp[m][state(k2)]+......+dp[m][state(kn)];** (kn即为最后一行（第m行）可行状态的编号）

```cpp
#include <cstdio>
#include <cstring>
using namespace std;
#define mod 100000000
int M,N,top = 0;
//top表示每行最多的状态数

int state[600],num[110];
//state存放每行所有的可行状态（即没有相邻的状态

int dp[20][600];
//dp[i][j]:对于前i行数据，每行有前j种可能状态时的解
int cur[20];
//cur[i]表示的是第i行整行的情况

inline bool ok(int x){	//判断状态x是否可行
   if(x&x<<1)	return false;//若存在相邻两个格子都为1，则该状态不可行
   return true;
}
void init(){			//遍历所有可能的状态
   top = 0;
   int total = 1 << N; //遍历状态的上界
   for(int i = 0; i < total; ++i){
       if(ok(i))state[++top] = i;
   }
}
inline bool fit(int x,int k){ //判断状态x 与第k行的实际状态的逆是否有‘重合’
   if(x&cur[k])return false; //若有重合，（即x不符合要求）
   return true;  //若没有，则可行
}

int main(){
    while(scanf("%d%d",&M,&N)!= EOF){
       init();
       memset(dp,0,sizeof(dp));
       for(int i = 1; i <= M; ++i){
           cur[i] = 0;
           int num;
 //输入时就要按位来存储，cur[i]表示的是第i行整行的情况，每次改变该数字的二进制表示的一位
           for(int j = 1; j <= N; ++j){
                scanf("%d",&num);  //表示第i行第j列的情况（0或1）
               if(num == 0) //若该格为0
				   cur[i] +=(1<<(N-j));
        //则将该位置为1（注意要以相反方式存储，即1表示不可放牧
           }
       }
       for(int i = 1;i <= top;i++){
           if(fit(state[i],1)){  //判断所有可能状态与第一行的实际状态的逆是否有重合
                dp[1][i] = 1;  //若第1行的状态与第i种可行状态吻合，则dp[1][i]记为1
           }

	   }

	   /*
	   状态转移过程中，dp[i][k] =Sigma dp[i-1][j] （j为符合条件的所有状态）
		*/
       for(int i = 2; i <= M; ++i){  //i索引第2行到第M行
           for(int k = 1; k <= top; ++k){
    //该循环针对所有可能的状态，找出一组与第i行相符的state[k]
                if(!fit(state[k],i))continue; //判断是否符合第i行实际情况
                for(int j = 1; j <= top ;++j){
        //找到state[k]后，再找一组与第i-1行符合，且与第i行（state[])不冲突的状态state[j]
                   if(!fit(state[j],i-1))continue;  //判断是否符合第i-1行实际情况
                   if(state[k]&state[j])continue;  //判断是否与第i行冲突
                   dp[i][k] = (dp[i][k] +dp[i-1][j])%mod;  //若以上皆可通过，则将'j'累加到‘k'上
                }
           }
       }
       int ans = 0;
       //累加最后一行所有可能状态的值，即得最终结果
       for(int i = 1; i <= top; ++i){
           ans = (ans + dp[M][i])%mod;
       }
       printf("%d\n",ans);
   }
}
```





