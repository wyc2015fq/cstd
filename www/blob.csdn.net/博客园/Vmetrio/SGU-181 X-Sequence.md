# SGU-181 X-Sequence - Vmetrio - 博客园







# [SGU-181 X-Sequence](https://www.cnblogs.com/wangmengmeng/p/5539604.html)





训练比赛的时候没有写出来...



搜了发题解...



链接:http://www.cppblog.com/willing/archive/2010/05/02/114167.aspx



代码:

```
1 #include <stdio.h>
 2 #include <string.h>
 3 
 4 int main() {
 5     int A, a, b, c, m, k;
 6     scanf ("%d%d%d%d%d%d", &A, &a, &b, &c, &m, &k);
 7     int ans[1000] = {A%m};
 8     int flag[1001];
 9     memset(flag, -1, sizeof(int)*1001);
10     flag[A%m]=0;
11     int i;
12     for (i = 1; flag[ans[i]=(a*ans[i-1]*ans[i-1]+b*ans[i-1]+c)%m]<0; ++i) {
13         flag[ans[i]] = i;
14     }
15     if (k==0) {
16         printf ("%d\n", A);
17     } else if (k >= flag[ans[i]]) {
18         printf ("%d\n", ans[(k-flag[ans[i]])%(i-flag[ans[i]])+flag[ans[i]]]);
19     } else {
20         printf ("%d\n", ans[k]);
21     }
22     return 0;
23 }
```














