# HDU 1850 Being a Good Boy in Spring Festival 在春节做乖孩子（Nim博弈，微变形） - xcw0754 - 博客园
# [HDU 1850 Being a Good Boy in Spring Festival 在春节做乖孩子（Nim博弈，微变形）](https://www.cnblogs.com/xcw0754/p/4503422.html)
题意：
思路：
　　如果全部扑克牌数目异或的结果ans为0，则必输，输出0。否则，必须要给对方一个P状态，可以对所有扑克堆进行逐个排查，将ans^a[i]就可以得到除了a[i]之外其他扑克数的异或结果tmp，目的就是要使得tmp为0，那么肯定要从a[i]堆中拿走一些牌使得剩下的牌数异或tmp为0。可能出现的情况（1）tmp比a[i]还大，那么这不是正确的方案。（2）tmp=0，那么要将a[i]全部取走。（3）tmp<a[i]，那么要取走a[i]-tmp张牌，留下tmp张，刚好异或结果为0。
```
1 #include <iostream>
 2 #include <cstdio>
 3 using namespace std;
 4 int a[101];
 5 int main()
 6 {
 7     //freopen("input.txt", "r", stdin);
 8     int n;
 9     while(cin>>n,n)
10     {
11         for(int i=0; i<n; i++)
12             scanf("%d",&a[i]);
13 
14         int ans=a[0], cnt=0;
15         for(int i=1; i<n; i++)
16             ans^=a[i];
17 
18         if(!ans)    cout<<"0"<<endl;    //必输
19         else
20         {
21             for(int i=0; i<n; i++)
22             {
23                 int tmp=ans^a[i];
24                 if(a[i]-tmp>=0)
25                     cnt++;
26             }
27             cout<<cnt<<endl;
28         }
29     }
30     return 0;
31 }
AC代码
```

