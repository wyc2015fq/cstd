# C++中的函数对象（Function Object）（二） - BonChoix的专栏 - CSDN博客





2012年10月10日 04:27:59[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：7178








                
       除了自定义的函数对象，标准库还为我们提供了一系列现成的函数对象， 比如常见的数学、逻辑运算等。例如：negate<type>(),plus<type>(),minus<type>(),multiplies<type>(),divides<type>(),modulus<type>(),equal_to<type>,greater<type>(),less<type>(),logical_not<type>(),logical_and<type>(),等等。

       关于函数对象的最后一个很重要的概念是“函数适配器”。函数适配器，本质上讲也是一个函数对象。这个函数对象通过将一个或多个函数对象或者特定的数据按一定规则组合起来，以完成某些特定的功能。标准库为我们提供了几种函数适配器，例如：通过bind1st和bind2nd两个包装函数即可返回相应的适配器，这两个函数各有两个形参，分别为二元函数对象和一个数值，适配器自动把数值赋给函数对象的第1个(bind1st)或第2个(bind2nd)参数，并返回一个一元的函数对象。例如以下语句：
```cpp
find_if(vec.begin(),vec.end(),bind2nd(modulus<int>(),2));
```

 modulus<int>()初始化一个二元的函数对象，bind2nd函数把‘2’赋予其第二个参数，返回一个一元函数对象，即判断一个整数是否为偶数。这个语句的目的即找出容器vec中第一个偶数。

       注意：不要误把bind1st和bind2nd当成是函数适配器，它们仅仅是两个普通的包装函数模板，它们返回的才是真正的函数适配器。对应的函数对象类型分别为binder1st和binder2nd。这两个函数模板实现如下所示：
```cpp
template<typename Operation,typename T>
binder1st bind1st(const Operation &op, const T &t)
{
	return binder1st<Operation>(op,typename Operation::first_argument_type(t));
}
```

```cpp
template<typename Operation,typename T>
binder1st bind2nd(const Operation &op, const T &t)
{
	return binder2nd<Operation>(op,typename Operation::second_argument_type(t));
}
```
先不管具体细节，后面马上会说到。我们只关注函数的实现，很容易看出返回的分别是类binder1st<Operation>和binder2nd<Operation>的对象，即我们所说的函数适配器。

       除了这两个函数适配器，标准库中还定义了两个"取反器"，分别可以通过调用辅助函数not1和not2来获得，即unary_negate<type>和binary_negate<type>。此外，包装函数模板mem_func_ref返回的适配器用于自动调用对象的成员函数，具体细节及其他适配器可以到C++参考网站查看。

       函数适配器是C++中一个强有力的工具，它允许我们针对多个函数对象按照需要进行任意组合，来产生意义丰富的表达式。这种方法被称为"function composition"。比如对于函数f(x),g(x),h(x,y)，对他们进行组合，可以生成f(g(x)),g(f(x)),h(f(x),g(x))等复杂的函数。这里f,g分别对应了一元函数对象，h对应了二元函数对象，组合函数可以认为是函数适配器。

       不幸的是，标准库并没有给我们提供太多的适配器。因此，想要很好地利用它，还需要我们自己来定义。我们已经知道，自定义函数对象是件很容易的事情，但是想要让函数对象能在适配器中进行组合，则需要额外的一些工作。为了方便我们创建能在适配器中运用的函数对象，标准库定义了两个有用的结构，分别为：
```cpp
template <class Arg, class Result>
struct unary_function
{
	typedef Arg argument_type;
	typedef Result result_type;
};
```

```cpp
template <class Arg1, class Arg2, class Result>
struct binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};
```
这两个结构定义了几个类型成员，来表示函数对象参数和返回值的类型。其中unary_function代表一元函数对象，binary_function代表二元函数对象。通过让我们自定义的函数对象继承相应的结构，即可在函数适配器中使用。标准库的函数对象正是这样设计的，我们拿适配器binder1st作为例子来说明下，类binder1st如下：
```cpp
template<class Operation>
class binder1st: public unary_function<typename Operation::second_argument_type, typename Operation::return_type>
{
public:
	binder1st(const Operation &op, const typename Operation::first_argument_type &arg): operation(op),arg_1st(arg) {}
	typename Operation::return_type operator() (const typename Operation::second_argument_type &arg_2nd) const
	{
		return operation(arg_1st,arg_2nd);
	}
			
private:
	Operation operation;
	typename Operation::first_argument_type arg_1st;
};
```
适配器通过接受一个二元的函数对象(Operation)和一个该二元函数对象第一个参数类型的值(arg)调用构造函数。在其重载的"()"操作符函数中，只接受一个参数，对应其基类unary_function的argument_type,返回类型为unary_function的return_type。类定义中频繁用到了typename
 Operation::first_argument、typename Operation::second_argument_type及typename Operation::result_type，充分说明了对Operation的要求，只要Operation继承了binary_function即可。我们以一个计算pow(x,y)的函数对象为例子演示一下：
```cpp
template<typename T1, typename T2>
class Pow: public binary_function<T1,T2,T1>
{
public:
	T1 operator() (T1 base, T2 exp) const
	{
		return std::pow(base,exp);
	}
};
```
比如我们需要一个计算任意数字立方的函数对象，即可通过bind2nd(Pow<float,int>(),3)来得到。

       最后，我们通过自定义一个函数适配器来结束本文。要实现的功能为: h(f(x),g(x))。首先这是个一元的函数对象，只接受一个参数x，它组合了三个函数对象H,F,G。可以这样来实现：
```cpp
template<typename H,typename F,typename G>
class MyAdapter: unary_function<typename F::argument_type, typename H::return_type>
{
public:
	MyAdapter(const H &h, const F &f, const G &g):m_h(h),m_f(f),m_g(g) {}
	typename H::return_type operator() (const typename F::argument_type &x) const
	{
		return m_h(m_f(x),m_g(x));
	}
			
private:
	H m_h;
	F m_f;
	G m_g;
};
```



       （完）



