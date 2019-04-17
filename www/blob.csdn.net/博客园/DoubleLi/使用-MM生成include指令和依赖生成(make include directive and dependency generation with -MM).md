# 使用-MM生成include指令和依赖生成(make include directive and dependency generation with -MM) - DoubleLi - 博客园







I want a build rule to be triggered by an include directive if the target of the include is out of date or doesn't exist.

Currently the makefile looks like this:
program_NAME := wget++
program_H_SRCS := $(wildcard *.h)
program_CXX_SRCS := $(wildcard *.cpp)
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_CXX_OBJS)

DEPS = make.deps

.PHONY: all clean distclean

all: $(program_NAME) $(DEPS)

$(program_NAME): $(program_OBJS)
    $(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
    @- $(RM) $(program_NAME)
    @- $(RM) $(program_OBJS)
    @- $(RM) make.deps

distclean: clean

make.deps: $(program_CXX_SRCS) $(program_H_SRCS)
    $(CXX) $(CPPFLAGS) -MM $(program_CXX_SRCS) > make.deps

include $(DEPS)

The problem is that it seems like the include directive is executing before the rule to build make.deps which effectively means that make is either getting no dependency list if make.deps doesn't exist or always getting the make.deps from the previous build and not the current one.

For example:
$ make clean 
$ make
makefile:32: make.deps: No such file or directory
g++  -MM addrCache.cpp connCache.cpp httpClient.cpp wget++.cpp > make.deps
g++    -c -o addrCache.o addrCache.cpp
g++    -c -o connCache.o connCache.cpp
g++    -c -o httpClient.o httpClient.cpp
g++    -c -o wget++.o wget++.cpp
g++      addrCache.o connCache.o httpClient.o wget++.o -o wget++

**Edit**

I read the [docs for the include directive](http://theory.uwinnipeg.ca/localfiles/infofiles/make/make_16.html), and it sounds like if the include target doesn't exist it will continue processing the parent makefile try and build the target, but it's not completely clear to me how this works:

> 
If an included makefile cannot be found in any of these directories, a warning message is generated, but it is not an immediately fatal error; processing of the makefile containing the include continues. Once it has finished reading makefiles, make will try to remake any that are out of date or don't exist. See section How Makefiles Are Remade. Only after it has tried to find a way to remake a makefile and failed, will make diagnose the missing makefile as a fatal error.


**ANSWER**

This is a modification of the answer I accepted. The one thing missing was that the dependency files also depend on the sources, and won't get regenerated unless they are added to the deps files which are being included:
%.d: $(program_CXX_SRCS)
    @ $(CXX) $(CPPFLAGS) -MM $*.cpp | sed -e 's@^\(.*\)\.o:@\1.d \1.o:@' > $@

sed adds the name of the .d file to the beginning of each dependency line like so:
foo.d foo.o: foo.cpp foo.h bar.h baz.h

I got the idea from this amazing paper on the dangers of recursive make:

[Recursive Make Considered Harmful](http://aegis.sourceforge.net/auug97.pdf)

I also add the following to the makefile:
clean_list += ${program_SRCS:.c=.d}

# At the end of the makefile
# Include the list of dependancies generated for each object file
# unless make was called with target clean
ifneq "$(MAKECMDGOALS)" "clean"
-include ${program_SRCS:.c=.d}
endif

解决方案

You are relying on an implicit rule to compile your .cpp files. You have to redefine it to use the -MM and -MF flags that will create the dependency file.
%.o: %.cpp
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -MM -MF $@.d

Then, you have to include these dependencies files in the Makefile, using -include that will not error when the dependencies files do not exist yet (on the first time, or after a clean).
program_DEPS := $(program_OBJS:.o=.o.d)
-include $(program_DEPS)

And remember to add the rm command for the dependencies files in the clean rule.

本文地址：[IT屋](http://www.it1352.com/) » [make include directive and dependency generation with -MM](http://www.it1352.com/462114.html)



问 题







如果include的目标已过期或不存在，我想要一个构造规则由include指令触发。



目前makefile像这样：


  program_NAME：= wget ++ 
 program_H_SRCS：= $（通配符* .h）
 program_CXX_SRCS ：= $（wildcard * .cpp）
 program_CXX_OBJS：= $ {program_CXX_SRCS：.cpp = .o} 
 program_OBJS：= $（program_CXX_OBJS）

 DEPS = make.deps 

 .PHONY：all clean distclean 

 all：$（program_NAME）$（DEPS）

 $（program_NAME）：$（program_OBJS）
 $（LINK.cc）$（program_OBJS）-o $（program_NAME）

 clean：
 @  -  $（RM）$（program_NAME）
 @  -  $ （program_OBJS）
 @  -  $（RM）make.deps 

 distclean：clean 

 make.deps：$（program_CXX_SRCS）$（program_H_SRCS ）
 $（CXX）$（CPPFLAGS）-MM $（program_CXX_SRCS）> make.deps 

 include $（DEPS）



问题是似乎include指令是在规则之前执行make.deps，这有效地意味着make或者没有依赖列表，如果make.deps不存在或者总是从上一个版本获取make.deps，而不是当前版本



例如：


  $ make clean 
 $ make 
 makefile：32：make.deps：没有这样的文件或目录
 g ++ -MM addrCache.cpp connCache.cpp httpClient.cpp wget ++。cpp> make.deps 
 g ++ -c -o addrCache.o addrCache.cpp 
 g ++ -c -o connCache.o connCache.cpp 
 g ++ -c -o httpClient.o httpClient.cpp 
 g ++ -c -o wget ++。o wget ++。cpp 
 g ++ addrCache.o connCache.o httpClient.o wget ++。o -o wget ++ 



**编辑**



我阅读了[include指令的文档](http://www.it1352.com/%E2%80%9Chttp%EF%BC%9A//theory.uwinnipeg)，它听起来像包含目标不存在它会继续处理父makefile try和构建目标，但它不是完全清楚我是如何工作：



> 

如果包含的makefile不能是
在任何在这些目录中，生成一个
警告消息，但它
不是一个立即致命的错误; 
处理包含
的makefile包含继续。一旦它有
完成阅读makefile，make将
尝试重新制作任何过时的
或不存在。参见如何
Makefile的Remade。只有在
尝试找到一个方法来重新创建一个
makefile并失败后，将使
诊断丢失的makefile为
致命错误。




**ANSWER**



这是我接受的答案的修改。缺少的一个事情是依赖文件也依赖于源，并且不会被重新生成，除非它们被添加到包含的deps文件：


 ％d：$（program_CXX_SRCS）
 @ $（CXX）$（CPPFLAGS）-MM $ *。cpp | sed -e'@ ^ \（。* \）\.o：@ \1.d \1.o：@'> $ @ 



sed .d 文件到每个依赖项行的开头，如下所示：


  foo.d foo.h：foo.cpp foo.h bar.h baz.h 



这个惊人的文章对递归make危险的想法：



[递归使用有害](http://www.it1352.com/%E2%80%9Chttp://aegis.sourceforge.net/auug97.pdf%E2%80%9Drel)



我还在makefile中添加以下内容：


  clean_list + = $ {program_SRCS：.c = .d} 

＃在makefile的末尾
＃包括为每个生成的依赖列表对象文件
＃除非使用目标clean调用
 ifneq“$（MAKECMDGOALS）”“clean”
 -include $ {program_SRCS：.c = .d} 
 endif 


解决方案

您正在依靠隐式规则来编译.cpp文件。您必须重新定义它才能使用将创建依赖文件的-MM和-MF标志。


 ％。o： .cpp 
 $（CXX）$（CPPFLAGS）$（CXXFLAGS）-c $& -o $ @ -MM -MF $ @。d 



然后，您必须包括这些依赖文件在Makefile中，使用 -include ，当依赖文件不存在时（第一次或清除后），它不会报错。


  program_DEPS：= $（program_OBJS：.o = .od）
 -include $（program_DEPS）



并记住在清理规则中为依赖文件添加rm命令。













