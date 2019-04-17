# C++ 引用 & 的详解 - 在思索中前行！ - CSDN博客





2015年03月30日 22:51:31[_Tham](https://me.csdn.net/txl16211)阅读数：591标签：[C++ 引用																[引用运算符																[运算符](https://so.csdn.net/so/search/s.do?q=运算符&t=blog)
个人分类：[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)





转载来自：[CSDNinsistGoGo  （http://blog.csdn.net/insistgogo/article/details/6655077）](http://blog.csdn.net/insistgogo/article/details/6655077)
引用的应用：
常引用：

语法：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- const 类型 &引用名=目标变量名;  


作用：防止通过引用对目标变量进行修改，达到了引用的安全性

注意：是防止通过引用进行修改，但可以直接对变量进行修改

使用范围：只要一个参数声明过来时只读的，都可以使用。常见有函数传参时，如果不想让参数被改变，则声明为常引用

举例1：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- #include<iostream>       
- usingnamespace std;  
- void main()  
- {  
- int a=0;  
- constint& b=a;  
-     a=1;//正确，不能通过引用进行修改，但是可以直接对它自己进行修改
-     b=1;//错误，b为a的引用，使用const的作用就是防止通过引用（b），对a进行修改
-     cout<<a<<b<<endl;  
-     system("pause");  
- }  


举例2：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- void ss(constint& a)  
- {  
-        a=1;//错误，不能通过a对它进行改变
- }  


说明：引用型参数应该在能被定义为const的情况下，尽量定义为const 。


举例3：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- 前提：  
- string foo( );  
- void bar(string & s);   
- 调用：  
- bar(foo( ));     
- bar("hello world");  


错误原因：试图将一个const类型的对象转换为非const类型是非法的。

因为：foo( )和"hello world"串都会产生一个临时对象，而在C++中，这些临时对象都是const类型的。

返回函数引用：函数返回一个变量，可以做左值和右值

语法：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- 类型& 函数名(形参列表及类型说明)  
- {  
- //函数体
- }  


优点：在内存中不产生被返回值的副本，而会直接得到返回值的存储单元，既可以做左值，也可以做右值。

举例：

返回函数引用：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- #include<iostream>       
- usingnamespace std;  
- int& B(int &n);  
- int main()  
- {  
- int a = 10;  
- int& b = B(a); //把a传入函数B中的n，之后返回n给b，这时b就是n也是a的引用，都指向一个空间
-     cout << b << endl;  
-     cout << a << endl;  
-     system("pause");  
- }  
- 
- int& B(int &n)  
- {  
-     n++;  
- return n;  
- }  


分析：return n; 就相当于是 return a;，返回到 b 的就是 a 的引用, b 就是 a对n 和 b 的操作会直接影响 a, 因为它们是同一個东西.

想对的例子：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- int A(int n) //函数返回的是一个值，而非变量
- {  
- return n;  
- }  


说明：int c=A(a);这样调用，n和a不是同一个空间，而是a的一个复制体，最终返回的只是个值的副本（这个副本不是n，n已经释放），没有引用任何东西，c得到的仅仅是

返回的值，而不是一个空间，所以以后对c的操作也没对a有任何影响。

返回函数引用常用的使用方法：把需要返回的参数，以引用或指针方式当作函数的参数，传入函数中。

举例：

函数返回值使用引用的方式传入：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- int& abc(int a, int b, int c, int& result)  
- {  
-     result = a + b + c;  
- return result;  
- }  


或

函数返回值使用指针的方式传入：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- int& abc(int a, int b, int c, int *result)  
- {  
-     *result = a + b + c;  
- return *result;  
- }  


这里的变量result不是一个临时变量，在函数外有效，因此，返回函数引用是可以的

错误的方式：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- int& abc(int a, int b, int c)  
- {  
- return a + b + c;  
- }  


这里a+b+c仅仅代表的是一个值，而不是一个可以引用的内存单元，而返回函数要求的是可以引用的变量，因此是错误的

注意：

1、不能返回局部变量的引用。

原因：局部变量会在函数返回后被销毁，被返回的引用就成为了"无所指"的引用，程序会进入未知状态。

2、不能返回函数内部new分配的内存的引用。

原因：虽然new申请的空间是被显式释放的，这就不存在局部空间被销毁的情况，但是被函数返回的引用只是作为一个临时变量，而没有赋予实际的变量，那么

这个引用所指向空间就无法释放，造成内存泄漏。

3、返回函数引用，可以和某些运算符重载结合，如超载输入输出运算符>>和<<。

原因：要支持运算符的连用，这时返回函数引用可以使赋值操作符的返回值必须是一个左值，以便可以被继续赋值。


说明：

1、引用的目的主要用于在函数参数传递中，解决大块数据或对象的传递效率和空间不如意的问题，而单纯给某个变量取个别名是毫无意义的。

2、用引用传递函数的参数，能保证参数传递中不产生副本，提高传递的效率，且通过const的使用，保证了引用传递的安全性。

3、常见的函数引用的时候：

流操作符<<和>>的返回值：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- friend ostream& operator<<(ostream& cout,const Point& p);//使用友元函数重载<<输出运算符   
- friend istream& operator>>(istream& cin,Point& p);//使用友元函数重载>>输出运算符  


赋值操作符=的返回值和参数：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- Point& Point::operator=(const Point&p)//重载赋值运算符


拷贝构造函数的参数：



**[cpp]**[view plain](http://blog.csdn.net/insistgogo/article/details/6655077#)[copy](http://blog.csdn.net/insistgogo/article/details/6655077#)

- Point::Point(const  Point& p)//拷贝构造函数](https://so.csdn.net/so/search/s.do?q=引用运算符&t=blog)](https://so.csdn.net/so/search/s.do?q=C++ 引用&t=blog)




