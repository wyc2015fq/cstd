# C语言中的静态函数的作用 - maopig的专栏 - CSDN博客
2011年08月25日 19:46:18[maopig](https://me.csdn.net/maopig)阅读数：1364
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载
在C语言中为什么要用静态函数(static function)？
如果不用这个static关键字，好象没有关系。那么，用了static以后，有什么作用呢？
我们知道，用了static的变量，叫做静态变量，其意义是，该变量的值在下次调用时，还继续保留前次调用时的值。
这里在一个函数前加上static关键字，其意思是这样的：
原来static函数是“restrict visibility of the **function**”，即该函数的作用域在另一个文件中是不可见的。
在C++中，函数的作用域一般是局限于其所定义的类中，虽静态函数不常见。在C++，“a static member (function)is part of its class but not part of any object”。
举例来说：
文件file1.c:
void func1( void )
{
}
extern void func2( void )
{
}
static void func3( void )
{
}
文件file2.c:
void func1( void );
void func2( void );
void func3( void );
int main( void )
{
func1(); 
  func2(); 
  func3(); 
  return 0;
}
在Cygwin下(或Linux下)用GCC编译：
  $ gcc file1.c file2.c
。。。 file2.c:(.text+0x35): undefined reference to `_func3'
这个错误信息表示func3没有定义，即它在file2.c中不可见。
