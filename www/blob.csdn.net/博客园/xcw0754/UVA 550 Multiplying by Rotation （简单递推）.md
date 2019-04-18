# UVA 550 Multiplying by Rotation （简单递推） - xcw0754 - 博客园
# [UVA 550 Multiplying by Rotation （简单递推）](https://www.cnblogs.com/xcw0754/p/4606421.html)
题意：有些数字是可以这样的：abcd*k=dabc，例如179487 * 4 = 717948，仅仅将尾数7移动到前面，其他都不用改变位置及大小。这里会给出3个数字b、d、k，分别代表b进制、尾数、第2个乘数。既然是尾数，必有d<b。求这个abc...d一共有几位数（按b进制算）。
思路：举例，进制是十，假设这个数字是6位的，那么abcdef代表了这6位数，f已经给出，是7，且另个乘数是4。因为abcde7*4=7abcde。
（1）推e ： 4*7=28，那么e一定是为8。
（2）推d：abcd87*4=abcd8，去掉不必要的东西继续算，abcd8*4=abcd，那么4*8=32，还得加上第1步中遗留的2，32+2=34，那么d肯定是4；
（3）推c：abc487*4=abc48,去掉冗余，abc4*4=abc，那么4*4=16+3=19，那么c就为9。
（4）以此类推，直到a=7为止。
这里已经假设了是6位的，是因为我已经知道了答案，我们可以不必要假设是几位的，只需要一直推下去，直到推出最后一个数字就行了（题意给的d），且要无进位才行。有进位还需要继续直到进位为0。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 int a, b, c;
 5 int main()
 6 {
 7    // freopen("input.txt", "r", stdin);
 8     while(~scanf("%d%d%d",&a,&b,&c))
 9     {
10         int e=b;
11         int f=0;
12         int cnt=1;
13         int g=0;
14         while(1)
15         {
16             g=e*c+f;    //总结果
17             e=g%a;      //e为每次计算的最后一位
18             f=g/a;      //f为需要进位的数
19             if(g==b)    //等于开头
20             {
21                 printf("%d\n",cnt);
22                 break;
23             }
24             cnt++;
25         }
26     }
27     return 0;
28 }
AC代码
```

