# STL中sort的使用 _Updated 150721 - CD's Coding - CSDN博客





2013年09月25日 19:55:33[糖果天王](https://me.csdn.net/okcd00)阅读数：602










**STL中sort函数用法简介**

做 ACM题的时候，排序是一种经常要用到的操作。如果每次都自己写个冒泡之类的O(n^2)排序，不但程序容易超时，而且浪费宝贵的比赛时间，还很有可能写错。STL里面有个sort函数，可以直接对数组排序，复杂度为n*log2(n)。

使用这个函数，需要包含头文件#include <algorithm>。

这个函数可以传两个参数或三个参数。第一个参数是要排序的区间首地址，第二个参数是区间尾地址的下一地址。也就是说，排序的区间是[a,b)。简单来说，有一个数组int
 a[100]，要对从a[0]到a[99]的元素进行排序，只要写sort(a,a+100)就行了，默认的排序方式是升序。

拿我出的“AC的策略”这题来说，需要对数组t的第0到len-1的元素排序，就写sort(t,t+len);

对向量 v排序也差不多，sort(v.begin(),v.end());

排序的数据类型不局限于整数，只要是定义了小于运算的类型都可以，比如字符串类string。

如果是没有定义小于运算的数据类型，或者想改变排序的顺序，就要用到第三参数——比较函数。比较函数是一个自己定义的函数，返回值是bool型，它规定了什么样的关系才是“小于”。想把刚才的整数数组按降序排列，可以先定义一个比较函数cmp

```cpp
bool cmp(int a,int b)  { return a>b; }
```



排序的时候就写 sort(a,a+100,cmp);

假设自己定义了一个结构体 node

```cpp
struct node{ 
int a; 
int b; 
double c; 
}
```




有一个 node类型的数组node
 arr[100]，想对它进行排序：先按a值升序排列，如果a值相同，再按b值降序排列，如果b还相同，就按c降序排列。就可以写这样一个比较函数：

以下是代码片段：

```cpp
bool cmp(node x,node y) { 
if(x.a!=y.a) return x.a>y.b;
if(x.b!=y.b) return x.b>y.b; 
return x.c>y.c; 
}
```




排序时写 sort(arr,a+100,cmp);

最后看一个完整的实例，初赛时的一道题目“文件名排序 ”。

以下是代码片段：

```cpp
#include<iostream> 
#include<algorithm> 
#include<string> 
using namespace std; 
// 定义一个结构体来表示文件， a 代表文件名， b 代表文件类型（要么 "File" 要么 "Dir" ）
struct node{ 
string a,b; 
}; 

//ASCII 码中，所有大写字母排在所有小写字母前面， 'A'<'Z'<'a'<'z' 
// 而这题要求忽略大小写，所以不能直接用字符串的比较。自定义了一个 lt 函数，就是 less than 的意思
// 先把两个字符串全部转化为小写，再比较大小（字典序）

bool lt(string x,string y) 
{ 
int i; 
for(i=0;i<x.length();i++) 
if(x[i]>='A'&&x[i]<='Z') 
x[i]='a'+(x[i]-'A'); 
for(i=0;i<y.length();i++) 
if(y[i]>='A'&&y[i]<='Z') 
y[i]='a'+(y[i]-'A'); 
return x<y; 
} 

// 自定义的比较函数，先按 b 值升序排列（也就是 "Dir" 排在 "File" 前面）
// 如果 b 值相同，再按 a 升序排列，用的是刚才定义的 lt 函数

bool comp(node x,node y) 
{ 
if(x.b!=y.b)return x.b<y.b; 
return lt(x.a,y.a); 
} 

int main() 
{ 
node arr[10001]; 
int size=0; 
while(cin>>arr[size].a>>arr[size].b) 
size++; 
sort(arr,arr+size,comp); 
for(int i=0;i<size;i++) 
cout<<arr[i].a<<" "<<arr[i].b<<endl; 
return 0; 
}
```



<Updated 20150721>


感谢Zoecur指出上述代码中的两处错误，已改正，为两年前的不注意道歉。


以及，之前在竞赛中遇到过sort和qsort的混淆问题，弄清之后也贴于此处。 


sort和qsort所需要的cmp函数不同，并且调用的时候的姿势也不同——





【sort_cmp.cpp】



```cpp
#include <queue>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define MAXN 100

struct In 
{    
	int data;   
	char str[100];   
}s[MAXN];   
  
// 按照结构体中字符串str字典序排序   
inline int cmp (const In a, const In b){return strcmp( a.str , b.str );}

struct Point  
{   
	int x;   
	int y;   
}p[MAXN];   
  
// 按照 x 从小到大排序，当 x 相等时按照 y 从大到小排序   
int cmp2( const Point a , const Point b )
{   
	if(a.x != b.x) return a.x - b.x;   
	else return b.y - a.y;   
}   

int main()
{
	sort(s,s+MAXN,cmp);  
	sort(p,p+MAXN,cmp2); 
	return 0;	
}
```






【qsort_cmp.cpp】

```cpp
#include <queue>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define MAXN 100

struct In 
{    
	int data;   
	char str[100];   
}s[MAXN];   
  
// 按照结构体中字符串str字典序排序   
int cmp ( const void *a , const void *b )   
{   
	return strcmp( (*(In *)a).str , (*(In *)b).str );   
}   

struct Point  
{   
	int x;   
	int y;   
}p[MAXN];   
  
// 按照 x 从小到大排序，当 x 相等时按照 y 从大到小排序   
int cmp2( const void *a , const void *b )
{   
	struct Point *c = (Point *)a;   
	struct Point *d = (Point *)b;   
	if(c->x != d->x) return c->x - d->x;   
	else return d->y - c->y;   
}   

int main()
{
	qsort(s,MAXN,sizeof(s[0]),cmp);  
	qsort(p,MAXN,sizeof(p[0]),cmp2); 
	return 0;	
}
```


关于较为常用的凸包排序cmp函数，今年校赛也有设计到，将此题也贴于此处方便查阅

[校赛Solution传送门](http://blog.csdn.net/okcd00/article/details/46301605)



### **Problem_[G]raphic**





**timelimit 2 seconds / memory limit 256 megabytes**



**Description**


Curabis always do excellent job on computation geometry problems. Indeed, he has acquired the first solution award about the computation geometry problems in the ACM-ICPC Asia Regional several times. However, JKi, his stupid teammate, do it much worse. 


In order to help poor JKi, Curabis decide provide him with some practice problem set about computation geometry.


One of those problems is this:


Give a set of N2d-points on the plane, is there any three 2d-points of the 2d-point set areable to constructive an triangle, and all 2d-points of this set are either inside of the triangle or on the edge of the triangle?


Would you please help JKi finishing this program?



**Input**


The first line of the INPUT contain a positive integer T(T<=20) which denotes the number of test cases.


The first line of each case contains an integer N.


The next N(3<=N<=1000) lines of each case contain the 2d-points set. The i-th line contains two real numbers Xi, Yi denote the position of the 2d-point.


It is guaranteed that -1000.00<= Xi, Yi <=1000.00, and all real numbers in the input keep two decimal places exactly.


It is guaranteed that all 2d-points in any case can not stand on one straight line over all.



**Output**


In each line print an upper-case letter — ‘Y’ for the triangle is existed, otherwise ‘N’ instead.


|**Sample Input:**|**Sample Output:**|
|----|----|
|340.00 0.000.00 2.002.00 0.000.50 0.5040.00 0.000.00 2.002.00 0.002.00 2.0050.00 0.000.00 0.001.00 0.002.00 0.001.00 1.00|YNY|


梗: 其实我想说这个是不是博熊的ID啊…… <Update>啊啦被我猜对了~~

题意：有好多个点，给所有点的坐标，问是否存在三个点，他们构成的三角形可以包住所有的点（在线上和在点上也算包住）


思路：啊，包住，凸包嘛~ 题目等效于，这些点的凸包定点数是否不大于3，是的话输出Y，反之输出N


（可以作为凸包模板哟）






```cpp
#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
const double EPS=1e-10;

double add(double a,double b)
{
    if( fabs(a+b) < EPS*( abs(a)+abs(b)) )return 0;
    return a+b;
}

struct P
{
    double x;
    double y;
    P(){}
    P(double x,double y):x(x),y(y){}
    P operator+ (P p)
    {return P(add(x,p.x),add(y,p.y));}
    P operator- (P p)
    {return P(add(x,-p.x),add(y,-p.y));}
    P operator* (double d)
    {return P(x*d,y*d);}
    double dot(P p)//neiji
    {return add(x*p.x,y*p.y);}
    double det(P p)//waiji
    {return add(x*p.y,-y*p.x);}
};

bool cmp_x(const P& p,const P& q)
{
    if(p.x != q.x) return p.x<q.x;
    return p.y<q.y;
}

vector<P> convex_hull(P* ps,int n)
{
    sort(ps,ps+n,cmp_x);
    //for(int i=0;i<n;i++){cout<<ps[i].x<<":"<<ps[i].y<<endl;}
    int k=0;
    vector<P> qs(n<<1);
    for(int i=0;i<n;i++)
    {
        while( k>1 && (qs[k-1]-qs[k-2]).det(ps[i]-qs[k-1])<=0 )k--;
        qs[k++]=ps[i];
    }
    for(int i=n-2,t=k; i>=0; i--)
    {
        while( k>t && (qs[k-1]-qs[k-2]).det(ps[i]-qs[k-1])<=0 )k--;
        qs[k++]=ps[i];
    }
    qs.resize(k-1);
    return qs;
}

int N;
P ps[1024];

void solve()
{
    vector<P> qs = convex_hull(ps,N);
    //cout<<qs.size()<<endl;
    if(qs.size()<=3)printf("Y\n");
    else printf("N\n");
}

int main()
{
    freopen("G.in","r",stdin);
    int T; scanf("%d",&T);
    for(int _T=0;_T<T;_T++)
    {
        double tx,ty;
        scanf("%d",&N);
        memset(ps,0,sizeof ps);
        for(int i=0;i<N;i++)
        {
            cin>>tx>>ty;
            //cout<<tx<<":"<<ty<<endl;
            ps[i].x=tx;
            ps[i].y=ty;
        }
        solve();
    }
    return 0;
}
```













