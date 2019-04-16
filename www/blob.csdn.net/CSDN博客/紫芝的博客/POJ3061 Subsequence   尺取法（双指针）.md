# POJ3061 Subsequence   尺取法（双指针） - 紫芝的博客 - CSDN博客





2018年11月11日 15:19:23[紫芝](https://me.csdn.net/qq_40507857)阅读数：252








## [Subsequence](https://cn.vjudge.net/problem/POJ-3061)

## [POJ - 3061](https://cn.vjudge.net/problem/21444/origin)

A sequence of N positive integers (10 < N < 100 000), each of them less than or equal 10000, and a positive integer S (S < 100 000 000) are given. Write a program to find the minimal length of the subsequence of consecutive elements of the sequence, the sum of which is greater than or equal to S.

Input

The first line is the number of test cases. For each test case the program has to read the numbers N and S, separated by an interval, from the first line. The numbers of the sequence are given in the second line of the test case, separated by intervals. The input will finish with the end of file.

Output

For each the case the program has to print the result on separate line of the output file.if no answer, print 0.

Sample Input

```
2
10 15
5 1 3 5 10 7 4 9 2 8
5 11
1 2 3 4 5
```

Sample Output

```
2
3
```

### 题意：

**给以一个正整数序列，求一个长度最短的连续子序列，并使该序列的和，大于等给定正整数S，输出最短子序列的长度**

区间【5，10】

![](https://img-blog.csdnimg.cn/20181111145726652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

区间【4，15】 

![](https://img-blog.csdnimg.cn/20181111150052854.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

 总结算法：
- 当前区间和小于S时，右端点右移
- 当前区间和大于等于S时，左端点右移

尺取法：通过移动左右两个端点，遍历一个序列，求得最短区间的算法；

时间复杂度O(n);

应用尺取法求解问题有两个特征：
- 问题的答案是满足某个条件的最短连续序列，或区间
- 任意两个合法区域（满足题目要求的区间）[a,b]  ,  [c,d] ,若a>c,必有d>=b

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





