# 【strstr】#23 A. You're Given a String... - CD's Coding - CSDN博客





2014年05月30日 15:29:03[糖果天王](https://me.csdn.net/okcd00)阅读数：676标签：[codeforces																[substring																[指针																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=指针&t=blog)













You're given a string of lower-case Latin letters. Your task is to find the length of its longest substring that can be met in the string at least twice. These occurrences can overlap (see sample test 2).




Input


The first input line contains the string. It's guaranteed, that the string is non-empty, consists of lower-case Latin letters, and its length doesn't exceed 100.




Output


Output one number — length of the longest substring that can be met in the string at least twice.




Sample test(s)




input
abcd




output
0



input
ababa




output
3



input
zzz




output
2














题意：

给出一个不超过100字符的字符串，请找出至少出现过两次的子串中最长的子串长度




本来吓了一跳……这是DP么……就算是我也不会呀……只能O(n^3)啊……

一看数据……100……

额……直接暴力了…… 这次用的是 strstr(Momstring,Kidstring,Kidlength)函数（返回第一次找到的指针），挺好用的~~



```cpp
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>

/*************************************************
strstr
原型：const char * strstr ( const char * str1, cosnt char *str2);
            char * strstr ( char * str1, const char * str2);
参数：	str1，待查找的字符串指针；
		str2，要查找的字符串指针。
说明：	在str1中查找匹配str2的子串，并返回指向首次匹配时的第一个元素指针。
		如果没有找到，返回NULL指针。
***************************************************/


using namespace std;
int main()
{
	//freopen("in.txt","r",stdin);
	char line[101];
	char find[100];
	scanf("%s",line);
	int len=strlen(line);
	for(int l=len-1;l>0;l--)
	{
		for(int i=0;i<=len-l;i++)
		{
			strncpy(find,line+i,l);//从line里的第i位开始，数l个字符的字符串扔给find
			find[l]='\0';//很重要，不封口简直是作死
			if(strstr(strstr(line,find)+1,find))//找到一个，然后在他后头一位开始找第二个
			{
				printf("%d",l);
				return 0;
			} 
		}
	}
	printf("0");
	return 0;
}
```


【Updated 2016/01/18】

 其实可以O(n^2)直接记录下有多少子串，用map来hash记数，当出现次数不小于2时更新maxlen即可。



```cpp
#include <map>
#include <cstdio>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <algorithm>  
using namespace std;  
map <string,int> mii;
map <string,int>::iterator it;
int main()
{
	int ans = 0 ;
	string s; cin>>s;
	for(int i=0;i<s.length();i++)
		for(int j=i+1;j<=s.length();j++)
		{
			string cur = s.substr(i,j-i);
			//cout<<cur<<endl;
			mii[cur]++;	
			if (mii[cur]>=2) ans = cur.length()>ans ? cur.length():ans;
		}
	cout<<ans<<endl;
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=substring&t=blog)](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)




