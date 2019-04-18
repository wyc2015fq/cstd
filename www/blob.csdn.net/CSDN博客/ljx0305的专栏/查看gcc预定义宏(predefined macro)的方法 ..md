# 查看gcc预定义宏(predefined macro)的方法 . - ljx0305的专栏 - CSDN博客
2012年10月31日 15:19:27[ljx0305](https://me.csdn.net/ljx0305)阅读数：561
[GNU C Compiler](http://en.wikipedia.org/wiki/GNU_Compiler_Collection)`gcc -dM -E - < [/dev/null](http://en.wikipedia.org/wiki//dev/null)`cpp -dM  < /dev/null[HP-UX](http://en.wikipedia.org/wiki/HP-UX) ansi C compiler`cc -v fred.c` (where `fred.c` is
 a simple test file)[SCO OpenServer](http://en.wikipedia.org/wiki/SCO_OpenServer) C compiler`cc -## fred.c` (where `fred.c` is
 a simple test file)[Sun Studio](http://en.wikipedia.org/wiki/Sun_Studio_(software)) C/C++ compiler`cc -## fred.c` (where `fred.c` is
 a simple test file)[IBM AIX](http://en.wikipedia.org/wiki/IBM_AIX) XL C/C++ compiler`cc -qshowmacros -E fred.c` (where `fred.c` is
 a simple test file)    
引用:[http://blog.csdn.net/jubincn/article/details/6901747](http://blog.csdn.net/jubincn/article/details/6901747)
