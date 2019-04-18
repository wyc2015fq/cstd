# PCL第三方库：Eigen, Flann , Qhull, VTK,  Boost简介 - wishchinYang的专栏 - CSDN博客
2013年09月02日 11:09:11[wishchin](https://me.csdn.net/wishchin)阅读数：3387
前言：
       PCL作为机器人软件的一个基础类库，融合了基础结构、算法和三维显示。其大量使用第三方库，使用了Boost、Eigen、Flann、VTK、Boost、CUdnn等。
**第一预备役：Boost**
     Boost库是为C++语言标准库提供扩展的一些C++[程序库](http://baike.baidu.com/view/421066.htm)的总称。
[Boost库](http://baike.baidu.com/view/5545936.htm)由Boost社区组织开发、维护。其目的是为C++程序员提供免费、同行审查的、可移植的[程序库](http://baike.baidu.com/view/421066.htm)。[Boost库](http://baike.baidu.com/view/5545936.htm)可以与C++标准库完美共同工作，并且为其提供扩展功能。[Boost库](http://baike.baidu.com/view/5545936.htm)使用Boost
 License来授权使用，根据该协议，商业的非商业的使用都是允许并鼓励的。
Boost社区建立的初衷之一就是为C++的标准化工作提供可供参考的实现，Boost社区的发起人Dawes本人就是C++标准委员会的成员之一。在[Boost库](http://baike.baidu.com/view/5545936.htm)的开发中，Boost社区也在这个方向上取得了丰硕的成果。在送审的C++标准库TR1中，有十个[Boost库](http://baike.baidu.com/view/5545936.htm)成为标准库的候选方案。在更新的TR2中，有更多的[Boost库](http://baike.baidu.com/view/5545936.htm)被加入到其中。从某种意义上来讲，[Boost库](http://baike.baidu.com/view/5545936.htm)成为具有实践意义的准标准库。
可下载Boost C++ Libraries[1]安装boost库。大部分boost库功能的使用只需包括相应头文件即可，少数（如正则表达式库，文件系统库等）需要链接库。里面有许多具有工业强度的库，如graph库。
很多Boost中的库功能堪称对语言功能的扩展，其构造用尽精巧的手法，不要贸然的花费时间研读。Boost另外一面，[比如](http://baike.baidu.com/view/6814120.htm)Graph这样的库则是具有工业强度，结构良好，非常值得研读的精品代码，并且也可以放心的在产品代码中多多利用。
      官方网站：[http://www.boost.org/](http://www.boost.org/)
      截止今日，官方最新版本为1.54，VS可以通过下载源码编译运行，例程如：
### [vs2012编译boost_1_53_0：](http://blog.csdn.net/liukang0618/article/details/9149881)
[http://blog.csdn.net/liukang0618/article/details/9149881](http://blog.csdn.net/liukang0618/article/details/9149881)
    补充：我安装时正好是VS2012出现问题，tools工具损坏，没心情修补，直接下载的exe包：
    地址：[http://sourceforge.net/projects/boost/files/boost-binaries/1.54.0/](http://sourceforge.net/projects/boost/files/boost-binaries/1.54.0/)
     自己进行挑选合适的版本进行安装即可,省去了编译步骤。
百科上的评论，有点意思：
        Boost 包含近百个[程序库](http://baike.baidu.com/view/421066.htm)，其中不乏具有工程实用价值的佳品。每个人口味与技术背景不一样，对 Boost 的取舍也不一样。就我的个人经验而言，首先可以使用绝对无害的库，例如 noncopyable、scoped_ptr、static_assert 等，这些库的学习和使用都比较简单，容易入手。其次，有些功能自己实现起来并不困难，正好
 Boost 里提供了现成的代码，那就不妨一用，[比如](http://baike.baidu.com/view/6814120.htm)date_time 和 circular_buffer 等。然后，在新项目中，对于消息传递和资源管理可以考虑采用更加现代的方式，例如用 function/bind 在某些情况下代替[虚函数](http://baike.baidu.com/view/161302.htm)作为库的回调接口、借助[shared_ptr](http://baike.baidu.com/view/2276295.htm)实现[线程安全](http://baike.baidu.com/view/1298606.htm)的对象回调等等。这二者会影响整个程序的设计思路与风格，需要通盘考虑，如果正确使用[智能指针](http://baike.baidu.com/view/1391603.htm)，在现代
 C++ 程序里一般不需要出现 delete 语句。最后，对某些性能不佳的库保持警惕，[比如](http://baike.baidu.com/view/6814120.htm) lexical_cast。总之，在项目组成员人人都能理解并运用的基础上，适当引入现成的 Boost 组件，以减少重复劳动，提高生产力。
        Boost 是一个宝库，其中既有可以直接拿来用的代码，也有值得借鉴的设计思路。试举一例：[正则表达式](http://baike.baidu.com/view/94238.htm)库 regex 对[线程安全](http://baike.baidu.com/view/1298606.htm)的处理。
早期的 [RegEx](http://baike.baidu.com/view/2967644.htm) 类不是[线程安全](http://baike.baidu.com/view/1298606.htm)的，它把“[正则表达式](http://baike.baidu.com/view/94238.htm)”和“匹配动作”放到了一个类里边。由于有可变数据，[RegEx](http://baike.baidu.com/view/2967644.htm)
 的对象不能跨[线程](http://baike.baidu.com/view/1053.htm)使用。如今的 regex 明确地区分了不可变（immutable）与可变（mutable）的数据，前者可以安全地跨线程共享，后者则不行。[比如](http://baike.baidu.com/view/6814120.htm)[正则表达式](http://baike.baidu.com/view/94238.htm)本身（basic_regex）与一次匹配的结果（match_results）是不可变的；而匹配动作本身（match_regex）涉及状态更新，是可变的，于是用可重入的函数将其封装起来，不让这些数据泄露给别的线程。正是由于做了这样合理的区分，regex
 在正常使用时就不必加锁。
     Donald Knuth 在“Coders at Work”一书里表达了这样一个观点：如果程序员的工作就是摆弄参数去调用现成的库，而不知道这些库是如何实现的，那么这份职业就没啥乐趣可言。换句话说，固然我们强调工作中不要重新发明轮子，但是作为一个合格的程序员，应该具备自制轮子的能力。非不能也，是不为也。
C/C++ 语言的一大特点是其标准库可以用语言自身实现。C 标准库的 strlen、strcpy、strcmp 系列函数是教学与练习的好题材，C++ 标准库的 complex、string、vector 则是类、资源管理、模板编程的绝佳示范。在深入了解 STL 的实现之后，运用 STL 自然手到擒来，并能自动避免一些错误和低效的用法。
对于 Boost 也是如此，为了消除使用时的疑虑，为了用得更顺手，有时我们需要适当了解其内部实现，甚至编写简化版用作对比验证。但是由于 Boost 代码用到了日常[应用程序开发](http://baike.baidu.com/view/1753085.htm)中不常见的高级语法和技巧，并且为了跨多个平台和[编译器](http://baike.baidu.com/view/487018.htm)而大量使用了预处理宏，阅读
 Boost 源码并不轻松惬意，需要下一番功夫。另一方面，如果沉迷于这些有趣的底层细节而忘了原本要解决什么问题，恐怕就舍本逐末了。
       Boost 中的很多库是按[泛型编程](http://baike.baidu.com/view/1105026.htm)的范式来设计的，对于熟悉[面向对象编程](http://baike.baidu.com/view/10110.htm)的人而言，或许面临一个思路的转变。[比如](http://baike.baidu.com/view/6814120.htm)，你得熟悉[泛型编程](http://baike.baidu.com/view/1105026.htm)的那套术语，如concept、  
 model、refinement，才容易读懂 Boost.Threads 的文档中关于各种锁的描述。我想，对于熟悉 STL 设计理念的人而言，这不是什么大问题。
       在某些领域，Boost 不是唯一的选择，也不一定是最好的选择。[比如](http://baike.baidu.com/view/6814120.htm)，要生成公式化的[源代码](http://baike.baidu.com/view/60376.htm)，我会首选用[脚本语言](http://baike.baidu.com/view/76320.htm)写一小段[代码生成](http://baike.baidu.com/view/8477718.htm)程序，而不用
 Boost.Preprocessor；要在 C++ 程序中嵌入[领域特定语言](http://baike.baidu.com/view/1616623.htm)，我会首选用 Lua 或其他语言[解释器](http://baike.baidu.com/view/592974.htm)，而不用 Boost.Proto；要用 C++ 程序解析[上下文无关文法](http://baike.baidu.com/view/110877.htm)，我会首选用
 ANTLR 来定义词法与语法规则并生成解析器（parser），而不用 Boost.Spirit。总之，使用 Boost 时心态要平和，别较劲去改造 C++ 语言。把它有助于提高生产力的那部分功能充分发挥出来，让项目从中受益才是关键
### [*2.*C++矩阵处理工具——Eigen](http://blog.csdn.net/abcjennifer/article/details/7781936)
不可否认，此女子的逻辑很清晰...:文章链接：[http://blog.csdn.net/abcjennifer/article/details/7781936](http://blog.csdn.net/abcjennifer/article/details/7781936)
若有评论，希望到原文....
首先推荐几个可以在C++中调用的数学平台：eigen、bias、lapack、svd、[CMatrix](http://www.openpr.org.cn/index.php/All/46-CMatrix-class/View-details.html)，本文着重eigen做以讲解，希望对各位有所帮助。
下面是本文主线，主要围绕下面几点进行讲解：
**********************************************************************************************
Eigen是什么？
Eigen3哪里下载？
Eigen3的配置
Eigen3 样例代码有没有？
去哪里更深入学习？
**********************************************************************************************
Eigen是什么？
Eigen是C++中可以用来调用并进行矩阵计算的一个库，里面封装了一些[类](http://eigen.tuxfamily.org/dox-2.0/annotated.html)，需要的头文件和功能如下：
![](https://img-my.csdn.net/uploads/201207/24/1343132342_6136.jpg)
[Eigen的主页](http://eigen.tuxfamily.org/dox/index.html)上有一些更详细的Eigen介绍。
Eigen3哪里下载？
[这里是](http://download.csdn.net/detail/abcjennifer/4449957)我下好的，[这里](http://eigen.tuxfamily.org/index.php?title=Main_Page#Download)是官网主页，请自行下载，是个code包，不用安装。
Eigen的配置
![](https://img-my.csdn.net/uploads/201207/24/1343133124_3315.jpg)
直接上图了，附加包含目录那里填上你放Eigen文件夹的位置即可。
Eigen的样例代码有没有？
当然有，这篇文章重点就是这里！
以下是我整理的一些常用操作，基本的矩阵运算就在下面了，算是个入门吧~主要分以下几部分：
![](https://img-my.csdn.net/uploads/201207/24/1343133892_6291.jpg)
建议大家放到编译环境里去看，因为我这里有一些region的东西，编译器下更方便看~
```cpp
#include <iostream>  
#include <Eigen/Dense>  
  
//using Eigen::MatrixXd;  
using namespace Eigen;  
using namespace Eigen::internal;  
using namespace Eigen::Architecture;  
  
using namespace std;  
  
  
int main()  
{  
  
#pragma region one_d_object  
  
    cout<<"*******************1D-object****************"<<endl;  
  
    Vector4d v1;  
    v1<< 1,2,3,4;  
    cout<<"v1=\n"<<v1<<endl;  
  
    VectorXd v2(3);  
    v2<<1,2,3;  
    cout<<"v2=\n"<<v2<<endl;  
  
    Array4i v3;  
    v3<<1,2,3,4;  
    cout<<"v3=\n"<<v3<<endl;  
  
    ArrayXf v4(3);  
    v4<<1,2,3;  
    cout<<"v4=\n"<<v4<<endl;  
  
#pragma endregion  
  
#pragma region two_d_object  
      
    cout<<"*******************2D-object****************"<<endl;  
    //2D objects:  
    MatrixXd m(2,2);  
  
    //method 1  
    m(0,0) = 3;  
    m(1,0) = 2.5;  
    m(0,1) = -1;  
    m(1,1) = m(1,0) + m(0,1);  
  
    //method 2  
    m<<3,-1,  
        2.5,-1.5;  
    cout <<"m=\n"<< m << endl;  
  
#pragma endregion  
  
#pragma region Comma_initializer  
  
    cout<<"*******************Initialization****************"<<endl;  
  
    int rows=5;  
    int cols=5;  
    MatrixXf m1(rows,cols);  
    m1<<( Matrix3f()<<1,2,3,4,5,6,7,8,9 ).finished(),  
        MatrixXf::Zero(3,cols-3),  
        MatrixXf::Zero(rows-3,3),  
        MatrixXf::Identity(rows-3,cols-3);  
    cout<<"m1=\n"<<m1<<endl;  
  
#pragma endregion  
  
#pragma region Runtime_info  
      
    cout<<"*******************Runtime Info****************"<<endl;  
  
    MatrixXf m2(5,4);  
    m2<<MatrixXf::Identity(5,4);  
    cout<<"m2=\n"<<m2<<endl;  
  
    MatrixXf m3;  
    m3=m1*m2;  
    cout<<"m3.rows()="<<m3.rows()<<"  ;  "  
             <<"m3.cols()="<< m3.cols()<<endl;  
      
    cout<<"m3=\n"<<m3<<endl;  
  
#pragma endregion  
      
#pragma region Resizing  
      
    cout<<"*******************Resizing****************"<<endl;  
  
    //1D-resize   
    v1.resize(4);  
    cout<<"Recover v1 to 4*1 array : v1=\n"<<v1<<endl;  
  
    //2D-resize  
    m.resize(2,3);  
    m.resize(Eigen::NoChange, 3);  
    m.resizeLike(m2);  
    m.resize(2,2);  
      
#pragma endregion  
  
#pragma region Coeff_access  
      
    cout<<"*******************Coefficient access****************"<<endl;  
  
    float tx=v1(1);  
    tx=m1(1,1);  
    cout<<endl;  
  
#pragma endregion  
  
#pragma  region Predefined_matrix  
  
    cout<<"*******************Predefined Matrix****************"<<endl;  
  
    //1D-object  
    typedef  Matrix3f   FixedXD;  
    FixedXD x;  
      
    x=FixedXD::Zero();  
    x=FixedXD::Ones();  
    x=FixedXD::Constant(tx);//tx is the value  
    x=FixedXD::Random();  
    cout<<"x=\n"<<x<<endl;  
  
    typedef ArrayXf Dynamic1D;  
    //或者 typedef VectorXf Dynamic1D  
    int size=3;  
    Dynamic1D xx;  
    xx=Dynamic1D::Zero(size);  
    xx=Dynamic1D::Ones(size);  
    xx=Dynamic1D::Constant(size,tx);  
    xx=Dynamic1D::Random(size);  
    cout<<"xx=\n"<<x<<endl;  
  
    //2D-object  
    typedef MatrixXf Dynamic2D;  
    Dynamic2D y;  
    y=Dynamic2D::Zero(rows,cols);  
    y=Dynamic2D::Ones(rows,cols);  
    y=Dynamic2D::Constant(rows,cols,tx);//tx is the value  
    y=Dynamic2D::Random(rows,cols);  
  
#pragma endregion  
  
#pragma region Arithmetic_Operators  
  
    cout<<"******************* Arithmetic_Operators****************"<<endl;  
  
    //add & sub  
    MatrixXf m4(5,4);  
    MatrixXf m5;  
    m4=m2+m3;  
    m3-=m2;  
  
    //product  
    m3=m1*m2;  
   
    //transposition  
    m5=m4.transpose();  
    //m5=m.adjoint();//伴随矩阵   
      
    //dot product  
    double xtt;  
    cout<<"v1=\n"<<v1<<endl;  
    v2.resize(4);  
    v2<<VectorXd::Ones(4);  
    cout<<"v2=\n"<<v2<<endl;  
  
    cout<<"*************dot product*************"<<endl;  
    xtt=v1.dot(v2);  
    cout<<"v1.*v2="<<xtt<<endl;  
  
    //vector norm  
    cout<<"*************matrix norm*************"<<endl;  
    xtt=v1.norm();  
    cout<<"norm of v1="<<xtt<<endl;  
    xtt=v1.squaredNorm();  
    cout<<"SquareNorm of v1="<<xtt<<endl;  
  
#pragma endregion  
  
cout<<endl;  
}
```
去哪里更深入学习？
      Please refer to [Eigen中的类及函数](http://eigen.tuxfamily.org/dox-2.0/classEigen_1_1MatrixBase.html)、[Eigen的官方教程](http://eigen.tuxfamily.org/dox-2.0/TutorialCore.html)，和一些[教程上的相关内容](http://eigen.tuxfamily.org/dox-2.0/functions.html)。
**3.FLANN - Fast Library for Approximate Nearest Neighbors**
      目前最完整的（近似）最近邻开源库。不但实现了一系列查找算法，还包含了一种自动选取最快算法的机制。
FLANN is written in C++ and contains bindings for the following languages: C, MATLAB and Python.算是比较完整的支持吧！
[http://www.cs.ubc.ca/research/flann/](http://www.cs.ubc.ca/research/flann/)
官网的介绍非常详细，几乎是面面俱到，希望能仔细阅读....
其中有一条新闻：
- You can find binary installers for FLANN on the [Point Cloud Library](http://www.pointclouds.org/downloads/) project page. Thanks to the PCL developers!
不过还是选用最新的包自己编译的好.
此外，视觉博客的相关介绍：[http://www.cvchina.info/tag/flann/](http://www.cvchina.info/tag/flann/)
**4.VTK 视觉化工具函数库**
**转自维基：[http://zh.wikipedia.org/wiki/VTK](http://zh.wikipedia.org/wiki/VTK)**
**视觉化工具函式库**（VTK， Visualization Toolkit）是一个[开放源码](http://zh.wikipedia.org/wiki/%E9%96%8B%E6%94%BE%E6%BA%90%E7%A2%BC)，[跨平台](http://zh.wikipedia.org/wiki/%E8%B7%A8%E5%B9%B3%E5%8F%B0)、支援[平行处理](http://zh.wikipedia.org/wiki/%E5%B9%B3%E8%A1%8C%E8%99%95%E7%90%86)（VTK曾用于处理大小近乎1个[Petabyte](http://zh.wikipedia.org/wiki/Petabyte)的资料，其平台为[美国](http://zh.wikipedia.org/wiki/%E7%BE%8E%E5%9C%8B)[Los
 Alamos国家实验室](http://zh.wikipedia.org/wiki/Los_Alamos%E5%9C%8B%E5%AE%B6%E5%AF%A6%E9%A9%97%E5%AE%A4)所有的具1024个[处理器](http://zh.wikipedia.org/wiki/%E8%99%95%E7%90%86%E5%99%A8)之大型系统）的图形应用函式库。2005年实曾被美国陆军研究实验室用于即时模拟[俄罗斯](http://zh.wikipedia.org/wiki/%E4%BF%84%E7%BE%85%E6%96%AF)制反导弹战车ZSU23-4受到平面波攻击的情形，其计算节点高达2.5兆个之多。
该函式库以开放源码的[BSD授权](http://zh.wikipedia.org/wiki/BSD%E8%AE%B8%E5%8F%AF%E8%AF%81)释出。
**官方主页：[http://www.vtk.org/](http://www.vtk.org/)**
