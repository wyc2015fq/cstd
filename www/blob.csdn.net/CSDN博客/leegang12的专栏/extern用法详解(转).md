# extern用法详解(转)  - leegang12的专栏 - CSDN博客
2011年05月21日 18:18:00[leegang12](https://me.csdn.net/leegang12)阅读数：353标签：[编译器																[include																[语言																[c																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[VC++语言](https://blog.csdn.net/leegang12/article/category/824576)
一，extern用法：
extern 告诉编译器这个变量或这个函数是存在的。
1 基本解释
　　extern可以置于变量或者函数前，以标示变量或者函数的定义在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义。另外，extern也可用来进行链接指定。
2 问题：extern 变量
　　在一个源文件里定义了一个数组：char a[6];  
     在另外一个文件里用下列语句进行了声明：extern char *a；  
　　请问，这样可以吗？  
　　答案与分析：
　　1)、不可以，程序运行时会告诉你非法访问。原因在于，指向类型T的指针并不等价于类型T的数组。extern char *a声明的是一个指针变量而不是字符数组，因此与实际的定义不同，从而造成运行时非法访问。应该将声明改为extern char a[ ]。
　　2)、例子分析如下，如果a[] = "abcd",则外部变量a=0x61626364 (abcd的ASCII码值)，*a显然没有意义。　显然a指向的空间（0x61626364）没有意义，易出现非法内存访问。
　　3)、这提示我们，在使用extern时候要严格对应声明时的格式，在实际编程中，这样的错误屡见不鲜。
　　4)、extern用在变量声明中常常有这样一个作用，你在*.c文件中声明了一个全局的变量，这个全局的变量如果要被引用，就放在*.h中并用extern来声明。
　　3 问题：extern 函数1
　　常常见extern放在函数的前面成为函数声明的一部分，那么，C语言的关键字extern在函数的声明中起什么作用？
　　答案与分析：
　　如果函数的声明中带有关键字extern，仅仅是暗示这个函数可能在别的源文件里定义，没有其它作用。即下述两个函数声明没有明显的区别：
extern int f(); 和int f();  
　　当然，这样的用处还是有的，就是在程序中取代include “*.h”来声明函数，在一些复杂的项目中，我比较习惯在所有的函数声明前添加extern修饰。
**4 问题：extern [函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)2**
　　当[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)提供方单方面修改[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)原型时，如果使用方不知情继续沿用原来的extern申明，这样编译时编译器不会报错。但是在运行过程中，因为少了或者多了输入参数，往往会照成系统错误，这种情况应该如何解决？
　　答案与分析：
　　目前业界针对这种情况的处理没有一个很完美的方案，通常的做法是提供方在自己的xxx_pub.h中提供对外部[接口](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BD%D3%BF%DA)的声明，然后调用方include该头文件，从而省去extern这一步。以避免这种错误。
　　宝剑有双锋，对extern的应用，不同的场合应该选择不同的做法。
**5 问题：extern “C”**
　　在[C++](http://www.bc-cn.net/Article/kfyy/cjj/Index.html)环境下使用C[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)的时候，常常会出现编译器无法找到obj模块中的C[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)定义，从而导致链接失败的情况，应该如何解决这种情况呢？
　　答案与分析：
[C++](http://www.bc-cn.net/Article/kfyy/cjj/Index.html)语言在编译的时候为了解决[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)的多态问题，会将[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)名和参数联合起来生成一个中间的[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)名称，而[C语言](http://www.bc-cn.net/Article/kfyy/cyy/Index.html)则不会，因此会造成链接时找不到对应[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)的情况，此时C[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)就需要用extern “C”进行链接指定，这告诉编译器，请保持我的名称，不要给我生成用于链接的中间[函数](http://www.bc-cn.net/Article/Search.asp?Field=Title&ClassID=&keyword=%BA%AF%CA%FD)名。
　　下面是一个标准的写法：
//在.h文件的头上
#ifdef __cplusplus
#if __cplusplus
extern "C"{
　#endif
　#endif /* __cplusplus */ 
　…
　…
　//.h文件结束的地方
　#ifdef __cplusplus
　#if __cplusplus
}
#endif
#endif /* __cplusplus */
