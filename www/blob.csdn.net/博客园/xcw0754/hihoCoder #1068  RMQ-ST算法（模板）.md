# hihoCoder #1068 : RMQ-ST算法（模板） - xcw0754 - 博客园
# [hihoCoder #1068 : RMQ-ST算法（模板）](https://www.cnblogs.com/xcw0754/p/4506423.html)
|**AC**|G++|826ms|146MB|
思路：
　　时间复杂度O(nlogn)。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <set>
 7 #include <deque>
 8 #include <map>
 9 #include <algorithm>
10 #include <vector>
11 #include <iostream>
12 #define max(X,Y) ((X) > (Y) ? (X) : (Y))
13 #define min(X,Y) ((X) < (Y) ? (X) : (Y))
14 #define pii pair<int,int>
15 #define back que[rear-1]
16 #define INF 0x3f3f3f3f
17 #define LL long long
18 #define ULL unsigned long long
19 using namespace std;
20 const double PI  = acos(-1.0);
21 const int N=1e6+20;
22 int a[N];
23 struct ST
24 {
25     int st[N][32];
26     void init(int a[],int n)  //注意a的下标
27     {
28         for(int i=1; i<=n; i++) st[i][0]=a[i];
29         for(int q=1; (1<<q)<=n; q++)   //以(1<<i)为距离
30             for(int j=1; j+(1<<q)-1<=n; j++)   //枚举起点
31                 st[j][q]=min(st[j][q-1], st[j+(1<<q-1)][q-1]);
32     }
33     int query(int L,int R)
34     {
35         int len=R-L+1;
36         for(int i=30; i>=0; i--)//这里可以预处理
37         {
38             if( (len>>i)&1 )
39             {
40                 return min(st[L][i], st[R-(1<<i)+1][i]);
41             }
42         }
43 
44     }
45 }st;
46 
47 int main()
48 {
49     //freopen("input.txt", "r", stdin);
50     int n, m, L, R;scanf("%d",&n);
51     for(int i=1; i<=n; i++) scanf("%d",&a[i]);
52     st.init(a,n);
53 
54     scanf("%d",&m);
55     while(m--)
56     {
57         scanf("%d%d",&L,&R);
58         printf("%d\n", st.query(L,R));
59     }
60     return 0;
61 }
AC代码
```
　　用模板重打了一次。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=1000100;
 4 int w[N], pre[N][32], n, q;
 5 void pre_cal()
 6 {
 7     for(int i=0; i<n; i++)    pre[i][0]=w[i];
 8     for(int i=2,q=1; i<=n; i*=2,q++)
 9         for(int j=0; j<n; j++)
10             if(j+i-1<n)    pre[j][q]=min(pre[j][q-1],pre[j+i/2][q-1]);
11             else    break;
12 }
13 int main()
14 {
15     //freopen("input.txt", "r", stdin);
16     cin>>n;
17     for(int i=0; i<n; i++)    scanf("%d",&w[i]);    //输入重量
18     pre_cal();    //预处理
19 
20     cin>>q;
21     int L, R;
22     for(int i=0; i<q; i++)  //输入查询，并直接处理输出
23     {
24         scanf("%d%d", &L, &R);
25         int len=R-L+1, tmp=0, cnt=0;
26 
27         for(int i=0; i<30; i++)//找出二进制最高位的1
28         {
29             if(!(len>>i))
30             {
31                 tmp=((len>>(i-1))<<(i-1));
32                 break;
33             }
34             cnt++;
35         }
36         if(R==L)    printf("%d\n",w[L-1]);
37         else if(tmp==len)    printf("%d\n",pre[L-1][cnt-1]);//这步其实可省。
38         else  printf("%d\n",min(pre[L-1][cnt-1], pre[R-tmp][cnt-1]));
39     }
40     return 0;
41 }
AC代码
```

