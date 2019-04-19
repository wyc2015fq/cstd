# The 3n + 1 problem - HJ - CSDN博客
2016年05月09日 17:09:24[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：638
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
hdu  The 3n + 1 problem  （1032）
Problem Description 
Problems in Computer Science are often classified as belonging to a certain class of problems (e.g., NP, Unsolvable, Recursive). In this problem you will be analyzing a property of an algorithm whose classification is not known for all possible inputs.
Consider the following algorithm: 
```
1.      input n
2.      print n
3.      if n = 1 then STOP
4.           if n is odd then n <- 3n + 1
5.           else n <- n / 2
6.      GOTO 2
```
Given the input 22, the following sequence of numbers will be printed 22 11 34 17 52 26 13 40 20 10 5 16 8 4 2 1 
It is conjectured that the algorithm above will terminate (when a 1 is printed) for any integral input value. Despite the simplicity of the algorithm, it is unknown whether this conjecture is true. It has been verified, however, for all integers n such that 0 < n < 1,000,000 (and, in fact, for many more numbers than this.) 
Given an input n, it is possible to determine the number of numbers printed (including the 1). For a given n this is called the cycle-length of n. In the example above, the cycle length of 22 is 16. 
For any two numbers i and j you are to determine the maximum cycle length over all numbers between i and j. 
Input 
The input will consist of a series of pairs of integers i and j, one pair of integers per line. All integers will be less than 1,000,000 and greater than 0. 
You should process all pairs of integers and for each pair determine the maximum cycle length over all integers between and including i and j. 
You can assume that no opperation overflows a 32-bit integer.
Output 
For each pair of input integers i and j you should output i, j, and the maximum cycle length for integers between and including i and j. These three numbers should be separated by at least one space with all three numbers on one line and with one line of output for each line of input. The integers i and j must appear in the output in the same order in which they appeared in the input and should be followed by the maximum cycle length (on the same line). 
Sample Input 
1 10 
100 200 
201 210 
900 1000
Sample Output 
1 10 20 
100 200 125 
201 210 89 
900 1000 174
题目的意思就是在输入的两个数m,n的范围之内，找出一个数i,然后这个i要符合两个 
要求: 
如果i是奇数,则把i乘以3加1; 
如果是偶数,就把i除以2. 
并且计算i最后等于1的时候总共判断的次数,如果这个次数是在m,n的范围之内最多的一个数,则最后输出这个次数的结果,否则,继续寻找下一个数.
很好解决,分几步： 
首先一个大循环遍历n,m的所有数； 
在循环内定义变量记下数i最后等于1时所需要的次数; 
最后比较，输出最多的次数；
```cpp
#include<iostream>
using namespace std;
int main()
{
    long long n,m;
    while(cin>>n>>m)
    {
        long long temp;
        cout<<n<<" "<<m<<" ";
        if(n>m)
        {
            temp=n;
            n=m;
            m=temp;
        }
        long long max=0,t=0;
        for(long long i=n;i<=m;i++)
        {
            long long blag;
            long long a=i;
            blag=0;    //定义变量计数，每循环一次就赋初值为零
            while(a!=1)
            {
              if(a%2==0)
                a=a/2;
              else
                a=a*3+1;
              blag++;
            }
            blag++;
            if(max<blag) max=blag;  //比较，总是将最大值赋给max
        }
        cout<<max<<endl;
    } 
    return 0;
}
```
