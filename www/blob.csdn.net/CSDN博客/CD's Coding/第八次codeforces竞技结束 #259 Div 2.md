# 第八次codeforces竞技结束 #259 Div 2 - CD's Coding - CSDN博客





2014年08月02日 11:30:28[糖果天王](https://me.csdn.net/okcd00)阅读数：461








这次是Round #259，哈哈正好是吾辈的寝室号呢，算不算主场呢^_^

这次赛间A了3题，1A1WA1A1MLE1A，其中那个mle我开了一个1e4 * 1e4的数组，最后还没用上这个数组……哭瞎，删掉了之后就AC了

Now Let's Record——










Twilight Sparkle once got a crystal from the Crystal Mine. A crystal of size *n* (*n* is
 odd; *n* > 1) is an *n* × *n* matrix with a diamond
 inscribed into it.


You are given an odd integer *n*. You need to draw a crystal of size *n*.
 The diamond cells of the matrix should be represented by character "D". All other cells of the matrix should be represented by character "*".
 Look at the examples to understand what you need to draw.




Input


The only line contains an integer *n* (3 ≤ *n* ≤ 101; *n* is
 odd).




Output


Output a crystal of size *n*.




Sample test(s)




input
3




output
*D*
DDD
*D*




input
5




output
**D**
*DDD*
DDDDD
*DDD*
**D**




input
7




output
***D***
**DDD**
*DDDDD*
DDDDDDD
*DDDDD*
**DDD**
***D***












题意是说有一个奇数n，要求打出一张这样的图，一个最大的菱形元素为D，其余的为*，这不是C语言课的第二课家庭作业么……



然后作为一个程序员，强迫症就犯了Q^Q 我想把代码写漂亮点不想模拟……然后就用草稿纸写式子……写完比他们就慢了好多了Q^Q

### Code：



```cpp
#include <cmath> 
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int n;
	cin>>n;
	int mid=n/2;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(abs(j-mid)<=mid-abs(mid-i))cout<<'D';
			else cout<<'*';
		}
		cout<<endl;
	}
	
	return 0;
}
```














One day, Twilight Sparkle is interested in how to sort a sequence of integers *a*1, *a*2, ..., *a**n*in
 non-decreasing order. Being a young unicorn, the only operation she can perform is a unit shift. That is, she can move the last element of the sequence to its beginning:
*a*1, *a*2, ..., *a**n* → *a**n*, *a*1, *a*2, ..., *a**n* - 1.

Help Twilight Sparkle to calculate: what is the minimum number of operations that she needs to sort the sequence?




Input


The first line contains an integer *n*(2 ≤ *n* ≤ 105).
 The second line contains *n* integer numbers *a*1, *a*2, ..., *a**n*(1 ≤ *a**i* ≤ 105).




Output


If it's impossible to sort the sequence output -1. Otherwise output the minimum number of operations Twilight Sparkle needs to sort it.




Sample test(s)




input
2
2 1




output
1




input
3
1 3 2




output
-1




input
2
1 2




output
0












这题的意思是有一个数组，只有一个操作，就是把最后一个数扔到第一个的前面去，问可不可以通过这种操作将给定数列编程非递减数列。



为了满足这个条件，我们倒过来想，一个非递减数列被用这种方法扔来扔去之后会变成什么呢？——两个非递减数列，且后一个的最大值不能大于前一个的最小值

所以我们需要考虑的问题是：

1）给定数列中非递减数列不得多于2个

2）当非递减数列为2个时，数列最后一位小于等于数列第一位

### Code：



```cpp
#include <cmath> 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
int a[100001]={0};
int dp[100001]={0};

int main()
{
	int n,cnt=0,flag=0;	cin>>n;
	for(int i=0;i<n;i++)
	{
		cin>>a[i];
		if(!flag)
		{
			if(!i && a[i]>=a[i-1])continue;
			if(a[i]<a[i-1]) 
			{
				if(a[i]<=a[0])cnt=1,flag=1;
				else 
				{		
					flag=cnt=-1;
					break;
				}
			}
		}
		else
		{
			if(a[i]>=a[i-1])cnt++;
			else if(a[i]<a[i-1])
			{
				flag=cnt=-1;
				break;
			}
			if(a[i]>a[0])
			{
				flag=cnt=-1;
				break;
			}
		}
		//cout<<i<<":"<<cnt<<endl;
	}
	if(flag==0)cout<<0<<endl;
	else cout<<cnt<<endl;
	return 0;
}
```









Twilight Sparkle was playing Ludo with her friends Rainbow Dash, Apple Jack and Flutter Shy. But she kept losing. Having returned to the castle, Twilight Sparkle became interested in the dice that were used in the game.


The dice has *m* faces: the first face of the dice contains a dot, the second one contains two dots, and so on, the *m*-th
 face contains *m*dots. Twilight Sparkle is sure that when the dice is tossed, each face appears with probability ![](http://espresso.codeforces.com/4a6dcb043a4e66ef2bd9dd59ca7be7f0048f2ceb.png).
 Also she knows that each toss is independent from others. Help her to calculate the expected maximum number of dots she could get after tossing the dice *n* times.




Input


A single line contains two integers *m* and *n* (1 ≤ *m*, *n* ≤ 105).




Output


Output a single real number corresponding to the expected maximum. The answer will be considered correct if its relative or absolute error doesn't exceed 10  - 4.




Sample test(s)




input
6 1




output
3.500000000000




input
6 3




output
4.958333333333




input
2 2




output
1.750000000000






Note


Consider the third test example. If you've made two tosses:
- 
You can get 1 in the first toss, and 2 in the second. Maximum equals to 2.
- 
You can get 1 in the first toss, and 1 in the second. Maximum equals to 1.
- 
You can get 2 in the first toss, and 1 in the second. Maximum equals to 2.
- 
You can get 2 in the first toss, and 2 in the second. Maximum equals to 2.


The probability of each outcome is 0.25, that is expectation equals to:
![](http://espresso.codeforces.com/0c198ca13cd3b68986a83dd1bb24ec5f6be5a8ca.png)

You can read about expectation using the following link: http://en.wikipedia.org/wiki/Expected_value








这道题的意思是有m个数从1到m，我们要从中取n次，问这n次取值最大值的期望为多少。

举例说明，m=2,n=2 取两次的话我们可以取到的是(1,1)(1,2)(2,1)(2,2)最大值为1，2，2，2，那么期望就为7/4。

![](https://img-blog.csdn.net/20140802114745575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


大家应该发现，既然是取max的话那么n维下max为i的数的出现次数应该是 i^n-（i-1）^n

我们把这些都拆开来看：

Ex(1)=1*(1^n)

Ex(2)=2*(2^n-1^n)

Ex(3)=3*(3^n-2^n)

Ex(4)=4*(4^n-3^n)

![](https://img-blog.csdn.net/20140802114432665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





都加起来之后，发现这些东西减着减着都没有了。

![](https://img-blog.csdn.net/20140802114151377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





当然别忘了除以m的n次方~ 因为我们要的是概率乘以权值而不是频数乘以权值~

最后得到这样的一个公式——

![](https://img-blog.csdn.net/20140802114702140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





关于减号右边那个东西，最好不要算i的n次方除以m的n次方哦，肯定会挂的~，咱们先算里头的然后快速幂就好啦^_^（我错了，当时我真的突然脑子一着急就想不出快速幂怎么写了，这种递归是我当时临时想的，不规范各种问题大家还是绕道去模板里找个规范的快速幂比较好）

我会说看着29行的代码我一直不敢交么Q^Q，好可怕 C题代码那么短不太好吧……

### Code：



```cpp
#include <cmath> 
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;
int m,n,num;
double ans;

double quick_pow(double a,int n)
{
	if(n==1)return a;
	if(n%2==0) return quick_pow(a*a,n>>1);
	else return a*quick_pow(a*a,n>>1);
}

double calc()
{
	double tmp=0;
	for(int i=m-1;i>=0;i--)		tmp+= quick_pow((double)i/(double)m,n);
	return tmp;
}

int main()
{
	cin>>m>>n; 
	printf("%f",m-calc());
	return 0;
}
```










