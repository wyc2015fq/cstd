# lib库或dll中使用stl做为接口在模块间传入数据，变量定义重复问题。 - 深之JohnChen的专栏 - CSDN博客

2018年09月05日 18:06:36[byxdaz](https://me.csdn.net/byxdaz)阅读数：220


1、error LNK2005: "public: void __thiscall std::basic_ofstream<char,struct std::char_traits<char> > xxx.obj 中定义

出现上面情况，说明多个库中导出了stl的string，导致重复定义。

解决方法：

第一种：在dll或静态库种不要使用stl中变量作为接口。

第二种：在link commandline加上一行/FORCE:MULTIPLE

/FORECE 是用来忽略重复定义。但是该修改之后会在编译的时候产生警告。

warning LNK4088: 因 /FORCE 选项生成了映像；映像可能不能运行

虽然不影响运行，但是如果要消除这些警告，将接口中的stl的string，vector等变量修改成C形式数据结构。

2、在一个lib库中用到了另外一个库的dll，lib库的运行时库是“多线程DLL(/MD)”,（属性 C/C++ ->代码生成->运行库），dll工程使用的运行库是“多线程调试DLL(/MDd)”。导致向dll的接口传递std::string参数时报错。

解决方法：

将lib项目的运行库改为“多线程调试DLL(/MDd)”.

原因分析：

不同的模块各自有一份C运行时库代码、或者根本没有C运行时库，导致了各个模块会有各自的堆。如果在A堆中申请空间，到B堆中释放就会有崩溃，在模块A申请的空间，必须在模块A中释放。以STL的string为例，通过修改编译选项验证了这个问题。string在赋值的时候需要释放掉原来的内存空间，然后再申请新的内存空间存储新的内容，如果跨模块了，释放的时候就存在“A模块申请B模块释放”的问题，导致程序崩溃。当程序中有多个模块时，必须保证所有模块使用的C运行时库是一致的。

