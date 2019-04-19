# __attribute__((noreturn)) - Koma Hub - CSDN博客
2019年02月26日 22:02:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：14
个人分类：[GNU](https://blog.csdn.net/Rong_Toa/article/category/8661294)
This attribute tells the compiler that the function won't ever return, and this can be used to suppress errors about code paths not being reached. The C library functions abort() and exit() are both declared with this attribute:
```
extern void exit(int)   __attribute__((noreturn));
extern void abort(void) __attribute__((noreturn));
```
Once tagged this way, the compiler can keep track of paths through the code and suppress errors that won't ever happen due to the flow of control never returning after the function call.
In this example, two nearly-identical C source files refer to an "exitnow()" function that never returns, but without the **__attribute__** tag, the compiler issues a warning. The compiler is correct here, because it has no way of knowing that control doesn't return.
```
$ cat test1.c
```
```cpp
extern void exitnow();
int foo(int n)
{
        if ( n > 0 )
	{
                exitnow();
		/* control never reaches this point */
	}
        else
                return 0;
}
```
```cpp
$ cc -c -Wall test1.c
test1.c: In function `foo':
test1.c:9: warning: this function may return with or without a value
```
But when we add **__attribute__**, the compiler suppresses the spurious warning:
```
$ cat test2.c
```
```cpp
extern void exitnow() __attribute__((noreturn));
int foo(int n)
{
        if ( n > 0 )
                exitnow();
        else
                return 0;
}
```
```
$ cc -c -Wall test2.c
no warnings!
```
*[http://unixwiz.net/techtips/gnu-c-attributes.html](http://unixwiz.net/techtips/gnu-c-attributes.html)*
