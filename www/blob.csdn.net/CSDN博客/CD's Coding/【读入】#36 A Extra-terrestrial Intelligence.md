# 【读入】#36 A. Extra-terrestrial Intelligence - CD's Coding - CSDN博客





2014年07月21日 14:53:05[糖果天王](https://me.csdn.net/okcd00)阅读数：572
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Recently Vasya got interested in finding extra-terrestrial intelligence. He made a simple extra-terrestrial signals’ receiver and was keeping a record of the signals for *n* days
 in a row. Each of those *n* days Vasya wrote a 1 in his notebook if he had received a signal that day and a 0 if he hadn’t. Vasya thinks that he has found
 extra-terrestrial intelligence if there is a system in the way the signals has been received, i.e. if all the intervals between successive signals are equal. Otherwise, Vasya thinks that the signals were sent by some stupid aliens no one cares about. Help
 Vasya to deduce from the information given by the receiver if he has found extra-terrestrial intelligence or not.




Input


The first line contains integer *n* (3 ≤ *n* ≤ 100) —
 amount of days during which Vasya checked if there were any signals. The second line contains *n* characters 1 or 0 — the record Vasya kept each of those *n* days.
 It’s guaranteed that the given record sequence contains at least three 1s.




Output


If Vasya has found extra-terrestrial intelligence, output YES, otherwise output NO.




Sample test(s)




input
8
00111000




output
YES




input
7
1001011




output
NO




input
7
1010100




output
YES












这道题……我太蠢了，真的太蠢了……我一直wa了7次，就因为我看到这个是数字然后用%d来读……全都读完了……



改成%c即AC……

这道题的意思是在10序列中看1的位置是否等距，所以使用flag记录第一个和第二个出现的1，获得步长pace，然后如果之后1不是按照pace的步长出现，即为NO，读完即为YES



```cpp
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
	int n,pace=0,p=0;
	int first=0,flag=0;
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout); 
	cin>>n;
	char c;
	scanf("%c",&c);
	for(int i=1;i<=n;i++)
	{
		scanf("%c",&c);
		if(flag==0)
		{
			if(c=='1')flag=1;
			else;
		}
		else if(flag==1)
		{
			if(c=='1')flag=2;
			else pace++;
		}
		else
		{
			if(c=='1')
			{
				if(p!=pace)
				{
					puts("NO");
					return 0;
				}
				else p=0;
			}
			else p++;
		}
	} 
	puts("YES");
	return 0;
}
```











