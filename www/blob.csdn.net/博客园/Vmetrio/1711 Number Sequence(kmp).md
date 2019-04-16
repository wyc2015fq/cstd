# 1711 Number Sequence(kmp) - Vmetrio - 博客园







# [1711 Number Sequence(kmp)](https://www.cnblogs.com/wangmengmeng/p/5487174.html)





Problem Description

Given two sequences of numbers : a[1], a[2], ...... , a[N], and b[1], b[2], ...... , b[M] (1 <= M <= 10000, 1 <= N <= 1000000). Your task is to find a number K which make a[K] = b[1], a[K + 1] = b[2], ...... , a[K + M - 1] = b[M]. If there are more than one K exist, output the smallest one.





Input

The first line of input is a number T which indicate the number of cases. Each case contains three lines. The first line is two numbers N and M (1 <= M <= 10000, 1 <= N <= 1000000). The second line contains N integers which indicate a[1], a[2], ...... , a[N]. The third line contains M integers which indicate b[1], b[2], ...... , b[M]. All integers are in the range of [-1000000, 1000000].





Output

For each test case, you should output one line which only contain K described above. If no such K exists, output -1 instead.





Sample Input


2
13 5
1 2 1 2 3 1 2 3 1 3 2 1 2
1 2 3 1 3
13 5
1 2 1 2 3 1 2 3 1 3 2 1 2
1 2 3 2 1






Sample Output


6
-1






Source

[HDU 2007-Spring Programming Contest](http://acm.hdu.edu.cn/search.php?field=problem&key=HDU+2007-Spring+Programming+Contest&source=1&searchmode=source)





Recommend

lcy   |   We have carefully selected several similar problems for you:  [1358](http://acm.hdu.edu.cn/showproblem.php?pid=1358)[3336](http://acm.hdu.edu.cn/showproblem.php?pid=3336)[3746](http://acm.hdu.edu.cn/showproblem.php?pid=3746)[1867](http://acm.hdu.edu.cn/showproblem.php?pid=1867)[2203](http://acm.hdu.edu.cn/showproblem.php?pid=2203)



题解:kmp模板 编译错误了多次,无爱啦...



代码:

```
1 #include <iostream>
 2 #include <cstdio>
 3 using namespace std;
 4 int p[10010],s[1000010];
 5 int n,m;
 6 int nex[10010];
 7 
 8 void get()
 9 {
10     int plen=m;
11     nex[0]=-1;
12     int k=-1,j=0;
13     while(j < plen){
14         if(k==-1 || p[j] == p[k]){
15             ++j;
16             ++k;
17             if(p[j] != p[k])
18                 nex[j]=k;
19             else
20                 nex[j]=nex[k];
21         }
22         else{
23             k=nex[k];
24         }
25     }
26 }
27 
28 int kmp()
29 {
30     int i=0,j=0;
31     int slen=n;
32     int plen=m;
33     while(i < slen && j< plen){
34         if(j==-1 || s[i]==p[j]){
35             ++i;
36             ++j;
37         }
38         else{
39             j=nex[j];
40         }
41     }
42     if(j == plen)
43         return i-j+1;
44     else
45         return -1;
46 }
47 
48 int main()
49 {
50     int t;
51     scanf("%d",&t);
52     while(t--){
53         scanf("%d%d",&n,&m);
54         for(int i=0; i<n; i++)
55             scanf("%d",&s[i]);
56         for(int i=0; i<m; i++)
57             scanf("%d",&p[i]);
58         get();
59         printf("%d\n",kmp());
60     }
61 }
```














