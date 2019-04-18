# LeetCode OJ Palindrome Number（回文数） - xcw0754 - 博客园
# [LeetCode OJ Palindrome Number（回文数）](https://www.cnblogs.com/xcw0754/p/4081158.html)
```
1 class Solution {
 2 public:
 3     bool isPalindrome(int x) {
 4         int r=0,init=x;
 5         if(init==0)     return true;
 6         if(init<0)      return false;
 7         while(init!=0){
 8             r=r*10+init%10;
 9             init=init/10;
10         }
11         if(r==x)
12             return true;
13         else
14             return false;
15     }
16 };
```
题目：
　　判断一个整数是不是回文数，即一个数翻转过来是否跟原来的数仍一样。
　　需要考虑负数，负数无回文数。
解法：
　　翻转提供的数字，即123的话，需要翻转为321.
　　再判断翻转后两个数字还是否相同。

