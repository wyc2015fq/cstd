# 第二十七次codeforces竞技结束 #288 Div 2 - CD's Coding - CSDN博客





2015年01月30日 01:53:34[糖果天王](https://me.csdn.net/okcd00)阅读数：536











Problems

![](http://codeforces.com/static/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.com/contest/508/problem/A)|[Pasha and Pixels](http://codeforces.com/contest/508/problem/A)standard input/output2 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x3234|
|[B](http://codeforces.com/contest/508/problem/B)|[Anton and currency you all know](http://codeforces.com/contest/508/problem/B)standard input/output0.5 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x2848|
|[C](http://codeforces.com/contest/508/problem/C)|[Anya and Ghosts](http://codeforces.com/contest/508/problem/C)standard input/output2 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x1671|





这次出了前三题，原为第三位的Avator加了100来分变成第二了，这下终于三个ID都1650+了，可喜可贺，希望能多撑几天吧……













Pasha loves his phone and also putting his hair up... But the hair is now irrelevant.


Pasha has installed a new game to his phone. The goal of the game is following. There is a rectangular field consisting of *n* row with *m*pixels
 in each row. Initially, all the pixels are colored white. In one move, Pasha can choose any pixel and color it black. In particular, he can choose the pixel that is already black, then after the boy's move the pixel does not change, that is, it remains black.
 Pasha loses the game when a 2 × 2 square consisting of black pixels is formed.


Pasha has made a plan of *k* moves, according to which he will paint pixels. Each turn in his plan is represented as a pair of numbers *i*and *j*,
 denoting respectively the row and the column of the pixel to be colored on the current move.


Determine whether Pasha loses if he acts in accordance with his plan, and if he does, on what move the 2 × 2 square consisting of black pixels is formed.




Input


The first line of the input contains three integers *n*, *m*, *k* (1 ≤ *n*, *m* ≤ 1000, 1 ≤ *k* ≤ 105) —
 the number of rows, the number of columns and the number of moves that Pasha is going to perform.


The next *k* lines contain Pasha's moves in the order he makes them. Each line contains two integers *i* and *j* (1 ≤ *i* ≤ *n*, 1 ≤ *j* ≤ *m*),
 representing the row number and column number of the pixel that was painted during a move.




Output


If Pasha loses, print the number of the move when the 2 × 2 square consisting of black pixels is formed.


If Pasha doesn't lose, that is, no 2 × 2 square consisting of black pixels is formed during the given *k* moves,
 print 0.




Sample test(s)




input
2 2 4
1 1
1 2
2 1
2 2




output
4




input
2 3 6
2 3
2 2
1 3
2 2
1 2
1 1




output
5




input
5 3 7
2 3
1 2
1 1
4 1
3 1
5 3
3 2




output
0















给你一个n*m的棋盘，每次你走到的格子都会被涂黑，接下来有k步，每一步都告诉你我这步走的是哪一个格子（Row/Col坐标），要求输出在第几步时第一次出现了2X2的黑色正方形，如果走完了都没出现，则输出0。

看了看数据范围，似乎……模拟完全无压呢

我们就真的给一个棋盘，每走一步我们就把这个格子 mp[a][b]=1 标记一下，然后找左上右上左下右下，这四个2X2的正方形是不是全黑，是的话输出然后return，不是的话就继续咯~

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

int mp[1002][1002]={0};

bool judge(int r,int c)
{
	if(mp[r-1][c]==1)
	{
		if(mp[r][c-1]==1)
		{
			if(mp[r-1][c-1]==1)return true;
		}
		if(mp[r][c+1]==1)
		{
			if(mp[r-1][c+1]==1)return true;
		}
	}
	if(mp[r+1][c]==1)
	{
		if(mp[r][c-1]==1)
		{
			if(mp[r+1][c-1]==1)return true;
		}
		if(mp[r][c+1]==1)
		{
			if(mp[r+1][c+1]==1)return true;
		}
	}
	return false;
}

int main()
{
	int n,m,k;	cin>>n>>m>>k;
	for(int i=1;i<=k;i++)
	{
		int r,c;	scanf("%d%d",&r,&c);
		mp[r][c]=1;
		if(judge(r,c))
		{
			cout<<i;
			return 0;
		} 
	}
	cout<<0;
	return 0;
}
```












Berland, 2016. The exchange rate of currency you all know against the burle has increased so much that to simplify the calculations, its fractional part was neglected and the exchange rate is
 now assumed to be an integer.


Reliable sources have informed the financier Anton of some information about the exchange rate of currency you all know against the burle for tomorrow. Now Anton knows that tomorrow the exchange
 rate will be an even number, which can be obtained from the present rate by swapping exactly two distinct digits in it. Of all the possible values that meet these conditions, the exchange rate for tomorrow will be the maximum possible. It is guaranteed that
 today the exchange rate is an odd positive integer *n*. Help Anton to determine the exchange
 rate of currency you all know for tomorrow!




Input


The first line contains an odd positive integer *n* — the exchange rate of currency you all know for
 today. The length of number *n*'s representation is within range from 2 to 105,
 inclusive. The representation of *n* doesn't contain any leading zeroes.




Output


If the information about tomorrow's exchange rate is inconsistent, that is, there is no integer that meets the condition, print  - 1.


Otherwise, print the exchange rate of currency you all know against the burle for tomorrow. This should be the maximum possible number of those that are even and that are obtained from today's
 exchange rate by swapping exactly two digits. Exchange rate representation should not contain leading zeroes.




Sample test(s)




input
527




output
572




input
4573




output
3574




input
1357997531




output
-1















给一个很长的数字，至少两位数，最多10的五次方位的一个奇数，让你调换其中两个数位，将其变为可变的各种选择中最大的偶数并输出，如果做不到则输出-1。

很显然，最后一位决定了奇偶，所以调换的两位中确定了一位是末位，然后，做不到的意思就是数字中没有偶数数字，特例排除后我们来考虑如何最大。

因为数字太大，所以我们不能直接加减甚至无法记录最大值，但是我们在纸上写一写就能发现，很明显调换后与调换前的差一定是一个 A99999...999B 的数，而AB作为一个两位数来看就是两个调换位调换前后的两位数的差，即 10*B+A-10*A-B=9*(B-A) ，连9我们都不必要了…… 我们就简单的通过正负（zf）、9的个数（digit）和差值（sub）来记录就可以啦~ 话说我为啥要把正负提出来！！！ 直接正负和差值用一个int不就可以了么！我真傻……真的……

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

string s;	
string ans;
int zf,digit,sub;
int tzf,td,ts,len; //temp zf,dig,sub
map<int,int> m;

void update(int pos)
{
	zf=tzf;
	digit=td;
	sub=ts;
	ans=s;
	char temp=ans[pos];
	ans[pos]=ans[len-1];
	ans[len-1]=temp;
}

int main()
{
	m.clear();
	cin>>s;
	len=s.length();
	int tail=s[len-1]-'0';
	for(int i=0;i<len;i++)
	{
		int num=s[i]-'0';
		if(num%2==0) m[i]=num;
	}
	if(m.empty())
	{
		cout<<-1;
		return 0;
	}
	zf=-1,digit=100001,sub=99;
	tzf=0,td=0,ts=0; //temp zf,dig,sub
	for (map<int,int>::iterator it=m.begin(); it!=m.end(); ++it)
	{
		//cout << it->first << " => " << it->second << '\n';
		int pos=it->first;
		int now=s[pos]-'0';
		tzf=1;
		ts=tail-now; //*9 略去 
		td=s.length()-pos;
		if(ts<0){tzf=-1;ts=-ts;}
		
		if(tzf>zf) update(pos);
		else if(tzf==1 && zf==1)
		{
			if(td>digit) update(pos);
			else if(td==digit)
			{
				if(ts>sub) update(pos);
			}
		}
		else if(tzf==-1 && zf==-1)
		{
			if(td<digit) update(pos);
			else if(td==digit)
			{
				if(ts<sub) update(pos);
			}
		}
	}
	cout<<ans;
	return 0;
}
```


### Another Code:





```cpp
#include<stdio.h>
#include<string.h>
int n,i,x,minpos=-1;
char a[100010],t;
int main(){
    scanf("%s",a);
    n=strlen(a);
    for(i=0;i<n-1;i++){
        if((a[i]-'0')%2==0){
            if(a[n-1]>a[i]){
                t = a[n-1] , a[n-1] = a[i] , a[i] = t;
                printf("%s\n",a);
                return 0;
            }
            else{
                minpos = i;
            }
        }
    }
    if(minpos==-1){
        printf("-1\n");
    }
    else{
        t = a[n-1] , a[n-1] = a[minpos] , a[minpos] = t;
        printf("%s\n",a);
        return 0;
    }
}
```












Anya loves to watch horror movies. In the best traditions of horror, she will be visited by *m* ghosts tonight. Anya has lots of candles prepared for the visits,
 each candle can produce light for exactly *t* seconds. It takes the girl one second to light one candle. More formally, Anya can spend one second to light
 one candle, then this candle burns for exactly *t* seconds and then goes out and can no longer be used.


For each of the *m* ghosts Anya knows the time at which it comes: the *i*-th
 visit will happen *w**i* seconds
 after midnight, all *w**i*'s
 are distinct. Each visit lasts exactly one second.


What is the minimum number of candles Anya should use so that during each visit, at least *r* candles are burning? Anya can start to light a candle at any
 time that is integer number of seconds from midnight, possibly, at the time before midnight. That means, she can start to light a candle integer number of seconds before midnight or integer number of
 seconds after a midnight, or in other words in any integer moment of time.




Input


The first line contains three integers *m*, *t*, *r* (1 ≤ *m*, *t*, *r* ≤ 300),
 representing the number of ghosts to visit Anya, the duration of a candle's burning and the minimum number of candles that should burn during each visit.


The next line contains *m* space-separated numbers *w**i* (1 ≤ *i* ≤ *m*, 1 ≤ *w**i* ≤ 300),
 the *i*-th of them repesents at what second after the midnight the *i*-th
 ghost will come. All *w**i*'s
 are distinct, they follow in the strictly increasing order.




Output


If it is possible to make at least *r* candles burn during each visit, then print the minimum number of candles that Anya needs to light for that.


If that is impossible, print  - 1.




Sample test(s)




input
1 8 3
10




output
3




input
2 10 1
5 8




output
1




input
1 1 3
10




output
-1






Note

Anya can start lighting a candle in the same second with ghost visit. But this candle isn't counted as burning at this visit.


It takes exactly one second to light up a candle and only after that second this candle is considered burning; it means that if Anya starts lighting candle at moment x, candle is buring from second x + 1 to second x + t inclusively.


In the first sample test three candles are enough. For example, Anya can start lighting them at the 3-rd, 5-th
 and 7-th seconds after the midnight.


In the second sample test one candle is enough. For example, Anya can start lighting it one second before the midnight.


In the third sample test the answer is  - 1, since during each second at most one candle can burn but Anya needs three candles to light up the room at the moment
 when the ghost comes.










安娜是个小姑娘，有m只鬼晚上要来安娜家（鬼登门拜访的时间以升序给出），小姑娘每秒钟可以点燃1根蜡烛，蜡烛可以燃烧t秒，每只鬼到她家的时候她家需要有至少r根蜡烛在燃烧，问最少用多少根蜡烛可以渡过难关。



首先我们需要考虑“-1”即做不到，是什么情况，每秒都在点蜡烛但是在任何一秒都无法实现同时有r根蜡烛在亮着，那么，m<r时就放弃吧，输出-1。

当m>=r时，我们在任一根蜡烛燃烧的最后一秒时点燃一根蜡烛，蜡烛熄灭的同时新蜡烛燃烧第一秒，就可以达成延续r根蜡烛的状态。首先，第一只鬼来的时候很明显为了不浪费蜡烛的持续时间，我们从鬼来的前一秒点燃开始向前推，前1秒到前r秒都在点蜡烛，这时在第一个鬼来的时候刚好r根蜡烛而且他们总剩余燃烧时间最大，什么你问我如果鬼第一秒就来了怎么点？仔细看题：**That means, she can start to light a candle integer number of seconds before midnight or integer number of seconds after a midnight, or in other words in any integer moment of time.**

然后我们用lgt（light）数组来记录每一秒的当前亮度是多少，为了得知某根蜡烛啥时候灭，我们在点燃的时候就在lgt[当前秒数+t]处赋值-1，这样dp推到那里的时候就会自动减一达到减少的效果了，我们从第一秒开始推，当遇到鬼了我们要看现在光够不够，不够的话从前一秒开始向前补蜡烛，然后一路向后，最终蜡烛数必然是最小，也算是一个贪心dp的感觉吧

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

int vit[666]={0};
int lgt[666]={0};	//degree of light

int main()
{	
	int c=0;
	int m,t,r;	cin>>m>>t>>r;
	if(t<r) {cout<<-1;return 0;}
	for(int i=1;i<=m;i++)
		scanf("%d",&vit[i]);
	int now=1;
	for(int i=1;i<=300;i++)
	{
		lgt[i]+=lgt[i-1];
		if(i==vit[now])
		{
			if(lgt[i]!=r)
			{
				int dec=r-lgt[i];
				c+=dec;
				for(int vt=i+t;i+t-vt<dec;vt--)
				{
					//cout<<"decing..."<<endl;
					lgt[vt]--;
				}
				lgt[i]=r;
			}
			now++;
		}
		//cout<<"lgt["<<i<<"]:"<<lgt[i]<<endl;
	} 
	cout<<c;
	return 0;
}
```























