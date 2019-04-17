# Makefile 实际用例分析（三） ------- 是用GUN automake 处理自己的工程 - DoubleLi - 博客园






前面两篇已经说过了自己怎么去为一个工程写makefile：

[第一篇](http://blog.csdn.net/shanshanpt/article/details/17198413)

[第二篇](http://blog.csdn.net/shanshanpt/article/details/17199695)



现在这一篇说的是怎么使用GNU的工具去写一个符合开源标准的Makefile呢！

首先我觉你应该参考：

[Automake](http://www.gnu.org/software/automake/)

[Autoconf](http://www.gnu.org/software/autoconf/)



这里就一步一步来解释：依然使用之前两篇中的例子ir_tree！



还是先看看我的文件布局是怎么样的！



顶层文件夹：相比之前多了Makefile.am和configure.ac文件，还有四个辅助文件( 顾名思义，我就不多说了 )：

AUTHORS、

ChangeLog、

NEWS、

README

![](http://img.blog.csdn.net/20131208134338640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



然后src文件夹中：相比之前多了一个Makefile.am文件

![](http://img.blog.csdn.net/20131208134434859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



lib里面：注意，这一次，我是将静态库放在这里一起make出来，而不是之前make好，然后在调用！

所有看lib中内容：

![](http://img.blog.csdn.net/20131208134754375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

也有一个Makefile.am,是为了生成libnew.a而需要的！



include文件夹中：还是和以前一样

![](http://img.blog.csdn.net/20131208134909953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

OK，基本结构说完了，然后需要说的是这些新增加的文件的意义！！！

configure.ac文件是为了生成configure文件需要的！

Makefile.am是为了生成makefile.in文件需要的

最后又configure和Makefile.in一起生成Makefile文件！！！！



关于这个自动生成的流程，看下面这个图：**来自维基百科**

![](http://img.blog.csdn.net/20131208135526265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



前提，你安装了automake和autoconf

好，我解释一下：

1：

autoscan这是一个命令，需在你的源文件的文件夹下执行，然后会生成一个configure.scan的模板文件，这个模板是configure.ac模板，所以，将configure.scan改名为configure.ac，然后开始修改！注意configure.ac其实就是一些M4 宏，AC开头代表Autoconf使用，AM开头代表Automake使用。

注意：我们不要去畏惧这个文件，虽然很多开源软件中的configure.ac写的比较复杂，但是我们需要时刻认识到一点是：这只不过是一个环境检测文件，检测你的代码版本，检测代码文件是否存在，检测哪些头文件是否存在，检测你需要的库是否存在，检验是否支持这样的类型数据，检测...

所以随便由你写！对于具体的你的需求，然后需要使用什么M4宏来处理！具体的这些宏，你可以去官方网站查看手册！



2：

好了，假如我们已经写好了configure.ac( 后面具体会说！ )，那么下一步干嘛呢，我们需要aclocal命令执行以下，看看是否需要M4的支持！如果需要会生成：

![](http://img.blog.csdn.net/20131208141044671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbnNoYW5wdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如果没有生成aclocal.m4，那么也没什么，说明不需要这个而已。



3：

关于autoheader命令，为了一个配置头文件通常是config.h而需要的，如果你不需要，直接省略！不要执行这个命令，但是注意在configure.ac中就不要去检测这个config.h的头文件了！否则报错哦！



4：

好了，下面就是需要自己写非常中的Makefile.am文件了！

Makefile.am到底是什么呢？也不要畏惧，只不过是指示生成什么样的东西，生成.o还是.a，还是一个binary？

然后是怎么生成，其实不需要说怎么生成，只需要将这个文件的依赖文件写出来就OK了！！！其实很easy！



对于Makefile.am文件，一般顶层文件肯定有一个，然后在你的源文件中肯定有一个，用于指示怎么生成最终的binary，然后就是你的库生成文件夹中肯定有一个，为了指示怎么生成.a或者.so的库！



所以Makefile.am文件真的很简单，就是这样指示生成文件而已。。。对于复杂的Makefile.am，是因为考虑了很多条件，但是根本的本质还是一样的！



5：

OK，假如我们已经将所有.am文件生成好了，下面干嘛呢！

下面执行autoconf生成configre文件



6：

然后再使用automake -a生成Makefile.in文件，GOOG！好了！我们基本快要大功告成了！



7：

下面是我们最熟悉最熟悉的：

./configure     

make

make install

都是可以执行了~~~~~



上面说的就是总体的一个流程！



################################################################



下面，我们就结合ir_tree的实例一起来简单的写一个！



******* 温馨提示：我下面直接在后面注释习惯不好，可能会编译出错，所有如果你使用了，那么轻删除或者将这些注释换行



>>> configure.ac



#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)                                       # 这里是工具版本号，不关心
AC_INIT(ir_tree, 1.0, ********@gmail.com)     # AC_INIT：指示可执行名称 + 版本号 + BUG-report            
AC_CONFIG_SRCDIR([src/test.c])               # AC_CONFIG_SRCDIR：为了检验源文件是否缺失，你可以检测所有的                                                                       # 文件，这里为了示意，检查一个


AM_CONFIG_HEADER(config.h)                 # 这里为了检查头文件config.h，如果你不需要，就不需要检查
AM_INIT_AUTOMAKE(ir_tree,1.0)                # 这一句可以不需要，也是指示可执行名称 + 版本号 


# Checks for programs.                                # 下面检验一些系统程序是否存在
AC_PROG_CC                                             # 检验CC(就是[C语言](http://lib.csdn.net/base/c)编译程序)是否存在
# 如果你是要的C++，那么AC_PROG_CXX，所有你需要什么，到uanfang手册去查找就OK了！

# Checks for libraries.                                  # 检查一些库（包括系统库和你引入的库）
AC_CHECK_LIB([m], [sqrt])                         # 这里检查数学库-lm

# 其实AC_CHECK_LIB的完整格式是：AC_CHECK_LIB([lib], [func],[if_exist_action], [not_exist_action])

# 后面的func是你使用到的函数名  


AC_PROG_RANLIB                                    # 这里的意义是：当你使用了静态库的时候，需要这一句

                                                                    # 如果你使用了共享库，那么使用AC_PROG_LIBTOOL

# Checks for header files.                           # 下面检查一些头文件，只检查你认为需要检查的即可！然后查手册！
#AC_HEADER_STDC                                
#AC_CHECK_HEADERS([float.h malloc.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.    # 下面检查一些类型定义是否允许
#AC_C_CONST

# Checks for library functions.                  # 下面检查一些库函数是否合理
#AC_FUNC_MALLOC                              # 检查malloc函数
AC_CHECK_FUNCS([pow sqrt])             # 检查pow和sqrt函数
                                                                 # 哪些你需要的你自己肯定知道，然后查手册，放进了检查就OK了~

# 下面是很重要的！

# 就是最终输出哪些文件，由上面的结构知道，有几个Makefile.am就几个Makefile，那么输出文件自然如下！

#
AC_OUTPUT([
  Makefile 
  src/Makefile 
  lib/Makefile
])



好了configure.ac，即环境检测文件写好了，那么下面就是一层一层的Makefile.am文件了



>>> 先看顶层的 Makefile.am

特别的简单：

AUTOMAKE_OPTIONS=foreign       # 这里是一个automake的选项，foreign  ，具体的可以查一下，不是很重要
SUBDIRS=lib src                               # 注意这个SUBDIRS就很重要的，这里指示所有的子Makefile.am所在的目录，

                                                          #  然后执行的时候会一次去执行！注意是：SUBDIRS不是SUBDIR！！！

  # 由上面的文件分布可以看到，lib中是生成libnew.a库的Makefile.am，src中是

   # 生成最终的可执行需要的Makefile.am。所以有两个子文件!!! 
CURRENTPATH=$(shell /bin/pwd)   # 指示当前路径 
INCLUDES=-I$(CURRENTPATH)/include   

# 注意：这个include是为了指示src中头文件之外的其他头文件所在地目录！需要export到后面的子Makefile.am中去！
export INCLUDES                             # 导出！！





>>> 现在进入lib中看Makefile.am:

AUTOMAKE_OPTIONS=foreign     # 不多说

noinst_LIBRARIES=libnew.a           # 注意这里是生成库文件，那么就是XXXX_LIBRARIES
                                                        # noinst即not install的缩写，所以无需安装这个库，当然你可以指示安装
libnew_a_SOURCES=new.c           # 注意前面的命名：‘.’要换成'_'，指示源文件！



# 注意这里有一个隐藏条件：就是上层Makefile.am传递下来的INCLUDES，在这里面也是有使用的，不然无法找到所需头文件的！



所以这个Makefile.am基本就是指示需要生成什么，然后这个生成文件的依赖文件是什么，如此而已！！！



>>> 然后在进入src中看看Makefile.am：

bin_PROGRAMS=ir_tree     # 这个是最终生成的binary名称：使用bin_PROGRAMS

ir_tree_SOURCES=card.c card.h index.c index.h node.c rect.c split_l.c split_l.h test.c  # 指示依赖的源文件


ir_tree_LDADD=$(top_srcdir)/lib/libnew.a    # 指示依赖的库文件( 其实是链接的时候需要增加的信息 )

                                                                      # 注意这里不需要指示-lm，因为我们在configure.ac中已经检查，

                                                                      # 那么需要的时候，自动添加（其实系统库都会自己添加）

# 解释top_srcdir：就是顶层文件夹目录


ir_tree_LDFLAGS=-D_GNU_SOURCE       #这是一个链接FLAG，表示需要用生成.deps依赖文件

DEFS+=-D_GNU_SOURCE



OK，所有的多讲完了，那么最重要的还是需要理解Makefile.am和configure.ac的作用！

不要畏惧，自己想添加什么就添加什么！



OK，

下面你就可以:

执行之前说的那些命令：

aclocal

acheader

autoconf

automake -a

./configure

make



OK... 看看运行结果对不对吧。。。



如果你需要这个工程，同样免费下载：[ir-tree](http://download.csdn.net/detail/shanshanpt/6678621)

from:http://blog.csdn.net/shanshanpt/article/details/17200035









