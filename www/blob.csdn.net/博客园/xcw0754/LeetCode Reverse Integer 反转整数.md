# LeetCode  Reverse Integer 反转整数 - xcw0754 - 博客园
# [LeetCode  Reverse Integer 反转整数](https://www.cnblogs.com/xcw0754/p/4084102.html)
```
1 class Solution {
 2 public:
 3     int reverse(int x) {
 4         int e,s,num,k=0;
 5         num=x;
 6         e=0;
 7         if(x<0)
 8             num=-1*x;
 9         while( num!=0 ){
10             s=num%10;
11             e=e*10+s;
12             num=num/10;
13             k++;
14         }
15         if(x<0)
16             return -e;
17         else
18             return e;
19     }
20 };
```
题意：
Example1: x = 123, return 321
Example2: x = -123, return -321
将一个整数的个位拆出来，放到另一个整数的最高位。
再将十位拆出来，放到另一个整数的此高位。
。。。。。
直到将x的最高位都给拆出来给另一个整数当个位。
注意：
1.正负号要和原来相同，所以得提取出来好一点。
思路：
1.直接拆，用除号÷ 和求余%解决。

