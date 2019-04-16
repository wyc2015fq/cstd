# 胡小兔的OI日志3 完结版 - 胡小兔 - 博客园







# [胡小兔的OI日志3 完结版](https://www.cnblogs.com/RabbitHu/p/OI_Diary_3.html)





# 胡小兔的 OI 日志 3 (2017.9.1 ~ 2017.10.11）

标签： 日记

# [查看最新](#返回顶部)

## 2017-09-02 [51nod 1378](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1378) 夹克老爷的愤怒 | 树形DP

> 
夹克老爷逢三抽一之后，由于采用了新师爷的策略，乡民们叫苦不堪，开始组织起来暴力抗租。

夹克老爷很愤怒，他决定派家丁常驻村中进行镇压。

诺德县 有N个村庄，编号0 至 N-1，这些村庄之间用N - 1条道路连接起来。

家丁都是经过系统训练的暴力机器，每名家丁可以被派驻在一个村庄，并镇压当前村庄以及距离该村庄不超过K段道路的村庄。

夹克老爷一贯奉行最小成本最大利润的原则，请问要实现对全部村庄的武力控制，夹克老爷需要派出最少多少名家丁?

Input

第1行：2个数N, K中间用空格分隔(1<= N <= 100000, 0 <= K <= N)。

之后N-1行：每行2个数S, E中间用空格分隔，表示编号为S的村庄同编号为E的村庄之间有道路相连。(0 <= S, E < N)。

Output

输出一个数说明要实现对全部村庄的武力控制，夹克老爷需要派出最少多少名家丁?


有趣的树形DP？

使用贪心策略，由叶子节点向根，能不放就不放。

dp[i]如果是正的，表示i还能向上覆盖多少节点；如果是负的，说明当前节点至少需要向下覆盖多少节点。

在每个点的所有儿子中，找到dp的最小值。

如果最小值等于-k，则当前节点必须要放家丁。

else 如果最大值能覆盖最小值，则当前节点不用放，dp[当前节点] = 最大值 - 1。

else 如果最大值不能覆盖最值，则dp[当前节点] = 最小值 - 1。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
const int N = 100005;
int n, k;
int ecnt, adj[N], nxt[N * 2], go[N * 2], dp[N], ans;
void add(int u, int v){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
void dfs(int u, int pre){
    int minn = 0, maxn = 0, v;
    for(int e = adj[u]; e; e = nxt[e]){
        if((v = go[e]) == pre) continue;
        dfs(v, u);
        maxn = max(maxn, dp[v]);
        minn = min(minn, dp[v]);
    }
    if(minn == -k) ans++, dp[u] = k;
    else if(maxn + minn > 0) dp[u] = maxn - 1;
    else dp[u] = minn - 1;
}
int main(){
    scanf("%d%d", &n, &k);
    for(int i = 1, u, v; i < n; i++)
        scanf("%d%d", &u, &v), add(u + 1, v + 1), add(v + 1, u + 1);
    dfs(1, 0);
    if(dp[1] < 0) ans++;
    printf("%d\n", ans);
    return 0;
}
```

## 2017-09-03 [51nod 1398](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1398) 等公交 | DP 期望

> 
小镇的公交车站里有N辆公交，标号为0,1,2，...,N-1。这个小镇的公交运作模式比较奇葩，当必须有一辆车离开车站时，系统会随机从N辆车中选择一辆车，其中任意一辆车i被选中的概率为prob[i]/100，当车i被选中后它会离开车站，并且在之后的time[i]的时间内完成它的行程并返回车站。然后系统又开始随机选N辆车之一（存在同一辆车被连续多次选中的可能）。这个车站在0时刻发出第一班车。如果你在s时刻到达车站，且一定搭乘下一班车，那么你等待的时间的期望是多少？

Input

多组测试数据，第一行一个整数T，表示测试数据数量，1<=T<=4

每组测试数据有相同的结构构成：

每组数据第一行包含两个整数N与s，即车辆总数与到达时刻，其中1<=N<=100 ,0<=s<=100000;

之后N行，每行两个整数表示车i的信息，time[i]与prob[i],其中1<=prob[i]<=100,1<=time[i]<=100000;

其中确保prob[i]之和为100.

Output

每组数据一行输出，即等待时间的期望。（四舍五入小数后保留4位数字）


棵DP。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
#define NL putchar('\n')
using namespace std;
typedef long long ll;
int read(){
    char c; bool op = 0;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
void write(int x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 103, S = 100003;
int T, n, s, tim[N];
double prob[N], dp[S];

int main(){
    T = read();
    while(T--){
        memset(dp, 0, sizeof(dp));
        n = read(), s = read();
        for(int i = 1; i <= n; i++)
            tim[i] = read(), prob[i] = read() / 100.0;
        for(int i = s - 1; i >= 0; i--)
            for(int j = 1; j <= n; j++)
                if(i + tim[j] >= s) dp[i] += prob[j] * (i + tim[j] - s);
                else dp[i] += prob[j] * dp[i + tim[j]];
        printf("%.4lf\n", dp[0]);
    }
    return 0;
}
```

## 2017-09-04 [51nod 1437](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1437) 迈克步 | 单调栈

> 
有n只熊。他们站成一排队伍，从左到右依次1到n编号。第i只熊的高度是ai。

一组熊指的队伍中连续的一个子段。组的大小就是熊的数目。而组的力量就是这一组熊中最小的高度。

迈克想知道对于所有的组大小为x（1 ≤ x ≤ n）的，最大力量是多少。


熟悉的单调栈！

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
#define newline putchar('\n');
using namespace std;
typedef long long ll;
int read(){
    bool op = 0; char c;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
void write(int x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}

const int N = 200005;
int n, a[N], stk[N], top, l[N], r[N], w[N];

int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        a[i] = read();
    for(int i = 1; i <= n; i++){
        while(top && a[stk[top]] >= a[i])
            r[stk[top]] = i, top--;
        l[i] = stk[top];
        stk[++top] = i;
    }
    while(top){
        r[stk[top]] = n + 1;
        top--;
    }
    for(int i = 1; i <= n; i++)
        w[r[i] - l[i] - 1] = max(w[r[i] - l[i] - 1], a[i]);
    for(int i = n - 1; i; i--)
        w[i] = max(w[i], w[i + 1]);
    for(int i = 1; i <= n; i++)
        write(w[i]), putchar(' ');
    newline;
    return 0;
}
```

## 2017-09-04 [51nod 1444](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1444) 破坏道路 | 最短路

> 
在某一个国家，那儿有n个城市，他们通过m条双向道路相连。城市从1到n编号。如果城市a和b通过一条道路直接相连，那么他们之间的距离就是一个小时。这个国家的道路网络可以允许你从任意一个城市到达另外的城市。

现在你要破坏尽可能多的道路，但是要保证从城市s1到t1不超过l1小时，并且从城市s2到t2不超过l2小时。

输出最多可以破坏的道路数目，如果没有解，请输出-1

Input

单组测试数据。

第一行有两个整数n,m(1 ≤ n ≤ 3000, n-1 ≤ m ≤ min(3000,n*(n-1)/2) )。

接下来m行，每行有两个整数 ai, bi (1 ≤ ai, bi ≤ n, ai ≠ bi)，表示ai和bi之间有一条道路。

输入保证是一个连通图。

最后两行每行有三个整数s1, t1, l1和 s2, t2, l2, (1 ≤ si, ti ≤ n, 0 ≤ li ≤ n)。

Output

输出一个整数，表示最多可以破坏的道路数目，如果没有解，输出-1。


因为每条边的长度都是1，所以可以 BFS 在 \(O(n^2)\) 的时间内处理每两点之间的最短路长度。

我们的目的是留下的边越少越好。由于有两条路径，如果两条路径有重叠则可以减少留下的边。\(O(n^2)\) 枚举重叠路径(i -> j)，如果 dis[s1][i] + dis[j][t1] + dis[i][j] <= l1 && dis[s2][i] + dis[j][t2] + dis[i][j] <= l2 可以更新。要注意的是，如果dis[s1][i] + dis[j][t1] + dis[i][j] <= l1 && dis[s2][j] + dis[i][t2] + dis[i][j] <= l2 也可以更新，因为两个s不一定总是连着重叠路径的同一端点。

另外还要注意的是，两条边可以没有重叠路径，所以一开始ans要设为只保留两条最短路的答案。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
#define newline putchar('\n');
using namespace std;
typedef long long ll;
int read(){
    bool op = 0; char c;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
void write(int x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
/********* 以上是模板 *********/
const int N = 3005;
int n, m, ans, l, r;
int ecnt, adj[N], go[2 * N], nxt[2 * N];
int dis[N][N], q[N];
int s1, t1, l1, s2, t2, l2;
bool vis[N];

void add(int u, int v){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
void bfs(){
    for(int i = 1; i <= n; i++){
        l = 1, r = 0; //清空队列
        memset(vis, 0, sizeof(vis)); //清空vis
        q[++r] = i, vis[i] = 1;
        while(l <= r){
            int u = q[l++], v; //取出队首u
            for(int e = adj[u]; e; e = nxt[e])
                if(!vis[v = go[e]])
                    vis[v] = 1, q[++r] = v, dis[i][v] = dis[i][u] + 1;
            //记录vis，将v放入队列，更新v的dis
        }
    }
}

int main(){
    n = read(), m = read();
    for(int i = 1, u, v; i <= m; i++){
        u = read(), v = read();
        add(u, v), add(v, u);
    }
    bfs();
    s1 = read(), t1 = read(), l1 = read();
    s2 = read(), t2 = read(), l2 = read();
    if(dis[s1][t1] > l1 || dis[s2][t2] > l2){
        printf("-1\n");
        return 0;
    }
    ans = m - dis[s1][t1] - dis[s2][t2]; //就差这一句！
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            if(dis[s1][i] + dis[j][t1] + dis[i][j] <= l1 && dis[s2][i] + dis[j][t2] + dis[i][j] <= l2) 
                ans = max(ans, m - dis[i][j] - dis[s1][i] - dis[j][t1] - dis[s2][i] - dis[j][t2]);
            if(dis[s1][i] + dis[j][t1] + dis[i][j] <= l1 && dis[s2][j] + dis[t2][i] + dis[i][j] <= l2)
                ans = max(ans, m - dis[i][j] - dis[s1][i] - dis[j][t1] - dis[s2][j] - dis[t2][i]);
        }
    }
    printf("%d\n", ans);
    return 0;
}
```

## 2017-09-04 [51nod 1451](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1451) 合法三角形 | 计算几何？

> 
有n个不同的点，问有多少组三元组能构成面积非0的三角形。

Input

单组测试数据。

第一行一个整数n (1 ≤ n ≤ 2000)，表示点的数目。

接下来n行，每行包含两个整数 xi, yi ( -100 ≤ xi, yi ≤ 100)，表示第i个点的坐标。输入保证点是两两不同的。

Output

输出合法的三角形数目。


枚举每个点，将其他点按照与这个点连线的斜率排序，遇到斜率相同的说明共线，共线则所有由“枚举的那个点与线上的其他两个点”构成的三角形都不合法。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
#define newline putchar('\n');
using namespace std;
typedef long long ll;
int read(){
    bool op = 0; char c;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
void write(int x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
/********* 以上是模板 *********/
const int N = 2005;
struct poi {
    int x, y;
    bool operator < (poi b) const{ //比较斜率
        if(x == 0) return 0;
        if(b.x == 0) return 1;//特判：没有斜率的算是最大
        if(x * b.x > 0) return y * b.x < x * b.y;
        else return y * b.x > x * b.y;
    }
    bool operator == (poi b) const{
        if(x * b.x == 0 && x + b.x != 0) return 0;
        return y * b.x == x * b.y;
    }
} a[N], sorter[N];
int n;
ll ans;
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        a[i].x = read(), a[i].y = read();
    for(int i = 1; i <= n; i++){ //i 与 其他两个点 形成三角形
        int cnt = 0;
        for(int j = 1; j <= n; j++)
            if(j != i){
                sorter[++cnt].x = a[j].x - a[i].x; //将除了i以外的点形成的向量 i -> j 放进sorter排序
                sorter[cnt].y = a[j].y - a[i].y;
            }
        sort(sorter + 1, sorter + n); //按照与i形成的直线斜率排序
        ans += (n - 1) * (n - 2) / 2;
        for(int j = 2; j < n; j++){
            int cnt = 1;
            while(j < n && sorter[j] == sorter[j - 1])
                cnt++, j++;
            ans -= cnt * (cnt - 1) / 2;
        }
    }
    printf("%lld\n", ans / 3);
    return 0;
}
```

@ 快速读入记得判负数………………

## 2017-09-05 [51nod 1432](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1452) 加括号 | 高精度

> 
有一个算术表达式 x1 Δ x2 Δ x3 Δ ,..., Δ xn， x1,x2,x3,...,xn 是1到 9的数字， Δ是'+'或者'*'。现在要求你在这个表达式中加一对括号，使得这个式子的值最大。样例解释：3 + 5 * (7 + 8) * 4 = 303。


可以发现, 加的括号一定是两端外面都有乘号或者一端靠着算式的尽头, 而乘号的个数非常少, 那么暴力枚举一下加括号的位置即可.

高精度别开太大了, 会T......

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
const int N = 2505;
struct bigint {
    short l, n[1005];
    bigint(){
    l = 1;
    memset(n, 0, sizeof(n));
    }
    void operator = (int x){
    memset(n, 0, sizeof(n));
        if(!x)
        l = 1;
    else{
        l = 0;
        while(x)
        n[++l] = x % 10, x /= 10;
    }
    }
    bigint operator + (const bigint &b) const{
    bigint c;
    c.l = max(l, b.l) + 1;
    for(int i = 1; i <= c.l; i++){
        c.n[i] += n[i] + b.n[i];
        c.n[i + 1] += c.n[i] / 10;
        c.n[i] %= 10;
    }
    if(!c.n[c.l]) c.l--;
    return c;
    }
    bigint operator * (const bigint &b) const{
    bigint c;
    c.l = l + b.l;
    for(int i = 1; i <= l; i++)
        for(int j = 1; j <= b.l; j++)
        c.n[i + j - 1] += n[i] * b.n[j];
    for(int i = 1; i <= c.l; i++)
        c.n[i + 1] += c.n[i] / 10, c.n[i] %= 10;
    if(!c.n[c.l]) c.l--;
    return c;
    }
    bool operator < (const bigint &b) const{
    if(l != b.l) return l < b.l;
    for(int i = l; i; i--)
        if(n[i] != b.n[i])
        return n[i] < b.n[i];
    return 0;
    }
    void out(){
    for(int i = l; i; i--) putchar('0' + n[i]);
    putchar('\n');
    }
} ans;
int n, nxt[N], pre[N], lst[N], lcnt = 1;
char str[2*N];
bool op[N];
bigint data[N], mdata[N];
void init(){
    nxt[0] = 1, pre[n + 1] = n, nxt[n + 1] = n + 1;
    for(int i = 1; i <= n; i++)
        data[i] = mdata[i], pre[i] = i - 1, nxt[i] = i + 1;
}
void multi(int l, int r){ // deal with [l, r)
    for(int i = nxt[l]; i < r; i = nxt[i])
    if(op[i]){
        data[pre[i]] = data[pre[i]] * data[i]; //update previous data
        nxt[pre[i]] = nxt[i], pre[nxt[i]] = pre[i]; //delete i
    }
}
void add(int l, int r){
    for(int i = nxt[l]; i < r; i = nxt[i]){
    data[pre[i]] = data[pre[i]] + data[i];
    nxt[pre[i]] = nxt[i], pre[nxt[i]] = pre[i];
    }
}
int main(){
    scanf("%s", str);
    int len = strlen(str);
    n = len / 2 + 1;
    lst[1] = 1; // we can add a '(' on the left of 1
    mdata[1] = str[0] - '0';
    for(int i = 2; i < len; i += 2){
    op[i / 2 + 1] = (str[i - 1] == '*');
    if(str[i - 1] == '*') lst[++lcnt] = i / 2 + 1;
    mdata[i / 2 + 1] = str[i] - '0';
    }
    lst[++lcnt] = n + 1; // we can also add a ')' on the right of n, namely, on the left of n + 1
    for(int i = 1; i <= lcnt; i++)
    for(int j = i; j <= lcnt; j++){
        init();
        multi(lst[i], lst[j]);
        add(lst[i], lst[j]);
        multi(1, n + 1);
        add(1, n + 1);
        if(ans < data[1]) ans = data[1];
    }
    ans.out();
    return 0;
}
```

## 2017-09-06 [51nod 1454](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1454&judgeId=336519) 升排列 | 找规律

> 
定义长度为n的排列为数组 p = [p1, p2, ..., pn] ，这个数组包含n个整数，他们都在1到n之间，并且两两不同。我们说这个排列把1映射到 p1 ，2映射到 p2 ，依此类推。

下面介绍一下排列的循环表示。一个环是一串数字，这一串数字中每一个数字被映射到下一个数字，最后一个数字被映射到第一个数字。排列p的循环表示是由一系列的环构成的。比如排列p = [4, 1, 6, 2, 5, 3]的循环表示是 (142)(36)(5)，因为1映射到4，4映射到2，2映射到1，3和6相互映射，5是自己映射自己。

排列可能会有多个循环表示，所以这儿再定义一个标准的循环表示。第一步，把每一个环中的数字按照降序排列。然后把每一个环按照环中最大的数字升序排列。对于上面的例子， [4, 1, 6, 2, 5, 3]的标准循环表示是(421)(5)(63)。

把环旁边的圆括号拿掉之后我们会得到另外一个排列，比如[4, 1, 6, 2, 5, 3] 会得到 [4, 2, 1, 5, 6, 3]。

一些排列经过上述的变换之后，排列是不变的。现在我们把所有长度为n的满足这样的条件的排列按照字典序排列。请你找出排在第k位的是什么排列。

样例解释：标准循环表示为(1)(32)(4)，拿掉括号之后就是[1 3 2 4]。第一个排列是 [1, 2, 3, 4]，第二个是[1, 2, 4, 3]。


可以通过打表 ~~或看题解~~ 等方式发现：合法的序列只能是 1~n 排列在一些位置交换相邻两个数（每个数只能被交换一次）得到的序列。

首先我们想想如何对于序列长度n求出这种合法序列的个数呢？

设 f[x] 为合法序列个数，则序列最左边的元素可能被交换过，也有可能没被交换过。没交换过的情况与长为 n - 1 的序列一一对应；交换过的情况下，左边第一个元素一定是和左边第二个元素交换的，这种情况与长为 n - 2 的序列一一对应。所以 f[x] = f[x - 1] + f[x - 2]。

怎么又是斐波那契！

接下来考虑怎么求出要求的答案。

最左数交换过后（变成次左数）一定会变大，那么最左数交换过的序列的字典序一定比没交换过的更大。

假设当前要处理的序列长度为 i，要找第 k 大的，那么最左数不交换的方案有 f[i - 1] 种，如果 k <= f[i - 1]，那么左数不用交换，接下来在 i - 1 长度的序列中找第 k 个；如果 k > f[i - 1]，则最左数要交换，接下来在 i - 2 长度的序列中找地 k - f[i - 1] 个。

```
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
const int N = 55;
ll fib[N];
ll n, k, ans[N];
int main(){
    fib[0] = fib[1] = 1;
    for(int i = 2; i < 55; i++)
        fib[i] = fib[i - 1] + fib[i - 2];
    scanf("%lld%lld", &n, &k);
    for(int i = 1; i <= n; i++){
        if(k <= fib[n - i]) ans[i] = i;
        else {
            ans[i] = i + 1, ans[i + 1] = i;
            k -= fib[n - i];
            i++;
        }
    }
    for(int i = 1; i <= n; i++)
        printf("%lld ", ans[i]);
    putchar('\n');
    return 0;
}
```

## 2017-09-07 [51nod 1461](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1461) 稳定桌 | 线段树 树状数组

> 
有一张桌子，有n个腿。第i根腿的长度是li。

现在要拿掉一些腿，使得桌子稳定，拿掉第i根腿需要di的能量。

稳定的条件是，假如拿掉若干条腿之后，桌子还有k个腿，那么长度最长的腿的数目要超过一半。比如桌子有5根腿，那么至少要有三根腿是最长的。另外，只有一根腿的桌子是稳定的，两个腿的桌子想要稳定，必需长度是一样的。

你的任务是拿掉若干腿，使得桌子稳定，并且所消耗的能量要最少。

Input

单组测试数据。

第一行有一个整数n (1 ≤ n ≤ 10^5)，表示刚开始桌子腿的数目。

第二行包含n个整数li (1 ≤ li ≤ 10^5)，表示第i个腿的长度。

第三行包含n个整数 di (1 ≤ di ≤ 10^5)，表示拿掉第i个腿所消耗的能量。

Output

输出使得桌子稳定所消耗的最少能量。


数据结构只会线段树2333

把所有腿儿按照长度排个序，从最长的开始砍（是真的砍，就是在数据结构中删除这些腿儿），记住已经砍了多少，可以算出为了使桌子稳定还需要砍多少（设还要砍x个），然后在某数据结构中找出当前还剩下的腿儿中权值最小的x个来砍（不是真的砍，就是不用删除，只是求出权值和），然后得到了当前答案。取最小答案即可。

那么这个神奇的无所不能的“某数据结构”是啥呢？

我写的是一个类似约瑟夫问题的线段树加上一个树状数组求前缀和……

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>
#define INF 0x3f3f3f3f3f3f3f3f
using namespace std;
typedef long long ll;
ll read(){
    char c; bool op = 0;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    ll ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005;
struct leg {
    ll l, w, rank;
    bool operator < (const leg &b) const{
        return w < b.w;
    }
} legs[N];
bool cmp(const leg &a, const leg &b){
    return a.l > b.l;
}
//所有腿一开始按照w排序后分配rank、建树、建树状数组，再按l排序
int n;
ll tree[N], ans = INF, sumw; //树状数组记录腿的w的前缀和(腿按w从小到大排序)
int alive[4*N]; //约瑟夫线段树记录还有几个活腿
void tree_add(int p, ll x){
    while(p <= n) tree[p] += x, p += p & -p;
}
ll tree_ask(int p){
    ll res = 0;
    while(p) res += tree[p], p -= p & -p;
    return res;
}
void alive_build(int k, int l, int r){
    if(l == r) return (void) (alive[k] = 1);
    int mid = (l + r) >> 1;
    alive_build(k << 1, l, mid);
    alive_build(k << 1 | 1, mid + 1, r);
    alive[k] = alive[k << 1] + alive[k << 1 | 1];
}
int alive_query(int k, int l, int r, int x){ //问现存的第x个w是谁
    if(l == r) return l; // 现存的第x个w是一开始的第l个w
    int mid = (l + r) >> 1;
    if(x <= alive[k << 1]) return alive_query(k << 1, l, mid, x);
    return alive_query(k << 1 | 1, mid + 1, r, x - alive[k << 1]);
}
void alive_erase(int k, int l, int r, int x){ //删除x号w
    if(l == r) return (void) (alive[k] = 0);
    int mid = (l + r) >> 1;
    if(x <= mid) alive_erase(k << 1, l, mid, x);
    else alive_erase(k << 1 | 1, mid + 1, r, x);
    alive[k] = alive[k << 1] + alive[k << 1 | 1];
}
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        legs[i].l = read();
    for(int i = 1; i <= n; i++)
        legs[i].w = read();
    sort(legs + 1, legs + n + 1);
    for(int i = 1; i <= n; i++)
        tree_add(i, legs[i].w), legs[i].rank = i; //把按w排好的腿的权值放进去
    sort(legs + 1, legs + n + 1, cmp); //按长度从大到小排序
    alive_build(1, 1, n);
    for(int i = 1; i <= n;){ //腿的编号i，与i号腿长度相等的腿数tot, 已砍的腿总w为sumw
        int tot = 0;
        ll tmpsumw = 0;
        while(legs[i + tot].l == legs[i].l){ //要把长度和i号腿相等的腿都从树中删掉
            alive_erase(1, 1, n, legs[i + tot].rank); //在线段树中删除rank号w
            tree_add(legs[i + tot].rank, -legs[i + tot].w); //在树状数组中删除rank号w
            tmpsumw += legs[i + tot].w; //放在tmpsumw里面
            tot++;
        }
        if(tot > (n - i + 1) / 2) //如果tot大于剩下的腿的一半
            ans = min(ans, sumw); //则只砍现在已经砍的这些腿就够了
        else{
            int need = n - i + 2 - 2 * tot;// 还需要砍多少条腿：(n - i + 1) - (2 * tot - 1)
            int till = alive_query(1, 1, n, need); //找一下w第need大的腿的rank
            ans = min(ans, sumw + tree_ask(till)); //还需要耗费w第need大的腿的w前缀和那么多能量
        }
        sumw += tmpsumw;
        i += tot;
    }
    printf("%lld\n", ans);
    return 0;
}
```

@ 不开longlong见祖宗……

## 2017-09-07 [51nod 1463](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1463) 找朋友 | 树状数组 离线

> 
给定：

两个长度为n的数列A 、B

一个有m个元素的集合K

询问Q次

每次询问[l,r]，输出区间内满足|Bi-Bj|∈K 的最大Ai+Aj

数据约定：

n,Q<=100000

m <= 10

0<=A[i]<=1000000000

1<=B[i]<=n

1<=K[i]<=n

保证B[i]互不相等


B数组是1~n的排列，所以每个值只对应一个位置。

将询问离线读入。

从右向左扫一遍数组，设当前位置为i，如果满足|Bi - Bj| ∈ K 的一个j在i的右边，则更新j对应的“最大值”，为Ai + Aj。

如果扫到的i是某个询问的左端点，该询问的右端点为r，则对应的答案是r的前缀最大值，用树状数组可以维护。

TIPS：涉及区间的题，除了区间查询，还可以考虑离线~

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;

int read(){
    char c; bool op = 0;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
void write(int x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 100005;
int n, Q, m, k[12], a[N], b[N], pos[N], ans[N];
int data[N]; //前缀最大值
struct query {
    int l, r, id;
    bool operator < (const query &b) const{
        return l > b.l;
    }
} q[N];
void update(int p, int x){
    while(p <= n) data[p] = max(data[p], x), p += p & -p;
}
int getmax(int p){
    int res = 0;
    while(p) res = max(data[p], res), p -= p & -p;
    return res;
}
int main(){
    n = read(), Q = read(), m = read();
    for(int i = 1; i <= n; i++)
        a[i] = read();
    for(int i = 1; i <= n; i++)
        b[i] = read(), pos[b[i]] = i;
    for(int i = 1; i <= m; i++)
        k[i] = read();
    for(int i = 1; i <= Q; i++)
        q[i].l = read(), q[i].r = read(), q[i].id = i;
    sort(q + 1, q + Q + 1); //将询问按照从大到小排序
    for(int i = n, cur = 1; i; i--){
        for(int j = 1; j <= m; j++){
            if(b[i] - k[j] > 0 && pos[b[i] - k[j]] > i)
                update(pos[b[i] - k[j]], a[i] + a[pos[b[i] - k[j]]]);
            if(b[i] + k[j] <= n && pos[b[i] + k[j]] > i)
                update(pos[b[i] + k[j]], a[i] + a[pos[b[i] + k[j]]]);
        }
        while(cur <= Q && i == q[cur].l){
            ans[q[cur].id] = getmax(q[cur].r);
            cur++;
        }
    }
    for(int i = 1; i <= Q; i++)
        write(ans[i]), putchar('\n');
    return 0;
}
```

## 2017-09-08 [51nod 1500](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1500) 苹果曼和树 | 树形DP

> 
苹果曼有一棵n个点的树。有一些（至少一个）结点被标记为黑色，有一些结点被标记为白色。

现在考虑一个包含k(0 ≤ k < n)条树边的集合。如果苹果曼删除这些边，那么会将这个树分成(k+1)个部分。每个部分还是一棵树。

现在苹果曼想知道有多少种边的集合，可以使得删除之后每一个部分恰好包含一个黑色结点。答案对1000000007 取余即可。

Input

单组测试数据。

第一行有一个整数n (2 ≤ n ≤ 10^5)，表示树中结点的数目。

第二行有n-1个整数p[0],p[1],...,p[n-2] (0 ≤ p[i] ≤ i)。表示p[i]和(i+1)之间有一条边。结点从0开始编号。

第三行给出每个结点的颜色，包含n个整数x[0],x[1],...,x[n-1] (x[i]是0或者1)。如果x[i]是1，那么第i个点就是黑色的，否则是白色的。

Output

输出答案占一行。

|u是否与黑点相连|
|----|
|是|
|是|
|否|
|否|

那么 dp[u][1] （u与黑点相连） 可以从之前的dp[u][1] 和 dp[u][0] 转移，dp[u][0] 只能从之前的 dp[u][0] 转移。

```
dp[u][1] = dp[u][1] * (dp[v][0] + dp[v][1]) + dp[u][0] * dp[v][1];
dp[u][0] = dp[u][0] * (dp[v][0] + dp[v][1]);
```

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    char c; bool op = 0;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005, P = 1000000007;
int n;
ll dp[N][2];
int ecnt, adj[N], nxt[2 * N], go[2 * N];
void add(int u, int v){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
void dfs(int u, int pre){
    int v;
    for(int e = adj[u]; e; e = nxt[e]){
        if((v = go[e]) == pre) continue;
        dfs(v, u);
        dp[u][1] = (dp[u][1] * (dp[v][0] + dp[v][1]) % P + dp[u][0] * dp[v][1] % P) % P;
        dp[u][0] = dp[u][0] * (dp[v][0] + dp[v][1]) % P;
    }
}
int main(){
    n = read();
    for(int i = 1, u; i < n; i++)
        u = read() + 1, add(u, i + 1), add(i + 1, u);
    for(int i = 1; i <= n; i++)
        dp[i][read()] = 1;
    dfs(1, 0);
    printf("%lld\n", dp[1][1]);
    return 0;
}
```

## 2017-09-08 [51nod 1668](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1668) 非010串 | 矩阵乘法

> 
如果一个01字符串满足不存在010这样的子串，那么称它为非010串。

求长度为n的非010串的个数。(对1e9+7取模)

Input

一个数n，表示长度。(n<1e15)

Output

长度为n的非010串的个数。(对1e9+7取模)


类似走k步。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
const int N = 8, P = 1000000007;
struct matrix {
    ll g[N][N];
    matrix(){
        memset(g, 0, sizeof(g));
    }
    matrix operator * (const matrix &b) {
        matrix c;
        for(int k = 0; k < N; k++)
            for(int i = 0; i < N; i++)
                for(int j = 0; j < N; j++)
                    c.g[i][j] = (c.g[i][j] + g[i][k] * b.g[k][j]) % P;
        return c;
    }
    void out(){
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++)
                printf("%lld%c", g[i][j], " \n"[j == N - 1]);
    }
} op, ans;
ll n;
matrix qpow(matrix a, ll x){
    matrix ret;
    for(int i = 0; i < N; i++)
        ret.g[i][i] = 1;
    while(x){
        if(x & 1) ret = ret * a;
        a = a * a;
        x >>= 1;
    }
    return ret;
}
int main(){
    scanf("%lld", &n);
    if(n < 3){
        if(n == 0) printf("0\n");
        else if(n == 1) printf("2\n");
        else if(n == 2) printf("4\n");
        return 0;
    }
    for(int i = 0; i < N; i++){
        op.g[i][(i << 1 | 1) & 7] = 1;
        op.g[i][i << 1 & 7] = 1;
    }
    for(int i = 0; i < N; i++){
        op.g[2][i] = op.g[i][2] = 0;
        if(i != 2) ans.g[i][0] = 1;
    }
    op = qpow(op, n - 3);
    ans = op * ans;
    ll res = 0;
    for(int i = 0; i < N; i++)
        res = (res + ans.g[i][0]) % P;
    printf("%lld\n", res);
    return 0;
}
```

## 2017-09-09 [51nod 1495](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1495) 中国好区间 | 尺取法

所谓尺取法，就是类似一个队列，两个指针移来移去……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
const int N = 10000005;
ll n, a[N], k, t, b, c, p;
int main(){
    scanf("%lld%lld%lld%lld%lld%lld%lld", &n, &k, &t, &a[0], &b, &c, &p);
    for(int i = 1; i <= n; i++)
        a[i] = (a[i - 1] * b + c) % p;
    ll l = 1, r = 0, sum = 0, ans = 0;
    while(l <= n){
        while(sum < k && r < n){
            r++;
            if(a[r] >= t) sum++;
        }
        if(sum < k) break;
        ans += n - r + 1;
        if(a[l] >= t) sum--;
        l++;
    }
    printf("%lld\n", ans);
    return 0;
}
```

@ 不开longlong见那啥

## [51nod 1009](http://www.51nod.com/onlineJudge/submitDetail.html#!judgeId=339783) 数字1的数目 | 数位DP？

考虑23?45这样一个数。

假如 ? 是0，那么这一位是1的数字有 23 * 100 个。（因为"?"前面的数可能有23种，如01100, 01101, 01102...; 02100, 02101...; 22100, 22101....; "?"后面的数可能有100种。）

假如"?"是1，那么这一位是1的数字有 23 * 100 + 45 + 1种。（因为23 * 100的部分同上，另外还有 23101, 23102 ... 23145。)

假如"?"是大于1的数，那么这一位是1的数字有 24 * 100 种。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
ll n, pw[13], ans;
int main(){
    scanf("%lld", &n);
    pw[0] = 1;
    for(int i = 1; i <= 10; i++)
        pw[i] = pw[i - 1] * 10;
    for(int i = 1; i <= 10; i++){
        ll frt = n / pw[i];
        ll bck = n % pw[i - 1];
        ll ths = n / pw[i - 1] % 10;
        if(ths == 0) ans += frt * pw[i - 1];
        else if(ths == 1) ans += frt * pw[i - 1] + bck + 1;
        else ans += (frt + 1) * pw[i - 1];
    }
    printf("%lld\n", ans);
    return 0;
}
```

## [51nod 1043](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1043) 幸运号码 | DP

> 
1个长度为2N的数，如果左边N个数的和 = 右边N个数的和，那么就是一个幸运号码。

例如：99、1230、123312是幸运号码。

给出一个N，求长度为2N的幸运号码的数量。由于数量很大，输出数量 Mod 10^9 + 7的结果即可。


左边的部分开头不能为0，右边的部分开头可以为0，枚举和，乘法原理即可。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int n;
const int M = 9005, P = 1000000007;
ll f[M], h[M], ans;
int main(){
    scanf("%d", &n);
    h[0] = 1;
    for(int i = 1; i < n; i++)
        for(int j = 9 * n; j >= 0; j--)
            for(int k = min(9, j); k; k--)
                h[j] = (h[j] + h[j - k]) % P;
    for(int j = 0; j <= 9 * n; j++)
        for(int k = min(9, j); k; k--)
            f[j] = (f[j] + h[j - k]) % P;
    for(int j = 9 * n; j >= 0; j--)
        for(int k = min(9, j); k; k--)
            h[j] = (h[j] + h[j - k]) % P;
    for(int i = 1; i <= 9 * n; i++)
        ans = (f[i] * h[i] % P + ans) % P;
    printf("%lld\n", ans);
    return 0;
}
```

## [51nod 1693](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1693) 水群 | 最短路 SPFA

> 
众所周知，水群是一件很浪费时间的事，但是其实在水群这件事中，也可以找到一些有意思的东西。

比如现在，bx2k就在研究怎样水表情的问题。

首先，bx2k在对话框中输入了一个表情![](http://img.51nod.com/upload/000FBF8E/08D30490CCFA65530000000000000016.png)，接下来，他可以进行三种操作。

第一种，是全选复制，把所有表情全选然后复制到剪贴板中。

第二种，是粘贴，把剪贴板中的表情粘贴到对话框中。

第三种，是退格，把对话框中的最后一个表情删去。

假设当前对话框中的表情数是num0，剪贴板中的表情数是num1，

那么第一种操作就是num1=num0

第二种操作就是num0+=num1

第三种操作就是num0--

现在bx2k想知道，如果要得到n(1<=n<=10^6)个表情，最少需要几次操作。

请你设计一个程序帮助bx2k水群吧。


要是没有网上的题解，我肯定没有毅力把这道题优化到AC……

优化：
- 连 i -> k * i （复制粘贴，边权为k）时，只连k为质数的边即可。
- 只会用到 2, 3, 5, 7, 11, 13 这几个质数………………
- 不要连边！连边跑得贼慢！

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <queue>
#define INF 0x3f3f3f3f
using namespace std;
const int N = 1000006;
const int prime[8] = {0, 2, 3, 5, 7, 11, 13};
int n, dis[N];
queue <int> q;
bool inq[N];
int main(){
    scanf("%d", &n);
    for(int i = 2; i <= n + 5; i++) dis[i] = INF;
    dis[1] = 0, inq[1] = 1, q.push(1);
    while(!q.empty()){
        int u = q.front(), v;
        inq[u] = 0, q.pop();
        if(dis[u] + 1 < dis[u - 1]){
            dis[u - 1] = dis[u] + 1;
            if(!inq[u - 1]) inq[u - 1] = 1, q.push(u - 1);
        }
        for(int j = 1; j <= 6 && prime[j] * u <= n + 5; j++)
            if(dis[u] + prime[j] < dis[v = u * prime[j]]){
                dis[v] = dis[u] + prime[j];
                if(!inq[v]) inq[v] = 1, q.push(v);
            }
    }
    printf("%d\n", dis[n]);
    return 0;
}
```

@ 边能直接推出来的时候不要物理连接……尤其是卡得特别紧的时候……

## [51nod 1122](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1122) 机器人走方格 V4 | 矩阵乘法 组合数？

> 
四个机器人a b c d，在2 * 2的方格里，一开始四个机器人分别站在4个格子上，每一步机器人可以往临近的一个格子移动或留在原地（同一个格子可以有多个机器人停留），经过n步后有多少种不同的走法，使得每个毯子上都有1机器人停留。由于方法数量巨大，输出 Mod 10^9 + 7的结果。

Input

输入1个数N（0 <= N <= 10^9)

Output

输出走法的数量 Mod 10^9 + 7


枚举最后四个机器人的位置，分别计算即可。

@ 最后加起来的时候没取模……………………

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
const int P = 1000000007;
struct matrix {
    ll g[4][4];
    matrix(){
        memset(g, 0, sizeof(g));
    }
    matrix operator * (const matrix &b) const {
        matrix c;
        for(int k = 0; k < 4; k++)
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++)
                    c.g[i][j] = (c.g[i][j] + g[i][k] * b.g[k][j]) % P;
        return c;
    }
    void out() const{
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                printf("%lld%c", g[i][j], " \n"[j == 3]);
    }
} op;
matrix qpow(matrix a, int x){
    matrix ret;
    for(int i = 0; i < 4; i++)
        ret.g[i][i] = 1;
    while(x){
        if(x & 1) ret = ret * a;
        a = a * a;
        x >>= 1;
    }
    return ret;
}
int n;
int per[4] = {0, 1, 2, 3};
ll tot;
int main(){
    scanf("%d", &n);
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            op.g[i][j] = !(j == 3 - i);
    op = qpow(op, n);
    for(int k = 0; k < 24; k++){
        ll ans = 1;
        for(int i = 0; i < 4; i++)
            ans = ans * op.g[i][per[i]] % P;
        tot = (tot + ans) % P;
        next_permutation(per, per + 4);
    }
    printf("%lld\n", tot);
    return 0;
}
```

## [51nod 1021](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1021) 石子归并 | 水题

@ 数组越界了！！！

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
using namespace std;
int read(){
    char c;
    bool op = 0;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
const int N = 105;
int n, sum[N], dp[N][N];
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        sum[i] = sum[i - 1] + read();
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            dp[i][j] = INF;
    for(int l = 1; l < n; l++)
        for(int i = 1; i <= n - l; i++)
            for(int j = 0; j < l; j++)
                dp[i][l] = min(dp[i][l], dp[i][j] + dp[i + j + 1][l - j - 1] + sum[i + l] - sum[i - 1]);
    printf("%d\n", dp[1][n - 1]);
    return 0;
}
```

## [51nod 1112](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1112) KGold | 题解说堆然而我写的线段树

> 
给出N个人在0时刻的财富值M[i]（所有人在0时刻的财富互不相等），以及财富增长速度S[i]，随着时间的推移，某些人的财富值会超越另外一些人。如果时间足够长，对于财富增长最快的人来说，他的财富将超越所有其他对手。

求发生的前10000次超越，分别是谁超过了谁？如果总的超越次数不足10000，则输出所有超越，如果1次超越都不会发生，则输出No Solution。

输出按照超越发生的时间排序，同一时刻发生的超越，按照超越者的编号排序，如果编号也相同，则按照被超越者的编号排序。所有排序均为递增序。

Input

第1行：N，表示人的数量。(1 <= N <= 10000)

第2 - N + 1行：每行2个数，分别是初始的财富值M[i]，和财富增长速度S[i]。(0 <= M[i] <= 10^5, 1 <= S[i] <= 100)

Output

输出前10000次超越，超越者和被超越者的编号。如果总的超越次数不足10000，则输出所有。如果1次超越都不会发生，则输出No Solution。

输出按照超越发生的时间排序，同一时刻发生的超越，按照超越者的编号排序，如果超越者编号也相同，则按照被超越者的编号排序。所有排序均为递增序。


黑书说这道题是用堆，然鹅我用了线段树2333~

对于每个人(i)，维护比他有钱且与他的财富值差距最小的人(设这个人编号为pre[i]，若pre[i] = j 则设nxt[j] = i)，每次取出超越pre[i]的时间最早的i，让这个人去超越，同时修改与他相关的人(nxt[i], pre[i], pre[pre[i]])使得pre和nxt数组符合超越后的实际情况。

注意，虽然是输出前10000次，但实际要求的要更多，重新排序后取前10000个。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
int read(){
    char c;
    bool op = 0;
    while((c = getchar()) < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while((c = getchar()) >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
struct frac {
    bool inf;
    int up, down;
    frac(): inf(0), up(0), down(0) {}
    bool operator < (const frac &b) const{
        if(inf || b.inf) return b.inf;
        return up * b.down < b.up * down;
    }
    bool operator != (const frac &b) const{
        return up * b.down != b.up * down;
    }
};
frac tofrac(int _up, int _down){
    frac ret;
    if(_up <= 0 || _down <= 0) ret.inf = 1;
    else ret.up = _up, ret.down = _down;
    return ret;
}
struct answer {
    int faster, slower;
    frac time;
    bool operator < (const answer & b) const{
        if(time != b.time) return time < b.time;
        if(faster != b.faster) return faster < b.faster;
        return slower < b.slower;
    }
};
const int N = 20005;
frac data[4 * N];
int n, p[N], v[N], pre[N], nxt[N], lst[N], minp[4 * N];
answer ans[4 * N];
void build(int k, int l, int r){
    if(l == r)
        return (void) (data[k] = tofrac(p[pre[l]] - p[l], v[l] - v[pre[l]]), minp[k] = l);
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    if(data[k << 1 | 1] < data[k << 1]) data[k] = data[k << 1 | 1], minp[k] = minp[k << 1 | 1];
    else data[k] = data[k << 1], minp[k] = minp[k << 1];
}
bool judge(){
    return !data[1].inf;
}
void update(int k, int l, int r, int pos){
    if(l == r) return (void) (data[k] = tofrac(p[pre[l]] - p[l], v[l] - v[pre[l]]));
    int mid = (l + r) >> 1;
    if(pos <= mid) update(k << 1, l, mid, pos);
    else update(k << 1 | 1, mid + 1, r, pos);
    if(data[k << 1 | 1] < data[k << 1]) data[k] = data[k << 1 | 1], minp[k] = minp[k << 1 | 1];
    else data[k] = data[k << 1], minp[k] = minp[k << 1];
}
bool cmp(int a, int b){
    return p[a] < p[b];
}

int main(){

    n = read();
    for(int i = 1; i <= n; i++)
        lst[i] = i, p[i] = read(), v[i] = read();
    sort(lst + 1, lst + n + 1, cmp);
    for(int i = 1; i <= n; i++)
        pre[lst[i]] = lst[i + 1], nxt[lst[i]] = lst[i - 1];
    pre[lst[1]] = lst[1], nxt[lst[n]] = lst[n];
    build(1, 1, n);
    if(!judge()){
        printf("No Solution\n");
        return 0;
    }
    int cnt;
    for(cnt = 0; cnt <= 50000 && judge();){
        cnt++;
        ans[cnt].faster = minp[1];
        ans[cnt].slower = pre[minp[1]];
        ans[cnt].time = data[1];
        int u1 = minp[1], u0 = nxt[u1], u2 = pre[u1], u3 = pre[u2];
        pre[u0] = u2, pre[u2] = u1, pre[u1] = u3;
        nxt[u1] = u2, nxt[u2] = u0, nxt[u3] = u1;
        update(1, 1, n, u0);
        update(1, 1, n, u1);
        update(1, 1, n, u2);
        update(1, 1, n, u3);
    }
    sort(ans + 1, ans + cnt + 1);
    for(int i = 1; i <= min(cnt, 10000); i++){
        printf("%d %d\n", ans[i].faster, ans[i].slower);
    }
    return 0;
}
```

## [24oj 1187](http://192.168.0.233/JudgeOnline/problem.php?id=1187) 酒店 | 题解说线段树然而我写的分块

> 
【题目描述】

A城市有个超级大酒店，住房部只有一层，只向南，但房间有N个（编号1, 2 .. N）。刚开始所有房间都是空的，但酒店肯定会有人来住宿，有些房间就要被入住。但客人经常会问一个问题，最长连续的空房间是多少，因为客人想连续的住在一起。于是，有下列三个操作：

1 a b：表示从房间a开始连续b个房间，将被入住（不管有没有人已经入住）

2 a b：表示从房间a开始连续b个房间，将清空入住(不管有没有人入住)

3：表示询问当前最长连续的空房间是多少？

对于3的询问，输出相应答案。

【输入】

首先输入N和P, N如问题描述，P表示有多少个操作

然后输入P个操作

【输出】

对于第三种操作，输出相应答案

【数据规模和约定】

3<=N<=16,000

3<=P<=200,000


```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
const int N = 20005, S = 150;
int n, q, s, m; //一个块的长度是s, 一共m个块
int le[S], ri[S], ma[S];
bool vis[N];
void pushdown(int x){
    if(ma[x] == 0 || ma[x] == s)
        for(int i = (x - 1) * s + 1; i <= min(n, x * s); i++)
            vis[i] = ma[x] ? 0 : 1;
}
void update(int x){ //更新第x个块
    int st = (x - 1) * s + 1;
    int ed = min(x * s, n);
    le[x] = ri[x] = ma[x] = 0;
    int i = st;
    while(i <= ed && vis[i] == 0) le[x]++, i++;
    i = ed;
    while(i >= st && vis[i] == 0) ri[x]++, i--;
    int last = 0;
    for(i = st; i <= ed; i++)
        if(vis[i] == 0) last++, ma[x] = max(ma[x], last);
        else last = 0;
}
void change(int l, int r, bool val){ //将[l, r]修改为val
    int stx = (l - 1) / s + 1, edx = (r - 1) / s + 1;
    if(stx == edx){
        pushdown(edx);
        for(int i = l; i <= r; i++)
            vis[i] = val;
        update(stx);
        return;
    }
    pushdown(stx);
    for(int i = l; i <= min(n, stx * s); i++)
        vis[i] = val;
    update(stx);
    pushdown(edx);
    for(int i = r; i > (edx - 1) * s; i--)
        vis[i] = val;
    update(edx);
    for(int i = stx + 1; i <= edx - 1; i++)
        ma[i] = le[i] = ri[i] = val ? 0 : s;
}
int query(){
    int last = 0, ret = 0;
    for(int i = 1; i <= m; i++){
        ret = max(ret, last + le[i]);
        ret = max(ret, ma[i]);
        if(ma[i] < s) last = ri[i];
        else last += s;
    }
    return ret;
}
void init(){
    s = sqrt(n);
    m = ceil((double) n / s);
    for(int i = 1; i < m; i++)
        ma[i] = le[i] = ri[i] = s;
    for(int i = n + 1; i <= m * s; i++)
        vis[i] = 1;
    update(m);
}
int main(){
    n = read(), q = read();
    init();
    while(q--){
        int op = read(), l, r;
        if(op == 1){
            l = read(), r = read();
            change(l, l + r - 1, 1);
        }
        else if(op == 2){
            l = read(), r = read();
            change(l, l + r - 1, 0);
        }
        else{
            printf("%d\n", query());
        }
    }
    return 0;
}
```

@ 第一次写分块……还是自己脑补的分块……记得每次修改前都要pushdown！

> 
## 随手记一下

HDU 4918


## [24oi vijos](https://vijos.org/d/dl24oi/p/59bf84ebd3d8a123308bbe7f) 火星探险 | 扫描线法 线段树

[一个关于扫描线法+线段树的详细博客](http://blog.csdn.net/qq_18661257/article/details/47658191)

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;
typedef long long ll;
const int N = 300005, M = 100005;
int n, data[4*N], cover[4*N];
ll ans = 0, wid = 0;;
struct lines {
    int h, l, r, x;
    bool operator < (const lines &b) const{
        return h < b.h;
    }
} line[2*M];
void change(int k, int l, int r, int ql, int qr, int x){
    if(ql <= l && qr >= r){
        cover[k] += x;
        if(cover[k]) data[k] = r - l + 1;
        else data[k] = data[k << 1] + data[k << 1 | 1];
        return;
    }
    int mid = (l + r) >> 1;
    if(ql <= mid) change(k << 1, l, mid, ql, qr, x);
    if(qr > mid) change(k << 1 | 1, mid + 1, r, ql, qr, x);
    if(cover[k]) data[k] = r - l + 1;
    else data[k] = data[k << 1] + data[k << 1 | 1];
}
int main(){
    scanf("%d", &n);
    for(int i = 1, a, b, c, d; i <= n; i++){
        scanf("%d%d%d%d", &a, &b, &c, &d);
        line[2 * i - 1] = (lines) {b, a, c, 1};
        line[2 * i] = (lines) {d, a, c, -1};
    }
    sort(line + 1, line + 2 * n + 1);
    for(int i = 1, last = 0; i <= 2 * n; i++){
        ans += wid * (line[i].h - last);
        change(1, 1, 300000, line[i].l + 1, line[i].r, line[i].x);
        last = line[i].h;
        wid = data[1];
    }
    printf("%lld\n", ans);
    return 0;
}
```

## [CodeVS 3044](http://codevs.cn/problem/3044/) 矩形面积求并 | 扫描线法线段树 离散化

第一次写需要离散化的题……

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#define eps 0.000000001
using namespace std;
const int N = 103;
int n, cover[16 * N], cnt;
struct Line {
    double l, r, h;
    int x;
    bool operator < (const Line &b) const{
        return h < b.h;
    }
} line[2 * N];
double data[16 * N], lst[2 * N];
int find(double x){
    return lower_bound(lst + 1, lst + cnt + 1, x) - lst;
}
void change(int k, int l, int r, int ql, int qr, int x){
    if(ql <= l && qr >= r){
        cover[k] += x;
        if(cover[k] > 0) data[k] = lst[r] - lst[l - 1];
        else data[k] = data[k << 1] + data[k << 1 | 1];
        //printf("data[%d, %d] = %lf\n", l - 1, r, data[k]);
        return;
    }
    int mid = (l + r) >> 1;
    if(ql <= mid) change(k << 1, l, mid, ql, qr, x);
    if(qr > mid) change(k << 1 | 1, mid + 1, r, ql, qr, x);
    if(cover[k] > 0) data[k] = lst[r] - lst[l - 1];
    else data[k] = data[k << 1] + data[k << 1 | 1];
}
int main(){
    while(scanf("%d", &n), n){
        memset(data, 0, sizeof(data));
        memset(cover, 0, sizeof(cover));
        memset(lst, 0, sizeof(lst));
        cnt = 0;
        for(int i = 1; i <= n; i++){
            double a, b, c, d;
            scanf("%lf%lf%lf%lf", &a, &b, &c, &d);
            line[i * 2 - 1] = (Line){a, c, b, 1};
            line[i * 2] = (Line){a, c, d, -1};
            lst[++cnt] = a, lst[++cnt] = c;
        }
        sort(lst + 1, lst + cnt + 1);
        sort(line + 1, line + 2 * n + 1);
        double ans = 0, last = 0;
        for(int i = 1; i <= 2 * n; i++){
            ans += data[1] * (line[i].h - last);
            last = line[i].h;
            change(1, 1, cnt, find(line[i].l) + 1, find(line[i].r), line[i].x);
        }
        printf("%.2lf\n", ans);
    }
    return 0;
}
```

@ 线段树大小不是 4 * "n"，而是 4 * 可能出现的离散化点数……

## [51nod 1533](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1533) 一堆的堆 | 树状数组

谜啊……算一下复杂度，发现经过一番证明，“暴力”能过……

“暴力”是这样的：从小到大把每个数加到树状数组对应位置中；在把一个数放进去之前，先求一下它作为父节点**对每个k**的贡献。

一开始就没转过先枚举节点后枚举k的这个弯来……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    bool op = 0;
    char c;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
void write(int x){
    if(x == 0) return (void) putchar('0');
    if(x < 0) putchar('-'), x = -x;
    int s[10], cnt = 0;
    while(x) s[cnt++] = x % 10, x /= 10;
    while(--cnt >= 0) putchar('0' + s[cnt]);
}
const int N = 200005;
int n, tree[N], ans[N];
struct node {
    int num, id;
    bool operator < (const node &b) const{
        return num < b.num;
    }
} a[N];
void add(int p){
    while(p <= n) tree[p]++, p += p & -p;
}
int ask(int p){
    int ret = 0;
    while(p) ret += tree[p], p -= p & -p;
    return ret;
}
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        a[i].id = i, a[i].num = read();
    sort(a + 1, a + n + 1);
    for(int i = 1; i <= n; ){
        int nxt = i;
        while(nxt <= n && a[nxt].num == a[i].num) nxt++;
        for(int j = i; j < nxt; j++){
            int v = a[j].id;
            for(int k = 1; k < n && k * (v - 1) + 1 < n; k++)
                ans[k] += ask(min(n, k * v + 1)) - ask(k * (v - 1) + 1);
        }
        for(int j = i; j < nxt; j++)
            add(a[j].id);
        i = nxt;
    }
    for(int k = 1; k < n; k++)
        write(ans[k]), putchar(" \n"[k == n - 1]);
    return 0;
}
```

@ 读入优化没写负数………………

## 2017-09-21 [51nod 1717](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1717) 好数

只有平方数不是好数。

@ 忘了两个数相乘溢出 long long 的事……

```
#include <cstdio>
using namespace std;
typedef long long ll;
ll n;
int main(){
    scanf("%lld", &n);
    ll l = 0, r = 1000000000, mid;
    while(l < r){
        mid = (l + r + 1) >> 1;
        if(mid * mid <= n) l = mid;
        else r = mid - 1;
    }
    printf("%lld\n", n - l);
    return 0;
}
```

## 2017-09-21 [51nod 1737](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1737) 配对

> 
给出一棵带边权的树，要求给树上的节点两两配对，使每对点的距离之和最大。


就是求……所有点到重心的距离和？！

@ 似乎爆栈了？！

```
#include <cstdio>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
const int N = 100005;
int n;
int sze[N], G, Gsze = INF, q[N], r, fa[N], son[N];
int ecnt, adj[N], nxt[2 * N], go[2 * N];
ll w[2 * N], dep[N], ans;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
void add(int u, int v, ll ww){
    go[++ecnt] = v;
    w[ecnt] = ww;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
int main(){
    n = read();
    for(int i = 1, u, v, ww; i < n; i++){
        u = read(), v = read(), ww = read();
        add(u, v, (ll) ww);
        add(v, u, (ll) ww);
    }
    q[r = 1] = 1;
    for(int l = 1, u, v; l <= r; l++){
        u = q[l];
        sze[u] = 1;
        for(int e = adj[u]; e; e = nxt[e])
            if(v = go[e], v != fa[u]){
                fa[v] = u;
                q[++r] = v;
            }
    }
    for(int l = r, u; l; l--){
        u = q[l];
        sze[fa[u]] += sze[u];
        son[fa[u]] = max(son[fa[u]], sze[u]);
        son[u] = max(n - sze[u], son[u]);
        if(son[u] < Gsze) G = u, Gsze = son[u];
    }
    q[r = 1] = G;
    memset(fa, 0, sizeof(fa));
    for(int l = 1, u, v; l <= r; l++){
        u = q[l];
        for(int e = adj[u]; e; e = nxt[e])
            if(v = go[e], v != fa[u]){
                dep[v] = dep[u] + w[e];
                fa[v] = u;
                q[++r] = v;
            }
        ans += dep[u];
    }
    printf("%lld\n", ans);
    return 0;
}
```

## [51nod 1821](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1821) 最优集合 | 贪心 单调栈

> 
一个集合S的优美值定义为：最大的x，满足对于任意i∈[1,x]，都存在一个S的子集S'，使得S'中元素之和为i。

给定n个集合，对于每一次询问，指定一个集合S1和一个集合S2，以及一个数k，要求选择一个S2的子集S3(|S3|<=k)，使得S1∪S3的优美值最大。

（集合元素可以重复）

Input

第一行一个数n，（n<=1000）

接下来n行，每行描述一个集合：

第一个数m，表示集合大小，接下来m个数，表示集合中的元素（m<=1000，元素<=10^9）

第n+2行一个数T，表示询问次数（T<=10000）

接下来T行，每行3个数a，b，k，表示指定第a个集合为S1，第b个集合为S2，k的意义如题（a<=n,b<=n,k<=100,000）

Output

T行，每行一个数，表示对应询问所能达到的最大优美值


假如我们已经知道一个集合的优美值是ans，现在要加入一个元素x，那么新的优美值是多少呢？如果 x <= ans + 1 则是 ans + x，否则这个x没有用处，仍然是ans。

那么我们把元素入栈，在加元素的时候，首先考虑集合a中的元素，再考虑集合b中的元素；每次选择 <= ans + 1 的最大元素。

@ 多了好多终止循环的条件………………

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <iostream>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
ll read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 1003;
ll n, T, cnt[N], a[N][N], stka[N], stkb[N], topa, topb;
int main(){
    n = read();
    for(int i = 1; i <= n; i++){
        cnt[i] = read();
        for(int j = 1; j <= cnt[i]; j++)
            a[i][j] = read();
        sort(a[i] + 1, a[i] + cnt[i] + 1);
    }
    T = read();
    while(T--){
        topa = topb = 0;
        ll A = read(), B = read(), k = read(), ans = 0, pa = 1, pb = 1, x = 1;
        while(1){
            while(pa <= cnt[A] && a[A][pa] <= ans + 1) stka[++topa] = a[A][pa++];
            while(pb <= cnt[B] && a[B][pb] <= ans + 1) stkb[++topb] = a[B][pb++];
            if(topa) ans += stka[topa--];
            else if(topb && x <= k) ans += stkb[topb--], x++;
            else break;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
```

## [51nod 1804](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1804) 小C的多边形 | 打表找规律 输出优化

这时限卡得……

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;
const int N = 1000005;
int n, ans[N];
void write(int x){
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
int main(){
    scanf("%d", &n);
    if(n & 1){
        printf("0\n");
        return 0;
    }
    for(int i = 1; i <= n / 2; i++)
        ans[n - i * 2 + 1] = i;
    for(int i = 1; i < n / 2; i++)
        ans[n - i * 2] = i + n / 2;
    for(int i = 1; i < n; i++)
        write(i), putchar(" \n"[i == n - 1]);
    for(int i = 1; i < n; i++)
        write(ans[i]), putchar(" \n"[i == n - 1]);
    return 0;
}
```

## 2017-09-22 [51nod 1791](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1791) 合法括号子段 | DP 栈

秒？

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
const int N = 1100005;
int T, stk[N], top, n;
ll dp[N], ans;
char s[N];
int main(){
    scanf("%d", &T);
    while(T--){
        ans = top = 0;
        scanf("%s", s + 1);
        n = strlen(s + 1);
        for(int i = 1; i <= n; i++){
            dp[i] = 0;
            if(s[i] == '(')
                stk[++top] = i;
            else if(top){
                int cp = stk[top--];
                dp[i] = dp[cp - 1] + 1;
                ans += dp[i];
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}
```

@ 题目只限定了所有数据的大小之和的时候，不要每组数据都memset！

## 2017-09-22 [51nod 1732](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1732) 51nod婚姻介绍所 | DP

秒……

（哪天还是得学一下后缀数组）

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
void write(int x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 1003;
int n, dp[N][N], q;
char s[N];
int main(){
    n = read();
    scanf("%s", s + 1);
    for(int i = n; i; i--)
        for(int j = n; j; j--)
            if(s[i] == s[j])
                dp[i][j] = dp[i + 1][j + 1] + 1;
    q = read();
    while(q--){
        int a = read() + 1, b = read() + 1;
        write(dp[a][b]), putchar('\n');
    }
    return 0;
}
```

## 2017-09-22 [51nod 1718](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1718) Cos的多项式 | 数学？

使2cos(x) = 1即可，那么x = π/3。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int main(){
    ll n, ans;
    scanf("%lld", &n);
    n = abs(3 - n % 6);
    if(n == 0) ans = -2;
    else if(n == 1) ans = -1;
    else if(n == 2) ans = 1;
    else ans = 2;
    printf("%lld\n", ans);
    return 0;
}
```

## 2017-09-22 [51nod 1714](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1714) B君的游戏 | 博弈论 NIM游戏

简单记一下nim游戏：

nim游戏：有n堆石子，两个人轮流从任意一堆石子中拿走任意个石子，最后无石子可拿的人输。

这个游戏的结论是：只有在所有石子异或和为0的情况下先手必输，其余情况下先手必胜。

证明：

需要证明以下三个命题：
- 这个结论把所有无子可拿的情况都判定为先手必输。
- 这个结论判定为先手必胜的情况，先手必然有一种方案将当前局面变为一个异或和为0的新局面。
- 这个结论判定为先手必输的情况，先手的每一种方案（如果有方案）达到的新局面异或和都为0。

证明过程：
- 无子可拿就是石子数都为0，所以异或和也是0。
- 设这个局面的异或和为x，则x的最高位是1（显然不是0……不然x就是0了……），可以找到一个（石子数中）这一位也是1的石子堆（不然这一位1是哪来的……）。那么对这一堆的石子数进行修改：最高位改成0，其余位也针对x的对应位进行修改，如果x的对应位是0则不动，是1则取反，显然得到的新石子数比原来的小，这样修改后，总异或和变为0.
- 修改后的局面异或和必然与之前不同，原先异或和为0，所以新局面也一定为0。

显然，对于多个同时进行的nim游戏（每一轮玩家可以选择其中任意一个进行操作），每个游戏的异或和的异或和，就是总游戏局面的异或和，性质不变。

许多博弈游戏都可以转化为nim游戏，只是原版nim中的“石子数”要换成别的什么。

这类问题的共同点是都可以转化为在有向无环图上移动n个棋子的形式（先无法移动的人输）。原版中的石子数，在新版中，变成了“节点的SG函数”，SG(u) = u能到达的节点的SG中第一个没有出现过的数。

这样表示后，仍然可以用异或和求解。

例如现在这道题：

> 
双方轮流操作。每次操作，选一个正整数x，将其移除，再添加7个数字 \(x_1,x_2...x_7\) 。要求对于 \(x_i\) ，满足 \(0 \le x_i<x\) 且 $x & x_i = x_i $
\(（x_i \le 2^{64})\)


一个数就相当于一堆石子，石子数是二进制数中“1”的个数。但是由于每个数可以变成七个数，那么每个数可以转化出的新局面就是每种七个数的方案中，七个数的SG的异或和。

在本题中SG(x)中x的范围只有64，所以打表即可。打表的时候也要注意优化好……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef unsigned long long ll;
ll read(){
    char c;
    while(c = getchar(), c < '0' || c > '9');
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
ll sg[70] = {0,1,2,4,8,16,32,64,128,255,256,512,1024,2048,3855,4096,8192,13107,16384,21845,27306,32768,38506,65536,71576,92115,101470,131072,138406,172589,240014,262144,272069,380556,524288,536169,679601,847140,1048576,1072054,1258879,1397519,2005450,2097152,2121415,2496892,2738813,3993667,4194304,4241896,4617503,5821704,7559873,8388608,8439273,8861366,11119275,11973252,13280789,16777216,16844349,17102035,19984054,21979742};
ll calc(ll x){
    ll ret = 0;
    while(x) ret += x & 1, x >>= 1;
    return ret;
}
ll n, ans;
int main(){
    n = read();
    while(n--) ans ^= sg[calc(read())];
    if(ans) printf("B\n");
    else printf("L\n");
    return 0;
}
```

## [51nod 1686](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1686) 第K大的区间 | 二分 尺取法

> 
定义一个区间的值为其众数出现的次数。

现给出n个数，求将所有区间的值排序后，第K大的值为多少。

Input

第一行两个数n和k(1<=n<=100000,k<=n*(n-1)/2)

第二行n个数，0<=每个数<2^31


偷偷用\(O(n \log n \log n)\)卡过……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    char c;
    while(c = getchar(), c < '0' || c > '9');
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
const int N = 100005;
int n, k, cnt, lst[N], a[N], b[N];
int data[4 * N];
void change(int k, int l, int r, int p, int x){
    if(l == r) return (void) (data[k] += x);
    int mid = (l + r) >> 1;
    if(p <= mid) change(k << 1, l, mid, p, x);
    else change(k << 1 | 1, mid + 1, r, p, x);
    data[k] = max(data[k << 1], data[k << 1 | 1]);
}
int find_pos(int x){
    return lower_bound(lst + 1, lst + cnt + 1, x) - lst;
}
bool chk(int mid){ //猜测第k大的区间众数数是mid
    memset(data, 0, sizeof(data));
    //接下来计数区间众数数大于等于mid的有多少，如果总数大于等于k则mid可能猜小了，反之mid一定猜大了
    int r = 0, sum = 0;
    for(int l = 1; l <= n; l++){
        while(r < n && data[1] < mid)
            change(1, 1, cnt, find_pos(a[++r]), 1);
        if(r == n && data[1] < mid) break;
        sum += n - r + 1;
        if(sum >= k) return 1;
        change(1, 1, cnt, find_pos(a[l]), -1);
    }
    return 0;
}
int main(){
    n = read(), k = read();
    for(int i = 1; i <= n; i++)
        b[i] = a[i] = read();
    sort(b + 1, b + n + 1);
    for(int last = -1, i = 1; i <= n; i++)
        if(b[i] != last)
            last = b[i], lst[++cnt] = b[i];
    int l = 1, r = n, mid;
    while(l < r){
        mid = (l + r + 1) >> 1;
        if(chk(mid)) l = mid;
        else r = mid - 1;
    }
    printf("%d\n", l);
    return 0;
}
```

## [51nod 1682](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1682) 中位数计数 | 前缀和的奇妙应用？

> 
中位数定义为所有值从小到大排序后排在正中间的那个数，如果值有偶数个，通常取最中间的两个数值的平均数作为中位数。

现在有n个数，每个数都是独一无二的，求出每个数在多少个包含其的区间中是中位数。

Input

第一行一个数n(n<=8000)

第二行n个数，0<=每个数<=10^9


对于每个数，设比它小的为-1，比它大的为1，它本身为0，那么一个中位数为它的区间区间和应当是0。

那么求一下前缀和，再求有多少对位置满足一个在左、一个在右，并且二者的前缀和相等即可。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
const int N = 8005;
int n, a[N], d[N], cnt[2 * N];
int main(){
    scanf("%d", &n);
    for(int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for(int i = 1; i <= n; i++){
        int ans = 0;
        for(int j = 1; j <= n; j++)
            d[j] = a[j] > a[i] ? 1 : -1;
        d[i] = 0;
        cnt[n] = 1;
        for(int j = 1; j <= n; j++){
            d[j] += d[j - 1];
            if(j < i) cnt[d[j] + n]++;
            else ans += cnt[d[j] + n];
        }
        for(int j = 1; j <= n; j++)
            cnt[d[j] + n] = 0;
        printf("%d%c", ans, " \n"[i == n]);
    }
    return 0;
}
```

## [51nod 1677](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1677) TreeCnt | 组合数 树形DP 不要用DFS

> 
给定一棵n个节点的树，从1到n标号。选择k个点，你需要选择一些边使得这k个点通过选择的边联通，目标是使得选择的边数最少。

现需要计算对于所有选择k个点的情况最小选择边数的总和为多少。


计算每一条边的贡献：一条边有贡献当且仅当选择的k个点在它的两侧都有，即所有方案数减去k个点同在一侧的方案数，即 C(n, k) - C(sze[i], k) - C(n - sze[i], k)。

@ DFS爆栈可真是……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
ll read(){
    char c;
    while(c = getchar(), c < '0' || c > '9');
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
const int N = 200005, P = 1000000007;
ll n, k, fac[N] = {1}, inv[N], ans;
int ecnt, adj[N], go[2 * N], nxt[2 * N], sze[N];
int q[N], r, fa[N];
ll qpow(ll a, ll x){
    ll ret = 1;
    while(x){
        if(x & 1) ret = ret * a % P;
        a = a * a % P;
        x >>= 1;
    }
    return ret;
}
void add(int u, int v){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
ll C(int n, int m){
    if(n < m) return 0;
    return fac[n] * inv[m] % P * inv[n - m] % P;
}
int main(){
    n = read(), k = read();
    for(int i = 1; i <= n; i++)
        fac[i] = fac[i - 1] * i % P;
    inv[n] = qpow(fac[n], P - 2);
    for(int i = n; i; i--)
        inv[i - 1] = inv[i] * i % P;
    for(int i = 1, u, v; i < n; i++)
        u = read(), v = read(), add(u, v), add(v, u);
    q[r = 1] = 1, fa[1] = 1;
    for(int l = 1, u, v; l <= r; l++){
        u = q[l];
        sze[u] = 1;
        for(int e = adj[u]; e; e = nxt[e])
            if(!fa[v = go[e]])
                fa[v] = u, q[++r] = v;
    }
    for(int l = r, u; l; l--)
        if(u = q[l], u != 1)
            sze[fa[u]] += sze[u];
    ans = n * C(n, k);
    for(int i = 1; i <= n; i++)
        ans = (ans - C(sze[i], k) - C(n - sze[i], k)) % P;
    printf("%lld\n", (ans + P) % P);
    return 0;
}
```

## [51nod 1799](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1799) 二分答案 | 组合数

我们模拟他的二分，可以求出二分时都“用到”了哪些点作为缩小搜索范围的依据。

那么这些点中，在k右边的点上的数必须比m小，另一些必须比m大，这样才能让搜索的点最终停留在k这个位置。

那么用组合数计算即可。

问题是这个阶乘比较大，打表又打不下……怎么办呢？用【隔点打表法】即可。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;

int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}

const int P = 1000000007;
const ll fac[] = {1,682498929,491101308,76479948,723816384,67347853,27368307,625544428,199888908,888050723,927880474,281863274,661224977,623534362,970055531,261384175,195888993,66404266,547665832,109838563,933245637,724691727,368925948,268838846,136026497,112390913,135498044,217544623,419363534,500780548,668123525,128487469,30977140,522049725,309058615,386027524,189239124,148528617,940567523,917084264,429277690,996164327,358655417,568392357,780072518,462639908,275105629,909210595,99199382,703397904,733333339,97830135,608823837,256141983,141827977,696628828,637939935,811575797,848924691,131772368,724464507,272814771,326159309,456152084,903466878,92255682,769795511,373745190,606241871,825871994,957939114,435887178,852304035,663307737,375297772,217598709,624148346,671734977,624500515,748510389,203191898,423951674,629786193,672850561,814362881,823845496,116667533,256473217,627655552,245795606,586445753,172114298,193781724,778983779,83868974,315103615,965785236,492741665,377329025,847549272,698611116};

int n, m, k, cntl, cntr;

ll getfac(int x){
    ll ret = fac[x / 10000000];
    for(int i = x / 10000000 * 10000000 + 1; i <= x; i++)
        ret = ret * i % P;
    return ret;
}
ll qpow(ll a, ll x){
    ll ret = 1;
    while(x){
        if(x & 1) ret = ret * a % P;
        a = a * a % P;
        x >>= 1;
    }
    return ret;
}
ll A(int m, int n){
    if(n < m) return 0;
    return getfac(n) * qpow(getfac(n - m), P - 2) % P;
}

int main(){
    scanf("%d%d%d", &n, &m, &k);
    int l = 1, r = n, mid = (l + r) / 2;
    while(l <= r){
        if(mid <= k) l = mid + 1, cntl++;
        else r = mid - 1, cntr++;
        mid = (l + r) / 2;
    }
    ll ans = A(cntl, m) % P * A(cntr, n - m) % P * getfac(n - cntl - cntr) % P;
    printf("%lld\n", ans);
    return 0;
}
```

## 2017-09-25 [51nod 1674](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1674) 区间的价值V2 | 位运算的性质 ST表 预处理log

我的方法复杂度不是非常优啊……但是预处理了log之后能卡过。

预处理log：

```
for(int i = 1, j = 0; i <= 100000; j++)
    while(i <= 100000 && (1 << (j + 1)) > i) 
        lg[i] = j, i++;
```

and和or运算和min、max一样，有【可以写ST表】的性质。

而对于同一左端点来说，与不同的右端点组成的区间的and和or值都不是很多，每种运算最多只有\(\log 10^9\)种。

那么我们枚举左端点，二分出下一个答案与上次不同的右端点。

然后居然卡过了……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
ll read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005, P = 1000000007;
ll n, ans, a[N], And[N][23], Or[N][23];
int lg[N];
void init(){
    for(int i = 1, j = 0; i <= 100000; j++)
        while(i <= 100000 && (1 << (j + 1)) > i) lg[i] = j, i++;
    for(int i = 1; i <= n; i++)
        And[i][0] = Or[i][0] = a[i];
    for(int j = 1; (1 << j) <= n; j++)
        for(int i = 1; i + (1 << j) - 1 <= n; i++){
            And[i][j] = And[i][j - 1] & And[i + (1 << (j - 1))][j - 1];
            Or[i][j] = Or[i][j - 1] | Or[i + (1 << (j - 1))][j - 1];
        }
}
ll getAnd(int l, int r){
    int j = lg[r - l + 1];
    return And[l][j] & And[r - (1 << j) + 1][j];
}
int getOr(int l, int r){
    int j = lg[r - l + 1];
    return Or[l][j] | Or[r - (1 << j) + 1][j];
}
int findR(int L, int l, ll lastAnd, ll lastOr){ // find the last [L, R] that is same as lastAnd && lastOr
    int r = n, mid;
    while(l < r){
        mid = (l + r + 1) >> 1;
        if(lastAnd == getAnd(L, mid) && lastOr == getOr(L, mid)) l = mid;
        else r = mid - 1;
    }
    return l;
}
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        a[i] = read();
    init();
    for(int i = 1; i <= n; i++){
        int R, lastR = i - 1;
        ll lastAnd = a[i], lastOr = a[i];
        while(lastR < n){
            R = findR(i, lastR + 1, lastAnd, lastOr);
            lastAnd = getAnd(i, R), lastOr = getOr(i, R);
            ans = (ans + (R - lastR) * lastAnd % P * lastOr) % P;
            lastR = R;
        }
    }
    printf("%lld\n", ans);
    return 0;
}
```

@ ST表的查找部分，第二个是[r - (1 << j) + 1][j]。

## [51nod 1672](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1672) 区间交 | 贪心 线段树

> 
小A有一个含有n个非负整数的数列与m个区间，每个区间可以表示为li,ri。

它想选择其中k个区间， 使得这些区间的交的那些位置所对应的数的和最大。（是指k个区间共同的交，即每个区间都包含这一段，具体可以参照样例）

在样例中，5个位置对应的值分别为1,2,3,4,6，那么选择[2,5]与[4,5]两个区间的区间交为[4,5]，它的值的和为10。

Input

第一行三个数n,k,m(1<=n<=100000,1<=k<=m<=100000)。

接下来一行n个数ai，表示小A的数列(0<=ai<=10^9)。

接下来m行，每行两个数li,ri，表示每个区间(1<=li<=ri<=n)。


因为都是正数，所以固定一个左端点，右端点越靠右越好。

把区间按左端点排序，每次把当前左端点看做所选区间中最靠右的，那么要所选区间中最靠右的右端点尽量靠右，那么在线段树中找到第k大的右端点即可。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
typedef long long ll;
ll read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005;
int n, k, m;
ll sum[N], ans;
struct range{
    int l, r;
}rge[N];
bool cmp(const range &a, const range &b){
    return a.l < b.l;
}
int data[4 * N];
void change(int k, int l, int r, int p, int x){
    if(l == r) return (void) (data[k] += x);
    int mid = (l + r) >> 1;
    if(p <= mid) change(k << 1, l, mid, p, x);
    else change(k << 1 | 1, mid + 1, r, p, x);
    data[k] = data[k << 1] + data[k << 1 | 1];
}
int query(int k, int l, int r, int x){
    if(l == r) return l;
    int mid = (l + r) >> 1;
    if(data[k << 1] >= x) return query(k << 1, l, mid, x);
    else return query(k << 1 | 1, mid + 1, r, x - data[k << 1]);
}
int main(){
    n = read(), k = read(), m = read();
    for(int i = 1; i <= n; i++)
        sum[i] = sum[i - 1] + read();
    for(int i = 1; i <= m; i++)
        rge[i].l = read(), rge[i].r = read();
    sort(rge + 1, rge + m + 1, cmp);
    for(int i = 1; i <= m;){
        int rem = i;
        while(rge[i].l == rge[rem].l) change(1, 1, n, rge[i++].r, 1);
        //找第k大的r
        if(i > k) ans = max(ans, sum[query(1, 1, n, i - k)] - sum[rge[rem].l - 1]);
    }
    printf("%lld\n", ans);
    return 0;
}
```

## [51nod 1670](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1670) 打怪兽 | 期望 组合数 DP

> 
学姐在玩一款叫做“绿帽大作战”的游戏。游戏中，她会遇到n个人，每个人有一个“忠贞值”a[i]，学姐有一个“绿色值”x。如果学姐遇到的人忠贞值小于等于学姐的绿色值，那么这个人（的伴侣）会被学姐绿，同时学姐的绿色值会增加1；如果学姐遇到的人的忠贞值大于学姐的绿色值，那么游戏结束。

现在给出n个人的忠贞值，这n个人被学姐遇到的顺序随机排列。学姐想知道她在游戏结束时的绿色值期望的是多少。由于学姐不喜欢小数，请输出这个答案乘以n!的值。


只需知道：遇到第i个怪时，学姐有多少种可能会存活。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;

ll read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005, P = 1000000007;
ll n, hp[N], dp[N] = {1}, weak, ans;
ll qpow(ll a, ll x){
    ll ret = 1;
    while(x){
        if(x & 1) ret = ret * a % P;
        a = a * a % P;
        x >>= 1;
    }
    return ret;
}
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        hp[i] = read(), dp[0] = dp[0] * i % P;
    sort(hp + 1, hp + n + 1);
    for(int i = 1, j = 1; i <= n; i++){ //已经打了i - 1个小怪兽
        while(j <= n && hp[j] < i) j++, weak++; //如果小怪兽的血量小于等于当前lyk血量(i - 1)
        if(weak < i) break; //如果小怪兽之前已经打完了
        dp[i] = dp[i - 1] * (weak - i + 1) % P * qpow(n - i + 1, P - 2) % P;
        //存活的概率：(weak - (i - 1)) / (n - (i - 1))
        ans = (ans + dp[i]) % P;
    }
    printf("%lld\n", ans);
    return 0;
}
```

@ "ans = ans + dp % P" ...

## [51nod 1624](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1624) 取余最长路 | set 取模DP？

> 
佳佳有一个n*m的带权矩阵，她想从(1,1)出发走到(n,m)且只能往右往下移动，她能得到的娱乐值为所经过的位置的权的总和。有一天，她被下了恶毒的诅咒，这个诅咒的作用是将她的娱乐值变为对p取模后的值，这让佳佳十分的不开心，因为她无法找到一条能使她得到最大娱乐值的路径了！她发现这个问题实在是太困难了，既然这样，那就只在3*n的矩阵内进行游戏吧！

现在的问题是，在一个3*n的带权矩阵中，从(1,1)走到(3,n)，只能往右往下移动，问在模p意义下的移动过程中的权总和最大是多少。

Input

单组测试数据

第一行两个数n(1<=n<=100000),p(1<=p<=1000000000)。

接下来3行，每行n个数，第i行第j列表示a[i][j]表示该点的权(0 < =a[i][j] < p)。

Output

一个整数表示答案。


为什么数据只有三行？

这是让我们……枚举两个“拐点”啊！

设第一行的拐点横坐标为x，第二行的拐点横坐标为y：

```
x    y
####........
...######...
........####
```

那么所获得的权值就是：

\(sum1[x] + sum2[y] - sum2[x - 1] + sum3[n] - sum3[y - 1]\)
\(= (sum1[x] - sum2[x - 1]) + (sum3[n] - sum3[y - 1] + sum2[y])\)

第二个式子共分为两部分，一部分只与x有关，一部分只与y有关。我们倒序枚举i，把它的\((sum3[n] - sum3[i - 1] + sum2[i])\)加入一个set中，再在set中取出与它的\(sum1[i] - sum2[i - 1]\)加起来模p答案最大的值，更新答案。

什么样的两个数加起来模p最大呢？注意到这两个数都是 < p 的，那么加起来的和要么是最后一个 <= p - 1 的，要么就是最后一个 <= 2 * p - 1 的。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <set>
using namespace std;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005;
int n, p, sum[3][N], ans;
set <int> st;
int main(){
    n = read(), p = read();
    for(int i = 0; i < 3; i++)
        for(int j = 1; j <= n; j++)
            sum[i][j] = (sum[i][j - 1] + read()) % p;
    for(int i = n; i; i--){
        st.insert(((sum[2][n] - sum[2][i - 1] + sum[1][i]) % p + p) % p);
        int left = sum[0][i] - sum[1][i - 1];
        set <int> :: iterator it1 = st.lower_bound(p - left), it2 = st.lower_bound(2 * p - left);
        if(it1 != st.begin()){
            it1--;
            ans = max(ans, (*it1 + left) % p);
        }
        if(it2 != st.begin()){
            it2--;
            ans = max(ans, (*it2 + left) % p);
        }
    }
    printf("%d\n", ans);
    return 0;
}
```

## [51nod 1603](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1603) 限高二叉排列树 | DP

> 
求n个节点、高度不小于h的二叉搜索树的种数。


```
#include <cstdio>
using namespace std;
typedef unsigned long long ll;
const int N = 80;
ll n, h, dp[N][N]; //dp[i][j]: i个节点，高度不小于j的种数
int main(){
    scanf("%llu%llu", &n, &h);
    dp[0][0] = 1;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= i; j++)
            for(int k = 0; k < i; k++)
                dp[i][j] += dp[k][j - 1] * (2 * dp[i - k - 1][0] - dp[i - k - 1][j - 1]);
        dp[i][0] = dp[i][1]; //dp[i][0]表示i个节点的所有二叉树的种类数。一开始这里没搞清楚。
    }
    printf("%llu\n", dp[n][h]);
    return 0;
}
```

## [51nod 1557](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1557) 两个集合 | 思维题？二分

> 
小X有n个互不相同的整数： p1,p2,...,pn 。他想把这些整数分到两个集合A和B里边。但是要符合下面两个条件。
- 如果x属于A，那么a-x也肯定属于A。
- 如果x属于B，那么b-x也肯定属于B。

判断一下是否存在一种方案来分配这些数字到集合A，B中。

注意：如果一个集合为空也是可以的。


对于一个存在的数x，存在以下三种情况：
- 不存在 a - x，也不存在 b - x。这时x在任意一个集合都是不合法的，直接输出NO。
- a - x 和 b - x 中只存在一个。如果存在 a - x，把 x 和 a - x 都放在A集合；否则放在B集合。
- a - x 和 b - x 都存在，则如果把x放在A集合，a - x一定放在A集合，b - x 也不能放在B集合（因为这样x也需要在B集合），那么b - x也要在A集合，那么一定要存在 a - (b - x)。如果把x放在B集合，一定要存在 b - (a - x)。如果这两个都不存在，就输出NO。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;

int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
const int N = 100005;
int n, p[N], a, b;
int main(){
    n = read(), a = read(), b = read();
    for(int i = 1; i <= n; i++)
        p[i] = read();
    sort(p + 1, p + n + 1);
    for(int i = 1; i <= n; i++){
        int* pair_a = lower_bound(p + 1, p + n + 1, a - p[i]);
        int* pair_b = lower_bound(p + 1, p + n + 1, b - p[i]);
        if(*pair_a == a - p[i] && *pair_b == b - p[i]){
            int* pair_pair_a = lower_bound(p + 1, p + n + 1, b - a + p[i]);
            int* pair_pair_b = lower_bound(p + 1, p + n + 1, a - b + p[i]);
            if(*pair_pair_a != b - a + p[i] && *pair_pair_b != a - b + p[i]){
                printf("NO\n");
                return 0;
            }
        }
        else if(*pair_a != a - p[i] && *pair_b != b - p[i]){
            printf("NO\n");
            return 0;
        }
    }
    printf("YES\n");
    return 0;
}
```

## [51nod 1524](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1524) 可除图的最大团 | DP 最大团

> 
对于一般的图，最大团问题是一个NP-难的问题。然而，对于一些特殊的图，最大团问题可以有比较有效的解决方案。

关于最大团问题的概念，请百度之。^_^

在一个正整数集合A上定义可除图。 A = {a1, a2, ..., an} ，图上的顶点就是集合A中的数字。两个数字 ai 和 aj (i ≠ j) 之间有一条边的条件是 ai 能够被 aj 整除，或者 aj 能够被 ai 整除.

现在给定一个正整数集A，请找出这个集合所确定的可除图的最大团。

样例解释：在这个例子中，最大团是3，可以选择 {3,6,18}。

Input

单组测试数据。

第一行有一个整数n (1≤n≤10^6)，表示A的大小。

第二行有n个不一样的整数 a1,a2,...,an (1≤ai≤10^6)，表示A中的元素。

Output

输出一个整数，表示最大团中的点数。


如何（用极其可怕的复杂度）求一个图的最大团？

首先搞个最大团，枚举不在团上的点，如果与团上每个点都有边相连则可以选择加不加入这个点（递归处理），否则不能加入这个点。

现在对于这个特殊的图，我们发现——如果一个团中最小值是某个点的倍数，那么那个点一定与团中所有点有边。就可以考虑加不加入了。事实上我们可以DP。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 1000000;
int n, dp[N + 5], ans, lst[N + 5], cnt;
bool exist[N + 5], np[N + 5];
void euler(){
    np[0] = np[1] = 1;
    for(int i = 2; i <= N; i++){
        if(!np[i]) lst[++cnt] = i;
        for(int j = 1; j <= cnt && i * lst[j] <= N; j++){
            np[i * lst[j]] = 1;
            if(i % lst[j] == 0) break;
        }
    }
}
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        exist[read()] = 1;
    euler();
    for(int i = N; i; i--){
        dp[i] = exist[i];
        for(int j = 1; lst[j] * i <= N && lst[j] * i > 1; j++)
            dp[i] = max(dp[i], dp[i * lst[j]] + exist[i]);
        ans = max(ans, dp[i]);
    }
    printf("%d\n", ans);
    return 0;
}
```

## [51nod 1524](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1487) 占领资源 | 思维题？

> 
有一个矩形区域被划分为N行M列的网格，每个格子里有一定数量的资源并记录在矩阵val中，坐标(x,y)位置上资源量为val[x][y]，其val中每个元素的值为0~9的整数。如果你在某个网格(a,b)上造一座保护塔，那么你可以占领K个网格中的资源，这K个格子分别是(a+dx[1],b+dy[1]),(a+dx[2],b+dy[2]),...,(a+dx[K],b+dy[K])，注意（a，b）这格本身可能未必会被占领。现在你能建造不超过2个塔，问最多能占领多少资源？一个网格被多个塔占领时其资源只计算一次。另外如果计算的位置(a+dx[i],b+dy[i])在网格外，则不贡献任何资源。

Input

多组测试数据，第一行一个整数T，表示测试数据数量，1<=T<=5

每组测试数据有相同的结构构成：

每组数据第一行三个整数N，M，K,其中2<=N，M<=100，1<=K<=10。

之后会有N行，每行M个元素，表示val矩阵。每个元素为0~9，占一个字符，元素间没空格。

再接下来有K行，每行两个整数dx[i]与dy[i],其中-(N-1)<=dx[i]<=N-1,-(M-1)<=dy[i]<=(M-1).

Output

每组数据一行输出，即可占领的最大资源总量。


每个点最多只会与 K^2 个点有重叠，把这些点单独处理，剩下的（不冲突的）取最大值即可。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 202, K = 22;
int T, n, m, k, ans, val[N][N], gain[N][N], dx[K], dy[K], vis[N * N], gained[N][N];
struct Data{
    int pos, num;
    bool operator < (const Data &b) const{
        return num > b.num;
    }
} data[N * N];
void init(){
    memset(val, 0, sizeof(val));
    memset(gain, 0, sizeof(gain));
    memset(vis, 0, sizeof(vis));
    memset(gained, 0, sizeof(gained));
    memset(dx, 0, sizeof(dx));
    memset(dy, 0, sizeof(dy));
    memset(data, 0, sizeof(data));
    ans = 0;
    n = read(), m = read(), k = read();
    for(int i = 1; i <= n; i++){
        char c;
        for(int j = 1; j <= m; j++){
            while(c = getchar(), c < '0' || c > '9');
            val[i][j] = c - '0';
        }
    }
    for(int i = 1; i <= k; i++)
        dx[i] = read(), dy[i] = read();
}
bool islegal(int x, int y){
    return x > 0 && y > 0 && x <= n && y <= m;
}
int hsh(int x, int y){
    return (x - 1) * m + y;
}
int main(){
    T = read();
    while(T--){
        init();
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= m; j++){
                for(int q = 1; q <= k; q++)
                    if(islegal(i + dx[q], j + dy[q]))
                        gain[i][j] += val[i + dx[q]][j + dy[q]]; //gain是(i, j)本身能收获的答案
                data[hsh(i, j)] = (Data){hsh(i, j), gain[i][j]};
            }
        sort(data + 1, data + n * m + 1);
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= m; j++){
                vis[hsh(i, j)] = hsh(i, j);
                for(int q = 1; q <= k; q++)
                    if(islegal(i + dx[q], j + dy[q]))
                        gained[i + dx[q]][j + dy[q]] = hsh(i, j); //将(i, j)能收获的点标记一下
                for(int q = 1; q <= k; q++){ //(i, j)要到达的点是(i + dx[q], j + dy[q])
                    for(int e = 1; e <= k; e++){ //另外一个点(another_i, another_j)从方向e也到达这个点
                        int another_i = i + dx[q] - dx[e], another_j = j + dy[q] - dy[e];
                        if(islegal(another_i, another_j) && vis[hsh(another_i, another_j)] != hsh(i, j)){
                            //如果这个点没有被访问过
                            vis[hsh(another_i, another_j)] = hsh(i, j); //那么标记为访问过
                            int another_gain = gain[i][j];//another_gain是(i, j)与(another_i, another_j)构成的答案
                            for(int h = 1; h <= k; h++) //开始枚举(another_i, another_j)能收获的点
                                if(islegal(another_i + dx[h], another_j + dy[h]) && gained[another_i + dx[h]][another_j + dy[h]] != hsh(i, j))
                                    another_gain += val[another_i + dx[h]][another_j + dy[h]];
                            ans = max(ans, another_gain);
                        }
                    }
                }
                int maxpos = 1;
                while(maxpos <= n * m && vis[data[maxpos].pos] == hsh(i, j)) maxpos++;
                ans = max(ans, data[maxpos].num + gain[i][j]);
            }
        printf("%d\n", ans);
    }
    return 0;
}
```

@ 还是把所有数组都初始化一下吧……

## [51nod 1490](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1490) 多重游戏 | 博弈论 树形DP

> 
有一个两人游戏，游戏是这样的，有n个非空串。在游戏的过程是，两个玩家轮流向一个字符串后面加字母，刚开始字符串是空的。每一次操作是向当前字符串后面添加字符，形成的新字符串一定要是这n个串中某一个或几个的前缀，如果无法做到，就输了。

这样的游戏似乎过于简单了，现在对这个游戏进行一下改进，让玩家玩K次这样的游戏，第i次的败者，将会作为第i+1次的先手进行这个游戏。第k次游戏的赢家就是整个游戏的赢家。

现在给定n个字符串和k，问是先手胜还是后手胜。

Input

单组测试数据。

第一行有两个整数n 和 k (1 ≤ n ≤ 10^5; 1 ≤ k ≤ 10^9)

接下来n行每一行有一个非空串。字符串的总长度不超过10^5，都由小字母构成。

Output

如果先手胜输出First，否则输出Second。


这题挺有趣的……

事实证明，我还是没有游戏中“绝顶聪明”的Alice和Bob们聪明……我naive地想：只要求出是否先手必胜就好了……然鹅实际上由于有好几局，先手可以故意输掉前面的局以保证最后一句自己能获胜。

可以对节点维护四种状态：
- 先手必输，后手必胜
- 先手必胜，后手必输
- 先手想赢就能赢、想输就能输，后手无法决定，完全取决于先手
- 后手想赢就能赢、想输就能输，先手无法决定，完全取决于后手

然后……抄了抄别人的代码，发现：别人可以用位运算轻松处理四种关系……

首先给每种状态编号、研究达到这种状态的条件：
|编号|
|----|
|0|
|1|
|2|
|3|

对于节点u，枚举每个子节点v，sg[u] |= sg[v] ^ 3 即可。

神奇的位运算……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005;
int n, k, cnt = 1, nxt[N][26], sg[N];
char s[N];
void insert(){
    int now = 1, l = strlen(s);
    for(int i = 0; i < l; i++){
        if(!nxt[now][s[i] - 'a']) nxt[now][s[i] - 'a'] = ++cnt;
        now = nxt[now][s[i] - 'a'];
    }
}
void dfs(int u){
    bool vis = 0;
    for(int i = 0; i < 26; i++)
        if(nxt[u][i]){
            vis = 1;
            dfs(nxt[u][i]);
            sg[u] |= sg[nxt[u][i]] ^ 3;
        }
    if(!vis) sg[u] = 1;
}
int main(){
    n = read(), k = read() & 1;
    for(int i = 1; i <= n; i++)
        scanf("%s", s), insert();
    dfs(1);
    if(sg[1] == 0 || sg[1] == 1) puts("Second");
    else if(sg[1] == 3) puts("First");
    else puts(k ? "First" : "Second");
    return 0;
}
```

## [51nod 1503](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1503) 猪和回文 | 回文串 DP

> 
一只猪走进了一个森林。很凑巧的是，这个森林的形状是长方形的，有n行，m列组成。我们把这个长方形的行从上到下标记为1到n，列从左到右标记为1到m。处于第r行第c列的格子用(r,c)表示。

刚开始的时候猪站在(1,1),他的目标是走到(n,m)。由于猪回家心切，他在(r,c)的时候，只会往(r+1,c)或(r,c+1)走。他不能走出这个森林。

这只猪所在的森林是一个非同寻常的森林。有一些格子看起来非常相似，而有一些相差非常巨大。猪在行走的过程中喜欢拍下他经过的每一个格子的照片。一条路径被认为是漂亮的当且仅当拍下来的照片序列顺着看和反着看是一样的。也就是说，猪经过的路径要构成一个回文。

数一数从(1,1)到(n,m)有多少条漂亮路径。答案可能非常巨大，请输出对 109+7 取余后的结果。

样例解释：有三种可能

Input

单组测试数据。

第一行有两个整数 n,m (1≤n,m≤500)，表示森林的长和宽。

接下来有n行，每行有m个小写字母，表示每一个格子的类型。同一种类型用同一个字母表示，不同的类型用不同的字母表示。


题面是讽刺不会做这道题的我是猪么……

类似传纸条，dp[i][j][k][h]表示从起点出发到(i, j)、从终点出发到(k, h)路过的字符串相同的方案数。已知i, j, k可以算出h，所以第四维是不必要的，只有dp[i][j][k]，时间复杂度\(O(n^3)\)，空间上可以压缩掉一维。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
typedef long long ll;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 503, P = 1000000007;
int n, m, dp[2][N][N], ans;
char mp[N][N], c;
int geth(int i, int j, int k){
    return n + m - i - j + 2 - k;
}
int main(){
    n = read(), m = read();
    for(int i = 1; i <= n; i++)
        scanf("%s", mp[i] + 1);
    if(mp[1][1] == mp[n][m]) dp[1][1][n] = 1;
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++)
            for(int k = i; k <= n; k++){
                if(!(i == 1 && j == 1 && k == n)) dp[i & 1][j][k] = 0;
                int h = geth(i, j, k);
                if(h < j) continue;
                if(mp[i][j] == mp[k][h]){
                    dp[i & 1][j][k] = (dp[i & 1][j][k] + dp[(i & 1) ^ 1][j][k]) % P;
                    dp[i & 1][j][k] = (dp[i & 1][j][k] + dp[(i & 1) ^ 1][j][k + 1]) % P;
                    dp[i & 1][j][k] = (dp[i & 1][j][k] + dp[i & 1][j - 1][k]) % P;
                    dp[i & 1][j][k] = (dp[i & 1][j][k] + dp[i & 1][j - 1][k + 1]) % P;
                }
                if(i + j - 1 == (n + m) / 2) ans = (ans + dp[i & 1][j][k]) % P;
            }
    printf("%d\n", ans);
    return 0;
}
```

@ 滚动数组记得想好初始化

## [51nod 1478](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1478) 括号序列的最长合法子段 | DP 栈

类似 51nod 1791。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
const int N = 1000005;
int n, dp[N], stk[N], top, maxn, cnt;
char s[N];
int main(){
    scanf("%s", s + 1);
    n = strlen(s + 1);
    for(int i = 1; i <= n; i++){
        if(s[i] == '(') stk[++top] = i;
        else if(top){
            dp[i] = dp[stk[top] - 1] + i - stk[top] + 1;
            top--;
            maxn = max(maxn, dp[i]);
        }
    }
    for(int i = 1; i <= n; i++)
        if(dp[i] == maxn) cnt++;
    if(maxn == 0) printf("0 1\n");
    else printf("%d %d\n", maxn, cnt);
    return 0;
}
```

## [51nod 1464](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1464) 半回文 | Trie树 DP

> 
一个字符串t是半回文的条件是，对于所有的奇数i(1≤i≤|t|+12)，ti = t|t| − i + 1 始终成立，|t|表示字符串t的长度。下标从1开始。例如"abaa", "a", "bb", "abbbaa"都是半回文，而"ab", "bba"和"aaabaa"则不是。

现在有一个字符串s，只由小写字母a,b构成，还有一个数字k。现在要求找出s的半回文子串中字典序排在第k位的串，字符串可以是一样，只要所在的位置不同就是不一样的串。

样例解释：

这个样例中半回文子串是 a, a, a, a, aa, aba, abaa, abba, abbabaa, b, b, b, b, baab,bab, bb, bbab, bbabaab (按照字典序排序).

Input

单组测试数据。

第一行有一个字符串s(1 ≤ |s| ≤ 5000)，只包含'a' 和 'b'，|s|表示s的长度。

第二行有一个正整数k。k不超过s子串中是半回文串的总数目。

Output

输出排在第k位的半回文子串。


用DP预处理出N^2个子串是否是半回文串，然后把以每个字母开头的字符串插入Trie树（优化：如果后面再没有半回文串了，就停止插入）。

空间卡得好紧啊……

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
const int M = 5005, N = 8000000;
int n, m, vis[M], cnt[N], sze[N], nxt[N][2], fa[N], tot;
char s[M];
bool dp[M][M];
void dp_init(){
    for(int i = n; i; i--){
        dp[i][i] = 1;
        for(int j = i + 1; j <= n; j++){
            if(s[i] == s[j]) {
                if(i + 2 >= j - 2) dp[i][j] = 1;
                else dp[i][j] = dp[i + 2][j - 2];
            }
            if(dp[i][j]) vis[i] = j;
        }
    }
}
void _insert(int k){
    int now = 0;
    for(int i = k; i <= n; i++){
        if(!nxt[now][s[i] - 'a'])
            nxt[now][s[i] - 'a'] = ++tot, fa[tot] = now;
        now = nxt[now][s[i] - 'a'];
        if(dp[k][i]) cnt[now]++;
        if(i == vis[k]) break;
    }
}
void bfs(){
    for(int i = tot; i; i--)
        sze[i] += cnt[i], sze[fa[i]] += sze[i];
    sze[0] = 0;
}
void _find(){
    int now = 0;
    while(sze[nxt[now][0]] || sze[nxt[now][1]]){
        if(m <= cnt[now]) break;
        m -= cnt[now];
        if(m <= sze[nxt[now][0]]) putchar('a'), now = nxt[now][0];
        else putchar('b'), m -= sze[nxt[now][0]], now = nxt[now][1];
    }
}
int main(){
    scanf("%s%d", s + 1, &m);
    n = strlen(s + 1);
    dp_init();
    for(int i = 1; i <= n; i++)
        _insert(i);
    bfs();
    _find();
    putchar('\n');
    return 0;
}
```

@ 没有限定字符串长度之和的时候……指针Trie树还是有用的……

## [51nod 1460](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1460) 连接小岛 | set 贪心

> 
有n个小岛，每一个小岛是直线型的，他们不相互相交，第i个小岛所占的区间是[li, ri],而且, ri < li+1 对于所有的 1 ≤ i ≤ n-1。现在要将相邻的小岛用桥连接起来。现在有一条桥的长度是a，第i个岛和第i+1个岛能够连接的条件是，存在x,y使得 li ≤ x ≤ ri, li+1 ≤ y ≤ ri+1 且 y - x = a成立。

现在有m条桥，每条桥最多被使用一次，问能否把这些岛连接起来。

样例解释：在这个样例中，把第2条桥两个端点放在3和8，把第三条桥两个端点放在7和10，把第一条桥的端点放在10和14。

Input

单组测试数据。

第一行有两个整数n (2 ≤ n ≤ 2*10^5) 和 m (1 ≤ m ≤ 2*10^5)，表示岛的数目和桥的数目。

接下来n行，每行有两个整数 li 和 ri (1 ≤ li ≤ ri ≤ 10^18)，表示岛的两个端点。

接下来一行有m个整数 a1, a2, ..., am (1 ≤ ai ≤ 10^18)，表示每一条桥的长度。

Output

如果能够将n座岛连接起来输出YES，否则输出NO。


只有相邻小岛可以相连，对于每对岛，可使用的桥的长度有一个确定的范围，可以想象成一个区间，将区间按照右端点从小到大排序，每次找小于右端点的桥中，左端点的lower_bound即可。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <set>
using namespace std;
typedef long long ll;
ll read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
const int N = 200005;
int n, m;
ll bridge[N];
multiset <ll> st;
struct range {
    ll l, r;
    bool operator < (const range &b) const{
        return r < b.r;
    }
} rge[N];
int main(){
    n = read(), m = read();
    ll lastl = read(), lastr = read(), thisl, thisr;
    for(int i = 1; i < n; i++){
        thisl = read(), thisr = read();
        rge[i].l = thisl - lastr, rge[i].r = thisr - lastl;
        lastl = thisl, lastr = thisr;
    }
    sort(rge + 1, rge + n);
    for(int i = 1; i <= m; i++)
        bridge[i] = read();
    sort(bridge + 1, bridge + m + 1);
    int cur = 1;
    for(int i = 1; i < n; i++){
        while(cur <= m && bridge[cur] <= rge[i].r)
            st.insert(bridge[cur++]);
        multiset <ll> :: iterator it = st.lower_bound(rge[i].l);
        if(it == st.end()){
            printf("NO\n");
            return 0;
        }
        st.erase(it);
    }
    printf("YES\n");
    return 0;
}
```

@ 把m打成n……

## [51nod 1422](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1422) 沙拉酱前缀 | 离线 二分

说好的“数据结构”呢？！

> 
沙拉酱非常喜欢数字序列。这正是他要弄一个关于构造序列的算法的原因。

沙拉酱拿了一张白纸。然后他开始用m个步骤来制作一个序列。每一步他要么向这个序列的末尾添加一个数字，要么拿这个序列的开头l个数字，然后在末尾添加c次。对于第二种操作，一般的，如果当前序列是 a1,a2,...,an ,那么经过操作之后序列将变成 a1,a2,...,an[,a1,a2,...,al] （方括号里面的内容会重复c次）。

一天过去了，沙拉酱也完成了他的序列。现在他想知道某个位置是什么数字。

Input

单组测试数据。

第一行包含一个整数m (1 ≤ m ≤ 10^5)，表示构造序列的步骤数目。

接下来m行包含每一个步骤的信息。第一个数字是类型(1或2)。类型1表示在序列后面加一个数字，这种情况下后面会跟一个整数xi (1 ≤ xi ≤ 10^5),表示被加在后面的数字。类型2表示复制一段长度为 li 前缀然后接到后面 ci 次，这种情况下后面会跟两个整数 li, ci(1 ≤ li ≤ 10^5, 1 ≤ ci ≤ 10^4)，li 是前缀的长度，ci是复制的次数。输入中保证li不会大于当前序列的长度。

接下来一行包含一个整数n (1 ≤ n ≤ 10^5)，表示查询的数量。接下来一行中包含n个正整数，每一个整数表示要查询的位置。题目保证这些数字大小不会超过序列的长度。序列的下标从1开始。

Output

对于每一个查询，输出对应查询位置的数字。两个查询之间用空格分开。具体格式看样例。


先把操作读入进来，可以知道每次操作之后序列的长度是多少。对于询问，二分它被创建是在哪一次操作，如果这个操作是1，则直接输出；如果是2，我们知道它与这次操作前就存在的哪个位置数字相同，递归查找。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
ll read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    ll ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return op ? -ret : ret;
}
const int N = 100005;
int m, n, op[N], a[N], b[N];
ll len[N];
int main(){
    m = read();
    for(int i = 1; i <= m; i++){
        op[i] = read(), a[i] = read();
        if(op[i] == 1) len[i] = len[i - 1] + 1;
        else{
            b[i] = read();
            len[i] = len[i - 1] + (ll) b[i] * a[i];
        }
    }
    n = read();
    for(int i = 1; i <= n; i++){
        ll x = read();
        while(1){
            int p = lower_bound(len + 1, len + m + 1, x) - len;
            if(op[p] == 1) {
                printf("%d%c", a[p], " \n"[i == n]);
                break;
            }
            else
                x = (x - len[p - 1] - 1) % a[p] + 1;
        }
    }
    return 0;
}
```

@ 不开longlong见祖宗！

## [51nod 1243](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1243) 排船的问题 | 二分答案

玩sans模拟器有令人清醒的奇效。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
const int N = 50005;
ll n, x, m, p[N];
bool check(int mid){
    int lastp = 0;
    for(int i = 1; i <= n; i++){
        if(p[i] + mid - x < lastp) return 0;
        lastp = max(lastp + 2 * x, p[i] - mid + x);
    }
    return lastp <= m;
}
int main(){
    scanf("%lld%lld%lld", &n, &x, &m);
    for(int i = 1; i <= n; i++)
        scanf("%lld", &p[i]);
    int l = 0, r = m, mid;
    while(l < r){
        mid = (l + r) >> 1;
        if(check(mid)) r = mid;
        else l = mid + 1;
    }
    printf("%d\n", l == m ? -1 : l);
    return 0;
}
```

## [51nod 1335](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1335) 子序列翻转

x一定是原串和排序后的串的第一处不同。暴力枚举y。

```
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
const int N = 2505;
int T, n;
char origin[N], sorted[N], changed[N], best[N];
int main(){
    scanf("%d", &T);
    while(T--){
        int x = 0, ans = 0;
        scanf("%s", origin);
        n = strlen(origin);
        for(int i = 0; i <= n; i++)
            sorted[i] = best[i] = origin[i];
        sort(sorted, sorted + n);
        for(x = 0; x < n; x++)
            if(origin[x] != sorted[x])
                break;
        if(x == n){
            printf("0 0\n");
            continue;
        }
        for(int i = 0; i < x; i++)
            changed[i] = origin[i];
        for(int y = x; y < n; y++){
            int len = y - x;
            for(int i = 0; i <= len; i++)
                changed[x + i] = origin[y - i];
            for(int i = y + 1; i < n; i++)
                changed[i] = origin[i];
            bool better = 0;
            for(int i = x; i < n; i++)
                if(best[i] != changed[i]){
                    better = changed[i] < best[i];
                    break;
                }
            if(better) {
                ans = y;
                for(int i = x; i < n; i++)
                    best[i] = changed[i];
            }
        }
        printf("%d %d\n", x, ans);
    }
    return 0;
}
```

## [51nod 1202](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1202) 子序列个数 | DP

> 
给出一个序列，求不同的子序列的个数。


dp[i]表示[1, i]中可以找到的不同子序列个数。

如果a[i]之前没有出现过，dp[i] = 2 * dp[i - 1] + 1（选择a[i]+不选a[i]+只有a[i]本身）

如果a[i]之前出现过，则之前加进去过一些以a[i]结尾的序列，这次应当减去。另外，“只有a[i]本身”这个选项也是不合法的。那么dp[i] = 2 * dp[i - 1] - dp[a[i]上一次出现的位置 - 1]。

```
#include <cstdio>
using namespace std;
int read(){
    char c;
    while(c = getchar(), c < '0' || c > '9');
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
const int N = 100005, P = 1000000007;
int n, a[N], pos[N], dp[N];
int main(){
    n = read();
    for(int i = 1; i <= n; i++)
        a[i] = read();
    for(int i = 1; i <= n; i++){
        if(pos[a[i]]) dp[i] = ((2 * dp[i - 1] % P - dp[pos[a[i]] - 1]) % P + P) % P;
        else dp[i] = (2 * dp[i - 1] + 1) % P;
        pos[a[i]] = i;
    }
    printf("%d\n", dp[n]);
    return 0;
}
```

## [51nod](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1287) 加农炮 | 线段树

> 
一个长度为M的正整数数组A，表示从左向右的地形高度。测试一种加农炮，炮弹平行于地面从左向右飞行，高度为H，如果某处地形的高度大于等于炮弹飞行的高度H（A[i] >= H），炮弹会被挡住并落在i - 1处，则A[i - 1] + 1。如果H <= A[0]，则这个炮弹无效，如果H > 所有的A[i]，这个炮弹也无效。现在给定N个整数的数组B代表炮弹高度，计算出最后地形的样子。

例如：地形高度A = {1, 2, 0, 4, 3, 2, 1, 5, 7}, 炮弹高度B = {2, 8, 0, 7, 6, 5, 3, 4, 5, 6, 5}，最终得到的地形高度为：{2, 2, 2, 4, 3, 3, 5, 6, 7}。

Input

第1行：2个数M, N中间用空格分隔，分别为数组A和B的长度(1 <= m, n <= 50000)

第2至M + 1行：每行1个数，表示对应的地形高度(0 <= A[i] <= 1000000)。

第M + 2至N + M + 1行，每行1个数，表示炮弹的高度(0 <= B[i] <= 1000000)。

Output

输出共M行，每行一个数，对应最终的地形高度。


类似约瑟夫问题的线段树——要求第x个剩下的数在哪的做法一样，

现在要求第一个比x大的数在哪，只需判断如果左子树最大值比x大就到左侧，否则到右侧即可。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
using namespace std;
typedef unsigned long long ull;
int read(){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    int ret = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        ret = ret * 10 + c - '0';
    return ret;
}
void write(int x){
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 50005;
int n, m, a[N], data[4 * N], pos[N];
void build(int k, int l, int r){
    if(l == r) return (void)(data[k] = a[l], pos[l] = k);
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    data[k] = max(data[k << 1], data[k << 1 | 1]);
}
void change(int k, int l, int r, int p, int x){
    if(l == r) return (void)(data[k] += x);
    int mid = (l + r) >> 1;
    if(p <= mid) change(k << 1, l, mid, p, x);
    else change(k << 1 | 1, mid + 1, r, p, x);
    data[k] = max(data[k << 1], data[k << 1 | 1]);
}
int query(int k, int l, int r, int x){
    if(l == r) return l;
    int mid = (l + r) >> 1;
    if(data[k << 1] >= x) return query(k << 1, l, mid, x);
    else return query(k << 1 | 1, mid + 1, r, x);
}
int main(){
    n = read(), m = read();
    for(int i = 1; i <= n; i++) a[i] = read();
    build(1, 1, n);
    for(int i = 1, p, b; i <= m; i++) {
        b = read();
        if(data[1] < b) continue;
        p = query(1, 1, n, b);
        if(p > 1) change(1, 1, n, p - 1, 1);
    }
    for(int i = 1; i <= n; i++)
        write(data[pos[i]]), putchar('\n');
    return 0;
}
```

## [51nod 1366](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1366) 贫富差距 | Floyd最短路 差分约束？

> 
n个人(n <= 50)之间有些是朋友，朋友间的贫富差距不超过d(d <= 1000)，问两个人间贫富差距最大是多少？


朋友关系相当于一条权值为d的双向边，求任意两个人间的最短路的最大值。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
using namespace std;
const int N = 53;
char s[N];
int T, n, d, dis[N][N], ans;;
int main(){
    scanf("%d", &T);
    while(T--){
        scanf("%d%d", &n, &d);
        ans = 0;
        for(int i = 1; i <= n; i++){
            scanf("%s", s + 1);
            for(int j = 1; j <= n; j++)
                dis[i][j] = s[j] == 'Y' ? d : INF;
            dis[i][i] = 0;
        }
        for(int k = 1; k <= n; k++)
            for(int i = 1; i <= n; i++)
                for(int j = 1; j <= n; j++)
                    dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                ans = max(dis[i][j], ans);
        printf("%d\n", ans < INF ? ans : -1);    
    }
    return 0;
}
```

## [51nod 1484](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1484) 猜数游戏 | 离散化 线段树

> 
现在有一款小游戏叫做“猜数游戏”。这个游戏的目的是从一个迷宫中找到一个出口。这个迷宫的形状是一棵高度为h的完全二叉树。玩家刚开始站在根部，出口是在某一个叶子结点上面。

现在我们来定义每一个结点的编号:

· 根部是1

· 某个内部结点的编号为 i (i ≤ 2h−1−1) 时，他的左儿子编号为2i，右儿子编号为2i+1。

根的深度定为1，其它结点的深度是他的父亲的深度+1。深度为h的结点是叶子。出口在某个叶子结点上，但是游戏玩家并不知道在哪个叶子，所以他现在要开始猜。

玩家每次会问ancestor(exit, i)属于[L,R]吗？这儿 ancestor(v, i) 表示结点v在第i层的那个祖先。然后游戏会给出"Yes"或"No"的回答。这个游戏的回答也不一定是完全合法的。有一些时候他会骗玩家。

现在有一些询问和回答，要求你从中判断一下，这个游戏有没有在说谎。如果游戏没有说谎，并且出口被推断出来，那么请输出出口，如果游戏没有说谎，但是出口不能被唯一判定出来，请输出“Data not sufficient!”。否则输出“Game cheated!”

结点u是结点v的祖先，当且仅当满足以下条件之一：

· u和v一样，

· u 是 v的父亲，

· u 是v的父亲的祖先。

样例解释：在这个例子中有 8个叶子结点。经过第一个询问之后，出口被确定在 [10, 14]区间内。经过第二个和第三个询问，只有14号符合条件。请结合图例进行理解。
![](http://img.51nod.com/upload/000FBEF4/08D2913C8F4356490000000000000010.png)

Input

单组测试数据。

第一行有两个整数h, q (1 ≤ h ≤ 50, 1 ≤ q ≤ 10^5)，表示树的高度和询问的数量。

接下来q行，每行包含 i, L, R, ans (1 ≤ i ≤ h, 2^(i - 1) ≤ L ≤ R ≤ 2^i - 1, ans∈{0,1}),表示询问出口在第i层的祖先属于L，R吗？ans=1表示YES，否则表示NO。

Output

如果游戏给出的信息是自相矛盾的，那么输出 Game cheated!。

如果可以唯一确定出口。那么输出出口的编号。

否则输出Data not sufficient!。

Input示例

4 3

4 10 14 1

3 6 6 0

2 3 3 1

Output示例

14


别的大佬用的都是……STLset和map……

好像只有我手敲了一个离散化线段树……

终于会写离散化线段树了！

注意，这里离散化一个点代表的是一个区间[lst[i], lst[i + 1])。

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <set>
#define space putchar(' ')
#define enter putchar('\n')
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 200005;
struct Query{
    ll l, r;
    bool op;
} q[N];
int n, Q;
bool data[4*N];
ll lst[N], cnt;
set <ll> st;
void change(int k, int l, int r, int ql, int qr){
    if(data[k] || (ql <= l && qr >= r)) return (void) (data[k] = 1);
    int mid = (l + r) >> 1;
    if(ql <= mid) change(k << 1, l, mid, ql, qr);
    if(qr > mid) change(k << 1 | 1, mid + 1, r, ql, qr);
}
bool query(int k, int l, int r, int p){
    if(data[k] || l == r) return data[k];
    int mid = (l + r) >> 1;
    if(p <= mid) return query(k << 1, l, mid, p);
    else return query(k << 1 | 1, mid + 1, r, p);
}
int main(){
    read(n), read(Q);
    for(int i = 1; i <= Q; i++){
        ll l, r, h;
        bool op;
        read(h), read(l), read(r), read(op);
        q[i].l = l * ((ll)1 << (n - h));
        q[i].r = (r + 1) * ((ll)1 << (n - h));
        q[i].op = op;
        st.insert(q[i].l), st.insert(q[i].r);
    }//每次修改的区间都是[l, r)，离散化后每个坐标i代表的都是[lst[i], lst[i + 1])。
    st.insert((ll)1 << (n - 1)), st.insert((ll)1 << n); //把最后一行的最左侧和最右侧的右侧加入
    for(set <ll> :: iterator it = st.begin(); it != st.end(); it++)
        lst[++cnt] = *it;
    for(int i = 1; i <= Q; i++){
        int l = lower_bound(lst + 1, lst + cnt + 1, q[i].l) - lst;
        int r = lower_bound(lst + 1, lst + cnt + 1, q[i].r) - lst;
        if(q[i].op){
            if(l > 1) change(1, 1, cnt, 1, l - 1);
            if(r < cnt) change(1, 1, cnt, r, cnt - 1);
        }
        else
            change(1, 1, cnt, l, r - 1);
    }
    ll tot = 0, ans = 0;
    for(int i = 1; i < cnt; i++)
        if(!query(1, 1, cnt, i))
            tot += lst[i + 1] - lst[i], ans = lst[i];
    if(tot == 0) puts("Game cheated!");
    else if(tot > 1) puts("Data not sufficient!");
    else write(ans), enter;
    return 0;
}
```

## [51nod 1109](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1109) 01组成的N的倍数 | BFS

真的只需要BFS……

记录每个BFS值得上一个数字、模n的值、最后一位数是0还是1。

输出的时候不断回溯上一个数字就好了。

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <queue>
#define space putchar(' ')
#define enter putchar('\n')
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 1000005;
int n, cnt = 1;
bool vis[N], val[N];
int pre[N], mo[N];
queue <int> q;
void out(int u){
    if(pre[u]) out(pre[u]);
    putchar('0' + val[u]);
}
void bfs(){
    q.push(1);
    mo[1] = val[1] = vis[1] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        if(!vis[mo[u] * 10 % n]){
            mo[++cnt] = mo[u] * 10 % n;
            vis[mo[cnt]] = 1;
            val[cnt] = 0;
            pre[cnt] = u;
            if(mo[cnt] == 0){
                out(cnt), enter;
                break;
            }
            q.push(cnt);
        }
        if(!vis[(mo[u] * 10 + 1) % n]){
            mo[++cnt] = (mo[u] * 10 + 1) % n;
            vis[mo[cnt]] = 1;
            val[cnt] = 1;
            pre[cnt] = u;
            if(mo[cnt] == 0){
                out(cnt), enter;
                break;
            }
            q.push(cnt);
        }
    }
}
int main(){
    read(n);
    if(n == 1) puts("1");
    else bfs();
    return 0;
}
```

## [51nod 1453](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1453) 抽彩球 | 组合数

> 
一个袋子中有n个彩球，他们用k种不同的颜色染色。颜色被从1到k编号。同一种颜色的球看成是一样的。现在从袋中一个一个的拿出球来，直到拿完所有的球。对于所有颜色为i (1<=i<=k-1)的球，他的最后一个球总是在编号比他大的球拿完之前拿完，问这样情况有多少种。

样例解释：这个样例中有2个1号颜色的球，2个2号颜色的球，1个3号颜色的球。三种方案是：

1 2 1 2 3

1 1 2 2 3

2 1 1 2 3

Input

单组测试数据。

第一行给出一个整数k(1 ≤ k ≤ 1000)，表示球的种类。

接下来k行，每行一个整数ci,表示第i种颜色的球有ci个(1 ≤ ci ≤ 1000)。

球的总数目不超过1000。

Output

输出总数对1,000,000,007的模即可。

Input示例

3

2

2

1

Output示例

3


最后一位的顺序是一定的，就是：1, 2, 3, ...

那么每次只需把剩下的往前面插就好了。

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <queue>
#define space putchar(' ')
#define enter putchar('\n')
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 1005, P = 1000000007;
ll c[N][N], n, a[N], ans = 1;
int main(){
    c[0][0] = 1;
    for(int i = 1; i <= 1000; i++)
        for(int j = 0; j <= 1000; j++)
            c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % P;
    read(n);
    for(int i = 1; i <= n; i++)
        read(a[i]);
    for(int tot = 0, i = 1; i <= n; i++){
        ans = ans * c[tot + a[i] - 1][a[i] - 1] % P;
        tot += a[i];
    }
    write(ans), enter;
    return 0;
}
```

## [51nod 1420](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1420) 数袋鼠好有趣 | 二分答案

> 
有一堆数，一个数可以与一个大于等于它的两倍的数相互配对，每个数仅能与一个数配对。


飞翔的戴维琼斯说……二分答案……

……令我“茅厕顿开”？

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#define space putchar(' ')
#define enter putchar('\n')
#define INF 0x3f3f3f3f
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 500005;
int n, lst[N];
bool check(int mid){
    int p = mid + 1;
    for(int i = 1; i <= mid; i++, p++){
        while(p <= n && lst[p] < 2 * lst[i]) p++;
        if(p > n) return 0;
    }
    return 1;
}
int main(){
    read(n);
    if(n == 1) {
        write(1), enter;
        return 0;
    }
    for(int i = 1; i <= n; i++)
        read(lst[i]);
    sort(lst + 1, lst + n + 1);
    int l = 1, r = n / 2, mid;
    while(l < r){
        mid = (l + r + 1) >> 1;
        if(check(mid)) l = mid;
        else r = mid - 1;
    }
    write(n - l), enter;
    return 0;
}
```

## [51nod 1084](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1084) 矩阵取数游戏（类似传纸条） | DP

蜜汁RE？

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 250;
ll m, n, a[N][N], dp[N][N][N];
int main(){
    read(m), read(n);
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++)
            read(a[i][j]);
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++)
            for(int k = 1; k <= n; k++){
                int h = i + j - k;
                dp[i][j][k] = max(dp[i][j][k], dp[i][j - 1][k]);
                dp[i][j][k] = max(dp[i][j][k], dp[i][j - 1][k - 1]);
                dp[i][j][k] = max(dp[i][j][k], dp[i - 1][j][k]);
                dp[i][j][k] = max(dp[i][j][k], dp[i - 1][j][k - 1]);
                if(i == k) dp[i][j][k] += a[i][j];
                else dp[i][j][k] += a[i][j] + a[k][h];
            }
    write(dp[n][m][n]), enter;
    return 0;
}
```

## [51nod 1424](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1424) 零树 | 树形DP

> 
有一棵以1为根的树，他有n个结点，用1到n编号。第i号点有一个值vi。

现在可以对树进行如下操作：

步骤1：在树中选一个连通块，这个连通块必须包含1这个结点。

步骤2：然后对这个连通块中所有结点的值加1或者减1。

问最少要经过几次操作才能把树中所有结点都变成0。

注意：步骤1与步骤2合在一起为一次操作。

Input

单组测试数据。

第一行有一个整数n(1 ≤ n ≤ 10^5)

接下来n-1行，每行给出 ai 和 bi (1 ≤ ai, bi ≤ n; ai ≠ bi)，表示ai和bi之间有一条边，输入保证是一棵树。

最后一行有n个以空格分开的整数，表示n个结点的值v1, v2, ..., vn (|vi| ≤ 10^9)。

Output

输出一个整数表示最少的操作步数。.

Input示例

3

1 2

1 3

1 -1 1

Output示例

3


f[u]表示以u为根的子树至少需要执行几次减1操作，g[u]表示以u为根的子树至少需要执行几次加1操作。

f[u] = max{f[v]}, g[u] = max{g[v]}。

如果执行完f和g之后，根节点u不为0，则还要单独把u给变成0，这也会使f或g增加。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
#define LINF = 0x3f3f3f3f3f3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 100005;
int n;
int ecnt, adj[N], nxt[2 * N], go[2 * N];
ll a[N], f[N], g[N];
void add(int u, int v){
    go[++ecnt] = v;
    nxt[ecnt] = adj[u];
    adj[u] = ecnt;
}
void dfs(int u, int pre){
    int v;
    for(int e = adj[u]; e; e = nxt[e])
        if(v = go[e], v != pre){
            dfs(v, u);
            f[u] = max(f[u], f[v]);
            g[u] = max(g[u], g[v]);
        }
    if(a[u] - f[u] + g[u] >= 0) f[u] += a[u] - f[u] + g[u];
    else g[u] += -(a[u] - f[u] + g[u]);
}
int main(){
    read(n);
    for(int i = 1, u, v; i < n; i++)
        read(u), read(v), add(u, v), add(v, u);
    for(int i = 1; i <= n; i++)
        read(a[i]);
    dfs(1, 0);
    write(f[1] + g[1]), enter;
    return 0;
}
```

## [51nod 1394](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1394) 差和问题 | 离散化 树状数组

开学的时候就想写的题……

现在我终于能稍微顺利一点地敲出一道离散化了……

题解：加入一个元素的时候，它会使答案增加 （比它小的元素个数 * 元素的值 - 比它小的元素之和）+（比它大的元素之和 - 比它大的元素个数 * 元素的值）。

这样只需要维护“比它小的元素个数”和“比它小的元素之和”。

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 200005;
ll n, Q, a[N], lst[N], cnt, exi[N];
ll sum[N], ans;
struct Query{
    ll type, v;
} q[N];
ll find_pos(ll v){
    return lower_bound(lst + 1, lst + cnt + 1, v) - lst;
}
void add(ll p){
    for(int i = p; i <= cnt; i += i & -i)
        exi[i]++, sum[i] += lst[p];
}
void del(ll p){
    for(int i = p; i <= cnt; i += i & -i)
        exi[i]--, sum[i] -= lst[p];
}
ll ask_exi(ll p){
    ll ret = 0;
    while(p) ret += exi[p], p -= p & -p;
    return ret;
}
ll ask_sum(ll p){
    ll ret = 0;
    while(p) ret += sum[p], p -= p & -p;
    return ret;
}
bool exist(ll p){
    return ask_exi(p) - ask_exi(p - 1) > 0;
}
int main(){
    read(n), read(Q);
    for(int i = 1; i <= n; i++)
        read(a[i]), lst[++cnt] = a[i];
    for(int i = 1; i <= Q; i++){
        read(q[i].type);
        if(q[i].type < 3)
            read(q[i].v), lst[++cnt] = q[i].v;
    }
    sort(lst + 1, lst + cnt + 1);
    for(int i = 1; i <= n; i++){
        ll p = find_pos(a[i]);
        ans += ask_exi(p) * a[i] - ask_sum(p);
        ans += ask_sum(cnt) - ask_sum(p) - (ask_exi(cnt) - ask_exi(p)) * a[i];
        add(p);
    }
    for(int i = 1; i <= Q; i++){
        if(q[i].type == 1){
            ll p = find_pos(q[i].v);
            ans += ask_exi(p) * q[i].v - ask_sum(p);
            ans += ask_sum(cnt) - ask_sum(p) - (ask_exi(cnt) - ask_exi(p)) * q[i].v;
            add(p);
        }
        else if(q[i].type == 2){
            ll p = find_pos(q[i].v);
            if(!exist(p)) {
                write(-1), enter;
                continue;
            }
            ans -= ask_exi(p) * q[i].v - ask_sum(p);
            ans -= ask_sum(cnt) - ask_sum(p) - (ask_exi(cnt) - ask_exi(p)) * q[i].v;
            del(p);
        }
        else
            write(ans), enter;
    }
    return 0;
}
```

@ 离散化的数组开小了……GG……

## [51nod 1513](http://www.51nod.com/onlineJudge/questionCode.html#!problemId=1513) 树上的回文 | bfs lca 位运算-异或

> 
罗马种了一棵树，树上有n个点。每个点有一个小写英文字母。1号点是树的根，剩下的n-1个点都有一个父亲。点和父亲之间通过一个边相连。第i个点的父亲是pi,且pi<i。

一个点的深度是从根到当前点的路径上经过的点数。根的深度是1。

U在v的子树中，当且仅当u往根方向走可以到达v。特别的，v也是在v的子树中。

罗马给你m个查询，第i个查询包含两个整数vi,hi。现在收集在vi子树中且深度是hi的结点。判断一下把这些结点重新排列是否能组成一个回文。

样例解释：

一个字符串s是回文，当且仅当他正着读和反着读是一样的。空串也是回文。

第一查询中，点1包含z，可以构成回文。

第二个查询中点5和6包含c，d，这个不能组成回文。

第三个查询中，没有点是深度4的，组成一个空串。

第四个查询中，也是构成了一个空串。

第五个查询中，2,3,4符合查询条件，包含字母a,c,c。可以构成cac。

Input

单组测试数据。

第一行包含两个n, m (1≤n,m≤500000)，表示树的结点数目和查询数目。

第二行有n-1个数字 p2,p3,...,pn ，表示每一个点的父亲。(1≤pi<i).

接下来一行有n个小写字母，第i个表示第i个点上面的字母。

接下来m行每一行有一个查询，包含vi, hi (1≤vi,hi≤n)，表示点编号和深度。

Output

对于每一个查询，如果能够组成回文，输出Yes,否则输出No。

Input示例

6 5

1 1 1 3 3

zacccd

1 1

3 3

4 1

6 1

1 2

Output示例

Yes

No

Yes

Yes

Yes


可以把树“从上到下、从左到右，一行一行”地写成一个序列，则题中所求的这些节点一定会挨在一起。加入我们知道第一个节点的位置fi、最后一个节点的位置se、前缀异或和sum[i]，则sum[se]^sum[fi - 1]可以知道哪些出现了奇数次，进而可以求出是否可行。

关键是求出fi和se：可以用类似求LCA的方法，记录每个节点往上走j步会到达哪个祖先，然后两次二分找出fi和se即可。

@ 写序列的时候，我给点重新编了个号，然后……就把这个编号和原有编号整混了……GG……

```
#include <cstdio>
#include <cstring>
#include <algorithm>
#define INF 0x3f3f3f3f
#define space putchar(' ')
#define enter putchar('\n')
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
        if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
        x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 500005;
int n, m, bfn[N], q[N], anc[N][23], dep[N], sum[N], adj[N], nxt[N];
char data[N];
void add(int u, int v){
    nxt[v] = adj[u], adj[u] = v;
}
void bfs(){
    int l = 1, r = 0, u;
    q[++r] = 1, dep[1] = 1;
    while(l <= r){
        u = q[l], bfn[u] = l++;
        for(int v = adj[u]; v; v = nxt[v])
            anc[v][0] = u, q[++r] = v, dep[v] = dep[u] + 1;
    }
}
void init(){
    for(int j = 1; (1 << j) <= n; j++)
        for(int i = 1; i <= n; i++)
            anc[i][j] = anc[anc[i][j - 1]][j - 1];
    for(int i = 1; i <= n; i++)
        sum[i] = sum[i - 1] ^ (1 << (data[q[i]] - 'a'));
}
int getanc(int v, int d){
    for(int i = 0; d; i++, d >>= 1)
        if(d & 1) v = anc[v][i];
    return v;
}
bool judge(int se, int fi){
    int ans = sum[se] ^ sum[fi - 1];
    if((se - fi + 1) & 1) return ans - (ans & -ans) == 0;
    else return ans == 0;
}
int main(){
    read(n), read(m);
    for(int v = 2, u; v <= n; v++)
        read(u), add(u, v);
    scanf("%s", data + 1);
    bfs();
    init();
    while(m--){
        int u, h, l, r, mid, d, fi, se;
        read(u), read(h), d = h - dep[u];
        if(d < 0) {
            puts("Yes");
            continue;
        }
        l = bfn[u], r = n;
        while(l < r){
            mid = (l + r) >> 1;
            if(bfn[getanc(q[mid], d)] < bfn[u]) l = mid + 1;
            else r = mid;
        }
        if(getanc(q[l], d) != u) {
            puts("Yes");
            continue;
        }
        fi = l;
        r = n;
        while(l < r){
            mid = (l + r + 1) >> 1;
            if(bfn[getanc(q[mid], d)] > bfn[u]) r = mid - 1;
            else l = mid;
        }
        se = l;
        if(judge(se, fi)) puts("Yes");
        else puts("No");
    }
    return 0;
}
```

# [返回顶部](#查看最新)












