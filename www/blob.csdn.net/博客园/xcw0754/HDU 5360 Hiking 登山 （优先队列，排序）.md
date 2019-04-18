# HDU 5360 Hiking 登山 （优先队列，排序） - xcw0754 - 博客园
# [HDU 5360 Hiking 登山 （优先队列，排序）](https://www.cnblogs.com/xcw0754/p/4710446.html)
题意：
　　有n个人可供邀请去hiking，但是他们很有个性，每个人都有个预期的人数上下限[Li,Ri]，只有当前确定会去的人数在这个区间内他才肯去。一旦他答应了，无论人数怎样变更，他都不会反悔。问最多能邀请多少人去？输出邀请顺序（包括那些不去的人）。
思路：
　　有点像贪心法的《活动时间安排》？但是本题的人数是会变化的，会动态影响到后面的人的决策。
　　下面以“去邀请谁”作为一个决策。每次决策时，要考虑所有符合条件的人，其中对人数上限要求低的先挑，然后人数cnt++。在此次决策之后，可能又有人符合条件了，将这些人与前面剩下的人一块考虑，依然挑出那个对人数上限要求低的人，注意，随着cnt的增大，前面剩下的人之中也会出现一些不符合条件的了，这些人不予以考虑。
　　瓶颈在每次都要挑出那个对人数上限要求低的人，而这可以用堆来实现。所以总复杂度为O(nlogn)。
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL long long
 5 using namespace std;
 6 const int N=101000;
 7 struct node
 8 {
 9     int down, up, pos;
10     node(){};
11     node(int down ,int up, int pos):down(down),up(up),pos(pos){};
12     bool operator <(const node &b) const
13     {
14         return up==b.up? down>b.down: up>b.up;
15     } //up小的在前面
16 }a[N];
17 inline int cmp(node a,node b){return  a.down < b.down;}
18 
19 
20 vector<int> ans;
21 int cal(int n)
22 {
23     sort(a, a+n, cmp);      //按起始数字排序
24     priority_queue<node> que;
25     int left=0, num=0;
26     while(left<n || !que.empty() )
27     {
28         while(left<n && (num==a[left].down || que.empty()) )
29             que.push(a[left++]);    //把符合条件的全部装进队列。或者队列为空时也必须装。
30 
31         while( !que.empty() )        //处理队列中符合条件的，每次之多处理1个符合条件的。
32         {
33             node t=que.top();que.pop();
34             ans.push_back(t.pos);       //无论是否能邀请到，都必须出来。
35 
36             if( num>=t.down && num<=t.up )
37             {
38                 num++;          //一旦邀请到一个，num改变，可能又有符合条件的人要进队了。
39                 break;          //所以要退出，也许另一个着急的人才即将要进队。
40             }
41         }
42     }
43     return num;
44 }
45 
46 int main()
47 {
48     freopen("input.txt", "r", stdin);
49     int t, n;
50     cin>>t;
51     while(t--)
52     {
53         scanf("%d",&n);
54         ans.clear();
55         for(int i=0; i<n; i++)  scanf("%d",&a[i].down);
56         for(int i=0; i<n; i++)  scanf("%d",&a[i].up);
57         for(int i=0; i<n; i++)  a[i].pos=i+1;
58 
59         printf("%d\n", cal(n));
60         for(int i=0; i<ans.size(); i++) printf("%d ",ans[i]);
61         cout<<endl;
62     }
63     return 0;
64 }
AC代码
```

