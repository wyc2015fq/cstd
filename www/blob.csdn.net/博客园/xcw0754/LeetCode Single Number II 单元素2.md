# LeetCode Single Number II 单元素2 - xcw0754 - 博客园
# [LeetCode Single Number II 单元素2](https://www.cnblogs.com/xcw0754/p/4669023.html)
题意：给一个序列，其中只有1个元素只出现1次，其他的都一定出现3次。问这个出现一次的元素是多少？
思路：
（1）全部元素拆成二进制，那么每个位上的1的个数应该是3的倍数，如果不是3的倍数，则ans的这个位肯定为1。
　　22ms
```
1 class Solution {
 2 public:
 3     int singleNumber(vector<int>& nums) {
 4         int times[32]={0};
 5         for(int i=0; i<nums.size(); i++)
 6             for(int j=0; j<=31; j++)
 7                 if( 1&(nums[i]>>j) )
 8                     times[j]++;
 9 
10         int ans=0;
11         for(int i=31; i>=0; i--)
12         {
13             ans<<=1;
14             if(times[i]%3 )    ans|=1;//如果有余数，则保障最低位为1
15         }
16         return ans;
17     }
18 };
AC代码
```
(2)用位运算。道理同方法一。为了方便理解，写复杂一点。共需要3个变量，s保存出现过2次的1，one保存出现1次的1，还有一个临时变量t做记录用。
　　首先，如果s和one中同时出现1，视为该位已经出现了3的倍数个1，要将one和s中该位清零。因为s中的1代表该位已经出现了2次1。
　　最后，one就是结果，表示一个只出现1次的元素。而s表示一个只出现2次的元素，当然，与此题无关。
```
1 class Solution {
 2 public:
 3     int singleNumber(vector<int>& nums) {
 4         int s=0, one=0;
 5         for(int i=0; i<nums.size(); i++)
 6         {
 7             s|=(~(one^nums[i]))&one;//保存出现了2次的1
 8             one^=nums[i];
 9             int t=s&one;//t所有为1的位都要在s和one中对应位清零，因为该位的1出现了3次，下面两步时清除工作。 
10             s&=~t;
11             one&=~t;
12         }
13         return one;
14     }
15 };
AC代码
```

