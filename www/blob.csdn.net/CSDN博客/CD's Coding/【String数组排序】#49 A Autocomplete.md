# 【String数组排序】#49 A. Autocomplete - CD's Coding - CSDN博客





2014年07月30日 16:54:28[糖果天王](https://me.csdn.net/okcd00)阅读数：553标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Autocomplete is a program function that enables inputting the text (in editors, command line shells, browsers etc.) completing the text by its inputted part. Vasya is busy working on a new browser called 'BERowser'. He happens to be working on the autocomplete
 function in the address line at this very moment. A list consisting of *n* last visited by the user pages and the inputted part *s* are
 known. Your task is to complete *s* to make it an address of one of the pages from the list. You have to find the lexicographically smallest address having
 a prefix *s*.




Input


The first line contains the *s* line which is the inputted part. The second line contains an integer *n* (1 ≤ *n* ≤ 100)
 which is the number of visited pages. Then follow *n* lines which are the visited pages, one on each line. All the lines have lengths of from 1 to 100 symbols
 inclusively and consist of lowercase Latin letters only.




Output


If *s* is not the beginning of any of *n* addresses of
 the visited pages, print *s*. Otherwise, print the lexicographically minimal address of one of the visited pages starting from *s*.


The lexicographical order is the order of words in a dictionary. The lexicographical comparison of lines is realized by the '<' operator in the modern programming
 languages.




Sample test(s)




input
next
2
nextpermutation
nextelement




output
nextelement




input
find
4
find
findfirstof
findit
fand




output
find




input
find
4
fondfind
fondfirstof
fondit
fand




output
find












问能够把字符串s补齐的字符串中字典序最小的是哪个



用substr来分析这个字符串是否是以s为前缀的，然后用string数组来存。

最后stl的sort字典序排序之后输出头。

这题我用了string数组排序，需要注意的是stl的sort来排序的话起始位置为起始位置，但结束位置为终止位置加一



```cpp
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;
string us[1001];

int main()
{
	string s;	cin>>s;
	int n;	cin>>n;	
	char huiche;
	scanf("%c",&huiche);
	int len=s.length(),idx=0;
	for(int i=0;i<n;i++)
	{
		string stmp; cin>>stmp;
		//cout<<stmp.substr(0,len);
		if(stmp.length()<len);
		else if(stmp.substr(0,len)==s) 
		{
			us[++idx]=stmp;
			//cout<<idx;
		}
	}
	if(!idx)cout<<s<<endl;
	else
	{
		sort(us+1,us+idx+1);	//begin & end+1
		//for(int i=1;i<=idx;i++)	cout<<i<<":"<<us[i]<<endl;
		cout<<us[1]<<endl;
	}
	return 0;
}
```








