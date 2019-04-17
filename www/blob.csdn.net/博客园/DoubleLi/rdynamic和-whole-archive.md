# rdynamic和-whole-archive - DoubleLi - 博客园






遇到如下情况，主程序通过dlopen来打开.so文件，但是.so用到了主程序的log函数。

编译so时，通过引用主程序头文件来编译通过，头文件有log函数声明：

  extern "C" { 
   void print()
  }

在主程序的.c文件里有函数的具体实现。



但是dlopen后运行so中函数时，出现找不到相应的symbol。

这时候就需要在编译主程序ld时加上参数-rdynamic，该参数的作用是：将指示连接器把所有符号（而不仅仅只是程序已使用到的外部符号，但不包括静态符号，比如被static修饰的函数）都添加到动态符号表（即.dynsym表）里，以便那些通过dlopen()或backtrace()（这一系列函数使用.dynsym表内符号）这样的函数使用。



-rdynamic
Pass the flag ‘-export-dynamic’ to the ELF linker, on targets that support
it. This instructs the linker to add all symbols, not only used ones, to the
dynamic symbol table. This option is needed for some uses of dlopen or to
allow obtaining backtraces from within a program.



-g是编译选项，而-rdynamic是链接选项

参考：http://www.lenky.info/archives/2013/01/2190



小例子：



a.cc





**[cpp]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- #include "stdio.h"  
- #include <dlfcn.h>  
- 
- extern "C" {  
- void print()  
- {  
-     printf("I am in so file!\n");  
- }  
- 
- void fun()  
- {  
- void * err = dlopen("./libtmp.so", RTLD_LAZY);  
-     printf("dlopen = %p\n", err);  
- if (err == NULL) {  
-         printf("err=%s\n", dlerror());  
-     }     
- }  
- }  



a.h







**[cpp]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- extern "C" void print();  
- extern "C" void fun();  // 函数声明和定义都要有extern “C”，或者都没有，否则调用时出现undefined symbol fun  
- 
- #define NODE_MODULE \  
- extern "C" { \  
- static void print_main() __attribute__((constructor)) // dlopen时会自动调用该contructor函数</span>  
- static void print_main() { \  
-        print(); \  
-    } \  
- }   







so.cc





**[cpp]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- #include "a.h"  
- #include "stdio.h"  
- 
- NODE_MODULE  






foo.h





**[python]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- <span style="font-size:18px;">void foo();</span>  






foo.cc





**[cpp]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- #include "stdio.h"  
- 
- void foo()  
- {  
-     printf("foo === \n");  
- }  









main.cc



**[cpp]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- #include "a.h"  
- 
- int main(void)  
- {  
-     fun();  
- return 0;  
- }  


Makefile





**[cpp]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- all:dynamic  
- 
- libtmp.so:so.cc  
-     g++ -fPIC -shared -o $@ $^   
- 
- a.o:  
-     g++ -c a.cc -fPIC  
- 
- liba.a:a.o  
-     ar -r $@  $^   
- 
- libso.so: foo.cc liba.a  
-     g++ -fPIC -shared -o $@ $< -L./ -la -Wl,--whole-archive -la  -Wl,--no-whole-archive -ldl  
- 
- dynamic:libso.so libtmp.so  
-     g++ -o $@ main.cc -Wl,--rpath=. -L./ -lso rdynamic  
- 
- clean:  
-     rm dynamic liba.a a.o libtmp.so  








运行dynamic后输出为：





**[python]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- I am in so file!  
- dlopen = 0xdeb030  



如果没有-rdynamic，则输出为：







**[python]**[view plain](http://blog.csdn.net/fingding/article/details/46906773#)[copy](http://blog.csdn.net/fingding/article/details/46906773#)



- dlopen = (nil)  
- err=./libtmp.so: undefined symbol: print  



如果没有-Wl,--whole-archive -la  -Wl,--no-whole-archive，也会有错误：undefined symbol: print







--whole-archive 可以把 在其后面出现的静态库包含的函数和变量输出到动态库，--no-whole-archive 则关掉这个特性

使用readelf -s libso.so | grep fun来查看libso.so的符号表里是否有fun这个函数暴露出来。有--whole-archive的可以查到fun，而没有--whole-archive的，则找不到fun



先理清一下code

可执行文件dynamic依赖与libso.so，而libso.so有包含liba.a，在liba.a的函数fun调用dlopen来打开libtmp.so

主函数调用liba.a的函数来打开libtmp.so



-fvisibility=hidden

　　设置默认的ELF镜像中符号的可见性为隐藏。使用这个特性可以非常充分的提高连接和加载共享库的性能，生成更加优化的代码，提供近乎完美的API输出和防止符号碰撞。我们强烈建议你在编译任何共享库的时候使用该选项。

-fvisibility-inlines-hidden

        默认隐藏所有内联函数，从而减小导出符号表的大小，既能缩减文件的大小，还能提高运行性能，我们强烈建议你在编译任何共享库的时候使用该选项



所以编译的时候也不能有-fvisibility=hidden和-fvisibility-inlines-hidden。如果有，也会在dlopen时造成错误：undefined symbol

总结:

本实例虽小，但用到了不少编译选项

a: __attribute__((constructor))
主程序main函数之前被执行或dlopen时被执行

b: -rdynamic

ld时将动态库的的所有符号都输出到符号表，以便dlopen和backtrace也能调用

c: --whole-archive -la -Wl,--no-whole-archive

静态库的符号导入到动态库的符号表中，默认是hidden的

d: -fvisibility=hidden和-fvisibility-inlines-hidden

ELF镜像中符号的可见性为隐藏（在实验过程中不太好用，待研究）


在编译nodejs第三方模块时都会碰到这样的问题，第三方模块依赖与nodejs进行编译，而第三方模块又是通过dlopen来打开的，这就要求nodejs编译时将一下第三方模块需要的函数都暴露出来。



参考：

http://www.fx114.net/qa-225-106759.aspx

http://os.chinaunix.net/a2010/0112/1060/000001060902_3.shtml









