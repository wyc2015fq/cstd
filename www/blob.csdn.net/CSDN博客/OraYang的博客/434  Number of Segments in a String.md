
# 434. Number of Segments in a String - OraYang的博客 - CSDN博客

2017年08月13日 14:07:19[OraYang](https://me.csdn.net/u010665216)阅读数：95标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Count the number of segments in a string, where a segment is defined to be a contiguous sequence of non-space characters.
Please note that the string does not contain anynon-printablecharacters.
Example:
Input:"Hello, my name is John"Output:5
思路：
本题，利用c++标准库函数，其中要注意的是标点符号后面还会有空格，所以直接忽略标点符号。
代码：

```python
class Solution {
public:
    int countSegments(string s) {
        int sum=0;
        if(s.size()==0)
            return 0;
        for(int i=0;i<s.size();i++)
        {
            if(isspace(s[i]))
                sum++;
        }
        return sum+1;
    }
};
```





