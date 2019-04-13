
# 541. Reverse String II - OraYang的博客 - CSDN博客

2017年08月26日 08:54:04[OraYang](https://me.csdn.net/u010665216)阅读数：352所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a string and an integer k, you need to reverse the first k characters for every 2k characters counting from the
 start of the string. If there are less than k characters left, reverse all of them. If there are less than 2k but greater than or equal to k characters, then reverse the first k characters and left the other as original.
Example:
Input:s = "abcdefg", k = 2Output:"bacdfeg"
Restrictions:The string consists of lower English letters only.
Length of the given string and k will in the range [1, 10000]
思路：本题，目的是每隔2k个字符，反转前k个字符，如果剩余不足k个就全部反转。
代码：

```python
class Solution {
public:
    string reverseStr(string s, int k) {
        for(int i =0;i<s.size();i+=2*k)
        {
            reverse(s.begin()+i,min(s.begin()+i+k,s.end()));
        }
        return s;
    }
};
```


