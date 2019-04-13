
# 169. Majority Element - OraYang的博客 - CSDN博客

2017年07月27日 09:39:22[OraYang](https://me.csdn.net/u010665216)阅读数：178标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array of sizen, find the majority element. The majority element is the element that appearsmore than`⌊
 n/2 ⌋`times.
You may assume that the array is non-empty and the majority element always exist in the array.
思路：
本题乍一看是求最大频繁项，如果了解C++可以通过hash表来做详情请戳[点击打开链接](https://discuss.leetcode.com/topic/17446/6-suggested-solutions-in-c-with-explanations)，本题要抓住**more than**`⌊
 n/2 ⌋`times这
句话的意思，数组有7个数，频繁项有4个，数组有8个数频繁项有5个，即数组中一定存在大于等于2次的相邻值。理解这层意思后，代码就好写了。
代码：

```python
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int major = nums[0],count = 1;
        for(int i = 1;i<nums.size();i++)
        {
            if(count == 0)
            {
                count++;
                major = nums[i];
            }
            else if(major==nums[i])
                count++;
            else
                count--;
        }
        return major;
        
    }
};
```


