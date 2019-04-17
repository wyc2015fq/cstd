# 283. Move Zeroes 移动零 - 别说话写代码的博客 - CSDN博客





2019年02月24日 20:49:47[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[Move Zeroes																[移动零																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=移动零&t=blog)



|Given an array `nums`, write a function to move all `0`'s to the end of it while maintaining the relative order of the non-zero elements.**Example:****Input:**`[0,1,0,3,12]`**Output:**`[1,3,12,0,0]`**Note**:- You must do this **in-place** without making a copy of the array.- Minimize the total number of operations.|给定一个数组 `nums`，编写一个函数将所有 `0` 移动到数组的末尾，同时保持非零元素的相对顺序。**示例:****输入:**`[0,1,0,3,12]`**输出:**`[1,3,12,0,0]`**说明**:- 必须在原数组上操作，不能拷贝额外的数组。- 尽量减少操作次数。|
|----|----|

思路：

**第一种**：i和j指针同时从前往后扫，i表示不为0 的元素，j指向0元素。当i指向的元素不为0时，nums[i]和nums[j]交换。

```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        for(int i=0,j=0;i<nums.size();++i)
            if(nums[i]) swap(nums[i],nums[j++]);
    }
};
```

**第二种**：将不为0 的元素从前往后赋值，另外一个变量统计0元素的个数，然后将后面 元素全部 置为0.

```cpp
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int k=0,n=nums.size();
        for(int i=0;i<n;++i) 
            if(nums[i]) nums[k++]=nums[i];
        for(int i=n-1;i>k-1;--i) nums[i]=0;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Move Zeroes&t=blog)




