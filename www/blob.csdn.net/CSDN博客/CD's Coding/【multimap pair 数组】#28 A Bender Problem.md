# 【multimap/pair/数组】#28 A. Bender Problem - CD's Coding - CSDN博客





2014年06月05日 18:00:47[糖果天王](https://me.csdn.net/okcd00)阅读数：794
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Robot Bender decided to make Fray a birthday present. He drove *n* nails and numbered them from 1 to *n* in
 some order. Bender decided to make a picture using metal rods. The picture is a closed polyline, which vertices should be nails (in the given order). The segments of the polyline should be parallel to the coordinate axes. Polyline is allowed to have self-intersections.
 Bender can take a rod and fold it exactly once in any place to form an angle of 90 degrees. Then he can attach the place of the fold to some unoccupied nail and attach two ends of this rod to adjacent nails. A nail is considered unoccupied if there is no rod
 attached to it (neither by it's end nor the by the fold place). No rod could be used twice. It is not required to use all the rods.


Help Bender to solve this difficult task.




Input


The first line contains two positive integers *n* and *m* (4 ≤ *n* ≤ 500, 2 ≤ *m* ≤ 500, *n* is
 even) — the amount of nails and the amount of rods. *i*-th of the following *n* lines
 contains a pair of integers, denoting the coordinates of the *i*-th nail. Nails should be connected in the same order as they are given in the input. The
 last line contains *m* integers — the lenghts of the rods. All coordinates do not exceed 104 by
 absolute value. Lengths of the rods are between 1 and 200 000. No
 rod can be used twice. It is guaranteed that all segments of the given polyline are parallel to coordinate axes. No three consecutive nails lie on the same line.




Output


If it is impossible to solve Bender's problem, output NO. Otherwise, output YES in
 the first line, and in the second line output *n*numbers — *i*-th
 of them should be the number of rod, which fold place is attached to the *i*-th nail, or -1,
 if there is no such rod.


If there are multiple solutions, print any of them.




Sample test(s)




input
4 2
0 0
0 2
2 2
2 0
4 4




output
YES
1 -1 2 -1 




input
6 3
0 0
1 0
1 1
2 1
2 2
0 2
3 2 3




output
YES
1 -1 2 -1 3 -1 




input
6 3
0 0
1 0
1 1
2 1
2 2
0 2
2 2 3




output
NO





















看到一道好长的题我主要是比较懒的看题，就去cf上想找到题意，然后——

然后我就看到了这么些看起来好吓人的解题报告：

A. [Bender
 Problem](http://codeforces.ru/contest/28/problem/A)
Solution by [Polichka](http://codeforces.ru/profile/Polichka)
Let's look at the first nail. If it is occupied by the fold place, then Bender will put next fold place on the third
 nail, then on fifth and so on. Else, is the first nail occupied by end, than second, fourth, sixth and so on nail will be occupied by the fold places.
Let's see, if we can complete our polyline with the first nail, occupied by rhe fold place. It means we should check,
 if we have an unused pod with length *dist*(*nails*[*n*], *nails*[1]) + *dist*(*nails*[1], *nails*[2]).
 Then check the third nail and so on.
If we have completed the polyline, then we have an answer. Else repeat previous procedure, but starting from the second
 nail




**А. Задача Бендера.**



Будем говорить, что мы крепим*j*-ый прут к*i*-ому
 гвоздю, если мы крепим*j*-ый прут местом сгиба к *i* -ому
 гвоздю.


Для начала поймем, что если мы прикрепляем прут к первому гвоздю, то остальные прутья мы сможем прикреплять только к третьему, пятому и т.д. Соответственно, если мы крепим прут ко второму гвоздю, то
 все остальные прутья мы можем крепить к четвертому, шестому и т.д. То есть либо мы крепим все прутья к гвоздям с четным номером, либо все к гвоздям с нечетным.

Если задача имеет решение, то нам надо выбрать к каким по четности гвоздям мы будем крепить прутья, а также поднабор из*n*/
 2 прутьев, который и будет считаться ответом.

Если мы крепим*j*-ый прут к*i*-ому
 гвоздю, то*len*[*j*] =*dist*(*i, i*- 1) +*dist*(*i, i*+ 1), где*len*[*j*]
 - длина*j*-ого прута, а*dist*(*i*,*k*)
 - расстояние между гвоздями с номерами*i*и*k*.

Ограничения задачи позволяли решать ее за*O*(*nm*).
  Проходим внешним циклом по гвоздям, к которым будут крепиться пруты(сначала по гвоздям с четными номерами, потом по гвоздям с нечетными номерами), проходим внутренним циклом по прутьям и проверяем: можем ли мы прикрепить хотя бы один из оставшихся прутьев
 к текущему гвоздю. Если можем, то помечаем этот прут и далее его не рассматриваем, если нет, то проделываем то же самое для гвоздей с нечетными номерами, но при этом нужно снять все пометки с прутьев, сделанные при рассмотрении гвоздей с четными номерами.
 Если же и для нечетного случая мы не нашли ответ, то искомого поднабора не существует.

这道题我个人嗯理解的意思是有这么多个钉子，相邻的俩一定是水平或者竖直的，有这么多线要做个框把它框起来，每根线可以折着用，问咋框。

My Source Code： 30ms AC 0k length：1578 B



```cpp
#include <cmath> 
#include <vector>
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

struct point
{
	int x;
	int y;
}a[505];//list
int		n=0,m=0;
int 	len[505];
int		ans[505];
int		mrk[505];

void print()
{
	puts("YES");
	for ( int i=1; i<=n; i++ )
		printf("%d ",(ans[i]) ? ans[i] : -1);
}

bool judge(int i, int j)
{
	return len[j] == ( abs(a[i-1].x - a[i].x) + abs(a[i-1].y - a[i].y) ) + ( abs(a[i+1].x - a[i].x) + ( abs(a[i+1].y - a[i].y ) ) ) ;//绳长是否与两个钉子间距相等 
}

int main()
{
	memset(len,0,sizeof(len));
	memset(ans,0,sizeof(ans));
	memset(mrk,0,sizeof(mrk));
	cin>>n>>m;
	for(int i=1;i<=n;i++)	cin>>a[i].x>>a[i].y;
	for(int j=1;j<=m;j++)	cin>>len[j];
	//consider about the head and tail
	a[0].x = a[n].x;
	a[0].y = a[n].y;
	a[n+1].x = a[1].x;
	a[n+1].y = a[1].y;
	//start from 1st or 2nd
	int type = 1 ,flag = 0, t = 0;
	for(int i=type;i<=n;i+=2)
	{
		flag = 0;
		for (int j=1; j<=m; j++) 
		if (!mrk[j] && judge(i,j))  
		{
			flag = 1;
			ans[i] = j;
			mrk[j] = 1;
			break; 
		}
		if ( flag == 0 ) { t = 1; break; }
	}
	if ( !t ) print();
	else
	{
		type++;
		memset(ans,0,sizeof(ans));
		memset(mrk,0,sizeof(mrk));
		for(int i=type;i<=n;i+=2)
		{
			flag = 0;
			for (int j=1; j<=m; j++) 
			if (!mrk[j] && judge(i,j))  
			{
				flag = 1;
				ans[i] = j;
				mrk[j] = 1;
				break; 
			}
			if ( flag == 0 ) { puts("NO");return 0;}
		}
		print();
	}	
	return 0;
}
```









备看——




C++：

【multimap】解法 From：watashi@cf



```cpp
#include <map>
#include <cstdio>
#include <algorithm>

using namespace std;

int main() {
	int n, m, t, x[1024], y[1024];
	multimap<int, int> r, rr;
	multimap<int, int>::iterator it;

	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i) {
		scanf("%d%d", &x[i], &y[i]);
	}
	x[0] = x[n];
	y[0] = y[n];
	x[n + 1] = x[1];
	y[n + 1] = y[1];
	for (int i = 0; i <= n; ++i) {
		x[i] = abs(x[i + 1] - x[i]) + abs(y[i + 1] - y[i]);
	}
	for (int i = 1; i <= m; ++i) {
		scanf("%d", &t);
		rr.insert(make_pair(t, i));
	}

	for (int s = 1; s <= 2; ++s) {
		bool flag = true;
		r = rr;
		y[s - 1] = -1;
		for (int i = s; i <= n; i += 2) {
			it = r.find(x[i - 1] + x[i]);
			if (it == r.end()) {
				flag = false;
				break;
			}
			y[i] = it->second;
			y[i + 1] = -1;
			r.erase(it);
		}
		if (flag) {
			puts("YES");
			for (int i = 1; i <= n; ++i) {
				printf("%d ", y[i]);
			}
			return 0;
		}
	}
	puts("NO");

	return 0;
}
```




C++： 【pair】

From：zeonsgtr@cf



```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <sstream>
#include <numeric>
#include <climits>
#include <string>
#include <cctype>
#include <ctime>
#include <cmath>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <set>
using namespace std;

#define x first
#define y second

typedef pair<int, int> Point;

const int N = 555;

int n, m;
int len[N];
int ans[N];
bool visited[N];
Point a[N];

int dist(const Point &a, const Point &b, const Point &c) {
	return abs(a.x - b.x) + abs(a.y - b.y) + abs(b.x - c.x) + abs(b.y - c.y);
}

bool check(bool flag) {
	memset(visited, 0, sizeof(visited));
	memset(ans, -1, sizeof(ans));
	for (int i = flag; i < n; i += 2) {
		int need = dist(a[(i - 1 + n) % n], a[i], a[(i + 1) % n]);
		flag = false;
		for (int j = 0; j < m; ++j)
			if (!visited[j] && len[j] == need) {
				flag = true;
				visited[j] = true;
				ans[i] = j + 1;
				break;
			}
		if (!flag)
			return false;
	}
	return true;
}

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i)
		scanf("%d%d", &a[i].x, &a[i].y);
	for (int i = 0; i < m; ++i)
		scanf("%d", len + i);
	if (check(0) || check(1)) {
		puts("YES");
		for (int i = 0; i < n; ++i)
			printf("%d%c", ans[i], i < n - 1 ? ' ' : '\n');
	} else {
		puts("NO");
	}
}
```








C++: 

15ms 0k完美解法 From：Choojae@cf





```cpp
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <cctype>
#include <algorithm>
#define maxn 505

using namespace std;

int n,m,l[maxn],x[maxn],y[maxn],ans[maxn];
bool flag[maxn];

void init()
 {
  scanf("%d%d",&n,&m);
  int i;
  for (i=0;i<n;i++)scanf("%d%d",&x[i],&y[i]);
  for (i=1;i<=m;i++)scanf("%d",&l[i]);
 }
 
void work()
 {
  int i,st,j,k;
  for (st=0;st<=1;st++)
   {
    memset(ans,-1,n*4);
    memset(flag+1,false,m*sizeof(bool));
    for (i=st;i<n;i+=2)
     {
      k=abs(x[i]-x[(i+n-1)%n])+abs(y[i]-y[(i+n-1)%n])+abs(x[(i+1)%n]-x[i])+abs(y[(i+1)%n]-y[i]);
      for (j=1;j<=m;j++)if (l[j]==k && !flag[j]){flag[j]=true;ans[i]=j;break;}
      if (ans[i]==-1)break;
     }
    if (i>=n)
     {
      puts("YES");
      for (i=0;i<n-1;i++)printf("%d ",ans[i]);printf("%d\n",ans[n-1]);
      return;
     }
   }
  puts("NO");
 }
 
int main()
 {
  init();
  work();
  return 0;
 }
```








Python解法：



```python
R=lambda:map(int,raw_input().split())
n,m=R()
a=map(lambda x:complex(*x),(R() for _ in xrange(n)))
b=R()
def nail(i):
  c={}
  for u,x in enumerate(b):c[x]=c.get(x,[])+[u+1]
  t=[-1]*n
  for u in xrange(0,n,2):
    w=int(abs(a[i]-a[i-1])+abs(a[(i+1)%n]-a[i])+0.5)
    if c.get(w):
      t[i]=c[w].pop()
    else:
      return 0
    i+=2
  return 'YES\n'+' '.join(map(str,t))
print nail(0) or nail(1) or 'NO'
```







