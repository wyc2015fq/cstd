# NBUT 1119 Patchouli's Books （STL应用） - xcw0754 - 博客园
# [NBUT 1119 Patchouli's Books （STL应用）](https://www.cnblogs.com/xcw0754/p/4538809.html)
题意：
　　输入一个序列，每个数字小于16，序列元素个数小于9. 要求将这个序列所有可能出现的顺序输出，而且要字典序。
思路：
　　先排序，输出该升序序列，再用next_permutation进行转变即可，它会调整一次序列，并且字典序是比传入的序列要大一些，也就是离传入序列字典序最近的序列。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=10;
 4 char has[50]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
 5 char a[N];
 6 void print(char x)
 7 {
 8     printf("%c ",x);
 9 }
10 int main()
11 {
12 
13     //freopen("input.txt", "r", stdin);
14     int n;
15     while(cin>>n)
16     {
17         int tmp;
18         for(int i=0; i<n; i++)
19         {
20             scanf("%d",&tmp);
21             a[i]=has[tmp];
22         }
23         
24         sort(a,a+n);
25         for_each(a,a+n-1,print);
26         printf("%c\n",a[n-1]);
27         
28         while( next_permutation(a,a+n))
29         {
30             for_each(a,a+n-1,print);
31             printf("%c\n",a[n-1]);
32         }
33     }
34     return 0;
35 }
AC代码
```

