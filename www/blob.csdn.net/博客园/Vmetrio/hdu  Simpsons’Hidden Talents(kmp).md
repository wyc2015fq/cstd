# hdu   Simpsons’Hidden Talents(kmp) - Vmetrio - 博客园







# [hdu   Simpsons’Hidden Talents(kmp)](https://www.cnblogs.com/wangmengmeng/p/5490980.html)





Problem Description

Homer: Marge, I just figured out a way to discover some of the talents we weren’t aware we had.
Marge: Yeah, what is it?
Homer: Take me for example. I want to find out if I have a talent in politics, OK?
Marge: OK.
Homer: So I take some politician’s name, say Clinton, and try to find the length of the longest prefix
in Clinton’s name that is a suffix in my name. That’s how close I am to being a politician like Clinton
Marge: Why on earth choose the longest prefix that is a suffix???
Homer: Well, our talents are deeply hidden within ourselves, Marge.
Marge: So how close are you?
Homer: 0!
Marge: I’m not surprised.
Homer: But you know, you must have some real math talent hidden deep in you.
Marge: How come?
Homer: Riemann and Marjorie gives 3!!!
Marge: Who the heck is Riemann?
Homer: Never mind.
Write a program that, when given strings s1 and s2, finds the longest prefix of s1 that is a suffix of s2.





Input

Input consists of two lines. The first line contains s1 and the second line contains s2. You may assume all letters are in lowercase.





Output

Output consists of a single line that contains the longest string that is a prefix of s1 and a suffix of s2, followed by the length of that prefix. If the longest such string is the empty string, then the output should be 0.
The lengths of s1 and s2 will be at most 50000.



Sample Input


clinton

homer

riemann


marjorie




Sample Output


0
rie 3




Source

[HDU 2010-05 Programming Contest](http://acm.hdu.edu.cn/search.php?field=problem&key=HDU+2010-05+Programming+Contest+&source=1&searchmode=source)



Recommend

lcy   |   We have carefully selected several similar problems for you:  [2595](http://acm.hdu.edu.cn/showproblem.php?pid=2595)[2596](http://acm.hdu.edu.cn/showproblem.php?pid=2596)[2597](http://acm.hdu.edu.cn/showproblem.php?pid=2597)[2598](http://acm.hdu.edu.cn/showproblem.php?pid=2598)[2599](http://acm.hdu.edu.cn/showproblem.php?pid=2599)





题解:求最长的相同前缀和后缀...注意kmp函数中的要遍历第二条串的次数...



代码:


```
1 #include <vector>
 2 #include <map>
 3 #include <set>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <cmath>
 8 #include <cstdlib>
 9 #include <string>
10 #include <cstring>
11 #include <queue>
12 using namespace std;
13 #define INF 0x3f3f3f3f
14 
15 char p[100010],s[100010];
16 int nex[100010],ex[100010];
17 
18 void get(char *p)
19 {
20     int plen=strlen(p);
21     nex[0]=-1;
22     int k=-1,j=0;
23     while(j < plen){
24         if(k==-1 || p[j] == p[k]){
25             ++j;
26             ++k;
27             if(p[j] != p[k])
28                 nex[j]=k;
29             else
30                 nex[j]=nex[k];
31         }
32         else{
33             k=nex[k];
34         }
35     }
36 }
37 
38 void kmp(char *p,char *s)
39 {
40     get(p);
41     ex[0]=0;
42     int i=0,j=0;
43     int slen=strlen(s);
44     int plen=strlen(p);
45     while(i < slen ){
46         if(j==-1 || s[i]==p[j]){
47             ++i;
48             ++j;
49             ex[i]=j;
50         }
51         else{
52             j=nex[j];
53         }
54     }
55     /*if(j == plen)
56         return i-j;
57     else
58         return -1;*/
59 }
60 
61 int main()
62 {
63     while(~scanf("%s%s",p,s)){
64         kmp(p,s);
65         int len=strlen(s);
66         if(ex[len]==0)
67             printf("0\n");
68         else{
69             for(int i=0; i<ex[len]; i++)
70                 printf("%c",p[i]);
71             printf(" %d\n",ex[len]);
72         }
73     }
74     return 0;
75 }
```















