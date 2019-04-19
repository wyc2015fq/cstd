# PHP 内核分析（2）：ZTS和zend_try - 文章 - 伯乐在线
原文出处： [叶剑峰](http://www.cnblogs.com/yjf512/p/6092708.html)
这里阅读的php版本为PHP-7.1.0 RC3，阅读代码的平台为linux
# ZTS
我们会看到文章中有很多地方是:


```
#ifdef ZTS
# define CG(v) ZEND_TSRMG(compiler_globals_id, zend_compiler_globals *, v)
#else
# define CG(v) (compiler_globals.v)
extern ZEND_API struct _zend_compiler_globals compiler_globals;
#endif
```
这里的ZTS是个什么概念呢。我们经常使用的php都是运行在单进程，单线程环境，比如cgi，都是一个请求进来，就一个进程为它服务，当请求结束了，进程也就结束了。所以比如像全局变量，php内核就没有考虑多线程同时修改获取的时候线程安全问题。后来，php渐渐也在往单进程多线程服务器方向发展。那么这个时候，就会需要有一个层来专门处理线程安全问题。这个就是TSRM(Thread Safe Resource Management)。
但是php默认是关闭线程安全的。在编译的时候，你可以指定参数开启编译一个线程安全版本的php。（–enable-maintainer-zts 选项， Windows 平台为 –enable-zts）这个就是这里的ZTS的由来。
比如上面的例子，CG(V) 在非线程安全下获取的是全局结构compiler_globals结构的v属性，在线程安全下获取的是通过ZEND_TSREMG方法来获取。
# zend_try
我们会看到zend_try_catch相关的代码如下：


```
zend_try {
        ...exec_try
    } zend_catch {
        ...exec_catch
    } zend_end_try();
```
把宏展开，我们可以看到大概代码如下：


```
{                                                            
        JMP_BUF *__orig_bailout = EG(bailout);                    
        JMP_BUF __bailout;                                        
                                                                
        EG(bailout) = &__bailout;                                
        if (SETJMP(__bailout)==0) {
          {
               ...exec_try
          }
        } else {                                                
            EG(bailout) = __orig_bailout;
           {
               ...exec_catch
           }
       }                                                        
       EG(bailout) = __orig_bailout;                            
}
```
这个是什么意思呢，需要先理解下setjmp和longjmp，这两个函数是linux提供的方法。他们是组合起来使用的，达到协同程序的功能


```
#include 
#include 
jmp_buf env;
void foo() {
    printf("before jmpn");
    int ret = setjmp(env);
    if(ret == 0) {
        return;
    } else {
        printf("return %dn", ret);
    }
    printf("after jmpn");
}
int main(int argc, char* argv[]) {
    foo();
    longjmp(env, 999);
    return 0;
}
// 输出：
/*
before jmp
return 999
after jmp
*/
```
上面的这个例子，setjmp的时候相当于程序片段1把主动权交出来，然后执行if(ret == 0)下面的程序，直到遇到longjmp，把执行权还给了片段1，并且设置jmp_buf为999，片段1继续执行，发现了ret!=0，就输出return 999。
好了，回到这个程序:


```
{                                                            
        JMP_BUF *__orig_bailout = EG(bailout);                    
        JMP_BUF __bailout;                                        
                                                                
        EG(bailout) = &__bailout;                                
        if (SETJMP(__bailout)==0) {
          {
               ...exec_try
          }
        } else {                                                
            EG(bailout) = __orig_bailout;
           {
               ...exec_catch
           }
       }                                                        
       EG(bailout) = __orig_bailout;                            
}
```
这个程序里面的exec_try代码段里面，在遇到错误的时候，需要返回的时候，就会包含一个longjmp函数的调用。这样，就形成了我们平时调用try…catch…finnal的功能：
1 先保存全局变量里面的bailout
2 使用setjmp来做跳转执行下面的程序
3 执行exec_try
4 如果exec_try这个代码段里面有longjmp，并且longjmp返回非0（一般也确实非0），就执行exec_catch
5 最后，把全局变量里面的bailout恢复
这里可能会有两个疑惑，如果exec_try里面没有longjmp怎么办，那就直接只执行了exec_try，就跳过exec_catch了。这个也是标准的用setjmp和longjmp实现try catch的写法。
这两个的实现弥补了goto关键字只能在函数内部进行跳转的限制。这个叫做“长跳转”。
所以在PHP代码中，如果你执行的函数有可能抛出异常。不妨使用这个方式把你要执行的程序放在里面。
# 参考
[http://blog.lucode.net/skills/talk-about-setjmp-and-longjmp.html](http://blog.lucode.net/skills/talk-about-setjmp-and-longjmp.html)
