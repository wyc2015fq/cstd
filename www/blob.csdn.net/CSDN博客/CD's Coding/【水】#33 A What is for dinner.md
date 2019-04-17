# 【水】#33 A. What is for dinner? - CD's Coding - CSDN博客





2014年07月19日 12:33:54[糖果天王](https://me.csdn.net/okcd00)阅读数：627标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















In one little known, but very beautiful country called Waterland, lives a lovely shark Valerie. Like all the sharks, she has several rows of teeth, and feeds on crucians. One of Valerie's distinguishing features is that while eating one crucian she uses only
 one row of her teeth, the rest of the teeth are "relaxing".


For a long time our heroine had been searching the sea for crucians, but a great misfortune happened. Her teeth started to ache, and she had to see the local dentist, lobster Ashot. As a professional, Ashot quickly relieved Valerie from her toothache. Moreover,
 he managed to determine the cause of Valerie's developing caries (for what he was later nicknamed Cap).


It turned that Valerie eats too many crucians. To help Valerie avoid further reoccurrence of toothache, Ashot found for each Valerie's tooth its residual viability. Residual viability of a tooth is a value equal to the amount of crucians that Valerie can eat
 with this tooth. Every time Valerie eats a crucian, viability of all the teeth used for it will decrease by one. When the viability of at least one tooth becomes negative, the shark will have to see the dentist again.


Unhappy, Valerie came back home, where a portion of crucians was waiting for her. For sure, the shark couldn't say no to her favourite meal, but she had no desire to go back to the dentist. That's why she decided to eat the maximum amount of crucians from the
 portion but so that the viability of no tooth becomes negative.


As Valerie is not good at mathematics, she asked you to help her to find out the total amount of crucians that she can consume for dinner.


We should remind you that while eating one crucian Valerie uses exactly one row of teeth and the viability of each tooth from this row decreases by one.




Input


The first line contains three integers *n*, *m*, *k* (1 ≤ *m* ≤ *n* ≤ 1000, 0 ≤ *k* ≤ 106)
 — total amount of Valerie's teeth, amount of tooth rows and amount of crucians in Valerie's portion for dinner. Then follow *n* lines, each containing two
 integers: *r* (1 ≤ *r* ≤ *m*) — index of the
 row, where belongs the corresponding tooth, and *c* (0 ≤ *c* ≤ 106)
 — its residual viability.


It's guaranteed that each tooth row has positive amount of teeth.




Output


In the first line output the maximum amount of crucians that Valerie can consume for dinner.




Sample test(s)




input
4 3 18
2 3
1 2
3 6
2 3




output
11




input
2 2 13
1 13
2 12




output
13















这道题可能题目比较长比较吓人，但是实际上是可以水的^_^。（我错了我写的就是A题肯定都能水<(＿　＿)>

这道题的意思是鲨鱼有n个牙齿，分别分布于m排上，有k个食物想要吃，但是每个牙齿的剩余活力不能到负数，每吃掉一个就要消耗某一排牙齿的每一颗牙齿各1点活力。

所以其实需求的是Min(k,Sum(min(arr)))的节奏（即每排牙齿中活力最小的活力和与k中较小的那个）




Code:



```cpp
#include <cmath>   
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>
using namespace std;

int row[1001]; 
int main()
{
	memset(row,1000001,sizeof row); 
	int n,m,k,total=0;
	cin>>n>>m>>k;
	for(int i=0;i<n;i++)
	{
		int r=0,c=0;
		cin>>r>>c;
		if(row[r]>c)row[r]=c;
	}
	for(int j=1;j<=m;j++)
	{
		total+=row[j];
	}
	cout<<min(total,k);
	return 0;
}
```





