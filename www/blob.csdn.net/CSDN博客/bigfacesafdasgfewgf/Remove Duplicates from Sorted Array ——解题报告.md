# Remove Duplicates from Sorted Array ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月12日 10:31:09[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：579











    【题目】




Given a sorted array, remove the duplicates in place such that each element appear only once and return the new length.


Do not allocate extra space for another array, you must do this in place with constant memory.


For example,

Given input array nums = `[1,1,2]`,


Your function should return length = `2`, with the first two elements of nums being `1` and `2` respectively.
 It doesn't matter what you leave beyond the new length.


    【分析】

    需要注意一点，不仅要return去重之后vector的长度，还要让vector的本身去重，也就是说，不能只得到长度而数组本身不去重。




    【代码】

    运行时间：62ms



```cpp
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.empty())
            return 0;
        
        int len = 1; 
        int cur = nums[0];
        for(int i = 1; i < nums.size(); i++)
        {
            if(cur == nums[i])
            {
                nums.erase(nums.begin() + i);
                i--;
                continue;
            }
            else
            {
                len++;
                cur = nums[i];
            }
        }
        return len;
    }
};
```





