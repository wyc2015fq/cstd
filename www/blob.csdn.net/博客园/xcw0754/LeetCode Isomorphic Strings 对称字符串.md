# LeetCode  Isomorphic Strings  对称字符串 - xcw0754 - 博客园
# [LeetCode  Isomorphic Strings  对称字符串](https://www.cnblogs.com/xcw0754/p/4504798.html)
![](https://images0.cnblogs.com/blog2015/641737/201505/142320010326494.png)
题意：如果两个字符串是对称的，就返回true。对称就是将串1中的同一字符都一起换掉，可以换成同串2一样的。
思路：ASCII码表哈希就行了。需要扫3次字符串，共3*n的计算量。复杂度O（n）。从串左开始扫，若字符没有出现过，则赋予其一个特定编号，在哈希表中记录，并将该字符改成编号。对串2同样处理。其实扫2次就行了，但是为了短码。
```
1 class Solution {
 2 public:
 3     void cal(string &s)
 4     {
 5         int has[129]={0}, cnt=0;
 6         for(int i=0; i<s.size(); i++)
 7         {
 8             if(!has[s[i]])
 9                 has[s[i]]=++cnt;
10             s[i]=has[s[i]];
11         }
12     }
13     
14     bool isIsomorphic(string s, string t) {
15         if(s.size()!=t.size())    return false;
16         if(s.size()==1)    return true;
17         cal(s);
18         cal(t);
19         for(int i=0; i<s.size(); i++)
20             if(s[i]!=t[i])    return false;
21         return true;
22     }
23 };
AC代码
```

