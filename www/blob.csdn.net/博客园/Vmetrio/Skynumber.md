# Skynumber - Vmetrio - 博客园







# [Skynumber](https://www.cnblogs.com/wangmengmeng/p/5510119.html)






Time Limit: 1000ms

Memory Limit: 128000KB

64-bit integer IO format:      Java class name:


[Submit](http://115.159.40.116/contest_show.php?cid=32)[Status](http://115.159.40.116/contest_show.php?cid=32#status/B)


key天生对数字特别敏感，一次偶然的机会，他发现了一个有趣的四位数2992，这个数，它的十进制数表示，其四位数字之和为2+9+9+2=22，它的十六进制数BB0，其四位数字之和也为22，同时它的十二进制数表示1894，其四位数字之和也为22。key非常喜欢这种四位数(三种进制的和相等)，由于他的发现，所以这里我们命名其为key数。但是要判断这样的数还是有点麻烦啊，那么现在请你帮忙来判断任何一个十进制的四位数，是不是key数吧。




### Input

输入含有一些四位正整数，如果为0，则输入结束。




### Output

若n为Sky数，则输出“#n is a sky Number.”，否则输出“#n is not a sky Number.”。每个结果占一行。注意：#n表示所读入的n值。




### Sample Input
2992
1234
0

### Sample Output
2992 is a Sky Number.
1234 is not a Sky Number.



[Submit](http://115.159.40.116/contest_show.php?cid=32)[Status](http://115.159.40.116/contest_show.php?cid=32#status/B)






代码:

```
1 #include <stdio.h>
 2 #include <string.h>
 3 #include <math.h>
 4 #include <limits.h> 
 5 #include <algorithm>
 6 #include <iostream>
 7 #include <ctype.h>
 8 #include <iomanip>
 9 #include <queue>
10 #include <map>
11 #include <stdlib.h>
12 
13 using namespace std;
14  
15 int sum_k(int n,int k)
16 {
17     int s=0;
18     while(n!=0)
19     {
20         s+=n%k;
21         n/=k;
22     }
23     return s;
24 }
25 int main()
26 {
27     int n;
28     while(~scanf("%d",&n)){
29         if(n==0)
30             break;
31         int a,b,c;
32         a=sum_k(n,10);
33         b=sum_k(n,12);
34         c=sum_k(n,16);
35         if(a==b&&a==c)
36             printf("%d is a Sky Number.\n",n);
37         else
38             printf("%d is not a Sky Number.\n",n);
39     }
40 }
```














