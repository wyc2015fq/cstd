# C++精进篇（二）之―指针和引用 - fanyun的博客 - CSDN博客
2016年03月25日 22:23:37[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：802标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
（1）指针，是object的地址；
（2）引用，是object的别名。
      备注：（不存在引用的指针，因为引用不是实际的object；存在指针的引用，因为指针是实际的object。）
      那指针的引用怎么定义？
int i = 42;
int *p;        // p是整型的指针
int *&r = p;    // r是引用，它引用的类型是int*
r = &i;        // r就是p，因此p被赋值为i的地址
*r = 0;        // r就是p，p指向i，i被赋值为0
     理解r的定义时，从右往左依次理解&和*。
    指针-对于一个类型T，T*就是指向T的指针类型，也即一个T*类型的变量能够保存一个T对象的地址，而类型T是可以加一些限定词的，如const、volatile等等。见下图，所示指针的含义：
![](https://img-blog.csdn.net/20160325104519827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     引用-引用是一个对象的别名，主要用于函数参数和返回值类型，符号X&表示X类型的引用。见下图，所示引用的含义：
![](https://img-blog.csdn.net/20160325104554577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     引用不可以为空，但指针可以为空。前面也说过了引用是对象的别名，引用为空——对象都不存在，怎么可能有别名！故定义一个引用的时候，必须初始化。因此如果你有一个变量是用于指向另一个对象，但是它可能为空，这时你应该使用指针；如果变量总是指向一个对象，你的设计不允许变量为空，这时你应该使用引用。不初始化，连编译都通不过！如图1.1所示：
![](https://img-blog.csdn.net/20160325104646923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                                                  如图1.1
声明指针是可以不指向任何对象，也正是因为这个原因，使用指针之前必须做判空操作，而引用就不必。
引用不可以改变指向，对一个对象"至死不渝"；但是指针可以改变指向，而指向其它对象。说明：虽然引用不可以改变指向，但是可以改变初始化对象的内容。例如就++操作而言，对引用的操作直接反应到所指向的对象，而不是改变指向；而对指针的操作，会使指针指向下一个对象，而不是改变所指对象的内容。如图1-2所示：
![](https://img-blog.csdn.net/20160325104753205?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160325104840300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                                                           如图1-2
     可以通过断点调试，自行验证。
     引用的大小是所指向的变量的大小，因为引用只是一个别名而已；指针是指针本身的大小，4个字节。见下图1-3所示：
![](https://img-blog.csdn.net/20160325104939536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                                                             图1-3
     由上可见，引用比指针使用起来形式上更漂亮，使用引用指向的内容时可以之间用引用变量名，而不像指针一样要使用*；定义引用的时候也不用像指针一样使用&取址。
     引用比指针更安全，在C++标准开发建议：尽可能多的使用引用，少用指针。指针指向一块内存，它的内容是所指内存的地址；而引用则是某块内存的别名，引用不改变指向；由于不存在空引用，并且引用一旦被初始化为指向一个对象，它就不能被改变为另一个对象的引用，因此引用很安全。对于指针来说，它可以随时指向别的对象，并且可以不被初始化，或为NULL，所以不安全。const 指针虽然不能改变指向，但仍然存在空指针，并且有可能产生野指针（即多个指针指向一块内存，free掉一个指针之后，别的指针就成了野指针）。
  （1）常量指针与常量引用，Const关键字对引用和指针的影响：
     常量指针：指向常量的指针，在指针定义语句的类型前加const，表示指向的对象是常量。定义指向常量的指针只限制指针的间接访问操作，而不能规定指针指向的值本身的操作规定性。
![](https://img-blog.csdn.net/20160325105040066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     常量指针定义"constint* pointer=&a"告诉编译器，*pointer是常量，不能将*pointer作为左值进行操作。
![](https://img-blog.csdn.net/20160325105128741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     常量引用：指向常量的引用，在引用定义语句的类型前加const，表示指向的对象是常量。也跟指针一样不能利用引用对指向的变量进行重新赋值操作。
（2）指针常量VS引用常量：
    在指针定义语句的指针名前加const，表示指针本身是常量。在定义指针常量时必须初始化！而这是引用天生具来的属性，不用再引用指针定义语句的引用名前加const。
    指针常量定义"int* const pointer=&b"告诉编译器，pointer是常量，不能作为左值进行操作，但是允许修改间接访问值，即*pointer可以修改。
![](https://img-blog.csdn.net/20160325105248587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   （3）常量指针常量VS常量引用常量
     常量指针常量：指向常量的指针常量，可以定义一个指向常量的指针常量，它必须在定义时初始化。常量指针常量定义"const int* const pointer=&c"告诉编译器，pointer和*pointer都是常量，他们都不能作为左值进行操作。
     而就不存在所谓的"常量引用常量"，因为跟上面讲的一样引用变量就是引用常量。C++不区分变量的const引用和const变量的引用。程序决不能给引用本身重新赋值，使他指向另一个变量，因此引用总是const的。如果对引用应用关键字const，起作用就是使其目标称为const变量。即没有：Const double const& a=1；只有const double& a=1；
    总结：有一个规则可以很好的区分const是修饰指针，还是修饰指针指向的数据——画一条垂直穿过指针声明的星号（*），如果const出现在线的左边，指针指向的数据为常量；如果const出现在右边，指针本身为常量。而引用本身与天俱来就是常量，即不可以改变指向。
   （4）   指针传递和引用传递：
     指针传递参数本质上是值传递的方式，它所传递的是一个地址值。值传递过程中，被调函数的形式参数作为被调函数的局部变量处理，即在栈中开辟了内存空间以存放由主调函数放进来的实参的值，从而成为了实参的一个副本。值传递的特点是被调函数对形式参数的任何操作都是作为局部变量进行，不会影响主调函数的实参变量的值。引用传递过程中，被调函数的形式参数也作为局部变量在栈中开辟了内存空间，但是这时存放的是由主调函数放进来的实参变量的地址。被调函数对形参的任何操作都被处理成间接寻址，即通过栈中存放的地址访问主调函数中的实参变量。正因为如此，被调函数对形参做的任何操作都影响了主调函数中的实参变量。引用传递和指针传递是不同的，虽然它们都是在被调函数栈空间上的一个局部变量，但是任何对于引用参数的处理都会通过一个间接寻址的方式操作到主调函数中的相关变量。而对于指针传递的参数，如果改变被调函数中的指针地址，它将影响不到主调函数的相关变量。
    如果想通过指针参数传递来改变主调函数中的相关变量，那就得使用指向指针的指针，或者指针引用。如图1-4所示：
![](https://img-blog.csdn.net/20160325105327322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   （5）    我们利用下面一段简单的代码来深入分析指针和引用:
![](https://img-blog.csdn.net/20160325105434208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     敲完以后，G++ test1.cpp,对生成的a.out,反汇编：objdump -d a.out，得到main函数的一段汇编代码如下：
![](https://img-blog.csdn.net/20160325105554818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160325105631943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160325105704584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
          从汇编代码可以看出实际上指针和引用在编译器中的实现是一样的：有上面分析可知：虽然指针和引用最终在编译中的实现是一样的，但是引用的形式大大方便了使用也更安全。而且可以知道，引用也是占用内存的。
