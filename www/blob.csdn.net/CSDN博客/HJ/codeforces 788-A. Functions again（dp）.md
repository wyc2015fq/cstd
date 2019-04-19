# codeforces 788-A. Functions again（dp） - HJ - CSDN博客
2017年03月31日 21:51:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：372
个人分类：[====ACM====																[---------OJ---------																[codeforce																[--------动态规划---------																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Something happened in Uzhlyandia again… There are riots on the streets… Famous Uzhlyandian superheroes Shean the Sheep and Stas the Giraffe were called in order to save the situation. Upon the arriving, they found that citizens are worried about maximum values of the Main Uzhlyandian Function f, which is defined as follows: 
![这里写图片描述](http://codeforces.com/predownloaded/46/60/4660016cb51fe73726a0e0024bd02d214c153792.png)
In the above formula, 1 ≤ l < r ≤ n must hold, where n is the size of the Main Uzhlyandian Array a, and |x| means absolute value of x. But the heroes skipped their math lessons in school, so they asked you for help. Help them calculate the maximum value of f among all possible values of l and r for the given array a. 
Input
The first line contains single integer n (2 ≤ n ≤ 105) — the size of the array a.
The second line contains n integers a1, a2, …, an (-109 ≤ ai ≤ 109) — the array elements. 
Output
Print the only integer — the maximum value of f. 
Examples 
Input
5 
1 4 2 3 1
Output
3
Input
4 
1 5 4 7
Output
6
Note
In the first sample case, the optimal value of f is reached on intervals [1, 2] and [2, 5].
In the second case maximal value of f is reachable only on the whole array.
题目大意： 
在n个数中找到这么一个区间【l,r】满足公式要求，也就是相邻两个数之差的绝对值乘以-1的次数次方，奇数位为正，偶数位为负，要求最后和最大。
解题思路： 
很容易想到dp，如果从奇数位开始，那么奇数位必然为正，偶数位必然为负，反之，如果从偶数位开始，那么偶数位必然为正，奇数位必然为负。只要分奇偶循环各自走一遍，求最大和就好了。
```cpp
#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;
long long d[100005],s[100005],a[100005],p[100005];
int main()
{
    long long n;
    while(cin>>n)
    {
        for(int i=0;i<n;i++)
          cin>>s[i];
        for(int i=1;i<n;i++)
          a[i]=abs(s[i]-s[i-1]);
        d[1]=a[1];
        d[2]=max(d[1]-a[2],a[2]);
        for(int i=3;i<n;i+=2)         //奇数位开头 
          d[i]=max(a[i],d[i-2]-a[i-1]+a[i]);
        p[2]=a[2];
        p[3]=max(a[3],p[2]-a[3]);
        for(int i=4;i<n;i+=2)         //偶数位开头 
          p[i]=max(a[i],p[i-2]-a[i-1]+a[i]);
        long long ans=d[1];
        for(int i=3;i<n;i+=2)
          ans=max(ans,d[i]);
        for(int i=2;i<n;i+=2)
          ans=max(ans,p[i]);
        cout<<ans<<endl;
    }
    return 0;
}
```
