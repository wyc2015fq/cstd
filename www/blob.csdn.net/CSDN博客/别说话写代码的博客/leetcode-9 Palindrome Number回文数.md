# leetcode-9: Palindrome Number回文数 - 别说话写代码的博客 - CSDN博客





2018年12月14日 14:41:33[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：35标签：[leetcode 9																[Palindrome Number																[leetcode 回文数](https://so.csdn.net/so/search/s.do?q=leetcode 回文数&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Palindrome Number&t=blog)




题目：
|Determine whether an integer is a palindrome. An integer is a palindrome when it reads the same backward as forward.**Example 1:**```Input: 121Output: true```**Example 2:**```Input: -121Output: falseExplanation: From left to right, it reads -121. From right to left, it becomes 121-. Therefore it is not a palindrome.```**Example 3:**```Input: 10Output: falseExplanation: Reads 01 from right to left. Therefore it is not a palindrome.```**Follow up:**Coud you solve it without converting the integer to a string?|判断一个整数是否是回文数。回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。**示例 1:**```输入: 121输出: true```**示例 2:**```输入: -121输出: false解释: 从左向右读, 为 -121 。 从右向左读, 为 121- 。因此它不是一个回文数。```**示例 3:**```输入: 10输出: false解释: 从右向左读, 为 01 。因此它不是一个回文数。```|
|----|----|

思路：很简单，如果是负数直接返回false，否则用一个数字表示他的回文，只需要判断他们俩是否相等

```cpp
class Solution {
public:
    bool isPalindrome(int x) {
        if(x<0) return false;
        int ret=0,y=x;
        while(y!=0)
        {
            ret = ret*10+(y%10);
            y/=10;
        }
        if(ret == x) return true;
        else return false;
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode 9&t=blog)




