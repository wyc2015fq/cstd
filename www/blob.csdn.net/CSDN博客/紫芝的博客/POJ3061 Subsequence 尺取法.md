# POJ3061 Subsequence 尺取法 - 紫芝的博客 - CSDN博客





2018年07月26日 00:18:35[紫芝](https://me.csdn.net/qq_40507857)阅读数：20
个人分类：[解题模板																[二分的力量](https://blog.csdn.net/qq_40507857/article/category/7595714)](https://blog.csdn.net/qq_40507857/article/category/7565904)








# Subsequence
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 20108||**Accepted:** 8587|

### Description

A sequence of N positive integers (10 < N < 100 000), each of them less than or equal 10000, and a positive integer S (S < 100 000 000) are given. Write a program to find the minimal length of the subsequence of consecutive elements of the sequence, the sum of which is greater than or equal to S.

### Input

The first line is the number of test cases. For each test case the program has to read the numbers N and S, separated by an interval, from the first line. The numbers of the sequence are given in the second line of the test case, separated by intervals. The input will finish with the end of file.

### Output

For each the case the program has to print the result on separate line of the output file.if no answer, print 0.

### Sample Input

```
2
10 15
5 1 3 5 10 7 4 9 2 8
5 11
1 2 3 4 5
```

### Sample Output

```
2
3
```

Source

[Southeastern Europe 2006](http://poj.org/searchproblem?field=source&key=Southeastern+Europe+2006)

### 题意：

给n个元素和一个元素和S，求连续序列的和大于等于S的最短序列长度

### 思路1：尺取法

尺取法又称为双指针，需要一个left指针和right指针，像一把长度为S的尺子一样，可以简单高效的解决这个问题

从第一个元素开始遍历，若sum小于s，则sum+=a[right],right++,若sum>=S,sum-=a[left],left++,

在这个过程中，更新right-left的最小值，即最小长度

```cpp
#include<cstdio>
#include<algorithm>
using namespace std;
const int maxn=1e5+10;
int a[maxn];
int n,s;//n个元素，元素和S
int ruler()
{
    int left=0,right=0;//双指针
    int len=n+1;//和大于等于S的最短序列长度
    int sum=0;//元素和
        while(1){
        while(right<n&&sum<s)
            sum+=a[right++];
            if(sum<s)   break;//已经到最后一个元素
            len=min(len,right-left);//更新最小值
            sum-=a[left++];
        }
      return len;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d%d",&n,&s);
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);
    int res=ruler();
    if(res<n+1) printf("%d\n",res);
    else    printf("0\n");//不存在
    }
    return 0;
}
```

### 思路2：二分

预处理序列的前缀和，得到一个单调递增的数列，然后二分查找大于等于S的连续子序列

```cpp
#include<cstdio>
#include<algorithm>
using namespace std;
const int maxn=1e5+10;
int a[maxn];
int n,s;//n个元素，元素和S
int work()
{
    for(int i=1;i<n;i++)//预处理前缀和
    a[i]+=a[i-1];
    int sum=a[n-1];//所有元素和
    int len=n+1;
    for(int i=0;a[i]+s<=a[n-1];i++)
    {
        int tmp=lower_bound(a+i,a+n,a[i]+s)-a-i;
        len=min(len,tmp);
    }
      return len;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d%d",&n,&s);
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);
    int res=work();
    if(res<n+1) printf("%d\n",res);
    else    printf("0\n");//不存在
    }
    return 0;
}
```






