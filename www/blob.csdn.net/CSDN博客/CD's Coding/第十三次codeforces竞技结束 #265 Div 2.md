# 第十三次codeforces竞技结束 #265 Div 2 - CD's Coding - CSDN博客





2014年09月16日 13:47:21[糖果天王](https://me.csdn.net/okcd00)阅读数：593








以后比完赛就要趁早来写解题报告……等得久了之后一旦下一场没打好，写这一场的时候就也没心情了……

这一场是打得较好的一场，Rank达到了1694，离DIv1最近的一次……

但是#266 掉到1593了，因为精神状态太差……




废话不多说先来写一些265的解题报告










Sergey is testing a next-generation processor. Instead of bytes the processor works with memory cells consisting of *n* bits. These bits are numbered from 1 to *n*.
 An integer is stored in the cell in the following way: the least significant bit is stored in the first bit of the cell, the next significant bit is stored in the second bit, and so on; the most significant bit is stored in the *n*-th
 bit.


Now Sergey wants to test the following instruction: "add 1 to the value of the cell". As a result of the instruction, the integer that is written in the cell must
 be increased by one; if some of the most significant bits of the resulting number do not fit into the cell, they must be discarded.


Sergey wrote certain values ​​of the bits in the cell and is going to add one to its value. How many bits of the cell will change after the operation?




Input


The first line contains a single integer *n* (1 ≤ *n* ≤ 100)
 — the number of bits in the cell.


The second line contains a string consisting of *n* characters — the initial state of the cell. The first character denotes the state of the first bit of the
 cell. The second character denotes the second least significant bit and so on. The last character denotes the state of the most significant bit.




Output


Print a single integer — the number of bits in the cell which change their state after we add 1 to the cell.




Sample test(s)




input
4
1100




output
3




input
4
1111




output
4






Note


In the first sample the cell ends up with value 0010, in the second sample — with 0000.










这个题目是给你一个反着写的二进制数，问你加一之后有多少个数位变化了，题意不是很容易理解，但是理解了之后就简单的很呢



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

int val[101],ans[101];

int main()
{
	int n;
	string s;
	cin>>n>>s;
	memset(val,0,sizeof val);
	memset(ans,0,sizeof ans);
	for(int i=0;i<n;i++)
		val[i]=s[i]-'0';
	int flag=0;
	for(int pos=0;pos<n;pos++)
	{
		if(val[pos]==0 && flag==0)
		{
			ans[pos]=1;
			flag=1;
		}	
		else if(val[pos]==1 && flag==0)
		{
			ans[pos]=0;
			flag=0;
		}
		else ans[pos]=val[pos];
	}
	int cnt=0;
	for(int i=0;i<n;i++)
	{
		if(ans[i]!=val[i])cnt++;
		//cout<<ans[i]<<":"<<val[i]<<endl;
	}
	cout<<cnt<<endl;
	return 0;
}
```















Over time, Alexey's mail box got littered with too many letters. Some of them are read, while others are unread.


Alexey's mail program can either show a list of all letters or show the content of a single letter. As soon as the program shows the content of an unread letter, it becomes read letter (if the program shows the content of a read letter nothing happens). In
 one click he can do any of the following operations:
- 
Move from the list of letters to the content of any single letter.
- 
Return to the list of letters from single letter viewing mode.
- 
In single letter viewing mode, move to the next or to the previous letter in the list. You cannot move from the first letter to the previous one or from the last letter to the next one.


The program cannot delete the letters from the list or rearrange them.


Alexey wants to read all the unread letters and go watch football. Now he is viewing the list of all letters and for each letter he can see if it is read or unread. What minimum number of operations does Alexey need to perform to read all unread letters?




Input


The first line contains a single integer *n* (1 ≤ *n* ≤ 1000)
 — the number of letters in the mailbox.


The second line contains *n* space-separated integers (zeros and ones) — the state of the letter list. The *i*-th
 number equals either 1, if the *i*-th number is unread, or 0,
 if the *i*-th letter is read.




Output


Print a single number — the minimum number of operations needed to make all the letters read.




Sample test(s)




input
5
0 1 0 1 0




output
3




input
5
1 1 0 0 1




output
4




input
2
0 0




output
0






Note


In the first sample Alexey needs three operations to cope with the task: open the second letter, move to the third one, move to the fourth one.


In the second sample the action plan: open the first letter, move to the second letter, return to the list, open the fifth letter.


In the third sample all letters are already read.










邮箱里有很多邮件，其中1代表还没看过，0代表看过了的，每次点进去一封邮件、点出来回到邮件列表、在邮件内点击查看下一封都是需要操作一次的。问你最少操作多少次才能把邮件看完（注意是看完哦，看完最后一封不需要点出来的哦）



想想我平时看邮件的习惯（强迫症自信最优解）：点进第一封没读过的邮件，然后看看下一封读过没，读过了的话点出来再点进之后的第一封没读过的，要是没读过的话直接点击下一封去读。

于是就可以直接边读入边操作了，嘛当时保险点还是中规中矩的写的每一个分四种情况（00、01、10、11，0-没读过，1-读过了）

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

int sta[1024];
int dp[1024];

int main()
{
	int n;	cin>>n;
	memset(sta,0,sizeof sta);
	memset(dp,0,sizeof dp);
	for(int i=1;i<=n;i++)
	{
		scanf("%d",&sta[i]);
		if(sta[i]==1 && sta[i-1]==1) dp[i]=dp[i-1]+1;
		else if(sta[i]==1 && sta[i-1]==0) dp[i]=dp[i-1]+1;
		else if(sta[i]==0 && sta[i-1]==1) dp[i]=dp[i-1]+1;
		else if(sta[i]==0 && sta[i-1]==0) dp[i]=dp[i-1];
		if(sta[n]==0 && dp[n]!=0)	dp[n]--;
		//cout<<dp[i]<<" ";
	}
	cout<<dp[n]<<endl;
	
	return 0;
}
```














Paul hates palindromes. He assumes that string *s* is tolerable if
 each its character is one of the first *p* letters of the English alphabet and *s* doesn't
 contain any palindrome contiguous substring of length 2 or more.


Paul has found a tolerable string *s* of length *n*.
 Help him find the lexicographically next tolerable string of the same length or else state that such string does not exist.




Input


The first line contains two space-separated integers: *n* and *p* (1 ≤ *n* ≤ 1000; 1 ≤ *p* ≤ 26).
 The second line contains string *s*, consisting of *n* small
 English letters. It is guaranteed that the string is tolerable (according to the above definition).




Output


If the lexicographically next tolerable string of the same length exists, print it. Otherwise, print "NO" (without the quotes).




Sample test(s)




input
3 3
cba




output
NO




input
3 4
cba




output
cbd




input
4 4
abcd




output
abda






Note


String *s* is lexicographically larger (or simply larger)
 than string *t* with the same length, if there is number *i*,
 such that *s*1 = *t*1,
 ..., *s**i* = *t**i*, *s**i* + 1 > *t**i* + 1.


The lexicographically next tolerable string is the lexicographically minimum tolerable string which is larger than the given one.


A palindrome is a string that reads the same forward or reversed.










说是有一个长度为n的字符串，字符只能在字母表的前p个选取，当前字符串内一定不存在回文序列（一个字符的不算哦），问你它的下一个不存在回文的"能忍的"字符串是什么，如果没有输出NO。

这个就得讲讲咯，我们从末尾开始一次一次加一，遇到超过p限制了就变成a，前一位加一（n进制模拟嘛^_^），如果数字们都用完了就No了，这个其实很容易想到的哦，然后就是判定啦，咱们到底啥算能用啥算不能用呢？

嘿嘿就是这里啦~ 我们进一位的时候你以为当前位变成1嘛？不不不，我们直接就去遍历前一位不要这一位啦，这样的话是不是只要【当前判定位】的前2位和前1位和自己不一样那么它前面都一定不回文呀？

那么，我找到了之后怎么回头确定后头的字符以保证不回文还得最小呢？

请看：012012012012012012012012..... 如果当前位和前一位有0或者1了咋办呢？ ^_^你说呢？

砍掉一个向后找呗120120120120...\201201201201201201... ^_^

那么就over咯~

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
using namespace std;

#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}

int val[1024];

int main()
{
	int n,p;	cin>>n>>p;
	string s;	cin>>s;
	if(n==1 && p==1){cout<<"NO"<<endl;return 0;}
	memset(val,0,sizeof val);
	for(int i=0;i<n;i++)
		val[i]=s[i]-'a';
	int pos=n-1;
	val[pos]++;
	while(1)
	{
		if(pos==0)
		{
			if(val[pos]>=p) {cout<<"NO"<<endl;return 0;}
			else {break;}
		}
		else if(val[pos]>=p)
		{
			val[pos]=0;
			pos--;
			val[pos]++;
		}
		else
		{
			if((pos>1 && val[pos]==val[pos-2]) || val[pos]==val[pos-1])	val[pos]++;
			else {break;}
		}
	}
	for(int i=pos+1;i<n;i++)
	{
		for(int j=0;j<p;j++)
		{
			if(i>0 && j==val[i-1])continue;
			else if(i>1 && j==val[i-2])continue;
			else {val[i]=j;break;}
		}
	}
	for(int i=0;i<n;i++)
	{
		printf("%c",'a'+val[i]);
	}
	return 0;
}
```

























