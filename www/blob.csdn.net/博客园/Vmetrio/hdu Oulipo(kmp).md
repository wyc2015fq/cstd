# hdu  Oulipo(kmp) - Vmetrio - 博客园







# [hdu  Oulipo(kmp)](https://www.cnblogs.com/wangmengmeng/p/5487252.html)







Problem Description

The French author Georges Perec (1936–1982) once wrote a book, La disparition, without the letter 'e'. He was a member of the Oulipo group. A quote from the book:

Tout avait Pair normal, mais tout s’affirmait faux. Tout avait Fair normal, d’abord, puis surgissait l’inhumain, l’affolant. Il aurait voulu savoir où s’articulait l’association qui l’unissait au roman : stir son tapis, assaillant à tout instant son imagination, l’intuition d’un tabou, la vision d’un mal obscur, d’un quoi vacant, d’un non-dit : la vision, l’avision d’un oubli commandant tout, où s’abolissait la raison : tout avait l’air normal mais…

Perec would probably have scored high (or rather, low) in the following contest. People are asked to write a perhaps even meaningful text on some subject with as few occurrences of a given “word” as possible. Our task is to provide the jury with a program that counts these occurrences, in order to obtain a ranking of the competitors. These competitors often write very long texts with nonsense meaning; a sequence of 500,000 consecutive 'T's is not unusual. And they never use spaces.

So we want to quickly find out how often a word, i.e., a given string, occurs in a text. More formally: given the alphabet {'A', 'B', 'C', …, 'Z'} and two finite strings over that alphabet, a word W and a text T, count the number of occurrences of W in T. All the consecutive characters of W must exactly match consecutive characters of T. Occurrences may overlap.







Input

The first line of the input file contains a single number: the number of test cases to follow. Each test case has the following format:

One line with the word W, a string over {'A', 'B', 'C', …, 'Z'}, with 1 ≤ |W| ≤ 10,000 (here |W| denotes the length of the string W).
One line with the text T, a string over {'A', 'B', 'C', …, 'Z'}, with |W| ≤ |T| ≤ 1,000,000.





Output

For every test case in the input file, the output should contain a single number, on a single line: the number of occurrences of the word W in the text T.







Sample Input


3

BAPC

 BAPC 

AZA


AZAZAZA


VERDI


AVERDXIVYERDIAN




Sample Output


1
3
0




Source

[华东区大学生程序设计邀请赛_热身赛](http://acm.hdu.edu.cn/search.php?field=problem&key=%BB%AA%B6%AB%C7%F8%B4%F3%D1%A7%C9%FA%B3%CC%D0%F2%C9%E8%BC%C6%D1%FB%C7%EB%C8%FC_%C8%C8%C9%ED%C8%FC&source=1&searchmode=source)



Recommend

lcy   |   We have carefully selected several similar problems for you:  [1358](http://acm.hdu.edu.cn/showproblem.php?pid=1358)[3336](http://acm.hdu.edu.cn/showproblem.php?pid=3336)[3746](http://acm.hdu.edu.cn/showproblem.php?pid=3746)[2203](http://acm.hdu.edu.cn/showproblem.php?pid=2203)[3374](http://acm.hdu.edu.cn/showproblem.php?pid=3374)



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
15 char p[10010],s[1000010];
16 int nex[10010];
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
38 int kmp(char *s,char *p)
39 {
40     int i=0,j=0,ans=0;
41     int slen=strlen(s);
42     int plen=strlen(p);
43     while(i < slen && j< plen){
44         if(j==-1 || s[i]==p[j]){
45             ++i;
46             ++j;
47         }
48         else{
49             j=nex[j];
50         }
51         if(j == plen){  //重点注意，这里是为了回到当匹配完后，next[j]应该回到的位置 
52             j=nex[j];   //例： a="aza" b="azazaza" 第一次结束后，next[j]应该所指的位置为a中的‘z’,然后继续匹配  
53             ans++;
54         }
55     }
56 
57     return ans;
58 }
59 
60 int main()
61 {
62     int t;
63     scanf("%d",&t);
64     while(t--){
65         scanf("%s",p);
66         scanf("%s",s);
67         get(p);
68         printf("%d\n",kmp(s,p));
69     }
70 }
```














