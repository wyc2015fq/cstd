# hdu  1061 Rightmost Digit（快速幂） - HJ - CSDN博客
2016年07月12日 16:54:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：238
个人分类：[====ACM====																[----------数学----------																[矩阵计算																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308049)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Given a positive integer N, you should output the most right digit of N^N.
Input 
The input contains several test cases. The first line of the input is a single integer T which is the number of test cases. T test cases follow. 
Each test case contains a single positive integer N(1<=N<=1,000,000,000).
Output 
For each test case, you should output the rightmost digit of N^N.
Sample Input 
2 
3 
4
Sample Output 
7 
6
Hint
In the first case, 3 * 3 * 3 = 27, so the rightmost digit is 7. 
In the second case, 4 * 4 * 4 * 4 = 256, so the rightmost digit is 6.
解析： 
    题目意思就是求一个数n的n次方的结果对10取模。
首先我们来分析一下，整数n最大是10的9次方，就算你想到取模公式
```
(n*n)%10==(n%10*n%10)%10
```
可是你有想过这个循环要最大是开多大吗。题目明确要求时间不超过1000MS，很显然这样写是会超时的。 
   那么我们就要想办法把循环的次数，也就是时间复杂度从O(n)往小了去想。 
   这个时候可能就会想到二分，因为有的时候在处理大量数据的时候都有用到二分思想，比如说排序，查找等等。 
   那么具体的二分怎么实现呢，就是把十进制整数n二进制表示出来，比如说要计算7的7次方，我们可以分成以下几步来做
```
（7*7）*（7*7）*7*（7*7）==(（（7*7）*（7*7）)*(7*7）)*7
```
如果没看懂没关系，可以看代码，具体模板有详解
```cpp
#include<iostream>
using namespace std;
#define a 10
int mod(int n)
{
    int m=n%a,sum=1;    //对乘数n先取模定义为m,对输出结果赋初值1
    while(n)
    {
        if(n&1)       //进行位运算并且判断是否存在2的次方，也就是说判断化作二进制数之后当前位置是1还是0
          sum=(sum*m)%a;    //逐级连乘，并且每次连乘都对结果取模
        m=m*m%a;      //类似于二进制转换，乘数成倍扩大，但对结果仍然取模
        n>>=1;      //移位操作
    }
    return sum;    //输出最终结果sum，注意这个时候sum已经是取模之后的结果
}
int main()
{
    int n,t;
    cin>>t;
    while(t--)
    {
        cin>>n;
        cout<<mod(n)<<endl;  //对n的n次方进行快速幂取模操作
    }
    return 0;
}
```
