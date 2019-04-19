# codeforces 743-C. Vladik and fractions（构造） - HJ - CSDN博客
2017年07月13日 16:06:58[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：188
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Vladik and Chloe decided to determine who of them is better at math. Vladik claimed that for any positive integer n he can represent fraction ![这里写图片描述](http://codeforces.com/predownloaded/7e/a3/7ea334dc5ba22c666c8f87f28a3dbd96119e1bbd.png)as a sum of three distinct positive fractions in form ![这里写图片描述](http://codeforces.com/predownloaded/0e/20/0e203d54bfbb1cc777cf935c83d40b57e96ec58d.png).
Help Vladik with that, i.e for a given n find three distinct positive integers x, y and z such that ![这里写图片描述](http://codeforces.com/predownloaded/43/6a/436aaafa23e0ed17d16e5e10aab481e508f74c7c.png). Because Chloe can’t check Vladik’s answer if the numbers are large, he asks you to print numbers not exceeding 109.
If there is no such answer, print -1. 
Input
The single line contains single integer n (1 ≤ n ≤ 104). 
Output
If the answer exists, print 3 distinct numbers x, y and z (1 ≤ x, y, z ≤ 109, x ≠ y, x ≠ z, y ≠ z). Otherwise print -1.
If there are multiple answers, print any of them. 
Examples 
Input
3
Output
2 7 42
Input
7
Output
7 8 56
题目大意： 
给你一个n，找出一组最小的三个数x、y、z，满足x< y< z，并且2/n=1/x+1/y+1/z。
解题思路： 
构造出 
```
2/n = 1/n + 1/(n+1) + 1/(n*(n+1))
```
即此时x、y、z是最小的。 
但是有两种情况应该排除在外，第一种n=1的时候，不能构造，第二种n=3的时候，可以构造成 2/3 = 1/2 + 1/7 + 1/42
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n;
    while(cin>>n)
    {
        if(n==1)
          cout<<-1<<endl;
        else
        {
            if(n==3)
              cout<<2<<" "<<7<<" "<<42<<endl;
            else
              cout<<n<<" "<<n+1<<" "<<n*(n+1)<<endl;
        }
    }
    return 0;
}
```
