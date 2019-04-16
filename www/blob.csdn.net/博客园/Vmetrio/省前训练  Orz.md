# 省前训练...Orz - Vmetrio - 博客园







# [省前训练...Orz](https://www.cnblogs.com/wangmengmeng/p/5526247.html)





## A. 异形卵


Time Limit: 1000ms

Memory Limit: 128000KB

64-bit integer IO format:      Java class name:


[Submit](http://115.159.40.116/contest_show.php?cid=35)[Status](http://115.159.40.116/contest_show.php?cid=35#status/A)[PID: 5287](http://115.159.40.116/problem_show.php?pid=5287)



我们探索宇宙，是想了解浩瀚星空的奥妙，但我们却很少意识到宇宙深处藏匿的危险，它们无时无刻不紧盯着我们的地球。如果外星人拜访我们，结果可能与哥伦布当年踏足美洲大陆不会有什么两样，这是历史，也是现实。

在ZDM-777星球上发现的休眠异形卵，其外表与常见的卵不同，表面被一层石墨覆盖。当人走近时，那层石墨开始消融，能看到里面的异形卵正在活动，异形卵是活物，具备一些热量或压力传感器这些基本的中枢神经系统，通过感知周围的热量，选择热量最大处寄生。不过，假如周围有不适合被寄生处，异形卵就选择休眠。

周围的热量可以用一串整数a1,a2,……,an来表示，异形卵具有一定的长度L，异形卵总是选择ai+ai+1+…+ai+L-1达到最大值处寄生。若周围的热量低于0，异形卵则选择休眠。

异形卵是如何感知它的寄生处呢？我们探索宇宙，是想了解浩瀚星空的奥妙，但我们却很少意识到宇宙深处藏匿的危险，它们无时无刻不紧盯着我们的地球。如果外星人拜访我们，结果可能与哥伦布当年踏足美洲大陆不会有什么两样，这是历史，也是现实。

在ZDM-777星球上发现的休眠异形卵，其外表与常见的卵不同，表面被一层石墨覆盖。当人走近时，那层石墨开始消融，能看到里面的异形卵正在活动，异形卵是活物，具备一些热量或压力传感器这些基本的中枢神经系统，通过感知周围的热量，选择热量最大处寄生。不过，假如周围有不适合被寄生处，异形卵就选择休眠。

周围的热量可以用一串整数a1,a2,……,an来表示，异形卵具有一定的长度L，异形卵总是选择ai+ai+1+…+ai+L-1达到最大值处寄生。若周围的热量低于0，异形卵则选择休眠。

异形卵是如何感知它的寄生处呢？




### Input

第一行： K 表示有多少组测试数据。 
接下来对每组测试数据有2行，第1行: L N 
第2行：a1 a2 …… aN

【约束条件】
2≤K≤5 L≤N, 1≤L≤10 1≤N≤1000 -100≤ ai≤100 
数据之间有一个空格。





### Output

对于每组测试数据，输出一行：异形卵能寄生的起始位置。若有多处可以寄生，则选择小的起始位置。若无处可以寄生，则输出0。




### Sample Input
2
3 5
30  0 100 -30 100
3 5
-100 80 -80 -100  80


### Sample Output
3
0


代码:

```
1 #include <cstdio>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <string>
 5 #include <algorithm>
 6 #include <queue>
 7 #include <map>
 8 #include <set>
 9 #include <vector>
10 #include <iostream>
11 using namespace std;
12 #define for0(i, n) for(int i=0; i<(n); ++i)
13 #define for1(i,a,n) for(int i=(a);i<=(n);++i)
14 #define for2(i,a,n) for(int i=(a);i<(n);++i)
15 #define for3(i,a,n) for(int i=(a);i>=(n);--i)
16 #define for4(i,a,n) for(int i=(a);i>(n);--i)
17 #define CC(i,a) memset(i,a,sizeof(i))
18 #define ll long long
19 #define MOD 1000000007
20 #define inf 0x3f3f3f3f
21 
22 int a[111];
23 
24 int main()
25 {
26     int l,n,i,j,k;
27     scanf("%d",&k);
28     while(k--){
29         int x;
30         int sum=-100;
31         scanf("%d%d",&l,&n);
32         for(i=0; i<n; i++)
33             scanf("%d",&a[i]);
34         for(i=0; i<=n-l; i++){
35             int p=0;
36             for(j=0; j<l; j++){
37                 p+=a[i+j];
38             }
39             if(p>sum){
40                 sum=p;
41                 x=i;
42             }
43         }
44         if(sum!=-100)
45             printf("%d\n",x+1);
46         else
47             printf("0\n");
48     }
49 }
```



## G. 摘枇杷


Time Limit: 2000ms

Memory Limit: 128000KB

64-bit integer IO format:      Java class name:


[Submit](http://115.159.40.116/contest_show.php?cid=35)[Status](http://115.159.40.116/contest_show.php?cid=35#status/G)[PID: 5264](http://115.159.40.116/problem_show.php?pid=5264)



理工学院的枇杷快熟了，ok，大家都懂得。而且大家都知道，学校的枇杷树都是一列一列的。现在小Y同学已经在筹划怎么摘枇杷了。现在我们假设有一列枇杷树，而且每棵枇杷树上枇杷果的数量小Y都已经知道了。

假设现在有n棵枇杷树，小Y可以把这n棵枇杷树分成m组，每组枇杷果的数量是这组内每棵枇杷树上枇杷果数量的和。注意，每组的枇杷树必须是连续的。（每组最少1棵树，最多n棵树）。小Y把枇杷往寝室拿的时候是一组一组拿的，所花费的力气等于这m组中枇杷果最多的那组枇杷果的数量。现在小Y想花尽量少的力气把这些枇杷果拿回寝室。




### Input

多组测试数据，以EOF结束（<= 100组）
每组测试数据第一行有两个数n（n <= 1000）和m（1 <=m <= n）
第二行有n个数，分别代表每颗树上枇杷果的数量




### Output

输出小Y同学所花费的最小的力气，每个结果占一行。




### Sample Input
3 2
1 2 3
7 5
1 4 3 1 5 2 4

### Sample Output
3
5


代码:

```
1 #include <cstdio>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <string>
 5 #include <algorithm>
 6 #include <queue>
 7 #include <map>
 8 #include <set>
 9 #include <vector>
10 #include <iostream>
11 using namespace std;
12 #define for0(i, n) for(int i=0; i<(n); ++i)
13 #define for1(i,a,n) for(int i=(a);i<=(n);++i)
14 #define for2(i,a,n) for(int i=(a);i<(n);++i)
15 #define for3(i,a,n) for(int i=(a);i>=(n);--i)
16 #define for4(i,a,n) for(int i=(a);i>(n);--i)
17 #define CC(i,a) memset(i,a,sizeof(i))
18 #define ll long long
19 #define MOD 1000000007
20 #define inf 0x3f3f3f3f
21 
22 int a[1111];
23 int n,m,maxx,sum;
24 
25 bool pd(int x)
26 {
27     int s=0,ans=0;
28     for(int i=0; i<n; i++){
29         if(a[i]>x)
30             return false;
31         if(s+a[i]>x){
32             ans++;
33             s=a[i];
34             if(ans>m-1)
35                 return false;
36         }
37         else
38             s+=a[i];
39     }
40     return true;
41 }
42 
43 int get()
44 {
45     int l,r,mid;
46     l=maxx,r=sum;
47     while(l<=r){
48         mid=(l+r)/2;
49         if(pd(mid))
50             r=mid-1;
51         else
52             l=mid+1;
53     }
54     return l;
55 }
56 
57 int main()
58 {
59     while(~scanf("%d%d",&n,&m)){
60         maxx=0,sum=0;
61         for(int i=0; i<n; i++){
62             scanf("%d",&a[i]);
63             sum+=a[i];
64             maxx=max(maxx,a[i]);
65         }
66         printf("%d\n",get());
67     }
68     return 0;
69 }
```



## E. CardTrick


Time Limit: 1000ms

Memory Limit: 128000KB

64-bit integer IO format:      Java class name:


[Submit](http://115.159.40.116/contest_show.php?cid=35)[Status](http://115.159.40.116/contest_show.php?cid=35#status/E)[PID: 5291](http://115.159.40.116/problem_show.php?pid=5291)



The magician shuffles a smallpack of cards, holds it face down and performs the following procedure:
- The top card is moved to the bottom of the pack. The new top card is dealt face up onto the table. It is the Ace of Spades.
- Two cards are moved one at a time from the top to the bottom. The next card is dealt face up onto the table. It is the Two of Spades.
- Three cards are moved one at a time…
- This goes on until the *n*th and last card turns out to be the *n* of Spades.

This impressive trick works ifthe magician knows how to arrange the cards beforehand (and knows how to give afalse shuffle). Your program has to determine the initial order of the cardsfor a given number of cards, 1 ≤ n ≤ 13.




### Input

On the first line of the input is a single positive integer k, telling the number of test cases to follow. 1 ≤ k ≤ 10 Each case consists of one line containing the integer n. 1 ≤ n ≤ 13




### Output

For each test case, output a line with the correct permutation of the values 1 to n, space separated. The first number showing the top card of the pack, etc…




### Sample Input
2
4
5


### Sample Output
2 1 4 3
3 1 4 5 2


代码:

```
1 #include <cstdio>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <string>
 5 #include <algorithm>
 6 #include <queue>
 7 #include <stack>
 8 #include <map>
 9 #include <set>
10 #include <vector>
11 #include <iostream>
12 using namespace std;
13 #define for0(i, n) for(int i=0; i<(n); ++i)
14 #define for1(i,a,n) for(int i=(a);i<=(n);++i)
15 #define for2(i,a,n) for(int i=(a);i<(n);++i)
16 #define for3(i,a,n) for(int i=(a);i>=(n);--i)
17 #define for4(i,a,n) for(int i=(a);i>(n);--i)
18 #define CC(i,a) memset(i,a,sizeof(i))
19 #define ll long long
20 #define MOD 1000000007
21 #define inf 0x3f3f3f3f
22 
23 int a[100];
24 
25 int main()
26 {
27     int n,m;
28     queue<int> q;
29     scanf("%d",&n);
30     while(n--){
31         scanf("%d",&m);
32         for(int i=0; i<m; i++){
33             q.push(i);
34         }
35         int num=1;
36         while(!q.empty()){
37             for(int i=0; i<num; i++){
38                 int x=q.front();
39                 q.pop();
40                 q.push(x);
41             }
42             int y=q.front();
43             q.pop();
44             a[y]=num++;
45         }
46         printf("%d",a[0]);
47         for(int i=1; i<m; i++)
48             printf(" %d",a[i]);
49         printf("\n");
50     }
51     return 0;
52 }
```





## A. 走迷宫


Time Limit: 1000ms

Memory Limit: 128000KB

64-bit integer IO format:      Java class name:


[Submit](http://115.159.40.116/contest_show.php?cid=36)[Status](http://115.159.40.116/contest_show.php?cid=36#status/A)


Dr.Kong设计的机器人卡多非常爱玩，它常常偷偷跑出实验室，在某个游乐场玩之不疲。这天卡多又跑出来了，在SJTL游乐场玩个不停，坐完碰碰车，又玩滑滑梯，这时卡多又走入一个迷宫。整个迷宫是用一个N * N的方阵给出，方阵中单元格中填充了一个整数，表示走到这个位置的难度。



这个迷宫可以向上走，向下走，向右走，向左走，但是不能穿越对角线。走迷宫的取胜规则很有意思，看谁能更快地找到一条路径，其路径上单元格最大难度值与最小难度值之差是最小的。当然了，或许这样的路径不是最短路径。

     机器人卡多现在在迷宫的左上角（第一行，第一列）而出口在迷宫的右下角（第N行，第N列）。

卡多很聪明，很快就找到了这样的一条路径。你能找到吗？




### Input

有多组测试数据，以EOF为输入结束的标志
第一行： N 表示迷宫是N*N方阵 (2≤ N≤ 100)
接下来有N行，	每一行包含N个整数，用来表示每个单元格中难度 (0≤任意难度≤120)。




### Output

输出为一个整数，表示路径上最高难度与和最低难度的差。




### Sample Input
5
1 1 3 6 8
1 2 2 5 5
4 4 0 3 3
8 0 2 3 4
4 3 0 2 1

### Sample Output
2


代码:

```
1 #include <cstdio>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <string>
 5 #include <algorithm>
 6 #include <queue>
 7 #include <stack>
 8 #include <map>
 9 #include <set>
10 #include <vector>
11 #include <iostream>
12 using namespace std;
13 #define for0(i, n) for(int i=0; i<(n); ++i)
14 #define for1(i,a,n) for(int i=(a);i<=(n);++i)
15 #define for2(i,a,n) for(int i=(a);i<(n);++i)
16 #define for3(i,a,n) for(int i=(a);i>=(n);--i)
17 #define for4(i,a,n) for(int i=(a);i>(n);--i)
18 #define CC(i,a) memset(i,a,sizeof(i))
19 #define LL long long
20 #define MOD 1000000007
21 #define inf 0x3f3f3f3f
22 
23 int mp[111][111],vis[111][111];
24 int dx[4]={0,0,-1,1};
25 int dy[4]={1,-1,0,0};
26 int n,l,r;
27 
28 int dfs(int x,int y,int l,int r)
29 {
30     int px,py;
31     if(x>n || y>n || x<0 || y<0)  return 0;
32     if(mp[x][y]<l || mp[x][y]>r) return 0;
33     if(vis[x][y]) return 0;
34     if(x==n && y==n)  return 1;
35     vis[x][y]=1;
36     for(int i=0; i<4; i++){
37         px=x+dx[i];
38         py=y+dy[i];
39         if(dfs(px,py,l,r))
40             return 1;
41     }
42     return 0;
43 }
44 
45 bool pd(int x)
46 {
47     for(int i=0; i<=(120-x); i++){
48         memset(vis,0,sizeof(vis));
49         if(dfs(1,1,i,i+x))
50             return true;
51     }
52     return false;
53 }
54 
55 int main()
56 {
57     while(~scanf("%d",&n)){
58         for(int i=1; i<=n; i++)
59             for(int j=1; j<=n; j++)
60                 cin>>mp[i][j];
61         l=abs(mp[n][n]-mp[1][1]);
62         //printf("%d\n",l);
63         r=120;
64         while(l<r){
65             int mid=(l+r)/2;
66             if(pd(mid))
67                 r=mid;
68             else
69                 l=mid+1;
70             //printf("%d\n",l);
71             //printf("%d\n",r);
72         }
73         printf("%d\n",l);
74     }
75 }
```





## D. Substring


Time Limit: 1000ms

Memory Limit: 128000KB

64-bit integer IO format:      Java class name:


[Submit](http://115.159.40.116/contest_show.php?cid=36)[Status](http://115.159.40.116/contest_show.php?cid=36#status/D)



You are given a string input. You are to find the longest substring of input such that the reversal of the substring is also a substring of input. In case of a tie, return the string that occurs earliest in input. 

Note well: The substring and its reversal may overlap partially or completely. The entire original string is itself a valid substring . The best we can do is find a one character substring, so we implement the tie-breaker rule of taking the earliest one first.




### Input

The first line of input gives a single integer, 1 ≤ N ≤ 10, the number of test cases. Then follow, for each test case, a line containing between 1 and 50 characters, inclusive. Each character of input will be an uppercase letter ('A'-'Z').




### Output

Output for each test case the longest substring of input such that the reversal of the substring is also a substring of input




### Sample Input
3                   
ABCABA
XYZ
XCVCX


### Sample Output
ABA
X
XCVCX
代码:

```
1 #include <cstdio>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <string>
 5 #include <algorithm>
 6 #include <queue>
 7 #include <stack>
 8 #include <map>
 9 #include <set>
10 #include <vector>
11 #include <iostream>
12 using namespace std;
13 #define for0(i, n) for(int i=0; i<(n); ++i)
14 #define for1(i,a,n) for(int i=(a);i<=(n);++i)
15 #define for2(i,a,n) for(int i=(a);i<(n);++i)
16 #define for3(i,a,n) for(int i=(a);i>=(n);--i)
17 #define for4(i,a,n) for(int i=(a);i>(n);--i)
18 #define CC(i,a) memset(i,a,sizeof(i))
19 #define LL long long
20 #define MOD 1000000007
21 #define inf 0x3f3f3f3f
22 
23 int dp[55][55];
24 char x[55],y[55];
25 
26 int main()
27 {
28     int t;
29     scanf("%d",&t);
30     while(t--){
31         int maxx=0,path=0;
32         memset(dp,0,sizeof(dp));
33         scanf("%s",x);
34         int len=strlen(x);
35         int k=len-1;
36         for(int i=0; i<len&&k>=0; i++)
37             y[i]=x[k--];
38         y[len]='\0';
39         for(int i=0; i<len; i++)
40             for(int j=0; j<len; j++){
41                 if(x[i]==y[j])
42                     dp[i+1][j+1]=dp[i][j]+1;
43                 if(dp[i+1][j+1]>maxx){
44                     maxx=dp[i+1][j+1];
45                     path=i;
46                 }
47             }
48         for(int i=path-maxx+1; i<=path; i++){
49             printf("%c",x[i]);
50         }
51         printf("\n");
52     }
53 }
```





## E. BOBSLEDDING


Time Limit: 1000ms

Memory Limit: 128000KB

64-bit integer IO format:      Java class name:


[Submit](http://115.159.40.116/contest_show.php?cid=36)[Status](http://115.159.40.116/contest_show.php?cid=36#status/E)






Dr.Kong has entered a bobsled competition because he hopes his hefty weight will give his an advantage over the L meter course (2 <= L<= 1000). Dr.Kong will push off the starting line at 1 meter per second, but his speed can change while he rides along the course. Near the middle of every meter Bessie travels, he can change his speed either by using gravity to accelerate by one meter per second or by braking to stay at the same speed or decrease his speed by one meter per second.

Naturally, Dr.Kong must negotiate N (1 <= N <= 500) turns on the way down the hill. Turn i is located T_i  meters from the course start (1 <= T_i <= L-1), and  he must be enter the corner meter at  a peed of at most S_i  meters per second (1 <= S_i <= 1000).  Dr.Kong can cross the finish line at any speed he likes.

Help Dr.Kong learn the fastest speed he can attain without exceeding the speed limits on the turns.

Consider this course with the meter markers as integers and the  turn speed limits in brackets (e.g., '[3]'):

       0    1   2   3   4   5   6   7[3]   8   9  10  11[1]  12   13[8]    14                    

(Start) |------------------------------------------------------------------------|  (Finish)   



Below is a chart of  Dr.Kong 's speeds at the beginning of each meter length of the course:

Max:                               [3]             [1]      [8]

Mtrs:   0   1   2   3   4   5   6   7   8   9  10  11  12   13   14 

Spd:    1   2   3   4   5   5   4   3   4   3   2   1   2   3    4

His maximum speed was 5 near the beginning of meter 4.







### Input

There are multi test cases,your program should be terminated by EOF
Line 1: Two space-separated integers: L and N
Lines 2..N+1: Line i+1 describes turn i with two space-separated integers: T_i and S_i




### Output

Line 1: A single integer, representing the maximum speed which Dr.Kong can attain between the start and the finish line, inclusive.




### Sample Input
14 3                                            
7 3
11 1
13 8

### Sample Output
5


代码:

```
1 #include <cstdio>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <string>
 5 #include <algorithm>
 6 #include <queue>
 7 #include <stack>
 8 #include <map>
 9 #include <set>
10 #include <vector>
11 #include <iostream>
12 using namespace std;
13 #define for0(i, n) for(int i=0; i<(n); ++i)
14 #define for1(i,a,n) for(int i=(a);i<=(n);++i)
15 #define for2(i,a,n) for(int i=(a);i<(n);++i)
16 #define for3(i,a,n) for(int i=(a);i>=(n);--i)
17 #define for4(i,a,n) for(int i=(a);i>(n);--i)
18 #define CC(i,a) memset(i,a,sizeof(i))
19 #define LL long long
20 #define MOD 1000000007
21 #define inf 0x3f3f3f3f
22 
23 int ao[1111];
24 
25 int main()
26 {
27     int l,n,x,y;
28     while(~scanf("%d%d",&l,&n)){
29         memset(ao,0,sizeof(ao));
30         int p,sd;
31         for(int i=0; i<n; i++){
32             scanf("%d%d",&x,&y);
33             ao[x]=y;
34         }
35         sd=1,p=1;
36         for(int i=0; i<=l; i++){
37             if(ao[i]==0)
38                 ao[i]=sd;
39             else{
40                 if(ao[i]>ao[i-1])
41                     ao[i]=ao[i-1]+1;
42                 else{
43                     p=i;
44                     while(ao[p-1]-ao[p]>1){
45                         ao[p-1]=ao[p]+1;
46                         p--;
47                     }
48                 }
49             }
50             sd=ao[i]+1;
51         }
52         int maxx=0;
53         for(int i=0; i<=l; i++)
54             maxx=max(maxx,ao[i]);
55         printf("%d\n",maxx);
56     }
57 }
```














