# setjmp 和 longjmp 函数使用详解 - 文章 - 伯乐在线
原文出处： [大米粒ing](http://blog.csdn.net/chenyiming_1990/article/details/8683413)
在网上看到的，觉得很有用，copy过来的。
非局部跳转语句—setjmp和longjmp函数。非局部指的是，这不是由普通C语言goto，语句在一个函数内实施的跳转，而是在栈上跳过若干调用帧，返回到当前函数调用路径上的某一个函数中。


```
#include <setjmp.h>
Int setjmp(jmp_buf  env);
```
返回值：若直接调用则返回0，若从longjmp调用返回则返回非0值的longjmp中的val值


```
void longjmp(jmp_buf env,int val);
```
调用此函数则返回到语句setjmp所在的地方，其中env 就是setjmp中的 env，而val 则是使setjmp的返回值变为val。
当检查到一个错误时,则以两个参数调用longjmp函数，第一个就是在调用setjmp时所用的env，第二个参数是具有非0值的val，它将成为从setjmp处返回的值。使用第二个参数的原因是对于一个setjmp可以有多个longjmp。


```
#include <stdio.h>  
#include <setjmp.h>  
   
static jmp_buf buf;  
   
void second(void) {  
    printf("second\n");         // 打印  
    longjmp(buf,1);             // 跳回setjmp的调用处 - 使得setjmp返回值为1  
}  
   
void first(void) {  
    second();  
    printf("first\n");          // 不可能执行到此行  
}  
   
int main() {     
    if ( ! setjmp(buf) ) {  
        first();                // 进入此行前，setjmp返回0  
    } else {                    // 当longjmp跳转回，setjmp返回1，因此进入此行  
        printf("main\n");       // 打印  
    }  
   
    return 0;  
}
```
上述程序将输出:


```
second
main
```
注意到虽然`first()`子程序被调用，”`first`“不可能被打印。”`main`“被打印，因为条件语句`if ( ! setjmp(buf) )`被执行第二次。
使用setjmp和longjmp要注意以下几点：
1、setjmp与longjmp结合使用时，它们必须有严格的先后执行顺序，也即先调用setjmp函数，之后再调用longjmp函数，以恢复到先前被保存的“程序执行点”。否则，如果在setjmp调用之前，执行longjmp函数，将导致程序的执行流变的不可预测，很容易导致程序崩溃而退出
2.  longjmp必须在setjmp调用之后，而且longjmp必须在setjmp的作用域之内。具体来说，在一个函数中使用setjmp来初始化一个全局标号，然后只要该函数未曾返回，那么在其它任何地方都可以通过longjmp调用来跳转到 setjmp的下一条语句执行。实际上setjmp函数将发生调用处的局部环境保存在了一个jmp_buf的结构当中，只要主调函数中对应的内存未曾释放 （函数返回时局部内存就失效了），那么在调用longjmp的时候就可以根据已保存的jmp_buf参数恢复到setjmp的地方执行。
### 异常处理
在下例中，`setjmp`被用于包住一个例外处理，类似[`try`](http://zh.wikipedia.org/w/index.php?title=Try-catch_block&action=edit&redlink=1)。`longjmp`调用类似于`throw`语句，允许一个异常返回给`setjmp`一个异常值。下属代码示例遵从[1999 ISO C standard](http://zh.wikipedia.org/wiki/C99)与[Single UNIX Specification](http://zh.wikipedia.org/wiki/Single_UNIX_Specification)：仅在特定范围内引用`setjmp`
- `if`，`switch`或它们的嵌套使用的条件表达式
- 上述情况下与`!`一起使用或者与整数常值比较
- 作为单独的语句(不使用其返回值)
遵从上述规则使得创建程序环境缓冲区更为容易。更一般的使用`setjmp`可能引起[未定义行为](http://zh.wikipedia.org/wiki/%E6%9C%AA%E5%AE%9A%E4%B9%89%E8%A1%8C%E4%B8%BA)，如破坏局部变量；编译器被要求保护或警告这些用法。但轻微的复杂用法如`switch ((exception_type = setjmp(env))) { }`在文献与实践中是常见的，并保持了相当的可移植性。


```
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <setjmp.h>  
   
void first(void);  
void second(void);  
   
/* This program's output is: 
  
calling first 
calling second 
entering second 
second failed with type 3 exception; remapping to type 1. 
first failed, exception type 1 
  
*/  
   
/* Use a file scoped static variable for the exception stack so we can access 
 * it anywhere within this translation unit. */  
static jmp_buf exception_env;  
static int exception_type;  
   
int main() {  
    void *volatile mem_buffer;  
   
    mem_buffer = NULL;  
    if (setjmp(exception_env)) {  
        /* if we get here there was an exception */  
        printf("first failed, exception type %d\n", exception_type);  
    } else {  
        /* Run code that may signal failure via longjmp. */  
        printf("calling first\n");  
        first();  
        mem_buffer = malloc(300); /* allocate a resource */  
        printf(strcpy((char*) mem_buffer, "first succeeded!")); /* ... this will not happen */  
    }  
    if (mem_buffer)  
        free((void*) mem_buffer); /* carefully deallocate resource */  
    return 0;  
}  
   
void first(void) {  
    jmp_buf my_env;  
   
    printf("calling second\n");  
    memcpy(my_env, exception_env, sizeof(jmp_buf));  
    switch (setjmp(exception_env)) {  
        case 3:  
            /* if we get here there was an exception. */  
            printf("second failed with type 3 exception; remapping to type 1.\n");  
            exception_type = 1;  
   
        default: /* fall through */  
            memcpy(exception_env, my_env, sizeof(jmp_buf)); /* restore exception stack */  
            longjmp(exception_env, exception_type); /* continue handling the exception */  
   
        case 0:  
            /* normal, desired operation */  
            second();  
            printf("second succeeded\n");  /* not reached */  
    }  
    memcpy(exception_env, my_env, sizeof(jmp_buf)); /* restore exception stack */  
}  
   
void second(void) {  
    printf("entering second\n" ); /* reached */  
    exception_type = 3;  
    longjmp(exception_env, exception_type); /* declare that the program has failed */  
    printf("leaving second\n"); /* not reached */  
}
```
