# Python函数式实现单例特性 - =朝晖= - 博客园
# [Python函数式实现单例特性](https://www.cnblogs.com/dhcn/p/7124886.html)
      传统的单例一般是基于类的特性实现，[Python](http://lib.csdn.net/base/python)模块是天生的单例，下面来个简单的借助模块和函数实现单例特性：
- gdb = None  
- def get_gdb():  
- global gdb  
- if gdb is None:  
-         gdb = initGDB()  
- return gdb  
使用这个单例：
- import get_gdb  
- gdb = get_gdb()  
