# 【模拟+读入】#21 A. Jabber ID - CD's Coding - CSDN博客





2014年05月30日 13:49:28[糖果天王](https://me.csdn.net/okcd00)阅读数：767
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Jabber ID on the national Berland service «Babber» has a form<username>@<hostname>[/resource], where
- <username> — is a sequence of Latin letters (lowercase or uppercase), digits or underscores characters «_»,
 the length of <username> is between 1 and 16, inclusive.
- <hostname> — is a sequence of word separated by periods (characters «.»),
 where each word should contain only characters allowed for <username>, the length of each word is between 1 and 16, inclusive. The length of <hostname> is
 between 1 and 32, inclusive.
- <resource> — is a sequence of Latin letters (lowercase or uppercase), digits or underscores characters «_»,
 the length of <resource> is between 1 and 16, inclusive.


There are the samples of correct Jabber IDs: mike@codeforces.com,007@en.codeforces.com/contest.


Your task is to write program which checks if given string is a correct Jabber ID.




Input


The input contains of a single line. The line has the length between 1 and 100 characters, inclusive. Each characters has ASCII-code between 33 and 127, inclusive.




Output


Print YES or NO.




Sample test(s)




input
mike@codeforces.com




output
YES




input
john.smith@codeforces.ru/contest.icpc/12




output
NO















这道题是一个给你一个邮箱地址看看合不合规矩的判定：

1.由“A@B/C”的形式组成，A在16个字符内，B在32个字符（不包括.）内，C在16个字符内；

2.可以没有“/C”，但是@的前后都至少要有一个字符

3.B可以由好多个'.'分开，但是每个单词不能超过16个字符。

4.允许的字符在'_' isupper() isdigit() islower() 范围内




我傻乎乎的还想用



```cpp
scanf("%s@%s/%s",a,b,c);
```
这样的格式来偷懒……看来是正则表达式看的异想天开了。

AC-Code：



```cpp
#include <cstdio>  
#include <string>
#include <cstring>  
#include <iostream>  
//http://blog.csdn.net/okcd00/article/details/27669883
using namespace std;  
#define Giveup {cout<<"NO"<<endl;return 0;}
char str[101]; 
bool judge(char c)
{ return (c=='_'||islower(c)||isdigit(c)||isupper(c)); }

int main()  
{  
    //freopen("in.txt","r",stdin);  
    int cnt=0;  
    int count=0;//each words - Whole hostname  
    int type=0;//0-username 1-hostname 2-resource  
      
    while(scanf("%s",str)!=EOF)
    {
		type=0;//init is username input    
    	int len=strlen(str);      
    	for(int i=0;i<len;i++)  
    	{  
    	    char c=str[i];  
    	    //change kind  
    	    if(c=='@')  
    	    {  
    	        if(type==1) Giveup
    	        type=1;  
    	        //change to hostname  
    	        if(cnt<1||cnt>16) Giveup;
    	        cnt=0;  
    	    }  
    	    else if(c=='/')  
    	    {  
    	        if(type==2) Giveup
    	        type=2;
				//change to resource  
   	        	if(cnt<1||cnt>16) Giveup
            	if(count<1||count>32) Giveup
            	cnt=0;  
        	}  
        	else if(c=='.')  
        	{  
        	    if(type!=1) Giveup
        	    if(cnt<1||cnt>16) Giveup
        	    cnt=0;  
        	}  
        	else if(c>127||c<33) Giveup
        	else if(type==0) 
        	{  
        	    if(judge(c))   
        	    {  
        	        cnt++;  
        	        continue;  
        	    }  
        	    else Giveup
        	}  
        	else if(type==1)  
        	{  
        	    if(judge(c))   
        	    {  
        	        cnt++;  
        	        count++;  
        	        continue;     
        	    }  
        	    else Giveup
        	}  
        	else if(type==2)  
        	{  
        	    if(judge(c))  
        	    {  
        	        cnt++;  
        	        continue;  
        	    }   
        	    else Giveup
        	}  
    	}  	
    	if(cnt<0||cnt>16) Giveup
    	if(type>=1&&cnt>0) cout<<"YES"<<endl;
    	else Giveup
    }
}
```






调试用代码:



```cpp
#include<map>
#include<string>
#include<cstdio>
#include<ctype.h>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
#define Nx cout<<"NO"

char str[101];

int main()
{
	//freopen("in.txt","r",stdin);
	int cnt=0;
	int count=0;//each words - Whole hostname
	int type=0;//0-username 1-hostname 2-resource
	
	type=0;//init is username input
	while(scanf("%s",str)!=EOF);
	int len=strlen(str);	
	for(int i=0;i<len;i++)
	{
		char c=str[i];
		//change kind
		if(c=='@')
		{
			if(type==1)
			{
				Nx;
				//printf("NO in @");
				return 0;
			} 
			type=1;
			//change to hostname
			if(cnt<1||cnt>16)
			{
				Nx;
				//printf("NO in @");
				return 0;
			} 
			cnt=0;
		}
		else if(c=='/')
		{
			if(type==2)
			{
				Nx;
				//printf("NO in @");
				return 0;
			} 
			type=2;//change to resource
			if(cnt<1||cnt>16)
			{
				Nx;
				//printf("NO in host small");
				return 0;
			} 
			if(count<1||count>32)
			{
				Nx;
				//printf("NO in host big");
				return 0;
			} 
			cnt=0;
		}
		else if(c=='.')
		{
			if(type!=1) 
			{
				Nx;
				//printf("NO in .");
				return 0;
			} 
			if(cnt<1||cnt>16)
			{
				Nx;
				//printf("NO in .");
				return 0;
			} 
			cnt=0;
		}
		else if(c>127||c<33)	//test if ASCII is right
		{
			//if(scanf("%c",&c)!=EOF)  test failed
			{
				Nx;
				//printf("\nNO in range: %c",c);
				return 0;
		
			}
		}
		else if(type==0)
		{
			if(isupper(c)||islower(c)||isdigit(c)||c=='_') 
			{
				cnt++;
				continue;
			}
			else 	Nx;
					//printf("NO in c wrong 0");
					return 0;
		}
		else if(type==1)
		{
			if(isupper(c)||islower(c)||isdigit(c)||c=='_') 
			{
				cnt++;
				count++;
				continue;	
			}
			else 	Nx;
				//	printf("NO in c wrong 1");
					return 0;
		}
		else if(type==2)
		{
			if(isupper(c)||islower(c)||isdigit(c)||c=='_')
			{
				cnt++;
				continue;
			} 
			else 	Nx;
				//	printf("NO in c wrong 2");
					return 0;
		}
	}
	if(cnt<0||cnt>16)
	{
		Nx;
		//printf("NO in resource");
		return 0;
	}
	
	if(type>=1&&cnt>0) 	printf("YES");
	else Nx;
	return 0;
}
```







