# acdream 1408 "Money, Money, Money" （水） - xcw0754 - 博客园
# [acdream 1408 "Money, Money, Money" （水）](https://www.cnblogs.com/xcw0754/p/4607649.html)
题意：给出一个自然数x，要求找两个自然数a和b，任意多个a和b的组合都不能等于x，且要可以组合成大于x的任何自然数。如果找不到就输出两个0.（输出任意一个满足条件的答案）
思路：x=偶数时，无法构成，输出两0。x=奇数时，必定能构成，且a和b中必有一奇，因为要构成奇数，必定要有奇数，可以偶+奇、奇+奇。为了构成大于x的偶数，首先令a=2，这就满足了要求了，单靠a又不会构成x这个奇数，而b应该怎么取？取x+2，因为x+1是偶数，可以由2来组成，而x+2直接就是b，而x+3可以b+a啊。就是说，大于x的偶数，全由2组成，大于x的奇数，由b+k*a组成，这里k为[0,无穷]。这是其中一个合法答案。
　　好像可以利用两个互质数x、y，而ax+by组成任意自然数那个特性来做。忘了。
```
1 // a b 要能够表示所有大于 x 的数字
 2  
 3 #include <iostream>
 4 #include <cstdio>
 5  
 6 using namespace std;
 7  
 8 int main(void)
 9 {
10     //freopen("in.txt", "r", stdin);
11  
12     long long n = 0;
13     while(cin>>n)
14     {
15         if (n%2==0)
16             cout<<"0 0"<<endl;
17         else
18         {
19             // 2 和 奇数 能够拼接任何数
20             cout<<"2 "<<n+2<<endl;
21         }
22     }
23     return 0;
24 }
AC代码
```

