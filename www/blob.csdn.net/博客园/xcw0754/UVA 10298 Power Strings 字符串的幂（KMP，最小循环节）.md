# UVA 10298 Power Strings 字符串的幂（KMP，最小循环节） - xcw0754 - 博客园
# [UVA 10298 Power Strings 字符串的幂（KMP，最小循环节）](https://www.cnblogs.com/xcw0754/p/4681463.html)
题意：
　　定义a为一个字符串，a*a表示两个字符相连，即 an+1=a*an ，也就是出现循环了。给定一个字符串，若将其表示成an，问n最大为多少？
思路：
　　如果完全不循环，顶多就是类似于abc1这样，即n=1。但是如果循环出现了，比如abab，那就可以表示成(ab)2。还有一点，就是要使得n尽量大，那么当出现abababab时，应该要这么表示(ab)4，而不是(abab)2。
　　此题用神奇的KMP解决，也就是主要利用next数组。举例说明。
　　一般出现循环的都会大概是这样的：abcabcabc。而这样是没有出现循环的：ababa，5个数字，质数，怎样都不会循环啦。那么下面就拿abcabcabc来举例。
　　abcabcabc的next数组表示成：
　　next[0~9]={-1，0，0，0，1，2，3，4，5，6}对吧？但是我们通常用的只是next[0~8]而已，现在next[9]派上用场了。here comes.....
　　若len%(len-next[len])==0，则最小循环节为len/(len-next[len])，否则为1。为虾米？
首先是，这两小段是匹配的，对吧？根据next数组都能看出来啦。
abcabcabc
　　abcabcabc
接着，因为次串next[9]=6，那么len-next[9]=3，也就是说串头还剩下3个字符，说的就是"abc"。如果喔，len%(len-next[len])=9%(9-6)=9%3=0呢，最小循环节浮现了，就是串头"abc"，又为虾米？ 拆串来看看呗：
　　abc abc abc
　　我将他们3个子串分别命名为A和B和C。
　　既然next[9]说明了s[9]的前面居然有6个和串头匹配，那么AB=BC，自然B=A啦（再看next去）。也就是说如果除了串头A，剩下的字符BC都能够是A的个数的倍数，这事就成了。只要不是倍数，那么就没什么循环的可能了。
　　A刚好3个字符，后面的BC加起来是6个，刚好是3的倍数，会循环。
可以搭配这篇文章继续看：http://www.cnblogs.com/jackge/archive/2013/01/05/2846006.html
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=1000010;
 7 
 8 char qstr[N];
 9 int qnext[N];
10 
11 void get_next(int len)
12 {
13     qnext[0]=-1;
14     int i=0;
15     int j=-1;   //模式串
16     while(i<len)
17     {
18         if(j==-1||qstr[j]==qstr[i])   qnext[++i]=++j;
19         else    j=qnext[j];
20     }
21 }
22 
23 int main()
24 {
25     freopen("input.txt", "r", stdin);
26     while(scanf("%s",qstr),qstr[0]!='.')
27     {
28         int len=0;
29         get_next(len=strlen(qstr));
30 
31         int ans=len%(len-qnext[len])==0?len/(len-qnext[len]):1;
32         printf("%d\n",ans);
33     }
34     return 0;
35 }
AC代码
```

