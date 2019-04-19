# 编译原理：用lex/flex做词法分析 - Koma Hub - CSDN博客
2018年04月15日 11:55:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：370
最近在自学《编译原理》，感觉对于我来说有点难度。
写这个的目的是为了做笔记，感谢[https://blog.csdn.net/xiaowei_cqu/article/details/7760927](https://blog.csdn.net/xiaowei_cqu/article/details/7760927)这篇文章的原创作者，接下来的内容也是这篇文章里的内容，如有侵权请联系删除。（只是为了做个笔记）
**Lex语言格式：**
```
说明部分 /*包含模式宏定义和C语言的说明信息*/
%%
规则部分 /*转换识别*/
%%
用户代码 /*规则动作部分所需的辅助过程的C代码*/
```
**状态转换图**如下：
![](https://img-blog.csdn.net/20180415103826341)
**运行环境：**直接在cygwin里面安装了flex/lex来运行的，感觉还不错。
```
$ lex --version
lex 2.6.4
$ gcc --version
gcc (GCC) 6.4.0
```
**1.编写一个Lex程序。该程序拷贝一个文件，并将文件中的每个非空的空白符序列替换为单个空格**
lex程序：
```cpp
%{
%}
%%
[ \t]+ {printf(" ");}//表示遇到连续多个空白符（或制表符）,则替换为一个空格符,编译时请删除此注释
\n|.   {printf("%s",yytext);}//表示其余的语句均照常打印出来,编译时请删除此注释
%%
#pragma comment(lib,"y1.lib")
int main(void)
{
    yyin=stdin;
    return yylex();
}
```
编译后生成的lex.yy.c过长，此处不做展示。进行操作的输入文件如下：
```
My name is RT.       space
UPC       University.
Hello     world     !
```
整体的编译运行结果：
```
Toa@DESKTOP-499IG24 ~
$ lex a.l
Toa@DESKTOP-499IG24 ~
$ gcc lex.yy.c -lfl
Toa@DESKTOP-499IG24 ~
$ ./a.exe < a.txt
My name is RT. space
UPC University.
Hello world !
```
可以看出，词法分析的结果中删除了多余的空格。
**2.编写一个Lex程序。该程序拷贝一个C文件，并将程序中的关键字float的每个实例替换成double**
lex程序：
```cpp
%{
%}
%%
//表示如果是在双引号(")中（即为字符串）,则照常打印,编译时请删除此注释
\".*\"  {printf("%s",yytext);}
[^ \t\n]+ {printf("%s",yytext);}
//表示如果遇到float,且附加模式是后面跟有空白符,则将float替换为double,编译时请删除此注释
float/[ \t]+ {printf("double");}
\n|. {printf("%s",yytext);}
%%
#pragma comment(lib,"y1.lib")
int main(void)
{
    yyin=stdin;
    return yylex();
}
```
测试用的a.txt文件内容：
```
My name is RT.       space
float UPC       University.
Hello     world     !
float a = 12345.0;
int b = 10;
```
整体的编译与运行：
```
Toa@DESKTOP-499IG24 ~
$ lex a.l
Toa@DESKTOP-499IG24 ~
$ gcc lex.yy.c -lfl
Toa@DESKTOP-499IG24 ~
$ ./a.exe < a.txt
My name is RT.       space
double UPC       University.
Hello     world     !
double a = 12345.0;
int b = 10;
```
**3.识别字符串[https://blog.csdn.net/u014708761/article/details/49725343](https://blog.csdn.net/u014708761/article/details/49725343)**
```cpp
%{
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
%}
DIGIT [0-9]
ID [a-z][a-z0-9]*
%%
{DIGIT}+                    {printf("整数:   %s(%d)\n",yytext,atoi(yytext));}
{DIGIT}+"."{DIGIT}+             {printf("实数:   %s(%g)\n",yytext,atof(yytext));}
if|then|begin|end|program|while|repeat      {printf("关键字: %s\n",yytext);}
{ID}                        {printf("标识符: %s\n",yytext);}
"+"|"-"|"*"|"/"                 {printf("运算符: %s\n",yytext);}
"{"[^}\n]*"}";
[\t\n\x20]+;
.                       {printf("不能识别的字符:%s\n",yytext);}
%%
int main(int argc,char **argv)
{
    ++argv;
    --argc;
    if(argc>0) yyin=fopen(argv[0],"r");
    else yyin=stdin;
    yylex();
    return 0;
}
int yywrap()
{
    return 1;
}
```
输入的文本文件：
```
aword
bword
cword
awor
3.212
123123
234
```
编译与运行：
```
Toa@DESKTOP-499IG24 ~
$ lex a.l
Toa@DESKTOP-499IG24 ~
$ gcc lex.yy.c -lfl
Toa@DESKTOP-499IG24 ~
$ ./a.exe < a.txt
标识符: aword
标识符: bword
标识符: cword
标识符: awor
实数:   3.212(3.212)
整数:   123123(123123)
整数:   234(234)
```
