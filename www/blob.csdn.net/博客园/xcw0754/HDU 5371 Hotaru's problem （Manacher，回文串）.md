# HDU 5371 Hotaru's problem （Manacher，回文串） - xcw0754 - 博客园
# [HDU 5371 Hotaru's problem （Manacher，回文串）](https://www.cnblogs.com/xcw0754/p/4722537.html)
题意：给一个序列，找出1个连续子序列，将其平分成前，中，后等长的3段子序列，要求【前】和【中】是回文，【中】和【后】是回文。求3段最长为多少？由于平分的关系，所以答案应该是3的倍数。
思路：先Manacher求最长子串，利用期间所记录的P 数组，穷举一下所有可能的前两串，再用O(1)时间判断第3串是否符合要求。
　　具体做法：
　　（1）P[i]记录的是以i为中心，从i-P[i]+1到i+P[i]-1这段都是回文。由于前两段之和必为偶数，所以必须选取str[i]为'#'的。
　　（2）扫一遍每个'#'，以其最长的回文开始穷举（仅需将P[i]--即可，然后找到右边对应的'#'，判断P[i]是不是大于所穷举的长度），直到3段都满足要求了，跳出此‘#’，换下一个。
```
1 #include <iostream>
 2 #include <cstring>
 3 #include <vector>
 4 #include <stdio.h>
 5 using namespace std;
 6 const int N=100010;
 7 int n;
 8 int s[N*2];
 9 int P[N*2];
10 
11 
12 int cal(int len)
13 {
14     if(n<3) return 0;
15     memset(P,0,sizeof(P));
16     int id=1, mx=1;
17     P[0]=1;
18     P[1]=1;
19     for(int i=2; i<len; i++)
20     {
21         P[i] =i>mx? 1: min( P[2*id-i], mx-i);
22         while(s[i+P[i]]==s[i-P[i]])    P[i]++;
23         if(i+P[i]>mx)
24         {
25             id=i;
26             mx=i+P[i];
27         }
28     }
29     int ans=0;
30     for(int i=3; i+4<len; i+=2)
31     {
32         int tag=P[i]-1;
33         if( tag>1 && tag>ans )
34         {
35             while( P[i+tag]<=tag && tag>ans )    tag--;
36             if(tag>ans) ans=tag;
37         }
38     }
39 
40     return ans/2*3;
41 }
42 
43 int main()
44 {
45     //freopen("input.txt", "r", stdin);
46     int t, tmp, j=0;
47     cin>>t;
48 
49     while(t--)
50     {
51         scanf("%d", &n);
52         s[0]=-1;
53         s[1]=-2;
54         for(int i=0,j=2; i<n; i++)
55         {
56             scanf("%d",&tmp);
57             s[j++]=tmp;
58             s[j++]=-2;
59         }
60         s[n*2+2]=-30;
61 
62         printf("Case #%d: %d\n", ++j, cal( n*2+2 ));
63     }
64     return 0;
65 }
AC代码
```

