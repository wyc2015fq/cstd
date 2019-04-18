# C++11 新特性之 Class - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [usher2007](http://www.jobbole.com/members/usher2007) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
前 5 篇在这里：
- 《[C++11 新特性之新类型与初始化](http://blog.jobbole.com/102728/)》
- 《[C++11 新特性之类型推断与类型获取](http://blog.jobbole.com/104559/)》
- 《C++11 新特性之 lambda》
- 《[C++11 新特性之容器相关特性](http://blog.jobbole.com/104563/)》
- 《[C++11 新特性之智能指针](http://blog.jobbole.com/104569/)》
这是C++11新特性介绍的第六部分，涉及到Class的相关内容。
不想看toy code的读者，可以直接拉到文章最后看这部分的总结。
## sizeof
新标准中，sizeof可以直接用于获取Class::member的大小，而不用通过Class的实例。

C++
```
class TestClass
{
public:
	int member_i;
	char member_c;
};
cout<<"test sizeof class member:\n";
cout<<sizeof(TestClass::member_i)<<"\t"<<sizeof(TestClass::member_c)<<endl;
```
## default constructor
新标准中，可以通过=default强制编译器生成一个默认constructor。

C++
```
class TestClass
{
public:
	TestClass() = default;
	TestClass(const int i, const char c): member_i(i), member_c(c) {}
	int member_i;
	char member_c;
};
cout<<"test =default class construct:\n";
TestClass tc; // may cause error if no default construct.
cout<<tc.member_i<<'\t'<<(short)tc.member_c<<endl;
cout<<"test =default done."<<endl;
```
在上面的代码中，如果我们不提供默认constructor的话，无法通过TestClass tc定义一个实例。
## delegate constructor
新标准中，可以在初始化列表中将一个constructor初始化的工作委托给另一个constructor。


```
class TestClass
{
public:
	TestClass() = default;
	TestClass(const int i, const char c): member_i(i), member_c(c) {}
	TestClass(const int i): TestClass(i, 0) { member_c = 'T';}
	int member_i;
	char member_c;
};
cout<<"test delegating constructor:\n";
TestClass tc2(2);
cout<<tc2.member_i<<'\t'<<tc2.member_c<<endl;
```
## allocator.construct
新标准中，allocator.construct可以使用任意的构造函数。

C++
```
class TestClass
{
public:
	TestClass() = default;
	TestClass(const int i, const char c): member_i(i), member_c(c) {}
	TestClass(const int i): TestClass(i, 0) { member_c = 'T';}
	int member_i;
	char member_c;
};
cout<<"test allocator:\n";
allocator<TestClass> alloc;
auto p = alloc.allocate(10);
alloc.construct(p, 10);
cout<<p->member_i<<'\t'<<p->member_c<<endl;
```
## copy constructor
新标准中，可以通过=default要求编译器合成默认的拷贝/赋值构造函数。


```
class TestClass
{
public:
	TestClass() = default;
	TestClass(const int i, const char c): member_i(i), member_c(c) {}
	TestClass(const int i): TestClass(i, 0) { member_c = 'T';}
	TestClass(const TestClass&) = default;
	TestClass& operator=(const TestClass&);
	int member_i;
	char member_c;
};
cout<<"test =default class copy construct:\n";
TestClass tc3(tc2);
TestClass tc4 = tc2;
cout<<tc3.member_i<<'\t'<<tc3.member_c<<endl;
cout<<tc4.member_i<<'\t'<<tc4.member_c<<endl;
```
同样，新标准中也允许用=delete禁止拷贝。


```
class TestClass
{
public:
	TestClass() = default;
	TestClass(const int i, const char c): member_i(i), member_c(c) {}
	TestClass(const int i): TestClass(i, 0) { member_c = 'T';}
	TestClass(const TestClass&) = delete;
	TestClass& operator=(const TestClass&);
	int member_i;
	char member_c;
};
TestClass& TestClass::operator=(const TestClass&) = default;
cout<<"test =delete class copy construct:\n";
//TestClass tc5(tc2); // error: use of deleted function ‘TestClass::TestClass(const TestClass&)’
//cout<<tc5.member_i<<'\t'<<tc5.member_c<<endl;
cout<<"test =delete done."<<endl;
```
## override和final
新标准中提供了override和final两个关键字，用于标识子类对父类中虚函数的重写（override）或禁止重写（final）。


```
class TestClass
{
public:
	TestClass() = default;
	TestClass(const int i, const char c): member_i(i), member_c(c) {}
	TestClass(const int i): TestClass(i, 0) { member_c = 'T';}
	TestClass(const TestClass&) = default;
	TestClass& operator=(const TestClass&);
	virtual void print_msg() {cout<<member_i<<'\t'<<member_c<<endl;}
	virtual void final_foo() final {}
	int member_i;
	char member_c;
};
TestClass& TestClass::operator=(const TestClass&) = default;
class SubTestClass final: public TestClass
{
	public:
		using TestClass::TestClass;
		SubTestClass(const int i): TestClass(i, 'S') {}
		void print_msg() override;
		//void print_msg(char c) override;
		//‘void SubTestClass::print_msg(char)’ marked override, but does not override
		
		//void final_foo() {}
		//overriding final function ‘virtual void TestClass::final_foo()’
};
//class SubSubTestClass: public SubTestClass {}; 
// cannot derive from ‘final’ base ‘SubTestClass’ in derived type ‘SubSubTestClass’
void SubTestClass::print_msg() 
{
	cout<<"i: "<<member_i<<'\t'<<"c: "<<member_c<<endl;
}
cout<<"test override:\n";
TestClass *stc_ptr = new SubTestClass(10); 
stc_ptr->print_msg();
SubTestClass stc(10);
TestClass tc6 = (TestClass)stc;
tc6.print_msg();
```
如果标识了override的函数实际上没有重写父类中的函数，或者标识final的函数被子类重写，编译器都会报错。
通样的，标识为final的类也不允许作为父类被继承。
## 委托父类构造函数
新标准中，也支持子类在初始化列表中直接委托父类的构造函数完成初始化。


```
class SubTestClass final: public TestClass
{
	public:
		using TestClass::TestClass;
		SubTestClass(const int i): TestClass(i, 'S') {}
		void print_msg() override;
};
cout<<"test inherit base class contructor:\n";
SubTestClass stc2(1024, 'H');
stc2.print_msg();
```
## 多继承与默认constructor
多重继承的子类可以直接继承父类的构造函数，但是如果父类中有形参列表完全相同的构造函数，则会产生冲突，这时需要子类自己定义一个自己版本的构造函数。


```
class TestClass2
{
public:
	TestClass2() = default;
	TestClass2(const int i) {}
};
class MultiSubClass: public TestClass, public TestClass2
{
public:
	using TestClass::TestClass;
	using TestClass2::TestClass2;
	// conflicts with version inherited from ‘TestClass’
	MultiSubClass(const int i): TestClass(i) {}
	MultiSubClass() = default;
};
cout<<"test multi inherit constructor:\n";
MultiSubClass mtc(1024);
mtc.print_msg();
return 0;
```
## 总结
- sizeof可以直接用于获取Class::member的大小，而不用通过Class的实例。
- 可以通过=default强制编译器生成一个默认constructor。
- 可以在初始化列表中将一个constructor初始化的工作委托给另一个constructor，以及父类的constructor。
- allocator.construct可以使用任意的构造函数。
- 可以通过=default要求编译器合成默认的拷贝/赋值构造函数，也可以通过=delete禁止拷贝。
- 新标准中提供了override和final两个关键字，用于标识子类对父类中虚函数的重写（override）或禁止重写（final），编译会对这两种情况进行检查。final还可以用于类的标识，表示禁止继承。
- 多重继承的子类可以直接继承父类的构造函数，但是如果父类中有形参列表完全相同的构造函数，则会产生冲突，这时需要子类自己定义一个自己版本的构造函数。
完整代码详见[class.cpp](https://github.com/yubo1911/saber/blob/master/learncpp11/class.cpp)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/08/a41e08fcaca939c581c931c115da0a8d.png)![](http://jbcdn2.b0.upaiyun.com/2016/08/c07df7ab8dad3a49f6ce602eb7d62949.jpg)
