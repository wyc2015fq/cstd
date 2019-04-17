# 【Pair】#15 A. Cottage Village - CD's Coding - CSDN博客





2014年05月04日 01:20:54[糖果天王](https://me.csdn.net/okcd00)阅读数：569
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)













A new cottage village called «Flatville» is being built in Flatland. By now they have already built in «Flatville» *n* square houses with the centres on the *Оx*-axis.
 The houses' sides are parallel to the coordinate axes. It's known that no two houses overlap, but they can touch each other.


The architect bureau, where Peter works, was commissioned to build a new house in «Flatville». The customer wants his future house to be on the *Оx*-axis,
 to be square in shape, have a side *t*, and touch at least one of the already built houses. For sure, its sides should be parallel to the coordinate axes,
 its centre should be on the *Ox*-axis and it shouldn't overlap any of the houses in the village.


Peter was given a list of all the houses in «Flatville». Would you help him find the amount of possible positions of the new house?




Input


The first line of the input data contains numbers *n* and *t* (1 ≤ *n*, *t* ≤ 1000).
 Then there follow *n* lines, each of them contains two space-separated integer numbers: *x**i**a**i*,
 where *x**i* — *x*-coordinate
 of the centre of the *i*-th house, and *a**i* —
 length of its side ( - 1000 ≤ *x**i* ≤ 1000, 1 ≤ *a**i* ≤ 1000).




Output


Output the amount of possible positions of the new house.




Sample test(s)




input
2 2
0 4
6 2




output
4




input
2 2
0 4
5 2




output
3




input
2 3
0 4
5 2




output
2






Note


It is possible for the *x*-coordinate of the new house to have non-integer value.





题意：有个x轴，轴上有n座房子，每座房子的位置是xi，边长是ai，有个人想要贴着其中一座房子建造他自己的一座边长为t的房子，有多少种建法

终于出现一个麻烦些的A题了，全网的C++解法也只有一个，倒是看到不少Python解这道题的……我想试试看用map试试看。

个人的想法是通过定义pair类型，扔进list来存储，然后使用list自带的sort排序后遍历一遍的方法……（我知道这个用数组容易而且是一个意思，但是我只是想学新东西而已……）




先把CE代码贴一下，待日后有人教我怎么用……(为什么DevC++都不认识first和second……)

后记:居然不能用'.'，要用'->'......

List-Pair 终于AC:



```cpp
#include<iostream>  
#include<algorithm>  
#include<list>  
#include<cstdio>  
#include<cstdlib>  
using namespace std;  
  
typedef pair<int,int> house;  
typedef list<house> infov;  
  
bool cmp(const house& p1, const house& p2) {   return p1.first < p2.first;   }  
/* 输出操作符重载：  
inline ostream& operator << (ostream& stream,house& val) 
{ 
    stream <<val.first << val.second ; 
    return stream; 
} 
*/  
int main()  
{  
    int n,cnt=0;  
    int t;
    scanf("%d%d", &n, &t);  
    infov info;  
    for(int i=0;i<n;i++)  
    {  
        int x_tmp, a_tmp;  
        scanf("%d%d", &x_tmp, &a_tmp);  
        info.push_back(make_pair(x_tmp,a_tmp));  
    }  
    info.sort();  
    //info.unique();  如果需要去重的话  
    //int testcnt=0;
    infov::iterator it,itemp;  
    for(it=info.begin();it!=info.end();++it)  
    {  
    	//cout<<it->first<<"\t"<<it->second<<endl;
        if(it!=info.begin())  
            if( (it->first - itemp->first)*2
				< (it->second + itemp->second) +t*2 );//cout<<"n1:"<<testcnt<<endl;  
            else if( (it->first - itemp->first)*2
					==(it->second + itemp->second +t*2 ) )
					{
						//cout<<"n2:"<<testcnt<<endl;  						
						cnt++;
					}
            else if( (it->first - itemp->first)*2
					>(it->second + itemp->second) +t*2 ) 
					{
						//cout<<"n3:"<<testcnt<<endl; 
						cnt+=2; 
					}
        itemp=it;  
        //testcnt++;
    }   
    cout<< cnt+2;  
    return 0;  
}
```





Map-Pair Failed:



```cpp
#include <cstdio>  
#include <iostream>  
#include <algorithm>  
#include <string.h>  
#include <cstdlib>  
#include <string>  
#include <map>  
using namespace std;  

/***********************************************
template <class T1, class T2> struct pair
{
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;
  pair() : first(T1()), second(T2()) {}
  pair(const T1& x, const T2& y) : first(x), second(y) {}
  template <class U, class V>
    pair (const pair<U,V> &p) : first(p.first), second(p.second) { }
}
************************************************/

int cmp(const pair<int,int>& a,const pair<int,int>& b)
{
	return a.first > b.first;
}
int main()  
{
	map<int, int, greater<int> > house;
	int n,t,cnt=0;
	scanf("%d%d",&n,&t);
	int tmp_x,tmp_a;
	for(int i=0;i<n;i++)
	{
		scanf("%d%d",&tmp_x,&tmp_a);
		house.insert(pair<int,int>(tmp_x,tmp_a));
	} 
	sort(house.begin(),house.end(),cmp);
	map<int,int>::iterator it,itemp; 
	for(it=house.begin();it!=house.end();++it)
	{ 
		if(it!=house.begin())  
			if( it->first 
					<(it->second+itemp->second)/2 +itemp->first );
		  		else if( it->first 
				  			==(it->second + itemp->second)/2 + itemp->first) cnt++;  
            		else if( it->first
								>(it->second + itemp->second)/2 +itemp->first)cnt+=2; 
    	itemp=it;
	}
	cout<<cnt+2;
	return 0;	
}
```





以下是[ymrfzr](http://my.csdn.net/ymrfzr)（CSDN）的解法……大意是把房子的左点和右点在x轴上显示出来，然后每次比较一个房子的左点和左房子的右点。



```cpp
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <algorithm>
using namespace std;

int n,t,cn,a[2005]; 
int main()
{
    int T,i,j,k;
    while(~scanf("%d %d",&n,&t))
	{
		for(i=0;i<n;i++)
		{
			scanf("%d %d",&j,&k);
			a[i*2]=j*2+k;
			a[i*2+1]=j*2-k;
		}
	sort(a,a+2*n);
	n*=2;
	t*=2;
	cn=2;
	for(i=1;i<n;i+=2)
	{
		if(a[i+1]-a[i]>t)cn+=2;
		else if(a[i+1]-a[i]==t)cn++;
	}
	printf("%d\n",cn);
	}
    return 0;
}
```




Python:



```python
# input
n,t = map(int , raw_input().split())
dict = {}
for i in range(n):
    x,a = map(int , raw_input().split())
    dict[x] = a

# getAns
ans = 2
list = dict.keys()
list.sort()
pre = -(1<<30)

for key in list:
    a = float(dict[key])/2
    if pre != -(1<<30):
       dis = abs((key-a)-pre)
       if dis > t:
          ans += 2
       elif dis == t:
          ans += 1
    pre = key+a

# output
print ans
```










