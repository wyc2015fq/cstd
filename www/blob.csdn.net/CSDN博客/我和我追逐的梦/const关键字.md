
# const关键字 - 我和我追逐的梦~~~ - CSDN博客


2011年10月21日 14:51:06[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：533标签：[reference																](https://so.csdn.net/so/search/s.do?q=reference&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=reference&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)



1.const 变量在定义时必须初始化，以后就不能修改其值了，就相当于常量。
2.const 对象默认为文件的局部变量。
在全局作用域里定义非const变量时，它在整个程序中都可以访问。
如：
//file_1.cc
int counter;  //定义全局变量
//file_2.cc
extern int counter; //声明、 使用file_1中的counter
++counter;
与其他变量不同，除非特别说明，在全局作用域声明的const变量是定义该对象的文件的局部变量。此变量只存在与那个文件中，不能被其他文件访问。但，通过指定const变量为extern，就可以在整个程序中访问const对象。
如：
//file_1.cc
extern const int val = fcn();//定义全局const变量
//file_2.cc
extern const int val;  //声明、 使用file_1中的const变量
...use val code ...
3.const引用
const引用是指向const对象的引用
const int val_1  = 10;
const int &refVal_1 =  val_1; //right:refVal_1  不能修改比如 refVal_1 = 20 error
int &refVal_2 = val_1; // error: nonconst reference to a const
const引用可以初始化为不同类型的对象或者用右值初始化
如：
int i= 10；
const int &r = 10; //right
const int &r1 = r + i; //right
但上面对与非const引用是错误的
非const引用只能绑定到与该引用同类型的对象
const引用则可以绑定到不同但相关的类型的对象或绑定到右值。
如：下面是正确的
double dval = 3.14;
const int &ref = dval;


