
# 561. Array Partition I - OraYang的博客 - CSDN博客

2017年08月26日 09:59:34[OraYang](https://me.csdn.net/u010665216)阅读数：360所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



本题：
Given an array of2nintegers, your task is to group these integers intonpairs of integer, say (a1,
 b1), (a2, b2),
 ..., (an, bn) which makes sum of min(ai,
 bi) for all i from 1 to n as large as possible.
Example 1:
Input:[1,4,3,2]Output:4Explanation:n is 2, and the maximum sum of pairs is 4 = min(1, 2) + min(3, 4).
Note:
nis a positive integer, which is in the range of [1, 10000].
All the integers in the array will be in the range of [-10000, 10000].
思路：本题求每组较小值得和，要保证所求的和最大，观察总结得出，只要将数组升序排序，遍历求和，每次i递增2
代码：

```python
class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int sum=0;
        for(int i=0;i<nums.size();i+=2)
        {
            sum+=nums[i];
        }
        return sum;
    }
};
```


