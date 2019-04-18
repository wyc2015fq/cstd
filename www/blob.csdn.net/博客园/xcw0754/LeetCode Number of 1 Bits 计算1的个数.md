# LeetCode  Number of 1 Bits 计算1的个数 - xcw0754 - 博客园
# [LeetCode  Number of 1 Bits 计算1的个数](https://www.cnblogs.com/xcw0754/p/4412907.html)
![](https://images0.cnblogs.com/blog2015/641737/201504/092254191028191.png)
题意：
　　提供一个无符号32位整型uint32_t变量n，返回其二进制形式的1的个数。
思路：
　　考察二进制的特性，设有k个1，则复杂度为O(k)。考虑将当前的数n和n-1做按位与，就会将n的最后一个1去掉，重复这样的操作就可以统计出1的个数了。（2015年春季 小米实习生的笔试题之一）
```
1 class Solution {
 2 public:
 3     int hammingWeight(uint32_t n) {
 4         int cnt=0;
 5         while(n)
 6         {
 7             n&=n-1;
 8             cnt++;
 9         }        
10         return cnt;
11     }
12 };
AC代码
```
python3
```
1 class Solution(object):
 2     def hammingWeight(self, n):
 3         """
 4         :type n: int
 5         :rtype: int
 6         """
 7         cnt=0
 8         while n:
 9             cnt+=1
10             n=n&(n-1)
11         return cnt
AC代码
```

