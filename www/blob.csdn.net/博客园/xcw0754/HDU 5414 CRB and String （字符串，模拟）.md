# HDU 5414  CRB and String （字符串，模拟） - xcw0754 - 博客园
# [HDU 5414  CRB and String （字符串，模拟）](https://www.cnblogs.com/xcw0754/p/4745907.html)
题意：给两个字符串s和t，如果能插入一些字符使得s=t，则输出yes，否则输出no。插入规则：在s中选定一个字符c，可以在其后面插入一个字符k，只要k!=c即可。
思路：特殊的情况就是s和t的最长相同字符的前缀不等长，则肯定插不进去，如s="aab"，而t="aaab"。还有就是，如果第一个字符就不相同了，输出NO，因为不能在最前面插入（看定义）！
　　只要将这个情况排除掉，其他的就好做了，从后面开始，匹配就往前扫，不匹配就在s中插入对应字符，如果s长度超过了t就输出NO，否则到最后肯定YES。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x7f7f7f7f
 4 #define LL long long
 5 using namespace std;
 6 const int N=100050;
 7 char s[N];
 8 string s1, s2;
 9 bool cal(int len1,int len2)
10 {
11     if( s1[0]!=s2[0] )  return false;
12     if(len1==len2 && s1==s2 )   return true;
13     if(len1==len2 && s1!=s2 )  return false;
14 
15     char c=s1[0];
16     int i;
17     for(i=0;  i<s1.size() && s1[i]==s2[i] && s1[i]==s1[0] ; i++ );//去掉前缀
18     if(s2[i]==c)    return false;   //无解
19 
20     if(i==s1.size())    s1="";
21     else                s1=s1.substr(i);
22     s2=s2.substr(i);
23 
24     for(int i,j; !s2.empty()  ; )
25     {
26         i=s2.size()-1;
27         j=s1.size()-1;
28         if( s1.empty() )
29         {
30             s1.push_back( s2[i]);
31             continue;
32         }
33         else if( s1[j]==s2[i] )  //相同就删
34         {
35             s1.erase(s1.end()-1);
36             s2.erase(s2.end()-1);
37         }
38         else        //不同就插
39         {
40             s1.push_back( s2[i]);
41         }
42         if(s1.size()>s2.size()) return false; 
43     }
44     return true;
45 }
46 
47 int main()
48 {
49     //freopen("input.txt", "r", stdin);
50     int t;
51     cin>>t;
52     while(t--)
53     {
54         scanf("%s", s );
55         s1=s;
56         scanf("%s", s );
57         s2=s;
58 
59         if( cal(s1.size(),  s2.size()) )
60             puts("Yes");
61         else
62             puts("No");
63     }
64     return 0;
65 }
AC代码
```

