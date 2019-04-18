# LeetCode Valid Anagram （简单题） - xcw0754 - 博客园
# [LeetCode Valid Anagram （简单题）](https://www.cnblogs.com/xcw0754/p/4918769.html)
题意：
　　给出两个字符串s和t，判断串t是否为s打乱后的串。
思路：
　　如果返回的是true，则两个串的长度必定相等，所有字符出现的次数一样。那么可以统计26个字母的次数来解决，复杂度O(n)。也可以排序后逐个比对，复杂度O(nlogn)。
第一种方法：
```
1 class Solution {
 2 public:
 3     bool isAnagram(string s,string t)
 4     {
 5         if(s.size()!=t.size())    return false;
 6         int cnt[26][2]={0};
 7         for(int i=0; i<s.size(); i++)
 8             cnt[s[i]-'a'][0]++;
 9         for(int i=0; i<t.size(); i++)
10             cnt[t[i]-'a'][1]++;
11         for(int i=0; i<26; i++)
12             if(cnt[i][0]^cnt[i][1])
13                 return false;
14         return true;
15     }
16 };
AC代码
```
```
1 class Solution {
 2 public:
 3     bool isAnagram(string s,string t)
 4     {
 5         if(s.size()!=t.size())    return false;
 6         int cnt[26]={0};
 7         for(int i=0; i<s.size(); i++)    cnt[s[i]-'a']++,cnt[t[i]-'a']--;
 8         for(int i=0; i<26; i++)    if(cnt[i])    return false;
 9         return true;
10     }
11 };
AC代码
```
第二种方法：
```
1 class Solution {
 2 public:
 3     bool isAnagram(string s,string t)
 4     {
 5         if(s.size()!=t.size())    return false;
 6         sort(s.begin(),s.end());
 7         sort(t.begin(),t.end());
 8         for(int i=0; i<s.size(); i++)
 9             if(s[i]^t[i])    return false;
10         return true;
11     }
12 };
AC代码
```
```
1 bool isAnagram(string s,string t)
2 {
3     sort(s.begin(),s.end());
4     sort(t.begin(),t.end());    
5     return s==t;
6 }
AC代码
```
python3
```
1 class Solution(object):
 2     def isAnagram(self, s, t):
 3         """
 4         :type s: str
 5         :type t: str
 6         ::rtype: bool
 7         """
 8         listt=list(t)
 9         for x in s:
10             try:
11                 listt.remove(x)
12             except ValueError:
13                 return False
14         return True if listt==[] else False
AC代码
```
```
1 class Solution(object):
 2     def isAnagram(self, s, t):
 3         """
 4         :type s: str
 5         :type t: str
 6         ::rtype: bool
 7         """
 8         dic1, dic2= {}, {}
 9         for x in s:
10             dic1[x]=dic1.get(x,0)+1
11         for x in t:
12             dic2[x]=dic2.get(x,0)+1
13         return dic1==dic2
AC代码
```
```
1 class Solution(object):
 2     def isAnagram(self, s, t):
 3         """
 4         :type s: str
 5         :type t: str
 6         ::rtype: bool
 7         """
 8         cnt1, cnt2= [0]*26, [0]*26
 9         for x in s:
10             cnt1[ord(x)-ord('a')]+=1
11         for x in t:
12              cnt2[ord(x)-ord('a')]+=1
13         return cnt1==cnt2
AC代码
```
```
1 class Solution(object):
2     def isAnagram(self, s, t):
3         """
4         :type s: str
5         :type t: str
6         ::rtype: bool
7         """
8         return sorted(s)==sorted(t)
AC代码
```

