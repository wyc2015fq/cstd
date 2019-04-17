# 287. Find the Duplicate Number 寻找重复数 - 别说话写代码的博客 - CSDN博客





2019年02月24日 21:14:55[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：21标签：[Find the Duplicate Number 																[寻找重复数																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=寻找重复数&t=blog)



|Given an array *nums* containing *n* + 1 integers where each integer is between 1 and *n* (inclusive), prove that at least one duplicate number must exist. Assume that there is only one duplicate number, find the duplicate one.**Example 1:****Input:**`[1,3,4,2,2]`**Output:** 2**Example 2:****Input:** [3,1,3,4,2]**Output:** 3**Note:**- You **must not** modify the array (assume the array is read only).- You must use only constant, *O*(1) extra space.- Your runtime complexity should be less than *O*(*n*2).- There is only one duplicate number in the array, but it could be repeated more than once.|给定一个包含 *n* + 1 个整数的数组 *nums*，其数字都在 1 到 *n *之间（包括 1 和 *n*），可知至少存在一个重复的整数。假设只有一个重复的整数，找出这个重复的数。**示例 1:****输入:**`[1,3,4,2,2]`**输出:** 2**示例 2:****输入:** [3,1,3,4,2]**输出:** 3**说明：**- **不能**更改原数组（假设数组是只读的）。- 只能使用额外的 *O*(1) 的空间。- 时间复杂度小于 *O*(*n*2) 。- 数组中只有一个重复的数字，但它可能不止重复出现一次。|
|----|----|

思路：题目要求不能修改原数组，不能增加空间，时间小于O(n)，但是题目给出数字都是1~n 之间。所以联想 到二分法。在这里二分法的思路是，mid=left+(right-left)/2;然后定义一个 统计<=mid的数字出现的次数。如果<=mid的数字出现的次数小于mid，那就说明重复元素在 大于mid的范围内，将right=mid+1；否则，说明重复元素在0~mid范围内，那就right=mid;

```cpp
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int left=0,right=nums.size();
        while(left<right)
        {
            int mid = left + (right-left)/2,count=0;
            for(auto n:nums) 
                if(n<=mid) count++;
            if(count<=mid) left=mid+1;
            else right=mid;
        }return right;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Find the Duplicate Number &t=blog)




