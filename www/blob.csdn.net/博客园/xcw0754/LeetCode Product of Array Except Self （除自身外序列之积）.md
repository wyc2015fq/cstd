# LeetCode Product of Array Except Self  （除自身外序列之积） - xcw0754 - 博客园
# [LeetCode Product of Array Except Self  （除自身外序列之积）](https://www.cnblogs.com/xcw0754/p/4650695.html)
![](https://images0.cnblogs.com/blog2015/641737/201507/161119374856567.png)
题意：给一个序列nums，要求返回一个序列ans，两序列元素个数相同，ans第i个元素就是除了nums[i]之外所有的数相乘之积。
思路：时间O(n)，额外空间O(0)。
　　第一次扫一遍，处理nums[0~i-1]的积作为ans[i]，这样的ans[i]就得到了i之前所有数之积，那么只剩下i后面所有数之积。
　　第二次从后往前扫，跟第一次的做法一样，但是这次得开个临时变量存储积了。
```
1 class Solution {
 2 public:
 3     vector<int> productExceptSelf(vector<int>& nums) {
 4         vector<int> ans(nums.size(),1);
 5         if(nums.size()==1)  return ans;
 6         for(int i=1; i<nums.size(); i++)    ans[i]=ans[i-1]*nums[i-1];  //i之前的部分先乘
 7 
 8         int sum=1;
 9         for(int i=nums.size()-2; i>=0; i--) //i后面部分用个变量sum存。
10         {
11             sum*=nums[i+1];
12             ans[i]*=sum;
13         }
14         return ans;
15     }
16 };
AC代码
```

