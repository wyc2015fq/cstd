# LeetCode  Word Pattern （模拟） - xcw0754 - 博客园
# [LeetCode  Word Pattern （模拟）](https://www.cnblogs.com/xcw0754/p/4934299.html)
题意：
　　给出一个模式串pattern，再给出一个串str，问str的模板是否是pattern。
思路：
　　注意点：只要对于所有pattern[i]相同的i，str中对应的所有words[i]也必须相同，反过来，一个words[i]对应的也只有一个pattern[i]。
　　乱搞：
```
1 class Solution {
 2 public:
 3     bool wordPattern(string pattern, string str) {
 4         set<string> sett[26];
 5         map<string,char> mapp;
 6         string t;char c;int pos=0;
 7         for(int i=0; i<str.size(); i++,pos++)
 8         {
 9             if(pattern.size()==pos)    return false;
10             t="";
11             c=pattern[pos];
12             while(i<str.size()&&str[i]==' ')    i++;
13             while(i<str.size()&&str[i]!=' ')     t+=str[i++];
14             sett[c-'a'].insert(t);
15             if(sett[c-'a'].size()>1)    return false;
16             if(!mapp[t])    mapp[t]=c;
17             else    if(mapp[t]!=c)    return false;
18         }
19         if(pos!=pattern.size())    return false;
20         return true;
21     }
22 };
AC代码
```
　　用流：
```
1 class Solution {
 2 public:
 3     bool wordPattern(string pattern, string str) {
 4         stringstream ss(str);
 5         set<string> sett[26];
 6         map<string,char> mapp;
 7         string t;char c;int pos=0;
 8         while(getline(ss,t,' '))
 9         {
10             if(pattern.size()==pos)    return false;
11             c=pattern[pos++];
12             sett[c-'a'].insert(t);
13             if(sett[c-'a'].size()>1)    return false;
14             if(!mapp[t])    mapp[t]=c;
15             if(mapp[t]!=c)    return false;
16         }
17         return pos==pattern.size();
18     }
19 };
AC代码
```

