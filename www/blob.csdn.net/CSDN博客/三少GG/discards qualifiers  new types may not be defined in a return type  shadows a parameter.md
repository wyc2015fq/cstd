# discards qualifiers || new types may not be defined in a return type|| shadows a parameter - 三少GG - CSDN博客
2013年11月14日 23:10:10[三少GG](https://me.csdn.net/scut1135)阅读数：1754
### [在C/C++重编译错误new types may not be defined in a return type](http://blog.csdn.net/jajaa/article/details/2694261)
TcpServer.cpp:29: error: new types may not be defined in a return type
这种错误的解决：就是在这个#include中某一个头文件的class定义最后没有打;
添上后,就解决了。
比如这里#include "TcpServer.h"中class TcpServer定义最后没加；，加上后问题解决。
如果用到类，经常忘了在最后加分号~~~^_^~~~
****************************************************************************
C++程序的错误提示` declaration of  'int a' shadows a parameter
是指定义了同名的参数，造成了隐藏，例如：
int fun(int a)
{
    int a= 0;//shadows a parameter
    return 0;
}
*******************************************************************************
[discards
 qualifiers问题](http://blog.csdn.net/rainkin1993/article/details/8034657)
今晚中秋之夜，苦逼的程序员只能呆在寝室。
正无聊的时候，想起最近学的数据结构，想完善一下代码，于是想给线性表重载一下<<运算符。
Out（）
**[cpp]**[view
 plain](http://blog.csdn.net/rainkin1993/article/details/8034657#)[copy](http://blog.csdn.net/rainkin1993/article/details/8034657#)
- template<class T>  
- void LineList<T>::Out(ostream& os)  
-     {  
- for(int p = 0; p < length; p++)  
-             os<<array[p]<<endl;  
-     }  
重载<<
**[cpp]**[view
 plain](http://blog.csdn.net/rainkin1993/article/details/8034657#)[copy](http://blog.csdn.net/rainkin1993/article/details/8034657#)
- friend ostream& operator<<(ostream& out,const LineList<T>&line)  
-     {  
-         line.Out(out);  
- return out;  
-     }  
但是问题出现了。。
**[cpp]**[view
 plain](http://blog.csdn.net/rainkin1993/article/details/8034657#)[copy](http://blog.csdn.net/rainkin1993/article/details/8034657#)
- error:passing ‘const LineList<int>’ as ‘this’ argument of ‘void LineList<T>::Out(ostream&) [with T = int]’ discards qualifiers  
百思不得其解，于是百度，google吧。。
发现Stackoverflow上也有人有相同的问题
下面是他的问题：
For my compsci class, I am implementing a Stack template class, but have run into an odd error:
> 
Stack.h: In member function ‘
```
const
 T Stack<T>::top() const
```
 [with T = int]’:
Stack.cpp:10: error: passing ‘
```
const
 Stack<int>
```
’ as ‘`this`’
 argument of ‘`void Stack<T>::checkElements()` [with
 T = int]’ discards qualifiers
`Stack<T>::top()` looks
 like this:
```
const T top() const {
    checkElements();
    return (const T)(first_->data);
}
```
`Stack<T>::checkElements()` looks
 like this:
```
void checkElements() {
    if (first_==NULL || size_==0)
        throw range_error("There are no elements in the stack.");
}
```
The stack uses linked nodes for storage, so `first_` is
 a pointer to the first node.
Why am I getting this error?
正确答案是
Your `checkElements()` function
 is not marked as `const` so
 you can't call it on `const` qualified
 objects.
`top()`,
 however is const qualified so in `top()`, `this` is
 a pointer to a const `Stack` (even
 if the`Stack` instance
 on which `top()` was
 called happens to be non-`const`),
 so you can't call`checkElements()` which always requires
 a non-`const` instance.
意思是说  在一个加了const限定符的成员函数中，不能够调用 非const成员函数。
因为如果调用了非const成员函数，就违反了const成员函数不改变对象的规定。
而error：...discards qualifiers 的意思就是缺少限定符
因此，他的解决办法很简单了 ，只要将checkElements()函数申明改为  checkElements()const就行了
## 再回到我的问题，
因为重载<<中有一个参数为 const LineList<T>&line
注意：const对象只能调用const成员函数；
所以调用line.Out（cout）的时候，Out（）函数必须是const成员函数，因而出现了错误discards qualifiers
解决办法就是将Out(ostream& os)改为 Out（ostream& os）const 。
