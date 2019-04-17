# c++ const 关键字 const与#define区别 - 别说话写代码的博客 - CSDN博客





2017年07月21日 09:25:42[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：191








1.定义常量（必须进行初始化，且常量不可修改）   const int a=10;   或int const a=10;

2.定义指向常量的指针(常量指针)（通过常量指针不能修改指针所指内容的值，可改变指向） int a=19;  const int *pa=&a;

3.定义指针常量（必须在声明时进行初始化，指针常量的值不可变，可以修改指针所指内容）

     int b=10;  int *const pb=&b;

4.定义指向常量的指针常量(指针常量指向指针对象，但他又指向常量，所以它指向的对象不可变）

     const int a=10;     const int *const pa=&a;








#include <iostream>

using std::cout;

using std::endl;




int main()

{

     const int a=100;//const常量，必须进行初始化，且常量值不可改变

     //int &pa=a;//常量不可引用

     //int *pa=&a;//只有常量指针才可以指向常量




     cout<<a<<endl;




     int b=1;

     const int *pb=&b;//常量指针,通过常量指针不能修改指针所指内容的值,可以改变指向

     //*pb=2;//不可以

     pb=&a;//改变指向可以

     b=2;//直接改变其值可以




     cout << *pb <<endl;




     int c=5;

     int * const pc=&c;//指针常量，可以修改指针所指内容的值，不可修改指向

     *pc=2;//可以改变其内容

     c=4;//可以直接改变内容

     //pc=&b;//错误，不可改变其指向




     cout << *pc <<endl;




     int d=55;

     const int * const pd=&d;//指向常量的指针常量，都不可变

     //*pd=1;//不可通过指针改变变量的值

     //pd=&a;//不可改变指向

     //d=6;//可以直接改变变量的值

     cout << *pd<< endl;




     return 0;

}








运行结果




![](https://img-blog.csdn.net/20170721092556283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







const 与 #define区别（尽量以const代替#define）

（1）编译器处理方式不同

     -define宏在预处理阶段展开

     -const常量在编译运行阶段使用

（2）类型和安全检查不同

     -define宏没有类型，不做任何类型检查，只是展开

     -const常量有具体类型，在编译阶段会执行类型检查



