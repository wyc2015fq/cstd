# LeetCode    Ugly Number （简单题） - xcw0754 - 博客园
# [LeetCode    Ugly Number （简单题）](https://www.cnblogs.com/xcw0754/p/4924523.html)
题意：
　　判断是一个数的质因子仅含有2，3，5这3个。
思路：
　　因子2比较容易解决，num/=num-(num&num-1)就可以了。3和5的需要通过循环来另判。
 C++
```
1 class Solution {
 2 public:
 3     bool isUgly(int num)
 4     {
 5         if(num==1)    return true;
 6         if(num<1)    return false;
 7         num/=num-(num&num-1);
 8         while(num%3==0)    num/=3;
 9         while(num%5==0)    num/=5;    
10         return num==1;
11     }
12 };
AC代码
```
python3
```
1 class Solution(object):
 2     def isUgly(self, num):
 3         """
 4         :type num: int
 5         :rtype: bool
 6         """
 7         if num<1:    return False
 8         for x in [2,3,5]:
 9              while num%x==0:
10                    num/=x        
11         return num==1
AC代码
```

