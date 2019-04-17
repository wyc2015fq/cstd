# lintcode-508. Wiggle Sort - 别说话写代码的博客 - CSDN博客





2019年02月21日 17:44:15[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：15标签：[Wiggle Sort																[lintcode](https://so.csdn.net/so/search/s.do?q=lintcode&t=blog)](https://so.csdn.net/so/search/s.do?q=Wiggle Sort&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)







|Given an unsorted array `nums`, reorder it **in-place** such that`nums[0] <= nums[1] >= nums[2] <= nums[3]....`### Example**Example 1:**```Input: [3, 5, 2, 1, 6, 4]Output: [1, 6, 2, 5, 3, 4]Explanation: This question may have multiple answers, and [2, 6, 1, 5, 3, 4] is also ok.```**Example 2:**```Input: [1, 2, 3, 4]Output: [2, 1, 4, 3]```|
|----|



从左到右 扫一遍，如果不满足条件就交换

```cpp
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        if(nums.size()<2) return ;
        for(int i=1;i<nums.size();++i)
        {
            if( ((i&1==0)&&nums[i]>nums[i-1]) ||
                ((i&1==1)&&nums[i]<nums[i-1]) )
                swap(nums[i],nums[i-1]);
        }
    }
};
```






