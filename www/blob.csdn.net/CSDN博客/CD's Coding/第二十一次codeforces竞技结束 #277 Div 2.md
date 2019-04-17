# 第二十一次codeforces竞技结束 #277 Div 2 - CD's Coding - CSDN博客





2014年11月22日 04:42:02[糖果天王](https://me.csdn.net/okcd00)阅读数：506








寝室外面有个男生在和女生打分手电话……时长85分钟，我只能含泪努力，结果还是坑掉了这场……55555‘




Problems

![](http://codeforces.com/static/images/icons/control.png)






|#|Name|||
|----|----|----|----|
|[A](http://codeforces.com/contest/486/problem/A)|[Calculating Function](http://codeforces.com/contest/486/problem/A)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x4256|
|[B](http://codeforces.com/contest/486/problem/B)|[OR in Matrix](http://codeforces.com/contest/486/problem/B)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x2520|
|[C](http://codeforces.com/contest/486/problem/C)|[Palindrome Transformation](http://codeforces.com/contest/486/problem/C)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x1769|
|[D](http://codeforces.com/contest/486/problem/D)|[Valid Sets](http://codeforces.com/contest/486/problem/D)standard input/output1 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x643|
|[E](http://codeforces.com/contest/486/problem/E)|[LIS of Sequence](http://codeforces.com/contest/486/problem/E)standard input/output2 s, 256 MB|![Submit](http://codeforces.com/static/images/icons/submit-22x22.png)![Add to favourites](http://codeforces.com/static/images/icons/star_gray_16.png)|![](http://codeforces.com/static/images/icons/user.png) x381|





大家都表示简单的一场，我一直被打断一直低级错误，WA数空前新高……哭













For a positive integer *n* let's define a function *f*:

*f*(*n*) =  - 1 + 2 - 3 + .. + ( - 1)*n**n*


Your task is to calculate *f*(*n*) for a given integer *n*.




Input


The single line contains the positive integer *n* (1 ≤ *n* ≤ 1015).




Output


Print *f*(*n*) in a single line.




Sample test(s)




input
4




output
2




input
5




output
-3






Note

*f*(4) =  - 1 + 2 - 3 + 4 = 2

*f*(5) =  - 1 + 2 - 3 + 4 - 5 =  - 3




题意的话就是说，-+-+-+-+这样的加减式子，有n个数字组成，问结果。

分奇偶然后两两一组求和~

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
	long long n=0;
	cin>>n;
	if(n%2==0)cout<<n/2;
	else cout<<n/2-n;
	return 0;
}
```














Let's define logical *OR* as an operation on two logical values (i. e. values that belong to the set {0, 1})
 that is equal to 1 if either or both of the logical values is set to 1,
 otherwise it is 0. We can define logical *OR* of
 three or more logical values in the same manner:

![](http://espresso.codeforces.com/661f67f4b56b351d145dce2e9a87cd7c5f6a4d64.png) where ![](http://espresso.codeforces.com/7bcdd7440cc300269a3395fa037d5af6351c3221.png) is
 equal to 1 if some *a**i* = 1,
 otherwise it is equal to 0.


Nam has a matrix *A* consisting of *m* rows
 and *n* columns. The rows are numbered from 1 to *m*,
 columns are numbered from 1 to *n*.
 Element at row *i* (1 ≤ *i* ≤ *m*)
 and column *j* (1 ≤ *j* ≤ *n*)
 is denoted as *A**ij*.
 All elements of *A* are either 0 or 1. From matrix *A*,
 Nam creates another matrix *B* of the same size using formula:

![](http://espresso.codeforces.com/4332dfa4828fa3b9679abe53387834c43c801689.png).


(*B**ij* is *OR* of
 all elements in row *i* and column *j* of
 matrix *A*)


Nam gives you matrix *B* and challenges you to guess matrix *A*.
 Although Nam is smart, he could probably make a mistake while calculating matrix *B*, since size of *A* can
 be large.




Input


The first line contains two integer *m* and *n* (1 ≤ *m*, *n* ≤ 100),
 number of rows and number of columns of matrices respectively.


The next *m* lines each contain *n* integers
 separated by spaces describing rows of matrix *B* (each element of *B* is
 either 0 or 1).




Output


In the first line, print "NO" if Nam has made a mistake when calculating *B*,
 otherwise print "YES". If the first line is "YES",
 then also print*m* rows consisting of *n* integers
 representing matrix *A* that can produce given matrix *B*.
 If there are several solutions print any one.




Sample test(s)




input
2 2
1 0
0 0




output
NO




input
2 3
1 1 1
1 1 1




output
YES
1 1 1
1 1 1




input
2 3
0 1 0
1 1 1




output
YES
0 0 0
0 1 0












有一个矩阵A，是由01组成的，然后逐行逐列进行OR逻辑运算，得到的结果为B矩阵。



简单来说，就是如果A里有个1，那么B里这个1的所在行和所在列都成1了。

给结果矩阵问原矩阵是什么。

先逐行读，找出全为1的行，记录位置，然后逐列读，找出全为1的列，同时判断不为全1行却出现1的话直接判NO。

获得行列位置之后一一对应，行、列用剩下的都和对方的第一个下标挤一挤就好。

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
int mat[111][111]={0};
int ans[111][111]={0};
int visrow[111]={0};
int col[111]={0},vcol=0;
int row[111]={0},vrow=0;
#define Max(a,b) ((a)>(b)?(a):(b))
#define Min(a,b) ((a)<(b)?(a):(b))

bool cmp(const int a, const int b)
{
	return a > b;
}



int main()
{
	int n,m;	cin>>n>>m;
	int flag=0,zero=0;
	for(int i=0;i<n;i++)
	{
		scanf("%d",&mat[i][0]);	flag=0;
		if(mat[i][0]) flag=1,zero=1;
		for(int j=1;j<m;j++)
		{
			scanf("%d",&mat[i][j]);
			if(mat[i][j]) zero=1;
			if(mat[i][j]!=mat[i][0])
			{
				flag=0;
			}
		}
		if(flag) row[vrow++]=i, visrow[i]=1;
	}
	for(int j=0;j<m;j++)
	{
		flag=0;
		if(mat[0][j]) flag=1;
		for(int i=1;i<n;i++)
		{
			if(mat[i][j]!=mat[0][j])
			{
				flag=0;
			}		
		}
		if(flag) col[vcol++]=j;
		else
		{
			for(int i=0;i<n;i++)
			{
				if(mat[i][j] && visrow[i]==0)
				{
					cout<<"NO";
					return 0;
				}
			}
		}
	}
	if(zero==0)
	{
		cout<<"YES"<<endl;
		for(int i=0;i<n;i++)
		{
			printf("%d",mat[i][0]);
			for(int j=1;j<m;j++)
			{
				printf(" %d",mat[i][j]);
			}
			printf("\n");
		}
		return 0;
	}
	if(vcol==0 || vrow==0)
	{
		cout<<"NO";
		return 0;
	}
	int l=max(vcol,vrow),s=min(vcol,vrow),cntp=0;
	for(cntp=0;cntp<s;cntp++)
	{
		ans[row[cntp]][col[cntp]]=1;	
	}
	if(l==vrow)	for(;cntp<l;cntp++)
	{
		ans[row[cntp]][col[0]]=1;
	}
	else for(;cntp<l;cntp++)
	{
		ans[row[0]][col[cntp]]=1;
	}
	printf("YES\n");
	for(int i=0;i<n;i++)
	{
		printf("%d",ans[i][0]);
		for(int j=1;j<m;j++)
		{
			printf(" %d",ans[i][j]);
		}
		printf("\n");
	}
	return 0;
}
```

















