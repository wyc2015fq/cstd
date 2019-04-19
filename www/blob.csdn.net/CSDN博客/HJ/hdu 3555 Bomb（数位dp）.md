# hdu  3555  Bomb（数位dp） - HJ - CSDN博客
2017年01月13日 17:23:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：1248
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------动态规划---------																[数位DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238033)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
The counter-terrorists found a time bomb in the dust. But this time the terrorists improve on the time bomb. The number sequence of the time bomb counts from 1 to N. If the current number sequence includes the sub-sequence “49”, the power of the blast would add one point. 
Now the counter-terrorist knows the number N. They want to know the final points of the power. Can you help them?
Input 
The first line of input consists of an integer T (1 <= T <= 10000), indicating the number of test cases. For each test case, there will be an integer N (1 <= N <= 2^63-1) as the description.
The input terminates by end of file marker.
Output 
For each test case, output an integer indicating the final points of the power.
Sample Input
3 
1 
50 
500
Sample Output
0 
1 
15
Hint 
From 1 to 500, the numbers that include the sub-sequence  “49”  are 
“49”,”149”,”249”,”349”,”449”,”490”,”491”,”492”,”493”,”494”,”495”,”496”,”497”,”498”,”499”, 
so the answer is 15.
题目大意：求出0~n之间含有49的数字的个数
开一个二维数组d[i][j]，i表是数字的位数，j表示要分出来的三类情况。
状态转移：
　　dp[i][0]代表长度为 i 并且不含有49的数字的个数；
　　dp[i][1]代表长度为 i 并且不含有49，但是最高位是9的数字的个数；
　　dp[i][2]代表长度为 i 并且含有49的数字的个数。 
数组 a[i] 从低位到高位存储 n 的每一位数字。
则：
dp[i][0] = dp[i-1][0] * a[i] - dp[i-1][1];  表示长度为 i 的不含有49的数字的个数等于长度为 i - 1 的不含有49的数字的个数*当前的数字，因为这个位置可以填0~a[i] - 1，然后再减去长度为 i - 1 的最高位是9的数字的个数，因为如果长度为 i - 1 的最高位是9的话，那么高一位就不能填4了，否则就组成了49。
dp[i][1] = dp[i-1][0]; 表示长度为 i 的并且不含有49同时最高位是9的数字的个数等于，长度为 i - 1 的不含有49的数字的个数，因为只要在它的高一位加上一个9就可以了。
dp[i][2] = dp[i-1][2] * a[i] + dp[i-1][1]; 表示长度为 i 的含有49的数字的个数等于，长度为 i - 1 的数字的个数*当前的数字，再加上长度为 i - 1 的并且不含有49同时最高位是9的数字的个数，因为这个时候，只要在高一位加上一个4就可以了，这样在最高的两位就组成了一个49。
　　做法是从数字的高位向低位扫描，对于第 i 位，
```
（1）　首先加上长度为 i - 1 的符合条件的数字个数；
　
　（2）　再讨论以前是不是出现过49，如果出现过，就要再追加上长度为 i - 1 的不符合条件的数字的个数，因为以前已经有49了；
　
　（3）　如果没有出现过，就要判断这一位是不是大于4呢，如果大于4，就要再追加上长度为 i - 1 的不含有49但是最高位是9的数字的个数，因为这个时候可以再这一位填4；
　
　（4）　然后就是判断一下，当前位和上一位是不是满足49，如果满足，标记出现了49了！为以后的判断做准备。
```
代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
__int64 dp[25][3];
void init()
{
    memset(dp,0,sizeof(dp));
    dp[0][0]=1;
    for(int i=1;i<22;i++)
    {
        dp[i][0]=dp[i-1][0]*10-dp[i-1][1];
        dp[i][1]=dp[i-1][0];
        dp[i][2]=dp[i-1][2]*10+dp[i-1][1];
    }
}
__int64 solve(__int64 n)
{
    __int64 ans=0,len=0,flag=0,i,j,d[25];
    while(n)
    {
        d[++len]=n%10;
        n/=10;
    } 
    d[++len]=0;
    for(i=len-1;i>=1;i--)
    {
        ans+=dp[i-1][2]*d[i];
        if(flag)
          ans+=dp[i-1][0]*d[i];
        if(!flag&&d[i]>4)
          ans+=dp[i-1][1];
        if(d[i+1]==4&&d[i]==9)         
          flag=1;
    } 
    return ans;
}
int main()
{
    int t;
    __int64 n;
    cin>>t;
    init();
    while(t--)
    {
        cin>>n;
        cout<<solve(n+1)<<endl;
    }
    return 0;
}
```
