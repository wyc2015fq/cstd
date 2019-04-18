# NBUT 1117 Kotiya's Incantation（字符输入处理） - xcw0754 - 博客园
# [NBUT 1117 Kotiya's Incantation（字符输入处理）](https://www.cnblogs.com/xcw0754/p/4538757.html)
题意：
　　比较两个串，有三种情况：完全相同，可见字符相同，不同。每个字符串以'-'结尾。难点在输入。
思路：
　　字符逐个读入，直到'-'为止，读出两串就可以直接进行判断。如果不足两串则结束。输入时需要注意：输入第一个串时可能不足一个串，读到EOF要及时结束，别一直循环。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <set>
 6 
 7 using namespace std;
 8 const int N=1000000;
 9 char a[N], b[N];
10 char aa[N], bb[N];
11 const int ch='-';
12 int len2=0,len22=0,  len1=0,len11=0;        //输入1
13 
14 
15 void check()
16 {
17     if(len11 !=len22)//连可见字符都不一样多，肯定“不同”
18     {
19         printf("DIFFERENT\n");
20         return;
21     }
22     
23     int i=0;
24     if(len1==len2 && strcmp(a,b)==0)  //长度相同才可能一样的
25     {
26         printf("SAME\n");
27         return ;
28     }
29     if(len11==len22 && strcmp(aa,bb)==0) //有可能相似
30     {
31         printf("SIMILAR\n");  
32         return;
33     }        
34     printf("DIFFERENT\n");
35 }
36 
37 int main()
38 {
39     //freopen("input.txt", "r", stdin);
40     char c;
41     while(1)
42     {
43         len1=len11=0;
44         while((c=cin.get())!=ch)
45         {
46             if(c==-1)    return 0;    //EOF在这
47             a[len1++]=c;
48             if(c>='a'&&c<='z'||c>='A'&&c<='Z')    aa[len11++]=c;
49         }
50         a[len1]= aa[len11]='\0';
51 
52         len2=len22=0;
53         while((c=cin.get())!=ch )
54         {
55             b[len2++]=c;
56             if(c>='a'&&c<='z'||c>='A'&&c<='Z')    bb[len22++]=c;
57         }
58         b[len2]=bb[len22]='\0';
59         check();
60     }
61     return 0;
62 }
AC代码
```

