# Linux中的线程局部存储（2） - 文章 - 伯乐在线
原文出处： [cywosp](http://blog.csdn.net/cywosp/article/details/26876231)
《[Linux 中的线程局部存储（1）](http://blog.jobbole.com/102540/)》提到了一种，其实在Linux中还有一种更为高效的线程局部存储方法，就是使用关键字__thread来定义变量。
__thread是GCC内置的线程局部存储设施（Thread-Local Storage），它的实现非常高效，与pthread_key_t向比较更为快速，其存储性能可以与全局变量相媲美，而且使用方式也更为简单。创建线程局部变量只需简单的在全局或者静态变量的声明中加入__thread说明即可。列如：


```
static __thread char t_buf[32] = {'\0'};
extern __thread int t_val = 0;
```
凡是带有__thread的变量，每个线程都拥有该变量的一份拷贝，且互不干扰。线程局部存储中的变量将一直存在，直至线程终止，当线程终止时会自动释放这一存储。__thread并不是所有数据类型都可以使用的，因为其只支持POD(Plain old data structure)[1]类型，不支持class类型——其不能自动调用构造函数和析构函数。同时__thread可以用于修饰全局变量、函数内的静态变量，但是不能用于修饰函数的局部变量或者class的普通成员变量。另外，__thread变量的初始化只能用编译期常量，例如：


```
__thread std::string t_object_1 ("Swift");                   // 错误，因为不能调用对象的构造函数
__thread std::string* t_object_2 = new std::string (); // 错误，初始化必须用编译期常量
__thread std::string* t_object_3 = nullptr;                // 正确，但是需要手工初始化并销毁对象
```
除了以上之外，关于线程局部存储变量的声明和使用还需注意一下几点：
- 如果变量声明中使用量关键字static或者extern，那么关键字__thread必须紧随其后。
- 与一般的全局变量或静态变量一样，线程局部变量在声明时可以设置一个初始化值。
- 可以使用C语言取地址符(&)来获取线程局部变量的地址。
__thread的使用例子可参考 https://github.com/ApusApp/Swift/blob/master/swift/base/logging.cpp 的实现及其单元测试对于那些非POD数据类型，如果想使用线程局部存储机制，可以使用对pthread_key_t封装的类来处理，具体方式可参考https://github.com/ApusApp/Swift/blob/master/swift/base/threadlocal.h 的实现以及其的单元测试
