# 安装LuaSocket - gauss的专栏 - CSDN博客
2015年09月13日 22:49:19[gauss](https://me.csdn.net/mathlmx)阅读数：561
这里为《[Lua基础
 coroutine —— Lua的多线程编程](http://blog.csdn.net/wzzfeitian/article/details/8832017)》做一下准备工作，因为用到了socket库，这里就说明一下怎么在fedora上安装luasocket，以防有的朋友的开发环境跟博主的一样，默认没有该库，又得自己到处去查怎么安装。
note：该库从文档看，好像只支持lua5.1，博主没有尝试过5.2，有兴趣的可以试一下是否可以。
首先去这个网站[http://w3.impa.br/~diego/software/luasocket/old/](http://w3.impa.br/~diego/software/luasocket/old/)下载源码，下载那个2.0.3就可以了，博主是用的这个，将其放到你的home目录（或者有写权限的其他目录都可以），用tar
 -zxvf filename 解压缩。
进入luasocket目录，有个config文件，打开这个文件，有2处为止要改：
INTALL_TOP_SHARE 和 INTALL_TOP_LIB两个变量，从opt目录改为usr目录，因为默认的环境变量设置为从usr下面的一些目录搜索.h文件和库文件，
修改后为：
**[plain]**[view plain](http://blog.csdn.net/wzzfeitian/article/details/8866390#)[copy](http://blog.csdn.net/wzzfeitian/article/details/8866390#)
- INSTALL_TOP_SHARE=/usr/local/share/lua/5.1  
- INSTALL_TOP_LIB=/usr/local/lib/lua/5.1  
下面的compiler and linker setting设置需要修改，因为博主这里看到是默认平台是MAC OS，将“for Mac OS X”下面的几行注释掉，将“for linux”下面几行打开，如下：
**[plain]**[view plain](http://blog.csdn.net/wzzfeitian/article/details/8866390#)[copy](http://blog.csdn.net/wzzfeitian/article/details/8866390#)
- #------  
- # Compiler and linker settings  
- # for Mac OS X  
- #  
- #CC=gcc  
- #DEF= -DLUASOCKET_DEBUG -DUNIX_HAS_SUN_LEN  
- #CFLAGS= $(LUAINC) $(COMPAT) $(DEF) -pedantic -Wall -O2 -fno-common  
- #LDFLAGS=-bundle -undefined dynamic_lookup  
- #LD=export MACOSX_DEPLOYMENT_TARGET="10.3";gcc  
- 
- #------  
- # Compiler and linker settings  
- # for Linux  
- CC=gcc  
- DEF=-DLUASOCKET_DEBUG   
- CFLAGS= $(LUAINC) $(DEF) -pedantic -Wall -O2 -fpic  
- LDFLAGS=-O -shared -fpic  
- LD=gcc  
然后保存退出。
运行
**[plain]**[view plain](http://blog.csdn.net/wzzfeitian/article/details/8866390#)[copy](http://blog.csdn.net/wzzfeitian/article/details/8866390#)
- make  
- sudo make install  
如果没有其他错误的话，就安装成功了，此时运行下require "socket"，没有错误提示了，大功告成。
![](https://img-blog.csdn.net/20130429112105083)
