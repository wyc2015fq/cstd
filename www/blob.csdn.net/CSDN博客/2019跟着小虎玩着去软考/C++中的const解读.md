# C++中的const解读 - 2019跟着小虎玩着去软考 - CSDN博客
2011年05月23日 18:44:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1464标签：[c++																[编译器																[class																[存储																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C/C++](https://blog.csdn.net/littletigerat/article/category/666612)
**C++****中的****const****解读**
# 一．const常量与宏常量相比的优势
1．const常量有数据类型，而宏常量没有数据类型；
2．编译器可以对前者进行类型安全检查，而对后者只进行字符替换，没有类型安全检查；
3．在宏常量的使用过程中，字符替换时可能会产生意料不到的错误；
# 二．为什么不能在类声明中初始化const数据成员？
class {
const int nCount;
}；
因为类的对象未被创建时，编译器不知道const 数据成员的值是什么。
# 三．如何理解类中的const数据成员的意义呢？
**const****数据成员只是：针对某个对象生存期内是常量，而对于整个类而言却是可变的。**因为类可以创建多个对象，不同的对象其const数据成员的值可以不同。
# 四．如何对类中的const数据成员进行初始化？
class A 
 { 
const int size = 100;    //错误
int array[size];         //错误，未知的size 
 } ；
# 五．如何建立在整个类中都恒定的常量？
应该使用类中的枚举常量来实现。具体如下：
 class A 
 {… 
   enum {size1=100, size2 = 200 }; 
int array1[size1]; 
int array2[size2];  
} ；
枚举常量不会占用对象的存储空间，他们在编译时被全部求值。
# 六．使用枚举变量的缺点是什么？
1．枚举常量的隐含数据类型是整数；
2．其最大值有限；
3．只能表示为整数，且不能表示浮点数，字符串等；
# 七．引用const常量使用需要注意什么？
A f; 
const A& e = f;
e只能访问声明为const的函数，而不能访问一般的成员函数；
# 八．对于非内部数据类型的输入参数，将“值传递”的方式改为“const引用传递”有什么好处？
目的是为了提高效率。例如：将void Func(A a)改为void Func(const A &a)
# 九．对于内部数据类型的输入参数，不要将“值传递”的方式改为“const引用传递”，为什么？
既达不到提高效率的目的，又降低了函数的可理解性。
例如void Func(int x)不应该改为void Func(const int &x) 
# 十．如何初始化类内部的常量？
很常见的方法就是初始化列表：
class A
{
public:
A(int i=0):test(i) {}
private:
const int i;
}；
# 十一．const在成员函数中的三种用法
1．参数；
2．返回值；
3．函数；
# 十二．使用const的一些建议
1．除了重载操作符外一般不要将返回值类型定为对某个对象的const引用；
2．不要轻易的将函数的返回值类型定为const；
3．在参数中使用const应该使用引用或指针，而不是一般的对象实例
