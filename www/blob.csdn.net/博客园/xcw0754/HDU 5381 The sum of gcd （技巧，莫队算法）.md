# HDU 5381 The sum of gcd （技巧，莫队算法） - xcw0754 - 博客园
# [HDU 5381 The sum of gcd （技巧，莫队算法）](https://www.cnblogs.com/xcw0754/p/4734773.html)
 题意：有一个含n个元素的序列，接下来有q个询问区间，对每个询问区间输出其 f(L,R) 值。
 思路：
　　天真单纯地以为是道超级水题，不管多少个询问，计算量顶多就是O(n2) ，就是暴力穷举每个区间，再直接开个1e8大的int数组保存其结果不就行了？呵呵，限制你内存，看你怎么死！即使给了你这么大的内存，O(n2) 也不容易过，计算量偏大，少一点也许可以。
　　贴个O(n2)代码。
```
1 #include <bits/stdc++.h>
 2 #define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
 3 #define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
 4 #define pii pair<int,int>
 5 #define INF 0x7f7f7f7f
 6 #define LL long long
 7 using namespace std;
 8 const int N=10005;
 9 struct node
10 {
11     int L, R, ans;
12 }que[N];
13 struct node1
14 {
15     int ll, rr, gcd;
16     node1(){};
17     node1(int ll,int rr,int gcd):ll(ll),rr(rr),gcd(gcd){};
18 }reg[N][50];
19 
20 int _gcd(int a, int b){return b == 0 ? a : _gcd(b, a % b);}
21 
22 int seq[N], n, q, pre[N];
23 int cnt[N];
24 void pre_cal()
25 {
26     memset(cnt,0,sizeof(cnt));
27     reg[n][0]=node1(n,n,seq[n]); //最后一个特殊处理
28     cnt[n]++;
29     for(int i=n-1; i>=1; i--)
30     {
31         int R=i, gcd=seq[i], tmp;
32         for(int j=0; j<cnt[i+1]; j++)
33         {
34             node1 &t=reg[i+1][j];
35             tmp=_gcd(gcd, t.gcd );
36             if(tmp!=gcd)    reg[i][cnt[i]++]=node1(i, R, gcd);
37             gcd=tmp;
38             R=t.rr;
39         }
40         reg[i][cnt[i]++]=node1(i, R, gcd);
41     }
42 }
43 
44 void cal()
45 {
46     for(int i=1; i<=n; i++)
47     {
48         pre[i-1]=0;
49         pre[i]=seq[i];
50         int p=0;
51         for(int j=i+1; j<=n; j++)
52         {
53             if(j>reg[i][p].rr)  p++;
54             pre[j]=reg[i][p].gcd;
55             pre[j]+=pre[j-1];
56         }
57         for(int j=1; j<=q; j++)
58             if( i>=que[j].L && i<=que[j].R )
59                 que[j].ans+=pre[que[j].R]-pre[i-1];
60     }
61     for(int i=1; i<=q; i++)    printf("%d\n",que[i].ans);
62 }
63 int main()
64 {
65     //freopen("input.txt", "r", stdin);
66     int t, L, R;
67     cin>>t;
68     while(t--)
69     {
70         scanf("%d", &n);
71         for(int i=1; i<=n; i++)    scanf("%d", &seq[i]);
72 
73         scanf("%d", &q);
74         for(int i=1; i<=q; i++)
75         {
76             scanf("%d %d", &que[i].L, &que[i].R);
77             que[i].ans=0;
78         }
79         pre_cal();
80         cal();
81     }
82 
83     return 0;
84 }
TLE代码
```
　　接着是尝试其他方法，分块！主要是复杂度为啥是O(n1.5*32)，分析：
　　（1）假如有10000个元素，那么分成100块，每块100个元素。将所有询问保存，按照 (块号，R) 来排序（暂不管他L是否是有序）。
　　（2）如果我们可以在O(32*n)的时间内计算出任意一个块的所有查询，那么仅有100块，每块O(32*n)，那么就是O(32*n1.5)。为虾米可以在O(32*n)内.....?
　　（3）拿第一个块来分析，因为它可能是计算量最大的，比如来个询问[1,1]，接着来个[100,10000]，再来个[2,100]。拿出两个最极限的例子讲，[1,1]和[100,10000]，这也是O(32*n)的。看成是两个点，这两个点的曼哈顿距离是10100，这就是O(n)了。在[1,1]往[100,10000]转移过程中，每移动一单位的曼哈顿距离时至少是需要O(logN)的，就是计算GCD的复杂度而已！
　　（4）每单位的曼哈顿距离如何转移才能O(logN)？这需要预处理了。假设要从[1,5]转移到[1,6]，只要答案加上以Right[6][1]即可，Right[6][1]定义为 gcd[i....6]之和，即Right[6][1]=gcd[6,6]+gcd[5,6]+gcd[4,5,6]+gcd[3,4,5,6]+gcd[2,3,4,5,6]+gcd[1,2,3,4,5,6]。
　　所以要先预处理出所有的Right和Left（相反而已），复杂度O(nlogn)，注意，并不是真的是这样记录的Right[6][1]，实际上记录的是，以6为R，如果gcd相同，记录其L位置，由于随着数字越来越多，gcd肯定是会变小或者不变，而每次变小起码少一半，那结果最多纪录32个就行了。即使这样，我们还得靠递推的关系来使复杂度达到O(nlogn)，否则仍不行。
　　递推是这样的，假设知道以i为结尾的所有段都知道了，则可以靠Right[i]的32个段推出32个Right[i+1]的段（32指的是上限，不是实际）。这只是新加入了个数字在原来的基础上多了个数字seq[i+1]，若seq[i+1]跟Right[i][1]的gcd（记作x）不是seq[i+1]，那么seq[i+1]独自可作为一段，记为Right[i+1][0]， 接着按Right[i][1]的L继续往左扩展。（好啰嗦！不如画矩阵看来得直接，总之，其实满足递推关系的，而且是线性的。）
　　可以看看这篇！http://blog.csdn.net/u014800748/article/details/47680899
　　注意：没有用longlong会挂！
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x7f7f7f7f
  4 #define LL long long
  5 using namespace std;
  6 const int N=10050;
  7 struct node
  8 {
  9     int L, R, pos;
 10     LL ans;
 11 }que[N];
 12 
 13 struct node1
 14 {
 15     int ll, rr;
 16     LL gcd;
 17     node1(){};
 18     node1(int ll,int rr,LL gcd):ll(ll),rr(rr),gcd(gcd){};
 19 };
 20 int seq[N], n, q, blocks;
 21 vector<node1> Left[N], Right[N];
 22 inline int cmp(node a,node b)
 23 {
 24     if(a.L/blocks == b.L/blocks ) return a.R<b.R;
 25     return a.L < b.L; //既然不同块，大的肯定在后面
 26 }
 27 inline int cmp1(node a,node b){return a.pos<b.pos;}
 28 
 29 
 30 
 31 
 32 void pre_cal()  //计算右上三角，左下三角
 33 {
 34     LL gcd;
 35     int L, R, tmp;
 36     //************固定左边Left*********************
 37     Left[n].push_back(node1(n, n, seq[n])); //最后一个特殊处理
 38     for(int i=n-1; i>=1; i--)
 39     {
 40         L=R=i;
 41         gcd=seq[i];
 42         for(int j=0; j<Left[i+1].size(); j++)
 43         {
 44             node1 &t=Left[i+1][j];
 45             tmp=__gcd(gcd, t.gcd );
 46             if(tmp!=gcd)
 47             {
 48                 Left[i].push_back(node1(L, R, gcd));
 49                 L=R+1;
 50                 gcd=tmp;
 51             }
 52             R=t.rr;
 53         }
 54         Left[i].push_back(node1(L, n, gcd));
 55     }
 56 
 57     //************固定右边Right*******************
 58     Right[1].push_back(node1(1, 1, seq[1])); //最后一个特殊处理
 59     for(int i=2; i<=n; i++)
 60     {
 61         L=R=i;
 62         gcd=seq[i];
 63         for(int j=0; j<Right[i-1].size(); j++)
 64         {
 65             node1 &t=Right[i-1][j];
 66             tmp=__gcd(gcd, t.gcd);
 67             if(tmp!=gcd)
 68             {
 69                 Right[i].push_back(node1(L, R, gcd));
 70                 R=L-1;
 71                 gcd=tmp;
 72             }
 73             L=t.ll;
 74         }
 75         Right[i].push_back(node1(1, R, gcd));
 76     }
 77 }
 78 
 79 
 80 
 81 LL updateR(int L,int R)
 82 {
 83     LL ans=0;
 84     for(int i=0; i<Right[R].size(); i++)
 85     {
 86         if( L >= Right[R][i].ll && L<=Right[R][i].rr  )
 87         {
 88             ans+= ( Right[R][i].rr - L +1 ) * Right[R][i].gcd;
 89             return ans;
 90         }
 91         ans+=( Right[R][i].rr - Right[R][i].ll +1 ) * Right[R][i].gcd;
 92     }
 93 }
 94 LL updateL(int L,int R)
 95 {
 96     LL ans=0;
 97     for(int i=0; i<Left[L].size(); i++)
 98     {
 99         if(R <= Left[L][i].rr && R>=Left[L][i].ll )
100         {
101             ans+=( R - Left[L][i].ll +1 ) * Left[L][i].gcd;
102             return ans;
103         }
104         ans+=( Left[L][i].rr - Left[L][i].ll +1 ) * Left[L][i].gcd;
105     }
106 }
107 
108 void cal()
109 {
110     sort(que, que+q, cmp);
111     LL ans=0;
112     for(int i=0, L=1, R=0; i<q; i++)
113     {
114         //**************将R逐步拉到相应位置******************
115         for( ; R>que[i].R; R--)    ans-=updateR(L, R);
116         for( ; R<que[i].R; R++)    ans+=updateR(L, R+1);
117 
118         //**************将L逐步拉到相应位置******************
119         for( ; L<que[i].L; L++)    ans-=updateL( L, R);
120         for( ; L>que[i].L; L--)    ans+=updateL( L-1, R);
121 
122         que[i].ans=ans;
123     }
124     sort(que, que+q, cmp1);
125     for(int i=0; i<q; i++)    printf("%lld\n", que[i].ans);
126 }
127 
128 int main()
129 {
130     freopen("input.txt", "r", stdin);
131     int t;
132     cin>>t;
133     for(int i=0; i<N; i++) que[i].pos=i;
134     while(t--)
135     {
136         for(int i=0; i<N; i++)    Left[i].clear(),Right[i].clear();
137 
138         scanf("%d", &n);
139         for(int i=1; i<=n ; i++)    scanf("%d", &seq[i]);
140         scanf("%d", &q);
141         for(int i=0; i<q; i++)    scanf("%d %d", &que[i].L, &que[i].R);
142         blocks=sqrt(n);
143         pre_cal();    //计算辅助数组
144         cal();
145     }
146 
147     return 0;
148 }
AC代码
```
　　还有一种线段树的方法，好像差不多思想：
　　看这篇：http://www.cnblogs.com/CSU3901130321/p/4733701.html
　　附上莫队算法学习blog：http://blog.csdn.net/mlzmlz95/article/details/43644653

