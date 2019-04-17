# 【简单演算】#86 A. Cifera - CD's Coding - CSDN博客





2014年12月15日 21:56:10[糖果天王](https://me.csdn.net/okcd00)阅读数：622标签：[Codeforces																[Beta Round #86																[Div. 2 Only																[Cifera](https://so.csdn.net/so/search/s.do?q=Cifera&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=Div. 2 Only&t=blog)













When Petya went to school, he got interested in large numbers and what they were called in ancient times. For instance, he learned that the Russian word "tma" (which now means "too much to be counted") used to stand for a thousand and "tma tmyschaya" (which
 literally means "the tma of tmas") used to stand for a million.


Petya wanted to modernize the words we use for numbers and invented a word petricium that represents number *k*.
 Moreover,petricium la petricium stands for number *k*2, petricium
 la petricium la petricium stands for *k*3 and
 so on. All numbers of this form are called petriciumus cifera, and the number's importance is the number of articles la in
 its title.


Petya's invention brought on a challenge that needed to be solved quickly: does some number *l* belong to the set petriciumus
 cifera? As Petya is a very busy schoolboy he needs to automate the process, he asked you to solve it.




Input


The first input line contains integer number *k*, the second line contains integer number *l* (2 ≤ *k*, *l* ≤ 231 - 1).




Output


You should print in the first line of the output "YES", if the number belongs to the set petriciumus
 cifera and otherwise print "NO". If the number belongs to the set, then print on the seconds line the only number — the importance of number *l*.




Sample test(s)




input
5
25




output
YES
1




input
3
8




output
NO












说是在某种语言里，一个数的平方叫做 xxx la xxx，立方是 xxx la xxx la xxx，也就是n次方就是n-1个la，给两个数，问第二个数是第一个数的某个次幂吗?是的话，表示出来有几个la呢？



那我们就 

```cpp
while(tmp<l) { tmp*=k; cnt++; }
```

这样一下~ 然后cnt就能说明一切啦~ 啊需要注意的是，超过范围但是发现不是某次幂要记得判断哦~



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
// http://codeforces.com/contest/114
// Cifera
int main()
{
	int cnt=0;
	ll k,tmp,l;	cin>>k>>l; tmp=k;
	while(tmp<l)
	{
		tmp*=k;
		cnt++;
	} 
	if(tmp==l)cout<<"YES"<<endl<<cnt;
	else cout<<"NO";
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Beta Round #86&t=blog)](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)




