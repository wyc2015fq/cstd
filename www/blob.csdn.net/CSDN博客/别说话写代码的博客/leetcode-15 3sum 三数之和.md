# leetcode-15:3sum 三数之和 - 别说话写代码的博客 - CSDN博客





2018年12月18日 11:33:31[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：34标签：[leetcode 15																[三数之和																[3sum](https://so.csdn.net/so/search/s.do?q=3sum&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=三数之和&t=blog)




题目：
|Given an array `nums` of *n* integers, are there elements *a*, *b*, *c* in `nums` such that *a*+ *b* + *c* = 0? Find all unique triplets in the array which gives the sum of zero.**Note:**The solution set must not contain duplicate triplets.**Example:**Given array nums = [-1, 0, 1, 2, -1, -4],A solution set is:[  [-1, 0, 1],  [-1, -1, 2]]|给定一个包含 *n* 个整数的数组 `nums`，判断 `nums` 中是否存在三个元素 *a，b，c ，*使得 *a + b + c = *0 ？找出所有满足条件且不重复的三元组。**注意：**答案中不可以包含重复的三元组。例如, 给定数组 nums = [-1, 0, 1, 2, -1, -4]，满足要求的三元组集合为：[  [-1, 0, 1],  [-1, -1, 2]]|
|----|----|

思路：先对原数组排序，然后遍历排序后的数组。在循环内可以做如下优化：1.若当前遍历的 数是正数，就 break;因为当前数组已经是有序，后面的数字肯定是正数，和就不会是0,；2.加上重复跳过功能，，若第2个数与前面数相等则跳过。

我们使target=0-遍历过的数，然后查看剩下的元素是否为target。我们使用两个指针，i从当前遍历元素下一个开始从前往后，j从后往前，若两个数之和刚好为target，将当前元素，i和j一起放入ret，然后两个指针跳过重复元素。若两数之和小于target，则左指针向右移动一位，若大于target，j向左移动一位。

注意：1.若vector.size()需要用到两次以上，最好定义一个变量来保存vector的size()，这会省时间。

2.不要在循环体内定义变量，这也会造成浪费时间。

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> ret;
        sort(nums.begin(),nums.end());
        if(nums.empty()) return {};
        int len=nums.size(),i,j,target;
        for(int k=0;k<len;++k)
        {
            if(nums[k]>0) break;
            if(k>0 && nums[k]==nums[k-1]) continue;
            i=k+1;
            j=len-1;
            target = 0-nums[k];
            while(i<j)
            {
                if(nums[i]+nums[j]==target)
                {
                    ret.push_back({nums[k],nums[i],nums[j]});
                    while(i<j && nums[j]==nums[j-1]) j--;
                    while(i<j && nums[i]==nums[i+1]) i++;
                    j--;i++;
                }else if(nums[i]+nums[j]<target) i++;
                else j--;
            }
        }
        return ret;     
    }
};
```

参考：

[http://www.cnblogs.com/grandyang/p/4481576.html](http://www.cnblogs.com/grandyang/p/4481576.html)](https://so.csdn.net/so/search/s.do?q=leetcode 15&t=blog)




