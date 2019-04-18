# Remove Element ——结题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月12日 13:39:43[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：621











     【题目】




Given an array and a value, remove all instances of that value in place and return the new length.


The order of elements can be changed. It doesn't matter what you leave beyond the new length.


    【分析】

    注意在遍历的时候要变长。




    【代码】






```cpp
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        if(nums.size() < 1)
            return 0;
        int len = nums.size();
        
        for(int i = 0 ; i < len; i++)
        {
            if(nums[i] != val)
                continue;
            else
            {
                len--;
                nums.erase(nums.begin() + i);
                i--;
            }
        }
        return len;
    }
};
```








