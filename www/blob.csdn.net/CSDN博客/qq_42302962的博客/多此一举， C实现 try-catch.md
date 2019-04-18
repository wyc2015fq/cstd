# 多此一举， C实现 try-catch - qq_42302962的博客 - CSDN博客
2018年05月28日 09:16:06[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：200
在做[NtyTcp](http://ntytcp/)的时候，有一些，就想用c来实现一套try-catch异常处理子系统。
不讨论C语言本身为什么不加try-catch，每个开发的朋友对于这个问题，都能说出一大堆的理由。
其实我也是不太喜欢强行在c中加入一个try-catch。就像把try-catch的原理跟自己的体会写出来。
首先我们来看看，try-catch的使用情景。
```
```cpp
try
```
```cpp
{
```
```cpp
```
```cpp
throw
```
```cpp
Excep;
```
```cpp
}
```
```cpp
catch
```
```cpp
(Excep) {
```
```cpp
} finally {
```
```cpp
}
```
```
try{ } 块是可能有异常的抛出的地方。throw Excep
catch (Excep) { } 是 捕获相应抛出异常的地方。
finally { } 是不论什么情形下，都是需要执行的代码块。
如果实现一套如此机制，有何实现的基础依赖。那就是setjmp与longjmp
讲到setjmp与longjmp，也是更说明一下
```
```cpp
#include <setjmp.h>
```
```cpp
#include <stdio.h>
```
```cpp
jmp_buf
```
```cpp
env;
```
```cpp
int
```
```cpp
count = 0;
```
```cpp
void
```
```cpp
sub_func(
```
```cpp
int
```
```cpp
idx) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"sub_func --> idx:%d\n"
```
```cpp
, idx);
```
```cpp
```
```cpp
longjmp
```
```cpp
(env, idx);
```
```cpp
}
```
```cpp
int
```
```cpp
main(
```
```cpp
int
```
```cpp
argc,
```
```cpp
char
```
```cpp
*argv[]) {
```
```cpp
```
```cpp
int
```
```cpp
idx = 0;
```
```cpp
```
```cpp
if
```
```cpp
((count =
```
```cpp
setjmp
```
```cpp
(env)) == 0) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"count:%d\n"
```
```cpp
, count);
```
```cpp
```
```cpp
sub_func(++idx);
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
if
```
```cpp
(count == 1) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"count:%d\n"
```
```cpp
, count);
```
```cpp
```
```cpp
```
```cpp
} {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"other count\n"
```
```cpp
);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
return
```
```cpp
0;
```
```cpp
}
```
```
先来定义个 
全局的jmp_buf env; 用来保存跳转的上下文。
int count = 0; 用来保存跳转返回值的。有点绕口，就是记录setjmp返回值的。
看到这里也许对setjmp与longjmp有点理解。再换个马甲，相信更有体会。
```
```cpp
#include <setjmp.h>
```
```cpp
#include <stdio.h>
```
```cpp
jmp_buf
```
```cpp
env;
```
```cpp
int
```
```cpp
count = 0;
```
```cpp
#define Try     if ((count = setjmp(env)) == 0)
```
```cpp
#define Catch(e)    else if (count == (e))
```
```cpp
#define Finally    ;
```
```cpp
#define Throw(idx)    longjmp(env, (idx))
```
```cpp
void
```
```cpp
sub_func(
```
```cpp
int
```
```cpp
idx) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"sub_func --> idx:%d\n"
```
```cpp
, idx);
```
```cpp
```
```cpp
Throw(idx);
```
```cpp
}
```
```cpp
int
```
```cpp
main(
```
```cpp
int
```
```cpp
argc,
```
```cpp
char
```
```cpp
*argv[]) {
```
```cpp
```
```cpp
int
```
```cpp
idx = 0;
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"count:%d\n"
```
```cpp
, count);
```
```cpp
```
```cpp
sub_func(++idx);
```
```cpp
```
```cpp
} Catch(1) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"count:%d\n"
```
```cpp
, count);
```
```cpp
```
```cpp
```
```cpp
} Finally {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"other count\n"
```
```cpp
);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
return
```
```cpp
0;
```
```cpp
}
```
```
try-catch就已经初具雏形了。这样只是基本实现，还有三个问题没有解决。
- 
  如何保证线程安全。
- 
 如何解决try-catch 嵌套。
- 
 如何避免，在不加try的代码块直接Throw。
如何保证线程安全。
使用线程的私有数据 pthread_key_t ，每一个线程都有一个try-catch的上下文环境。
如何解决try-catch嵌套的问题
使用一个栈式数据结构，每次try的时候压栈，每次throw的时候 出栈。
如何避免，在不加try的代码块直接Throw，
这个问题就比较好理解。既然每次try需要压栈，如果不加try，所以栈里面没有数据。在Throw之前先查看栈里面有没有数据。
那现在问题又来了，在pthread_key_t 与 链式栈如何结合？ 先看如下结构体，也许会有新的认识，结构体里面有一个prev的域。
这样就能跟pthread_key_t 来结合，每次讲结构体指针保存在pthread_key_t中，获取完之后，拿到prev的值。
```
```cpp
typedef
```
```cpp
struct
```
```cpp
_ntyExceptionFrame {
```
```cpp
```
```cpp
jmp_buf
```
```cpp
env;
```
```cpp
```
```cpp
int
```
```cpp
line;
```
```cpp
```
```cpp
const
```
```cpp
char
```
```cpp
*func;
```
```cpp
```
```cpp
const
```
```cpp
char
```
```cpp
*file;
```
```cpp
```
```cpp
ntyException *exception;
```
```cpp
```
```cpp
struct
```
```cpp
_ntyExceptionFrame *prev;
```
```cpp
```
```cpp
```
```cpp
char
```
```cpp
message[EXCEPTIN_MESSAGE_LENGTH+1];
```
```cpp
} ntyExceptionFrame;
```
```
下面把整个实现代码都贴出来，大家也可以去我的[github](http://github/)上面star，fork
```
```cpp
/*
```
```cpp
```
```cpp
* author : wangbojing
```
```cpp
```
```cpp
* email : 1989wangbojing@163.com
```
```cpp
```
```cpp
* github : https://github.com/wangbojing
```
```cpp
```
```cpp
*/
```
```cpp
#include <stdio.h>
```
```cpp
#include <string.h>
```
```cpp
#include <stdlib.h>
```
```cpp
#include <stdarg.h>
```
```cpp
#include <pthread.h>
```
```cpp
#include <setjmp.h>
```
```cpp
#define ntyThreadData      pthread_key_t
```
```cpp
#define ntyThreadDataSet(key, value)  pthread_setspecific((key), (value))
```
```cpp
#define ntyThreadDataGet(key)      pthread_getspecific((key))
```
```cpp
#define ntyThreadDataCreate(key)   pthread_key_create(&(key), NULL)
```
```cpp
#define EXCEPTIN_MESSAGE_LENGTH        512
```
```cpp
typedef
```
```cpp
struct
```
```cpp
_ntyException {
```
```cpp
```
```cpp
const
```
```cpp
char
```
```cpp
*name;
```
```cpp
} ntyException;
```
```cpp
ntyException SQLException = {
```
```cpp
"SQLException"
```
```cpp
};
```
```cpp
ntyException TimeoutException = {
```
```cpp
"TimeoutException"
```
```cpp
};
```
```cpp
ntyThreadData ExceptionStack;
```
```cpp
typedef
```
```cpp
struct
```
```cpp
_ntyExceptionFrame {
```
```cpp
```
```cpp
jmp_buf
```
```cpp
env;
```
```cpp
```
```cpp
int
```
```cpp
line;
```
```cpp
```
```cpp
const
```
```cpp
char
```
```cpp
*func;
```
```cpp
```
```cpp
const
```
```cpp
char
```
```cpp
*file;
```
```cpp
```
```cpp
ntyException *exception;
```
```cpp
```
```cpp
struct
```
```cpp
_ntyExceptionFrame *prev;
```
```cpp
```
```cpp
```
```cpp
char
```
```cpp
message[EXCEPTIN_MESSAGE_LENGTH+1];
```
```cpp
} ntyExceptionFrame;
```
```cpp
#define ntyExceptionPopStack   \
```
```cpp
```
```cpp
ntyThreadDataSet(ExceptionStack, ((ntyExceptionFrame*)ntyThreadDataGet(ExceptionStack))->prev)
```
```cpp
#define ReThrow                    ntyExceptionThrow(frame.exception, frame.func, frame.file, frame.line, NULL)
```
```cpp
#define Throw(e, cause, ...)    ntyExceptionThrow(&(e), __func__, __FILE__, __LINE__, cause, ##__VA_ARGS__, NULL)
```
```cpp
enum
```
```cpp
{
```
```cpp
```
```cpp
ExceptionEntered = 0,
```
```cpp
```
```cpp
ExceptionThrown,
```
```cpp
```
```cpp
ExceptionHandled,
```
```cpp
```
```cpp
ExceptionFinalized
```
```cpp
};
```
```cpp
#define Try do {                         \
```
```cpp
```
```cpp
volatile
```
```cpp
int
```
```cpp
Exception_flag;  \
```
```cpp
```
```cpp
ntyExceptionFrame frame;       \
```
```cpp
```
```cpp
frame.message[0] = 0;         \
```
```cpp
```
```cpp
frame.prev = (ntyExceptionFrame*)ntyThreadDataGet(ExceptionStack);    \
```
```cpp
```
```cpp
ntyThreadDataSet(ExceptionStack, &frame);  \
```
```cpp
```
```cpp
Exception_flag =
```
```cpp
setjmp
```
```cpp
(frame.env);           \
```
```cpp
```
```cpp
if
```
```cpp
(Exception_flag == ExceptionEntered) {
```
```cpp
```
```cpp
#define Catch(e) \
```
```cpp
```
```cpp
if
```
```cpp
(Exception_flag == ExceptionEntered) ntyExceptionPopStack; \
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
if
```
```cpp
(frame.exception == &(e)) { \
```
```cpp
```
```cpp
Exception_flag = ExceptionHandled;
```
```cpp
#define Finally \
```
```cpp
```
```cpp
if
```
```cpp
(Exception_flag == ExceptionEntered) ntyExceptionPopStack; \
```
```cpp
```
```cpp
} { \
```
```cpp
```
```cpp
if
```
```cpp
(Exception_flag == ExceptionEntered)  \
```
```cpp
```
```cpp
Exception_flag = ExceptionFinalized;
```
```cpp
#define EndTry \
```
```cpp
```
```cpp
if
```
```cpp
(Exception_flag == ExceptionEntered) ntyExceptionPopStack; \
```
```cpp
```
```cpp
}
```
```cpp
if
```
```cpp
(Exception_flag == ExceptionThrown) ReThrow; \
```
```cpp
```
```cpp
}
```
```cpp
while
```
```cpp
(0)
```
```cpp
static
```
```cpp
pthread_once_t once_control = PTHREAD_ONCE_INIT;
```
```cpp
static
```
```cpp
void
```
```cpp
init_once(
```
```cpp
void
```
```cpp
) {
```
```cpp
```
```cpp
ntyThreadDataCreate(ExceptionStack);
```
```cpp
}
```
```cpp
void
```
```cpp
ntyExceptionInit(
```
```cpp
void
```
```cpp
) {
```
```cpp
```
```cpp
pthread_once(&once_control, init_once);
```
```cpp
}
```
```cpp
void
```
```cpp
ntyExceptionThrow(ntyException *excep,
```
```cpp
const
```
```cpp
char
```
```cpp
*func,
```
```cpp
const
```
```cpp
char
```
```cpp
*file,
```
```cpp
int
```
```cpp
line,
```
```cpp
const
```
```cpp
char
```
```cpp
*cause, ...) {
```
```cpp
```
```cpp
va_list
```
```cpp
ap;
```
```cpp
```
```cpp
ntyExceptionFrame *frame = (ntyExceptionFrame*)ntyThreadDataGet(ExceptionStack);
```
```cpp
```
```cpp
if
```
```cpp
(frame) {
```
```cpp
```
```cpp
frame->exception = excep;
```
```cpp
```
```cpp
frame->func = func;
```
```cpp
```
```cpp
frame->file = file;
```
```cpp
```
```cpp
frame->line = line;
```
```cpp
```
```cpp
if
```
```cpp
(cause) {
```
```cpp
```
```cpp
va_start
```
```cpp
(ap, cause);
```
```cpp
```
```cpp
vsnprintf(frame->message, EXCEPTIN_MESSAGE_LENGTH, cause, ap);
```
```cpp
```
```cpp
va_end
```
```cpp
(ap);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
ntyExceptionPopStack;
```
```cpp
```
```cpp
longjmp
```
```cpp
(frame->env, ExceptionThrown);
```
```cpp
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
if
```
```cpp
(cause) {
```
```cpp
```
```cpp
char
```
```cpp
message[EXCEPTIN_MESSAGE_LENGTH+1];
```
```cpp
```
```cpp
va_start
```
```cpp
(ap, cause);
```
```cpp
```
```cpp
vsnprintf(message, EXCEPTIN_MESSAGE_LENGTH, cause, ap);
```
```cpp
```
```cpp
va_end
```
```cpp
(ap);
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"%s: %s\n raised in %s at %s:%d\n"
```
```cpp
, excep->name, message, func ? func :
```
```cpp
"?"
```
```cpp
, file ? file :
```
```cpp
"?"
```
```cpp
, line);
```
```cpp
```
```cpp
```
```cpp
}
```
```cpp
else
```
```cpp
{
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"%s: %p\n raised in %s at %s:%d\n"
```
```cpp
, excep->name, excep, func ? func :
```
```cpp
"?"
```
```cpp
, file ? file :
```
```cpp
"?"
```
```cpp
, line);
```
```cpp
```
```cpp
```
```cpp
}
```
```cpp
}
```
```cpp
/* ** **** ******** **************** debug **************** ******** **** ** */
```
```cpp
ntyException A = {
```
```cpp
"AException"
```
```cpp
};
```
```cpp
ntyException B = {
```
```cpp
"BException"
```
```cpp
};
```
```cpp
ntyException C = {
```
```cpp
"CException"
```
```cpp
};
```
```cpp
ntyException D = {
```
```cpp
"DException"
```
```cpp
};
```
```cpp
void
```
```cpp
*
```
```cpp
thread
```
```cpp
(
```
```cpp
void
```
```cpp
*args) {
```
```cpp
```
```cpp
pthread_t selfid = pthread_self();
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
Throw(A,
```
```cpp
"A"
```
```cpp
);
```
```cpp
```
```cpp
```
```cpp
} Catch (A) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch A : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
```
```cpp
} EndTry;
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
Throw(B,
```
```cpp
"B"
```
```cpp
);
```
```cpp
```
```cpp
```
```cpp
} Catch (B) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch B : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
```
```cpp
} EndTry;
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
Throw(C,
```
```cpp
"C"
```
```cpp
);
```
```cpp
```
```cpp
```
```cpp
} Catch (C) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch C : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
```
```cpp
} EndTry;
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
Throw(D,
```
```cpp
"D"
```
```cpp
);
```
```cpp
```
```cpp
```
```cpp
} Catch (D) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch D : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
```
```cpp
} EndTry;
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
Throw(A,
```
```cpp
"A Again"
```
```cpp
);
```
```cpp
```
```cpp
Throw(B,
```
```cpp
"B Again"
```
```cpp
);
```
```cpp
```
```cpp
Throw(C,
```
```cpp
"C Again"
```
```cpp
);
```
```cpp
```
```cpp
Throw(D,
```
```cpp
"D Again"
```
```cpp
);
```
```cpp
```
```cpp
} Catch (A) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch A again : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
```
```cpp
} Catch (B) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch B again : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
} Catch (C) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch C again : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
```
```cpp
} Catch (D) {
```
```cpp
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"catch B again : %ld\n"
```
```cpp
, selfid);
```
```cpp
```
```cpp
```
```cpp
} EndTry;
```
```cpp
```
```cpp
}
```
```cpp
#define THREADS        50
```
```cpp
int
```
```cpp
main(
```
```cpp
void
```
```cpp
) {
```
```cpp
```
```cpp
ntyExceptionInit();
```
```cpp
```
```cpp
Throw(D, NULL);
```
```cpp
```
```cpp
Throw(C,
```
```cpp
"null C"
```
```cpp
);
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"\n\n=> Test1: Try-Catch\n"
```
```cpp
);
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
Try {
```
```cpp
```
```cpp
Throw(B,
```
```cpp
"recall B"
```
```cpp
);
```
```cpp
```
```cpp
} Catch (B) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"recall B \n"
```
```cpp
);
```
```cpp
```
```cpp
} EndTry;
```
```cpp
```
```cpp
```
```cpp
Throw(A, NULL);
```
```cpp
```
```cpp
} Catch(A) {
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"\tResult: Ok\n"
```
```cpp
);
```
```cpp
```
```cpp
```
```cpp
} EndTry;
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"=> Test1: Ok\n\n"
```
```cpp
);
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"=> Test2: Test Thread-safeness\n"
```
```cpp
);
```
```cpp
#if 1
```
```cpp
```
```cpp
int
```
```cpp
i = 0;
```
```cpp
```
```cpp
pthread_t threads[THREADS];
```
```cpp
```
```cpp
```
```cpp
for
```
```cpp
(i = 0;i < THREADS;i ++) {
```
```cpp
```
```cpp
pthread_create(&threads[i], NULL,
```
```cpp
thread
```
```cpp
, NULL);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
for
```
```cpp
(i = 0;i < THREADS;i ++) {
```
```cpp
```
```cpp
pthread_join(threads[i], NULL);
```
```cpp
```
```cpp
}
```
```cpp
#endif
```
```cpp
```
```cpp
printf
```
```cpp
(
```
```cpp
"=> Test2: Ok\n\n"
```
```cpp
);
```
```cpp
}
```
```cpp
```
```
