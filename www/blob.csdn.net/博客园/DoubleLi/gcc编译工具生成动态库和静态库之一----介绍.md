# gcc编译工具生成动态库和静态库之一----介绍 - DoubleLi - 博客园






** 1.库的分类**

根据链接时期的不同，库又有静态库和动态库之分。

静态库是在链接阶段被链接的（好像是废话，但事实就是这样），所以生成的可执行文件就不受库的影响了，即使库被删除了，程序依然可以成功运行。

有别于静态库，动态库的链接是在程序执行的时候被链接的。所以，即使程序编译完，库仍须保留在系统上，以供程序运行时调用。（TODO：链接动态库时链接阶段到底做了什么）

**2 静态库和动态库的比较**

链接静态库其实从某种意义上来说也是一种粘贴复制，只不过它操作的对象是目标代码而不是源码而已。因为静态库被链接后库就直接嵌入可执行文件中了，这样就带来了两个问题。

首先就是系统空间被浪费了。这是显而易见的，想象一下，如果多个程序链接了同一个库，则每一个生成的可执行文件就都会有一个库的副本，必然会浪费系统空间。

再者，人非圣贤，即使是精心调试的库，也难免会有错。一旦发现了库中有bug，挽救起来就比较麻烦了。必须一一把链接该库的程序找出来，然后重新编译。

而动态库的出现正弥补了静态库的以上弊端。因为动态库是在程序运行时被链接的，所以磁盘上只须保留一份副本，因此节约了磁盘空间。如果发现了bug或要升级也很简单，只要用新的库把原来的替换掉就行了。

那么，是不是静态库就一无是处了呢？

答曰：非也非也。不是有句话么：存在即是合理。静态库既然没有湮没在滔滔的历史长河中，就必然有它的用武之地。想象一下这样的情况：如果你用libpcap库编了一个程序，要给被人运行，而他的系统上没有装pcap库，该怎么解决呢？最简单的办法就是编译该程序时把所有要链接的库都链接它们的静态库，这样，就可以在别人的系统上直接运行该程序了。

所谓有得必有失，正因为动态库在程序运行时被链接，故程序的运行速度和链接静态库的版本相比必然会打折扣。然而瑕不掩瑜，动态库的不足相对于它带来的好处在现今硬件下简直是微不足道的，所以链接程序在链接时一般是优先链接动态库的，除非用-static参数指定链接静态库。



gcc作为编译工具，用在Linux操作系统中，可以编译C、C++、Object-C、JAVA等语言。编译过程中可以带编译选项，选择编译过程。



一、GCC编译流程

1）预处理 Pre-Processing
2）编译 Compiling
3）汇编 Assembling
4）链接 Linking





二、GCC编译选项


1、gcc总体选项列表


1） -c ：指编译，不链接，生成目标文件“.o”。
2） -S ：只编译，不汇编，生成汇编代码“.S”。
3） -E ：只进行预编译/预处理，不做其他处理。
4） -o file：把输出文件输出到file里。
5） -g ：在可执行程序中包含标准调试信息。
6） -v ：打印出编译器内部编译各过程的命令行信息和编译器的版本。
7） -I dir ：在头文件的搜索路径列表中添加dir目录
8） -L dir ：在库文件的搜索路径列表中添加dir目录
9） -static ：连接静态库（静态库也可以用动态库链接方式链接）
10） -llibrary ：连接名为library的库文件（显示指定需要链接的动态库文件）


2、gcc告警和出错选项
1） -ansi ：支持符合ANSI标准的C程序
2） -pedantic ：允许发出ANSI C标准所列出的全部警告信息
3） -pedantic-error ：允许发出ANSI C标准所列出的全部错误信息
4） -w ：关闭所有警告
5） -Wall ：允许发出gcc提供的所有有用的报警信息
6） -werror ：把所有的告警信息转化为错误信息，并在告警发生时终止编译过程


3、gcc优化选项
gcc可以对代码进行优化，它通过编译选项“-On”来控制优化代码的生成，其中n是一个代表优化级别的整数。对于不同版本的gcc，
n的取值范围不一致，比较典型的范围为0变化到2或者3。
虽然优化选项可以加速代码的运行速度，但对于调试而言将是一个很大的挑战。因为代码在经过优化之后，原先在源程序中声明和使用
的变量很可能不再使用，控制流也可能会突然跳转到意外的地方，循环语句也可能因为循环展开而变得到处都有。



**三、GCC生成动态库和静态库**



1）动态库生成
1.单个源文件/目标直接生成动态库

```
a. 
gcc -fPIC -shared xxx.c -o libxxx.so
b.
gcc -fPIC -shared xxx.o -o libxxx.so
```

2.多个源文件/目标生成动态库

```
a.
gcc -fPIC -shared xxx1.c xxx2.c xxx3.c -o libxxx.so 
b.
gcc -fPIC -shared xxx1.o xxx2.o xxx3.o -o libxxx.so
```

2）静态库生成
1.单个源文件/目标直接生成静态库

```
a.
ar -rc libxxx.a xxx.o（正确方法）
b. ar -rc libxxx.a xxx.c  （静态库可以生成；当运行连接了该静态库的可执行程序会报错：could not read symbols:Archive has no index;run ranlib
 to add one）
```

2.多个源文件/目标生成静态库

```
a.
ar -rc libxxx.a xxx1.o xxx2.o xxx3.o （正确方法）
b.
ar -rc libxxx.a xxx1.c xxx2.c xxx3.c （静态库可以生成；当运行连接了该静态库的可执行程序会报错：could not read symbols:Archive has no index;run ranlib to add one）
```

四、多个源文件生成一个可执行文件

```
gcc xxx1.c xxx2.c xxx3.c xxx4.c main.c -o main
```



**实例**

一、动态链接库

1.创建hello.so动态库



- #include <stdio.h>  
- void hello(){  
-     printf("hello world\n");  
- }  
- 编译：gcc -fPIC -shared hello.c -o libhello.so  



2.hello.h头文件

- void hello();  



3.链接动态库

- #include <stdio.h>  
- #include "hello.h"  
- 
- int main(){  
-     printf("call hello()");  
-     hello();  
- }  
- 编译：gcc main.c -L. -lhello -o main  


这里-L的选项是指定编译器在搜索动态库时搜索的路径，告诉编译器hello库的位置。"."意思是当前路径.



3.编译成够后执行./main，会提示：

- In function `main':  
- 
- main.c:(.text+0x1d): undefined reference to `hello'  
- collect2: ld returned 1 exit status  


这是因为在链接hello动态库时，编译器没有找到。
解决方法：

- sudo cp libhello.so /usr/lib/  


这样，再次执行就成功输入:
call hello()



二、静态库

文件有：main.c、hello.c、hello.h
1.编译静态库hello.o: 

- gcc hello.c -o hello.o  #这里没有使用-shared  



2.把目标文档归档



- ar -r libhello.a hello.o  #这里的ar相当于tar的作用，将多个目标打包。  


程序ar配合参数-r创建一个新库libhello.a，并将命令行中列出的文件打包入其中。这种方法，如果libhello.a已经存在，将会覆盖现在文件，否则将新创建。

3.链接静态库

- gcc main.c -lhello -L. -static -o main  


这里的-static选项是告诉编译器,hello是静态库。
或者：

- gcc main.c libhello.a -L. -o main  


这样就可以不用加-static

4.执行./main

输出：call hello()




**makefile实例**




### 1. 静态库的生成

makefile命令的简介可参考：[跟我一起写 Makefile](http://www.chinaunix.net/old_jh/23/408225.html)。使用ar命令生成.a文件，可参考：[Linux下动态库(.so)和静态库(.a)](http://blog.csdn.net/felixit0120/article/details/7652907)

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# 1、准备工作，编译方式、目标文件名、依赖库路径的定义。
CC = g++
CFLAGS  := -Wall -O3 -std=c++0x 

# opencv 头文件和lib路径 
OPENCV_INC_ROOT = /usr/local/include/opencv 
OPENCV_LIB_ROOT = /usr/local/lib

OBJS = GenDll.o #.o文件与.cpp文件同名
LIB = libgendll.a # 目标文件名 

OPENCV_INC= -I $(OPENCV_INC_ROOT)

INCLUDE_PATH = $(OPENCV_INC)

LIB_PATH = -L $(OPENCV_LIB_ROOT)

# 依赖的lib名称
OPENCV_LIB = -lopencv_objdetect -lopencv_core -lopencv_highgui -lopencv_imgproc

all : $(LIB)

# 2. 生成.o文件 
%.o : %.cpp
    $(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_PATH) $(LIB_PATH) $(OPENCV_LIB) 

# 3. 生成静态库文件
$(LIB) : $(OBJS)
    rm -f $@
    ar cr $@ $(OBJS)
    rm -f $(OBJS)

tags :
     ctags -R *

# 4. 删除中间过程生成的文件 
clean:
    rm -f $(OBJS) $(TARGET) $(LIB)

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



### 2. 动态库的生成

第1、4步准备和收尾工作与静态库的保持一致，第2步和第3步所使用的命令稍有不同。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

# 1、准备工作，编译方式、目标文件名、依赖库路径的定义。
CC = g++
CFLAGS  := -Wall -O3 -std=c++0x 

# opencv 头文件和lib路径 
OPENCV_INC_ROOT = /usr/local/include/opencv 
OPENCV_LIB_ROOT = /usr/local/lib

OBJS = GenDll.o #.o文件与.cpp文件同名
LIB = libgendll.so # 目标文件名 

OPENCV_INC= -I $(OPENCV_INC_ROOT)

INCLUDE_PATH = $(OPENCV_INC)

LIB_PATH = -L $(OPENCV_LIB_ROOT)

# 依赖的lib名称
OPENCV_LIB = -lopencv_objdetect -lopencv_core -lopencv_highgui -lopencv_imgproc

all : $(LIB)

# 2. 生成.o文件 
%.o : %.cpp
    $(CC) $(CFLAGS) -fpic -c $< -o $@ $(INCLUDE_PATH) $(LIB_PATH) $(OPENCV_LIB) 

# 3. 生成动态库文件
$(LIB) : $(OBJS)
    rm -f $@
    g++ -shared -o $@ $(OBJS)
    rm -f $(OBJS)

tags :
     ctags -R *

# 4. 删除中间过程生成的文件 
clean:
    rm -f $(OBJS) $(TARGET) $(LIB)

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



-fpic 和 -shared 命令可参考：[Linux下动态库(.so)和静态库(.a)](http://blog.csdn.net/felixit0120/article/details/7652907)【注】这篇文章说可以使用ld命令生成.so文件，但我在测试时发会报错。

### 3. 动态库和静态库的调用

， 这两个的使用方法几乎没有区别。动态库的引用有显式和隐式两种，这里只说隐式调用。我使用main.cpp来测试生成的库文件， makefile如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

CC = g++
CFLAGS  := -Wall -O3 -std=c++0x 

OPENCV_INC_ROOT = /usr/local/include/opencv 
OPENCV_LIB_ROOT = /usr/local/lib
MY_ROOT = ../

OPENCV_INC= -I $(OPENCV_INC_ROOT)
MY_INC = -I $(MY_ROOT)

EXT_INC = $(OPENCV_INC) $(MY_INC)

OPENCV_LIB_PATH = -L $(OPENCV_LIB_ROOT)
MY_LIB_PATH = -L $(MY_ROOT)

EXT_LIB = $(OPENCV_LIB_PATH) $(MY_LIB_PATH) 

OPENCV_LIB_NAME = -lopencv_objdetect -lopencv_highgui -lopencv_imgproc -lopencv_core 
MY_LIB_NAME = -lgendll

all:test

test:main.cpp
    $(CC) $(CFLAGS) main.cpp $(EXT_INC) $(EXT_LIB) $(MY_LIB_NAME) $(OPENCV_LIB_NAME) -o test

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

### 4. 注意事项：

1、在测试过程中，经常会报错：找不到.so文件。一种简单的解决方法如下： 
在linux终端输入如下命令：

```bash
export LD_LIBRARY_PATH=/home/shaoxiaohu/lib:LD_LIBRARY_PATH:
```










