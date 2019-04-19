# hdu  5938  Four Operations（模拟） - HJ - CSDN博客
2017年03月13日 19:49:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：386
Problem Description 
Little Ruins is a studious boy, recently he learned the four operations!
Now he want to use four operations to generate a number, he takes a string which only contains digits ‘1’ - ‘9’, and split it into 5 intervals and add the four operations ‘+’, ‘-‘, ‘*’ and ‘/’ in order, then calculate the result(/ used as integer division).
Now please help him to get the largest result.
Input 
First line contains an integer T, which indicates the number of test cases.
Every test contains one line with a string only contains digits ‘1’-‘9’.
Limits 
1≤T≤105 
5≤length of string≤20
Output 
For every test case, you should output ‘Case #x: y’, where x indicates the case number and counts from 1 and y is the result.
Sample Input
1 
12345
Sample Output
Case #1: 1
题目大意： 
    给出一个长度大于5的只由‘1’~‘9’组成的字符串，现在给字符串之间找出4个空格，按照先后顺序填下‘+’、‘-’、‘*’、‘/（整除）’四个符号，要求最后计算的结果最大。
解题思路：
首先应该用‘-’分成两部分：
第一部分：**左边的两个加数和尽量大**；
因为和尽量大，那么可以肯定的是有一个加数一定只有一位数，因此可以从取第一位作为加数或者取‘-’前面一位作为加数的两种情况考虑，比较两种结果孰大孰小即可。
第二部分：**右边的两个数相乘再整除后面一个数的商尽量小**；
易得，两个数相乘一定是两个一位数，剩下的一定是作为除数！
另外，一定要注意的是，结果初始值一定要足够小！
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
#define INF 10000000000000005
char s[105];
long long check(int l,int r)
{
    long long sum=0;
    while(l<=r)
      sum=sum*10+s[l++]-'0';
    return sum;
}
int main()
{
    long long t,tt=1;
    cin>>t;
    while(t--)
    {
        cin>>s;
        int len=strlen(s)-1;
        long long a,b,c,d,e,ans=-18888,maxadd=-1,minmul=INF;  //ans尽量小
        for(int i=1;i<len-2;i++)
        {
            long long x=s[0]-'0',y=s[i]-'0';
            long long aa=check(1,i);
            aa+=x;
            long long bb=check(0,i-1);
            bb+=y;
            if(aa<bb)            //比较两种情况的结果大小 
              aa=bb;
            maxadd=aa;
            c=s[i+1]-'0';       
            d=s[i+2]-'0';         //取两个乘数 作为被除数 
            e=check(i+3,len);    //剩下的就是除数了 
            minmul=c*d/e;
            if(ans<maxadd-minmul)       //依次枚举，取最大值 
              ans=maxadd-minmul;
        }
        printf("Case #%lld: %lld\n",tt++,ans);
    } 
    return 0;
}
```
