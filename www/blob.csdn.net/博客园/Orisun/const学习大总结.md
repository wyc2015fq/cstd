# const学习大总结 - Orisun - 博客园







# [const学习大总结](https://www.cnblogs.com/zhangchaoyang/articles/1812639.html)






const学习大总结

1.const int i=8;

常量在定义后不能更改，所以定义时必须初始化

2.const对象默认为头文件的局部变量

非const变量默认为extern。要使const变量能够在其他文件中访问，必须显式地指定它为extern。

//file_1.cc

int counter;

//file_2.cc

extern int counter;

++counter;

--------------------------------

//file_1.cc

extern const int bufSize=fcn();

//file_2.cc

extern const int bufSize;

if(i<bufSize)



插一下extern的相关介绍

 extern   int   a；//这是变量声明，是告诉编译器到该文件外部去找这个文件的定义   
                              //声明是不分配内存的   
 int   a;//这是变量定义，变量定义是分配空间的   
  定义只能有一处，但声明可有多处，这些声明所指，都是定义时分配的内存空间   

 extern   int   a   是对变量的声明，而不是定义，该变量是其他文件中定义的，或是本文件中在该声明之后定义的全局变量。

 extern   int   a   定义的a变量表示该变量在使用前已经定义了，一般有两种情况，一是该变量是在其他的文件中定义的全局变量，简称外部变量。二种情况表示变量在本文件中定义的全局变量，但是在使用之后（因为作用域）定义的。  

3.const引用

首先说一下“引用”是别名，即引用只是它绑定的对象的另一个名字，作用在引用上的操作都实际作用在该引用绑定的对象上。

const引用是指向const对象的引用。

const int ival=1024;

const int &refval=ival;

int &ref2=ival;//错误，非常量引用不能绑定到常量对象

4.“一些”const对象定义在头文件中

如果const变量不是用常量表达式初始化的，就不应该把它定义在头文件中，而应该把它在一个源文件中定义并初始化。同时在头文件中为它添加extern声明，以使其能在多个文件共享。

5.指向const对象的指针

const double pi=3.14;

const double *ptr=π

double *ptr=π//错误，指向const对象的指针也必须具有const特性。

实践中通常把指向const对象的指针作为形参传给函数，以确保在函数中对象不会被修改。

6.const指针--本身的值不能被修改

int errNum=0;

int const * curAErr=&errNum;//curErr不能再指向其他对象

7.指向const对象的const指针

综合5，6可得

8.const引用作为形参

bool isShoter(const string &s1,const string &s2){

return s1.size()<s2.size();

}

一方面用引用作参数避免了对象的复制，而又是const引用，所以函数不有使用该引用来修改实参。

9.邻近原则

int const *ptr和const int *ptr：ptr是个指针，它指向的location是只读的。

int * const ptr：ptr是只读的，ptr是个指针。

int *&v1;        指向引用的指针

#include<iostream>
using namespace std;
int main(){
	int a=10;
	int b=20;
	int const * ptr1=&a;
	int * const ptr2=&a;
	const int * ptr3=&a;
	*ptr1=13;
	ptr2=&b;
	*ptr3=11;
	return 0;
}

![](https://pic002.cnblogs.com/images/2012/103496/2012090509395198.png)

可以看到编译器认为*ptr1和*ptr3这片locatoin是只读的，而对于ptr2认为这个variable本身是只读的。

10.const成员函数

不能修改类的数据成员。

11.返回类型为const的函数

const int fcn(){  }

extern const int bufSize=fcn();


12.const数据成员只能在初始化列表中进行初始化

这里要区分初始化和赋值。

class A{

　　const int a;

　　int b:

　　A(int i,int j):a(i){

　　　　b=j;

　　}

};

在构造函数中，对a就是进行初始化，而对b是进行赋值。赋值是擦除当前的对象用新值替换。












