
# 665. Non-decreasing Array - OraYang的博客 - CSDN博客

2017年08月31日 23:31:51[OraYang](https://me.csdn.net/u010665216)阅读数：530标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array with`n`integers, your task is to check if it could become non-decreasing
 by modifyingat most`1`element.
We define an array is non-decreasing if`array[i] <= array[i + 1]`holds for every`i`(1
 <= i < n).
Example 1:
Input:[4,2,3]Output:TrueExplanation:You could modify the first
```python
4
```
to
```python
1
```
to get a non-decreasing array.
Example 2:
Input:[4,2,1]Output:FalseExplanation:You can't get a non-decreasing array by modify at most one element.
Note:The`n`belongs to [1, 10,000].
思路：
本题思路如下：
step1:复制数组nums得nums_c
step2:遍历数组，当当前值小于前一个值时，对两个数组做删除操作，nums删除当前值，nums_c删除前面一个值
step3:分别遍历上述两个数组，当两个数组只要有一个符合非递减返回真，否则返回假
代码：

```python
class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        vector<int> nums_c = nums;
        int flag = 0,flag_c=0;
        for(int i=1;i<nums.size();i++)
        {
            if(nums[i]<nums[i-1])
            {
                nums.erase(nums.begin()+i-1);
                nums_c.erase(nums_c.begin()+i);
                break;
            }
        }
        if(count==1)
        {
            for(int j=1;j<nums.size();j++)
        {
            if(nums[j]<nums[j-1])
            {
                flag=1;
                break;
            } 
        }
             for(int j=1;j<nums.size();j++)
        {
            if(nums_c[j]<nums_c[j-1])
            {
                flag_c =1;
                break;
            }
        }
        }
        if(!flag||!flag_c)
            return true;
        else 
            return false;
    }
};
```



