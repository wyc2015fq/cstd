# HDU 5264 pog loves szh I (字符串，水) - xcw0754 - 博客园
# [HDU 5264 pog loves szh I (字符串，水)](https://www.cnblogs.com/xcw0754/p/4559705.html)
题意：设有两个串A和B，现将B反转，再用插入的形式合成一个串。如：A：abc   B：efg；反转B先，变gfe；作插入，agbfce。现在给出一个串，要求还原出A和B。
思路：扫一遍O（n），串A在扫的时候直接输出，串2在扫的时候反向存储，再输出。
```
1 #include <iostream>
 2 #include <cmath>
 3 #include <cstdio>
 4 #include <cstring>
 5 using namespace std;
 6 const int N=105;
 7 char str[N];
 8 char s2[N];
 9 
10 
11 
12 
13 void cal(int len)
14 {
15     int j=0;
16     for(int i=0,p=len-1; i<len; i+=2)
17     {
18         printf("%c",str[i]);
19         s2[j++]=str[p];
20         p-=2;
21     }
22     s2[j]='\0';
23     printf("\n%s\n",s2);
24 }
25 
26 int main()
27 {
28     //freopen("input.txt", "r", stdin);
29     int t;
30     cin>>t;
31     while(t--)
32     {
33         scanf("%s",str);
34         cal(strlen(str));
35     }
36 
37     return 0;
38 }
AC代码
```

