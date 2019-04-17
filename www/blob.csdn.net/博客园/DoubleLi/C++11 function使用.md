# C++11 function使用 - DoubleLi - 博客园






function是一组函数对象包装类的模板，实现了一个泛型的回调机制。

引入头文件

#include <functional>
using namespace std;
using namespace std::placeholders;  //bind的时候会用`

参考：[http://www.cnblogs.com/hujian/archive/2012/12/07/2807605.html](http://www.cnblogs.com/hujian/archive/2012/12/07/2807605.html)

fuction  bind:[http://blog.csdn.net/fjb2080/article/details/7527715](http://blog.csdn.net/fjb2080/article/details/7527715)

我们可以调用的对象有很多，比如普通函数、函数指针、lanmbda表达式、函数对象和类的成员函数等。

不管采用哪种方式，主要调用形式一样（返回值类型、传递给调用的实参类型），我们就可以使用同一种形式来调用。

这个时候就可以用到function模板，它给予我们在调用的方式上更大的弹性。

请看一下三种不同的函数定义：





**[cpp]**[view plain](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[copy](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

[print](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[?](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

- int add(int a, int b){  
- return a+b;  
- }  
- auto mod=[](int a, int b){return a%b;};  
- struct divide{  
- int operator()(int m, int n){  
- return m/n;  
-     }  
- };  




这三种都可以使用同一种调用形式，int(int, int)，调用方式如下：





**[cpp]**[view plain](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[copy](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

[print](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[?](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

- function<int(int,int)> func1= add;  
- function<int(int,int)> func2= divide();  
- function<int(int,int)> func3= mod;  
- cout<<func1(5, 6)<<endl;  
- cout<<func2(5, 6)<<endl;  
- cout<<func3(5, 6)<<endl;  



学会了使用function，可以继续如下进行抽象定义，不同类型采用相同的调用方法：







**[cpp]**[view plain](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[copy](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

[print](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[?](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

- map<string,function<int(int, int)>> funs =  
- {  
-     {"+", add},  
-     {"-", std::minus<int>()},//标准库的函数，参数为两个整数，可以参考前一篇博客  
-     {"/", divide()},//类成员函数  
-     {"*", [](int i,int j){return i*j;}},//lambda表达式  
-     {"%", mod},  
- };  
- funs["+"](4,6);  



以上就是function的简单使用。下面是从另一篇博客转载的，使用function的引用来保存函数对象。考虑下面代码：







**[cpp]**[view plain](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[copy](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

[print](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[?](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

- class CAdd  
- {  
- public:  
-     CAdd():m_nSum(0){NULL;}  
- int operator()(int i)  
-     {  
-         m_nSum += i;  
- return m_nSum;  
-     }  
- 
- int Sum() const  
-     {  
- return m_nSum;  
-     }  
- 
- private:  
- int m_nSum;  
- };  
- int main(int argc, const char * argv[])  
- {  
-     CAdd cAdd;  
-     function<int(int)> funcAdd1 = cAdd;  
-     function<int(int)> funcAdd2 = cAdd;  
-     cout<<funcAdd1(10)<<endl;  
-     cout<<funcAdd2(10)<<endl;  
-     cout<<cAdd.Sum()<<endl;  
- 
- return 0;  
- }  




上面的输出结果是 10 10 0。我们将同一个函数对象赋值给了两个function，然后分别调用这两个function，但函数中的成员变量的值没有保存，问题在哪里？因为function的缺省行为是拷贝一份传递给它的函数对象，于是f1,f2中保存的都是cAdd对象的拷贝。

[C++](http://lib.csdn.net/base/cplusplus)11提供了ref和cref函数来提供对象的引用和常引用的包装。要是function能够正确保存函数对象的状态，可以如下修改代码：





**[cpp]**[view plain](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[copy](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

[print](http://blog.csdn.net/hanbingfengying/article/details/28651507#)[?](http://blog.csdn.net/hanbingfengying/article/details/28651507#)

- function<int(int)> funcAdd3 = ref(cAdd);  
- function<int(int)> funcAdd4 = ref(cAdd);  
- cout<<funcAdd3(10)<<endl;  
- cout<<funcAdd4(10)<<endl;  
- cout<<cAdd.Sum()<<endl;  







        另外，两个function之间赋值时，如果源function保存的是函数对象的拷贝，则目标function保存的也是函数对象的拷贝。如果源function保存的是对函数对象的引用，则目标function保存的也是函数对象的引用。









