# HDU 1398 Square Coins 平方硬币 （普通母函数，水） - xcw0754 - 博客园
# [HDU 1398 Square Coins 平方硬币 （普通母函数，水）](https://www.cnblogs.com/xcw0754/p/4519285.html)
题意：
　　有17种硬币，每种的面值为编号的平方，比如 1，4，9，16....。给出一个数字，求组成这个面值有多少种组法？
思路：
　　用普通母函数解，主要做的就是模拟乘法，因为硬币是无限的，所以每个构造式中每一个项的系数都是1。我们只需要第n项的系数，大于n的并不需要，所以大于n的项就不用再做计算了。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=310;
 4 int main()
 5 {
 6     freopen("input.txt", "r", stdin);
 7     int n, ans[N]={0};
 8     while(scanf("%d",&n), n)
 9     {
10         for(int i=0; i<=n; i++) ans[i]=1;   //初始化为1，最小面值是1
11         int tmp[N]={0};                     //保存临时的结果
12         for(int i=2; i<18&&n>=i*i; i++)
13         {
14             for(int j=0; j<=n; j++)
15             {
16                 for(int k=0; j+k<=n; k+=i*i)
17                     tmp[j+k]+=ans[j];
18             }
19             memcpy(ans,tmp,sizeof(tmp));    //以ans作为结果来乘
20             memset(tmp,0,sizeof(tmp));      //置零
21         }
22         cout<<ans[n]<<endl;
23     }
24     return 0;
25 }
AC代码
```

