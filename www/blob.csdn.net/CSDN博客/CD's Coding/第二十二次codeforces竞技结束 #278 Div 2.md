# 第二十二次codeforces竞技结束 #278 Div 2 - CD's Coding - CSDN博客





2014年11月22日 04:57:43[糖果天王](https://me.csdn.net/okcd00)阅读数：578








这次的题目略可怕，只出了2道题就成了46名，直接加了161分进入了紫名……







# Codeforces Hot News!

Wow! Coder [CST14-okcd00](http://codeforces.com/profile/CST14-okcd00) competed
 in Codeforces Round #278 (Div. 2) and gained +161 rating points taking place 46

[Share
 on vk](http://codeforces.com/bestRatingChanges/500673#)[Share
 on facebook](http://codeforces.com/bestRatingChanges/500673#)[Share
 on twitter](http://codeforces.com/bestRatingChanges/500673#)[](http://www.addthis.com/bookmark.php?v=300&winname=addthis&pub=ra-4feb83083011e6dd&source=tbx32-300&lng=zh-CN&s=google_plusone_share&url=http%3A%2F%2Fcodeforces.com%2FbestRatingChanges%2F500673&title=CST14-okcd00%20-%20Impressive%20Success%20on%20Codeforces&ate=AT-ra-4feb83083011e6dd/-/-/546fa322812dacdf/2&frommenu=1&uid=546fa3222f1222dd&description=Wow!%20Coder%20CST14-okcd00%20competed%20in%20Codeforces%20Round%20%23278%20(Div.%202)%20and%20gained%20%2B161%20rating%20points%20taking%20place%2046&ct=1&pre=http%3A%2F%2Fcodeforces.com%2Fcontest%2F486%2Fsubmission%2F8659903&tt=0&captcha_provider=nucaptcha)











Giga Tower is the tallest and deepest building in Cyberland. There are 17 777 777 777 floors, numbered from  - 8 888 888 888 to 8 888 888 888.
 In particular, there is floor 0 between floor  - 1 and
 floor 1. Every day, thousands of tourists come to this place to enjoy the wonderful view.


In Cyberland, it is believed that the number "8" is a lucky number (that's why Giga Tower has 8 888 888 888 floors above the ground), and, an integer
 is lucky, if and only if its decimal notation contains at least one digit "8". For example, 8,  - 180, 808 are
 all lucky while42,  - 10 are not. In the Giga Tower, if you write code at a floor
 with lucky floor number, good luck will always be with you (Well, this round is #278, also lucky, huh?).


Tourist Henry goes to the tower to seek good luck. Now he is at the floor numbered *a*. He wants to find the minimum positive integer *b*,
 such that, if he walks *b* floors higher, he will arrive at a floor with a lucky number.




Input


The only line of input contains an integer *a* ( - 109 ≤ *a* ≤ 109).




Output


Print the minimum *b* in a line.




Sample test(s)




input
179




output
1




input
-1




output
9




input
18




output
10






Note


For the first sample, he has to arrive at the floor numbered 180.


For the second sample, he will arrive at 8.


Note that *b* should be positive, so the answer for the third sample is 10,
 not 0.








题意是有着一座顶天入地的塔，我现在在其中的一层，问向上爬最少几层就可以到一个含有数字8的楼层

鉴于最多走10层就能变成个位数为8，so……为何不枚举？

需要注意的是楼层可以为负数不要直接简单相加弄错了哦~




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

bool judge(ll n)
{
	ll nn=abs(n);
	while(nn)
	{
		int now=nn%10;
		if(now==8)return true;
		else nn/=10;
	}
	return false;
}

int main()
{
	ll n;	cin>>n;
	for(ll i=1;;i++)
	{
		if(judge(n+i))
		{
			printf("%d",i);
			return 0;
		} 
	}
	return 0;
}
```











There is an old tradition of keeping 4 boxes of candies in the house in Cyberland. The numbers of candies are special if
 their arithmetic mean, their median and their range are all equal.
 By definition, for a set {*x*1, *x*2, *x*3, *x*4} (*x*1 ≤ *x*2 ≤ *x*3 ≤ *x*4) arithmetic
 mean is ![](http://espresso.codeforces.com/daeb25d0b520beaea095158d4851903f527ffafc.png), median is ![](http://espresso.codeforces.com/7655b731471163052bb5fbb4c5414c495a2ca2e2.png) and range is *x*4 - *x*1. The
 arithmetic mean and median are not necessary integer. It is well-known that if those three numbers are same, boxes will create a "debugging field" and codes in the field will have no bugs.


For example, 1, 1, 3, 3 is the example of 4 numbers
 meeting the condition because their mean, median and range are all equal to 2.


Jeff has 4 special boxes of candies. However, something bad has happened! Some of the boxes could have been lost and now there are only *n* (0 ≤ *n* ≤ 4)
 boxes remaining. The *i*-th remaining box contains *a**i* candies.


Now Jeff wants to know: is there a possible way to find the number of candies of the 4 - *n* missing boxes, meeting the condition above (the
 mean, median and range are equal)?




Input


The first line of input contains an only integer *n* (0 ≤ *n* ≤ 4).


The next *n* lines contain integers *a**i*,
 denoting the number of candies in the *i*-th box (1 ≤ *a**i* ≤ 500).




Output


In the first output line, print "YES" if a solution exists, or print "NO"
 if there is no solution.


If a solution exists, you should output 4 - *n* more lines, each line containing an integer *b*,
 denoting the number of candies in a missing box.


All your numbers *b* must satisfy inequality 1 ≤ *b* ≤ 106.
 It is guaranteed that if there exists a positive integer solution, you can always find such *b*'s meeting the condition. If there are multiple
 answers, you are allowed to print any of them.

Given numbers *a**i* may
 follow in any order in the input, not necessary in non-decreasing.

*a**i* may
 have stood at any positions in the original set, not necessary on lowest *n* first positions.




Sample test(s)




input
2
1
1




output
YES
3
3




input
3
1
1
1




output
NO




input
4
1
2
2
3




output
YES






Note


For the first sample, the numbers of candies in 4 boxes can be 1, 1, 3, 3.
 The arithmetic mean, the median and the range of them are all2.


For the second sample, it's impossible to find the missing number of candies.


In the third example no box has been lost and numbers satisfy the condition.

You may output *b* in any order.










有四盒糖，糖的数量满足以下条件：



四盒糖的平均数和中位数相等，这个数值同时等于最大值和最小值的差。

我们优化一下，就是如果按照从小到大排序ABCD，那么A+D=B+C，2D-2A=B+C 故D=3A

好了这题成了4个盒子，其中一个为另一个的三倍，剩下俩的和等于这俩的和即可。

题目问：我现在只知道其中n个盒子（0~4）里的糖数，有没有可能满足上述条件，有的话输出糖数。

于是分情况讨论咯~ 

0）什么，你让我随便写？ 1223

1）有一个了，A范围在0-500，b的范围不大于1e6……那果断乘三得到D咯 AADD

2）有俩了，那得看看，两个数相同的话直接AADD，不相同的话只要大的不超过小的数的三倍，那大的就是B（也可以是C，无妨），小的是A，然后D=3A，C=3A+A-B

3）都给了仨了那就看看剩下的是谁呗：

如果给的数有一个是另一个的三倍，剩下的那个只要在这俩之间就行，C=A+D-B

如果没有三倍关系那说明是ABC或者BCD组合，我们把最小数乘三吧（反正范围大）

4）四个都有了直接算看看是否满足条件咯~




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
typedef long long ll;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))
int a[5]={0};
const int limit=1000000;
bool cmp(const int a, const int b)
{
	return a > b;
}


int main()
{
	int n;	cin>>n;
	for(int i=0;i<n;i++) scanf("%d",&a[i]);
	if(n==0) printf("YES\n1\n1\n3\n3\n");
	else if(n==1)
	{
		printf("YES\n%d\n%d\n%d\n",a[0]*2,a[0]*2,a[0]*3);
	}
	else if(n==2)
	{
		sort(a,a+2);
		if(a[0]==a[1] && a[0]*3<=limit) printf("YES\n%d\n%d\n",a[0]*3,a[0]*3);
		else if(a[0]==a[1] && a[0]%3==0) printf("YES\n%d\n%d\n",a[0]/3,a[0]/3);
		else
		{
			if(a[1]%a[0]==0 && a[1]/a[0]==3) printf("YES\n%d\n%d\n",a[0],a[1]);
			else if(a[1]/a[0]<3 && a[0]*3<=limit) printf("YES\n%d\n%d\n",a[0]*3,a[0]*3+a[0]-a[1]);
			else printf("NO\n"); //nonsence, ignore it
		}
		
	} 
	else if(n==3)
	{
		sort(a,a+3);
		if(a[2]%a[0]==0 && a[2]/a[0]==3) printf("YES\n%d\n",a[2]-a[1]+a[0]);
		else 
		{
			int rest1=a[2]+a[1]-a[0];	//a_4
			int rest2=a[0]+a[1]-a[2];	//a_1
			if(rest1%a[0]==0 && rest1/a[0]==3 && rest1<=limit) printf("YES\n%d\n",rest1);
			else if(a[2]%rest2==0 && a[2]/rest2==3 && rest2<=limit) printf("YES\n%d\n",rest2);
			else printf("NO\n");
		}
		
	}
	else if(n==4)
	{
		sort(a,a+4);
		if(a[0]+a[3]==a[1]+a[2] && a[3]-a[0]==(a[1]+a[2])/2 && (a[1]+a[2])%2==0 ) printf("YES\n");
		else printf("NO\n");
	}
	return 0;
}
```







A monster is attacking the Cyberland!


Master Yang, a braver, is going to beat the monster. Yang and the monster each have 3 attributes: hitpoints (*HP*), offensive power (*ATK*)
 and defensive power (*DEF*).


During the battle, every second the monster's HP decrease by *max*(0, *ATK**Y* - *DEF**M*),
 while Yang's HP decreases by*max*(0, *ATK**M* - *DEF**Y*),
 where index *Y* denotes Master Yang and index *M* denotes
 monster. Both decreases happen simultaneously Once monster's *HP* ≤ 0 and the same time Master Yang's *HP* > 0,
 Master Yang wins.


Master Yang can buy attributes from the magic shop of Cyberland: *h* bitcoins per *HP*, *a* bitcoins
 per *ATK*, and *d* bitcoins
 per *DEF*.


Now Master Yang wants to know the minimum number of bitcoins he can spend in order to win.




Input


The first line contains three integers *HP**Y*, *ATK**Y*, *DEF**Y*,
 separated by a space, denoting the initial *HP*, *ATK* and *DEF* of
 Master Yang.


The second line contains three integers *HP**M*, *ATK**M*, *DEF**M*,
 separated by a space, denoting the *HP*, *ATK* and *DEF* of
 the monster.


The third line contains three integers *h*, *a*, *d*, separated by a space, denoting the price of 1 *HP*, 1 *ATK* and 1 *DEF*.


All numbers in input are integer and lie between 1 and 100 inclusively.




Output


The only output line should contain an integer, denoting the minimum bitcoins Master Yang should spend in order to win.




Sample test(s)




input
1 2 1
1 100 1
1 100 100




output
99




input
100 100 100
1 1 1
1 1 1




output
0






Note


For the first sample, prices for *ATK* and *DEF* are
 extremely high. Master Yang can buy 99 HP, then he can beat the monster with 1 HP
 left.


For the second sample, Master Yang is strong enough to beat the monster, so he doesn't need to buy anything.










数据范围都这么少了不模拟枚举不开心


### Code：


```cpp
#include <cstdio>
#include <algorithm>

using namespace std;

int main()
{
    int hy, ay, dy, hm, am, dm, h, a, d, ans = 1e9, i, j;
    
    scanf("%d %d %d", &hy, &ay, &dy);
    scanf("%d %d %d", &hm, &am, &dm);
    scanf("%d %d %d", &h, &a, &d);
    
    for (i = 0; i <= 200; i++) {
        for (j = 0; j <= 100; j++) {
            int x = max(0, ay + i - dm);
            int y = max(0, am - dy - j);
            int z, t;
            
            if (x == 0) continue;
            t = (hm + x - 1) / x;
            z = max(0, y * t + 1 - hy);
            ans = min(ans, i * a + j * d + z * h);
        }
    }
    
    printf("%d\n", ans);
    
    return 0;
}
```

















