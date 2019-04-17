# windows下配置 GNU的gdb调试功能 - 在思索中前行！ - CSDN博客





2014年08月06日 19:37:39[_Tham](https://me.csdn.net/txl16211)阅读数：5366








1、配置


修改环境变量（前提电脑中存在gdb.exe）
1. 我的电脑->属性->环境......在path那一项后面添加你DEV-C++ Bin目录的路径（gdb.exe所在目录），如：
E:\Program Files\DEV-CPP\Bin (你自己的路径)
2. CMD命令行，输入： set path=%path%;E:\Program Files\DEV-CPP\Bin

这样之后，命令行就可以直接调用gdb命令了2、使用gdb调试程序
**GDB概述————**

GDB是GNU开源组织发布的一个强大的UNIX下的程序调试工具。或许，各位比较喜欢那种图形界面方式的，像VC、BCB等IDE的调试，但如果你是在UNIX平台下做软件，你会发现GDB这个调试工具有比VC、BCB的图形化调试器更强大的功能。所谓“寸有所长，尺有所短”就是这个道理。

一般来说，GDB主要帮忙你完成下面四个方面的功能：

    1、启动你的程序，可以按照你的自定义的要求随心所欲的运行程序。
    2、可让被调试的程序在你所指定的调置的断点处停住。（断点可以是条件表达式）
    3、当程序被停住时，可以检查此时你的程序中所发生的事。
    4、动态的改变你程序的执行环境。

从上面看来，GDB和一般的调试工具没有什么两样，基本上也是完成这些功能，不过在细节上，你会发现GDB这个调试工具的强大，大家可能比较习惯了图形化的调试工具，但有时候，命令行的调试工具却有着图形化工具所不能完成的功能。让我们一一看来。



为调试编译代码(Compiling Code for Debugging)

    为了使 gdb 正常工作, 你必须使你的程序在编译时包含调试信息. 调试信息包含你程序里的每个变量的类型和在可执行文件里的地址映射以及源代码的行号.  gdb 利用这些信息使源代码和机器码相关联.

    在编译时用 -g 选项打开调试选项.

gdb 基本命令

gdb 支持很多的命令使你能实现不同的功能. 这些命令从简单的文件装入到允许你检查所调用的堆栈内容的复杂命令, 表27.1列出了你在用 gdb 调试时会用到的一些命令. 想了解 gdb 的详细使用请参考 gdb 的指南页.

  表 27.1. 基本 gdb 命令.  

![](http://hi.csdn.net/attachment/201203/19/0_1332136060HnWO.gif)
|cd|改变工作目录|
|----|----|


|pwd|显示当前工作目录|
|----|----|


|Tbreak|设置临时断点。它的语法与break相同。区别在于用tbreak设置的断点执行一次之后立即消失|
|----|----|


|awatch|设置读写监视点。当要监视的表达式被读或写时将应用程序挂起。它的语法与watch命令相同|
|----|----|
|rwatch|设置读监视点，当监视表达式被读时将程序挂起，等侍调试。此命令的语法与watch相同|



 display：在应用程序每次停止运行时显示表达式的值。
info break：显示当前断点列表，包括每个断点到达的次数。 
info files：显示调试文件的信息。 
info func：显示所有的函数名。 
info local：显示当前函数的所有局部变量的信息。 


  info prog: 显示调试程序的执行状态。 
print: 显示表达式的值。 
delete命令: 删除断点。指定一个断点号码，则删除指定断点。
**一个调试示例——————**





1 新建一个源文件vi swap.cc

![](http://hi.csdn.net/attachment/201203/19/0_13321312233vfG.gif)


源文件内容如下：


#include<iostream>

using namespace std;

void swap(int &a,int &b)

{

        int tmp;

        tmp=a;

        a=b;

        b=tmp;

}


int main()

{

        int i,j;

        cout<<endl<<"Input two int number:"<<endl;

        cin>>i>>j;

        cout<<"Before swap(),i="<<i<<" j="<<j<<endl;

        swap(i,j);

        cout<<"After swap(),i="<<i<<" j="<<j<<endl<<endl;

        return 0;

}


直接复制粘贴生成源文件


 2.生成可执行文件 g++ -g -o swap swap.cc，注意必须使用-g参数，编译会加入调试信息，否则无法调试执行文件

![](http://hi.csdn.net/attachment/201203/19/0_1332131674N111.gif)


3.启动调试 gdb swap

![](http://hi.csdn.net/attachment/201203/19/0_1332131779zH88.gif)


       3.1  查看源文件 list 1，回车重复上一次指令

![](http://hi.csdn.net/attachment/201203/19/0_1332132852AmV1.gif)


             3.2设置调试断点 break 16，在第16行设置断点，info break查看断点信息（亦可使用缩写i
 b ）

![](http://hi.csdn.net/attachment/201203/19/0_1332133229I2aK.gif)

![](http://hi.csdn.net/attachment/201203/19/0_1332133538hH00.gif)


         3.3 调试  运行 输入run 或者r 

![](http://hi.csdn.net/attachment/201203/19/0_13321337928E0I.gif)


         3.3 单步调试，step 或者 s进入函数内部

![](http://hi.csdn.net/attachment/201203/19/0_1332133876DrOK.gif)


                3.4查看变量 print b 或者 p b

![](http://hi.csdn.net/attachment/201203/19/0_13321339794UxL.gif)


                3.5查看函数堆栈bt，退出函数finish

![](http://hi.csdn.net/attachment/201203/19/0_1332134294MXJX.gif)


                   3.6  继续运行直到下一个断点或主函数结束continue或者c

![](http://hi.csdn.net/attachment/201203/19/0_13321344199VGT.gif)


                  3.7 退出调试 输入q


![](http://hi.csdn.net/attachment/201203/19/0_1332134872aIf8.gif)



