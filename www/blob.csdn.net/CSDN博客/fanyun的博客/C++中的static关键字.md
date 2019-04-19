# C++中的static关键字 - fanyun的博客 - CSDN博客
2016年04月05日 11:01:10[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：594标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[](https://blog.csdn.net/column/details/.html)[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
C++的static有两种用法：面向过程程序设计中的static和面向对象程序设计中的static。前者应用于普通变量和函数，不涉及类；后者主要说明static在类中的作用。
一、面向过程设计中的static
1、静态全局变量
在全局变量前，加上关键字static，该变量就被定义成为一个静态全局变量。我们先举一个静态全局变量的例子，如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- #include<iostream>
- usingnamespace std;  
- 
- staticint n;  //定义静态全局变量
- 
- void fn()  
- {  
-     n++;  
-     cout<<n<<endl;  
- }  
- 
- int main(void)  
- {  
-     n = 20;  
-     cout<<n<<endl;  
-     fn();  
- return 0;  
- }  
静态全局变量有以下特点：- 该变量在全局数据区分配内存；
- 未经初始化的静态全局变量会被程序自动初始化为0（自动变量的值是随机的，除非它被显式初始化）；
- 静态全局变量在声明它的整个文件都是可见的，而在文件之外是不可见的；　
静态变量都在全局数据区分配内存，包括后面将要提到的静态局部变量。对于一个完整的程序，在内存中的分布情况如下图：
栈区
　　一般程序的由new产生的动态数据存放在堆区，函数内部的自动变量存放在栈区。自动变量一般会随着函数的退出而释放空间，静态数据（即使是函数内部的静态局部变量）也存放在全局数据区。全局数据区的数据并不会因为函数的退出而释放空间。细心的读者可能会发现，Example 1中的代码中将
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- staticint n;  //定义静态全局变量
改为
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- int n;  //定义全局变量
程序照样正常运行。
的确，定义全局变量就可以实现变量在文件中的共享，但定义静态全局变量还有以下好处：
       静态全局变量不能被其它文件所用；
       其它文件中可以定义相同名字的变量，不会发生冲突；
您可以将上述示例代码改为如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- //File1
- #include<iostream>
- usingnamespace std;  
- 
- void fn();  
- staticint n;  //定义静态全局变量
- 
- int main(void)  
- {  
-     n = 20;  
-     cout<<n<<endl;  
-     fn();  
- return 0;  
- }  
- 
- //File2
- #include<iostream>
- usingnamespace std;  
- 
- externint n;  
- 
- void fn()  
- {  
-     n++;  
-     cout<<n<<endl;  
- }  
编译并运行这个程序，您就会发现上述代码可以分别通过编译，但运行时出现错误。试着将
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- staticint n;  //定义静态全局变量
改为
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- int n;  //定义全局变量
再次编译运行程序，细心体会全局变量和静态全局变量的区别。
2、静态局部变量
在局部变量前，加上关键字static，该变量就被定义成为一个静态局部变量。
我们先举一个静态局部变量的例子，如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- #include<iostream>
- usingnamespace std;  
- 
- void fn();  
- 
- int main(void)  
- {  
-     fn();  
-     fn();  
-     fn();  
- return 0;  
- }  
- 
- void fn()  
- {  
- staticint n = 10;  
-     cout<<n<<endl;  
-     n++;  
- }  
通常，在函数体内定义了一个变量，每当程序运行到该语句时都会给该局部变量分配栈内存。但随着程序退出函数体，系统就会收回栈内存，局部变量也相应失效。
　　但有时候我们需要在两次调用之间对变量的值进行保存。通常的想法是定义一个全局变量来实现。但这样一来，变量已经不再属于函数本身了，不再仅受函数的控制，给程序的维护带来不便。
　　静态局部变量正好可以解决这个问题。静态局部变量保存在全局数据区，而不是保存在栈中，每次的值保持到下一次调用，直到下次赋新值。
静态局部变量有以下特点：
（1）该变量在全局数据区分配内存；
    （2）静态局部变量在程序执行到该对象的声明处时被首次初始化，即以后的函数调用不再进行初始化；
    （3）静态局部变量一般在声明处初始化，如果没有显式初始化，会被程序自动初始化为0；
    （4）它始终驻留在全局数据区，直到程序运行结束。但其作用域为局部作用域，当定义它的函数或语句块结束时，其作用域随之结束；
3、静态函数
　　在函数的返回类型前加上static关键字,函数即被定义为静态函数。静态函数与普通函数不同，它只能在声明它的文件当中可见，不能被其它文件使用。
静态函数的例子：
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- #include<iostream>
- usingnamespace std;  
- 
- staticvoid fn();   //声明静态函数
- 
- int main(void)  
- {  
-     fn();  
- return 0;  
- }  
- 
- void fn()     //定义静态函数
- {  
- int n = 10;  
-     cout<<n<<endl;  
- }  
定义静态函数的好处：
       静态函数不能被其它文件所用；
       其它文件中可以定义相同名字的函数，不会发生冲突；
二、面向对象的static关键字（类中的static关键字）
1、静态数据成员
在类内数据成员的声明前加上关键字static，该数据成员就是类内的静态数据成员。先举一个静态数据成员的例子。
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- #include<iostream>
- usingnamespace std;  
- 
- class Myclass  
- {  
- private:  
- int a , b , c;  
- staticint sum;  //声明静态数据成员
- public:  
-     Myclass(int a , int b , int c);  
- void GetSum();  
- };  
- 
- int Myclass::sum = 0;   //定义并初始化静态数据成员
- 
- Myclass::Myclass(int a , int b , int c)  
- {  
- this->a = a;  
- this->b = b;  
- this->c = c;  
-     sum += a+b+c;  
- }  
- void Myclass::GetSum()  
- {  
-     cout<<"sum="<<sum<<endl;  
- }  
- 
- int main(void)  
- {  
-     Myclass M(1 , 2 , 3);  
-     M.GetSum();  
-     Myclass N(4 , 5 , 6);  
-     N.GetSum();  
-     M.GetSum();  
- return 0;  
- }  
可以看出，静态数据成员有以下特点：- 对于非静态数据成员，每个类对象都有自己的拷贝。而静态数据成员被当作是类的成员。无论这个类的对象被定义了多少个，静态数据成员在程序中也只有一份拷贝，由该类型的所有对象共享访问。也就是说，静态数据成员是该类的所有对象所共有的。对该类的多个对象来说，静态数据成员只分配一次内存，供所有对象共用。所以，静态数据成员的值对每个对象都是一样的，它的值可以更新；
- 静态数据成员存储在全局数据区。静态数据成员定义时要分配空间，所以不能在类声明中定义。在Example 5中，语句int Myclass::Sum=0;是定义静态数据成员；
- 静态数据成员和普通数据成员一样遵从public,protected,private访问规则；
- 因为静态数据成员在全局数据区分配内存，属于本类的所有对象共享，所以，它不属于特定的类对象，在没有产生类对象时其作用域就可见，即在没有产生类的实例时，我们就可以操作它；
- 静态数据成员初始化与一般数据成员初始化不同。静态数据成员初始化的格式为：
＜数据类型＞＜类名＞::＜静态数据成员名＞=＜值＞
- 类的静态数据成员有两种访问形式：
＜类对象名＞.＜静态数据成员名＞ 或 ＜类类型名＞::＜静态数据成员名＞
如果静态数据成员的访问权限允许的话（即public的成员），可在程序中，按上述格式来引用静态数据成员 ；
- 静态数据成员主要用在各个对象都有相同的某项属性的时候。比如对于一个存款类，每个实例的利息都是相同的。所以，应该把利息设为存款类的静态数据成员。这有两个好处，第一，不管定义多少个存款类对象，利息数据成员都共享分配在全局数据区的内存，所以节省存储空间。第二，一旦利息需要改变时，只要改变一次，则所有存款类对象的利息全改变过来了；
- 同全局变量相比，使用静态数据成员有两个优势：
- 静态数据成员没有进入程序的全局名字空间，因此不存在与程序中其它全局名字冲突的可能性；
- 可以实现信息隐藏。静态数据成员可以是private成员，而全局变量不能；
2、静态成员函数
　　与静态数据成员一样，我们也可以创建一个静态成员函数，它为类的全部服务而不是为某一个类的具体对象服务。静态成员函数与静态数据成员一样，都是类的内部实现，属于类定义的一部分。普通的成员函数一般都隐含了一个this指针，this指针指向类的对象本身，因为普通成员函数总是具体的属于某个类的具体对象的。通常情况下，this是缺省的。如函数fn()实际上是this->fn()。**但是与普通函数相比，静态成员函数由于不是与任何的对象相联系，因此它不具有this指针。从这个意义上讲，它无法访问属于类对象的非静态数据成员，也无法访问非静态成员函数，它只能调用其余的静态成员函数。**下面举个静态成员函数的例子。
**[cpp]**[view
 plain](http://blog.csdn.net/hackbuteer1/article/details/7487694#)[copy](http://blog.csdn.net/hackbuteer1/article/details/7487694#)
- #include<iostream>
- usingnamespace std;  
- 
- class Myclass  
- {  
- private:  
- int a , b , c;  
- staticint sum;  //声明静态数据成员
- public:  
-     Myclass(int a , int b , int c);  
- staticvoid GetSum();  //声明静态成员函数
- };  
- 
- int Myclass::sum = 0;   //定义并初始化静态数据成员
- 
- Myclass::Myclass(int a , int b , int c)  
- {  
- this->a = a;  
- this->b = b;  
- this->c = c;  
-     sum += a+b+c;    //非静态成员函数可以访问静态数据成员
- }  
- void Myclass::GetSum()    //静态成员函数的实现
- {  
- //cout<<a<<endl;    //错误代码，a是非静态数据成员
-     cout<<"sum="<<sum<<endl;  
- }  
- 
- int main(void)  
- {  
-     Myclass M(1 , 2 , 3);  
-     M.GetSum();  
-     Myclass N(4 , 5 , 6);  
-     N.GetSum();  
-     Myclass::GetSum();  
- return 0;  
- }  
关于静态成员函数，可以总结为以下几点：
- 出现在类体外的函数定义不能指定关键字static；
- 静态成员之间可以相互访问，包括静态成员函数访问静态数据成员和访问静态成员函数；
- 非静态成员函数可以任意地访问静态成员函数和静态数据成员；
- 静态成员函数不能访问非静态成员函数和非静态数据成员；
- 由于没有this指针的额外开销，因此静态成员函数与类的全局函数相比速度上会有少许的增长；
- 调用静态成员函数，可以用成员访问操作符(.)和(->)为一个类的对象或指向类对象的指针调用静态成员函数，也可以直接使用如下格式：
＜类名＞::＜静态成员函数名＞（＜参数表＞）
调用类的静态成员函数。
