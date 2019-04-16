# hdu 亲和串(kmp) - Vmetrio - 博客园







# [hdu 亲和串(kmp)](https://www.cnblogs.com/wangmengmeng/p/5487313.html)





Problem Description

人随着岁数的增长是越大越聪明还是越大越笨，这是一个值得全世界科学家思考的问题,同样的问题Eddy也一直在思考，因为他在很小的时候就知道亲和串如何判断了，但是发现，现在长大了却不知道怎么去判断亲和串了，于是他只好又再一次来请教聪明且乐于助人的你来解决这个问题。
亲和串的定义是这样的：给定两个字符串s1和s2,如果能通过s1循环移位，使s2包含在s1中，那么我们就说s2 是s1的亲和串。





Input

本题有多组测试数据，每组数据的第一行包含输入字符串s1,第二行包含输入字符串s2，s1与s2的长度均小于100000。





Output

如果s2是s1的亲和串，则输出"yes"，反之，输出"no"。每组测试的输出占一行。





Sample Input


AABCD


CDAA

ASD


ASDF




Sample Output


yes
no




Author

Eddy





Recommend

lcy   |   We have carefully selected several similar problems for you:  [1358](http://acm.hdu.edu.cn/showproblem.php?pid=1358)[3336](http://acm.hdu.edu.cn/showproblem.php?pid=3336)[3746](http://acm.hdu.edu.cn/showproblem.php?pid=3746)[2205](http://acm.hdu.edu.cn/showproblem.php?pid=2205)[2207](http://acm.hdu.edu.cn/showproblem.php?pid=2207)





题解:注意循环...



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
16 int nex[100010];
17 int lens;
18 
19 void get(char *p)
20 {
21     int plen=strlen(p);
22     nex[0]=-1;
23     int k=-1,j=0;
24     while(j < plen){
25         if(k==-1 || p[j] == p[k]){
26             ++j;
27             ++k;
28             if(p[j] != p[k])
29                 nex[j]=k;
30             else
31                 nex[j]=nex[k];
32         }
33         else{
34             k=nex[k];
35         }
36     }
37 }
38 
39 int kmp(int lens,char *p)
40 {
41     int i=0,j=0;
42     int slen=lens;
43     int plen=strlen(p);
44     while(i < slen && j< plen){
45         if(j==-1 || s[i]==p[j]){
46             ++i;
47             ++j;
48         }
49         else{
50             j=nex[j];
51         }
52     }
53     if(j == plen)
54         return i-j;
55     else
56         return -1;
57 }
58 
59 int main()
60 {
61     while(~scanf("%s%s",s,p)){
62         lens=strlen(s);
63         for(int i=lens; i<2*lens; i++){
64             s[i]=s[i-lens];
65         }
66         get(p);
67         lens+=lens;
68         if(kmp(lens,p)!=-1)
69             puts("yes");
70         else
71             puts("no");
72     }
73 }
```















