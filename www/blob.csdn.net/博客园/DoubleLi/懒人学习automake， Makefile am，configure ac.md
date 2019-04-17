# 懒人学习automake， Makefile.am，configure.ac - DoubleLi - 博客园






已经存在Makefile.am，如何生成Makefile？



步骤：





**[plain]**[view plain](http://blog.csdn.net/maray/article/details/6676131#)[copy](http://blog.csdn.net/maray/article/details/6676131#)



- [root@localhost hello]# autoscan .///在当前文件夹中搜索  
- 
- [root@localhost hello]# cp configure.scan configure.ac //复制文件  
- [root@localhost hello]# vi configure.ac //编辑文件  
- 
-     编辑configure.ac，加入下面一行：  
-     AM_INIT_AUTOMAKE(hello,1.0)      //automake所必备的宏，必须添加  
- 
- [root@localhost hello]# aclocal     //执行aclocal生成aclocal.m4文件  
- [root@localhost hello]# autoconf   //执行autoconf生成configure文件  
- [root@localhost hello]# autoheader  
- [root@localhost hello]# automake --add-missing  
- [root@localhost hello]# touch NEWS; touch README; touch AUTHORS; touch ChangeLog    //创建NEWS等文件，如果没有自动生成，手工创建  
- [root@localhost hello]# automake --add-missing //再运行一次  
- [root@localhost hello]# ./configure    //配置，生成Makefile文件  
- [root@localhost hello]# make     //执行make命令  








以上过程可能出现一些警告，请忽略。最后，给出一个Makefile.am的内容作为例子：





**[plain]**[view plain](http://blog.csdn.net/maray/article/details/6676131#)[copy](http://blog.csdn.net/maray/article/details/6676131#)



- AM_LDFLAGS = -lpthread -lc -lm -lrt -ldl  
- CXXFLAGS = -D__STDC_LIMIT_MACROS -g -Wall -DORDER_SERIALIZE #-O2 -fno-strict-aliasing  
- 
- bin_PROGRAMS = parser_main  
- parser_main_SOURCES = parser_main.cpp \  
-                 Parser.cpp \  
-                 Lexer.cpp \  
-     SelectStmt.cpp \  
-     InsertStmt.cpp \  
-     UpdateStmt.cpp \  
-     DeleteStmt.cpp \  
-     Stmt.cpp \  
-     Expr.cpp \  
-                 Identifier.cpp  
- ~                                  










参考文献：

http://os.51cto.com/art/201006/207098.htm

http://os.51cto.com/art/201006/207099.htm

http://os.51cto.com/art/201006/207101.htm



[如何写Makefile.am](http://jianlee.ylinux.org/Computer/C%E5%92%8CGNU%E5%BC%80%E5%8F%91/makefile-am.html)









