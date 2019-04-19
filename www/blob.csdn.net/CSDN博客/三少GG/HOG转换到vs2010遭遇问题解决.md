# HOG转换到vs2010遭遇问题解决 - 三少GG - CSDN博客
2011年03月11日 00:50:00[三少GG](https://me.csdn.net/scut1135)阅读数：1398标签：[2010																[iterator																[多线程																[编程																[generation](https://so.csdn.net/so/search/s.do?q=generation&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)
个人分类：[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)
0: modified grouprectangle:std::min --->min
太扯淡了！！！
[http://blog.sina.com.cn/s/blog_4cfcbb660100pgwv.html](http://blog.sina.com.cn/s/blog_4cfcbb660100pgwv.html)
1.error C2039: “back_inserter”: 不是“std”的成员
使用 VS2010编译时需要在头部添加iterator引用的文件清单。。
否则会出现如下2个错误
 error C2039: “back_inserter”: 不是“std”的成员
 error C3861: “back_inserter”: 找不到标识符
之前编译虽然知道这两个错误的解决方法，但不记得文件，于是傻傻的先编译再根据出错信息查找错误文件
其实，只需要ctrl F全解决方案查找back_inserter就可以了-_- 
哦，忘了，最重要的一步
**在找到的文件头部添加**
** #include <iterator> **
2.
[http://archive.cnblogs.com/a/1954180/](http://archive.cnblogs.com/a/1954180/)
《Visual C++ 2010入门教程》系列四：VC2010中初学者常见错误、警告和问题（转载）
[http://hi.baidu.com/mykingdom_/blog/item/807ea92730c5a13d8744f9b1.html](http://hi.baidu.com/mykingdom_/blog/item/807ea92730c5a13d8744f9b1.html)
解决出现fatal error LNK1169: one or more multiply
3.“error LNK1169: 找到一个或多个多重定义的符号”的解决方法    (种种解决方法) 
问题描述如下：
有 三个源文件，A.h、B.cpp、C.cpp。
A.h是头文件，其中声明了三个变量a1、a2、 a3。
B.cpp是A.h中所声明的类的实现源代码，C.cpp是主程序文件。B.cpp和C.cpp中均包含头文件 A.h。
在编译时，编译能够通过，但链接时出了问题，出现”error   LNK1169:   找到一个或多个多重定义的符号“的错误。
经过分析，确定了这是由于两个实现文件中重复包含了头文件而造成的。可解决方法却始终找不到。
要 注意的是，在这里，在头文件中加入#ifndef……#endif这样的预编译命令是没用的，因为这是防止嵌套包含头文件的，而本例中并没有嵌套包含，是 在两个文件中分别包含。
因为这三个变量在两个实现文件中都要用到，所以一定要包含在A.h中。后来在网上找到了解决方法，其实很简单。
**就是在A.h中的三个变量声明前加上extern 关键字，然后在B.cpp中不加extern关键字再次声明这三个变量。于是编译链接顺利通过。**
**其实这是C++中比较基础的问题。**还有种情况是定义了函数，但在另个文件中准备用#include打开，但是结果还是会出现。
**直接包含不就在两个cpp文件中都定义了相同的函数/变量吗**，链接时会出现重复定义（你自己试试），所以需要使用extren申明一下即可，他们使用的是同一个实体。
例如：
1、你在a.cpp中定义了一个函数
void func()
{
}
希望在b.cpp中调用，调用前就需要进行声明，格式如下：
extern void func(); //extren 后面根的形式和函数定义形式要完全相同
void main（）
{
    func();
}
2、1、**你在a.cpp中定义了一个变量  int a;**
**希望在b.cpp中使用，使用前就需要进行声明，格式如下：extern int a; //extren 后面根的形式和变量定义形式要完全相同**
**void main(){    int b = a;}**
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/xiaosu123/archive/2010/06/12/5665729.aspx](http://blog.csdn.net/xiaosu123/archive/2010/06/12/5665729.aspx)
## #pragma once和#ifndef的区别
作者：Bruce Cheng   发布：2012-05-20 23:49   分类：[Design](http://www.mllog.com/category/design)  
 阅读：7 浏览数
   [1条评论](http://www.mllog.com/pragmaonce_ifndef_difference.html#comments)
#pragma once一直都用的不错，直到今天发才发现自己并不理解。pragma once和ifndef都是可以避免同一个文件被include多次，但还是有一些细微的区别的。
事例：同事喜欢把公共的头文件通过生成后事件复制到一个公共的include文件夹中
以下内容摘自百度百科：
方式一：
#ifndef __SOMEFILE_H__
#define __SOMEFILE_H__
… … // 一些声明语句
#endif
方式二：
#pragma once
… … // 一些声明语句
#ifndef的方式依赖于宏名字不能冲突，这不光可以保证同一个文件不会被包含多次，也能保证内容完全相同的两个文件不会被不小心同时包含。当然，缺点就是如果不同头文件的宏名不小心“撞车”，可能就会导致头文件明明存在，编译器却硬说找不到声明的状况
#pragma once则由编译器提供保证：同一个文件不会被包含多次。注意这里所说的“同一个文件”是指物理上的一个文件，而不是指内容相同的两个文件。带来的好处是，你不必再费劲想个宏名了，当然也就不会出现宏名碰撞引发的奇怪问题。对应的缺点就是如果某个头文件有多份拷贝，本方法不能保证他们不被重复包含。当然，相比宏名碰撞引发的“找不到声明”的问题，重复包含更容易被发现并修正。
总结：
#ifndef 由语言支持所以移植性好；#pragma once可以避免名字冲突。
还有另外一种是两者公用，不过只要有#ifndef存在就会有宏名冲突的问题。
======================================
编程中经常能遇到LNK2005错误——重复定义错误，其实LNK2005错误并不是一个很难解决的错误。弄清楚它形成的原因，就可以轻松解决它了。
造成LNK2005错误主要有以下几种情况： 
1．重复定义全局变量。可能存在两种情况： 
A、对于一些初学编程的程序员，有时候会以为需要使用全局变量的地方就可以使用定义申明一下。其实这是错误的，全局变量是针对整个工程的。正确的应该是在一个CPP文件中定义如下：int   g_Test;那么在使用的CPP文件中就应该使用：extern   int   g_Test即可，如果还是使用int   g_Test，那么就会产生LNK2005错误，一般错误错误信息类似：AAA.obj   error   LNK2005   int   book   c？book@@3HA   already   defined
   in   BBB.obj。切记的就是不能给变量赋值否则还是会有LNK2005错误。 
              这里需要的是“声明”，不是“定义”！根据C++标准的规定，一个变量是声明，必须同时满足两个条件，否则就是定义： 
(1)声明必须使用extern关键字；(2)不能给变量赋初值 
所以，下面的是声明: 
extern   int   a; 
下面的是定义 
int   a;   int   a   =   0;   extern   int   a   =0; 
B、对于那么编程不是那么严谨的程序员，总是在需要使用变量的文件中随意定义一个全局变量，并且对于变量名也不予考虑，这也往往容易造成变量名重复，而造成LNK2005错误。
2．头文件的包含重复。往往需要包含的头文件中含有变量、函数、类的定义，在其它使用的地方又不得不多次包含之，如果头文件中没有相关的宏等防止重复链接的措施，那么就会产生LNK2005错误。解决办法是在需要包含的头文件中做类似的处理：#ifndef   MY_H_FILE       //如果没有定义这个宏
#define   MY_H_FILE       //定义这个宏 
…….       //头文件主体内容 
……. 
#endif 
上面是使用宏来做的，也可以使用预编译来做，在头文件中加入： 
#pragma   once 
//头文件主体 
3．使用第三方的库造成的。这种情况主要是C运行期函数库和MFC的库冲突造成的。具体的办法就是将那个提示出错的库放到另外一个库的前面。另外选择不同的C函数库，可能会引起这个错误。微软和C有两种C运行期函数库，一种是普通的函数库：LIBC.LIB，不支持多线程。另外一种是支持多线程的：msvcrt.lib。如果一个工程里，这两种函数库混合使用，可能会引起这个错误，一般情况下它需要MFC的库先于C运行期函数库被链接，因此建议使用支持多线程的msvcrt.lib。所以在使用第三方的库之前首先要知道它链接的是什么库，否则就可能造成LNK2005错误。如果不得不使用第三方的库，可以尝试按下面所说的方法修改，但不能保证一定能解决问题，前两种方法是微软提供的：
A、选择VC菜单Project-> Settings-> Link-> Catagory选择Input，再在Ignore   libraries   的Edit栏中填入你需要忽略的库，如：Nafxcwd.lib;Libcmtd.lib。然后在Object/library   Modules的Edit栏中填入正确的库的顺序，这里需要你能确定什么是正确的顺序，呵呵，God   bless   you！
B、选择VC菜单Project-> Settings-> Link页，然后在Project   Options的Edit栏中输入/verbose:lib，这样就可以在编译链接程序过程中在输出窗口看到链接的顺序了。
C、选择VC菜单Project-> Settings-> C/C++页，Catagory选择Code   Generation后再在User   Runtime   libraray中选择MultiThread   DLL等其他库，逐一尝试。
关于编译器的相关处理过程，参考： 
http://www.donews.net/xzwenlan/archive/2004/12/23/211668.aspx 
这就是我所遇到过的LNK2005错误的几种情况，肯定还有其他的情况也可能造成这种错误，所以我不希望你在看完这篇文章以后，再遇到LNK2005错误时候，不动脑筋的想对号入座的排除错误。编程的过程就是一个思考的过程，所以还是多多开动你的头脑，那样收获会更多！
===============================================
无意中遇到了这样的编译问题, 于是google，发现在.h文件里定义变量，发生这问题的概率是灰常大的.
下面这段红字是参考网上的说法:
当你第一个使用这个头的.cpp文件生成.obj的时候，int i在里面定义了当另外一个使用这个的.cpp再次[单独]生成.obj的时候，int
 i又被定义然后两个obj被另外一个.cpp也include这个头的，连接在一起，就会出现重复定义.
不过经过一轮研究发现.. 其实这说法不怎么正确.. 最起码对VS来说..
经过实验, 发现只要有两个cpp同时包含一个定义了变量的.h文件，就会提示错误.
再回想下刚 考完的编译原理, obj是什么?  其实就是cpp翻译后的一坨中间代码.. 一般编译过程就是把cpp先翻译成obj, 最后把obj再连接成exe. 这样，不管中间有没有第三者把前两个连接一起, 最终obj还是会汇集在一起.. 下面可以看到，错误发生在链接，而不是翻译..
正在链接...
1>3.obj : error LNK2005: "int i" (?i@@3HA) 已经在 2.obj 中定义
1>D:/编程之美/vs2008代码/dfsdfadsfec/Debug/dfsdfadsfec.exe : fatal error LNK1169: 找到一个或多个多重定义的符号
本人得出的结论:
只要两个或者以上的.cpp同时包含了一个定义了变量的.h文件，就会提示错误.
不过注意, 这中间可能会包含些比较复杂的逻辑关系.. 
例如我的那坨代码.. 
假定一个定义了变量的1.h文件
有1.cpp和2.cpp, 1.cpp包含1.h,   2.cpp只包含2.h  但是2.h包含了1.h..  所以1.h中定义的变量编译的时候会在2.pp中有定义. 于是乎2.cpp翻译成的obj里也会有定义, 结果导致提示obj里重定义.
在头文件和cpp文件错中复杂的情况下, 根本就很难判断其祸根源.
所以得出结论:
没事就别在.h中定义变量，或者初始化静态成员变量(我就是后一种情况).
不过有种比较酷的做法, 就是直接改成.c编译.  据说c里遇到重定义,  会忽略为声明.
