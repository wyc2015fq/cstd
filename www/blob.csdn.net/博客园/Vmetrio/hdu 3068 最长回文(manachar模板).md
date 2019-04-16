# hdu 3068 最长回文(manachar模板) - Vmetrio - 博客园







# [hdu 3068 最长回文(manachar模板)](https://www.cnblogs.com/wangmengmeng/p/5478649.html)





Problem Description

给出一个只由小写英文字符a,b,c...y,z组成的字符串S,求S中最长回文串的长度.
回文就是正反读都是一样的字符串,如aba, abba等





Input

输入有多组case,不超过120组,每组输入为一行小写英文字符a,b,c...y,z组成的字符串S
两组case之间由空行隔开(该空行不用处理)
字符串长度len <= 110000





Output

每一行一个整数x,对应一组case,表示该组case的字符串中所包含的最长回文长度.





Sample Input


aaaa




abab




Sample Output


4

3




Source

[2009 Multi-University Training Contest 16 - Host by NIT](http://acm.hdu.edu.cn/search.php?field=problem&key=2009+Multi-University+Training+Contest+16+-+Host+by+NIT&source=1&searchmode=source)



Recommend

lcy   |   We have carefully selected several similar problems for you:  [1358](http://acm.hdu.edu.cn/showproblem.php?pid=1358)[3336](http://acm.hdu.edu.cn/showproblem.php?pid=3336)[3065](http://acm.hdu.edu.cn/showproblem.php?pid=3065)[3746](http://acm.hdu.edu.cn/showproblem.php?pid=3746)[1711](http://acm.hdu.edu.cn/showproblem.php?pid=1711)



manachar讲解:   http://blog.csdn.net/bruce_zeng/article/details/8629572



代码:

```
1 #include <vector>
 2 #include <set>
 3 #include <algorithm>
 4 #include <iostream>
 5 #include <cstdio>
 6 #include <cmath>
 7 #include <cstdlib>
 8 #include <string>
 9 #include <cstring>
10 #include <queue>
11 #include <stack>
12 using namespace std;
13 #define N 110010*2
14 
15 char s[N],str[N];
16 int p[N];
17 
18 int init()
19 {
20     int i,j=0;
21     str[j++]='$';
22     for(i=0;s[i];i++){
23         str[j++]='#';
24         str[j++]=s[i];
25     }
26     str[j++]='#';
27     str[j]='\0';
28     return j;
29 }
30 
31 void manachar(int n)
32 {
33     int mx=0,id,i;
34     p[0]=0;
35     for(i=1;i<n;i++){
36         if(mx>i)
37             p[i]=min(mx-i,p[2*id-i]);
38         else
39             p[i]=1;
40         while(str[i-p[i]]==str[i+p[i]])
41             p[i]++;
42         if(p[i]+i>mx){
43             mx=p[i]+i;
44             id=i;
45         }
46     }
47 }
48 
49 int main()
50 {
51     int n;
52     while(~scanf("%s",s)){
53         n=init();
54         manachar(n);
55         int ans=0;
56         for(int i=1;i<n; i++){
57             if(p[i]>ans)
58                 ans=p[i];
59         }
60         printf("%d\n",ans-1);
61     }
62 }
```














