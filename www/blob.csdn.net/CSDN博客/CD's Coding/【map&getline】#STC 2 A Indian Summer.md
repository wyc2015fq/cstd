# 【map&getline】#STC 2 A. Indian Summer - CD's Coding - CSDN博客





2014年07月28日 23:41:16[糖果天王](https://me.csdn.net/okcd00)阅读数：605

















Indian summer is such a beautiful time of the year! A girl named Alyona is walking in the forest and picking a bouquet from fallen leaves. Alyona is very choosy — she doesn't take a leaf if it matches the color and the species of the tree of one of the leaves
 she already has. Find out how many leaves Alyona has picked.




Input


The first line contains an integer *n* (1 ≤ *n* ≤ 100)
 — the number of leaves Alyona has found. The next *n*lines contain the leaves' descriptions. Each leaf is characterized by the species of the tree it has
 fallen from and by the color. The species of the trees and colors are given in names, consisting of no more than 10lowercase Latin letters. A name can not be an
 empty string. The species of a tree and the color are given in each line separated by a space.




Output


Output the single number — the number of Alyona's leaves.




Sample test(s)




input
5
birch yellow
maple red
birch yellow
maple yellow
maple green




output
4




input
3
oak yellow
oak yellow
oak yellow




output
1















这道题和[#42](http://blog.csdn.net/okcd00/article/details/38235321)一样用map<string,int>就会很方便呢~但是这里很狡猾，给你的东西里有空格分开，那么怎么办呢？~

1） string相加，直接加起来是字符串合并的效果

2）getline：getline(cin,str); //cin:输入流;str:string对象getline函数从输入流的下一行读取，并保存读取的内容到str中，但不包括换行符

接下来就是一样的map偷懒Code：



```cpp
#include <map> 
#include <queue>
#include <cmath> 
#include <memory> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
map<string,int> m;

int main()
{
	int n;	cin>>n;
	char c; scanf("%c",&c);
	for(int i=0;i<n;i++)
	{
		string str;
		getline(cin,str);
		m[str]=1;
	}
	int cnt=0;
	for(map<string,int>::iterator it=m.begin();it!=m.end();++it)
	{
		//cout<< it->first<<endl;
		cnt++;
	}
	cout<<cnt<<endl;
	return 0;
}
```











