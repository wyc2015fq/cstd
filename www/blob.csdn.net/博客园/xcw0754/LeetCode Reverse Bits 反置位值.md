# LeetCode Reverse Bits 反置位值 - xcw0754 - 博客园
# [LeetCode Reverse Bits 反置位值](https://www.cnblogs.com/xcw0754/p/4412980.html)
![](https://images0.cnblogs.com/blog2015/641737/201504/092328257741555.png)
题意：给定一个无符号32位整数，将其二进制形式左右反置，再以整型返回。
思路：循环32轮，将n往右挤出一位就补到ans的尾巴上。
```
1 class Solution {
 2 public:
 3     uint32_t reverseBits(uint32_t n) {
 4         if( !n ) return 0;
 5         uint32_t ans = 0;
 6         int i;
 7         for(i=0; i<32; i++ )
 8         {
 9             ans <<= 1;
10             if( n & 1 )
11                 ans |= 1;
12             n >>=1;
13         }
14         return ans;
15     }
16 };
Reverse Bits
```

