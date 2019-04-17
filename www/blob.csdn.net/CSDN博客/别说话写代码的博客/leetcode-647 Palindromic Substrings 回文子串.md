# leetcode-647. Palindromic Substrings 回文子串 - 别说话写代码的博客 - CSDN博客





2019年03月21日 21:30:07[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：9
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)








|Given a string, your task is to count how many palindromic substrings in this string.The substrings with different start indexes or end indexes are counted as different substrings even they consist of same characters.**Example 1:****Input:** "abc"**Output:** 3**Explanation:** Three palindromic strings: "a", "b", "c".**Example 2:****Input:** "aaa"**Output:** 6**Explanation:** Six palindromic strings: "a", "a", "a", "aa", "aa", "aaa".**Note:**- The input string length won't exceed 1000.|给定一个字符串，你的任务是计算这个字符串中有多少个回文子串。具有不同开始位置或结束位置的子串，即使是由相同的字符组成，也会被计为是不同的子串。**示例 1:****输入:** "abc"**输出:** 3**解释:** 三个回文子串: "a", "b", "c".**示例 2:****输入:** "aaa"**输出:** 6**说明:** 6个回文子串: "a", "a", "a", "aa", "aa", "aaa".**注意:**- 输入的字符串长度不会超过1000。|
|----|----|

思路：对每个字符，都判断一下所有可能的回文子串，包括长度为奇数和偶数。

```cpp
class Solution {
public:
    int countSubstrings(string s) {
        if(s.empty()) return 0;
        int n=s.size(),res=0;
        for(int i=0;i<n;++i)
        {
            tra(s,i,i,res);   //奇数个
            tra(s,i,i+1,res);   //偶数个
        }
        return res;
        
    }
    void tra(string &s,int i,int j,int &res)
    {
        while(i>=0&&j<s.size()&&s[i]==s[j])
        {
            i--; j++;
            res++;
        }
    }
};
```





