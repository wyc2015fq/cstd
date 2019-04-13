
# 581. Shortest Unsorted Continuous Subarray - OraYang的博客 - CSDN博客

2017年08月27日 11:46:13[OraYang](https://me.csdn.net/u010665216)阅读数：353标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an integer array, you need to find onecontinuous subarraythat if you only sort this subarray in ascending order, then the whole array will be sorted in ascending order, too.
You need to find theshortestsuch subarray and output its length.
Example 1:
Input:[2, 6, 4, 8, 10, 9, 15]Output:5Explanation:You need to sort [6, 4, 8, 10, 9] in ascending order to make the whole array sorted in ascending order.
Note:
Then length of the input array is in range [1, 10,000].
The input array may contain duplicates, so ascending order here means<=.
思路：本题主要利用如下思路（简单直接）：
step1:先利用C++ sort函数进行升序排序，维护两个变量first及end,并赋予初始值0，-1（防止数组本来就是升序有序的情况）
step2:从数组尾部遍历数组,利用end记录升序后数组与原先数组第一个不相同的值的下标
step3:从数组头部遍历数组,利用first记录升序后数组与原先数组第一个不相同的值的下标
step4:返回end-first+1
代码：

```python
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        vector<int> res = nums;
        sort(res.begin(),res.end());
        int first=0,end=-1;
        for(int i = res.size()-1;i>=0;i--)
        {
            if(res[i]!=nums[i])
            {
                end = i;
                break;
            }
                
        }
        for(int j = 0;j<=end;j++)
        {
            if(res[j]!=nums[j])
            {
                first = j;
                break;
            }
                
        }
        return end-first+1;
    }
};
```


