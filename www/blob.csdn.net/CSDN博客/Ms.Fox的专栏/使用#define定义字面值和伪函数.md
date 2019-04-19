# 使用#define定义字面值和伪函数 - Ms.Fox的专栏 - CSDN博客
2015年01月20日 11:18:13[lulu-lu](https://me.csdn.net/smbluesky)阅读数：1019
#define是C语言提供的宏定义命令，能够为程序员提供一定方便，在一定程度上提高程序的执行效率。
在程序中，有效范围为宏定义之后的本源文件结束，在此文件范围内有效。另外，用#undef可以终止宏定义的作用域，可灵活控制宏定义的作用范围。
（1）#define定义的常量无数据类型，只是简单的字符替换，没了类型的安全检查，且有时还会产生边际效应。
e.g：#define N100
         #define M 200+N
         当程序中使用M*N时，期望的结果是100*（200+N），实际结果是100*200+N
        const常量就没有这些问题。
（2）可以对const进行调试，但无法对#define进行调试
（3）const作用域仅限于局部变量的函数体，但#define是从定义点到整个程序的结束点。不过可用#undef取消其定义，从而限定其作用域范围。
（4）带参数的宏定义形式为：
> 
 #define <宏名>（<参数表>）<宏体>
#define MAX(x,y) (x)>(y)?(x):(y)
注意：在MAX（x，y）中不应该有空格，如果MAX(x，y)，则最终替换的字符为a=MAX(x，y)，将被展开为a=(x,y) (x)>(y)?(x):(y)，问题就来了。
(5)使用do{}while（false）将宏定义包含的多条表达式放到大括号中
             使用宏时，如果宏包含多个语句，一定要用大括号把宏括起来，以防在某些情况下，宏定义的多条语句只有一条语句被执行。
（6）关于…的使用
#define myprintf(templt，…)  fprintf（stderr,templt, _VA_ARGS_）     //没有对变参起名，用默认的宏_VA_ARGS_来代替它
#define myprintf(templt，args…)  fprintf（stderr,templt, args）         
  //显示的命名变参为args，那么在宏定义中就可以用args来代替变参，变参必须作为参数表的最后一项出现
myprintf("Error!") ； 被替换成 fprintf（stderr,"Error!"，）；多了个逗号，会出现编译错误
这时，就需要##了。##这个连接符号的作用是当_VA_ARGS_为空时，能够消除前门的逗号，这样就不会由语法错误啦。
> 
#define myprintf(templt，…)  fprintf（stderr,templt，## _VA_ARGS_）
myprintf("Error!") ； 被替换成 fprintf（stderr,"Error!"）；
