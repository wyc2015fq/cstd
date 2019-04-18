# LeetCode Missing Number （简单题） - xcw0754 - 博客园
# [LeetCode Missing Number （简单题）](https://www.cnblogs.com/xcw0754/p/4918839.html)
题意：
　　给一个含有n个整数的数组，数组中的元素应该是0～n。现在缺了其中某1个，找出缺少的那个整数？
思路：
　　0～n的总和是可以直接计算的，而缺少的那个就是sum减去数组的和。
```
1 int missingNumber(vector<int>& nums)
2 {
3     int sum=0;
4     for(int i=0; i<nums.size(); i++)
5         sum+=i+1-nums[i];
6     return sum;
7 }
AC代码
```

