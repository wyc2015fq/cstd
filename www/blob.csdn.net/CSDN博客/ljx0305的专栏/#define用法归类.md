# #define用法归类 - ljx0305的专栏 - CSDN博客
2011年04月22日 16:23:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：594标签：[c																[联想																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=联想&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C++																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
常规用法不再介绍，做如下几点说明和介绍
1. 带参数的宏只完成简单字符替换，之前不做计算实参的工作，如下
#define SUM(x,y) x+y
int a=3,b=2,c=1;
int s;
s=SUM(a+b,b)*SUM(c,b)+c;
            结果应该是 s=a+b+b*c+b+c=10。
2. define中的特殊标识符
#define Conn(x,y) x##y
#define ToChar(x) [#@x](mailto:#@x)
#define ToString(x) #x
int a=Conn(12,34);
char b=ToChar(a);
char c[]=ToString(a);
结果是 a=1234,c='a',c="1234";
可以看出 ## 是简单的连接符，#@用来给参数加单引号，#用来给参数加双引号即转成字符串。
更神奇的是
#define x(s) #s
   char *s = x(a   b/**/ c);
char *p = x(a/nb);结果是*s="a  b  c",*p="a//nb"，#s果然很厉害
3.define的多行定义
define可以替代多行的代码
#define MACRO(arg1, arg2) do { 
stmt1; 
stmt2; 
} while(0) 关键是要在每一个换行的时候加上一个"/" 
由此联想到了C中的关于字符串的一些默认规则
char s1[]="abc"  "efg";
char s2[]="abc"
"efg";
char s3[]="ab/
c";
char s4[]="ab
c";其中只有s4会产生编译错误，s1="abcefg",s2="abcefg",s3="abc"
4. 宏调用顺序
#define A 3
#define B 4
#define AB 5
#define Conn(x,y) x##y
int a=Conn(A,B);结果是a=5;可见一个宏的参数是另一个宏，先替换外面的宏，后替换参数。即Conn(A,B)=>AB,后AB=>5
 5.解决重复定义的问题
由于头文件包含可以嵌套，那么C文件就有可能包含多次同一个头文件，就可能出现重复定义的问题的。
通过条件编译开关来避免重复包含（重复定义）
例如
#ifndef __headerfileXXX__
＃define __headerfileXXX__
…
文件内容
…
#endif
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/czsd82/archive/2007/06/07/1642101.aspx](http://blog.csdn.net/czsd82/archive/2007/06/07/1642101.aspx)
