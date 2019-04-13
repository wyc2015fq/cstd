
# 6. ZigZag Conversion - OraYang的博客 - CSDN博客

2017年09月04日 13:52:25[OraYang](https://me.csdn.net/u010665216)阅读数：123标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目:
The string`"PAYPALISHIRING"`is written in a zigzag pattern on a given number of rows like
 this: (you may want to display this pattern in a fixed font for better legibility)
P   A   H   N
A P L S I I G
Y   I   RAnd then read line by line:`"PAHNAPLSIIGYIR"`
Write the code that will take a string and make this conversion given a number of rows:
string convert(string text, int nRows);`convert("PAYPALISHIRING", 3)`should
 return`"PAHNAPLSIIGYIR"`.
思路：
本题感觉题意上的理解就会使很多人陷入误区，首先分析题意的核心词汇是：written
 in a zigzag pattern on a given number of rows)在给定的行数上以锯齿形的方式写**)，**所以上述例子可以写成这样：
P.......A........H.......N
..A..P....L..S....I...I....G
....Y.........I........R
这样一来，就有思路了，可以先维护一个大小为rowNums的字符串数组，再遍历数组，设置标志位row及步长step，当row=0时，step置1，row+=step;当row=rowNums-1时，step置-1，row+=step，同时存储字符串，str[row].push_back;最后将字符串数组首尾相连。
代码：

```python
class Solution {
public:
    string convert(string s, int numRows) {
        if(numRows<=1)
            return s;
        vector<string> str(numRows,"");
        int step,row=0;
        for(int i=0;i<s.size();i++)
        {
            str[row].push_back(s[i]);
            if(row==0)
                step=1;
            else if(row == numRows-1)
                step=-1;
            row+=step;
        }
        s.clear();
        for(int j=0;j<numRows;j++)
        {
            s.append(str[j]);
        }
        return s;
    }
};
```



