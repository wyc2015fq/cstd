# GDB程序调试工具 - xcw0754 - 博客园
# [GDB程序调试工具](https://www.cnblogs.com/xcw0754/p/9815284.html)
使用gcc/g++编译程序时加-g选项以方便调试。启动gdb时不打印提示信息可以加-q选项。
设置系统允许产生core文件:
`$ulimit -c unlimited`
调试由test程序产生的core文件:
`$gdb ./test  core`
设置输出信息时的分页功能相关命令
```
set pagination on    # 或者 set height <n>
set pagination off    # 等同 set height unlimited
show height   # 查看输出页面高度
show width    # 查看输出页面宽度
```
列出所有函数
```
info functions [regex]    # 不带regex的话输出数量一般很多
info functions foo    # 列出含有foo的函数名
```
打断点。
```
b <line>    # 在指定行打断点
b 12
b <func_name>    # 在指定函数打断点
b main
b <filename>:<line>    # 在指定源文件指定行打断点
b test.c:15
tb <line>    # 打临时断点，命中一次后失效
b 15 if i==0    # 打条件断点
info b    # 打印所有断点信息
```
在运行`r`之后会停留在断点处，如果断点都在同一个函数中，按`n`之后是不会进入其他函数的，只会直接执行。此时可以在执行函数的那一行按`s`来进入该函数。
如果按了`s`进入了某个不带调试信息的函数，那么接下来按`n`都会在该函数内逐行执行，可以通过敲`finish`快速运行完该函数。或者可以敲`return <xxx>`来快速跳过下面的代码行而直接返回。
在运行程序的过程中，可以直接调用某个函数，使用的是call或print指令
```
call foo(234)
print(foos("abc"))
```
打印当前域变量
```
p var    # var是变量名
```
打印所有堆栈或者指定的帧
```
bt    # 打印所有堆栈
frame <number>    # 打印并跳转到指定序号的帧
frame <addr>    # 打印并跳转到指定地址的帧
info frame    # 打印当前帧的信息
```

