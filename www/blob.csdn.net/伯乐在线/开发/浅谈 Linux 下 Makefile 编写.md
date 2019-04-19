# 浅谈 Linux 下 Makefile 编写 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [answer](http://www.jobbole.com/members/answer) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**前言：**本文简要介绍Makefile文件的编写规范，结合具体项目中的应用进行讲解。
具体代码地址： [https://github.com/AnSwErYWJ/DogFood/blob/master/Makefile](https://github.com/AnSwErYWJ/DogFood/blob/master/Makefile)
## 简介
Make工具最主要也是最基本的功能就是通过makefile文件来描述源程序之间的相互关系并自动维护编译工作。而makefile 文件需要按照某种语法进行编写，文件中需要说明如何编译各个源文件并连接生成可执行文件，并要求定义源文件之间的依赖关系。
## make命令的选项与参数


```
-k：发生错误时任然继续执行，而不是在检测到第一个错误时停止。
-n：只输出要执行的步骤，不真正执行。
-f：告诉make命令哪个文件作为makefile文件。如果不存在就查找makefile文件，如果还不存在就查找Makefile。按惯例使用Makefile。
```
## 语法
makefile文件由一组依赖关系和规则组成。每个依赖关系由一个目标（即将要创建的文件）和一组该目标所依赖的源文件组成。
### 依赖关系
依赖关系定义了最终应用程序里的每个文件与源文件之间的关系。


```
a.out: main.o
main.o: main.c main.h
```
上面两行就是依赖关系：目标a.out:依赖于main.o，目标main.o依赖于main.c和main.h。所以，如果main.c和main.h发生更改，则需要重新编译整个程序。
如果需要一次创建多个目标文件，则使用为目标all：


```
all: a.out b.out
```
> 
注意：如果未指定一个all目标，则make命令只创建makefile文件的第一个目标。
### 规则
规则定义了目标的创建方式。
> 
注意：规则所在行必须以制表符tab开头。
下面就是一个简单的例子：


```
a.out: main.o
    gcc main.o
main.o: main.c main.h
    gcc -c main.c
```
make命令会自行判断文件的正确顺序，并使用给出的规则创建相应的文件，并在屏幕显示出来。如果多次编译，则make命令会读取makefile文件来确定重建目标的最少命令，而不会重复编译。
### 注释
makefile文件的注释以#开头，到一行的结束。
## 宏
makefile文件中也可以定义宏，也可以用make命令时在命令行定义，如果需要使用文件外部定义，最好不要用空格。


```
#宏定义
SRC = example.c
OBJS = example.o
CC = gcc
CFALGS = -o
#宏引用
test: sqlite3.o $(OBJS)
    $(CC) $(CFALGS) test sqlite3.o $(OBJS) -ldl -lpthread
```
内置宏：


```
$?：当前目标所依赖的文件列表中比当前目标文件还要新的文件
$@：当前目标的名字
$：当前依赖文件的名字
$*：不包括后缀名的当前依赖文件的名字
-：使make忽略错误
@：执行时不将该条命令显示在标准输出
```
## 应用
下面放一个我写的Makefile


```
SRC = example.c
OBJS = example.o
CC = gcc
test: sqlite3.o $(OBJS)
    $(CC) -o test sqlite3.o $(OBJS) -ldl -lpthread
sqlite3.o: sqlite3.c sqlite3.h
    $(CC) -c sqlite3.c -o sqlite3.o  
example1.o: $(SRC) sqlite3.h
    $(CC) -c $(SRC) -o example1.o
clean:
    -rm -f *.o test
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/a0fe2e7173dbb8e9958f604115b14e47.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/a1c7df04a021538432631eedf6eed47e.jpg)
