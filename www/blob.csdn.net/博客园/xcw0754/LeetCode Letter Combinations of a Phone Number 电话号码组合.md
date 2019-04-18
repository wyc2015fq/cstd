# LeetCode Letter Combinations of a Phone Number 电话号码组合 - xcw0754 - 博客园
# [LeetCode Letter Combinations of a Phone Number 电话号码组合](https://www.cnblogs.com/xcw0754/p/4677955.html)
题意：给一个电话号码，要求返回所有在手机上按键的组合，组合必须由键盘上号码的下方的字母组成。
思路：尼玛，一直RE，题意都不说0和1怎么办。DP解决。
```
1 class Solution {
 2 public:
 3     vector<string> ans;
 4     string str;
 5 
 6     void DFS(const string sett[], int siz, string t )
 7     {
 8         int n=str[siz]-'0';
 9         if(siz==str.size()){ans.push_back( t );return ;}
10         for(int i=0; i<sett[n].size(); i++)    DFS(sett, siz+1, t+sett[n][i] );
11     }
12 
13     vector<string> letterCombinations(string digits) {
14         if(digits.empty())  return vector<string> ();
15         str=digits;
16         const string mapp[]={"0","1","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
17         DFS(mapp, 0, "");
18         return ans;
19     }
20 };
AC代码
```

