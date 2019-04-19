# Linux C语言调用C++动态链接库 - maopig的专栏 - CSDN博客
2017年02月15日 22:13:22[maopig](https://me.csdn.net/maopig)阅读数：769
# [Linux C语言调用C++动态链接库](http://blog.csdn.net/sjin_1314/article/details/20958149)
标签： [C调用C++库](http://www.csdn.net/tag/C%E8%B0%83%E7%94%A8C%2B%2B%E5%BA%93)
2014-03-10 22:56
3744人阅读[评论](http://blog.csdn.net/sjin_1314/article/details/20958149#comments)(0)收藏[举报](http://blog.csdn.net/sjin_1314/article/details/20958149#report)
![](http://static.blog.csdn.net/images/category_icon.jpg)分类：
【Linux应用开发】*（48）*![](http://static.blog.csdn.net/images/arrow_triangle%20_down.jpg)
版权声明：本文为博主原创文章，未经博主允许不得转载。
如果你有一个c++做的动态链接库.so文件,而你只有一些相关类的声明,那么你如何用c调用呢,
C++创始人在编写C++的时候，C语言正盛行，他不得不让C++兼容C。C++最大的特性就是封装，继承，多态，重载。而这些特性恰恰是C语言所不具备的。至于多态，核心技术是通过虚函数表实现的，其实也就是指针。而对于重载，与C语言相比，其实就是编译方式不同而已：
 C++编译方式和C编译方式。对于函数调用，编译器只要知道函数的参数类型和返回值以及函数名就可以进行编译连接。那么为了让C调用C++接口或者是说C++调用C接口，就必须是调用者和被调用者有着同样的编译方式。这既是extern "C"的作用，extern “C”是的程序按照C的方式编译。
下面具体看下面的代码：
1、myclass.h 
**[cpp]**[view plain](http://blog.csdn.net/sjin_1314/article/details/20958149#)[copy](http://blog.csdn.net/sjin_1314/article/details/20958149#)[print](http://blog.csdn.net/sjin_1314/article/details/20958149#)[?](http://blog.csdn.net/sjin_1314/article/details/20958149#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/228590/fork)
- #include <iostream>
- usingnamespace std;  
- 
- class Myclass {  
- public:  
-     Myclass(){}  
-     ~Myclass(){}  
- void Operation();  
- };  
2、myclass.cpp
**[cpp]**[view plain](http://blog.csdn.net/sjin_1314/article/details/20958149#)[copy](http://blog.csdn.net/sjin_1314/article/details/20958149#)[print](http://blog.csdn.net/sjin_1314/article/details/20958149#)[?](http://blog.csdn.net/sjin_1314/article/details/20958149#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/228590/fork)
- #include "myclass.h"
- usingnamespace std;  
- void Myclass::Operation()  
- {  
-     cout << "Hi my name is sjin" <<endl;  
- }  
3 interface.h
**[cpp]**[view plain](http://blog.csdn.net/sjin_1314/article/details/20958149#)[copy](http://blog.csdn.net/sjin_1314/article/details/20958149#)[print](http://blog.csdn.net/sjin_1314/article/details/20958149#)[?](http://blog.csdn.net/sjin_1314/article/details/20958149#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/228590/fork)
- #ifdef __cplusplus
- extern"C"{  
- #endif
- 
- void interface();  
- 
- #ifdef __cplusplus
- }  
- #endif
4 interface.cpp
**[cpp]**[view plain](http://blog.csdn.net/sjin_1314/article/details/20958149#)[copy](http://blog.csdn.net/sjin_1314/article/details/20958149#)[print](http://blog.csdn.net/sjin_1314/article/details/20958149#)[?](http://blog.csdn.net/sjin_1314/article/details/20958149#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/228590/fork)
- #include "myclass.h"
- #include "interface.h"
- 
- #ifdef __cplusplus
- extern"C"{  
- #endif
- 
- void interface()  
- {  
-     Myclass obj;  
-     obj.Operation();  
- }  
- 
- #ifdef __cplusplus
- }  
- #endif
5 main.c
**[cpp]**[view plain](http://blog.csdn.net/sjin_1314/article/details/20958149#)[copy](http://blog.csdn.net/sjin_1314/article/details/20958149#)[print](http://blog.csdn.net/sjin_1314/article/details/20958149#)[?](http://blog.csdn.net/sjin_1314/article/details/20958149#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/228590/fork)
- #include "interface.h"
- 
- int main()  
- {  
-     interface();  
- return 0;  
- }  
具体编译流程
1】首先生成动态库
**[cpp]**[view plain](http://blog.csdn.net/sjin_1314/article/details/20958149#)[copy](http://blog.csdn.net/sjin_1314/article/details/20958149#)[print](http://blog.csdn.net/sjin_1314/article/details/20958149#)[?](http://blog.csdn.net/sjin_1314/article/details/20958149#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/228590/fork)
- g++  myclass.cpp interface.cpp -fPIC -shared -o libtest.so  
2】将动态库拷贝的/usr/lib目录下
3】编译main.c
gcc main.c -L.  -ltest
4】运行./a.out
![](https://img-blog.csdn.net/20140310225513468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2ppbl8xMzE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考资料：
http://blog.csdn.net/feiyinzilgd/article/details/6723882
http://www.bdqn.cn/news/201309/11368.shtml
