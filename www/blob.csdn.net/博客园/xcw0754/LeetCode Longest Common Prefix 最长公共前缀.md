# LeetCode Longest Common Prefix 最长公共前缀 - xcw0754 - 博客园
# [LeetCode Longest Common Prefix 最长公共前缀](https://www.cnblogs.com/xcw0754/p/4603457.html)
![](https://images0.cnblogs.com/blog2015/641737/201506/262339596747383.png)
题意：给多个字符串，返回这些字符串的最长公共前缀。
思路：直接逐个统计同一个位置上的字符有多少种，如果只有1种，那么就是该位是相同的，进入下一位比较。否则终止比较，返回前缀。可能有一个字符串会比较短，所以前缀最长也只是最短字符串的长度。
```
1 class Solution {
 2 public:
 3     string longestCommonPrefix(vector<string>& strs) {
 4         string ans="";
 5         if(strs.empty())    return ans;
 6         int has[129], j=0;
 7         while(1)
 8         {
 9             memset(has,0,sizeof(has));
10             for(int i=0; i<strs.size(); i++)
11             {
12                 if(j==strs[i].size())    return ans;
13                 has[strs[i][j]]++;
14             }
15             if(has[strs[0][j]]!=strs.size())    return ans;
16             ans+=strs[0][j++];
17         }
18     }
19 };
AC代码
```

