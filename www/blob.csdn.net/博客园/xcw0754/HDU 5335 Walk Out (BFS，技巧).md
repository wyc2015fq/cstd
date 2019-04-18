# HDU 5335 Walk Out (BFS，技巧) - xcw0754 - 博客园
# [HDU 5335 Walk Out (BFS，技巧)](https://www.cnblogs.com/xcw0754/p/4691424.html)
题意：有一个n*m的矩阵，每个格子中有一个数字，或为0，或为1。有个人要从（1，1）到达（n，m），要求所走过的格子中的数字按先后顺序串起来后，用二进制的判断大小方法，让这个数字最小。前缀0不需要输出！！
思路：主要考虑的是BFS解决。
　　如果grid[1，1]=1，那么这个二进制的位数也就定下来了，是n+m-1，很好解决，每个格子就只能往下或者往右，否则长度一定超过n+m+1，必定不是最优。
　　如果grid[1，1]=0，那么可能会出现绕了一个S型到终点的结果为0而已。所以不能用老办法，要先预处理一下。处理方式是，用BFS将所有grid[1，1]可达的0点标记出来，找出其中距离终点最近的那些0点（可能多个），如果他们的下和右边的点为1，这些点都进队，再用上边方式BFS即可求得答案（上面只是1个起点，这边有多个起点，不影响正确性）。
　　答案在哪？其实在BFS时每一层只能是0点或者是1点，为什么呢？如果有0点的话，还需要选择1点的吗？别忘了二进制的位数是固定了，选0肯定比选1要好，则在没有0的情况下再选1的。 在遍历时按层遍历，遍历到的点先分到两个集合A0和B1中，择所需即可，所以在遍历第i层时第i位的答案也就决定了。这是剪枝！
　　注意考虑只有1个点，2个点和S形等各种情况。
```
1 #include <bits/stdc++.h>
  2 #define INF 0x7f7f7f7f
  3 #define pii pair<int,int>
  4 #define LL unsigned long long
  5 using namespace std;
  6 const int N=1005;
  7 int n, m;
  8 char grid[N][N];
  9 vector<int> ans;
 10 int inq[N][N];
 11 
 12 void BFS(deque<pii> &que)
 13 {
 14     ans.push_back(1);
 15     while(!que.empty())
 16     {
 17         deque<pii> que0, que1;      //两种到达的方式，只取其一
 18         int siz=que.size();
 19         for(int i=0; i<siz; i++)    //被更新的都是同一源头的。
 20         {
 21             int a=que.front().first;
 22             int b=que.front().second;
 23             que.pop_front();
 24 
 25             if( a+1<=n && !inq[a+1][b] )    //下:要么你无路径可达，要么我比你小，我才更新你
 26             {
 27                 if(grid[a+1][b]=='0')    que0.push_back(make_pair(a+1,b));
 28                 else                     que1.push_back(make_pair(a+1,b));
 29             }
 30             if( b+1<=m && !inq[a][b+1] )    //右
 31             {
 32                 if(grid[a][b+1]=='0')    que0.push_back(make_pair(a,b+1));
 33                 else                     que1.push_back(make_pair(a,b+1));
 34             }
 35             inq[a+1][b]=inq[a][b+1]=1;
 36         }
 37 
 38         if(!que0.empty())    ans.push_back(0);
 39         else                 ans.push_back(1);
 40 
 41         if(!que0.empty())    que.insert(que.end(), que0.begin(), que0.end() );
 42         else                 que.insert(que.end(), que1.begin(), que1.end() );
 43     }
 44 }
 45 
 46 
 47 int cal()
 48 {
 49     memset(inq, 0, sizeof(inq));
 50 
 51     deque<pii> que;que.push_back( make_pair(1,1));
 52     if(grid[1][1]=='0') //若起点为0，找到所有离终点最近的前缀0
 53     {
 54         inq[1][1]=1;
 55         while(!que.empty())
 56         {
 57             int siz=que.size();
 58             for(int i=0; i<siz; i++)    //按层来BFS，按层记录最优答案
 59             {
 60                 int a=que.front().first;
 61                 int b=que.front().second;
 62                 que.pop_front();
 63 
 64                 if(a+1<=n && !inq[a+1][b] && grid[a+1][b]=='0')    que.push_back(make_pair(a+1,b));
 65                 if(a-1>0  && !inq[a-1][b] && grid[a-1][b]=='0')    que.push_back(make_pair(a-1,b));
 66 
 67                 if(b+1<=m && !inq[a][b+1] && grid[a][b+1]=='0')    que.push_back(make_pair(a,b+1));
 68                 if(b-1>0  && !inq[a][b-1] && grid[a][b-1]=='0')    que.push_back(make_pair(a,b-1));
 69 
 70                 inq[a+1][b]=inq[a-1][b]=inq[a][b+1]=inq[a][b-1]=1;  //防止重复进队
 71             }
 72         }
 73         int min_dis=INF;
 74         for(int i=1; i<=n; i++) //求最近的0距离终点的最小距离
 75         {
 76             for(int j=1; j<=m; j++)
 77             {
 78                 if(inq[i][j]&&grid[i][j]=='0')
 79                     min_dis=min(min_dis, n+m-j-i);
 80             }
 81         }
 82         if(grid[n][m]=='0' && min_dis==0) return 0;   //有0路可达终点
 83         for(int i=1; i<=n; i++)         //扫出距离为min_dis的所有0点
 84             for(int j=1; j<=m; j++)
 85                 if(inq[i][j] && grid[i][j]=='0' && min_dis==n+m-j-i && n+m-i-j!=0 )
 86                     que.push_back(make_pair(i,j));
 87 
 88         memset(inq,0,sizeof(inq));
 89         int siz=que.size();
 90         for(int i=0; i<siz; i++)    //将所有0点的下和右为1的点进队
 91         {
 92             int a=que.front().first;
 93             int b=que.front().second;
 94             que.pop_front();
 95             if(a+1<=n&&!inq[a+1][b]&&grid[a+1][b]=='1')    que.push_back(make_pair(a+1,b));
 96             if(b+1<=m&&!inq[a][b+1]&&grid[a][b+1]=='1')    que.push_back(make_pair(a,b+1));
 97             inq[a+1][b]=inq[a][b+1]=1;
 98         }
 99     }
100     BFS(que);
101     return ans.size();
102 }
103 
104 int main()
105 {
106     freopen("input.txt", "r", stdin);
107     int t, a, b;
108     char c;
109     cin>>t;
110     while(t--)
111     {
112         ans.clear();
113         scanf("%d %d",&n,&m);
114 
115         for(int i=1; i<=n; i++) //输入要注意
116             for(int j=1; j<=m; j++)
117             {
118                 c=getchar();
119                 if(c=='0'||c=='1' ) grid[i][j]=c;
120                 else    j--;
121             }
122         int s=cal();
123         if(s==0)    printf("0");
124         else    for(int i=0; i+1<ans.size(); i++)    printf("%d",ans[i]);//最后一个数字多余
125         printf("\n");
126     }
127     return 0;
128 }
AC代码
```

