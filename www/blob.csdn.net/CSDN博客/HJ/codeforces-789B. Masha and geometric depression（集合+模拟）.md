# codeforces-789B. Masha and geometric depression（集合+模拟） - HJ - CSDN博客
2017年03月31日 21:42:32[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：399
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Masha really loves algebra. On the last lesson, her strict teacher Dvastan gave she new exercise.
You are given geometric progression b defined by two integers b1 and q. Remind that a geometric progression is a sequence of integers b1, b2, b3, …, where for each i > 1 the respective term satisfies the condition bi = bi - 1·q, where q is called the common ratio of the progression. Progressions in Uzhlyandia are unusual: both b1 and q can equal 0. Also, Dvastan gave Masha m “bad” integers a1, a2, …, am, and an integer l.
Masha writes all progression terms one by one onto the board (including repetitive) while condition |bi| ≤ l is satisfied (|x| means absolute value of x). There is an exception: if a term equals one of the “bad” integers, Masha skips it (doesn’t write onto the board) and moves forward to the next term.
But the lesson is going to end soon, so Masha has to calculate how many integers will be written on the board. In order not to get into depression, Masha asked you for help: help her calculate how many numbers she will write, or print “inf” in case she needs to write infinitely many integers. 
Input
The first line of input contains four integers b1, q, l, m (-109 ≤ b1, q ≤ 109, 1 ≤ l ≤ 109, 1 ≤ m ≤ 105) — the initial term and the common ratio of progression, absolute value of maximal number that can be written on the board and the number of “bad” integers, respectively.
The second line contains m distinct integers a1, a2, …, am (-109 ≤ ai ≤ 109) — numbers that will never be written on the board. 
Output
Print the only integer, meaning the number of progression terms that will be written on the board if it is finite, or “inf” (without quotes) otherwise. 
Examples 
Input
3 2 30 4 
6 14 25 48
Output
3
Input
123 1 2143435 4 
123 11 -5453 141245
Output
0
Input
123 1 2143435 4 
54343 -13 6 124
Output
inf
Note
In the first sample case, Masha will write integers 3, 12, 24. Progression term 6 will be skipped because it is a “bad” integer. Terms bigger than 24 won’t be written because they exceed l by absolute value.
In the second case, Masha won’t write any number because all terms are equal 123 and this is a “bad” integer.
In the third case, Masha will write infinitely integers 123.
题目大意： 
给出一个等比数列的首项b1和公比q，以及这个数列的所有项的绝对值都不能大于l，现在在黑板上面写下m个数字，要求不能再写重复的数字，问最后这个数列中能写多少个。如果能写无穷多个输出“inf”。
解题思路：
先考虑公比为-1、0、1的情况，另外还要考虑一下首项b1为0的情况，接着就用循环走一遍，用集合筛一下就可以了。
```cpp
#include<iostream>
#include<cmath>
#include<set>
using namespace std;
set<long long>c;
int main()
{
    long long b1,q,l,m;
    while(cin>>b1>>q>>l>>m)
    {
        long long x,cnt=0;
        while(m--)
        {
            cin>>x;
            c.insert(x);
        }
        if(l<abs(b1))
        {
            cout<<0<<endl;
            continue;
        }
        if(b1==0)
        {
            if(c.find(0)!=c.end())
              cout<<0<<endl;
            else
              cout<<"inf"<<endl;
            continue;
        }
        if(q==-1)
        {
            if(c.find(b1)!=c.end()&&c.find(-b1)!=c.end())
              cout<<0<<endl;
            else
              cout<<"inf"<<endl;
        }
        else if(q==0)
        {
            if(c.find(0)!=c.end())
            {
                if(c.find(b1)!=c.end())
                  cout<<0<<endl;
                else
                  cout<<1<<endl;
            }
            else
              cout<<"inf"<<endl;
        }
        else if(q==1)
        {
            if(c.find(b1)!=c.end())
              cout<<0<<endl;
            else
              cout<<"inf"<<endl;
        }
        else 
        { 
            while(l>=abs(b1))    //如果公比不为1，则求出所有绝对值不超过l的数值存入b[]中 
            {
                if(c.find(b1)==c.end())
                  cnt++;
                b1*=q;
            }
            cout<<cnt<<endl;      //总数p减去不能输出的cnt个数即为可以输出的个数 
        }
    }
    return 0;
}
```
