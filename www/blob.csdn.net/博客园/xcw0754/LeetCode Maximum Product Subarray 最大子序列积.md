# LeetCode Maximum Product Subarray 最大子序列积 - xcw0754 - 博客园
# [LeetCode Maximum Product Subarray 最大子序列积](https://www.cnblogs.com/xcw0754/p/4672140.html)
题意：给一个size大于0的序列，求最大的连续子序列之积。（有正数，负数，0）
思路：正确分析这三种数。0把不同的可能为答案的子序列给隔开了，所以其实可以以0为分隔线将他们拆成多个序列来进行求积，这样就没有0了。
　　接着是负数，负数如果遇到一个负数，可能反而比那个正数要大，所以正负数都要保存，遍历一次即可。在奇数个负数时，其实可能的只有2种：（1）包含最前面一个负数的序列（2）包含最后面一个负数的序列（当然不包含最前面1个，同理（1）也是）。
```
1 class Solution {
 2 public:
 3     int maxProduct(vector<int>& nums) {
 4         int a=1, b=1, res=nums[0];
 5         for(int i=0; i<nums.size(); i++)
 6         {
 7             int t=a;
 8             a=max(b*nums[i],a*nums[i]); //a保存大的
 9             b=min(t*nums[i],b*nums[i]); //b保存小的
10             res=max(res, a);
11             //if(a<=0)    a=1;    //必须保证a大于0
12             if(b>=0)    b=1;    //必须保证b小于0。为了不改变原值，只能为1。
13         }
14         return res;
15     }
16 };
AC代码
```

