# Linux 下 make 命令 及 make 笔记 - 在思索中前行！ - CSDN博客





2015年03月26日 22:35:50[_Tham](https://me.csdn.net/txl16211)阅读数：1302









Linux 下 make 命令是系统管理员和程序员用的最频繁的命令之一。管理员用它通过命令行来编译和安装很多开源的工具，程序员用它来管理他们大型复杂的项目编译问题。本文我们将用一些实例来讨论 make 命令背后的工作机制。

## [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#make-%E5%A6%82%E4%BD%95%E5%B7%A5%E4%BD%9C%E7%9A%84)Make 如何工作的


对于不知道背后机理的人来说，make 命令像命令行参数一样接收目标。这些目标通常存放在以 “Makefile” 来命名的特殊文件中，同时文件也包含与目标相对应的操作。更多信息，阅读关于 Makefiles 如何工作的系列文章。


当 make 命令第一次执行时，它扫描 Makefile 找到目标以及其依赖。如果这些依赖自身也是目标，继续为这些依赖扫描 Makefile 建立其依赖关系，然后编译它们。一旦主依赖编译之后，然后就编译主目标（这是通过 make 命令传入的）。


现在，假设你对某个源文件进行了修改，你再次执行 make 命令，它将只编译与该源文件相关的目标文件，因此，编译完最终的可执行文件节省了大量的时间。

## [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#make-%E5%91%BD%E4%BB%A4%E5%AE%9E%E4%BE%8B)Make 命令实例


下面是本文所使用的测试环境：

```
OS —— Ubunut 13.04
Shell —— Bash 4.2.45
Application —— GNU Make 3.81
```


下面是工程的内容：

$ ls 
anotherTest.c Makefile test.c test.h



下面是 Makefile 的内容：

all:test test:test.o anotherTest.o 
    gcc -Wall test.o anotherTest.o -o testtest.o:test.c 
    gcc -c -Wall test.c 

anotherTest.o:anotherTest.c 
    gcc -c -Wall anotherTest.c 

clean: 
    rm -rf *.o test


现在我们来看 Linux 下一些 make 命令应用的实例：

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#1-%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E4%BE%8B%E5%AD%90)1. 一个简单的例子


为了编译整个工程，你可以简单的使用 `make` 或者在 make 命令后带上目标 `all`。

$ make 
gcc -c -Wall test.c 
gcc -c -Wall anotherTest.c 
gcc -Wall test.o anotherTest.o -o test


你能看到 make 命令第一次创建的依赖以及实际的目标。


如果你再次查看目录内容，里面多了一些 .o 文件和执行文件：

$ ls 
anotherTest.c anotherTest.o Makefile test test.c test.h test.o



现在，假设你对 test.c 文件做了一些修改，重新使用 make 编译工程：

$ make 
gcc -c -Wall test.c 
gcc -Wall test.o anotherTest.o -o test


你可以看到只有 test.o 重新编译了，然而另一个 Test.o 没有重新编译。


现在清理所有的目标文件和可执行文件 test，你可以使用目标 `clean`:

$ make clean
rm -rf *.o test$ ls
anotherTest.c Makefile test.c test.h



你可以看到所有的 .o 文件和执行文件 test 都被删除了。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#2-%E9%80%9A%E8%BF%87--b-%E9%80%89%E9%A1%B9%E8%AE%A9%E6%89%80%E6%9C%89%E7%9B%AE%E6%A0%87%E6%80%BB%E6%98%AF%E9%87%8D%E6%96%B0%E5%BB%BA%E7%AB%8B)2. 通过 -B 选项让所有目标总是重新建立


到目前为止，你可能注意到 make 命令不会编译那些自从上次编译之后就没有更改的文件，但是，如果你想覆盖 make 这种默认的行为，你可以使用 -B 选项。


下面是个例子：

$ make
make: Nothing to be done for`all’.

$ make -B
gcc -c -Wall test.c
gcc -c -Wall anotherTest.c
gcc -Wall test.o anotherTest.o -o test


你可以看到尽管 make 命令不会编译任何文件，然而 `make -B` 会强制编译所有的目标文件以及最终的执行文件。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#3-%E4%BD%BF%E7%94%A8--d-%E9%80%89%E9%A1%B9%E6%89%93%E5%8D%B0%E8%B0%83%E8%AF%95%E4%BF%A1%E6%81%AF)3. 使用 -d 选项打印调试信息


如果你想知道 make 执行时实际做了什么，使用 -d 选项。


这是一个例子：

$ make -d | more
GNU Make 3.81
Copyright (C) 2006 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.

This program built for x86_64-pc-linux-gnu
Reading makefiles…
Reading makefile `Makefile’…
Updating makefiles….
Considering target file `Makefile’.
Looking for an implicit rule for`Makefile’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.o’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.c’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.cc’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.C’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.cpp’.
Trying pattern rule with stem `Makefile’.
--More--




这是很长的输出，你也看到我使用了 `more` 命令来一页一页显示输出。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#4-%E4%BD%BF%E7%94%A8--c-%E9%80%89%E9%A1%B9%E6%94%B9%E5%8F%98%E7%9B%AE%E5%BD%95)4. 使用 -C 选项改变目录


你可以为 make 命令提供不同的目录路径，在寻找 Makefile 之前会切换目录的。


这是一个目录，假设你就在当前目录下:

```
$ ls 
file file2 frnd frnd1.cpp log1.txt log3.txt log5.txt
file1 file name with spaces frnd1 frnd.cpp log2.txt log4.txt
```


但是你想运行的 make 命令的 Makefile 文件保存在 ../make-dir/ 目录下，你可以这样做：

$ make -C ../make-dir/ 
make: Entering directory `/home/himanshu/practice/make-dir’ 
make: Nothing to be done for`all’. 
make: Leaving directory `/home/himanshu/practice/make-dir



你能看到 make 命令首先切到特定的目录下，在那执行，然后再切换回来。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#5-%E9%80%9A%E8%BF%87--f-%E9%80%89%E9%A1%B9%E5%B0%86%E5%85%B6%E5%AE%83%E6%96%87%E4%BB%B6%E7%9C%8B%E4%BD%9C-makefile)5. 通过 -f 选项将其它文件看作 Makefile


如果你想将重命名 Makefile 文件，比如取名为 my_makefile 或者其它的名字，我们想让 make 将它也当成 Makefile，可以使用 -f 选项。

make -f my_makefile



通过这种方法，make 命令会选择扫描 my_makefile 来代替 Makefile。


原文链接：[http://linoxide.com/how-tos/linux-make-command-examples/](http://linoxide.com/how-tos/linux-make-command-examples/)

## Make 学习笔记


make 是用来构建和管理工程的工具，它是一个命令，需要解释一个称为 makefile 中的指令，makefile 是描述工程中所有文件的编译、链接的规则。



make 可以根据依赖文件和目标文件的最后修改时间，来决定哪些文件需要更新，哪些文件不需要更新，这样能高效地构建工程。

## makefile

### 基本规则描述


makefile 主要是由规则来组成的，一个基本的规则描述如下：




|```cppTarget: Prerequisites``````cpp``````cppCommands```|
|----|




- **Target**：规则的目标，通常是目标文件或最终的可执行文件名，如 .o 文件或 .out 文件；还可以是 make 执行的动作名称，如 clean，这样的目标称为“伪目标”（Phony targets）。
- **Prerequisites**：依赖文件列表，通常一个目标依赖与一个或多个文件，伪目标是没有依赖文件的。
- **Commands**：规则的命令行，是 make 程序所需要执行的动作，由一堆 shell 命令所组成。注意：每个命令行需要以 `tab` 减缩进。

### makefile 包含


一个 makefile 中可以包含其它的 makefile，格式为：




|```cppinclude filenames```|
|----|





`include` 指示符告诉 make 程序暂停读取当前的 makefile，而转去读取 include 指定的一个或多个文件，完成之后再回到当前 makefile 继续读取。


include 所包含的文件如果不是绝对路径，那么 make 将会在从当前目录下寻找，不存在时会根据命令行选项 `-I 或 --include-dir`  指定的目录下搜索，如果也找不到会搜索系统的目录（如果其存在）：/usr/local/include、/usr/include 等。如果都找不到，给出警告提示但也不会立即退出，而是会继续处理 makefile 内容，当完成读取所有的 makefile 文件后，make 试图使用规则来创建未找到的文件，如果没有规则能创建它，则 make
 将会提示致命错误并退出。


可以是用 `-include 来代替 include` ，忽略由于包含文件找不到提示的错误。



### make 如何解析 makefile




GNU make 执行分为两个阶段：
- 读取所有的 makefile，内建所有的变量、明确规则和隐含规则，建立所有目标和依赖之间的依赖关系。
- 根据第一步所简历的依赖关系，决定哪些目标需要更新，并使用对应的规则进行重建这些目标。

> 

如何决定哪些目标需要更新呢？ 首先，make比较目标文件和所有的依赖文件的时间戳。如果目标的时间戳比所有依赖文件的时间戳更新（依赖文件在上一次执行make之后没有被修改），那么什么也不做。否则（依赖文件中的某一个或者全部在上一次执行make后已经被修改过），规则所定义的重建目标的命令将会被执行。


## makefile 中的规则


上节说到 makefile 中一个基本规则的结构，其实规则就是描述了何种情况下使用什么命令来重建一个特定的文件，该文件称为”目标文件“，规则中所罗列的其它文件称为目标的”依赖“，规则中的命令则是用来更新或者创建目标文件的方法。

### 规则的语法


上面其实也说到一个规则的通常语法，这里只是再强调几点：
- 规则命令可以有两种书写方式：（1）命令和目标、依赖放在同一行，但需要在依赖文件列表后使用分号隔开；（2）命令另起一行，必须以 `Tab`  字符开始。
- makefile 中注释是以 `#` 开始，但前面不能有 `tab` 字符，否则就被当场命令了。
- makefile 中对 `$` 有特殊的含义（变量或函数的引用），若在规则中需要使用 `$` ，需要写两个连续的 `$$` 。
- 当 makefile 出现较长的行时，可以使用反斜线 `\` 来独起一行，但斜线后不能有任何空格。
- 通常 makefile 中第一条规则的目标是整个 makefile 的 ”终极目标“，它是执行 make 的唯一目的，其它规则是在构建终极目标过程中被连带执行的，这些规则的目标在 makefile 中的顺序无关紧要。

### 依赖的类型


有两种不同类型的依赖：
- **常规依赖：**依赖文件任何一个被更新，目标文件都要做相应的被更新。
- **order-only 依赖：**这些依赖的更新不会导致目标被重建，只有在目标文件不存在时才参与目标的构建。


makefile 中书写规则时，order-only 依赖使用管道符号 `|` 开始，左边是常规依赖文件，右边是 order-only 依赖文件，格式如下：




|```cppTARGETS : NORMAL-PREREQUISITES | ORDER-ONLY-PREREQUISITES``````cpp``````cppCOMMANDS```|
|----|





### 文件名使用通配符


makefile 中通配符的用法和含义与 shell 中完全相同，如：

|*|任意字符串|
|----|----|
|?|仅与一个任意的字符匹配|
|[...]|同方括号中指定的任意一个字符匹配|
|[!...]|与所有不在方括号中所有字符匹配|



但并不是这些通配符可以出现在任何地方，通常只有两种场合：
- 规则的目标、依赖中，make 会自动将其展开
- 规则的命令中，在 shell 执行该命令的时候将其展开


例如：




|```cpp# 目标 object 依赖于所有以 .c 结尾的文件，若该目录下有 a.c、b.c、c.c，那么 make 会将其展开为``````cpp# object : a.c b.c c.c``````cppobject : *.c``````cpp``````cpp# make clean 时删除所有以 .o 结尾的文件``````cpp# 这个通配符在 shell 执行的时候才会展开``````cppclean :``````cpp``````cpprm -rf *.o```|
|----|






除了上面这两种场合之外，使用通配符可能与你预期的不一样，例如以通配符定义了一个变量，在规则对引用该变量：




|```cppobjects = *.o``````cpp``````cppfoo: $(objects)``````cpp``````cppgcc -o foo $(objects)```|
|----|






当目录下存在 .o 文件时，那么这些 .o 文件就是目标的依赖文件，目标 foo 会被重建；若不存在 .o 文件时，执行规则时会得到类似于”没有创建 *.o 文件的规则“ 的错误提示。可以使用 wildcard 函数来解决，后面再学习。




|```cppobjects := $(patsubst %.c,%.o,$(wildcard *.c))``````cpp``````cppfoo : $(objects)``````cpp``````cppgcc -o foo $(objects)```|
|----|







通配符与wildcard函数的区别
Icon


这里要注意的是通配符与 wildcard 函数的区别，虽然 `*.c` 和 `$(wildcard *.c)` 都可以表示当前目录下所有以 `.c` 结尾的文件名，用在规则的目标和依赖文件列表中并没有什么不同，但是如果用来定义变量就有本质的不同了，如果定义下面两个变量：




|```cpp# 当前目录下有三个 .c 文件：a.c b.c c.c``````cppSRC1 = *.c``````cppSRC2 = $(wildcard *.c)``````cpp``````cpp# 打印出SRC1 和 SRC2 变量的内容：``````cpp@echo``````cpp"SRC1:"``````cpp$(SRC1)``````cpp@echo``````cpp"SRC2:"``````cpp$(SRC2)```|
|----|






对于 SRC1 来说变量的内容就是 `*.c`，并不会认为这是通配符而将其展开；而对于 SRC2 来说，它用一个函数返回值来对它进行赋值， `wildcard` 函数的返回值是当前目录下匹配 `*.c` 的所有文件名，以空格隔开。



### 目录搜索


在一个大的工程中，通常将源代码和目标代码（.o 和可执行文件）放在不同的目录来管理，这时我们希望 make 提供目录自动搜索依赖文件的功能，指定依赖文件的搜索目录，当工程目录结构发生改变时不需要修改 makefile 的规则，只需要更改依赖文件的搜索目录即可。有以下几种方式可以达到我们的目标：
- **一般搜索：**make 可以识别一个特殊变量 `VPATH`，通过该变量可以指定依赖文件的搜索路径，在当前目录找不到依赖文件时，会自动依次搜索该变量所指定的目录
- **选择性搜索：**VPATH 会对所有文件生效，当需要为不同类型文件指定不同的搜索目录时，使用 `vpath` 来指定，但它不是变量而是 make 的一个关键字，有三种使用方法：
- `vpath PATTERN DIRECTORIES`   为符合模式 PATTERN 的文件指定搜索目录 DIRECTORIES
- `vpath PATTERN`  清除之前为符合模式 PATTERN 设置的文件搜索路径
- `vpath`  清除所有被设置的文件搜索路径



这有个问题是，通过目录搜索得到的目标的依赖文件可能在其它目录，但已经存在的规则命令却不知道在哪个目录，为了写出正确的规则命令，我们需要使用**自动化变量**，稍后再学习。

## makefile 中的命令


每条规则中命令和 shell 命令行是一致的，make 时会按顺序时一条一条地执行命令。

### 显示命令


通常 make 会将其执行的命令在执行前输出到屏幕上，当我们以 `@` 字符放置在命令行前，那么该条命令只执行而不会输出到屏幕上，如果该条命令有输出，那么还是会输出到屏幕的。如：




|```cpp#不带 @ 时，makefile 执行会输出两行，第一行是 make 输出的命令，第二行是 echo 命令的输出``````cpp# echo 正在编译***模块``````cpp# 正在编译***模块``````cppecho 正在编译***模块``````cpp``````cpp#带 @ 时，makefile 执行时 make 的命令将不会输出，但命令的输出还是会输出的:``````cpp# 正在编译***模块``````cppecho 正在编译***模块```|
|----|






还有两个实用技巧：
- make -n/-just-print 只显示命令但不会执行，有利于我们调试 makefile
- `make -s/-slient`   全面禁止命令的输出

### 命令执行


当规则的目标需要被构建或更新时，make 会一条条地执行其后的命令，一般使用环境变量 SHELL 所定义的系统 shell 来执行命令。


如果你需要让上一条命令的结果作用于下一条命令时，你应该使用分号来分割这两条命令；而不能写成单独的两行。如：




|```cpp# 单独两行，第一条 cd 命令没起作用，pwd 输出的仍然是 makefile 所在的目录``````cppexec1:``````cpp``````cppcd /home/guohl``````cpp``````cpppwd``````cpp``````cpp# 使用分号来分隔，pwd 在第一条命令基础上执行，输出的是 /home/guohl``````cppexec2:``````cpp``````cppcd /home/guohl; pwd```|
|----|






当一条命令执行完之后，make 会检测命令的返回码，如果返回成功，make 会执行下一条命令；如果命令出错了，make 会终止当前规则，有可能就终止所有规则的执行。但有时候命令的出错并不是表示错误，如果需要忽略命令的错误导致的终止的影响，有几种方法：
- 在命令行前加上一个减号 `-`，前面在介绍 `include` 包含时已经提到过这种用法；
- 使用 `make -i/-ignore-errors`  命令会全局忽略所有命令行的错误；
- 以 `.IGNORE`  标识的目标规则中的所有命令会忽略错误；
- `make -k/-keep-going`  如果某条规则命令出错终止该条规则，但会继续执行其它规则。

### 嵌套执行 make


在大的工程中，我们会将不同的模块或功能的源文件放到不同的目录中，我们可以在每个目录下写一个该目录的 makefile，在工程的根目录写一个“总控makefile”，这样有利于使我们的 makefile 变得简洁而且更加容易维护。


如有一个子目录叫 subdir，该目录下有个 makefile 来指明这个目录文件的编译规则，对于总控 makefile 可以这样写：




|```cppsubsystem:``````cpp``````cppcd subdir && make``````cpp``````cpp# 等价于：``````cppsubsystem:``````cpp``````cppmake -C subdir```|
|----|






总控 makefile 中定义的变量（如果显示声明）会传递到下级的 makefile 中的，但不会覆盖到下级 makefile 定义的同名变量，除非指定了 `-e`  参数。




|```cpp# 使用 export <var ...> 来声明一个变量，该变量会传递到下级 makefile 中``````cpp# 使用 unexport <var ...> 来声明一个变量，该变量不会传递到下级 makefile 中``````cpp``````cppexport variable = value``````cpp``````cpp# 等价于``````cppvariable = value``````cppexport variable```|
|----|






但有两个变量 `SHELL 和 MAKEFLAGS` 无论你是否 export，都会传递到下级 makefile 中的。

## makefile 中的变量


makefile 中的变量类似于 C/C++ 中的宏，代表一个文本字符串，在执行时会自动原模原样地在使用的地方展开，与宏不同的是你可以在 makefile 中改变其值。下面是变量的一些基础知识：
- 变量命名：可以包含字符、数字、下划线，但不能含有 `":"、"#"、"=" 或空字符，`大小写敏感
- 变量声明时需要赋初值，引用变量时需要在变量名前加上 `$`，最好用 `() 或 {}`  括起来
- 变量可以用在目标、依赖、命令及新的变量中

### 变量赋值


在定义一个变量时需要给它赋值，有四种赋值的方式：=、:=、?=、+=，主要区别在于：
- 前两者主要在定义变量的值时使用其他变量来构造的区别：
- ` =`  操作符赋值，左侧是为定义的新变量，右侧为引用已定义的变量的值，该已定义的变量不一定非要出现在新变量之前，也可以在之后定义
- `:=`  操作符赋值，这种赋值方法前面的变量就不能引用后面定义的变量了

- `?=`  操作符赋值，含义是如果左侧的变量没定义过，那么就定义该变量；否则什么也不做
- 
`+=`  操作符赋值可以在原变量后追加值，如果之前变量没定义过，就自动变成 `=` 赋值；如果之前变量定义过，则 `+=` 会继承前面赋值的操作符，例如前面使用 `:=` 来赋值的，那么这次的 `+=` 也是以 `:=` 来赋值


### 变量的高级用法


下面介绍两种变量的高级用法：
- 变量值的替换

我们可以替换变量中共有的部分，格式为  `$(var:a=b) 或者 ${var:a=b}`  ，意思是把 var 中所有以 a 字符结尾的的 a 替换成 b。
- **变量值当成变量**

使用 `$($(var))`  将变量 var 的值当成一个变量，再取变量的值，可以组合更深。

### 自动化变量


目标和依赖都是一系列的文件，如何书写一个命令完成不同的依赖文件生成相应的目标呢？自动化变量就可以帮助我们完成这个功能。


下面是所有自动化变量及其说明：

|变量|说明|
|----|----|
|`$@`|规则中的目标文件集，在模式规则中，如果有多个目标，该变量匹配目标中模式定义的集合|
|`$%`|仅当目标是函数库中的文件，表示规则中的目标成员名，例如目标是 `foo.a(bar.o)`，那么 `$%` 表示 `bar.o` ；如果不是函数库文件，其值为空|
|`$<`|依赖目标中的第一个目标名字，如果依赖目标是以模式（即 `%` ）定义的，那么将是符合模式的一系列文件集，是一个个取出来|
|`$?`|所有比目标新的依赖目标的文件集，以空格隔开|
|`$^`|所有的依赖目标的集合，以空格隔开，如果依赖目标有重复的，那么会去除重复|
|`$+`|与 `$^` 类似，只不过不去除重复|
|`$*`|表示目标模式中 `%` 及其前面的部分，如果目标是 `dir/a.foo.c` 并且目标模式定义为 `a.%.c`，那么该变量就表示 `dir/a.foo`|



上表中四个变量（ `$@、$<、$%、$*`）在扩展是只会有一个文件，其他三个的值是一个文件列表。

## makefile 中的函数


makefile 中可以使用函数来处理变量，函数调用的返回值可以当作变量来使用。

### 函数调用的语法


函数调用与变量引用很像，都是以 `$` 来标识的，如下：




|```cpp$(<function> <arguments>)``````cpp``````cpp#或者是``````cpp${<function> <arguments>}```|
|----|






其中 `<function>` 是函数名， `<arguments>` 是函数的参数，参数之间以逗号隔开。

### 字符串处理函数


下面介绍常用的字符串处理函数：

|函数名|原型|功能|返回值|示例|
|----|----|----|----|----|
|**subst**|`$(subst <from>, <to>, <text>)`|`把 <text>` 中的 <from> 字符串替换成 <to>|返回被替换过的字符串|$(subst ee, EE, feet on street)返回：fEEt on strEEt|
|**patsubst**|$(patsubst <pattern>, <replacement>, <text>)|把 <text> 中的单词（以空格、tab或回车、换行 分隔）匹配 <pattern> 模式的替换成 <replacement>|返回被替换之后的字符串|$(patsubst %.c,%.o,x.c.c bar.c)返回：x.c.o bar.o|
|**strip**|$(strip <string>)|去掉<string> 字符串中的开头和结尾的空字符|返回被去掉空字符的字符串|$(strip a b c )返回：a b c|
|**findstring**|$(findstring <find>, <in>)|在字符串 <in> 中查找 <find> 字串|找到返回 <find>，否则返回空字符串|$(findstring a, a b c)返回：a|
|**filter**|$(filter <pattern...>, <text>)|以 <pattern> 模式过滤 <text> 字符串中的单词，保留符合 <pattern> 单词|返回符合<pattern> 单词||
|**sort**|$(sort <list>)|||$(sort foo bar lose)返回：bar foo lose|
|**word**|$(word <n>, <text>)|取字符串 <text> 的第 <n> 个单词|||
|**wordlist**|$(wordlist <s>, <e>, <text>)|取字符串 <text> 中的第 <s> 到第 <e> 个单词|||
|**words**|$(words <text>)|统计 <text> 字符串中单词的个数|||
|**firstword**|$(firstword <text>)|取字符串 <text> 中的第一个单词|||


### 文件名操作函数

|函数名|函数原型|功能|返回值|示例|
|----|----|----|----|----|
|**dir**|$(dir <names...>)|从文件名序列 <names> 中取出目录部分|||
|**notdir**|$(notdir <names...>)|从文件名序列 <names> 中取出非目录部分|||
|**suffix**|$(suffix <names...>)|从文件名序列 <names> 中取出文件名的后缀|||
|**basename**|$(basename <names...>)|从文件名序列 <names> 中取出文件名的前缀|||
|**addsuffix**|$(addsuffix <suffix>, <names...>)|把后缀 <suffix> 添加到 <names> 中每个文件名后|||
|**addprefix**|$(addprefix <prefix>, <names...>)|把前缀 <prefix> 添加到 <names> 中每个文件名前|||
|**join**|$(join <list1>, <list2>)|把 <list2> 中的单词对应加到 <list1> 单词的后面|||
|**wildcard**|$(wildcard <pattern>)|列出所有当前路径下所有匹配 <pattern> 模式的文件名|||


### 其它函数

|函数名|函数原型|功能|返回值|示例|
|----|----|----|----|----|
|**shell**|$(shell <commands>)|使用 shell 命令来执行 <commands>|返回 <commands> 命令执行的结果||





