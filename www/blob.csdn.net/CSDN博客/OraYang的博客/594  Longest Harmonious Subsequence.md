
# 594. Longest Harmonious Subsequence - OraYang的博客 - CSDN博客

2017年08月28日 11:34:16[OraYang](https://me.csdn.net/u010665216)阅读数：183标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
We define a harmonious array is an array where the difference between its maximum value and its minimum value isexactly1.
Now, given an integer array, you need to find the length of its longest harmonious subsequence among all its possible[subsequences](https://en.wikipedia.org/wiki/Subsequence).
Example 1:
Input:[1,3,2,2,5,2,3,7]Output:5Explanation:The longest harmonious subsequence is [3,2,2,2,3].
Note:The length of the input array will not exceed 20,000.
思路：
本题分为以下几个步骤：
step1:对数组进行升序排序
step2:利用map存储排序后的数组对应的值和出现的次数
step3:遍历数组求差值是否为1，并维护两个变量当前计数count，及最大值max
代码：

```python
class Solution {
public:
    int findLHS(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int count =0,max=0;
        map<int,int> mp;
        if(nums.size()<=1)
            return 0;
        for(int i =0;i<nums.size();i++)
        {
            mp[nums[i]]++;
        }
        map<int,int>::iterator itr = mp.begin(),tr=mp.begin();
        cout<<itr->first<<endl;
        itr++;
        while(itr!=mp.end())
        {
            
            if((itr->first-tr->first)==1)
            {
                count =itr->second+tr->second;
                cout<<count<<endl;
                max = max>count?max:count;
                count = 0;
            }
            itr++;
            tr++;
        }
        max = max>count?max:count;
        return max;
        
    }
};
```


