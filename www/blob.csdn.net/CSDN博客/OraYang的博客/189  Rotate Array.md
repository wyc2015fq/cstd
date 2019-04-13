
# 189. Rotate Array - OraYang的博客 - CSDN博客

2017年07月28日 10:33:39[OraYang](https://me.csdn.net/u010665216)阅读数：173标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Rotate an array ofnelements to the right byksteps.
For example, withn= 7 andk= 3, the array`[1,2,3,4,5,6,7]`is
 rotated to`[5,6,7,1,2,3,4]`.
Note:
Try to come up as many solutions as you can, there are at least 3 different ways to solve this problem.
思路：
讲道理这道题目其实是题意不明的，由于测试集中k的大小是大于数组大小的，所以k本质上是k%nums.size()
代码：

```python
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        vector<int> t = nums;
        for (int i = 0; i < nums.size(); ++i) {
            nums[(i + k) % nums.size()] = t[i];
        }
    }
};
```


