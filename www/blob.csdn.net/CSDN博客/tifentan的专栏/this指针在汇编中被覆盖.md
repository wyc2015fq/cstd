# this指针在汇编中被覆盖 - tifentan的专栏 - CSDN博客

2017年04月01日 11:30:52[露蛇](https://me.csdn.net/tifentan)阅读数：298


昨天遇到个坑，在调试一个程序的时候，在一个类对象的函数处理过程中，this指针被修改。导致程序崩溃。

我的思路是在汇编中先找出this指针的位置，看究竟是怎么被修改了。参考了以下文章，和自己的调试，发现this指针是作为隐藏参数传递的。

http://blog.csdn.net/chenlycly/article/details/53102120

原来在这个函数内部调用了一个dll的api，这个api的参数在this前push进栈，但这个dll的api修改这个参数时越界了，把this也覆盖了。导致this指针出错。

大概是这样：

参数被写前            在api中参数被修改，覆盖了this

xxxx                         aaaa

xxxx  ----->              aaaa

this                           aaaa

...                              ...

ebp                          ebp

因此，绝对是dll有问题。或者是头文件声明的参数结构太短了。

果然查看了以下是头文件跟dll的版本不对应，新版的头文件参数结构变长了。修改后没问题了。

**以后利用第三方dll注意版本！！！**

