# 【水I/O】#20 A. BerOS file system - CD's Coding - CSDN博客





2014年05月28日 23:48:23[糖果天王](https://me.csdn.net/okcd00)阅读数：924
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















The new operating system BerOS has a nice feature. It is possible to use any number of characters '/' as a delimiter in path instead of one traditional '/'.
 For example, strings //usr///local//nginx/sbin// and /usr/local/nginx///sbin are
 equivalent. The character '/' (or some sequence of such characters) at the end of the path is required only in case of the path to the root directory, which
 can be represented as single character '/'.


A path called normalized if it contains the smallest possible number of characters '/'.


Your task is to transform a given path to the normalized form.




Input


The first line of the input contains only lowercase Latin letters and character '/' — the path to some directory. All paths start with at least one character '/'.
 The length of the given line is no more than 100 characters, it is not empty.




Output


The path in normalized form.




Sample test(s)




input
//usr///local//nginx/sbin




output
/usr/local/nginx/sbin












——————————————————————————




《Updated 2015/07/15》

回头来看了看这题，觉得以前写的还是略显凌乱，写了一个用STL-String的

AC-Code：



```cpp
#include <cstdio>  
#include <string>
#include <cstring>  
#include <iostream>  
//http://blog.csdn.net/okcd00/article/details/27373475
using namespace std;  
  
int main()   
{
	string s,ans=""; 
	cin>>s; 
	int len=s.length();
	while(s[len-1]=='/')len--;
	s=s.substr(0,len);
	for(int i=0;i<len;i++)
	{
		if(s[i]!='/') ans=ans+s[i];
		else if(i+1<len && s[i+1]!='/') ans=ans+s[i];
	}
	if(ans.length()==0)cout<<'/'<<endl;
	else cout<<ans<<endl;
	return 0;  
}
```



——————————————————————————

看到这道题，吾辈欣喜了一小会~，哇咔咔，水题，秒~

然后写了这个：



```cpp
#include<cstdio>
#include<string>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

int main()
{
	bool flag=true;
	char tmp;
	while(scanf("%c",&tmp)!=EOF)
	{
		if(tmp=='/')flag=true;
		else if(tmp!='/'&&flag==true)
		{
			printf("/%c",tmp);
			flag=false;
		}
		else if(tmp!='/'&&flag==false)printf("%c",tmp);
	}
	return 0;
}
```


然后……WA了…… 原因是///a//这样的case输出了/a/ 多了一个结尾的/，百思不得其解



然后，
wrong output format Unexpected end of file - token expected



再然后：



```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main() 
{
	char str[1000];
  	while(scanf("%s", str) != EOF) //get the whole line
	{
    	int len = strlen(str);		//length
    	for( int i=0; i<len; i++ ) 
		{
     		if(str[i] == '/') //把连在一起的'/'换成'.'只留一个'/' 
			 {
        		i++;
        		while(i<len && str[i] == '/') //下一个（不超出字符串）是否为'/' 
				{
          			str[i] = '.';
          			i++;
        		}
        		i--;
      		}
    	}
    	int k = 0;
    	char ans[1000];
    	for( int i=0; i<len; i++ ) if(str[i] != '.') ans[k++] = str[i];
    	while(k>0 && ans[k-1] == '/') k--;
    	ans[k] = '\0';//end
    	if(k == 0) printf("/\n");
    	else printf("%s\n", ans);
  	}
  return 0;
}
```







