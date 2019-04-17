# 169. Majority Element 求众数 - 别说话写代码的博客 - CSDN博客





2019年02月24日 20:29:19[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：21标签：[Majority Element																[求众数																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=求众数&t=blog)



|Given an array of size *n*, find the majority element. The majority element is the element that appears **more than**`⌊ n/2 ⌋`times.You may assume that the array is non-empty and the majority element always exist in the array.**Example 1:****Input:** [3,2,3]**Output:** 3**Example 2:****Input:** [2,2,1,1,1,2,2]**Output:** 2|给定一个大小为 *n *的数组，找到其中的众数。众数是指在数组中出现次数**大于**`⌊ n/2 ⌋` 的元素。你可以假设数组是非空的，并且给定的数组总是存在众数。**示例 1:****输入:** [3,2,3]**输出:** 3**示例 2:****输入:** [2,2,1,1,1,2,2]**输出:** 2|
|----|----|

思路：令res为第1个元素，count为res出现的次数。然后从第2个元素 开始，若当前元素为res，则count++，否则count--。 如果count为0了，那就 另res=nums[i]，然后count=1；一直遍历完以后，res就是出现次数最多的（按道理说其实还需要检查一下是不是满足出现一半以上的次数，但这个题不用）

```cpp
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        if(nums.empty()) return 0;
        int res=nums[0],count=1;
        for(int i=1;i<nums.size();++i)
        {
            if(res==nums[i]) count++;
            else count--;
            if(count==0) 
            {
                res=nums[i];
                count=1;
            }
        }return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Majority Element&t=blog)




