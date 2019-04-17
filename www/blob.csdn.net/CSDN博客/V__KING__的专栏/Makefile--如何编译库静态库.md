# Makefile--如何编译库静态库 - V__KING__的专栏 - CSDN博客





2017年07月18日 09:27:12[v__king__](https://me.csdn.net/V__KING__)阅读数：312








## 一. 编译模板

背景： 

1. 你有1.c， 2.c，3.c三个源文件 

2. 其实你是可以在3.c文件中这样：
```
#include "1.c"
#include "2.c"
int func3(){
    func1();
    func2();
    return 0;
}
```

这样在静态编译库的时候，3.c中的函数可能用到2.c中的函数func2()。这个时候，可以再Makefile中表明，只需要编译出来3.o的文件就行了，而不需要将1.o，2.o的文件都编译出来。




