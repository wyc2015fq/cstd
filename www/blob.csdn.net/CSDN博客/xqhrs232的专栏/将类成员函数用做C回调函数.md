# 将类成员函数用做C回调函数 - xqhrs232的专栏 - CSDN博客
2013年11月14日 21:20:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：834
原文地址::[http://www.51testing.com/html/38/n-806638.html](http://www.51testing.com/html/38/n-806638.html)
提出问题：
　　回调函数是基于C编程的Windows SDK的技术，不是针对C++的，程序员可以将一个C函数直接作为回调函数，但是如果试图直接使用C++的成员函数作为回调函数将发生错误，甚至编译就不能通过。
　　分析原因：
　　普通的C++成员函数都隐含了一个传递函数作为参数，亦即“this”指针，C++通过传递一个指向自身的指针给其成员函数从而实现程序函数可以访问C++的数据成员。这也可以理解为什么C++类的多个实例可以共享成员函数但是确有不同的数据成员。由于this指针的作用，使得将一个CALLBACK型的成员函数作为回调函数安装时就会因为隐含的this指针使得函数参数个数不匹配，从而导致回调函数安装失败
　　解决方案：
　　一、不使用成员函数，直接使用普通C函数，为了实现在C函数中可以访问类的成员变量，可以使用友元操作符（friend），在C++中将该C函数说明为类的友元即可。这种处理机制与普通的C编程中使用回调函数一样。
　　二、使用静态成员函数，静态成员函数不使用this指针作为隐含参数，这样就可以作为回调函数了。静态成员函数具有两大特点：其一，可以在没有类实例的情况下使用；其二，只能访问静态成员变量和静态成员函数，不能访问非静态成员变量和非静态成员函数。由于在C++中使用类成员函数作为回调函数的目的就是为了访问所有的成员变量和成员函数，如果作不到这一点将不具有实际意义。我们通过使用静态成员函数对非静态成员函数包装的办法来解决问题。类实例可以通过附加参数或全局变量的方式的方式传递到静态成员函数中。分别举例如下：
　　1、参数传递的方式
#include <iostream.h>
   class TClassA
   {
   public:
      void Display(const char* text) { cout << text << endl; };
      static void Wrapper_To_Call_Display(void* pt2Object, char* text);
      // more....
   };
   // 静态包装函数，能够调用成员函数Display(),本身做为回调函数来使用
   void TClassA::Wrapper_To_Call_Display(void* pt2Object, char* string)
   {
       // 显式类型转换
       TClassA* mySelf = (TClassA*) pt2Object;
       // 调用普通成员函数
       mySelf->Display(string);
   }
   // 回调函数的宿主,在这里回调用函数被使用
   void DoItA(void* pt2Object, void (*pt2Function)(void* pt2Object, char* text))
   {
      // 使用回调函数
      pt2Function(pt2Object, "hi, i'm calling back using a argument ;-)");
   }
   // 执行示例
   void Callback_Using_Argument()
   {
      TClassA objA;
      DoItA((void*) &objA, TClassA::Wrapper_To_Call_Display);
   }
2、全局变量的方式
#include <iostream.h>
   void* pt2Object;        // 全局变量,可以指向任意对象
   class TClassB
   {
   public:
      void Display(const char* text) { cout << text << endl; };
      static void Wrapper_To_Call_Display(char* text);
   };
   // 静态的包装函数
   void TClassB::Wrapper_To_Call_Display(char* string)
   {
       //需要保证全局变量值的正确性
       TClassB* mySelf = (TClassB*) pt2Object;
       mySelf->Display(string);
   }
   // 回调用函数的宿主,在这里回调用函数被使用
   void DoItB(void (*pt2Function)(char* text))
   {
      pt2Function("hi, i'm calling back using a global ;-)");   // make callback
   }
   // 执行示例
   void Callback_Using_Global()
   {
      TClassB objB;
      pt2Object = (void*) &objB;
      DoItB(TClassB::Wrapper_To_Call_Display);
   }
注意：通过上面两种方法的比较可以看出，第2种方法中静态包装函数可以和普通成员函数保持签名一致，当回调函数的宿主接口不能改变时，这种方法特别有用。但因为使用了全局变量，也不是一个好的设计。
