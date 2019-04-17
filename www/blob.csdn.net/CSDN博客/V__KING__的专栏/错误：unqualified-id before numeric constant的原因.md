# 错误：unqualified-id before numeric constant的原因 - V__KING__的专栏 - CSDN博客





2013年11月27日 16:19:54[v__king__](https://me.csdn.net/V__KING__)阅读数：898








                
今天编译库的时候出现这个问题unqualified-id before numeric constant，是因为变量重复定义了，主要是变量与宏定义同名了，及以下模式：

一个头文件定义一个接口体，

struct｛

int Size；

｝



然后在另外一个文件定义宏如下：

define Size



这时就会出现以上的错误，真的需要采用常用的习惯的命名原则，不然真不好查。

本文出自 “[zhouhb](http://hongbin0720.blog.51cto.com)” 博客，请务必保留此出处[http://hongbin0720.blog.51cto.com/3305400/640753](http://hongbin0720.blog.51cto.com/3305400/640753)



