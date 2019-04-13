
# 551. Student Attendance Record I - OraYang的博客 - CSDN博客

2017年08月26日 09:30:08[OraYang](https://me.csdn.net/u010665216)阅读数：279标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are given a string representing an attendance record for a student. The record only contains the following three characters:
'A': Absent.
'L': Late.
'P': Present.
A student could be rewarded if his attendance record doesn't containmore than one 'A' (absent)ormore than two continuous 'L'
 (late).
You need to return whether the student could be rewarded according to his attendance record.
Example 1:
Input:"PPALLP"Output:True
Example 2:
Input:"PPALLL"Output:False
思路：
本题思路很简单就是简单的逻辑判断，遍历数组，维护两个变量，count_A,count_L,如果当前字符是‘A'，count_A++,如果当前字符是’L',count_L++,如果当前字符是A,P，count_L置0。如果count_A>1或count_L>2就返回false否则返回true。
代码：

```python
class Solution {
public:
    bool checkRecord(string s) {
        int count_A =0, count_L =0;
        for(int i=0;i<s.size();i++)
        {
            if(s[i]=='A'){
                count_A++;
                count_L = 0;
            }
            if(s[i]=='P')
                count_L=0;
            if(s[i]=='L')
                count_L++;
            if(count_A>1||count_L>2)
                return false;
        }
        return true;
    }
};
```




