# 137. Single Number II 只出现一次的数字 II - 别说话写代码的博客 - CSDN博客





2019年02月22日 23:19:58[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13标签：[Single Number II																[leetcode																[只出现一次的数字 II](https://so.csdn.net/so/search/s.do?q=只出现一次的数字 II&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)





|Given a **non-empty** array of integers, every element appears *three* times except for one, which appears exactly once. Find that single one.**Note:**Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?**Example 1:****Input:** [2,2,3,2]**Output:** 3**Example 2:****Input:** [0,1,0,1,0,1,99]**Output:** 99|给定一个**非空**整数数组，除了某个元素只出现一次以外，其余每个元素均出现了三次。找出那个只出现了一次的元素。**说明：**你的算法应该具有线性时间复杂度。 你可以不使用额外空间来实现吗？**示例 1:****输入:** [2,2,3,2]**输出:** 3**示例 2:****输入:** [0,1,0,1,0,1,99]**输出:** 99|
|----|----|

思路：这种在多少个元素里找一个不同的题目，思路 先想位运算。这道题目除一个元素出现一次外其余元素都出现3次，所有出现3次的所有元素有个共同特征就是他们每一位的和都可以被3整除，那只需要用个32位（int是4字节，每个字节8位）元素，计算每个元素的和看是否被3整除，如果可以那返回值这一位的元素为0，如果不可以那返回值的这一位元素为1。因为除三取余只有两种可能，0或者1.

拓展：数组中除了两个数字之外，其余元素都出现两次。找出这两个只出现一次的数字。

思路：全部元素异或结果肯定至少有一位是1，根据这一位对所有元素分成两组，每组分别包含了除一个以外其他元素全部相同。再分别对每组进行异或找出最终两个元素。

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res=0,n=nums.size();
        for(int i=0;i<32;++i)
        {
            int sum=0;
            for(int j=0;j<n;++j)
                sum += (nums[j]>>i)&1;
            res += (sum % 3)<<i;
        }return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Single Number II&t=blog)




