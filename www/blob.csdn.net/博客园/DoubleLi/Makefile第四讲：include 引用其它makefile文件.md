# Makefile第四讲：include 引用其它makefile文件 - DoubleLi - 博客园






#### main.cpp

```cpp
#include "classes/fun.h"
int main() {
    Test::display("Hello makefile");
    return 0;
}
```

#### classes/fun.h

```cpp
#include <iostream>
using namespace std;

class Test {
public:
    void static display(const char *);
};
```

#### classes/fun.cpp

```cpp
#include "fun.h"
void Test::display(const char *str) {
    cout <<str <<endl;
}
```

#### classes/makefile

```
test.o:classes/fun.cpp
    g++ -c classes/fun.cpp -o test.o
```

#### makefile

```
test: test.o main.o
    g++ test.o main.o -o test

main.o:main.cpp
    g++ -c main.cpp -o main.o

include classes/makefile

clean:
    rm main.o test.o test
```

#### `include classes/makefile`中的include关键字就是引用其它的makefile，值得注意的是，在被引用的makefile文件中的代码，它的路径是相对于引用的makefile文件的路径（注意到`test.o:classes/fun.cpp`），思考一下：我可以把`include classes/makefile`这句写在开头吗？答案当然是不能，至于为什么，在[Makefile第一讲：一个简单的Makefile](http://www.oschina.net/[http://my.oschina.net/u/140638/blog/269777)找









