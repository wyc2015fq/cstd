
# 414. Third Maximum Number - OraYang的博客 - CSDN博客

2017年08月11日 10:07:20[OraYang](https://me.csdn.net/u010665216)阅读数：138标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given anon-emptyarray of integers, return thethirdmaximum number in this array. If it does not exist, return the maximum number. The time complexity must be in O(n).
Example 1:
Input:[3, 2, 1]Output:1Explanation:The third maximum is 1.
Example 2:
Input:[1, 2]Output:2Explanation:The third maximum does not exist, so the maximum (2) is returned instead.
Example 3:
Input:[2, 2, 3, 1]Output:1Explanation:Note that the third maximum here means the third maximum distinct number.
Both numbers with value 2 are both considered as second maximum.思路：本题主要是集合函数set的使用，但是有个小细节要注意，当set中个数小于3时，直接返回最大值就好了。
代码：

```python
class Solution {
public:
    int thirdMax(vector<int>& nums) {
        set<int> num;
        for(int i =0;i<nums.size();i++)
            num.insert(nums[i]);
        set<int>::iterator itr = --num.end();
        for(int j =num.size()-1;j>=0;j--,itr--)
            if(j==num.size()-3)
                return *itr;       
        itr = --num.end();
        return *itr;       
    }
};
```


