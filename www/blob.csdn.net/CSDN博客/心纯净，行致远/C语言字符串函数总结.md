# C语言字符串函数总结 - 心纯净，行致远 - CSDN博客





2018年05月19日 18:06:41[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：52标签：[字符串函数																[c语言](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串函数&t=blog)
个人分类：[【C】](https://blog.csdn.net/zhanshen112/article/category/7184374)








# strlen

用法：size_t strlen(const char *s)。返回字符串s的长度，不包括结尾的“\0”。

## 实例1：演示strlen的使用

```cpp
#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[]) 
{
	char line[]="hello";
	printf("strlen=%lu\n",strlen(line));
	printf("sizeof=%lu\n",sizeof(line));
	
	return 0;
}
/*
result:
strlen=5
sizeof=6
*/
```

## 实例2：自己编程实现strlen函数的功能

```cpp
#include <stdio.h>
#include <string.h>

int mylen(const char *s)
{
	int indx=0;
	while(s[indx]!='\0')
	{
		indx++;
	}
	return indx;
}

int main(int argc,char *argv[]) 
{
	char line[]="hello";
	printf("strlen=%lu\n",mylen(line));
	printf("sizeof=%lu\n",sizeof(line));
	
	return 0;
}
/*
result:
strlen=5
sizeof=6
*/
```

# strcmp

用法：int strcmp(const char *s1,const char *s2)。比较两个字符串大小，返回：0：s1==s2;    1：s1>s2;    -1:s1<s2;

实例1：strcmp用法

```cpp
#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="abc";
	printf("%d\n",strcmp(s1,s2));
	
	return 0;
}
/*
result:
0
*/
```

```cpp
#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="pbc";
	printf("%d\n",strcmp(s1,s2));
	
	return 0;
}
/*
result:
-1
*/
```

注意：两个字符串不能s1==s2进行比较，因为它们的内存地址永远不可能是相同的，因此无法比较，编译器也不允许。


实例2：

```cpp
#include <stdio.h>
#include <string.h>

int mycmp(char *s1,char *s2)
{
	int idx=0;
	while(1) //对数组进行操作
	{
		if(s1[idx]!=s2[idx]){
			break;
		}
		else if(s1[idx]=='\0'){
			break;
		}
	idx++;
	 } 
	 	
	 return s1[idx]-s2[idx];
}
int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="abc ";
	printf("%d\n",mycmp(s1,s2));
	
	return 0;
}
/*
result:
-32
*/
```

实例3：

```cpp
#include <stdio.h>
#include <string.h>

int mycmp(char *s1,char *s2)
{
	int idx=0;
	while(s1[idx]==s2[idx]&&s1[idx]!='\0')//对数组进行操组
	{
		idx++;
	 } 	 	
	 return s1[idx]-s2[idx];
}
int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="abc ";
	printf("%d\n",mycmp(s1,s2));
	
	return 0;
}
/*
result:
-32
*/
```

实例4：

```cpp
#include <stdio.h>
#include <string.h>

int mycmp(char *s1,char *s2)
{
	while(*s1==*s2&&*s1!='\0')  //对指针进行操作 
	{
		s1++; 
		s2++;
	 } 	 	
	 return *s1-*s2;
}
int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="abc ";
	printf("%d\n",mycmp(s1,s2));
	
	return 0;
}
/*
result:
-32
*/
```

# strcpy

strcpy usage:    char* strcpy(char* restrict dst,const char *restrict src)
function:    把src的字符串拷贝到dst
restrict表明src和dst不重叠


返回dst，目的是为了能连起代码来 


**复制一个字符串：**
char* dst=（char*）malloc（strlen（src）+1）; //加1是因为每个字符串结尾都有‘\0’


strcpy(dst,src)

实例1：

```cpp
#include <stdio.h>
#include <string.h>

char* mycpy(char* dst,char* src)
{
	int idx=0;
	while(src[idx]!='\0'){    //用数组实现
		idx++;
		dst[idx]=src[idx];
	}
	dst[idx]='\0';	//字符串结尾的'\0'要单独给 
	return dst;
}
int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="abc";
	strcpy(s1,s2);
	
	return 0;
}
```

实例2：用指针实现

```cpp
#include <stdio.h>
#include <string.h>

char* mycpy(char* dst,char* src)
{
	char* ret=dst; 
	while(*src!='\0'){
		*dst++=*src++;
	}
	*dst='\0';	//字符串结尾的'\0'要单独给 
	return *ret;
}
int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="abc";
	strcpy(s1,s2);
	
	return 0;
}
```

实例3：指针优化

```cpp
#include <stdio.h>
#include <string.h>

char* mycpy(char* dst,char* src)
{
	char* ret=dst; 
	while(*dst++=*src++)
	;
	*dst='\0';	//字符串结尾的'\0'要单独给 
	return *ret;
}
int main(int argc,char *argv[]) 
{
	char s1[]="abc";
	char s2[]="abc";
	strcpy(s1,s2);
	
	return 0;
}
```

# 字符串查找函数
**字符串中找字符**
char* strchr(const char*s,int c);
char* strrchr(const char*s,int c);
返回NULL，表示没有找到 
实例1: 查找第一个l


```cpp
#include <stdio.h>
#include <string.h>
int main(int argc,char *argv[]) 
{
	char s[]="hello";
	char *p=strchr(s,'l');
	printf("%s\n",p);
	
	return 0;
}

//result:llo
```

实例2：查找第二个字母l

```cpp
#include <stdio.h>
#include <string.h>
int main(int argc,char *argv[]) 
{
	char s[]="hello";
	char *p=strchr(s,'l');	//此时p=llo 
	p=strchr(p+1,'l');	//查找第二个l，使用p+1 
	printf("%s\n",p);
	
	return 0;
}

//result:lo
```

实例3：从右边查找l

```cpp
#include <stdio.h>
#include <string.h>
/*

*/

int main(int argc,char *argv[]) 
{
	char s[]="hello";
	char *p=strrchr(s,'l');	//此时p=llo 
	//p=strchr(p+1,'l');	//查找第二个l，使用p+1 
	printf("%s\n",p);
	
	return 0;
}

//result:lo
```

实例4：将p中的内容拷贝到t

```cpp
#include <stdio.h>
#include <string.h>
int main(int argc,char *argv[]) 
{
	char s[]="hello";
	char *p=strchr(s,'l');	//此时p=llo 
	char *t=(char*)malloc(strlen(p)+1);
	strcpy(t,p);
	printf("%s\n",t);
	free(t);
	return 0;
}

//result:llo
```

实例5：

```cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*

*/

int main(int argc,char *argv[]) 
{
	char s[]="hello";
	char *p=strchr(s,'l');	//此时p=llo 
	char c=*p;	//先把p给c 
	*p='\0';	//0赋给p，这样s数组将会到第一个0结束，实际上原s=he\0lo 
	char *t=(char*)malloc(strlen(s)+1);
	strcpy(t,s);
	printf("%s\n",t);
	*p=c;	//再把c赋给p，就可以恢复s 
	printf("%s\n",s);
	free(t);
	return 0;
}

//result:he hello
```

**字符串中找字符串**


char * strstr(constchar *s1, constchar *s2); //在s1中查找s2，如果找到返回首个s2的首地址 
char * strcasestr(constchar *s1, constchar *s2); //在s1中查找s2，不区分大小写，找到则返回首个s2的首地址
# strcat：连接两个字符串

```cpp
#include<stdio.h>
    char *mycat(char *dest,const char *str)
    {
       while(*dest!='\0'){
       dest++;}
       while(*dest++=*str++)
       {;}
       return *dest;
     }
     int main(void)
    {
       char a[20]="hello ";
       char b[]="world";
       mycat(a,b);
       printf("%s\n",a);
       return 0;
```

```cpp
#include <stdio.h>
    #include <stdlib.h>
    char *mycpy(char *a, char *b);
    int main()
    {
        char a[20] = "abc", b[] = "def";
        mycpy(a,b);
        printf("%s", a);
        return 0;
    }
    char *mycpy(char *a, char *b)
    {
        while (*a)
        {
            a++;
        }
         while(*a++=*b++)
        {
           ;
        }
        return a;
    }
```






