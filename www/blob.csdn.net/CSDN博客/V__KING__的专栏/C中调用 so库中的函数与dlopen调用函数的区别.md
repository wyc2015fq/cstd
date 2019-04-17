# C中调用.so库中的函数与dlopen调用函数的区别 - V__KING__的专栏 - CSDN博客





2017年12月11日 18:25:40[v__king__](https://me.csdn.net/V__KING__)阅读数：637








## 一. C中调用.so库中的函数与dlopen调用区别
- 调用及结果 并没有区别
- 区别在于，直接调用需要在工程中加入so的引用 

而用dlopen可以直接使用动态库，更灵活
- 另外，如果直接调用方式，so文件不存在会导致整个程序无法运行 

而用dlopen动态加载，可以运行后再判断是否加载成功，即使失败，还可以保留程序其它功能
- dlopen使用的时候，麻烦的是：相当于需要把.so库中的函数个一个一个的导出声明。
- 如果在插件中调用没有使用dlopen打开的my.so库，有的时候会出现错误。 前提条件是这样的：如果工程中加入了my.so库，但是没有调用my.so中的任何的函数。

## 二. 使用dlopen

参考：[https://www.cnblogs.com/Anker/p/3746802.html](https://www.cnblogs.com/Anker/p/3746802.html)



