# C++异常处理机制详解 - fanyun的博客 - CSDN博客
2017年07月30日 21:34:33[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：292标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**C++异常处理机制详解**
          C++的异常情况无非两种，一种是语法错误，即程序中出现了错误的语句、函数结构和类，致使编译程序无法进行。另一种是运行时发生的错误，一般与[算法](http://lib.csdn.net/base/datastructure)有关。
          关于语法错误， C++编译器的报错机制可以让我们轻松地解决这些错误;   第二种是运行时的错误，常见的有文件打开失败、数组下标溢出、系统内存不足等等。而一旦出现这些问题，引发算法失效、程序运行时无故停止等故障也是常有的。这就要求我们在设计软件算法时要全面。比如针对文件打开失败的情况，保护的方法有很多种，最简单的就是使用“return”命令，告诉上层调用者函数执行失败；另外一种处理策略就是利用c++的异常机制，抛出异常。这就是我们今天要说的C++异常处理机制：
            C++异常处理机制是一个用来有效地处理运行错误的非常强大且灵活的工具，它提供了更多的弹性、安全性和稳固性，克服了传统方法所带来的问题. 异常的抛出和处理主要使用了以下三个关键字： try、 throw 、 catch 。抛出异常即检测是否产生异常，在C++中，其采用throw语句来实现，如果检测到产生异常，则抛出异常。该语句的格式为：                            
```cpp
throw   表达式;
```
如果在try语句块的程序段中（包括在其中调用的函数）发现了异常，且抛弃了该异常，则这个异常就可以被try语句块后的某个catch语句所捕获并处理，捕获和处理的条件是被抛弃的异常的类型与catch语句的异常类型相匹配。由于C++使用数据类型来区分不同的异常，因此在判断异常时，throw语句中的表达式的值就没有实际意义，而表达式的类型就特别重要。 try-catch语句形式如下：
```cpp
try
{ 
            包含可能抛出异常的语句； 
} 
catch(类型名 [形参名]) // 捕获特定类型的异常 
{ 
            //处理1
} 
catch(类型名 [形参名]) // 捕获特定类型的异常 
{ 
           //处理2
} 
catch(...)    // 三个点则表示捕获所有类型的异常
{ 
}
```
示例1：处理除数为0的异常。该范例将上述除数为0的异常可以用try/catch语句来捕获异常，并使用throw语句来抛出异常，从而实现异常处理，实现代码如代码如下:
```cpp
#include<iostream.h>     //包含头文件 
#include<stdlib.h> 
    double    fuc(double x, double y)             //定义函数 
{ 
           if(y==0) 
{ 
                    throw     y;               //除数为0，抛出异常 
       } 
       return x/y;     //否则返回两个数的商 
} 
void main() 
{ 
       double    res; 
       try     //定义异常 
    { 
                res=fuc(2,3); 
                cout<<"The result of x/y is : "<<res<<endl; 
                res=fuc(4,0); 出现异常，函数内部会抛出异常 
      } 
      catch(double)             //捕获并处理异常 
     { 
                cerr<<"error of dividing zero.\n"; 
                exit(1);                //异常退出程序 
      } 
}
```
示例2：自定义异常类型
```cpp
#include "stdafx.h" 
#include<stdlib.h> 
#include<crtdbg.h> 
#include <iostream> 
// 内存泄露检测机制 
#define _CRTDBG_MAP_ALLOC  
#ifdef _DEBUG 
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#endif 
// 自定义异常类 
class    MyExcepction 
{ 
public: 
        // 构造函数,参数为错误代码 
        MyExcepction(int errorId) 
        { 
            // 输出构造函数被调用信息 
            std::cout << "MyExcepction is called" << std::endl; 
            m_errorId = errorId; 
        } 
        // 拷贝构造函数 
        MyExcepction( MyExcepction& myExp) 
        { 
            // 输出拷贝构造函数被调用信息 
std::cout << "copy construct is called" << std::endl; 
            this->m_errorId = myExp.m_errorId; 
        } 
       ~MyExcepction() 
        { 
            // 输出析构函数被调用信息 
            std::cout << "~MyExcepction is called" << std::endl; 
        } 
       // 获取错误码 
        int getErrorId() 
        { 
            return m_errorId; 
        } 
private:     
        // 错误码 
        int m_errorId; 
}; 
int    main(int argc, char* argv[]) 
{ 
        // 内存泄露检测机制 
        _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
        // 可以改变错误码,以便抛出不同的异常进行测试 
int throwErrorCode = 110; 
       std::cout << " input test code :" << std::endl; 
       std::cin >> throwErrorCode; 
       try
       { 
            if ( throwErrorCode == 110) 
            { 
                MyExcepction   myStru(110); 
                // 抛出对象的地址 -> 由catch( MyExcepction*    pMyExcepction) 捕获 
                // 这里该对象的地址抛出给catch语句,不会调用对象的拷贝构造函数 
                // 传地址是提倡的做法,不会频繁地调用该对象的构造函数或拷贝构造函数 
                // catch语句执行结束后,myStru会被析构掉 
                throw       &myStru;     
            } 
            else if ( throwErrorCode == 119 ) 
            { 
                MyExcepction myStru(119); 
                // 抛出对象,这里会通过拷贝构造函数创建一个临时的对象传出给catch 
                // 由catch( MyExcepction    myExcepction) 捕获 
                // 在catch语句中会再次调用通过拷贝构造函数创建临时对象复制这里传过去的对象 
                // throw结束后myStru会被析构掉 
                 throw    myStru;     
             } 
             else if ( throwErrorCode == 120 ) 
{ 
                  // 不提倡这样的抛出方法 
                  // 这样做的话,如果catch( MyExcepction*    pMyExcepction)中不执行delete操作则
//会发生内存泄露由catch( MyExcepction*    pMyExcepction) 捕获 
                  MyExcepction * pMyStru = new MyExcepction(120);  
                  throw pMyStru;     
             } 
             else
             { 
                  // 直接创建新对象抛出 
                  // 相当于创建了临时的对象传递给了catch语句 
                  // 由catch接收时通过拷贝构造函数再次创建临时对象接收传递过去的对象 
                  // throw结束后两次创建的临时对象会被析构掉 
                   Throw    MyExcepction(throwErrorCode);     
             }     
        } 
        catch( MyExcepction*    pMyExcepction) 
        { 
               // 输出本语句被执行信息 
               std::cout << "执行了 catch( MyExcepction*    pMyExcepction) " << std::endl; 
               // 输出错误信息 
               std::cout << "error Code : " << pMyExcepction->getErrorId()<< std::endl; 
               // 异常抛出的新对象并非创建在函数栈上，而是创建在专用的异常栈上,不需要进行delete 
               //delete pMyExcepction; 
        } 
catch ( MyExcepction myExcepction) 
        { 
            // 输出本语句被执行信息 
            std::cout << "执行了 catch ( MyExcepction myExcepction) " << std::endl; 
            // 输出错误信息 
            std::cout << "error Code : " << myExcepction.getErrorId()<< std::endl; 
        } 
        catch(...) 
        { 
              // 输出本语句被执行信息 
              std::cout << "执行了 catch(...) " << std::endl; 
              // 处理不了,重新抛出给上级 
              throw ; 
        } 
        // 暂停 
        int temp; 
        std::cin >> temp; 
        return 0; 
}
```
异常的接口声明
为了加强程序的可读性，使函数的用户能够方便地知道所使用的函数会抛出哪些异常，可以在函数的声明中列出这个函数可能抛出的所有异常类型，例如：
void   fun()    throw( A,B,C,D);
这表明函数fun()可能并且只可能抛出类型(A,B,C,D)及其子类型的异常。如果在函数的声明中没有包括异常的接口声明，则此函数可以抛出任何类型的异常，例如：
void     fun();
一个不会抛出任何类型异常的函数可以进行如下形式的声明：
void fun() thow();     
异常处理中需要注意的问题
(1). 如果抛出的异常一直没有函数捕获(catch)，则会一直上传到c++运行系统那里，导致 整个程序的终止。(2). 一般在异常抛出后资源可以正常被释放，但注意如果在类的构造函数中抛出异常，系统是不会调用它的析构函数的，处理方法是：如果在构造函数中要抛出异常，则在抛出前要记得删除申请的资源。(3). 异常处理仅仅通过类型而不是通过值来匹配的，所以catch块的参数可以没有参数名称，只需要参数类型。(4). 函数原型中的异常说明要与实现中的异常说明一致，否则容易引起异常冲突。(5). 应该在throw语句后写上异常对象时，throw先通过Copy构造函数构造一个新对象，再把该新对象传递给
 catch. 那么当异常抛出后新对象如何释放？异常处理机制保证：异常抛出的新对象并非创建在函数栈上，而是创建在专用的异 常栈上，因此它才可以跨接多个函数而传递到上层，否则在栈清空的过程中就会被销毁。所有从try到throw语句之间构造起来的对象的析构函数将被自动调用。但如果一直上溯到main函数后还没有找到匹配的catch块，那么系统调用terminate()终止整个程序，这种情况下不能保证所有局部对象会被正确地销毁。(6). catch块的参数推荐采用地址传递而不是值传递，不仅可以提高效率，还可以利用对
 象的多态性。另外，派生类的异常扑获要放到父类异常扑获的前面，否则，派生类的异常无法被扑获。(7). 编写异常说明时，要确保派生类成员函数的异常说明和基类成员函数的异常说明一致，即派生类改写的虚函数的异常说明至少要和对应的基类虚函数的异常说明相同，甚至更加严格，更特殊。
