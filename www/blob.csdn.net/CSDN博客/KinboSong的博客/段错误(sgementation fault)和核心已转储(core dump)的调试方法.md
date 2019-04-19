# 段错误(sgementation fault)和核心已转储(core dump)的调试方法 - KinboSong的博客 - CSDN博客
2018年04月12日 21:01:16[songjinbo3](https://me.csdn.net/KinboSong)阅读数：403
本文主要介绍gdb+core的调试方法，其他几种方法的介绍参考：[段错误产生原因及调试方法汇总](https://blog.csdn.net/oscarjulia/article/details/74256997)
一、printf方法调试
二、gdb方法调试
三、gdb+core文件的方法调试，步骤如下，具体参考：[gdb+core调试](https://blog.csdn.net/k346k346/article/details/48344263)
    1、设置生成core文件，有以下两种方法
    2、设置core文件的文件名和生成路径   
    3、gdb调试core
四、objdump或者elfdump调试
五、catchsegv调试
