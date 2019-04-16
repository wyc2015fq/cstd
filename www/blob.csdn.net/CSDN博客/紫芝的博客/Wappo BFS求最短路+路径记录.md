# Wappo BFS求最短路+路径记录 - 紫芝的博客 - CSDN博客





2018年07月15日 13:26:44[紫芝](https://me.csdn.net/qq_40507857)阅读数：247










# [点击打开链接](https://vijos.org/p/1044)

# 描述



Einstein最近迷上了一款手机益智游戏Wappo，但是由于IQ和RP等诸多原因，他一直无法通关，他希望你编一个程序来玩这个游戏。

Wappo的游戏规则是这样的：在一张m*n的地图上，你可以控制勇士每秒向上下左右任意方向移动一格，之后怪兽会朝着你的方向移动至多两格。

注意，怪兽会优先横向走，勇士和怪兽都不会穿墙。

勇士的目的地就是桥头，但是千万不能被怪兽吃掉。

陷阱是很有用的东西（一格），勇士是不能进入陷阱的，但是如果你的IQ足够高，就可以把怪兽引入陷阱，在接下来你的三次移动中，怪兽将无法移动，三秒后恢复正常。

现在给你地图的信息，请你用最少的步数走到桥上。

# 格式

## 输入格式

第一行是两个正整数m,n(m,n<=20),表示地图的大小为m*n.
接下来是n行，每行m个整数,表示一个格子周围墙的信息。其中
1-上方有墙
2-左方有墙
4-右方有墙
8-下方有墙
例如，一个格子的左、上、右都有墙，那么代表这个格子的整数是2+1+4=7。

接下来是n行，每行m个字符，表示一个格子里的其他信息，其中
.-nothing
S-勇士的初始位置
W-陷阱
M-怪兽的初始位置
E-目的地，即桥头
其中S,M,E均保证只出现一次。

## 输出格式

输出包含若干行，前r行为最少步数走到桥头的走法，每行为up,down,left,right中的一个，表示勇士的走法。最后一行输出最少步数r，格式见样例。

若存在多解，按照上左右下的优先顺序行走。

若无法走到桥头，只输出一行impossible。

# 样例1

## 样例输入1

```
6 6
0  8  8  8  8  0
4  3  1  1  5  2
4  2  0  4  6  2
4  2  0  4  6  2
4 10  8  8 12  2
0  1  1  1  1  0
......
.S..W.
....M.
......
...E..
......
```


Copy


## 样例输出1

```
right
right
down
down
down
total steps: 5
```


Copy


# 提示

友情提示：怪兽掉进陷阱以后，如果没有走出来，不会继续掉进去。

## 分析：

（1）怪兽被困陷阱里到时间了以后如果没有移动不算再掉进去（那样就永远出不来了）；

（2）在人走之后要判断是否到终点（如果已到终点则不用考虑怪兽了，只不过有的Wappo版本里即使人走到终点也不算过关，需要怪兽进行最后一次操作，若没有抓到人才算过关）以及是否直接碰到怪兽（那不是自杀？）


（3）怪兽在不能横着走的情况下会竖着走，如果不能竖着走的话就不动

（怪兽有点SB，如果人在它左上角则它只会向左或向上）



### 关于怪兽的行走逻辑，分两个阶段：
走第一步，如果怪兽与你的水平方向不一致，
那么怪兽会企图往竖直方向走动使得他与你的水平方向一致
这时侯可能竖直方向有墙（没有墙就走过去进入第二阶段）
那么怪兽就会往水平方向企图走，如果没有墙就走，否则原地不动
然后不再走第二步。
走第二步，和第一步类似，需要考虑优先级和可行性

```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i,n) for (int i=1;i<=n;i++)
#define REP(i,a,b) for (int i=a;i<=b;i++)
#define pb push_back
#define mp make_pair
#define ll long long
#define pos(x,y) (x+(y)*n)
const int N=100000+10;
const int inf=0x3f3f3f3f;
const ll mod=1000000007;
const double eps=1e-8;

int n,m;
int a[25][25];
int b[25][25];
struct node {
int x,y,x2,y2,t;
int pre;
int d;
int dir;
// t为陷入陷阱后还剩几秒钟出来,t=0表示没陷入陷阱
void operator=(node a) {
    x=a.x,y=a.y,x2=a.x2,y2=a.y2,t=a.t;
    pre=a.pre;
    d=a.d;
    dir=a.dir;
}   };
bool vis[25][25][25][25][5];//状态
node q[400*400*3+10];
// x,y,x2,y2,t
int head,tail;
int X,Y,X2,Y2,EX,EY;
bool ok(int x,int y) {
return 1<=x&&x<=n&&1<=y&&y<=m&&!b[x][y];
}
void print(node a) {
vector<int> v;
int steps=a.d;
while (1) {
if (a.pre==0) break;
v.pb(a.dir);
a=q[a.pre];
//cout<<a.x<<" "<<a.y<<endl;
}
while (v.size()) {
if (v.back()==1) cout<<"up"<<endl;
if (v.back()==2) cout<<"left"<<endl;
if (v.back()==3) cout<<"right"<<endl;
if (v.back()==4) cout<<"down"<<endl;
v.pop_back();
}
cout<<"total steps: "<<steps<<endl;
}
void work(int &x,int &y,int &x2,int &y2,node &tmp) {
if (tmp.t) tmp.t--;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
}
}
}
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
else {
if (y2<y) {
if (!(a[x2][y2]&4)) {
y2++;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (y2>y) {
if (!(a[x2][y2]&2)) {
y2--;
if (b[x2][y2]) tmp.t=3;
} else {
if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
} else if (x2>x) {
if (!(a[x2][y2]&1)) {
x2--;
if (b[x2][y2]) tmp.t=3;
}
} else if (x2<x) {
if (!(a[x2][y2]&8)) {
x2++;
if (b[x2][y2]) tmp.t=3;
}
}
}
}
}
}
if (!(x==EX&&y==EY)&&x==x2&&y==y2) return;
if (!vis[tmp.x][tmp.y][tmp.x2][tmp.y2][tmp.t]) {
vis[tmp.x][tmp.y][tmp.x2][tmp.y2][tmp.t]=1;
q[++tail]=tmp;
}
}
int main() {
//freopen("in.txt","r",stdin);
//freopen("out.txt","w",stdout);
cin>>m>>n;
FOR(i,n) FOR(j,m) cin>>a[i][j];
FOR(i,n) FOR(j,m) {
char t='\n';
while (t=='\n') cin>>t;
if (t=='S') X=i,Y=j;
else if (t=='W') b[i][j]=1;
else if (t=='M') X2=i,Y2=j;
else if (t=='E') EX=i,EY=j;
}
head=tail=1;
q[1]=node{X,Y,X2,Y2,0,0,0,0};
vis[X][Y][X2][Y2][0]=1;
while (head<=tail) {
node u=q[head++];
if (u.x==EX&&u.y==EY) {
print(u);
return 0;
}
node tmp;
tmp=u;
tmp.d++;
tmp.pre=head-1;
if (!(a[tmp.x][tmp.y]&1)&&ok(tmp.x-1,tmp.y)&&!(tmp.x2==tmp.x-1&&tmp.y2==tmp.y)) {
tmp.x--,tmp.dir=1;
work(tmp.x,tmp.y,tmp.x2,tmp.y2,tmp);
}
tmp=u;
tmp.d++;
tmp.pre=head-1;
if (!(a[tmp.x][tmp.y]&2)&&ok(tmp.x,tmp.y-1)&&!(tmp.x2==tmp.x&&tmp.y2==tmp.y-1)) {
tmp.y--,tmp.dir=2;
work(tmp.x,tmp.y,tmp.x2,tmp.y2,tmp);
}
tmp=u;
tmp.d++;
tmp.pre=head-1;
if (!(a[tmp.x][tmp.y]&4)&&ok(tmp.x,tmp.y+1)&&!(tmp.x2==tmp.x&&tmp.y2==tmp.y+1)) {
tmp.y++,tmp.dir=3;
work(tmp.x,tmp.y,tmp.x2,tmp.y2,tmp);
}
tmp=u;
tmp.d++;
tmp.pre=head-1;
if (!(a[tmp.x][tmp.y]&8)&&ok(tmp.x+1,tmp.y)&&!(tmp.x2==tmp.x+1&&tmp.y2==tmp.y)) {
tmp.x++,tmp.dir=4;
work(tmp.x,tmp.y,tmp.x2,tmp.y2,tmp);
}
}
cout<<"impossible\n"<<endl;
return 0;
}
```




