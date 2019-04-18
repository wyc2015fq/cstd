# Longest Common Prefix ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月07日 08:08:42[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：960











**    【题目】**

    Write a function to find the longest common prefix string amongst an array of strings.




**    【分析】**

     公共前缀指的是所有字符串的前缀都相同。显然，这个最长公共前缀的长度不会超过所有字符串中最短的那个。

    我们先求得最短串长minLen，然后遍历所有字符串中的前minLen是否相等。




**    【代码】**

    运行时间：7ms



```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(strs.empty())
            return "";
            
        int minLen = strs[0].length(); 
        for(int i = 0; i < strs.size(); i++)
        {
            minLen = minLen < strs[i].length() ? minLen : strs[i].length();
            //minLen = min(minLen, strs[i].size());  // obtain the minimal length
        }
        
        string res = "";
        bool same = true;
        for(int j = 0; same && j < minLen; j++)
        {
            for(int k = 1; k < strs.size(); k++)
            {
                if(strs[k][j] != strs[0][j])
                {
                    same = false;
                    break;
                }
            }
            if(same)
                res = res + strs[0][j];  // after the loop, res should ++
        }
        return res;
    }
};
```





