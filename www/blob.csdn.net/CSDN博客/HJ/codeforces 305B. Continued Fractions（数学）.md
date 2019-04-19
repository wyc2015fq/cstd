# codeforces 305B. Continued Fractions（数学） - HJ - CSDN博客
2017年04月08日 18:25:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：274
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
A continued fraction of height n is a fraction of form![这里写图片描述](http://codeforces.com/predownloaded/ef/5e/ef5e214e48d87f87b0cf4f44b605f3a6aac5ccea.png) . You are given two rational numbers, one is represented as ![这里写图片描述](http://codeforces.com/predownloaded/7b/21/7b213bbd1427432d309400301100bfaa85bd959f.png)and the other one is represented as a finite fraction of height n. Check if they are equal. 
Input
The first line contains two space-separated integers p, q (1 ≤ q ≤ p ≤ 1018) — the numerator and the denominator of the first fraction.
The second line contains integer n (1 ≤ n ≤ 90) — the height of the second fraction. The third line contains n space-separated integers a1, a2, …, an (1 ≤ ai ≤ 1018) — the continued fraction.
Please, do not use the %lld specifier to read or write 64-bit integers in С++. It is preferred to use the cin, cout streams or the %I64d specifier. 
Output
Print “YES” if these fractions are equal and “NO” otherwise. 
Examples 
Input
9 4 
2 
2 4
Output
YES
Input
9 4 
3 
2 3 1
Output
YES
Input
9 4 
3 
1 2 4
Output
NO
Note
In the first sample .![这里写图片描述](http://codeforces.com/predownloaded/42/b6/42b65deefbf5983b8d09438729ef19b86ca6848e.png)
In the second sample .![这里写图片描述](http://codeforces.com/predownloaded/8b/87/8b87b675e3934ecb9d786a078384811a4efa28c4.png)
In the third sample .![这里写图片描述](http://codeforces.com/predownloaded/78/ea/78ead2dd4a6919e690a35f0e1e1dc31af0e706f8.png)
解题思路：
注意翻转分子和分母，每次减去假分数的部分就行了。
```cpp
#include<iostream>
using namespace std;
__int64 s[100005];
int main()
{
    __int64 a,b,n,i;
    while(cin>>a>>b)
    {
        cin>>n;
        for(i=0;i<n;i++)
          cin>>s[i];
        for(i=0;i<n;i++)
        {
            if(b==0||a/b<s[i])
              break;
            a-=s[i]*b;
            swap(a,b);
        }
        if(i==n&&b==0)
          cout<<"YES"<<endl;
        else
          cout<<"NO"<<endl;
    } 
    return 0;
}
```
