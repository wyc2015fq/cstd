# 小问题大思考之 C++ 临时对象 - 文章 - 伯乐在线
原文出处： [imyfriend](http://blog.csdn.net/imyfriend/article/details/12886577)
C++中有这样一种对象：它在代码中看不到，但是确实存在。它就是临时对象—由编译器定义的一个没有命名的非堆对象（non-heap object）。为什么研究临时对象？主要是为了提高程序的性能以及效率，因为临时对象的构造与析构对系统性能而言绝不是微小的影响，所以我们应该去了解它们，知道它们如何造成，从而尽可能去避免它们。
临时对象通常产生于以下4种情况：
- 类型装换
- 按值传递
- 按值返回
- 对象定义
下面我们逐一看看：
1、类型转换：它通常是为了让函数调用成功而产生临时对象。发生于 “传递某对象给一个函数，而其类型与它即将绑定上去的参数类型不同” 的时候。
例如：


```
void test(const string& str);  
  
char buffer[] = "buffer";  
  
test(buffer); // 此时发生类型转换
```
此时，编译器会帮你进行类型转换：它产生一个类型为string的临时对象，该对象以buffer为参数调用string constructor。当test函数返回时，此临时对象会被自动销毁。
注意：对于引用（reference）参数而言，只有当对象被传递给一个reference-to-const参数时，转换才发生。如果对象传递给一个reference-to-non-const对象，不会发生转换。
例如：


```
void upper（string& str);  
  
char lower[] = "lower";  
  
upper(lower); // 此时不能转换，编译出错
```
此时如果编译器对reference-to-non-const对象进行了类型转换，那么将会允许临时对象的值被修改。而这和程序员的期望是不一致的。试想，在上面的代码中，如果编译器允许upper运行，将lower中的值转换为大写，但是这是对临时对象而言的，char lower[]的值还是“lower”，这和你的期望一致吗？
有时候，这种隐式类型转换不是我们期望的，那么我们可以通过声明constructor为explicit来实现。explicit告诉编译器，我们反对将constructor用于类型转换。
例如：


```
explicit string(const char*);
```
2、按值传递：这通常也是为了让函数调用成功而产生临时对象。当按值传递对象时，实参对形参的初始化与T formalArg = actualArg的形式等价。
例如：


```
void test（T formalArg);  
  
T actualArg;  
test(actualArg);
```
此时编译器产生的伪码为：


```
T _temp;  
  
_temp.T::T(acutalArg); // 通过拷贝构造函数生成_temp  
g(_temp);  // 按引用传递_temp  
_temp.T::~T(); // 析构_temp
```
因为存在局部参数formalArg，test（）的调用栈中将存在formalArg的占位符。编译器必须复制对象actualArg的内容到formalArg的占位符中。所以，此时编译器生成了临时对象。
3、按值返回：如果函数是按值返回的，那么编译器很可能为之产生临时对象。
例如：


```
class Integer {  
public:  
  friend Integer operator+(const Integer& a, const Integer& b);  
    
  Integer(int val=0): value(val) {  
  }  
    
  Integer(const Integer& rhs): value(rhs.value) {  
  }  
    
  Integer& operator=(const Integer& rhs);  
    
  ~Integer() {  
  }  
    
private:  
  int value;    
};  
  
Integer operator+(const Integer& a, const Integer& b) {  
  Integer retVal;  
    
  retVal.value = a.value + b.value;  
    
  return retVal;  
}  
  
Integer c1, c2, c3;  
c3 = c1 + c2;
```
编译器生成的伪代码：


```
struct Integer _tempResult; // 表示占位符，不调用构造函数  
operator+(_tempResult, c1, c2); // 所有参数按引用传递  
c3 = _tempResult; // operator=函数执行  
  
Integer operator+(const Integer& _tempResult, const Integer& a, const Integer& b) {  
  struct Integer retVal;  
  retVal.Integer::Integer(); // Integer(int val=0)执行  
    
  retVal.value = a.value + b.value;  
    
  _tempResult.Integer::Integer(retVal); // 拷贝构造函数Integer(const Integer& rhs)执行，生成临时对象。  
    
  retVal.Integer::~Integer(); // 析构函数执行  
    
  return;  
}  
    
  return retVal;  
}
```
如果对operator+进行返回值优化（RVO：Return Value Optimization），那么临时对象将不会产生。
例如：


```
Integer operator+(const Integer& a, const Integer& b) {    
  return Integer(a.value + b.value);  
}
```
编译器生成的伪代码：


```
Integer operator+(const Integer& _tempResult, const Integer& a, const Integer& b) {  
  _tempResult.Integer::Integer(); // Integer(int val=0)执行  
  _tempResult.value = a.value + b.value;  
    
  return;  
}
```
对照上面的版本，我们可以看出临时对象retVal消除了。
4、对象定义：
例如：


```
Integer i1(100); // 编译器肯定不会生成临时对象  
Integer i2 = Integer(100); // 编译器可能生成临时对象  
Integer i3 = 100; // 编译器可能生成临时对象
```
然而，实际上大多数的编译器都会通过优化省去临时对象，所以这里的初始化形式基本上在效率上都是相同的。
备注：
临时对象的生命期：按照C++标准的说法，临时对象的摧毁，是对完整表达式求值过程中的最后一个步骤。该完整表达式照成了临时对象的产生。
完整表达式通常是指包含临时对象表达式的最外围的那个。例如：
（（objA >1024）&&（objB <1024） ） ？ （objA – objB） ：（objB-objA）
这个表达式中一共含有5个表达式，最外围的表达式是？。任何一个子表达式所产生的任何一个临时对象，都应该在完整表达式被求值完成后，才可以销毁。
临时对象的生命周期规则有2个例外：
1、在表达式被用来初始化一个object时。例如：


```
String progName("test");  
String progVersion("ver-1.0");  
String progNameVersion = progName + progVersion
```
如果progName + progVersion产生的临时对象在表达式求值结束后就析构，那么progNameVersion就无法产生。所以，C++标准规定：含有表达式执行结果的临时对象，应该保留到object的初始化操作完成为止。
小心这种情况：


```
const char* progNameVersion = progName + progVersion
```
这个初始化操作是一定会失败的。编译器产生的伪码为：


```
String _temp;  
operator+(_temp, progName, progVersion);  
progNameVersion = _temp.String::operator char*();  
_temp.String::~String();
```
2、当一个临时对象被一个reference绑定时。例如：


```
const String& name = "C++";
```
编译器产生的伪码为：


```
String _temp;  
temp.String::String("C++");  
const String& name = _temp;
```
针对这种情况，C++标准上是这样说的：如果一个临时对象被绑定于一个reference，对象将保留，直到被初始化的reference的生命结束，或直到临时对象的生命范围结束—–看哪种情况先到达而定。
参考书籍：
1、《深度探索：C++对象模型》
2、《提高C++性能的编程技术》
3、《Effective C++》
4、《more effective C++》
5、《C++语言的设计和演化》
