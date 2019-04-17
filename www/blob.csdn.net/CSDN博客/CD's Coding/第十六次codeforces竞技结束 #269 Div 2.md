# 第十六次codeforces竞技结束 #269 Div 2 - CD's Coding - CSDN博客





2014年09月29日 16:33:24[糖果天王](https://me.csdn.net/okcd00)阅读数：571








这一场心不够静……

寝室内有杂音而且有持续不停的对话的话就无法集中精神……连续80分钟都没有滤清思路，等到寝室内安静下来，15分钟就完成了BC……

不过，时间还是浪费的太多……








Two polar bears Menshykov and Uslada from the St.Petersburg zoo and elephant Horace from the Kiev zoo got six sticks to play with and assess the animals' creativity. Menshykov, Uslada and Horace decided to make either an elephant or a bear from those sticks.
 They can make an animal from sticks in the following way:
- 
Four sticks represent the animal's legs, these sticks should have the same length.
- 
Two remaining sticks represent the animal's head and body. The bear's head stick must be shorter than the body stick. The elephant, however, has a long trunk, so his head stick must be as long as the body stick. Note that there are no limits on the relations
 between the leg sticks and the head and body sticks.


Your task is to find out which animal can be made from the given stick set. The zoo keeper wants the sticks back after the game, so they must never be broken, even bears understand it.




Input


The single line contains six space-separated integers *l**i* (1 ≤ *l**i* ≤ 9)
 — the lengths of the six sticks. It is guaranteed that the input is such that you cannot make both animals from the sticks.




Output


If you can make a bear from the given set, print string "Bear" (without the quotes). If you can make an elephant, print string "Elephant"
 (wıthout the quotes). If you can make neither a bear nor an elephant, print string "Alien" (without the quotes).




Sample test(s)




input
4 2 5 4 4 4




output
Bear



input
4 4 5 4 4 5




output
Elephant



input
1 2 3 4 5 6




output
Alien





Note


If you're out of creative ideas, see instructions below which show how to make a bear and an elephant in the first two samples. The stick of length 2 is in red, the sticks of length 4 are in green, the sticks of length 5 are in blue.
![](http://espresso.codeforces.com/bf3fd44cb7b1e2a01f2a813a4fef975b2ac06c43.png)




这道题问的是六根棍子是否可以拼成熊或者象，不能的话输出Alien

首先，为了证明它不是外星人，首先需要知道有没有4个（或以上，这个很重要，因为5、6也是可以拼的~）一样长度的棍子，然后判断剩下两根的长短即可。

我用的是排序后枚举三种情况的形式，为了方便确认4根腿以外的两根位置具体在哪里

### Code：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b){return a > b;}


int main()
{
	int l[6]; 
	for(int i=0;i<6;i++)
	{
		scanf("%d",&l[i]);
	}
	sort(l,l+6);
	if(count(l,l+6,l[0])>=4)
	{
		if(l[4]==l[5]) cout<<"Elephant"<<endl;
		if(l[4]< l[5]) cout<<"Bear"<<endl;
	}
	else if(count(l,l+6,l[1])>=4)
	{
		cout<<"Bear"<<endl;
	}
	else if(count(l,l+6,l[2])>=4)
	{
		if(l[0]==l[1]) cout<<"Elephant"<<endl;
		if(l[0]< l[1]) cout<<"Bear"<<endl;
	}
	else cout<<"Alien"<<endl;
	return 0;
}
```













It's time polar bears Menshykov and Uslada from the zoo of St. Petersburg and elephant Horace from the zoo of Kiev got down to business. In total, there are *n* tasks
 for the day and each animal should do each of these tasks. For each task, they have evaluated its difficulty. Also animals decided to do the tasks in order of their difficulty. Unfortunately, some tasks can have the same difficulty, so the order in which one
 can perform the tasks may vary.


Menshykov, Uslada and Horace ask you to deal with this nuisance and come up with individual plans for each of them. The plan is a sequence describing the order in which an animal should do all the *n* tasks.
 Besides, each of them wants to have its own unique plan. Therefore three plans must form three different sequences. You are to find the required plans, or otherwise deliver the sad news to them by stating that it is impossible to come up with three distinct
 plans for the given tasks.




Input


The first line contains integer *n* (1 ≤ *n* ≤ 2000)
 — the number of tasks. The second line contains *n* integers *h*1, *h*2, ..., *h**n*(1 ≤ *h**i* ≤ 2000),
 where *h**i* is
 the difficulty of the *i*-th task. The larger number *h**i* is,
 the more difficult the *i*-th task is.




Output


In the first line print "YES" (without the quotes), if it is possible to come up with three distinct plans of doing the tasks. Otherwise print in the first line
 "NO" (without the quotes). If three desired plans do exist, print in the second line *n* distinct
 integers that represent the numbers of the tasks in the order they are done according to the first plan. In the third and fourth line print two remaining plans in the same form.


If there are multiple possible answers, you can print any of them.




Sample test(s)




input
4
1 3 3 1




output
YES
1 4 2 3 
4 1 2 3 
4 1 3 2 




input
5
2 4 1 4 8




output
NO





Note


In the first sample the difficulty of the tasks sets one limit: tasks 1 and 4 must be done before tasks 2 and 3. That gives the total of four possible sequences of doing tasks : [1, 4, 2, 3], [4, 1, 2, 3], [1, 4, 3, 2], [4, 1, 3, 2]. You can print any three
 of them in the answer.


In the second sample there are only two sequences of tasks that meet the conditions — [3, 1, 2, 4, 5] and [3, 1, 4, 2, 5]. Consequently, it is impossible to make three distinct sequences of tasks.










有这么多件事情要做，他们的难度已经给出，大家都是从简单的开始做到难的，问是否有三种不同的完成顺序。



这不是很明显嘛~ 如果有2组【难度一致的两件事】或1组【难度一致的三件事】就可以啦~

前者的话是第一组换位置即为第二种，第二组换位置即为第三种；后者的话是012/120/201的顺序咯~

额，理念知道了大家自己写起来就快了，我下面这个是当时脑袋昏昏的写的，可乱了还有好多注释，这个理解了还是自己写的好哦^_^

### Code：



```cpp
#include <cmath> 
#include <queue>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
typedef pair<int,int> pii;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int har[2048];
int ans[2048][3];
int tri[3]={0};
int dou[3][3]={0};
priority_queue<pii> p_q;
/*
void swap2(int a,int b,int mrk)
{
	ans[a-1][mrk]=ans[b-1][0];
	ans[b-1][mrk]=ans[a-1][0];
}

void swap3(int a,int b,int c)
{
	int t1=ans[a][0],t2=ans[b][0],t3=ans[c][0];
	ans[a-1][1]=t1;ans[a-1][2]=t2;ans[a-1][0]=t3;
	ans[a-1][2]=t1;ans[a-1][0]=t2;ans[a-1][1]=t3;
}
*/
int main()
{
	int n,flag=1,pos=0;	scanf("%d",&n);
	memset(har,0,sizeof har);
	int cnt=0;
	for(int i=1;i<=n;i++)
	{
		int hard;
		scanf("%d",&hard);
		if(flag>=0)
		{
			if(har[hard]==0) 
			{
				har[hard]=i;
				//cout<<"har["<<hard<<"]="<<i<<endl;
			}
			else if(har[hard]>0)
			{
				dou[pos][0]=har[hard];
				dou[pos][1]=i;
				har[hard]=-1;
				//cout<<dou[pos][0]<<":"<<dou[pos][1]<<endl;
				pos++;
				//cout<<"har["<<hard<<"]="<<-1<<endl;
				//cout<<"pos:"<<pos<<endl;
				if(pos>=2) flag=-2;
			}
			else if(har[hard]==-1)
			{
				tri[0]=dou[0][0];
				tri[1]=dou[0][1];
				tri[2]=i;
				flag=-3;
			}
		}
		p_q.push(pii(0-hard,i));
		//cout<<"input:"<<hard<<": "<<i<<" flag:"<<flag<<" pos:"<<pos<<endl;
	}
	if(flag>=0)cout<<"NO"<<endl;
	else 
	{
		int posi=0;
		while(!p_q.empty())
		{
			pii tt=p_q.top();
			ans[posi][0]=ans[posi][1]=ans[posi][2]=tt.second;
			p_q.pop();
			//cout<<ans[posi][0]<<endl;
			posi++;
		}
		/*
		if(flag==-2)	
		{
			swap2(dou[0][0],dou[0][1],1);
			swap2(dou[1][0],dou[1][1],2);
		}
		else if(flag==-3)
		{
			swap3(tri[0],tri[1],tri[2]);
		}
		*/
		cout<<"YES"<<endl;
		//1st
		for(int j=0;j<n-1;j++)
			printf("%d ",ans[j][0]);
		printf("%d\n",ans[n-1][0]);
		//2nd
		if(flag==-2)
		{
			for(int j=0;j<n-1;j++)
			{
					 if(ans[j][1]==dou[0][0])printf("%d ",dou[0][1]);
				else if(ans[j][1]==dou[0][1])printf("%d ",dou[0][0]);
				else printf("%d ",ans[j][1]);
			}
			if(ans[n-1][1]==dou[0][0])printf("%d\n",dou[0][1]);
			else if(ans[n-1][1]==dou[0][1])printf("%d\n",dou[0][0]);
			else printf("%d\n",ans[n-1][1]);
		}
		else if(flag==-3)
		{
			for(int j=0;j<n-1;j++)
			{
					 if(ans[j][1]==tri[0])printf("%d ",tri[1]);
				else if(ans[j][1]==tri[1])printf("%d ",tri[2]);
				else if(ans[j][1]==tri[2])printf("%d ",tri[0]);
				else printf("%d ",ans[j][1]);
			}
			if(ans[n-1][1]==tri[0])printf("%d\n",tri[1]);
			else if(ans[n-1][1]==tri[1])printf("%d\n",tri[2]);
			else if(ans[n-1][1]==tri[2])printf("%d\n",tri[0]);
			else printf("%d\n",ans[n-1][1]);
		}
		//3rd
		if(flag==-2)
		{
			for(int j=0;j<n-1;j++)
			{
					 if(ans[j][1]==dou[1][0])printf("%d ",dou[1][1]);
				else if(ans[j][1]==dou[1][1])printf("%d ",dou[1][0]);
				else printf("%d ",ans[j][1]);
			}
			if(ans[n-1][1]==dou[1][0])printf("%d\n",dou[1][1]);
			else if(ans[n-1][1]==dou[1][1])printf("%d\n",dou[1][0]);
			else printf("%d\n",ans[n-1][1]);
		}
		else if(flag==-3)
		{
			for(int j=0;j<n-1;j++)
			{
					 if(ans[j][1]==tri[0])printf("%d ",tri[2]);
				else if(ans[j][1]==tri[1])printf("%d ",tri[0]);
				else if(ans[j][1]==tri[2])printf("%d ",tri[1]);
				else printf("%d ",ans[j][1]);
			}
			if(ans[n-1][1]==tri[0])printf("%d\n",tri[2]);
			else if(ans[n-1][1]==tri[1])printf("%d\n",tri[0]);
			else if(ans[n-1][1]==tri[2])printf("%d\n",tri[1]);
			else printf("%d\n",ans[n-1][1]);
		}
	}
	return 0;
}
```




Polar bears Menshykov and Uslada from the zoo of St. Petersburg and elephant Horace from the zoo of Kiev decided to build a house of cards. For that they've already found a hefty deck of *n* playing
 cards. Let's describe the house they want to make:
- 
The house consists of some non-zero number of floors.
- 
Each floor consists of a non-zero number of rooms and the ceiling. A room is two cards that are leaned towards each other. The rooms are made in a row, each two adjoining rooms share a ceiling made by another card.
- 
Each floor besides for the lowest one should contain less rooms than the floor below.


Please note that the house may end by the floor with more than one room, and in this case they also must be covered by the ceiling. Also, the number of rooms on the adjoining floors doesn't have to differ by one, the difference may be more.


While bears are practicing to put cards, Horace tries to figure out how many floors their house should consist of. The height of the house is the number of floors in it. It is possible that you can make a lot of different houses of different heights out of*n* cards.
 It seems that the elephant cannot solve this problem and he asks you to count the number of the distinct heights of the houses that they can make using exactly*n* cards.




Input


The single line contains integer *n* (1 ≤ *n* ≤ 1012)
 — the number of cards.




Output


Print the number of distinct heights that the houses made of exactly *n* cards can have.




Sample test(s)




input
13




output
1



input
6




output
0





Note


In the first sample you can build only these two houses (remember, you must use all the cards):
![](http://espresso.codeforces.com/8b2a119f94863341925432da15e0bfc1669fd95c.png)

Thus, 13 cards are enough only for two floor houses, so the answer is 1.


The six cards in the second sample are not enough to build any house.





问现在手头的卡牌能搭出多少种不同的层高。

我们先发掘一下隐藏条件：

1）每层的卡牌数应该是2+3*n

2）对于那个n，每层的n应该比它脑袋上那层的n大，比脚底下的那个要少

3）一个n层的塔所需要最少的卡牌数为2*n*(0+n-1)*n/2

只要把每层的两张搭着，剩下的除以三没有余数且可以从0-层数-1用完那就是可以了

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
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int main()
{
	ll c=1;
	ll n;	cin>>n;
	ll var=0;
	while(1)
	{
		ll rest=n-c*2,md=(n-c*2)%3;
		if( rest<0 || rest/3<(c-1)*c/2 )break;
		else if( rest/3==(n-1)*n/2) {var++;break;}
		else if( md!=0 ) c++;
		else var++,c++;
		//cout<<c<<endl;
	}
	cout<<var<<endl;
	return 0;
}
```










