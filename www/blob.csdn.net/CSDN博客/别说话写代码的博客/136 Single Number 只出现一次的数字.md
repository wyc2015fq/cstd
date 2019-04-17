# 136. Single Number 只出现一次的数字 - 别说话写代码的博客 - CSDN博客





2019年02月22日 22:47:17[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：16
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)








|Given a **non-empty** array of integers, every element appears *twice* except for one. Find that single one.**Note:**Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?**Example 1:****Input:** [2,2,1]**Output:** 1**Example 2:****Input:** [4,1,2,1,2]**Output:** 4|给定一个**非空**整数数组，除了某个元素只出现一次以外，其余每个元素均出现两次。找出那个只出现了一次的元素。**说明：**你的算法应该具有线性时间复杂度。 你可以不使用额外空间来实现吗？**示例 1:****输入:** [2,2,1]**输出:** 1**示例 2:****输入:** [4,1,2,1,2]**输出:** 4|
|----|----|

思路：位运算，用 异或，相同为0相异为1.就是说只有一个元素出现一次，其他都是两次，所有出现两次出现的值异或结果为0，再与只出现一 次的值异或 就找出来了。

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res=0;
        for(int i=0;i<nums.size();++i)
            res = res^nums[i];
        return res;
    }
};
```





