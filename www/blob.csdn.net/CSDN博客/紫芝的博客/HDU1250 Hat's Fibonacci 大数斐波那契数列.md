# HDU1250 Hat's Fibonacci 大数斐波那契数列 - 紫芝的博客 - CSDN博客





2018年05月07日 18:06:59[紫芝](https://me.csdn.net/qq_40507857)阅读数：35








# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1250)

# Hat's Fibonacci
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 12650    Accepted Submission(s): 4253**


Problem Description

A Fibonacci sequence is calculated by adding the previous two members the sequence, with the first two members being both 1.
F(1) = 1, F(2) = 1, F(3) = 1,F(4) = 1,  F(n>4) = F(n - 1) + F(n-2) + F(n-3) + F(n-4)
Your task is to take a number as input, and print that Fibonacci number.





Input

Each line will contain an integers. Process to end of file.





Output

For each case, output the result in a line.




Sample Input


100




Sample Output


4203968145672990846840663646Note:No generated Fibonacci number in excess of 2005 digits will be in the test data, ie. F(20) = 66526 has 5 digits.





Author

戴帽子的




Recommend

Ignatius.L   |   We have carefully selected several similar problems for you:  [1753](http://acm.hdu.edu.cn/showproblem.php?pid=1753)[1865](http://acm.hdu.edu.cn/showproblem.php?pid=1865)[1715](http://acm.hdu.edu.cn/showproblem.php?pid=1715)[1002](http://acm.hdu.edu.cn/showproblem.php?pid=1002)[2100](http://acm.hdu.edu.cn/showproblem.php?pid=2100)


## 解法：

题中要求的数最大可能到两千多位，直接开二维数组会空间超限，
所以可以用a[ ][ ]存取一个四位，五位…或八位数等，减小数组的大小，
下面以五位数为例，每次算的结果由以前的对10取余改变成对10万取余，
输出的时候找到第一位不是0的输出，以后的都按照%05d输出




```cpp
#include<stdio.h>
#include<string.h>
int a[10001][520];
void fun()
{
    a[1][0]=1;
    a[2][0]=1;
    a[3][0]=1;
    a[4][0]=1;
    for(int i=5; i<10000; i++)
    {
        int t=0;
        for(int j=0; j<=510; j++)
        {
            a[i][j]=a[i-1][j]+a[i-2][j]+a[i-3][j]+a[i-4][j]+t;
            t=a[i][j]/100000;
            a[i][j]=a[i][j]%100000;

        }
    }

}
int main()
{
    fun();
    int n;
    while(~scanf("%d",&n))
    {
        int i;
        for( i=510; a[n][i]==0; i--);
        printf("%d",a[n][i]);
        for(int j=i-1; j>=0; j--)
            printf("%05d",a[n][j]);
        printf("\n");
    }
    return 0;
}
```





