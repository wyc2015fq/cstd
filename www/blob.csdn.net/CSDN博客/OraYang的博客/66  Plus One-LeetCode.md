
# 66. Plus One - LeetCode - OraYang的博客 - CSDN博客

2017年07月14日 09:02:57[OraYang](https://me.csdn.net/u010665216)阅读数：284所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a non-negative integer represented as anon-emptyarray of digits, plus one to the integer.
You may assume the integer do not contain any leading zero, except the number 0 itself.
The digits are stored such that the most significant digit is at the head of the list.
思路：
题目本质上是简单的大数加法，只需要遍历数组，注意逢9进1即可
代码：
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int num = digits.size();
        int i;
        for(i = num-1;i>=0;i--)
        {
            if(digits[i]==9)
                digits[i] = 0;
            else
            {
                digits[i]++;
                return digits;
            }
        }
        if(i=-1)
        {
            digits[0]=1;
            digits.push_back(0);
            return digits;
        }
    }
};Tips:在运行**Custom Testcase**时，如果输入是[999999]，expect
 answer会报错：Line 17: std::vector<int> Solution::plusOne(std::vector<int>&): Assertion `carry == 0' failed.，这个问题已经提交了



