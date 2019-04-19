# linux下c和c++互相调用 - maopig的专栏 - CSDN博客
2017年02月15日 22:20:40[maopig](https://me.csdn.net/maopig)阅读数：1062

c调用cpp
创建个目录 创建4个文件 c.c--c文件 cpp.cpp--c++文件 cpp.hh--c++声明文件 Makefile
c.c
**[javascript]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- #include "cpp.hh"
- 
- int main()  
- {  
-     cpp_fun();  
- }  
cpp.cpp
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- #include "cpp.hh"
- #include <stdio.h>
- 
- #ifdef __cplusplus
- extern"C"
- {  
- #endif
- 
- void cpp_fun()  
- {  
-     printf("cpp_fun\n");  
- }  
- 
- #ifdef __cplusplus
- }  
- #endif
cpp.hh
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- #ifdef __cplusplus
- extern"C" {  
- #endif
- 
- void cpp_fun();  
- 
- #ifdef __cplusplus
- }  
- #endif
Makefile
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- #   c  调用   cpp     函数
- c:  
-     gcc -c *.c  
-     g++ -c *.cpp  
-     gcc -o c_test *.o -lstdc++  
- #   g++ -o c_test *.o
- 
- clean:  
-     rm *.o c_test  
make 一下 运行./c_test
执行显示如下:
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- cpp_fun  
这里Makefile中用g++ -o c_test *.o也可以编译成功 用gcc需要指定包含c++标准库libstdc++.so
make clean 清除
^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^_ ^
c++调用c
创建个目录 创建4个文件 c.c--c文件 cpp.cpp--c++文件 c.h--c声明文件 Makefile
c.c
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- #include "c.h"
- #include <stdio.h>
- 
- void c_fun()  
- {  
-     printf("c_fun\n");  
- }  
c.h
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- void c_fun();  
cpp.cpp
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- extern"C"{  
- #include "c.h"
- };  
- 
- int main()  
- {  
-     c_fun();  
- return 0;  
- }  
Makefile
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- #   cpp  调用 c   函数
- cpp:  
-     gcc -c *.c  
-     g++ -c *.cpp  
-     g++ -o cpp_test *.o  
- 
- clean:  
-     rm *.o cpp_test  
make一下 然后执行./cpp_test
执行显示结果如下:
**[cpp]**[view plain](http://blog.csdn.net/orz415678659/article/details/8307891#)[copy](http://blog.csdn.net/orz415678659/article/details/8307891#)
- c_fun  
make clean 清除
