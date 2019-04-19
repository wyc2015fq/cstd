# 编译原理：利用yacc/bison进行语法分析 - Koma Hub - CSDN博客
2018年04月15日 15:01:32[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：630
YACC（Yet Another Compiler-Compiler）是一个LALR（1）分析器自动生成器，是贝尔实验室在UNIX上首次实现，与LEX有直接的接口。此外GNU（GNU is not UNIX）的Bison是对YACC的兼容性扩充。
YACC自动构造分析器的模式及作用图：
![](https://img-blog.csdn.net/20180415145129436)
其步骤如下：
![](https://img-blog.csdn.net/20180415145635234)
YACC源程序结构：
```
说明部分
%%
翻译规则
%%
用C语言编写的辅助例程
```
给出一个源程序：[https://blog.csdn.net/u014708761/article/details/50153141](https://blog.csdn.net/u014708761/article/details/50153141)
```
%{
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
%}
%token DIGIT
%%
line    :expr'\n' { printf("%d\n",$1);return;}
        ;
expr    :expr'+'term { $$=$1+$3;}
		|expr'-'term { $$=$1-$3;}
		|term
		;
term	:term'*'factor {$$=$1*$3;}
		|factor
		;
factor	:'('expr')' {$$=$2;}
        |DIGIT
        ;
%%
main(){
    return yyparse();
}
int yylex(){
    int c;
    while ((c=getchar())==' ');
    if(isdigit(c)){
        yylval=c-'0';
        return DIGIT;
    }
    return c;
}
int yyerror(char *s){
    fprintf(stderr,"%s\n",s);
    return 1;
}
```
我的运行环境是cygwin下的bison或yacc：
```
Toa@DESKTOP-499IG24 ~
$ yacc --version
bison (GNU Bison) 3.0.4
由 Robert Corbett 和 Richard Stallman 编写。
版权所有 (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
Toa@DESKTOP-499IG24 ~
$ bison --version
bison (GNU Bison) 3.0.4
由 Robert Corbett 和 Richard Stallman 编写。
版权所有 (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
编译与运行：
```
Toa@DESKTOP-499IG24 ~
$ yacc a.y
Toa@DESKTOP-499IG24 ~
$ gcc y.tab.c -w
Toa@DESKTOP-499IG24 ~
$ ./a.exe
3+4*2*(2+3)
43
```
