# C++中的const修饰符 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:56:49[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：384








- 定义常量。const double PI=3.1415926;  这个好于在C中的宏定义；
- 指向常量的指针。const int*p; p=&x;  p本身是个变量，可以修改，指向其余的常量。但是p指向的内容*p不能直接修改。
- 常指针。int *const p=&x;  指针p本身就是一个常量，只可以初始化，不能赋值改变。但是p指向的内容*p可以修改；
- 指向常量的常指针。 const int *const p=&x;  指针p和p指向的内容*p都不能修改；

将指针定义为常量或将指针所指向的内容理解为不可直接修改，在实际编程中经常会用到如下方法：当用指针做形式参数时，通过加const限制，保护对应的实际参数，以防止在函数中通过指针修改了实际参数的值。例如：double Sum(const double *p)。




