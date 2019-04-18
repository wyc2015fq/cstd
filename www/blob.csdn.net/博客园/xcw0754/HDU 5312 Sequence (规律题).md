# HDU 5312 Sequence (规律题) - xcw0754 - 博客园
# [HDU 5312 Sequence (规律题)](https://www.cnblogs.com/xcw0754/p/4678625.html)
题意：
　　一个序列的第n项为3*n*(n-1)+1，而 n>=1，现在给一个正整数m，问其最少由多少个序列中的数组成？
思路：
　　首先，序列第1项是1，所以任何数都能构成了。但是最少应该是多少？对式子进行变形，**6*(n*(n-1)/2)+1**，看到了三角形数**n*(n-1)/2**，那么应该是6*（任意自然数）+x=m才对，因为最多只要3个三角形数就能组成任何自然数啦。
　　不妨试试m%6是多少？这样试图求x可以吗？因为任意自然数最多由3个组成，如果是k个，那么应该x>=k，别忘了还有个+1的项。x-k那部分，就只能由1来搞定了。
　　还有个前提，m%6=0怎么办？总不能由0个组成吧？那应该起码是1，所以(m-1)%6可以保证不为0。试试看m=13，则x=(13-1)%6+1=1，这样就真的能由1个序列中的数构成吗？序是1，7，19...好像没有。。。悲剧！应该是7+1+1+1+1+1+1才是，那6个1是补上去的。同理x=2也是需要保证序列中有两个数字能组成m才行，否则要x+=6才是答案。而3及以上就不用了，为什么我也不知道。。。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=10100;
 7 unordered_map<int,int> mapp;
 8 vector<int > seq;
 9 void pre_cal()
10 {
11     int t=1;
12     for(int i=1; i<20000&&t<=1000000000; i++)
13     {
14         t=3*i*(i-1)+1;
15         mapp[t]=1;
16         seq.push_back(t);
17     }
18 }
19 
20 bool cal(int m)
21 {
22     int q=0, p=seq.size()-1;
23     while(q<=p)
24     {
25         int  t=seq[q]+seq[p];
26         if(t==m)    return 1;
27         if(t>m) p--;
28         else    q++;
29     }
30     return 0;
31 }
32 
33 int main()
34 {
35     //freopen("input.txt", "r", stdin);
36     int t, m;
37     pre_cal();
38     cin>>t;
39     while(t--)
40     {
41         scanf("%d",&m);
42         int k=(m-1)%6+1;//k是不会超过6的。但是答案可能超过6。
43 
44         if(k>=3)    printf("%d\n",k);
45         else if(k==1)//特判
46         {
47             if(mapp[m]) printf("1\n");
48             else    printf("%d\n",k+=6);
49         }
50         else if(k==2)//特判
51         {
52             if(cal(m))  printf("2\n");
53             else printf("%d\n",k+=6);
54         }
55     }
56     return 0;
57 }
AC代码
```

