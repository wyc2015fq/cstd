# linux C 正则表达式 - ljx0305的专栏 - CSDN博客
2012年11月08日 11:49:41[ljx0305](https://me.csdn.net/ljx0305)阅读数：1115
最近有这么个需求,c搞搞正则,毕竟Regular Expression so强大^_^
标准的C和C++都不支持正则表达式，但有一些函数库可以辅助C/C++程序员完成这一功能，其中最著名的当数Philip
 Hazel的Perl-Compatible Regular Expression库，许多Linux发行版本都带有这个函数库。
编译正则表达式
为了提高效率，在将一个字符串与正则表达式进行比较之前，首先要用regcomp()函数对它进行编译，将其转化为regex_t结构：
int regcomp(regex_t *preg,constchar*regex,int cflags);
参数regex是一个字符串，它代表将要被编译的正则表达式；参数preg指向一个声明为regex_t的数据结构，用来保存编译结果；参数cflags决定了正则表达式该如何被处理的细节。
如果函数regcomp()执行成功，并且编译结果被正确填充到preg中后，函数将返回0，任何其它的返回结果都代表有某种错误产生。
匹配正则表达式
一旦用regcomp()函数成功地编译了正则表达式，接下来就可以调用regexec()函数完成模式匹配：
int regexec(const regex_t *preg,constchar*string,size_t nmatch,regmatch_t
 pmatch[],int eflags);
typedefstruct{
   regoff_t rm_so;
   regoff_t rm_eo;
} regmatch_t;
参数preg指向编译后的正则表达式，参数string是将要进行匹配的字符串，而参数nmatch和pmatch则用于把匹配结果返回给调用程序，最后一个参数eflags决定了匹配的细节。
在调用函数regexec()进行模式匹配的过程中，可能在字符串string中会有多处与给定的正则表达式相匹配，参数pmatch就是用来保存这些匹配位置的，而参数nmatch则告诉函数regexec()最多可以把多少个匹配结果填充到pmatch数组中。当regexec()函数成功返回时，从 string+pmatch[0].rm_so到string+pmatch[0].rm_eo是第一个匹配的字符串，而从 string+pmatch[1].rm_so到string+pmatch[1].rm_eo，则是第二个匹配的字符串，依此类推。
释放正则表达式
无论什么时候，当不再需要已经编译过的正则表达式时，都应该调用函数regfree()将其释放，以免产生内存泄漏。
void regfree(regex_t *preg);
函数regfree()不会返回任何结果，它仅接收一个指向regex_t数据类型的指针，这是之前调用regcomp()函数所得到的编译结果。
如果在程序中针对同一个regex_t结构调用了多次regcomp()函数，POSIX标准并没有规定是否每次都必须调用regfree()函数进行释放，但建议每次调用regcomp()函数对正则表达式进行编译后都调用一次regfree()函数，以尽早释放占用的存储空间。
报告错误信息
如果调用函数regcomp()或regexec()得到的是一个非0的返回值，则表明在对正则表达式的处理过程中出现了某种错误，此时可以通过调用函数regerror()得到详细的错误信息。
size_t regerror(int errcode,const regex_t *preg,char*errbuf,size_terrbuf_size);
参数errcode是来自函数regcomp()或regexec()的错误代码，而参数preg则是由函数regcomp()得到的编译结果，其目的是把格式化消息所必须的上下文提供给regerror()函数。在执行函数regerror()时，将按照参数errbuf_size指明的最大字节数，在
 errbuf缓冲区中填入格式化后的错误信息，同时返回错误信息的长度。
应用正则表达式
最后给出一个具体的实例，介绍如何在C语言程序中处理正则表达式。 
regexp.c:
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<regex.h>
/* 取子串的函数 */
staticchar*
substr (constchar*str,unsigned start,unsigned end)
{
unsigned n = end - start;
staticchar stbuf[256];
strncpy(stbuf, str + start, n);
  stbuf[n]= 0;
return stbuf;
}
/* 主程序 */
int
main (int argc,char**argv)
{
char*pattern;
int x, z, lno = 0, cflags = 0;
char ebuf[128], lbuf[256];
  regex_t reg;
  regmatch_t pm[10];
constsize_t nmatch = 10;
  pattern = argv[1];
  z = regcomp (&reg, pattern, cflags);
if(z != 0)
{
      regerror (z,&reg, ebuf,sizeof(ebuf));
fprintf(stderr,"%s: pattern '%s' \n", ebuf, pattern);
return 1;
}
/* 逐行处理输入的数据 */
while(fgets(lbuf,sizeof(lbuf),stdin))
{
++lno;
if((z =strlen(lbuf))> 0 && lbuf[z - 1]=='\n')
    lbuf[z - 1]= 0;
/*对每一行应用正则表达式进行匹配 */
      z = regexec (&reg, lbuf, nmatch, pm, 0);
if(z == REG_NOMATCH)
continue;
elseif(z != 0)
{
     regerror (z,&reg, ebuf,sizeof(ebuf));
fprintf(stderr,"%s: regcom('%s')\n", ebuf, lbuf);
return 2;
}
/* 输出处理结果 */
for(x = 0; x < nmatch && pm[x].rm_so !=-1;++x)
{
if(!x)
printf("%04d: %s\n", lno, lbuf);
printf(" $%d='%s'\n", x, substr (lbuf, pm[x].rm_so, pm[x].rm_eo));
}
}
/* 释放正则表达式 */
  regfree (&reg);
return 0;
}
Makefile:
CC = gcc 
CFLAGS =-g -Wall 
TARGET = regexp
EXECOBJS = ${TARGET}.o
$(TARGET): $(EXECOBJS)
             $(CC) $(CFLAGS)-o $(TARGET) $(EXECOBJS)
%.o:%.c
    $(CC) $(CFLAGS)-o $@ -c
 $<
clean:
    rm -rf *.o *.a *.so
 $(TARGET)
[root@zj:~/C_parm/regexp]# make
gcc -g -Wall -o regexp.o -c regexp.c 
gcc -g -Wall -o regexp regexp.o 
[root@zj:~/C_parm/regexp]#./c_regexp 'regex[a-z]*'< c_regexp.c
0004:#include<regex.h>
 $0='regex'
0023: regex_t reg;
 $0='regex'
0043: z = regexec (&reg, lbuf, nmatch, pm, 0);
 $0='regexec'
  不要说为什么一个文件还Makefile...以前我也很少用Makefile,win下的习惯!!!BS下自己,最近写linux
 code写大半年了,我就没自己敲过gcc了^_^.
其实关键是code总要不定的debug,你不能每次debug后都gcc -o ***-l -L....吧.filename没改变,我just
 make^_^
引用:[http://blog.chinaunix.net/uid-20729605-id-1884365.html](http://blog.chinaunix.net/uid-20729605-id-1884365.html)
