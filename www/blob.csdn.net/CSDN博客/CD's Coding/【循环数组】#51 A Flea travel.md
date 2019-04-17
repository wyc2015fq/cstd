# 【循环数组】#51 A. Flea travel - CD's Coding - CSDN博客





2014年07月31日 10:27:54[糖果天王](https://me.csdn.net/okcd00)阅读数：571标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















A flea is sitting at one of the *n* hassocks, arranged in a circle, at the moment. After minute number *k* the
 flea jumps through *k* - 1 hassoсks (clockwise). For example, after the first minute the flea jumps to the neighboring hassock. You should answer: will the
 flea visit all the hassocks or not. We assume that flea has infinitely much time for this jumping.




Input


The only line contains single integer: 1 ≤ *n* ≤ 1000 — number of hassocks.




Output


Output "YES" if all the hassocks will be visited and "NO" otherwise.




Sample test(s)




input
1




output
YES




input
3




output
NO















枚举题意和调试累死我了……Q^Q 居然memset和init写反了……

有一个跳蚤在一圈草丛中的一个，顺时针跳，第1分钟跳1个，第二分钟俩，第三分钟仨~

问这么无限下去能不能有朝一日给它遍历完。

其实不是无限啦~ 既然是一圈的话，跳N个不就是跳N-N%n个嘛~

那么我们记录每一个草丛第一次经过的时候跳跃力mod(n)是step，下一次又用这个跳跃力mod(n)到达这里之后不就是无限循环了嘛~

Code:



```cpp
#include <cstdio>
#include <memory>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	int n;	cin>>n;
	if(n==1)
	{
		cout<<"YES"<<endl;
		return 0;
	}
	int steps=0,mrk[1001];
	int now=0,cnt=0,flag=0,step=0;
	memset(mrk,-1,sizeof mrk);
	mrk[0]=0,cnt=1,step=0,now=0;
	while(1)
	{
		if(cnt==n)
		{
			flag=1;
			break;
		}
		
		step= (step+1)%n;
		now= (now+step)%n;
		
		if(mrk[now]==step)	break;
		if(mrk[now]==-1)
		{
			cnt++;
			mrk[now]=step;	
		}
		//cout<<now<<":"<<mrk[now]<<"  "<<cnt<<endl;
	} 
	if(flag)cout<<"YES"<<endl;
	else cout<<"NO"<<endl;
	return 0;
}
```














