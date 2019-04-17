# 【水输入】#52 A. Bar - CD's Coding - CSDN博客





2014年08月08日 19:11:57[糖果天王](https://me.csdn.net/okcd00)阅读数：645
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)
















题目需要我们数出数字为18以下的，以及和已有字符串相同的个数有多少个。（年龄小于18、喝的饮料为酒精饮料的个数）

混合输入处理，需要处理是数字还是字母的情况。

我们需要的函数为isupper() \ islower() \ isdigit()

这函数是通过ASCII码来判断某一个字符为大写字符、小写字符或数字的——

Code：






```cpp
#include <cstdio>
#include <memory>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
string name[11]={"ABSINTH","BEER","BRANDY","CHAMPAGNE",
"GIN","RUM","SAKE","TEQUILA","VODKA","WHISKEY","WINE"};

int main()
{
	int n,cnt=0;	cin>>n;
	char huiche;	scanf("%c",&huiche);
	for(int i=0;i<n;i++)
	{
		string now;	cin>>now;
		//cout<<now<<endl;
		if(isupper(now[0]) || islower(now[0]))
		{
			for(int j=0;j<11;j++)
			{
				if(now==name[j])
				{
					cnt++;
					//cout<<cnt<<":"<<now<<endl;
					break;
				}
			}
		}
		else if(isdigit(now[0]))
		{
			int num=0;
			for(int j=0;j<now.length();j++) num=num*10+(now[j]-'0');
			if(num<18) 
			{
				cnt++;
				//cout<<cnt<<":"<<now<<endl;
			}
		}
	} 
	cout<<cnt<<endl;
	return 0;
}
```





