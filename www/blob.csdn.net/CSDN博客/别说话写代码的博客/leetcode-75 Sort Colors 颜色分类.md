# leetcode-75. Sort Colors 颜色分类 - 别说话写代码的博客 - CSDN博客





2019年01月28日 21:58:12[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：85标签：[颜色分类																[sort colors																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=sort colors&t=blog)



|Given an array with *n* objects colored red, white or blue, sort them **[in-place](https://en.wikipedia.org/wiki/In-place_algorithm)**so that objects of the same color are adjacent, with the colors in the order red, white and blue.Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.**Note:** You are not suppose to use the library's sort function for this problem.**Example:****Input:** [2,0,2,1,1,0]**Output:** [0,0,1,1,2,2]**Follow up:**- A rather straight forward solution is a two-pass algorithm using counting sort.				First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array with total number of 0's, then 1's and followed by 2's.- Could you come up with a one-pass algorithm using only constant space?|给定一个包含红色、白色和蓝色，一共 *n *个元素的数组，**[原地](https://baike.baidu.com/item/%E5%8E%9F%E5%9C%B0%E7%AE%97%E6%B3%95)**对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。此题中，我们使用整数 0、 1 和 2 分别表示红色、白色和蓝色。**注意:**			不能使用代码库中的排序函数来解决这道题。**示例:****输入:** [2,0,2,1,1,0]**输出:** [0,0,1,1,2,2]**进阶：**- 一个直观的解决方案是使用计数排序的两趟扫描算法。				首先，迭代计算出0、1 和 2 元素的个数，然后按照0、1、2的排序，重写当前数组。- 你能想出一个仅使用常数空间的一趟扫描算法吗？|
|----|----|

思路：

**第一种**：题目中说的，迭代算出0,1,2分别出现了 多少次，然后按照0,1,2分别出现的 次数复制到原数组 中

```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int arr[3]={0},k=0;
        for(int i=0;i<nums.size();++i)
            arr[nums[i]]++;
        for(int i=0;i<3;++i)
            for(int j=0;j<arr[i];++j)
                nums[k++]=i;
    }
};
```

第二种：因为是红白蓝排序，所以使用两个指针，red指针从前往后，blue指针从后往前。若遇到为0的，red指针位置与其交换，若遇到为2的，blue指针与其交换，若遇到为1的，不用管。例如：[2,0,2,1,1,0]，首先red 指针指向0位置的2，blue指针指向5 位置的0

[2,0,2,1,1,0]    

[0,0,2,1,1,2]     从前往后遍历为2，blue位置与其交换，

[0,0,2,1,1,2]    

[0,0,1,1,2,2]     遍历为2，与blue位置交换

[0,0,1,1,2,2]

```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int red=0, blue=nums.size()-1;
        for(int i=0;i<=blue;++i)   //注意这里i<=blue
        {
            if(nums[i]==0) swap(nums[i],nums[red++]);
            else if(nums[i]==2) swap(nums[i--],nums[blue--]);  //记得交换之后i--,从i前一个位置开始
        }
    }
};
```](https://so.csdn.net/so/search/s.do?q=颜色分类&t=blog)




