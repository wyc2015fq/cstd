# C++中你不知道的语法 - gauss的专栏 - CSDN博客
2013年01月04日 00:13:44[gauss](https://me.csdn.net/mathlmx)阅读数：260
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
传构造函数，不会引起拷贝构造函数的调用
当传一个一个已经构造好的对象时，
**[cpp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- #include <iostream>
- #include <stdio.h>
- #include <vector>
- #include <algorithm>
- usingnamespace std;  
- class Foo  
- {  
- public:  
-     Foo(){cout << "ctr" << endl;}  
-     ~Foo(){cout << "de-ctr" << endl;}  
-     Foo(const Foo& rhs)  
-     {  
-         cout << "copy-ctr" << endl;  
-     }  
-     Foo& operator=(const Foo& rhs)  
-     {  
-         cout << "operator=" << endl;  
-     }  
- };  
- void f(Foo f)  
- {  
- }  
- int main()  
- {  
-     Foo a;  
-     f(a);  
- return 0;  
- }  
结果为：
**[c-sharp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- ctr  
- copy-ctr  
- dectr  
- dectr  
当传参时构造对象时，
**[c-sharp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- int main()  
- {  
-     f(Foo());  
- return 0;  
- }  
结果就变成：
**[c-sharp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- ctr  
- dectr  
引申：
同理在模板类当中，同样成立，你不要把它看成一个函数调用了，它是也一个临时对象。如下：
**[cpp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- #include <iostream>
- #include <stdio.h>
- #include <vector>
- #include <algorithm>
- usingnamespace std;  
- template <typename T>  
- class print  
- {  
- public:  
-     print(){cout << "ctr" << endl;}  
-     ~print(){cout << "de-ctr" << endl;}  
-     print(const print<T>& rhs)  
-     {  
-         cout << "copy-ctr" << endl;  
-     }  
-     print<T>& operator=(const print<T>& rhs)  
-     {  
-         cout << "operator=" << endl;  
-     }  
- void operator()(const T& elem)  
-     {  
-         cout << elem << endl;  
-     }  
- };  
- void f(print<int> f)  
- {  
-     f(1);  
- }  
- int main()  
- {  
-     f(print<int>());  
- return 0;  
- }  
new操作符-空间还可以这样分配
**[cpp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- #include <iostream>
- usingnamespace std;  
- class Foo  
- {  
- public:  
- int i;  
-     Foo(){ cout << "ctr" << endl;}  
-     ~Foo(){cout << "dectr" << endl;}  
-     Foo(const Foo& rhs)  
-     {  
-         i = rhs.i;  
-         cout << "copy-ctr" << endl;  
-     }  
-     Foo& operator=(const Foo& rhs)  
-     {  
-         i = rhs.i;  
-         cout << "operator=" << endl;  
- return *this;  
-     }  
- };  
- int main()  
- {  
- //test1
-     Foo* p = new Foo();  
-     p->i = 1;  
-     Foo f;  
-     f.i = 2;  
-     Foo *p2 = new(p) Foo(f);  
-     cout << p2->i << endl;  
-     p2->i = 3;  
-     cout << p->i;  
-     cout << endl;  
- delete p2;  
- //delete p; //肯定会出错,因为重复删除了同一个空间
-     cout << endl;  
- //test2
- char* str = newchar[sizeof(Foo)];  
-     Foo *p3 = new(str) Foo(f);  
-     cout << p3->i << endl;  
- return 0;  
- }  
在上例中，Foo *p2 = new(p) Foo(f); 意思是说在对象p的空间上，构造一个跟f一样的对象。
而Foo *p3 = new(str) Foo(f);是在一个字符串的空间上构造的。
下面两个函数实际上事一样的，你知道么？
**[cpp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- typedefvoid (*func)();  
- func set_handler1(func f)  
- {  
-     func tmp = f;  
- return tmp;  
- }  
- void (* set_handler2(void (*f)()))()  
- {  
-     func tmp = f;  
- return tmp;  
- }  
对象的隐式构造
**[cpp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- //隐式构造
- class Foo  
- {  
- public:  
-     Foo(int i){cout << "ctr" << endl;}  
-     Foo(const Foo& f){cout << "copy-ctr" << endl;}  
-     Foo& operator=(const Foo& f){cout << "operator=" << endl; return *this;}  
-     ~Foo(){cout << "dectr" << endl;}  
- };  
- 
- class Test  
- {  
- public:  
-     Foo f()  
-     {  
- return 1;  
-     }  
- };  
- 
- void test()  
- {  
- //1
-     Test t;  
-     Foo f1 = t.f();  
- 
- //2
-     Foo f2 = 1;  
- }  
- int main()  
- {  
-     test();   
- return 0;  
- }  
这是可以运行的,也是C++支持的.
如果你不想让它这样趁你不注意,偷偷摸摸的构造了对象, 你可以在构造函数前面加上explicit.
**[c-sharp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- class Foo  
- {  
- public:  
- explicit Foo(int i){cout << "ctr" << endl;}  
-     Foo(const Foo& f){cout << "copy-ctr" << endl;}  
-     Foo& operator=(const Foo& f){cout << "operator=" << endl; return *this;}  
-     ~Foo(){cout << "dectr" << endl;}  
- };  
这时,就会报错.
预编译指令的妙处
**[cpp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- #define _B(x) #x
- #define PRINT(x, y, z) PRINT_##z(x,y)
- class BigInteger  
- {  
- public:  
-     BigInteger(constchar *){}  
- };  
- 
- void PRINT_MAX(int i, int j)  
- {cout << (i>j?i:j) << endl;}  
- 
- void PRINT_MIN(int i, int j)  
- {cout << (i>j?j:i) << endl;}  
- 
- void test(int i, int j)  
- {  
- //1
-     BigInteger b = _B(12122222222222222222222222);  
-     cout << _B(12122222222222222222222222) << endl;  
- 
- //2
-     PRINT(i, j, MAX);  
-     PRINT(i, j, MIN);  
- }  
- 
- int main()  
- {     
-     test(1, 2);  
- return 0;  
- }  
你猜猜结果?
上面的#x表示把x生成成为一个字符串.
a##b是把ab连接成一个字符串.
上面的两个用法和用途,相信可以让你感到妙不可言.
结果:
**[c-sharp]**[view
 plain](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)[copy](http://blog.csdn.net/xiaofengsheng/article/details/5400917#)
- 12122222222222222222222222  
- 2  
- 1  
- 请按任意键继续. . .  
