# Makefile第二讲：打印出内容和使用变量 - DoubleLi - 博客园






摘要 `@echo "开始生成最终执行文件，请稍候..."`这一句便是将一条信息输出到终端，为何前边有个`@`符号呢？有了这个符号该命令本身就不会输出到终端（不理解，自己去掉或者加上`@`符号试试），实验证明，字符串可以不加引号。 看到`OBJS = main.o`了吗？这就是变量的定义，变量定义遵循一般语言的变量命名规则，可以是`_`和`$`开头，变量的值无论是字符串还是什么，都可以不要引号；有一种情况必须不要引号，就像上例这种，具体的....自己尝试吧。

[Makefile](http://www.oschina.net/search?scope=blog&q=Makefile)




#### main.cpp

```cpp
#include <iostream>
using namespace std;

int main() {
    cout <<"Hello makefile";
    return 0;
}
```

#### makefile（还是[第一节](http://my.oschina.net/u/140638/blog/269777)中的代码，稍作修改）

```
OBJS = main.o
test:$(OBJS)
    @echo "开始生成最终执行文件，请稍候..."
    g++ main.o -o test

main.o:main.cpp
    g++ -c main.cpp -o main.o

clean:
    rm main.o test
```
- `[@echo](http://my.oschina.net/echolee1987) "开始生成最终执行文件，请稍候..."`这一句便是将一条信息输出到终端，为何前边有个`@`符号呢？有了这个符号该命令本身就不会输出到终端（不理解，自己去掉或者加上`@`符号试试），实验证明，字符串可以不加引号。
- 看到`OBJS = main.o`了吗？这就是变量的定义，变量定义遵循一般语言的变量命名规则，可以是`_`和`$`开头，变量的值无论是字符串还是什么，都可以不要引号；有一种情况必须不要引号，就像上例这种，具体的….自己尝试吧。
- `$(OBJS)`这是对变量的调用。
- 还可以把shell命令的返回值作赋给变量，例如将当前目录的路径赋值给变量：`CURRENT_DIR = $(shell pwd)`










