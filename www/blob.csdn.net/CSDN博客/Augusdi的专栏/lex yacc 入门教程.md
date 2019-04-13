
# lex yacc 入门教程 - Augusdi的专栏 - CSDN博客


2017年03月20日 17:07:47[Augusdi](https://me.csdn.net/Augusdi)阅读数：1903


### 声明：原创作品，转载注明出处
### [http://www.cnblogs.com/vestinfo/](http://www.cnblogs.com/vestinfo/)
### 一、简介
推荐书籍《flex&bison》.
在UNIX下是flex和bison.网上介绍很多，大部分是写给懂的人看的，初学者一头雾水。这样来理解lex和yacc可能容易些：在linux下，有很多系统配置文件,一些linux下的软件也有配置文件，那么程序是如何读取配置文件中的信息的呢？
首先用到lex词法分析器，读取配置文件中的关键词（后面说到的token标记其实可看做关键词）。然后把关键词
递交给yacc，yacc对一些关键词进行匹配，看是否符合一定语法逻辑，如果符合就进行相应动作。
上面举得例子是分析配置文件内容的，当然可分析其他文件内容。

### 二、一个简单的lex文件例子
**1、来看flex&bison这本书开篇给出的例子：输入几行字符串，输出行数，单词数和字符的个数。**
**关于yylex即lex中相关变量系列3文章介绍。**

```python
/* just like Unix wc */
%{
int chars = 0;
int words = 0;
int lines = 0;
%}
%%
[a-zA-Z]+  { words++; chars += strlen(yytext); }
\n         { chars++; lines++; }
.          { chars++; }
%%
main(int argc, char **argv)
{
  yylex();
  printf("%8d%8d%8d\n", lines, words, chars);
}
```
**2、按照下面过程编译。**
\#flex test.l
\#gcc lex.yy.c –lfl
\#./a.out
![image9](http://images.cnblogs.com/cnblogs_com/vestinfo/201209/201209292129078898.png)
3、分析这个简单的lex文件：
（1）%%把文件分为3段，第一段是c和lex的全局声明，第二段是规则段，第三段是c代码。
（2）第一段的c代码要用%{和%}括起来，第三段的c代码不用。
（3）第二段规则段，[a-zA-Z]+  \n   . 是正则表达式，{}内的是c编写的动作。
**关于正则表达式系列3文章介绍。**
4、如果不用-lfl选项，代码可以为下面这样（具体原因见lex的库和函数分析）：

```python
int chars = 0;
int words = 0;
int lines = 0;
int yywrap();
%}
%%
[a-zA-Z]+  { words++; chars += strlen(yytext); }
\n         { chars++; lines++; }
.          { chars++; }
%%
main(int argc, char **argv)
{
  yylex();
  printf("%8d%8d%8d\n", lines, words, chars);
}
int yywrap()
{
	return 1;
}
```
三、修改第一个例子，将正则表达式放在全局声明中

```python
%{
int chars = 0;
int words = 0;
int lines = 0;
%}
mywords	[a-zA-Z]+ 
mylines	\n 
mychars	.  
%%
{mywords}  { words++; chars += strlen(yytext); }
{mylines}  { chars++; lines++; }
{mychars}  { chars++; }
%%
main(int argc, char **argv)
{
  yylex();
  printf("%8d%8d%8d\n", lines, words, chars);
}
```
编译一同上。
### 四、The Scanner as Coroutine（协同程序）
即怎样将扫描到的标记给其他程序使用，下面的例子，希望扫描到+ 或 -时做一个特殊输出。
**当调用yylex时，若扫描到return对应的标记时，yylex返回，且值就为return后的值；**
**若没扫描到return对应的标记，yylex继续执行，不返回。**
下次调用自动从前一次的扫描位置处开始。

```python
%{
enum yytokentype {
	ADD = 259,
	SUB = 260, 
};
%}
myadd	"+"
mysub	"-"
myother	.
%%
{myadd}    { return ADD; }
{mysub}    { return SUB; }
{myother}  { printf("Mystery character\n"); }
%%
main(int argc, char **argv)
{
	int tok;
	while(tok = yylex()) {				//yylex的返回值只能是ADD 或 SUB.
		if(tok == ADD || tok == SUB) {printf("meet + or -\n");}
		else {printf("this else statement will not be printed, \
			because if yylex return,the retrun value must be ADD or SUB.");}
	}
}
```
![image21](http://images.cnblogs.com/cnblogs_com/vestinfo/201209/201209292129076980.png)

### 五、yacc —— unix下是bison
**1、yacc语法规则部分和BNF类同，先来看BNF巴克斯范式。**
（1）<> 内包含的内容为必选项；
（2）[]  内的包含的内容为可选项；
（3）{ } 内包含的为可重复0至无数次的项；
（4） | 表示在其左右两边任选一项，相当于"OR"的意思；
（5）::= 是“被定义为”的意思；
（6）双引号“”内的内容代表这些字符本身；而double _quote用来表示双引号。
（7）BNF范式举例，下面的例子用来定义java中的for语句：
FOR_STATEMENT ::=
"for" "(" ( variable_declaration |
( expression ";" ) | ";" )
[ expression ] ";"
[ expression ]
")" statement
**2、yacc语法。**

```python
result: components { /*
        action to be taken in C */ }
        ;
```
（1）components是根据规则放在一起的终端和非终端符号，后面是{}括起来的执行的动作。
**3、语法例子。**

```python
param : NAME EQ NAME { 
	printf("\tName:%s\tValue(name):%s\n", $1,$3); }			
	| NAME EQ VALUE {
	printf("\tName:%s\tValue(value):%s\n",$1,$3);}
	;
```
```python
simple_sentence: subject verb object
      |     subject verb object prep_phrase ;
subject:    NOUN
      |     PRONOUN
      |     ADJECTIVE subject ;
verb:       VERB
      |     ADVERB VERB
      |     verb VERB ;
object:     NOUN
      |     ADJECTIVE object ;
prep_phrase:     PREPOSITION NOUN ;
```
（1）理解 |  的意思，|表示左右两边任选一项，如| subject verb object prep_phrase ;中|的左边为空，
所以该句表示匹配空或者subject verb object prep_phrase ;而上面还有一句subject verb object ，
所以
simple_sentence: subject verb object
| subject verb object prep_phrase ;
的意思是匹配subject verb object 或 subject verb object prep_phrase ;
### 六、flex和bison相结合。
test.l
```python
%{  
#include "test.tab.h"  
#include <stdio.h>  
#include <stdlib.h>  
%}  
%%  
a   {return A_STATE;}  
b   {return B_STATE;}  
c   {return C_STATE;}  
not   {return NOT;}  
%%
```
test.y

```python
%{  
#include <stdio.h>  
#include <stdlib.h>  
%}  
%token  A_STATE B_STATE C_STATE NOT  
%%  
program :     
    A_STATE B_STATE {  
		printf("1");  
    }  
    c_state_not_token  {  
		printf("2");  
	}  
    |    NOT {   
		printf("3");  
    }  
c_state_not_token : C_STATE {}  
%% 
yyerror(const char *s)
{
	fprintf(stderr, "error: %s\n", s);
} 
int main()
{
	yyparse();
	return 0;
}
```
编译：
![image](http://images.cnblogs.com/cnblogs_com/vestinfo/201209/201209292123064241.png)

### 七、文件信息分析。
**tset.l分析test.txt文件中的关键词（即test.y中的token标记），遇到token返回给test.y，test.y判断**
**是否符合一定语法，符合则进行相应动作。**
test.l
```python
%{
#include "test.tab.h"
#include <stdio.h>
#include <string.h>
%}
char [A-Za-z]
num [0-9]
eq [=]
name {char}+
age {num}+
%%
{name}		{ yylval = strdup(yytext); return NAME; }
{eq} 		{ return EQ; }
{age} 		{ yylval = strdup(yytext); return AGE; }
%%
int yywrap()
{
	return 1;
}
```
test.y
```python
%{
#include <stdio.h>  
#include <stdlib.h> 
typedef char* string;
#define YYSTYPE string
%}
%token NAME EQ AGE
%%
file : record file
    | record
;
record : NAME EQ AGE {
                printf("%s is %s years old!!!\n", $1, $3); }
;
%%
int main()
{
    extern FILE* yyin;
    if(!(yyin = fopen("test.txt", "r")))
    {
        perror("cannot open parsefile:");
        return -1;
    }    
    yyparse();
    fclose(yyin);
    return 0;
}
int yyerror(char *msg)
{
    printf("Error encountered: %s \n", msg);
}
```
test**.**txt
```python
ZhangSan=23
LiSi=34
WangWu=43
```
编译
![image](http://images.cnblogs.com/cnblogs_com/vestinfo/201209/201209292123079847.png)
### 八、
### token定义的标记的类型及union的使用。
token定义的标记的类型默认为int 且 默认赋值从258开始。如上面的例子，在生成的头文件
test.tab.h中有如下预编译，
/* Tokens.  */
\#ifndef YYTOKENTYPE
\# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     EQ = 259,
     AGE = 260
   };
\#endif如果想将token标记定义为其他类型呢？首先将类型定义在联合中，
%union {
   char *str;
   int  num;
   struct { int num1; int num2; } dnum;
}然后，如下定义，
%token <str> K_HOST K_ERROR
%token <str> WORD PATH STRING
%token <num> NUM 
%token <dnum> DNUM
### 补充 ：$$ $1 $2….
Each symbol in a bison rule has a value; the value of the target symbol (the one to the
left of the colon) is called 
$$
 in the action code, and the values on the right are numbered
$1, $2, and so forth, up to the number of symbols in the rule.
$$
——表示冒号的左边符号；$1——冒号右边第一个；$2——冒号右边第二个，依此类推。
如record : NAME EQ AGE { printf("%s is %s years old!!!\n", $1, $3); } ;
匹配NAME EQ AGE后，$1即NAME所表示的内容，$3即AGE所表示的内容。
[lex yacc 入门教程（3）正则表达式和lex变量及函数](http://www.cnblogs.com/helloweworld/archive/2012/09/29/2708935.html)
![image](http://images.cnblogs.com/cnblogs_com/vestinfo/201209/201209292134117612.png)![image](http://images.cnblogs.com/cnblogs_com/vestinfo/201209/20120929213411677.png)![image](http://images.cnblogs.com/cnblogs_com/vestinfo/201209/201209292134118202.png)
参考：http://www.ibm.com/developerworks/cn/linux/sdk/lex/\#resources













﻿﻿

