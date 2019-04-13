
# 303. Range Sum Query - Immutable - OraYang的博客 - CSDN博客

2017年08月05日 12:38:48[OraYang](https://me.csdn.net/u010665216)阅读数：188所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an integer arraynums, find the sum of the elements between indicesiandj(i≤j),
 inclusive.
Example:
Given nums = [-2, 0, 3, -5, 2, -1]
sumRange(0, 2) -> 1
sumRange(2, 5) -> -1
sumRange(0, 5) -> -3
Note:
You may assume that the array does not change.
There are many calls tosumRangefunction.
思路：
本题很简单，就考察了类的使用
代码：

```python
class NumArray {
public:
    NumArray(vector<int> nums) {
        accu.push_back(0);
        for (int num : nums)
            accu.push_back(accu.back() + num);
    }
    int sumRange(int i, int j) {
        return accu[j + 1] - accu[i];
    }
private:
    vector<int> accu;
};
```




