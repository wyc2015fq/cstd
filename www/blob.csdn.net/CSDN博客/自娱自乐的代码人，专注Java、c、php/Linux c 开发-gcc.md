# Linux c 开发 - gcc - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年10月06日 09:07:21[initphp](https://me.csdn.net/initphp)阅读数：1228
所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









gcc是linux下的一种编译器。c语言通常都是通过gcc进行编译而成。

一般gcc的编译过程如下图：

![](https://img-blog.csdn.net/20141006085636822?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们从最简单的例子开始讲解gcc的一些使用方法。后面我们会具体讲解每一个编译步骤如何来编译的。




常用文件：

.o 文件：linux下的c语言目标文件

.a文件：linux下的静态连接库，相当于多个.o合起来。**静态链接库在编译的时候加载生成目标文件，而在运行的时候不用加载库，对库没有依赖性。**

.so文件：linux动态链接库。**在目标文件运行时加载，手动加载，且对库有依赖性。**





### 1. gcc直接编译

看一个最简单的hello world的例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
int main() {
        printf("Hello World\n");
        return 0;
}
```
我们执行命令：



```cpp
gcc main.c
```



```
[admin@localhost c]$ gcc main.c
[admin@localhost c]$ ls
a.out  main.c
[admin@localhost c]$
```



可以看到多了一个a.out的文件，a.out是一个可执行的文件，我们可以执行一下可以看到输出了"hello world"：



```
[admin@localhost c]$ ./a.out 
Hello World
```



每次生成a.out文件会有些不好玩，我们可以通过下面的命令可以将a.out重命名为我们自己想要的文件名称：



```cpp
gcc main.c -o main //通过-o命令可以重命名
```



```
[admin@localhost c]$ gcc main.c -o main
[admin@localhost c]$ ls
main  main.c
[admin@localhost c]$
```





这个时候再执行./main这个文件：



```
[admin@localhost c]$ ./main 
Hello World
[admin@localhost c]$
```









### 2. gcc预处理



gcc编译过程可以看到最上面的图中有四个步骤：预处理(也称预编译，Preprocessing)、编译(Compilation)、汇编 (Assembly)和连接(Linking)。

我们需要对我们上面那段main.c进行预处理：



```cpp
gcc -E main.c -o main.i
```



这个时候你就能看到文件目录下面多了一个test.i的文件，test.i的文件中可以看到编译器预处理的结果，是将include的内容合并到了test.i文件中。



```
[admin@localhost c]$ gcc -E main.c -o main.i
[admin@localhost c]$ ls
main.c  mian.i
```






### 3. gcc编译为汇编代码



预处理之后，会生成main.i文件，我们对这个文件进行编译成汇编代码


`gcc -S main.i -o main.s //-S是进程编译成汇编代码`

这个时候多了一个main.s的文件，就是进行编译成汇编的文件





```
[admin@localhost c]$ gcc -S main.i -o main.s
[admin@localhost c]$ ls
main.c  main.i  main.s
```






### 4. gcc汇编



生成main.s的汇编代码之后，我们需要将汇编代码进行汇编的处理。生成main.o的文件


`gcc -c main.s -o main.o //-c是进行汇编`
```
[admin@localhost c]$ gcc -c main.s -o main.o
[admin@localhost c]$ ls
main.c  main.i  main.o  main.s
```








### 4. gcc连接

最近将生成的main.o文件连接起来生成最终的可执行文件。




`gcc main.o -o main //生成main可执行文件`
```
[admin@localhost c]$ gcc main.o -o main
[admin@localhost c]$ ls
main  main.c  main.i  main.o  main.s
```





### 4. gcc多个文件的编译



我们新增了一个test.h和test.c的文件，并且在main.c中调用了test.c中的方法，这个时候我们需要进行多个文件的编译。

test.h



```cpp
#include <stdio.h>
#include <stdlib.h>

void test();
```



test.c





```cpp
#include "test.h"
void test() {
        printf("Hello World\n");
}
```



main.c



```cpp
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
int main() {
        test();
        printf("Hello World\n");
        return 0;
}
```


编译命令：





```cpp
gcc test.c main.c -o main
```


或者





```cpp
[admin@localhost c]$ gcc -c test.c -o test.o
[admin@localhost c]$ gcc -c main.c -o main.o
[admin@localhost c]$ gcc test.o main.o -o main
[admin@localhost c]$ ./main 
Hello World
Hello World
```


### 7. 编译的时候提示警告信息

可以通过-Wall命令来提示编译过程中的警告信息，对代码调试之类有帮助



```cpp
gcc main.c -o main -Wall
```






### 8. 指定查找路径



如果我们上面这个例子的头文件放进cc/这个文件夹里面，这个时候main.c中需要下面的命令才能找到这个头文件。

命令：



```cpp
gcc -I cc/  -c main.c -o main.o
```




### 9. 连接库文件

假如我们要连接线程的库文件，可以使用下面的命令



```cpp
gcc main.c -o main -l pthread
```













