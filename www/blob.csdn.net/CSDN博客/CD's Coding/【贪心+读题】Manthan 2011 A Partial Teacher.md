# 【贪心+读题】Manthan 2011 A. Partial Teacher - CD's Coding - CSDN博客





2014年10月08日 22:16:25[糖果天王](https://me.csdn.net/okcd00)阅读数：475标签：[Partial Teacher																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=Partial Teacher&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

















A teacher decides to give toffees to his students. He asks *n* students to stand in a queue. Since the teacher is very partial, he follows the following rule
 to distribute toffees.


He looks at the first two students and gives more toffees to the student having higher marks than the other one. If they have the same marks they get the same number of toffees. The same procedure is followed for each pair of adjacent students starting from
 the first one to the last one.


It is given that each student receives at least one toffee. You have to find the number of toffees given to each student by the teacher such that the total number of toffees is minimum.




Input


The first line of input contains the number of students *n* (2 ≤ *n* ≤ 1000).
 The second line gives (*n* - 1) characters consisting of "L",
 "R" and "=". For each pair of adjacent students "L"
 means that the left student has higher marks, "R" means that the right student has higher marks and "="
 means that both have equal marks.




Output


Output consists of *n* integers separated by a space representing the number of toffees each student receives in the queue starting from the first one to the
 last one.




Sample test(s)




input
5
LRLR




output
2 1 2 1 2




input
5
=RRR




output
1 1 2 3 4















偏心的老师:

每个LR=分别代表了这个同学比左边那只同学考的低/比右边那只同学考的低/和左边那只相等，考得好的糖多，考的不好的糖少，问最少给糖方案

我之前一直理解错题意了Q^Q，我以为是L就减一就好，R加一就好，然后最后一起变成正数，（刚刚去查题意还看到一个歧义的解释是 “L代表左边的人比自己高，R表示右边的高，=表示左右相等”，这个解释的话如果改成“与左边的孩纸相比，。。。”就对了）错误代码如下：



```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#define MP(c,g) make_pair(c,g) 
using namespace std;
int dp[1005]={0};

int main()
{
	int n, min=1;
	scanf("%d",&n);
	char c;	scanf("%c",&c);
	memset(dp,0,sizeof dp);
	dp[0]=1;
	for(int i=1;i<n;i++)
	{
		scanf("%c",&c);
		if(c=='=') dp[i]=dp[i-1];
		else if(c=='L') dp[i]=dp[i-1]-1;
		else if(c=='R') dp[i]=dp[i-1]+1;
		if(dp[i]<min) min=dp[i];
	}
	for(int i=0;i<n;i++)
	{
		printf("%d",dp[i]-min+1);
		if(i<n-1)printf(" ");
	}
	return 0;
}
```


实际上是怎么个意思呢，这么跟你们讲应该就懂了^_^



9

RRRRRRLL

我们是不是会以为是123456765呀？不不不，最少应该是123456721，这样分糖最少~ 是否大概理解了呢^_^，分糖什么的【要最少】




Code:



```cpp
#include <queue>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

queue <int> q;
int main()
{
    while(!q.empty()) q.pop();
    int n;    string s;
    cin>>n;    cin>>s;
    for(int c=0;c<n;c++)
    {
        int i=c-1,j=c;
        int l=1,r=1;
        while(s[i]!='L'&&i>=0)	if(s[i]=='R') l++,i--;
								else i--;
        while(s[j]!='R'&&j<n)   if(s[j]=='L') r++,j++;
            					else j++;
        q.push(max(r,l));
    }
    while(q.size()!=1)
    {
        printf("%d ",q.front());
        q.pop();
    }
    printf("%d\n",q.front());
    return 0;
}
```












