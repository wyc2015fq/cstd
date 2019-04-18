# 使用boost.python进行混合开发 - wishchinYang的专栏 - CSDN博客
2014年02月18日 18:57:37[wishchin](https://me.csdn.net/wishchin)阅读数：2297
**使用boost.python进行混合开发**
原文请参照官网和各方参考，本文有改动！
参考链接：[http://blog.csdn.net/lanbing510/article/details/12197901](http://blog.csdn.net/lanbing510/article/details/12197901)
**引言：**
**[1  简介：](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1)**
Boost.Python 是[**Boost**](http://www.boost.org/)的一个组件。而 Boost是目前很红火的准 C++标准库，它提供了很多组件使得人们可以用 C++语言更方便地实现更多的功能。
 Boost.Python就是 Boost众多组件中的一个。但它是个特例，它的目的不是单纯地增强 C++语言的功能，而是希望利用 C++语言来增强 Python语言的能力。使用 Boost.Python库，我可以方便地将 C++语言写的程序库用于 Python语言，可以用 Python更好地完成更多的任务。
好吧，我承认，我忘了说很重要的一点。那就是，通过 Boost.Python，我们不仅仅可以用 C++来扩展 Python，我们还可以将 Python嵌入 C++。其实 Python运行环境本身就提供了一套嵌入 Python到其它语言的 API，不过是 C语言写的。有了这套 API，我们就完全可以将 Python嵌入到 C/C++语言程序中去。但是，由于 Python本身是一门面向对象的、动态类型的语言，还带垃圾收集，而 C是个面向过程的、静态类型的、不带垃圾收集的程序设计语言。因此，直接使用这套 C API非常痛苦。
 Boost.Python用面向对象 +模板的方法将这套 C API重新包装了一下，我们用起来就清爽多了。不过，目前这个包装还不完善，因此可能还是需要直接使用一部分 Python C API。等它长大了我再来介绍它。**[](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1.1)**
******[1.1  Boost.Python的特性](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1.1)******
**[](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1)**
目前 Boost.Python 特性包括：
o支持 C++引用和指针
oGlobally Registered Type Coercions
o自动跨模块类型转换
o高效的函数重载
o C++异常到 Python异常的转化
o参数默认值
o关键字参数
o在 C++程序中访问 Python语言中的对象
o导出 C++迭代器为 Python迭代器
o文档字符串
**[](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1)[](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1.2)**
**[](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1)**[1.2  跟其它工具的比较](http://bug.codecarver.org/python/boost-Z-H-1.html#node_toc_node_sec_1.2)****
目前有多个工具可以实现跟 Boost.Python类似的功能，如[**SWIG**](http://www.swig.org/)，[**SIP**](http://www.sip.org/)等。但是它们有很大的不同。SWIG和 SIP都定义了一种接口描述语言。我需要先写一个接口描述文件，用于描述我要导出的 C++函数和类。然后通过一个翻译器，将接口描述文件翻译成 C++程序。最后编译连接生成的 C++程序来生成扩展库。而 Boost.Python用于导出 C++函数和类的时候，我需要添加的也是 C++的代码，这是 Boost.Python的最大特点之一。
SWIG 比较适合用来包装 C语言程序，最近也开始增强一些对 C++的支持，但是到目前还不支持嵌套类等 C++特性。SIP似乎除了用在包装[**Qt**](http://www.trolltech.com/)库之外，就没几个人用。而 Boost.Python可能是这三者之间对 C++支持最好的一个。不过 Boost.Python也有缺点，就是它使用了大量模板技巧，因此当要导出的元素比较多时，编译非常慢。不过幸好作为“胶水”，我并不需要经常修改和重编译，而且如果使用预编译头的话可以进一步提高编译速度。
Boost.Python 的另外一个优点是，它的设计目标就是让 C++程序库可以透明地导出到 Python中去。即在完全不修改原来 C++程序的情况下，导出给 Python用。在这种设计理念下设计出来的 Boost.Python比同类工具支持了给完善的 C++特性，能够最大程度地保证不修改原 C++程序。要知道贸然修改别人的程序，往往会带来许多难以察觉的错误。
**1.3. 进行环境搭建**：Boost1.53 可以直接进行exe安装，省去了编译的麻烦；官网可下载 boost_1_53_0-msvc-10.0-64.exe
![](https://img-blog.csdn.net/20140217191138718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2. 建立一个dll工程，命名为例如CSLIC：进行代码练习**
   2.1. 从最小的C函数开始
添加包含路径和库路径之后，添加以下包含：
```cpp
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/to_python_converter.hpp>
void dummyFunc(){
    cout << "Dummy function called!" << endl;
}
```
对应的Wrapper为：
```cpp
BOOST_PYTHON_MODULE(CSLIC) {
def(fun, dummyFunc)；
}
```
这里需要留意的是，对应的MODULE里边的那个名字必须和 命名为例如CSLIC 里边制定的库 名字完全一样，否则python导入对应的模块时候会报错误。
关于第一次环境搭建，我们需要注意的有四点：
1.在编译时，需要让编译器知道 Boost.Python和 Python的头文件所在目录；
2.在连接时，需要让连接器知道 Boost.Python和 Python的库文件所在目录；
3.在连接时，让连接器知道我们要生成的是动态连接库，并且注意动态连接库的主文件名要跟模块名一致；
4.在运行 Python解释器并装入 Baby模块时，需要在当前目录或系统目录下找得到 Boost.Python和 Baby模块对应的动态连接库；
如果使用不同的操作系统、编译器或者 IDE、不同版本的 Python运行环境或 Boost.Python库，成功运行上面的例子需要的设置可能不同，但我们只要注意保证上面四点，应该不会有什么大问题。
2.2.  构建类和结构体（一个小例子）
```cpp
class Complex{
 public:
     double real;
     double imag;
     Complex(double rp,double ip);
    double GetArg()const;
 };
   使用以下胶水代码来包装：
class_<Complex>("Complex", init<double,double>())
   .def_readwrite("real", &Complex::real)
   .def_readwrite("imag", &Complex::imag)
   .def("GetArg", &Complex::GetArg)
```
   胶水代码的意思是，先构造一个临时对象，该对象的类型是 init<double, double> (模板类 init的一个实例)，然后用字符串 "Complex"和这个临时对象构造另一个临时对象，该对象的类型是 class_<Complex>
 (模板类 class_的一个实例)。然后调用第二个临时对象的 def_readwrite方法，该方法返回这个对象本身，因此可以接着再调用这个对象的 def_readwrite方法和 def方法。
一个完整的例子：
```cpp
#include<cmath>
 #include<boost/python.hpp>//包含 Boost.Python的头文件
 class Complex   {          //复数类
 public:
      double real;          //表示实部的成员
      double imag;          //表示虚部的成员
     //构造函数，以及初始化列表
     Complex(double rp,double ip):
         real(rp),          //初始化实部
         imag(ip)   {         //初始化虚部
     }
   
    //获取复数的幅角
    double GetArg()const{
          return atan2(imag, real);
      }
 };
```
```cpp
usingnamespace boost::python; // 引入命名空间
 BOOST_PYTHON_MODULE(CSLIC) {//胶水代码入口，导出一个名为“CSLIC”的模块
     //构造一个类型为 "boost::python::class_<Complex>"的对象 pyComplex
    //构造参数为字符串 "Complex"
    //表示要将 C++类 Complex导出到 Python中去，名字也叫 "Complex"
     class_<Complex> pyComplex("Complex", no_init);
      //导出它的构造方法，声明它的构造方法有两个 double类型的参数
     pyComplex.def(init<double,double>());
     //导出它的公有成员 real，
     //该成员在 Complex类中的位置是 &Complex::real
     //导出到 Python中之后的名字也是 "real"
    pyComplex.def_readwrite("real", &Complex::real);
    //导出它的公有成员 imag，
     //该成员在 Complex类中的位置是 &Complex::imag
     //导出到 Python中之后的名字也是 "imag"
     pyComplex.def_readwrite("imag", &Complex::imag);
     //导出它的成员方法 GetArg
    //该方法在 Complex类中的入口是 &Complex::GetArg
   //导出到 Python中之后的名字也是 "GetArg"
     pyComplex.def("GetArg", &Complex::GetArg);
 }
```
生成动态连接库 ADT.so (Linux下)或
 ADT.dll (Windows下)。然后我可以执行一段 Python脚本来验证一下：
**我遇到的问题：**
    (1):**对于函数重载：产生模板库不能展开问题 ErrorC2784，**ErrorC2780****
****    解决方法：消除重载，函数改名......****
下边的是一个成员函数重载的例子（其实和Free funciton的唯一差别就是声明导出的时候，要在class_<T>对象的那个.后边加def，而一般函数只要直接Def即可）:
```cpp
////////////////////////////////////////////////////////////////////////////////
//Overloadding
struct X{ 
    bool f(int a){
        return true;
    } 
    bool f(int a, double b) {
        return true;
    } 
    bool f(int a, double b, char c){
        return true;
    } 
    int f(int a, int b, int c){
        return a + b + c;
    };
};
```
声明的时候，则要费时一点：
```cpp
//helpers
    bool    (X::*fx1)(int)              = &X::f;
    bool    (X::*fx2)(int, double)      = &X::f;
    bool    (X::*fx3)(int, double, char)= &X::f;
    int     (X::*fx4)(int, int, int)    = &X::f;
    class_<X>("X")
        .def("f", fx1)
        .def("f", fx2)
        .def("f", fx3)
        .def("f", fx4)
```
（2）：对于不能正常初始化问题：利用WIndepends查看后，缺少两个com组件；
          打开dll文件，发现：
错误: 由于在隐性依赖模块中丢失导出函数，至少有一个模块有不能解析的导入。
错误: 发现不同 CPU 类型的模块。
警告: 至少有一个延时加载模块没找到。
警告: 由于在延时加载依赖模块中丢失导入函数，至少有一个模块具有不能解析的导入。
     由于VS对64位os的支持不完善，导致大量使用32位库，造成不兼容；
    解决方法： 切换到32位平台。
