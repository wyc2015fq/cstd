# codeforces 750-C. New Year and Rating（模拟） - HJ - CSDN博客
2017年07月17日 14:18:34[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：321
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Every Codeforces user has rating, described with one integer, possibly negative or zero. Users are divided into two divisions. The first division is for users with rating 1900 or higher. Those with rating 1899 or lower belong to the second division. In every contest, according to one’s performance, his or her rating changes by some value, possibly negative or zero.
Limak competed in n contests in the year 2016. He remembers that in the i-th contest he competed in the division di (i.e. he belonged to this division just before the start of this contest) and his rating changed by ci just after the contest. Note that negative ci denotes the loss of rating.
What is the maximum possible rating Limak can have right now, after all n contests? If his rating may be arbitrarily big, print “Infinity”. If there is no scenario matching the given information, print “Impossible”. 
Input
The first line of the input contains a single integer n (1 ≤ n ≤ 200 000).
The i-th of next n lines contains two integers ci and di ( - 100 ≤ ci ≤ 100, 1 ≤ di ≤ 2), describing Limak’s rating change after the i-th contest and his division during the i-th contest contest. 
Output
If Limak’s current rating can be arbitrarily big, print “Infinity” (without quotes). If the situation is impossible, print “Impossible” (without quotes). Otherwise print one integer, denoting the maximum possible value of Limak’s current rating, i.e. rating after the n contests. 
Examples 
Input
3 
-7 1 
5 2 
8 2
Output
1907
Input
2 
57 1 
22 2
Output
Impossible
Input
1 
-5 1
Output
Infinity
Input
4 
27 2 
13 1 
-50 1 
8 2
Output
1897
Note
In the first sample, the following scenario matches all information Limak remembers and has maximum possible final rating:
```
Limak has rating 1901 and belongs to the division 1 in the first contest. His rating decreases by 7.
With rating 1894 Limak is in the division 2. His rating increases by 5.
Limak has rating 1899 and is still in the division 2. In the last contest of the year he gets  + 8 and ends the year with rating 1907.
```
In the second sample, it’s impossible that Limak is in the division 1, his rating increases by 57 and after that Limak is in the division 2 in the second contest.
题目大意： 
 Anny一共参加了n场codeforces比赛，按时间给出参赛记录，每条记录包括ci，di。 ci表示在第i场rating的变化，di有两个可能值：
di=1 表示div1（rating>=1900）；
di=2 表示div2（rating<1900）；
问n场之后Anny的rating的最大值为多少？
解题思路： 
设置两个变量值p,q分别为正负无穷大进行分类讨论:
```
1）di=1,与1900比较取大；
2）如果di=2，与1899比较取小。
```
然后判断最后的结果是否为定值或者依然为无穷大即可。
```cpp
#include<iostream>
using namespace std;
#define INF 0x3f3f3f3f
int main()
{
    int n;
    while(cin>>n)
    {
        int a,b,c=-INF,d=INF;
        for(int i=0;i<n;i++)
        {
            cin>>a>>b;
            if(b==1)
              c=max(c,1900);
            else
              d=min(d,1899);
            if(c>d)
            {
                cout<<"Impossible"<<endl;
                return 0;
            }
            c+=a;
            d+=a;
        }
        if(d>=(INF-2e7))
          cout<<"Infinity"<<endl;
        else 
          cout<<d<<endl;
    } 
    return 0;
}
```
