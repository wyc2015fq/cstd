# hdu   5933  ArcSoft's Office Rearrangement（贪心） - HJ - CSDN博客
2017年03月13日 20:16:57[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：414
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
ArcSoft, Inc. is a leading global professional computer photography and computer vision technology company.
There are N working blocks in ArcSoft company, which form a straight line. The CEO of ArcSoft thinks that every block should have equal number of employees, so he wants to re-arrange the current blocks into K new blocks by the following two operations:
- merge two neighbor blocks into a new block, and the new block’s size is the sum of two old blocks’.
- split one block into two new blocks, and you can assign the size of each block, but the sum should be equal to the old block.
Now the CEO wants to know the minimum operations to re-arrange current blocks into K block with equal size, please help him.
Input 
First line contains an integer T, which indicates the number of test cases.
Every test case begins with one line which two integers N and K, which is the number of old blocks and new blocks.
The second line contains N numbers a1, a2, ⋯, aN, indicating the size of current blocks.
Limits 
1≤T≤100 
1≤N≤105 
1≤K≤105 
1≤ai≤105
Output 
For every test case, you should output ‘Case #x: y’, where x indicates the case number and counts from 1 and y is the minimum operations.
If the CEO can’t re-arrange K new blocks with equal size, y equals -1.
Sample Input
3 
1 3 
14 
3 1 
2 3 4 
3 6 
1 2 3
Sample Output
Case #1: -1 
Case #2: 2 
Case #3: 3
题目大意：
给出n个数据和一个k，现在要你判断能不能通过合并和拆分使得k个数据都具有相同的值。 
合并规则：
```
每次只能将相邻的数据合并，合并后的数据为两个相邻数据和；
```
拆分规则：
```
每次只能将一个数据拆分成两个，两个数据之和必须为这个拆分的数据；
```
每次合并操作或者每次拆分操作记为一次操作，问如果可以至少需要多少次操作？
解题思路：
刚开始觉得处理起来好麻烦，特别是还想了几组比较恶心的数据，就放在一边没做了。后来听了别人的思路，觉得贪心就好了，每次与平均数比较，将多的往后加，少的也往后加，如果刚刚好则取平均数倍数。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
long long s[10000005];
int main()
{
    long long t,tt=1,n,k,i;
    scanf("%lld",&t);
    while(t--)
    {
        scanf("%lld%lld",&n,&k);
        long long sum=0;
        for(i=0;i<n;i++)
        {
            scanf("%lld",&s[i]);
            sum+=s[i];
        }
        printf("Case #%lld: ",tt++);
        long long ans=0,m=0,ave;
        if(sum%k==0)
        {
            ave=sum/k;
            for(i=0;i<n;i++)
            {
                if(s[i]>ave)     //如果比平均数大
                {
                    ans+=s[i]/ave;   //首先可以分成s[i]/ave份，这是必须要的
                    m=s[i]%ave;
                    if(m==0)
                      ans--;
                    else
                    {
                        s[i+1]+=m;
                        ans++; 
                    }
                }
                else if(s[i]<ave) //如果比平均数小，干脆直接放到后面一个数据中处理
                {
                    s[i+1]+=s[i];
                    ans++;
                }
            }
            printf("%lld\n",ans);
        }
        else
          printf("-1\n");
    }
    return 0;
}
```
