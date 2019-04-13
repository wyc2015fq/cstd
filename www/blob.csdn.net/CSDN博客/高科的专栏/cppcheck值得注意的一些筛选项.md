
# cppcheck值得注意的一些筛选项 - 高科的专栏 - CSDN博客

2015年08月06日 17:42:06[高科](https://me.csdn.net/pbymw8iwm)阅读数：559




使用完cppcheck进行C++代码检测之后，可能筛选起来很麻烦，一般常见的优化有 emptiness，就是当你使用stl的时候，最好用empty替代size
还有就是 leak

