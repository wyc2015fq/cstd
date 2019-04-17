# Makefile第三讲：终端传值给Makefile、Makefile传值给C++代码 - DoubleLi - 博客园






摘要 终端传值给Makefile，咋传？只需在终端输入以下命令，那么就可以在Makefile文件中放心大担的使用$(abcde)这个变量了，它的值为BBB

[Makefile](http://www.oschina.net/search?scope=blog&q=Makefile)




### fun.h

```cpp
#include <iostream>

class Test {
public:
    void static display(const char *str);
};
```

### fun.cpp

```cpp
#include "fun.h"

void Test::display(const char *str) {
    std::cout <<str;
}
```

### main.cpp

```cpp
#include "fun.h"

int main() {
#if defined AAA
    Test::display("AAA\n");
#elif defined BBB
    Test::display("BBB\n");
#else
    Test::display("CCC\n");
#endif
    return 0;
}
```

### makefile

```
OBJS = main.o fun.o

test: $(OBJS)
    g++ $(OBJS) -o test

main.o: main.cpp
    g++ -c main.cpp -D$(abcde) -o main.o

fun.o: fun.cpp
    g++ -c fun.cpp -o fun.o

install:
    cp test ~/test

clean:
    rm *.o test
```

### 这是一个多文件时编译makefile的例子，可以看到`fun.h`始终没出现在代码里边，不用疑惑，`fun.cpp`已经include进来了。

### 一、终端传值给Makefile，咋传？只需在终端输入以下命令，那么就可以在Makefile文件中放心大担的使用`$(abcde)`这个变量了，它的值为`BBB`

```bash
$make abcde=BBB
```

### 二、Makefile文件传变量给C++代码，其实这是属于g++的范畴，和makefile无关，只需在g++上加参数`-D`即可，如上例的`g++ -c main.cpp -D$(abcde) -o main.o`，终端通过命令`make abcde=BBB`传变量给Makefile，Makefile又把该变量传给g++，g++在编译时定义该变量，于是main.cpp就可以使用到这个变量（应该是宏`#define BBB 1），如果只是传了一个变量名而不赋值，它的值就是1，如果想赋值，应该 这样：

```
$make abcde=BBB=3
```

### 那么`g++ -c main.cpp -D$(abcde) -o main.o`就会变成：`g++ -c main.cpp -DBBB=3 -o main.o`，在C++代码中便有：`#define BBB 3`

### *复制我例子中的源码，然后在命令行执行：`make abcde=AAA`、`make abcde=BBB`并运行编译出的程序看看有何不同。*










