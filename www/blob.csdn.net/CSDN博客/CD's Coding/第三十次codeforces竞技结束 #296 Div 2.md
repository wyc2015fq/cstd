# 第三十次codeforces竞技结束 #296 Div 2 - CD's Coding - CSDN博客





2015年03月18日 15:28:53[糖果天王](https://me.csdn.net/okcd00)阅读数：545











Problems

![](http://st.codeforces.com/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.com/contest/527/problem/A)|[Playing with Paper](http://codeforces.com/contest/527/problem/A)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x2429|
|[B](http://codeforces.com/contest/527/problem/B)|[Error Correct System](http://codeforces.com/contest/527/problem/B)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x953|
|[C](http://codeforces.com/contest/527/problem/C)|[Glass Carving](http://codeforces.com/contest/527/problem/C)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x584|





第三十次……好想到紫啊好想紫55555，两个1A，14min，然后我看Standing的时候你们造么！ 28名啊！！！

然后被C骗住了，我以为是zkw线段树……居然是考察数据结构set的问题……

然后……Failed System Test…… 原因？











One day Vasya was sitting on a not so interesting Maths lesson and making an origami from a rectangular *a* mm  × *b*mm
 sheet of paper (*a* > *b*). Usually the first step in making an origami is making a square piece of paper from the rectangular sheet by folding the
 sheet along the bisector of the right angle, and cutting the excess part.
![](http://espresso.codeforces.com/d08456364e85fba94b8052a28e15a78b332e20e7.png)

After making a paper ship from the square piece, Vasya looked on the remaining (*a* - *b*) mm  × *b* mm
 strip of paper. He got the idea to use this strip of paper in the same way to make an origami, and then use the remainder (if it exists) and so on. At the moment when he is left with a square piece of paper, he will make the last ship from it and stop.


Can you determine how many ships Vasya will make during the lesson?




Input


The first line of the input contains two integers *a*, *b* (1 ≤ *b* < *a* ≤ 1012)
 — the sizes of the original sheet of paper.




Output


Print a single integer — the number of ships that Vasya will make.




Sample test(s)




input
2 1




output
2




input
10 7




output
6




input
1000000000000 1




output
1000000000000






Note


Pictures to the first and second sample test.
![](http://espresso.codeforces.com/8b51f4e6a6b5bfbfc7a50451f2bd16fe45c6dbac.png)







说一张长方形的纸~，每次都以较短边为边长裁一个正方形下来折个纸船，问长宽已知的长方形纸能折多少个纸船。

我想说……做题的时候有这个图么！！！

每次如果裁剪的效果没有达到a的剩余部分<b，那下次依然要剪b边长的正方形。


其实看到这个图大家应该就立马懂了，每次不要一裁裁一个正方形，咱们要裁一串，就是 a/b 个。

### Code：



```cpp
#include <cmath> 
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

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	long long a=0,b=0,c=0,t=0;
	cin>>a>>b;
	while(a!=b)
	{
		if(a%b)
		{
			c+=(a/b);
			a=a%b;
			t=a;
			a=b;
			b=t;
		}
		else
		{
			c+=(a/b);
			a=b;
		}
	}
	cout<<c<<endl; 
	return 0;
}
```









Ford Prefect got a job as a web developer for a small company that makes towels. His current work task is to create a search engine for the website of the company. During the development process, he needs to write a subroutine for comparing strings *S* and *T* of
 equal length to be "similar". After a brief search on the Internet, he learned about theHamming distance between two strings *S* and *T* of
 the same length, which is defined as the number of positions in which *S* and *T* have
 different characters. For example, the Hamming distance between words "permanent" and "pergament"
 is two, as these words differ in the fourth and sixth letters.


Moreover, as he was searching for information, he also noticed that modern search engines have powerful mechanisms to correct errors in the request to improve the quality of search. Ford doesn't know much about human beings, so he assumed that the most common
 mistake in a request is swapping two arbitrary letters of the string (not necessarily adjacent). Now he wants to write a function that determines which two letters should be swapped in string*S*,
 so that the Hamming distance between a new string *S* and string *T* would
 be as small as possible, or otherwise, determine that such a replacement cannot reduce the distance between the strings.


Help him do this!




Input


The first line contains integer *n* (1 ≤ *n* ≤ 200 000)
 — the length of strings *S* and *T*.


The second line contains string *S*.


The third line contains string *T*.


Each of the lines only contains lowercase Latin letters.




Output


In the first line, print number *x* — the minimum possible Hamming distance between strings *S* and *T* if
 you swap at most one pair of letters in *S*.


In the second line, either print the indexes *i* and *j* (1 ≤ *i*, *j* ≤ *n*, *i* ≠ *j*),
 if reaching the minimum possible distance is possible by swapping letters on positions *i* and *j*,
 or print "-1 -1", if it is not necessary to swap characters.


If there are multiple possible answers, print any of them.




Sample test(s)




input
9
pergament
permanent




output
1
4 6




input
6
wookie
cookie




output
1
-1 -1




input
4
petr
egor




output
2
1 2




input
6
double
bundle




output
2
4 1






Note


In the second test it is acceptable to print *i* = 2, *j* = 3.










所谓海明距离，就是字符串s和字符串t字符不同的位置个数，比如acm和acg，有一个字符不同，所以是1.



题目问，如果最多允许把s中的两个不同位置的字符调换位置，那么调换后（也可以不调换）海明距离最小是多少。

假设原先海明距离是k，不调换肯定依然是k，调换的话如果要比k小只有2种情况：

1）[s]A [t]B 和某处的[s]B [t]A 调换位置，那么有两个不同处被解决了，即海明距离小了2个，最小为k-2.

2）[s]A [t]B 和某处的[s]B [t]C 调换位置，或

[s]A [t]B 和某处的[s]C [t]A 调换位置， 那么有一个不同处被解决了，即海明距离小了2个，最小为k-1.

那么，该怎么写呢?

由于字符的不同处可多可少，感觉如果全都消耗时间空间比较浪费所以我使用的是map，有的就放进来，读的过程中还可以同时把海明距离数出来，这是O(n)。

用map找由于其本质是红黑树，所以是在O(lgn)的时间内寻找到，综合来看最坏情况也是nlgn，可行。

### Code：



```cpp
#include <map>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef pair<char,char> pcc;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

map <pcc,int> mpi;
map <char,int> mci,mcii;

int main()
{
	int flag=0,x=-1,y=-1;
	int n=0,cnt=0;	cin>>n;
	string s,t;	cin>>s>>t;
	mpi.clear(); mci.clear(); mcii.clear();
	for(int i=0;i<n;i++)
	{
		if(s[i]!=t[i])
		{
			cnt++;
			if(flag<2 && mpi[make_pair(t[i],s[i])]!=0) flag=2,x=mpi[make_pair(t[i],s[i])],y=i+1;
			mpi[make_pair(s[i],t[i])]=i+1;
			if(flag<1 && mci[s[i]]!=0) flag=1,x=mci[s[i]],y=i+1;
			mci[t[i]]=i+1;
			//why forget a half ,my god...
			if(flag<1 && mcii[t[i]]!=0) flag=1,x=mcii[t[i]],y=i+1;
			mcii[s[i]]=i+1;
		}
	}
	cout<<cnt-flag<<endl<<x<<' '<<y<<endl;
	return 0;
}
```







Leonid wants to become a glass carver (the person who creates beautiful artworks by cutting the glass). He already has a rectangular *w* mm  × *h* mm
 sheet of glass, a diamond glass cutter and lots of enthusiasm. What he lacks is understanding of what to carve and how.


In order not to waste time, he decided to practice the technique of carving. To do this, he makes vertical and horizontal cuts through the entire sheet. This process results in making smaller rectangular fragments of glass. Leonid does not move the newly made
 glass fragments. In particular, a cut divides each fragment of glass that it goes through into smaller fragments.


After each cut Leonid tries to determine what area the largest of the currently available glass fragments has. Since there appear more and more fragments, this question takes him more and more time and distracts him from the fascinating process.


Leonid offers to divide the labor — he will cut glass, and you will calculate the area of the maximum fragment after each cut. Do you agree?




Input


The first line contains three integers *w*, *h*, *n* (2 ≤ *w*, *h* ≤ 200 000, 1 ≤ *n* ≤ 200 000).


Next *n* lines contain the descriptions of the cuts. Each description has the form *H y* or *V x*.
 In the first case Leonid makes the horizontal cut at the distance *y* millimeters (1 ≤ *y* ≤ *h* - 1)
 from the lower edge of the original sheet of glass. In the second case Leonid makes a vertical cut at distance *x* (1 ≤ *x* ≤ *w* - 1)
 millimeters from the left edge of the original sheet of glass. It is guaranteed that Leonid won't make two identical cuts.




Output


After each cut print on a single line the area of the maximum available glass fragment in mm2.




Sample test(s)




input
4 3 4
H 2
V 2
V 3
V 1




output
8
4
4
2




input
7 6 5
H 4
V 3
V 5
H 2
V 1




output
28
16
12
6
4






Note


Picture for the first sample test:
![](http://espresso.codeforces.com/76f76c4b91db5d63d733cd37c91bce09001259b7.png)
Picture for the second sample test:
![](http://espresso.codeforces.com/70c98df3646740cda1a5e6455b065203e1953191.png)



有一块大大的玻璃~~~

每次横着或者竖着砍一刀，砍完不要拿走，下一道也得砍到这条线所在的所有部件。

每次砍完之后输出当前碎片中最大面积的部件的面积。

首先要知道的是，千万别一个一个部件的比较面积大小哦，因为每刀都是垂直或者水平的，所以每部分的面积都等于所对应的长边线段和短边线段的积。

即：我们要求的其实只是长边上的最长线段和短边上的最长线段，输出它们的积。

这题的标签写的是《数据结构》，于是想到了set / multiset 的数据结构，感觉很适合这道题，因为其insert() , find()和erase()函数的存在，对这道题而言。

嘛，也顺带用两个例程说明下这三个函数的使用方法吧~ 毕竟set感觉并不是那么太常见（灵活使用的大大们请无视这句^_^）



```cpp
// erasing from multiset
#include <iostream>
#include <set>

int main ()
{
  std::multiset<int> mymultiset;
  std::multiset<int>::iterator it;

  // insert some values:
  mymultiset.insert (40);                            // 40
  for (int i=1; i<7; i++) mymultiset.insert(i*10);   // 10 20 30 40 40 50 60

  it=mymultiset.begin();
  it++;                                              //    ^

  mymultiset.erase (it);                             // 10 30 40 40 50 60

  mymultiset.erase (40);                             // 10 30 50 60

  it=mymultiset.find (50);
  mymultiset.erase ( it, mymultiset.end() );         // 10 30

  std::cout << "mymultiset contains:";
  for (it=mymultiset.begin(); it!=mymultiset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```


```cpp
// set::find
#include <iostream>
#include <set>

int main ()
{
  std::set<int> myset;
  std::set<int>::iterator it;

  // set some initial values:
  for (int i=1; i<=5; i++) myset.insert(i*10);    // set: 10 20 30 40 50

  it=myset.find(20);
  myset.erase (it);
  myset.erase (myset.find(40));

  std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```


```cpp
// set::insert (C++98)
#include <iostream>
#include <set>

int main ()
{
  std::set<int> myset;
  std::set<int>::iterator it;
  std::pair<std::set<int>::iterator,bool> ret;

  // set some initial values:
  for (int i=1; i<=5; ++i) myset.insert(i*10);    // set: 10 20 30 40 50

  ret = myset.insert(20);               // no new element inserted

  if (ret.second==false) it=ret.first;  // "it" now points to element 20

  myset.insert (it,25);                 // max efficiency inserting
  myset.insert (it,24);                 // max efficiency inserting
  myset.insert (it,26);                 // no max efficiency inserting

  int myints[]= {5,10,15};              // 10 already in set, not inserted
  myset.insert (myints,myints+3);

  std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```


说完了函数的使用方法，嘛，就来看看这题的解题Code吧

### Code：



```cpp
#include <set>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
#define i insert

char ch;
int n,w,h,t;
set<int> x,y,*s;
set<int>::iterator l,r;
multiset<int> lx,ly,*ls;

int main()
{
	cin>>w>>h>>n; x.i(0);x.i(w);y.i(0);y.i(h);lx.i(w);ly.i(h);
    for (int j=1; j<=n; ++j)
	{
		cin>>ch>>t;
        if (ch=='H') 
		{s=&y; ls=&ly;} 
		else 
		{s=&x; ls=&lx;}
		s->i(t); l=r=s->find(t); l--; r++; 
		ls->erase(ls->find(*r-*l));
		ls->i(t-*l);
		ls->i(*r-t);
		cout<<(ll)*lx.rbegin() * (ll)*ly.rbegin()<<endl;
	}
	return 0;
}
```

















