# hdu  4712  Hamming Distance（位运算+随机函数） - HJ - CSDN博客
2016年08月19日 18:49:47[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：259
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
(From wikipedia) For binary strings a and b the Hamming distance is equal to the number of ones in a XOR b. For calculating Hamming distance between two strings a and b, they must have equal length. 
Now given N different binary strings, please calculate the minimum Hamming distance between every pair of strings.
Input 
The first line of the input is an integer T, the number of test cases.(0大于T小于等于20) Then T test case followed. The first line of each test case is an integer N (2<=N<=100000), the number of different binary strings. Then N lines followed, each of the next N line is a string consist of five characters. Each character is ‘0’-‘9’ or ‘A’-‘F’, it represents the hexadecimal code of the binary string. For example, the hexadecimal code “12345” represents binary string “00010010001101000101”.
Output 
For each test case, output the minimum Hamming distance between every pair of strings.
Sample Input
2 
2 
12345 
54321 
4 
12345 
6789A 
BCDEF 
0137F
Sample Output
6 
7
解析：水题！最主要的是读懂题意，给出n个16进制数，长度控制为5，随机取两个数做异或运算，然后求出共有的1的个数，要求最后输出最少值。只要注意输入跟位运算即可，随机数选取懂的即可，不懂可以百度。代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<string>
#include<cmath>
using namespace std;
#define INF 1<<27
int a[100010];
int gets(int val)
{
    int res=0;
    while(val>0)
    {
        res+=val%2;         //累加异或运算完1的个数
        val/=2;
    }
    return res;
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        int n;
        scanf("%d",&n);
        for(int i=0;i<n;i++)
          scanf("%X",&a[i]);   //输入之后按16进制存入数组中
        int ans=INF;
        for(int i=1;i<=1000000;i++)
        {
            int xx=rand()%n;
            int yy=rand()%n;
            while(xx==yy)    //如果随机取的两个数相等，则重新随机取数
                yy=rand()%n;
            int val=a[xx]^a[yy];   //xx跟yy做异或运算
            ans=min(ans,gets(val));    //每次计算完1的个数之后都跟当前最小值比较，更新最小值的值
        }
        printf("%d\n",ans);
    }
    return 0;
}
```
