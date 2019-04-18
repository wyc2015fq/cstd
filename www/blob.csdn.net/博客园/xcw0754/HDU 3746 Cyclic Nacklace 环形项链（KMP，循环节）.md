# HDU 3746 Cyclic Nacklace 环形项链（KMP，循环节） - xcw0754 - 博客园
# [HDU 3746 Cyclic Nacklace 环形项链（KMP，循环节）](https://www.cnblogs.com/xcw0754/p/4681582.html)
题意：
　　给一个字符串，问：要补多少个字符才能让其出现循环？出现循环是指循环节与字符串长度不相等。比如abc要补多个变成abcabc。若已经循环，输出0。
思路：
　　根据最小循环节的公式，当len%(len-next[len])==0时，最小循环节为len/(len-next[len])，而当len%(len-next[len])!=0时，就没有循环节。可以通过在串尾补上一些字符，使得len%(len-next[len])==0，就会出现循环节了。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=100010;
 7 char str[N];
 8 int _next[N];
 9 
10 void get_next(int len)
11 {
12     _next[0]=-1;
13     int i=0, j=-1;   //模式串
14     while(i<len)
15     {
16         if(j==-1||str[j]==str[i])   _next[++i]=++j;
17         else    j=_next[j];
18     }
19 }
20 
21 int main()
22 {
23     freopen("input.txt", "r", stdin);
24     int t, len=0;
25     cin>>t;
26     while(t--)
27     {
28         scanf("%s",str);
29         get_next(len=strlen(str));
30         if(_next[len]==0)    printf("%d\n",len);       //这是没有循环节的情况
31         else if(len%(len-_next[len])==0) puts("0");  //有循环了
32         else    //整串不循环，补字符凑循环
33         {
34             int i=1;
35             while( i<len && (len+i)%(len-_next[len])!=0)   i++;//尝试在后面添加匹配的字符，最长不超len
36             printf("%d\n", i);
37         }
38     }
39     return 0;
40 }
AC代码
```

