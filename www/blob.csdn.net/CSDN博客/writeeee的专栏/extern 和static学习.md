# extern 和static学习 - writeeee的专栏 - CSDN博客
2016年05月13日 20:48:15[writeeee](https://me.csdn.net/writeeee)阅读数：236
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)
## extern 学习
extern 有两种用途
- extern “C”  functionname()
- extern 全局变量（exter 具有开放性）
第一种用法主要作用是使导出的方法没有加一些随即后缀．方法名清晰
> 
注意事项１．单方面修改extern 函数原型时（即开放接口变了，调用不变），编译不报错 
    （坑啊，曾经查了一周的问题，结果发现是客户端调用多传了一个参数）
第二种一般用于一个文件中定义变量，另外一个文件中通过extern 声明可直接使用，
> 
**１．一个应用场景＿设置全局变量**
  global.cpp 定义全局变量 　  
  global.h　中为extern 的声明（而不是定义），这样其他文件只要引用该头文件即可使用全局变量． 
**２．注意**
  在使用中注意extern 声明的类型要与定义的类型完全一致
参考
> 
[http://www.cnblogs.com/yc_sunniwell/archive/2010/07/14/1777431.html](http://www.cnblogs.com/yc_sunniwell/archive/2010/07/14/1777431.html)
## static 学习
static 有两层含义
> 
**静态性**，指生命周期从程序开始到程序结束，即使是在函数内部定义的static也是同样的声明周期． 
**封闭性**，即变量和函数局部于文件．
封闭性是说　　 
A.cpp 定义了　static int iA 
B.cpp 声明　extern int A;    　(编译多个文件命令如下)
> 
g++ A.cpp  B.cpp -o test
编译会发生错误　对‘iA’未定义的引用　，因为static拒绝了外部的访问．所以他
## exertn 与 static的异同
extern 和　static 都可使变量生命周期在整个程序运行期间 
extern 具有开放性，static 具有封闭性，他们本身是矛盾的因此不可在一起用，以下的应用是错误的
> 
extern static int myvar;//错误的声明
