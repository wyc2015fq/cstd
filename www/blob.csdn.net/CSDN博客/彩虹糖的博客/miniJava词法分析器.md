# miniJava词法分析器 - 彩虹糖的博客 - CSDN博客





2018年09月28日 10:02:29[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：297








# 一.miniJava介绍

miniJava是Java语言的一个子集，其支持类，有限的继承，简单数据类型和一些控制结构。

# 二.目的

设计并实现一个包含预处理功能的词法分析程序，加深对编译中词法分析过程的理解。

# 三.实验步骤

### （1）预处理

实现一个预处理程序，其可以打开一个包含源程序的文本文件（比如txt格式），并把对源程序没有意义的符号去掉。

首先编制一个源程序的输入过程，从键盘、文件或文本框输入若干行语句，依次存入输入缓冲区（字符型数据）；然后编制一个预处理子程序，去掉输入串中的回车符、换行符和跳格符等编辑性文字；把多个空白符合并为一个；去掉注释。

### （2）输入与输出

输入：所给文法的源程序字符串。 

输出：二元组（syn,token）构成的序列。其中， 

syn为单词种别码。 

Token为存放的单词自身字符串（如果是整型或浮点数，也用字符串储存）。

### （3）词法

**关键字：**

boolean class else extends false if int length main new public return static this true void while String System.out.println

**运算符和界符：**

operator = ’=’|’+’|’-’|’*’|’/’|"&&"|"||"|’!’|"=="|"!="|’<’|"<="|’>’|">="

delimiter = ’;’|’,’|’.’|’(’|’)’|’[’|’]’|’{’|’}’

**标识符：**

与Java标识符规则相同，应以字母、下划线、美元符开头，后跟字母、下划线、美元符或数字 ，Java标识符大小写敏感，长度无限制。不能与关键字相同。

**字符串：**

要表示的字符串应该在双引号" "之间，字符串的每一个元素应该都是ASCII码表中的内容，但不包括双引号"（这里考不考虑引进转义符？）

**注释：**

注释与Java要求相同，有两种形式一种是//之后是注释，换行代表结束，一种是在/* */之间的为注释。

**整型和浮点型要求与Java要求相同 **

### (4) 码表
|单词符号|种别码|单词符号|种别码|
|----|----|----|----|
|boolean|1|==|30|
|class|2|!=|31|
|else|3|<|32|
|extends|4|<=|33|
|false|5|>|34|
|if|6|>=|35|
|int|7|;|36|
|length|8|,|37|
|main|9|.|38|
|new|10|(|39|
|public|11|)|40|
|return|12|[|41|
|static|13|]|42|
|this|14|{|43|
|true|15|}|44|
|void|16|字符串|45|
|while|17|整数|46|
|String|18|浮点数|47|
|println|19|||
|double|20|||
|int|21|||
|=|22|||
|+|23|||
|-|24|||
|*|25|||
|/|26|||
|&&|27|||
||||28|||
|!|29|||

（5）正规式

letter: a | b | c | d | e | … | z | A | B | C | D | E | … | Z

digit: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

identifier: ( letter | _ | $ ) ( letter | digit | _ | $ )*

integer: ( 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 ) digit*

double: ( 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 ) digit*  . digit* ( 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 )

​ | 0 . digit* ( 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 )

(6) 文法分析

Program -> ClassDecl {ClassDecl}

ClassDecl -> "class" <ID> ["extends" <ID>] ’{’ {VarDecl} {MethodDecl} ’}’ 

MethodDecl -> "public" Type <ID> ’(’ [Formals] ’)’ ’{’ {VarDecl} {Statement} ’}’ 

                         | "public" "void" <ID> ’(’ [Formals] ’)’ ’{’ {VarDecl} {Statement} ’}’ 

                         | "public" "static" "void" "main" ’(’ "String" ’[’ ’]’ <ID> ’)’ ’{’ {VarDecl} {Statement} ’}’ 

Formals -> Type <ID> {’,’ Type <ID>}

VarDecl -> Type <ID> [’=’ FullExpr] ’;’ 

Type -> ElmType [’[’ ’]’]

ElmType -> "boolean" | "int" | <ID>

Statement -> ’{’ {Statement} ’}’ 

      | Lvalue ’=’ FullExpr ’;’ | Lvalue ’(’ [Params] ’)’’;’

      | "if" ’(’ Expr ’)’ Statement ["else" Statement] 

      | "while" ’(’ Expr ’)’ Statement 

      | "System.out.println" ’(’ [PrintArg] ’)’’;’

      | "return" [Expr] ’;’

Params -> Expr {’,’ Expr}

PrintArg -> Expr | <STR>

FullExpr -> "new" ElmType ’[’ <INT> ’]’ 

 | "new" <ID> ’(’ [Params] ’)’

 | Expr 

Expr -> Expr Binop Expr | Unop Expr | ’(’ Expr ’)’ | Lvalue ’(’ [Params] ’)’ | Lvalue ’.’ "length" ’(’ ’)’ | Lvalue | Literal

Lvalue -> [ "this" ’.’ ] <ID> {Deref} 

Deref -> ’[’ Expr ’]’ | ’.’ <ID> Literal -> <INT> | "true" | "false" 

Binop -> ’+’ |’-’ |’*’ |’/’ |"&&" |"||" |"==" |"!=" |’<’ |"<=" |’>’ |">=" 

Unop -> ’-’ | ’!’

参考：

[C语言源程序词法分析器（Java实现）](https://blog.csdn.net/sinat_37976731/article/details/78516527)

[miniJava](http://web.cecs.pdx.edu/~herb/cs322s10/mini-manual-v14.pdf)



