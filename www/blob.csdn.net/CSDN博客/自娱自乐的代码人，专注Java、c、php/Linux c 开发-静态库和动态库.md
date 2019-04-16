# Linux c 开发 - 静态库和动态库 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年02月14日 11:08:04[initphp](https://me.csdn.net/initphp)阅读数：1283
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









### 关于静态库和动态库

之前的篇章我们已经讲到了c语言的gcc，可以查看先关文章 [《Linux c 开发 - gcc》](http://blog.csdn.net/initphp/article/details/39825969)


1. 静态库。

静态库都是**lib**.a**格式的文件。利用静态库编译的可执行文件会相对比较大，因为静态库会把整个库都整合进目标代码中。

使用静态库有一个好处，可执行文件编译成功后，是独立的可执行文件，而不需要依赖于任何外部的函数库支持。

我们知道，目标文件一般都是由多个*.o的文件连接编译而成。而静态库**lib**.a** 你可以把它当成由多个**.o**文件组成的打包文件。




2. 动态库。

动态库都是以**lib*.so**格式的文件。动态函数库在编译的时候并不会被编译到目标代码中。你程序调用到动态函数库中的函数的时候才会去调用。

动态库的好处：编译生成的可执行文件比较小；动态函数库可以自由升级，现有的库并不需要跟着重新编译。

liunx下面有两个目录就放动态库 ** /lib     /usr/lib**




### 示例代码

我们先写一个示例代码：

main.c



```cpp
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include "sum.h"  
#include "get.h"  
  
  
//入口主函数  
int main() {  
    int x = 10;  
    int y = 20;  
    int z = sum(&x, &y);  
    puts("This is Main");  
    printf("Z:%d\n", z);  
    x = 20;  
    z = get(&x, &y);  
    printf("Z:%d\n", z);  
    return 1;  
}
```
val.c和val.c



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int val(int *x);
```

```cpp
#include "val.h"

int val(int *x) {
	puts("This is Value==");
	printf("X:%d \n", *x);
	return 0;
}
```
sum.h和sum.c



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sum(int *x, int *y);
```

```cpp
#include "sum.h"
#include "val.h"

int sum(int *x, int *y) {
	val(x);
	puts("This is SUM Method!=========HDH");
	return *x + *y;
}
```
get.h和get.c



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get(int *x, int *y);
```

```cpp
#include "get.h"

int get(int *x, int *y) {
	puts("This is get");
	return (*x) * (*y);
}
```

如果我们按照先编译成.o文件，然后连接生成目标可执行文件，我们会发现这个过程是比较痛苦的：



```
[admin@localhost test_c4]$ ls
get.c  get.h  libtest.a  main.c  sum.c  sum.h  val.c  val.h
[admin@localhost test_c4]$ rm -rf libtest.a 
[admin@localhost test_c4]$ ls
get.c  get.h  main.c  sum.c  sum.h  val.c  val.h
[admin@localhost test_c4]$ gcc -o get.o -c get.c
[admin@localhost test_c4]$ gcc -o val.o -c val.c
[admin@localhost test_c4]$ gcc -o sum.o -c sum.c
[admin@localhost test_c4]$ gcc -o main.o -c main.c
[admin@localhost test_c4]$ gcc -o main main.o val.o sum.o get.o
[admin@localhost test_c4]$ ls
get.c  get.h  get.o  main  main.c  main.o  sum.c  sum.h  sum.o  val.c  val.h  val.o
[admin@localhost test_c4]$ ./main 
This is Value==
X:10 
This is SUM Method!=========HDH
This is Main
Z:30
This is get
Z:400
```






### 静态库

我们将sum.c  val.c  get.c文件生成静态库libtest.a。

1. 首先先要生成.o的文件


`gcc -c sum.c val.c get.c`


2. 然后通过静态库生成命令**ar -rsv**，生成libtest.a文件


`ar -rsv libtest.a sum.o val.o get.o`结果：





```
[admin@localhost test_c4]$ ar -rsv libtest.a sum.o val.o get.o
ar: 正在创建 libtest.a
a - sum.o
a - val.o
a - get.o
[admin@localhost test_c4]$ ls
get.c  get.h  get.o  libtest.a  main.c  sum.c  sum.h  sum.o  val.c  val.h  val.o
```


3. 我们可以通过**ar -t**命令，查看有多少个.o文件




`ar -t libtest.a `结果：





```
[admin@localhost test_c4]$ ar -t libtest.a 
sum.o
val.o
get.o
```


4. 通过连接静态库方式生成可执行文件。



-L：命令主要用于搜索当前的目录。

**-l test**：就是调用我们的libtest.a静态库。我们调用libevent和线程，-l event  -l pthread都是一样的方式。



`gcc -o main main.c -L ./ -l test`


另外一种方式，直接输入静态库的文件名称：


`gcc main.c -o main ./libtest.a `

### 动态库

动态库是指**lib*.so**的文件。

1. 首先我们需要通过**gcc -O -fpic -shared -o** 命令来生成libtest.so这个动态库。


`gcc -O -fpic -shared -o libtest.so sum.c val.c get.c`

2. 我们需要使用动态库来编译main可执行文件。




`gcc main.c -o main ./libtest.so`结果：

```
[admin@localhost test_c4]$ ls
get.c  get.h  libtest.a  libtest.so  main  main.c  sum.c  sum.h  val.c  val.h
[admin@localhost test_c4]$ ./main 
This is Value==
X:10 
This is SUM Method!=========HDH
This is Main
Z:30
This is get
Z:400
```



3. 如果我们改变动态库的名称，则./main就不能运行了，因为main文件依赖于libtest.so文件。



```
[admin@localhost test_c4]$ mv libtest.so libtest2.so
[admin@localhost test_c4]$ ./main 
./main: error while loading shared libraries: ./libtest.so: cannot open shared object file: No such file or directory
```



4. 动态库的查看命令 **ldd**



`ldd libtest2.so`结果：





```
[admin@localhost test_c4]$ ldd libtest2.so 
	linux-vdso.so.1 =>  (0x00007fff4c5fe000)
	libc.so.6 => /lib64/libc.so.6 (0x00007fb2eaabe000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fb2eb08b000)
```


5. 上面的方式是隐式调用，我们也可以通过代码的方式**显示调用**libtest.so动态库。



显示调用需要4个函数支持：

dlopen：打开动态库

dlsym：获取动态库地址

dlerror：捕捉错误信息

doclose：关闭动态库

具体使用，这边不讲解了。











