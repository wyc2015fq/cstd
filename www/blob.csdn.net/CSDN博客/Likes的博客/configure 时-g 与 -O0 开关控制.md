# configure 时-g 与 -O0 开关控制 - Likes的博客 - CSDN博客
2018年12月14日 16:39:01[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：44标签：[configure																[CFLAGS																[export																[O0																[编译优化](https://so.csdn.net/so/search/s.do?q=编译优化&t=blog)](https://so.csdn.net/so/search/s.do?q=O0&t=blog)](https://so.csdn.net/so/search/s.do?q=export&t=blog)](https://so.csdn.net/so/search/s.do?q=CFLAGS&t=blog)](https://so.csdn.net/so/search/s.do?q=configure&t=blog)
个人分类：[C++](https://blog.csdn.net/songchuwang1868/article/category/7898933)
实验了一下调试tslib
首先是编译。
1. 允许调试，如下配置。
```bash
./configure --enable-debug --prefix=/opt
```
但这样虽然增加了-g 选项，但仍然是-O2 优化，gdb debug 时会跳行，不爽。
2. 变为-O0 选项，可以导出CFLAGS 变量，如下进行。
```bash
export CFLAGS='-DGCC_HASCLASSVISIBILITY -O0 -Wall -W'
```
再重新配置，编译即可。
