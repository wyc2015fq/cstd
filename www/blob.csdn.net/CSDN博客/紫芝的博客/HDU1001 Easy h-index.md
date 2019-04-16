# HDU1001 Easy h-index - 紫芝的博客 - CSDN博客





2018年07月14日 17:09:59[紫芝](https://me.csdn.net/qq_40507857)阅读数：108








# [点击打开链接](http://acm.hdu.edu.cn/contests/contest_showproblem.php?pid=1001&cid=798)

# Easy h-index

**Time Limit: 2000/1000 MS (Java/Others)    **

**Memory Limit: 32768/32768 K (Java/Others)**



**Total Submission(s): 761    Accepted Submission(s): 213**





Problem Description

比赛题目：

http://acm.hdu.edu.cn/downloads/2018ccpc_hn.pdf


The h-index of an author is the largest h where he has at least h papers with citations not less than h.

Bobo has published many papers.

Given a0,a1,a2,…,an which means Bobo has published ai papers with citations exactly i, find the h-index of Bobo.


Input

The input consists of several test cases and is terminated by end-of-file.

The first line of each test case contains an integer n.

The second line contains (n+1) integers a0,a1,…,an.

Output

For each test case, print an integer which denotes the result.

## Constraint

* 1≤n≤2⋅105

* 0≤ai≤109

* The sum of n does not exceed 250,000.

Sample Input

1

1 2

2

1 2 3

3

0 0 0 0

Sample Output

1

2

0



## 分析：

作者的h指数是一个最大的h，h代表至少有h篇文章被引用h次

a[i]表示被引用i次的文章个数，i从0开始，求作者的h指数

```cpp
#include <cstdio>
const int N = 2e5+10;
int a[N];
int main()
{
    int n;
    while (scanf("%d", &n) == 1) {
        for (int i = 0; i <= n; ++ i)
            scanf("%d", a + i);
        //预处理前缀和
        for(int i=n-1;i>=0;i--)
            a[i]+=a[i+1];
        for(int i=n;i>=0;i--)
        {
            if(a[i]>=i){
                printf("%d\n",i);
                break;
            }
        }
    }
}
```





