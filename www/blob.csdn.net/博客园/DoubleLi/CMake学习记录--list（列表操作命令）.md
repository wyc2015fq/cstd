# CMake学习记录--list（列表操作命令） - DoubleLi - 博客园






CMake是一个跨平台的工程管理工具，能方便的把工程转换为vs各个版本、Borland Makefiles、MSSYS Makefiles、NMake Makefiles等工程，对于经常在不同IDE下使用管理同一个工程提供了很大的便利。自己把使用过程中的点点滴滴记录下来。

`**list **`列表操作命令。

1、 list(LENGTH <list> <output variable>)

2、 list(GET <list> <element index> [<element index> ...] <output variable>)

3、 list(APPEND <list> <element> [<element> ...])

4、 list(FIND <list> <value> <output variable>)

例：(摘自WebKit-r169998的cmakliset文件）





**[plain]**[view plain](http://blog.csdn.net/Qyee16/article/details/8985698#)[copy](http://blog.csdn.net/Qyee16/article/details/8985698#)

[print](http://blog.csdn.net/Qyee16/article/details/8985698#)[?](http://blog.csdn.net/Qyee16/article/details/8985698#)

- # -----------------------------------------------------------------------------  
- # Determine which port will be built  
- # -----------------------------------------------------------------------------  
- #定义ALL_PORTS  
- set(ALL_PORTS Efl WinCE GTK)  
- set(PORT "NOPORT" CACHE STRING "choose which WebKit port to build (one of ${ALL_PORTS})")  
- set(PORT GTK)  
- #在ALL_PORTS中查找 ${PORT} == "GTK",并返回在ALL_PORTS 之中的索引 RET  
- list(FIND ALL_PORTS ${PORT} RET)  
- if (${RET} EQUAL -1)  
-     message(FATAL_ERROR "Please choose which WebKit port to build (one of ${ALL_PORTS})")  
- endif ()  










  　　list(INSERT <list> <element_index> <element> [<element> ...])

  　　list(REMOVE_ITEM <list> <value> [<value> ...])

  　　list(REMOVE_AT <list> <index> [<index> ...])

  　　list(REMOVE_DUPLICATES <list>)

  　　list(REVERSE <list>)

  　　list(SORT <list>)









