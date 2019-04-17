# 第二十九次codeforces竞技结束 #293 Div 2 - CD's Coding - CSDN博客





2015年03月04日 14:05:45[糖果天王](https://me.csdn.net/okcd00)阅读数：773











Problems

![](http://st.codeforces.com/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.com/contest/518/problem/A)|[Vitaly and Strings](http://codeforces.com/contest/518/problem/A)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x2731|
|[B](http://codeforces.com/contest/518/problem/B)|[Tanya and Postcard](http://codeforces.com/contest/518/problem/B)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x2749|
|[C](http://codeforces.com/contest/518/problem/C)|[Anya and Smartphone](http://codeforces.com/contest/518/problem/C)standard input/output1 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x2299|
|[D](http://codeforces.com/contest/518/problem/D)|[Ilya and Escalator](http://codeforces.com/contest/518/problem/D)standard input/output2 s, 256 MB|![Submit](http://st.codeforces.com/images/icons/submit-22x22.png)![Add to favourites](http://st.codeforces.com/images/icons/star_gray_16.png)|![](http://st.codeforces.com/images/icons/user.png) x1453|





向来赛完不写解题报告就会时运Down，以后不敢了Q^Q

这场比赛是相对较为简单，容易想到思路，适宜冲紫名的一场，但可惜Pretest数据可能弱了些让大家过的太爽了于是FST就多了起来反而掉分现象普及。

那么，一个个来看看吧








Vitaly is a diligent student who never missed a lesson in his five years of studying in the university. He always does his homework on time and passes his exams in time.


During the last lesson the teacher has provided two strings *s* and *t* to
 Vitaly. The strings have the same length, they consist of lowercase English letters, string *s* is lexicographically smaller than string *t*.
 Vitaly wondered if there is such string that is lexicographically larger than string *s* and at the same is lexicographically smaller than string *t*.
 This string should also consist of lowercase English letters and have the length equal to the lengths of strings *s* and *t*.


Let's help Vitaly solve this easy problem!




Input


The first line contains string *s* (1 ≤ |*s*| ≤ 100),
 consisting of lowercase English letters. Here, |*s*| denotes the length of the string.


The second line contains string *t* (|*t*| = |*s*|),
 consisting of lowercase English letters.


It is guaranteed that the lengths of strings *s* and *t* are
 the same and string *s* is lexicographically less than string *t*.




Output


If the string that meets the given requirements doesn't exist, print a single string "No such string" (without the quotes).


If such string exists, print it. If there are multiple valid strings, you may print any of them.




Sample test(s)




input
a
c




output
b




input
aaa
zzz




output
kkk




input
abcdefg
abcdefh




output
No such string






Note


String *s* = *s*1*s*2... *s**n* is
 said to be lexicographically smaller than *t* = *t*1*t*2... *t**n*,
 if there exists such *i*, that *s*1 = *t*1, *s*2 = *t*2, ... *s**i* - 1 = *t**i* - 1, *s**i* < *t**i*.





简单来说，给了两个字符串，问他们之间存在字典序夹在二者之间的字符串嘛？有的话随便输出一个，没有的话输出“No such string”，题目中已经告知了s一定字典序小于t，那么s的最后一位加一看看是不是和t一样不就行了嘛？一样就是不存在，不一样就输出咯？

嘿嘿，有坑哦~ 如果末位是z怎么办，如果末位是两个、三个……n个z怎么办呢？这不是数字可以9变成0然后进位哦~ 对了，我们自己用while写一个类似进位的操作不久好了嘛？

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
	string s,t; cin>>s>>t;
	int l=s.length()-1;
	while(s[l]=='z')
	{
		s[l]='a';
		l--;
	}
	s[l]=s[l]+1;
	if(s==t) cout<<"No such string";
	else cout<<s;
	return 0;
}
```









Little Tanya decided to present her dad a postcard on his Birthday. She has already created a message — string *s* of length *n*,
 consisting of uppercase and lowercase English letters. Tanya can't write yet, so she found a newspaper and decided to cut out the letters and glue them into the postcard to achieve string *s*.
 The newspaper contains string *t*, consisting of uppercase and lowercase English letters. We know that the length of string *t* greater
 or equal to the length of the string *s*.


The newspaper may possibly have too few of some letters needed to make the text and too many of some other letters. That's why Tanya wants to cut some *n* letters
 out of the newspaper and make a message of length exactly *n*, so that it looked as much as possible like *s*.
 If the letter in some position has correct value and correct letter case (in the string *s* and in the string that Tanya will make), then she shouts joyfully
 "YAY!", and if the letter in the given position has only the correct value but it is in the wrong case, then the girl says "WHOOPS".


Tanya wants to make such message that lets her shout "YAY!" as much as possible. If there are multiple ways to do this, then her second priority is to maximize
 the number of times she says "WHOOPS". Your task is to help Tanya make the message.




Input


The first line contains line *s* (1 ≤ |*s*| ≤ 2·105),
 consisting of uppercase and lowercase English letters — the text of Tanya's message.


The second line contains line *t* (|*s*| ≤ |*t*| ≤ 2·105),
 consisting of uppercase and lowercase English letters — the text written in the newspaper.


Here |*a*| means the length of the string *a*.




Output


Print two integers separated by a space:
- 
the first number is the number of times Tanya shouts "YAY!" while making the message,
- 
the second number is the number of times Tanya says "WHOOPS" while making the message.




Sample test(s)




input
AbC
DCbA




output
3 0




input
ABC
abc




output
0 3




input
abacaba
AbaCaBA




output
3 4












说有一个小盆友他在报纸上剪下字来拼一个明信片，如果和自己想的字符一样而且大小写也一样了她就说“YAY”，如果字符一样大小写不一样她就说“Whoops”



要求YAY最多的情况中Whoops最多时的两者数量。

简单的说，给定两个字符串，问在第二个字符串中有多少个a中的严格区分大小写字符，排除掉这些字符后不严格区分大小写的有多少个。

因为字符串不长，可以暴力枚举。

先读一遍s得知需要哪些东西（这里想想我当时为啥hash呢，map简直轻松愉快呀，读者可以试试使用map<char,int> mp，然后mp[a]++这样的操作，会比起数组hash来惬意的多），然后在t中找，严格区分大小写的数完记得减掉，然后再找一次不区分大小写的，输出两个数字即可。

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

int cntl[26]={0},cntu[26]={0};	//cnt
int nedl[26]={0},nedu[26]={0};	//need

int main()
{
	int yay=0,whoops=0;
	string s,t; cin>>s>>t;
	for(int i=0;i<s.length();i++)
	{
		if(isupper(s[i])) nedu[s[i]-'A']++;
		else nedl[s[i]-'a']++;
	}
	for(int i=0;i<t.length();i++)
	{
		if(isupper(t[i])) cntu[t[i]-'A']++;
		else cntl[t[i]-'a']++;
	}
	for(int i=0;i<26;i++)
	{
		if(nedl[i]!=0 && cntl[i]!=0)
		{
			int p=min(nedl[i],cntl[i]);
			nedl[i]-=p,cntl[i]-=p,yay+=p;
		}
		if(nedu[i]!=0 && cntu[i]!=0)
		{
			int p=min(nedu[i],cntu[i]);
			nedu[i]-=p,cntu[i]-=p,yay+=p;
		}
	}
	for(int i=0;i<26;i++)
	{
		if(nedl[i]!=0 && cntu[i]!=0)
		{
			int p=min(nedl[i],cntu[i]);
			nedl[i]-=p,cntu[i]-=p,whoops+=p;
		}
		if(nedu[i]!=0 && cntl[i]!=0)
		{
			int p=min(nedu[i],cntl[i]);
			nedu[i]-=p,cntl[i]-=p,whoops+=p;
		}
	}
	cout<<yay<<' '<<whoops<<endl;
	return 0;
}
```




Anya has bought a new smartphone that uses Berdroid operating system. The smartphone menu has exactly *n* applications,
 each application has its own icon. The icons are located on different screens, one screen contains *k* icons. The icons from the first to the *k*-th
 one are located on the first screen, from the (*k* + 1)-th to the 2*k*-th
 ones are on the second screen and so on (the last screen may be partially empty).


Initially the smartphone menu is showing the screen number 1. To launch the application with the icon located on the screen *t*,
 Anya needs to make the following gestures: first she scrolls to the required screen number *t*, by making *t* - 1 gestures
 (if the icon is on the screen *t*), and then make another gesture — press the icon of the required application exactly once to launch it.


After the application is launched, the menu returns to the first screen. That is, to launch the next application you need to scroll through the menu again starting from the screen number 1.


All applications are numbered from 1 to *n*. We know a certain
 order in which the icons of the applications are located in the menu at the beginning, but it changes as long as you use the operating system. Berdroid is intelligent system, so it changes the
 order of the icons by moving the more frequently used icons to the beginning of the list. Formally, right after an application is launched, Berdroid swaps the application icon and the icon of a preceding application (that is, the icon of an application on
 the position that is smaller by one in the order of menu). The preceding icon may possibly be located on the adjacent screen. The only exception is when the icon of the launched application already occupies the first place, in this case the icon arrangement
 doesn't change.


Anya has planned the order in which she will launch applications. How many gestures should Anya make to launch the applications in the planned order?


Note that one application may be launched multiple times.




Input


The first line of the input contains three numbers *n*, *m*, *k* (1 ≤ *n*, *m*, *k* ≤ 105) — the
 number of applications that Anya has on her smartphone, the number of applications that will be launched and the number of icons that are located on the same screen.


The next line contains *n* integers, permutation *a*1, *a*2, ..., *a**n* — the
 initial order of icons from left to right in the menu (from the first to the last one), *a**i* — 
 is the id of the application, whose icon goes *i*-th in the menu. Each integer from 1 to *n* occurs
 exactly once among *a**i*.


The third line contains *m* integers *b*1, *b*2, ..., *b**m*(1 ≤ *b**i* ≤ *n*) — the
 ids of the launched applications in the planned order. One application may be launched multiple times.




Output


Print a single number — the number of gestures that Anya needs to make to launch all the applications in the desired order.




Sample test(s)




input
8 3 3
1 2 3 4 5 6 7 8
7 8 1




output
7




input
5 4 2
3 1 5 2 4
4 4 4 4




output
8






Note


In the first test the initial configuration looks like (123)(456)(78), that is, the first screen contains icons of applications 1, 2, 3,
 the second screen contains icons 4, 5, 6, the third screen contains icons 7, 8.


After application 7 is launched, we get the new arrangement of the icons — (123)(457)(68).
 To launch it Anya makes 3gestures.


After application 8 is launched, we get configuration (123)(457)(86).
 To launch it Anya makes 3 gestures.


After application 1 is launched, the arrangement of icons in the menu doesn't change. To launch it Anya makes 1 gesture.


In total, Anya makes 7 gestures.





说有个智能手机，上面有n个APP，我要点其中的m个，每页最多可以放k个APP。

当我每次点击一个APP的时候，如果这个APP不在第一位，那么他就和他前面的一个APP调换位置，问我们按照顺序点击那m个APP要做多少个手势（滑动到下一页要一个手势，点击是一个手势，每点击一个APP出来都回到第一页）

点击某个APP需要的手势的个数其实就是(pos/k)+(pos%k==0?0:1)，即这个APP当前所在的位置除以每页最多放置的APP数，向上取整，因为我们需要滑动（当前所在页数-1）+点击1次=当前所在页数。

然后就是每次点击要前移一位的实现了：



```cpp
if(pos>1)
		{
			int t=fdnum[pos-1];
			fdnum[pos-1]=now;
			fdpos[now]=pos-1;
			fdnum[pos]=t;
			fdpos[t]=pos;
		}
```


我用的方法是：数组1：每个位置对应当前位置的APP编号，数组2：每个APP编号当前所在的位置，然后借助临时变量t进行swap操作。

当然别忘了他就在第一页的时候不用和前一个调换位置哦。

然后，坑来了——

孩子们永远是那句话……int不是好东西啊，LL大法好啊，动不动int就溢出了烦不烦呢！明明就前300可以紫名了你就是不开心溢出让我FST，叹气……

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

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

map<int,int> fdpos,fdnum; //num->pos & pos->num

int main()
{
	fdpos.clear();
	fdnum.clear();
	long long ans=0; // LL大法好……
	int n,m,k;	cin>>n>>m>>k;
	for(int i=1;i<=n;i++)
	{
		int now;	scanf("%d",&now);
		fdpos[now]=i;
		fdnum[i]=now;
	}
	for(int i=1;i<=m;i++)
	{
		int now;	scanf("%d",&now);
		int pos=fdpos[now];
		ans+=(pos/k)+(pos%k==0?0:1);
		//swap
		if(pos>1)
		{
			int t=fdnum[pos-1];
			fdnum[pos-1]=now;
			fdpos[now]=pos-1;
			fdnum[pos]=t;
			fdpos[t]=pos;
		}
	}
	cout<<ans;
	return 0;
}
```









Ilya got tired of sports programming, left university and got a job in the subway. He was given the task to determine the escalator load factor.


Let's assume that *n* people stand in the queue for the escalator. At each second one of the two following possibilities takes place: either the first person
 in the queue enters the escalator with probability *p*, or the first person in the queue doesn't move with probability (1 - *p*),
 paralyzed by his fear of escalators and making the whole queue wait behind him.


Formally speaking, the *i*-th person in the queue cannot enter the escalator until people with indices from 1 to *i* - 1 inclusive
 enter it. In one second only one person can enter the escalator. The escalator is infinite, so if a person enters it, he never leaves it, that is he will be standing on the escalator at any following second. Ilya needs to count the expected value of the number
 of people standing on the escalator after *t* seconds.


Your task is to help him solve this complicated task.




Input


The first line of the input contains three numbers *n*, *p*, *t* (1 ≤ *n*, *t* ≤ 2000, 0 ≤ *p* ≤ 1).
 Numbers *n* and *t* are integers, number *p*is
 real, given with exactly two digits after the decimal point.




Output


Print a single real number — the expected number of people who will be standing on the escalator after *t* seconds. The absolute or relative error mustn't
 exceed 10 - 6.




Sample test(s)




input
1 0.50 1




output
0.5




input
1 0.50 4




output
0.9375




input
4 0.20 2




output
0.4












没错这就是个DP……



啊对了题意啊题意……

有个无限长的电梯，有n个人一列纵队在排队上电梯，每个人只能在前头都没人了的时候才能上电梯，每秒钟，有p的概率排在第一的人上了电梯，问：t秒后，电梯上人数的数学期望……

数学你好……概率学啦……

用dp[i][j]来表示当i个人排队时在第t秒电梯上人数的数学期望。

那么我们知道dp[i][j]应该等于

当[（i-1个人，j-1秒）时的期望+1] * p （这个人上去了）

加上 当[（i个人，j-1秒）时的期望 * (1-p) （这个人没上去）

具体的看看代码吧

### Code：



```cpp
#include<bits/stdc++.h>
double dp[2005][2005];
int main()
{
	int n,t,i;
	double p;
	scanf("%d%lf%d",&n,&p,&t);
	for(i=1;i<=n;i++)
	{
		for(int j=1;j<=t;j++)
		dp[i][j]=(p*(1+dp[i-1][j-1])+(1-p)*(dp[i][j-1]));
	}
	printf("%.6lf",dp[n][t]);
	return 0;
}
```























