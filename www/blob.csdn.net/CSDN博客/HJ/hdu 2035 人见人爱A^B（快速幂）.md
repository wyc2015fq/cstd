# hdu  2035 人见人爱A^B（快速幂） - HJ - CSDN博客
2016年07月13日 08:43:31[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：252
Problem Description 
求A^B的最后三位数表示的整数。 
说明：A^B的含义是“A的B次方”
Input 
输入数据包含多个测试实例，每个实例占一行，由两个正整数A和B组成（1<=A,B<=10000），如果A=0, B=0，则表示输入数据的结束，不做处理。
Output 
对于每个测试实例，请输出A^B的最后三位表示的整数，每个输出占一行。
Sample Input 
2 3 
12 6 
6789 10000 
0 0
Sample Output 
8 
984 
1
解析：中文题，就是求次方，结果对1000取余，利用二分，进行位运算判断，直接给代码：
```
#include<iostream>
using namespace std;
#define a 1000
int mod(int m,int n)
{
    int k=m%a,sum=1;
    while(n)
    {
        if(n&1)      //位运算进行判断
          sum=sum*k%a;     //结果累乘取模
        k=k*k%a;    //乘数不断增大，目的是减少循环次数，使得时间复杂度为O（lgn）
        n>>=1;
    }
    return sum;
}
int main()
{
    int m,n;
    while(cin>>m>>n&&(m&&n))
    {
        cout<<mod(m,n)<<endl;
    }
    return 0;
}
```
接下来再看一道更水的：
A hard puzzle    hdu  1097
Problem Description 
lcy gives a hard puzzle to feng5166,lwg,JGShining and Ignatius: gave a and b,how to know the a^b.everybody objects to this BT problem,so lcy makes the problem easier than begin. 
this puzzle describes that: gave a and b,how to know the a^b’s the last digit number.But everybody is too lazy to slove this problem,so they remit to you who is wise.
Input 
There are mutiple test cases. Each test cases consists of two numbers a and b(0大于a,b小于或者等于2^30)
Output 
For each test case, you should output the a^b’s last digit number.
Sample Input 
7 66 
8 800
Sample Output 
9 
6
没什么好说的了，带模板对10取模
```
#include<iostream>
using namespace std;
#define a 10
int mod(int m,int n)
{
    int k=m%a,sum=1;
    while(n)
    {
        if(n&1)
          sum=(sum*k)%a;
        k=(k*k)%a;
        n>>=1;
    }
    return sum;
}
int main()
{
    int m,n;
    while(cin>>m>>n)
    {
        cout<<mod(m,n)<<endl;
    }
    return 0;
}
```
另外，快速幂取模还常常用于矩阵，具体的分析和代码可以看我的文章“矩阵快速幂”。
