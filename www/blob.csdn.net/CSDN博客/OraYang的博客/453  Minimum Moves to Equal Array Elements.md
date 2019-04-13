
# 453. Minimum Moves to Equal Array Elements - OraYang的博客 - CSDN博客

2017年08月17日 08:49:09[OraYang](https://me.csdn.net/u010665216)阅读数：187标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given anon-emptyinteger array of sizen, find the minimum number of moves required to make all array elements equal, where a move is incrementingn- 1 elements by 1.
Example:
Input:[1,2,3]Output:3Explanation:Only three moves are needed (remember each move increments two elements):
[1,2,3]  =>  [2,3,3]  =>  [3,4,3]  =>  [4,4,4]思路：本题很简单，问题是大小为n的数组每次只能n-1个数加一，问几次可以累加和到所有数组数字相等，本文采用归纳推导的方法，具体推导思路如下（注意数组中最小值，每次都必定参与累加计数）：
step1:如果数组大小为1，返回0；
step2:如果数组大小为2，a,b(a为最小值,a+X=b,X=b-a),那么返回b-a;
step3:如果数组大小为3，a,b,c(a为最小值，a加b-a次才能等于b,a加c-a次才能等于a,那么a一共加b-a+c-a次才能是数组想等),那么返回b-a+c-a
....
step4:如果数组大小为n，a,b,c,d...n,返回b-a+c-a+d-a...+n-a
代码：

```python
class Solution {
public:
    int minMoves(vector<int>& nums) {
        int minnum = nums[0],res =0;
        for(int num : nums)
            minnum = min(minnum,num);
        for(int num : nums)
            res+=num-minnum;
        return res;
    }
};
```


