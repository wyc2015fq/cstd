
# YACC 实例分析 - Augusdi的专栏 - CSDN博客


2017年03月20日 16:19:39[Augusdi](https://me.csdn.net/Augusdi)阅读数：4758


﻿﻿
本文例子来自于 <<lex & yacc >> 第二版
LEX 负责词法分析，每次解析出一个 token。
一、 token 的类型和值
token 具有类型，在计算器例子中，包括如下类型：
1）、 NUMBER     一串数字
2）、 NAME         一个名称
3）、 '+', '-', '*', '/' 等符号
同时 token 具有值，不同类型的 token， 值的含义不一样，例如，
'1000'：   类型是 NUMBER，值是1000
'abc'：     类型是 NAME， 值是 'abc'
LEX 解析出一个 token 后，将此 token 的值，保存在 yylval 变量中， 并将类型返回给 YACC。
为了能保存不同类型的值， yylval  被定义成 union

```python
%union {
  double dval;
  struct symtab *symp;
}
```
其中， dval 保存 NUMBER 类型的值，symp 保存 NAME 类型的值。
为了保存 NAME 类型的值，定义了一个结构
```python
struct symtab {
    char *name;
    double (*funcptr)();
    double value;
}
```
其中 name 记录了“符号”的名称，而 value 则用于保存计算结果，后文再介绍。
NAME 类型的 token，又被称为 “符号”， 跟我们写程序的时候定义的变量作用相同。
因此，当 LEX遇到数字串的时候，就把数字串的值保存到 yylval 的 dval 中，并返回 NUMBER 类型。遇到字符串的时候，根据字符串名称生成 symtab 结构，保存其名称，并将结构的地址保存到 yylval 的 symp 中，并返回 NAME 类型。遇到 '+', '-' 等符号的时候，则返回该符号的 ascii 码值。
二、 YACC 中的计算
在“产生式” 或者“规则”部分，通过 $1, $2, $3 的方式，可获取对应 token 的值，对这些变量的访问，实际就是对 yylval 的访问。此时，YACC 已经知道相应 token 的类型了，因此对 NUMBER 类型，token 的值就是 yylval.dval， 对 NAME 类型， token 的值就是  yylval.symp
例如：
```python
statement:  NAME '=' expression { $1->value = $3;  printf("(%s) = (%g)\n", $1->name, $1->value); }<br> 
expression:  
|   NUMBER          { $$ = $1;   }
|   NAME            { $$ = $1->value; }
```
对于 NUMBER， $1 对应的就是数值
对于 NAME， $1->name 就是符号名称
非终结符号的类型和值
```python
%type <dval> expression
```
三、 附录： 源码
symbol.h
```python
#define NSYMS 1024  /* maximum number of symbols */ 
struct symtab {
    char *name;
    double (*funcptr)();
    double value;
} symtab[NSYMS];
 
struct symtab *symlook(char* s);
```
symbol.c
```python
#include "symbol.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
struct symtab *symlook(char* s)
{
    char *p;
    struct symtab *sp;
    for(sp = symtab; sp < &symtab[NSYMS]; sp++) {
        /* is it already here? */
        if(sp->name && !strcmp(sp->name, s)) {
            printf("found symbol: (%s)\n", sp->name);
            return sp;
        }
        /* is it free */
        if(!sp->name) {
            sp->name = strdup(s);
            printf("add symbol: (%s)\n", s);
            return sp;
        }
        /* otherwise continue to next */
    }
    yyerror("Too many symbols");
    exit(1);    /* cannot continue */
} /* symlook */
```
calc.l
```python
%{
#include "y.tab.h"
#include "symbol.h"
#include <math.h>
%}
%%
([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {
        yylval.dval = atof(yytext);
        return NUMBER;
    }
[ \t]   ;        /* ignore white space */
[A-Za-z][A-Za-z0-9]*    {   /* return symbol pointer */
        struct symtab *sp = symlook(yytext);
        yylval.symp = sp;
        return NAME;
    }
"$" { return 0; }
\n  |
.   return yytext[0];
%%
```
calc.y
```python
%{
#include <string.h>
#include <math.h>
#include "symbol.h"
%}
%union {
    double dval;
    struct symtab *symp;
}
%token <symp> NAME
%token <dval> NUMBER
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS
%type <dval> expression
%%
statement_list: statement '\n'
    |   statement_list statement '\n'
    ;
statement:  NAME '=' expression { $1->value = $3;  printf("(%s) = (%g)\n", $1->name, $1->value); }
    |   expression      { printf("= %g\n", $1); } 
    ;
expression: expression '+' expression { $$ = $1 + $3; }
    |   expression '-' expression { $$ = $1 - $3; }
    |   expression '*' expression { $$ = $1 * $3; }
    |   expression '/' expression
                {   if($3 == 0.0)
                        yyerror("divide by zero");
                    else
                        $$ = $1 / $3;
                }
    |   '-' expression %prec UMINUS { $$ = -$2; }
    |   '(' expression ')'  { $$ = $2; }
    |   NUMBER          { $$ = $1;   }
    |   NAME            { $$ = $1->value; }
    |   NAME '(' expression ')' {
            if($1->funcptr)
                $$ = ($1->funcptr)($3);
            else {
                printf("%s not a function\n", $1->name);
                $$ = 0.0;
            }
        }
    ;
%%
```


