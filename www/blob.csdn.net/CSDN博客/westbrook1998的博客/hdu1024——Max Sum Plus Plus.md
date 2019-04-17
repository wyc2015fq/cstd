# hdu1024——Max Sum Plus Plus - westbrook1998的博客 - CSDN博客





2018年06月01日 15:11:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：46标签：[最大m子段和																[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=最大m子段和&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Now I think you have got an AC in Ignatius.L’s “Max Sum” problem. To be a brave ACMer, we always challenge ourselves to more difficult problems. Now you are faced with a more difficult problem.  

  Given a consecutive number sequence S 1, S 2, S 3, S 4 … S x, … S n (1 ≤ x ≤ n ≤ 1,000,000, -32768 ≤ S x ≤ 32767). We define a function sum(i, j) = S i + … + S j (1 ≤ i ≤ j ≤ n).  

  Now given an integer m (m > 0), your task is to find m pairs of i and j which make sum(i 1, j 1) + sum(i 2, j 2) + sum(i 3, j 3) + … + sum(i m, j m) maximal (i x ≤ i y ≤ j x or i x ≤ j y ≤ j x is not allowed).  

  But I`m lazy, I don’t want to write a special-judge module, so you don’t have to output m pairs of i and j, just output the maximal summation of sum(i x, j x)(1 ≤ x ≤ m) instead. ^_^  

  Input 

  Each test case will begin with two integers m and n, followed by n integers S 1, S 2, S 3 … S n.  

  Process to the end of file.  

  Output 

  Output the maximal summation described above in one line.  

  Sample Input 

  1 3 1 2 3 

  2 6 -1 4 -2 3 -2 3 

  Sample Output 

  6 

  8
最大m子段和问题，查了一下题解才理解了 

d[i]表示前i个数分成m段的最大和，p[i-1]表示前i-1个数分成m-1段的最大和 

状态转移方程：d[i]=max(d[i-1],p[i-1])+num[i] 

i位置的m段最大和分为两种情况，一是num[i]也归在前面这一子段里，那个d[i]就等于d[i-1] 

二是前面是m-1段最大和，而从num[i]开始新的一段,而p[i-1]就是记录前i-1个数分为m-1段的最大和，然后取这两个情况的最大值
代码：

```cpp
//最大m子段和问题，子段不交叉，可不连续
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=1000050;
const int INF=100000007;
//原序列
int num[MAXN];
//d[i]表示前i个数分成m段的最大和
int d[MAXN];
//p[i-1]表示前i-1个数分成m-1段的最大和
int p[MAXN];
//状态转移方程：d[i]=max(d[i-1],p[i-1])+num[i]
//i位置的m段最大和分为两种情况，一是num[i]也归在前面这一子段里，那个d[i]就等于d[i-1]
//二是前面是m-1段最大和，而从num[i]开始新的一段,而p[i-1]就是记录前i-1个数分为m-1段的最大和
//取这两个情况的最大值
int main(void){
    int m,n;
    while(~scanf("%d%d",&m,&n)){
        int t;
        //初始化
        for(int i=1;i<=n;i++){
            scanf("%d",&num[i]);
        }
        memset(d,0,sizeof(d));
        memset(p,0,sizeof(p));

        for(int i=1;i<=m;i++){
            t=-1*INF;
            for(int j=i;j<=n;j++){
                //状态转移
                d[j]=max(d[j-1],p[j-1])+num[j];
                //用来记录p[i]
                p[j-1]=t;
                t=max(t,d[j]);
            }
        }
        printf("%d\n",t);
    }
    return 0;
}
```







