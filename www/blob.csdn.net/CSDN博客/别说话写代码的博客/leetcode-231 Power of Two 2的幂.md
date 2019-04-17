# leetcode-231. Power of Two 2的幂 - 别说话写代码的博客 - CSDN博客





2019年03月11日 21:27:51[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：52标签：[2的幂																[Power of Two																[leetcode 231](https://so.csdn.net/so/search/s.do?q=leetcode 231&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Power of Two&t=blog)



|Given an integer, write a function to determine if it is a power of two.**Example 1:**```Input: 1Output: true Explanation: 20 = 1```**Example 2:**```Input: 16Output: trueExplanation: 24 = 16```**Example 3:**```Input: 218Output: false```|给定一个整数，编写一个函数来判断它是否是 2 的幂次方。**示例 1:**```输入: 1输出: true解释: 20 = 1```**示例 2:**```输入: 16输出: true解释: 24 = 16```**示例 3:**```输入: 218输出: false```|
|----|----|

思路：2的整数次方就是二进制中只有一个1的元素 

```cpp
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if(n>0 && !(n&(n-1))) return true;  //大于0并且二进制只有1个1
        return false;
    }
};
```](https://so.csdn.net/so/search/s.do?q=2的幂&t=blog)




