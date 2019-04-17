# C语言使用头文件时与"*.h" - Machine Learning with Peppa - CSDN博客





2018年01月08日 20:46:22[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：224
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)









使用尖括号的话，编译时会先在系统include目录里搜索，如果找不到才会在源代码所在目录搜索；使用双引号则相反，会先在源代码目录里搜索。这就意味着，当系统里（如/usr/include/里）有一个叫做[math.h](https://www.baidu.com/s?wd=math.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dBPhnvPADsnvD4PWNWrHF-0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHR3rjbdPj03)的头文件，而你的源代码目录里也有一个你自己写的[math.h](https://www.baidu.com/s?wd=math.h&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dBPhnvPADsnvD4PWNWrHF-0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHR3rjbdPj03)头文件，那么使用尖括号时用的就是系统里的；而使用双引号的话则会使用你自己写的那个。

比如c:\program
 files\microsoft visual stdio\vc98\include
引号""允许在用户当前配置的目录（比如存放.cpp/.c的目录）相同的目录下引用这个文件




