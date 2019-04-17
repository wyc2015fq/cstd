# #define命令的一些高级用法 - xiahouzuoxin - CSDN博客





2013年07月26日 13:14:43[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：18602











***===========================================================***

***define中的三个特殊符号：#，##，#@***

**===========================================================**


- #defineConn(x,y) x##y

- #define ToChar(x)#@x

- #define ToString(x)#x



(1)x##y表示什么？表示x连接y，举例说：


- int n = Conn(123,456);/*
 结果就是n=123456;*/

- char* str = Conn("asdf","adf");/*结果就是
 str = "asdfadf";*/



（2）再来看[#@x](mailto:#@x)，其实就是给x加上单引号，结果返回是一个const
 char。举例说：



char a = ToChar(1);结果就是a='1';

做个越界试验char a = ToChar(123);结果就错了;

但是如果你的参数超过四个字符，编译器就给给你报错了！


error C2015: too many characters in constant   ：P


(3）最后看看#x,估计你也明白了，他是给x加双引号



char* str = ToString(123132);就成了str="123132";





**===========================================================**


***常用的一些宏定义***

**===========================================================**

1 防止一个头文件被重复包含 



- #ifndef BODYDEF_H 

- #define BODYDEF_H 

- //头文件内容 

- 

- #endif



2 得到指定地址上的一个字节或字


- #define MEM_B( x )(*((byte *)(x)))

- #define MEM_W( x )(*((word *)(x)))




用法如下：


- #include<iostream>

- #include<windows.h>

- 

- #define MEM_B(x)(*((byte*)(x)))

- #define MEM_W(x)(*((WORD*)(x)))

- 

- int main()

- {

- int bTest = 0x123456;

- 

- 
    byte m = MEM_B((&bTest));/*m=0x56*/

- int n = MEM_W((&bTest));/*n=0x3456*/

- 

- return 0;

- }



3 得到一个field在结构体(struct)中的偏移量


- #defineOFFSETOF( type, field )((size_t)&(( type *) 0)-> field )



4 得到一个结构体中field所占用的字节数 



- #define FSIZ( type, field )sizeof(((type *) 0)->field )



5 得到一个变量的地址（word宽度） 



- #define B_PTR( var )((byte *)(void*)&(var))

- #define W_PTR( var )((word *)(void*)&(var))



6 将一个字母转换为大写


- #define UPCASE( c )(((c)>=''a''&&(c)<=''z'')?((c)- 0x20):(c))



7 判断字符是不是10进值的数字


- #define DECCHK( c )((c)>=''0''&&(c)<=''9'')



8 判断字符是不是16进值的数字 



- #define HEXCHK( c )(((c)>=''0''&&(c)<=''9'')||((c)>=''A''&&(c)<=''F'')||((c)>=''a''&&(c)<=''f''))



9 防止溢出的一个方法


- #define INC_SAT( val )(val =((val)+1 >(val))?(val)+1 :(val))



10 返回数组元素的个数 



- #define ARR_SIZE( a )(sizeof((a))/sizeof((a[0])))



11 使用一些宏跟踪调试

在调试时，我们可以设置__DEBUG宏，也可以再Makefile中使用-D编译选项设置，


```cpp
#define __DEBUG
```


使用方法为，
```cpp
#ifdef __DEBUG
printf("%s", ...);
#endif
```




另外，ANSI C标准中有几个标准预定义宏，前面几个（func...STDC）常用于printf(sprintf)等语句中：

__func__：在源代码中插入当前所在函数名；



__LINE__：在源代码中插入当前源代码行号；


__FILE__：在源文件中插入当前源文件名；


__DATE__：在源文件中插入当前的编译日期


__TIME__：在源文件中插入当前编译时间；


__STDC__：当要求程序严格遵循ANSI C标准时该标识被赋值为1；


__cplusplus：当编写C++程序时该标识符被定义。


其中__cplusplus常用于头文件中，格式如下：



```cpp
#ifndef _ZX_FUNC_H
#define _ZX_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

/* functions */
char *strdup (const char *s);

#ifdef __cplusplus
}
#endif

#endif
```


extern"C"表示将其中的代码按照C编译方法编译，目的是实现C++与C语言的调用。

C编译与C++编译的区别是：C会将上面strdup编译成_STRDUP符号，而C++会编译成_STRDUP_CHAR，这也是C++为什么能实现函数重载的原因。extern只能出现在C++文件中，一般如上面的方式置于头文件中。

**要在C中调用C++代码，需要在C代码中的函数或变量声明为extern类型，在C++中将函数或变量用extern "C"修饰。**



12 简单数学计算（绝对值，三角函数等）

```cpp
#define ABS( a ) ( ((a)>0) ? (a) : (-(a)) )
```




13 #define 一个复杂语句


比如交换a,b的值，



```cpp
#define(a,b) do { \
    int t = 0;
    t = a; \
    a = b; \
    b = t; \
} while(0)
```






注：#define的这些高级用法在Linux源代码很多处出现，可阅读参考Linux源代码。





