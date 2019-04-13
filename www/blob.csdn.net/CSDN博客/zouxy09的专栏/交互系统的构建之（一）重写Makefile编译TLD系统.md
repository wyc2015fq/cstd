
# 交互系统的构建之（一）重写Makefile编译TLD系统 - zouxy09的专栏 - CSDN博客


2012年08月29日 12:34:10[zouxy09](https://me.csdn.net/zouxy09)阅读数：6447


**交互系统的构建之（一）重写****Makefile****编译****TLD****系统**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
为了对TLD系统做一些功能的填充，例如添加语音合成来提示跟踪状态，或者加入语音识别来增加交互体验等，我就自己写了个Makefile来编译TLD系统。因为TLD的Makefile是通过cmake生成的，比较复杂，如果要加入语音合成等等API的库或者头文件的话，我都不知道在什么地方加，所以自己的整理了下，需要哪些头文件，哪些链接库，在什么地方添加等等，这些在自己写Makefile来管理后就清晰很多了。
**1****、重新调整下代码文件的结构**：
（先新建一个文件夹，再copy相应的文件进去）如下：
.
├──bounding_boxes.txt
├──include
│├──FerNNClassifier.h
│├──LKTracker.h
│├──TLD.h
│└──tld_utils.h
├──lib
│├──libFerNNClassifier.a
│├──libLKTracker.a
│├──libTLD.a
│└──libtld_utils.a
├──Makefile
├──run_tld
├──src
│├──FerNNClassifier.cpp
│├──LKTracker.cpp
│├──run_tld.cpp
│├──TLD.cpp
│└──tld_utils.cpp
├──parameters.yml
我把生成的run_tld放在了一级目录下，虽然编译产生的目标文件等也会在这个地方生成，有点乱，不是个好习惯，但是个人感觉每次编译和运行都得变换目录，比较麻烦，所以就先这样了。
**2****、编写****Makefile**
刚开始的时候，写了一个初级的Makefile，没有生成和链接静态库，没有加编译优化选项，然后发现运行起来，速度比作者的要慢很多；然后自己就加了将每个cpp文件都编译为lib*.a静态链接库，然后链接进去，发现速度还是没有提高（作者也有编译为静态库，但是我没详细去看Makefile，不知道最终的运行程序是否是链接静态库的）；后来去看了作者的Makefile，发现作者加了gcc编译优化选项：-O3 -DNDEBUG
先得看看这两个选项做了什么，上网搜到的如下：
"NDEBUG"是一个标准的ANSI宏，表示不进行调试编译。
大多数程序和库在编译时默认的优化级别是"2"(使用"-O2"选项)并且带有调试符号来编译，也就是CFLAGS="-O2 -g", CXXFLAGS=$CFLAGS。事实上，"-O2"已经启用绝大多数安全的优化选项了。
先说说"-O3"在"-O2"基础上增加的几项：
-finline-functions
允许编译器选择某些简单的函数在其被调用处展开，比较安全的选项，特别是在CPU二级缓存较大时建议使用。
-funswitch-loops
将循环体中不改变值的变量移动到循环体之外。
-fgcse-after-reload
为了清除多余的溢出，在重载之后执行一个额外的载入消除步骤。
同样的，我也加入了这两个编译优化选项，这时候编译运行起来速度就和作者的一样了。然后我上网搜了一下，发现"-O3"和"-O2"的优化选项各有利弊啊，所以我就修改为"-O2"试了一下，发现速度并没有降下来。不过我觉得作者应该是考虑到代码的一些问题，才用"-O3"，我是一个编程菜鸟，暂时还不能理解怎么深度的问题，呵呵，所以还是按作者的先用着吧。
**3****、附上编译****TLD****的****Makefile****文件：**
CXX=g++
AR=ar
GPATH=src lib
VPATH=src lib
CPPFLAGS= -O3 -DNDEBUG `pkg-config --cflags opencv` -Iinclude
LDFLAGS=-Llib
LIBS=`pkg-config --libs opencv`
SLIBS=lib/libTLD.a lib/libFerNNClassifier.a lib/libLKTracker.a lib/libtld_utils.a
OBJECTS=run_tld.o
TARGET=run_tld
$(TARGET):$(OBJECTS)  $(SLIBS)
$(CXX) $^ $(LDFLAGS) $(SLIBS) $(LIBS) -o $@
lib/libTLD.a:TLD.o
$(AR) rc $@ $<
lib/libFerNNClassifier.a:FerNNClassifier.o
$(AR) rc $@ $<
lib/libLKTracker.a:LKTracker.o
$(AR) rc $@ $<
lib/libtld_utils.a:tld_utils.o
$(AR) rc $@ $<
.cpp.o:
$(CXX) -c $< $(CPPFLAGS) -o $@
.c.o:
$(CC) -c $< $(CPPFLAGS) -o $@
clean:
-rm -rf $(TARGETS) *.o lib/*.a
**4****、未来的过程：**
后面我就会在TLD系统中添加：TTS语音合成、语音识别、手势和语音控制鼠标和键盘、运行前加入手掌的检测（这样就不用鼠标画目标box了）等等（具体见博客更新）。先用开源的模块把整体的一个交互系统的框架搭出来，然后再针对这个系统，自己试着去改进整体的交互性能（通过改进或者增加算法和改进工程实现等方式），提高交互的体验。然后再移植到嵌入式设备上。
这是一个漫长的过程，对于目前基础较差的我，慢慢往前走吧，呵呵，互勉！


