# 第十二次codeforces竞技结束 #263 Div 2 - CD's Coding - CSDN博客





2014年09月07日 21:03:33[糖果天王](https://me.csdn.net/okcd00)阅读数：656








这场打得比较顺畅，直接涨分到了1666

三道题33分钟全1A……可能短期内都无法超越自己了……

接下来写点题解













Toastman came up with a very easy task. He gives it to Appleman, but Appleman doesn't know how to solve it. Can you help him?


Given a *n* × *n* checkerboard. Each cell of the board has either character 'x',
 or character 'o'. Is it true that each cell of the board has even number of adjacent cells with 'o'?
 Two cells of the board are adjacent if they share a side.




Input


The first line contains an integer *n* (1 ≤ *n* ≤ 100).
 Then *n* lines follow containing the description of the checkerboard. Each of them contains *n* characters
 (either 'x' or 'o') without spaces.




Output


Print "YES" or "NO" (without the quotes) depending on the answer
 to the problem.




Sample test(s)




input
3
xxo
xox
oxx




output
YES




input
4
xxxo
xoxo
oxox
xxxx




output
NO












这道题的意思是，任何一个格子，与他相邻的格子中圆圈的个数都得是偶数，否则输出NO



那么我们读入一边然后遍历每一个格子咯~ 边缘和读入时候的换行符都要注意呢~

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

int main()
{
	int n,flag=0;	cin>>n;
	char c,map[101][101];
	for(int i=0;i<n;i++)
	{
		scanf("%c",&c);//huiche
		for(int j=0;j<n;j++)
		{
			scanf("%c",&map[i][j]);
		}
	} 
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			int cnt=0;
			if(i>0 && map[i-1][j]=='o')cnt++;
			if(i<n-1 && map[i+1][j]=='o')cnt++;
			if(j>0 && map[i][j-1]=='o')cnt++;
			if(j<n-1 && map[i][j+1]=='o')cnt++;
			//cout<<i<<" "<<j<<": "<<cnt<<endl;
			if(cnt%2!=0)flag=1; 
		}
	}
	if(flag)printf("NO");
	else printf("YES");
	return 0;
}
```




Appleman has *n* cards. Each card has an uppercase letter written on it. Toastman must choose *k* cards
 from Appleman's cards. Then Appleman should give Toastman some coins depending on the chosen cards. Formally, for each Toastman's card *i* you should calculate
 how much Toastman's cards have the letter equal to letter on *i*th, then sum up all these quantities, such a number of coins Appleman should give to Toastman.


Given the description of Appleman's cards. What is the maximum number of coins Toastman can get?




Input


The first line contains two integers *n* and *k* (1 ≤ *k* ≤ *n* ≤ 105).
 The next line contains *n* uppercase letters without spaces — the *i*-th
 letter describes the *i*-th card of the Appleman.




Output


Print a single integer – the answer to the problem.




Sample test(s)




input
15 10
DZFDFZDFDDDDDDF




output
82




input
6 4
YJSNPI




output
4






Note


In the first test example Toastman can choose nine cards with letter D and one additional card with any letter. For each card with D he
 will get 9 coins and for the additional card he will get 1 coin.





这道题的意思是，我有n张牌，需要从里面选取k张，每张牌的点数是我选的这种牌的张数，如ABBCCC=1*1+2*2+3*3，我们知道，当和一定时，最大的那个数越大，则平方和越大，所以我们先排序，然后从多的往少的拿就好

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
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
using namespace std;
typedef long long ll;

int amt[27];
bool cmp(const int a, const int b){return a > b;}

int main()
{
	char c;
	int n,k;	cin>>n>>k;
	scanf("%c",&c);	//huiche
	memset(amt,0,sizeof amt);
	
	for(int i=1;i<=n;i++)
	{
		scanf("%c",&c);
		amt[(c-'A')]++;
	}
	sort(amt,amt+26,cmp);
	//for(int i=0;i<26;i++)cout<<amt[i]<<endl;
	ll ans=0;
	for(int i=0;i<26;i++)
	{
		if(k>=amt[i]) 
		{
			ans+=(ll)amt[i]*(ll)amt[i];
			k-=amt[i];
		}
		else
		{
			ans+=(ll)k*(ll)k;
			k=0;
		}
		
		if(k==0)
		{
			printf("%I64d",ans);
			return 0;
		}
	}
	return 0;
}
```




Appleman and Toastman play a game. Initially Appleman gives one group of *n* numbers to the Toastman, then they start to complete the following tasks:
- 
Each time Toastman gets a group of numbers, he sums up all the numbers and adds this sum to the score. Then he gives the group to the Appleman.
- 
Each time Appleman gets a group consisting of a single number, he throws this group out. Each time Appleman gets a group consisting of more than one number, he splits the group into two non-empty groups (he can do it in any way) and gives each of them to Toastman.


After guys complete all the tasks they look at the score value. What is the maximum possible value of score they can get?




Input


The first line contains a single integer *n* (1 ≤ *n* ≤ 3·105).
 The second line contains *n* integers *a*1, *a*2,
 ..., *a**n* (1 ≤ *a**i* ≤ 106)
 — the initial group that is given to Toastman.




Output


Print a single integer — the largest possible score.




Sample test(s)




input
3
3 1 5




output
26




input
1
10




output
10






Note


Consider the following situation in the first example. Initially Toastman gets group [3, 1, 5] and adds 9 to the score, then he give the group to Appleman. Appleman splits group [3, 1, 5] into two groups: [3, 5] and [1]. Both of them should be given to Toastman.
 When Toastman receives group [1], he adds 1 to score and gives the group to Appleman (he will throw it out). When Toastman receives group [3, 5], he adds 8 to the score and gives the group to Appleman. Appleman splits [3, 5] in the only possible way: [5] and
 [3]. Then he gives both groups to Toastman. When Toastman receives [5], he adds 5 to the score and gives the group to Appleman (he will throws it out). When Toastman receives [3], he adds 3 to the score and gives the group to Appleman (he will throws it out).
 Finally Toastman have added 9 + 1 + 8 + 5 + 3 = 26 to the score. This is the optimal sequence of actions.





这道题和刚才的B题有着异曲同工之妙

这道题的意思是，有两个人AB，A负责把所有只有一个数字的集合扔掉，同时把多于一个数字的集合分成两个集合扔给B，B负责把获得的集合里的数字们都加起来然后还给A。问获得的数字和最大是多少。

我们可以模拟一下，有n个数字的话，我们不管怎么分，数字们被重复计算的次数是和n有关的一个式子：

从大到小分别是n,n,n-1,n-2......3,2次，所以，自然是把大的数字放在前面啦~

### Code:



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
using namespace std;
typedef long long ll;
ll ans=0;

int num[300033];
bool cmp(const int a, const int b){return a > b;}

int main()
{
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		scanf("%d",&num[i]);
		ans+=num[i];
	}
	sort(num,num+n,cmp);
	ans-=num[0];
	for(int i=0;i<n;i++)
	{
		ans+=(ll)num[i]*(ll)(n-i);
	}
	printf("%I64d",ans);
	return 0;
}
```

















