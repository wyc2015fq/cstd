# hdu 5978 To begin or not to begin（概率期望） - HJ - CSDN博客
2017年10月27日 13:28:21[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：130
[http://acm.hdu.edu.cn/showproblem.php?pid=5978](http://acm.hdu.edu.cn/showproblem.php?pid=5978)
题目大意： 
有k个黑球和一个红球，同时放在一个箱子里，A和B两个人轮流取出并且不放回。问第一个人先取，跟第二个人能摸中红球的概率期望大小比较。
解题思路： 
假如k为奇数，那么k+1就为偶数，两个人能摸中红球的概率期望相等； 
假如k为偶数，那么k+1就为奇数，第一个人先取，能多取一次，能摸中红球的概率期望就会比第二个人大。用化简之后的公式比较就是
[(k+1)/2+1] / (k+1)   和   [(k+1)/2] / (k+1)    (分子中的“/”为整除)
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int n;
    while(cin>>n)
      printf("%d\n",n%2?0:1);
    return 0;
}
```
