
# makefile 使用 Tricks - Zhang's Wikipedia - CSDN博客


2018年05月16日 22:49:05[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：85


.phony是表示目标是伪目标，并不生成相应的文件。.phony标志的文件总是执行的。
## 1. 短横（-）与@
@（常用在 echo 之前）：make 在执行编译打包等命令前会在命令行输出此命令，称之为回显，如果要执行的命令前以字符@开始，则不会回显该条命令，
echoXXX...\# 执行该条命令时，将输出：echoXXX...
XXX...\# 不仅会输出echo要输出的信息（XXX...），还会输出echo这一命令信息@echoXXX...\# 执行命令时，将输出：XXX...\# 不会输出echo命令
-短横（用在 clean 目标， rm 命令前），忽略本条命令的执行成功与否，会继续执行后续命令，类似于 try catch finally 的 finally 代码段
clean:rm *.orm *.a如果 rm *.o 命令发生错误，rm *.a 将不会被执行，clean:
    -rm *.o-rm *.a不论 -rm.o，是否执行成功，-rm.a 都会被执行；

## 2. 特殊符号
$@, $^, $<
$@ 代表目标
$^ 代表所有的依赖对象
$< 代表第一个依赖对象
=/:=/+=
+=：变量的值的尾部增加内容
foo= abcfoo+= efg\# foo 的值就是 abc efg:=：立即生效；=：变量被引用时递归生效；
bar=456foo=${bar}bar=${xxx}xxx=123all:echo ${foo}\# 递归生效，echo 显示的内容为123bar=456foo:=${bar}bar=${xxx}xxx=123all:echo ${foo}\# 立即生效，echo 显示的内容为 456
3. all
all:hello another
hello:hello.cpp
    g++ -o$@$<another:another.cpp
    g++ -o$@$<make hello 的话只编译 hello.cpp
make another 的话只编译 another.cpp
直接 make 或 make all 的话会执行 hello.cpp 和 another.cpp 的编译命令
后面不加参数的话，会把第一个目标作为默认的（因此，make 等价于 make all，因为 all 为第一个目标）

