# Linux c 开发 - Makefile工具 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月26日 14:46:24[initphp](https://me.csdn.net/initphp)阅读数：1951
所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









Makefile是一个c语言的编译工具。如果学过Java，可能会认识Maven工具，makefile也是类似的工作。

Makefile能帮助c语言建立自动化的编译。一旦写好，执行一个make命令就可以编译整个工程。当然编写Makefile文件的时候有很多知识点在里面。这篇文章主要讲解如何编写基础性以及常用的Makefile文件。




### 1. 没有makefile的编译

我们先看一个例子：

val.h和val.c



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
**main.c 入口文件**



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


上面的例子中如果我们编译成功则需要：

1. 首先将val.c编译成中间目标文件 val.o

2. 将get.c编译成中间目标文件 get.o

3. 将sum.c编译成中间目标文件 sum.o

4. 将main.c编译成中间目标文件 main.o

5. 然后将上面的4个中间文件连接起来，编译成目标可执行文件 main

执行的命令如下：




```cpp
gcc -c val.c
gcc -c get.c
gcc -c sum.c
gcc -c main.c
gcc -o main main.o val.o sum.o get.o
```



命令执行完之后，在文件夹目录下会多.o的文件和main 可执行文件。

![](https://img-blog.csdn.net/20150126192627865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**执行 ./main 命令**，输出：



```
[admin@localhost test_c]$ ./main 
This is Value==
X:10 
This is SUM Method!=========HDH
This is Main
Z:30
This is get
Z:400
```



那么问题来了，假如我的项目非常大，有1000个文件要编译怎么办？于是就有了makefile这样的编译工具。




### 2. 使用makefile编译c项目

Centos上首先要安装make的命令工具，可以通过yum的方式安装


`yum install make`

然后我们编写一个makefile的文件的例子（**文章后面都会以这个例子展开**），其中**文件名Makefile**



```
main:get.o sum.o val.o main.o
        gcc -o main get.o sum.o val.o main.o
main.o:main.c
        gcc -c main.c
val.o:val.h val.c
        gcc -c val.c
sum.o:val.h sum.h sum.c
        gcc -c sum.c
get.o:get.h get.c
        gcc -c get.c
clean:
        rm -rf *.o
```



然后我们在当前目录下** 执行make命令**

![](https://img-blog.csdn.net/20150126193741515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

目录下发现已经有编译成功的main：

![](https://img-blog.csdn.net/20150126193838390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

通过makefile文件，我们可以看到我们原先编译一个c程序，需要每一行命令都去敲，现在我们可以放到Makefile文件中，然后执行一下make命令，就能编译成功，这的确是令人比较兴奋的事情。但这仅仅是开始，makefile我们要学的东西很多，继续往下看。





### 3. 细说Makefile

#### 1. Makefile的文件名称

Makefile的文件名称一般为：makefile或者Makefile，我们一般推荐Makefile，因为大写的首字母开头会比较引人注目。

当执行make工具命令的时候，工具会自动找到当前文件夹下Makefile或者makefile这个文件，然后执行里面的内容。




#### 2. 目标可执行文件

先看下上面例子中的第一行：


`main:get.o sum.o val.o main.o`
其中**main 是目标可执行文件**，也就是编译最后成功的文件名称。而冒号后面就是这个可执行文件需要依赖的.o的中间目标文件。

一般目标可执行文件都在文件最前面。







#### 3. 中间目标文件

继续看上面例子中：



```
main.o:main.c
        gcc -c main.c
val.o:val.h val.c
        gcc -c val.c
sum.o:val.h sum.h sum.c
        gcc -c sum.c
get.o:get.h get.c
        gcc -c get.c
```
上面例子中，例如**get.o就是中间目标文件**，而冒号后面是指生成这个get.o中间目标文件需要的依赖：get.h和get.c






#### 4. 生成目标文件的命令

看例子中



```
main:get.o sum.o val.o main.o
        gcc -o main get.o sum.o val.o main.o
```
发现第二行是一行gcc的命令，这行命令代表的意思是将各种中间目标文件连接起来，生成最终的目标可执行文件 main



主要特别注意的是，命令行都是在第二行，而且需要**[tab]键的空格**，否则就会不生效。




#### 5. 换行符 \

如果一行太长，我们可以通过** 符号 \ **来进行换行。



```
main:get.o sum.o val.o \
 main.o
```


#### 6. clean 清除操作



```cpp
clean:
        rm -rf *.o
```
clean不是一个目标文件，而是一个动作命令。我们可以执行下面的命令




`make clean`这个命令会清除生成的目标中间文件 .o的文件。






#### 7. 执行流程

1. 当用户敲下make命令的时候，make工具会去找当前c工程目录下的Makefile或者makefile这个文件。

2. 找到Makefile这个文件后，他会找到第一个目标文件（main），然后去当前c工程的目录下找是否有main这个可执行的文件，然后会去判断main这个可执行文件是否是最新的，如果是最新的，则不需要编译。

3. 如果不存在main这个可执行文件，make命令就会去生成.o的中间目标文件，然后再生成main可执行目标文件。

4. 如果main存在，make会去比较中间文件.o文件是否有更新，如果是最新的则不更新，不是最新的则继续往下找依赖，找到有需要更新的目标文件的时候就会执行编译命令。

**5. 说白了，整个make的依赖性，make会一层层的去找依赖关系，并且比较是否需要更新，直到编译出最终的可执行的目标文件。**

举个例子，如果我在上面的c语言程序例子中，只改动**main.c**这个文件，则执行make命令，命令只会编译main.o和连接生成main可执行文件两步，而不是全部都重新编译一遍。因为val.o get.o 和sum.o并没有改动，而且是最新的，所以不需要编译。如果项目比较大，这样的方式可以加快编译速度。

![](https://img-blog.csdn.net/20150126201708156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




#### 8. 变量使用

makefile中也可以使用变量。变量可以用 **${object} 或者 $(object)**这样的方式使用。

使用变量的好处就是大大简化了Makefile编写的难度，增强了Makefile文件的可维护性。


我们改造上面的Makefile例子：



```
object = get.o sum.o val.o \
main.o
main:${object}
        gcc -o main ${object}
main.o:main.c
        gcc -c main.c
val.o:val.h val.c
        gcc -c val.c
sum.o:val.h sum.h sum.c
        gcc -c sum.c
get.o:get.h get.c
        gcc -c get.c
clean:
        rm -rf *.o
```





#### 9. make的自动推导功能

我们上面的例子中可以看到：



```
main.o:main.c
        gcc -c main.c
val.o:val.h val.c
        gcc -c val.c
sum.o:val.h sum.h sum.c
        gcc -c sum.c
get.o:get.h get.c
        gcc -c get.c
```
每次我们写一个中间目标文件的时候，都需要将相应的依赖关系写出来，然后写上gcc -c xxx.c的命令。



make工具有一个自动推导的功能：

例如sum.o文件，make会自动推导出依赖的文件 sum.c，于是在sum.o冒号后面就可以将sum.c隐藏

并且有了sum.c的推导，make工具可以可以推导出下面的gcc命令：gcc -c sum.c。于是这个gcc的命令行也可以隐藏了。

于是我们继续改进上面的例子，会发现现在简化了很多：



```
object = get.o sum.o val.o \
main.o
main:${object}
        gcc -o main ${object}
main.o:get.h sum.h
val.o:val.h
sum.o:val.h sum.h
get.o:get.h
clean:
        rm -rf *.o
```





#### 10. include引用其他文件

通过**include <filename>**，可以将别的Makefile文件引入进来。

例如我们object这个变量内容放入test.vm文件中：



```
object = get.o sum.o val.o \
main.o
```
看下Makefile文件的使用：



```
include test.vm
main:${object}
        gcc -o main ${object}
main.o:get.h sum.h
val.o:val.h
sum.o:val.h sum.h
get.o:get.h
clean:
        rm -rf *.o
```

这个时候，我们执行make命令，发现Makefile和之前都放置在一起的时候是一样的效果。




#### 11. .PHONY 声明伪目标

可以用**.PHONY**来声明clean这样的不生成可执行文件的伪目标。可以看下Makefile改造后的例子：



```
object = get.o sum.o val.o \
main.o
main:${object}
        gcc -o main ${object}
main.o:get.h sum.h
val.o:val.h
sum.o:val.h sum.h
get.o:get.h
.PHONY: clean cleanall
clean:
        rm -rf *.o
cleanall:clean
        rm -rf main
```
其中cleanall 既删除main文件，也执行clean下的命令，因为cleanall依赖于 clean伪目标。






#### 12. 生成多个可执行文件

如果要生成多个可执行文件怎么办？大家都知道，Makefile的终极生成的目标文件只有一个，多个可以通过依赖的方式来进行生成：



```
object = get.o sum.o val.o \
main.o
main:test ${object}
        gcc -o $@ ${object}
test:${object}
        gcc -o $@ ${object}
main.o:get.h sum.h
val.o:val.h
sum.o:val.h sum.h
get.o:get.h
.PHONY: clean cleanall
clean:
        rm -rf *.o
cleanall:clean
        rm -rf main
```







#### 13. 常用变量和符号

**$(MAKE) **：是make命令

**$(CC)** ：是gcc的命令

**$@**：可以获取到当前的目标

**@echo**： 这个命令是输出一行字符串。make -s 则只显示echo输出的命令，不显示执行的命令行。

如果是多条命令的，可以通过换行来实现；如果是两条命令有关联，可以通过;号来分隔。

忽略命令的出错情况，可以在命令前面加 - 符号，例如：-rm -rf *.o


可以看下例子：



```
object = get.o sum.o val.o \
main.o
main:$(object)
        @echo it is compile main.c now!..............
        $(CC) -o $@ $(object)
        $(MAKE) clean
main.o:get.h sum.h
val.o:val.h
sum.o:val.h sum.h
get.o:get.h
.PHONY: clean cleanall
clean:
        -rm -rf *.o
cleanall:clean
        -rm -rf main
```

定义变量：



```
object = get.o sum.o val.o \
main.o
```



使用变量：



```
$(object)
foo = $(object) test.o
```



**if语句，未定义：undefined**



```
ifeq (0,${MAKELEVEL})
    mkdir test_path
endif
```


空格使用：





```
nullstring :=
space := $(nullstring)
```








#### 14. 嵌套执行Makefile

有些情况下，我们会把Makefile 不同模块和不同功能的源文件放到不同的目录下，这个时候我们可以通过：

**@(make) -C src/   **这样的命令来执行。

这个命令相当于先进入src目录，再执行make命令。

如果想让你的变量传递到下一层的Makefile中，则可以通过下面的命令**export**，如果不想传递则**unexport**：


`export variable = value`



#### 15. 定义命令包

如果命令经常使用到，可以定义命令包：



```
define test
mkdir test
endef
```
使用：




`$(test)`



#### 16. 文件搜寻

当项目比较大的时候，源文件会按照不同的目录去分类，这个时候就需要通过**VPATH**来搜索。VPATH是makefile的一个特殊变量，定义了源文件的存放目录。


`VAPTH=src:../header`上面的意思就是回去src目录和../header目录查询源文件。
























