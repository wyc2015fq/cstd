# GDB调试 - V__KING__的专栏 - CSDN博客





2017年02月27日 19:00:26[v__king__](https://me.csdn.net/V__KING__)阅读数：372
个人分类：[linux																[c](https://blog.csdn.net/V__KING__/article/category/1711535)](https://blog.csdn.net/V__KING__/article/category/1733387)








# gdb调试

## 断点设置

```
gdb run.exe
break file_name.c:88
break function_name:88

run arg1 arg2  // arg1 arg2是run.exe的参数

set print pretty on    //set结构体格式输出就很漂亮
```

### 断点设置保存

断点可以使用break file_name.c:88 

其中88是行号

在gdb中，可以使用如下命令将设置的断点保存下来：

```
(gdb) save breakpoints file-name-to-save.bp
//保存的文件file-name-to-save.bp如下格式：
break ltkc_frameextract.c:42
break ltkc_frameextract.c:44
break ltkc_frameextract.c:79
break ltkc_frameextract.c:96
```

下此调试时，可以使用如下命令批量设置保存的断点,调试的时候可以修改*.bg文件：

```
(gdb) source file-name-to-save.bp
或者：
gdb run.exe -x file-name-to-save.bp
```

### 断点删除

断点的删除与断点的设置同样的重要。删除断点的命令有两个: 
**delete**

用法：delete [breakpoints num] [range…] 

delete可删除单个断点，也可删除一个断点的集合，这个集合用连续的断点号来描述。 

例如： 

delete 5 

delete 1-10 

clear list.c:12 //删除文件：行号的所有断点
**clear**

用法:clear 
- 删除所在行的多有断点。
- clear location 

clear 删除所选定的环境中所有的断点 

clear location location描述具体的断点。
例如： 

clear list_insert         //删除函数的所有断点 

clear list.c:list_delet   //删除文件：函数的所有断点 

clear 12                  //删除行号的所有断点 

clear list.c:12           //删除文件：行号的所有断点
clear 删除断点是基于行的，不是把所有的断点都删除。

### 自动display

每次调到 bread点的时候，都会自动的把变量值打印出来，所以使**用起来很方便**

```java
display var1
display/a *pMen@menLen    //动态内存(函数或者malloc)
display pMen              //静态内存
```

### 如何设置改变变量的值
- 在gdb中，可以用“set var variable=expr”命令设置变量的值，以上面代码为例：

```matlab
Breakpoint 2, func () at a.c:5
5                   int i = 2;
(gdb) n
7                   return i;
(gdb) set var i = 8
(gdb) p i
$4 = 8
```
- 改变地址中的内容 

也可以用“set {type}address=expr”的方式，含义是给存储地址在address，变量类型为type的变量赋值，仍以上面代码为例：

```
Breakpoint 2, func () at a.c:5
5                   int i = 2;
(gdb) n
7                   return i;
(gdb) p &i
$5 = (int *) 0x8047a54
(gdb) set {int}0x8047a54 = 8
(gdb) p i
$6 = 8
```

### gdb结构体格式输出
`set print pretty on`
> 
参考：[http://blog.csdn.net/unix21/article/details/9991925](http://blog.csdn.net/unix21/article/details/9991925)








