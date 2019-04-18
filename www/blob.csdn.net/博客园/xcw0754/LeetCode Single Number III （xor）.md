# LeetCode Single Number III （xor） - xcw0754 - 博客园
# [LeetCode Single Number III （xor）](https://www.cnblogs.com/xcw0754/p/4918688.html)
题意：
　　给一个数组，其中仅有两个元素是出现1次的，且其他元素均出现2次。求这两个特殊的元素？
思路：
　　跟查找单个特殊的那道题是差不多的，只是这次出现了两个特殊的。将数组扫一遍求全部元素的异或和 x，结果也就是这两个特殊的元素的异或和了。现在必须找到两个当中的一个，才能恢复出另外一个。注意到x的二进制位中必定有1，那么只需要找到其中任意一个1对应的整数，再将数组中所有该位为1的计算他们的异或和a，a就是结果了，而另一个数b=x^a。
```
1 vector<int> singleNumber(vector<int>& nums) {
2     int x=0;
3     for(int i=0; i<nums.size(); i++)    x^=nums[i];
4     int lowbit=x-(x&(x-1)), a=0;
5     for(int i=0; i<nums.size(); i++)
6         if( lowbit & nums[i] )    a^=nums[i];
7     return vector<int>{a,x^a};
8 }
AC代码
```

