# C++ 11 新特性之 template - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [usher2007](http://www.jobbole.com/members/usher2007) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
前 7 篇在这里：
- 《[C++11 新特性之新类型与初始化](http://blog.jobbole.com/102728/)》
- 《[C++11 新特性之类型推断与类型获取](http://blog.jobbole.com/104559/)》
- 《[C++11 新特性之 lambda](http://blog.jobbole.com/104548/)》
- 《[C++11 新特性之容器相关特性](http://blog.jobbole.com/104563/)》
- 《[C++11 新特性之智能指针](http://blog.jobbole.com/104569/)》
- 《[C++11 新特性之 Class](http://blog.jobbole.com/104665/)》
- 《[C++ 11 新特性之右值引用与移动](http://blog.jobbole.com/105461/)》
这是C++11新特性介绍的第八部分，涉及到template相关的新特性。
不想看toy code的读者可以直接拉到文章最后看这部分的总结。
# function
C++提供了很多种可调用对象，例如函数指针、lambda、重载了operator()的对象等。有时我们需要将这些对象统一管理，这时使用如下这种方式是不行的：


```
int add(int i, int j) { return i + j; }
struct divide
{
	int operator()(int i, int j)
	{
		return i / j;
	}
};
std::map<std::string, int(*)(int, int)>> binops = {
	{"+", add},
	{"-", std::minus<int>()},
	{"*", [](int i, int j) {return i * j; }},
	{"/", divide()},
};
```
无法直接将binops中的可调用对象直接转换成int(*)(int, int)。
C++11新标准中提供了一个名为function的标准库类，可以用function来表示以上所有的函数调用，只要这些函数调用的参数类型和返回类型是一致的。


```
std::cout<<"test function<T>:\n";
std::map<std::string, std::function<int(int, int)>> binops = {
	{"+", add},
	{"-", std::minus<int>()},
	{"*", [](int i, int j) {return i * j; }},
	{"/", divide()},
};
std::cout<<"+:\t"<<binops["+"](1, 2)<<"\n";
std::cout<<"-:\t"<<binops["-"](1, 2)<<"\n";
std::cout<<"*:\t"<<binops["*"](1, 2)<<"\n";
std::cout<<"/:\t"<<binops["/"](1, 2)<<"\n";
std::cout<<"test function<T> done.\n"<<std::endl;
```
# 模板友元
在新标准中，可以声明一个类的模板参数类型为类的友元。


```
template<typename T> class Bar
{
friend T;
protected:
	int val = 100;
};
class Foo
{
public:
	void print_bar(Bar<Foo> &bar) {std::cout<<"bar:\t"<<bar.val<<std::endl;}
};
std::cout<<"test friend template type:\n";
Bar<Foo> bar;
Foo foo;
foo.print_bar(bar);
std::cout<<"test friend template type done.\n"<<std::endl;
```
可以看到，由于友元机制，在Foo中可以直接访问到Bar的protected属性。
# 模板别名
新标准中，可以使用using为模板声明别名。


```
template<typename T> using twin = std::pair<T, T>;
template<typename T> using str_int = std::pair<T, int>;
std::cout<<"test template alias:\n";
twin<std::string> twin_str = {"abc", "def"};
std::cout<<"twin_str:\t"<<twin_str.first<<'\t'<<twin_str.second<<std::endl;
str_int<std::string> strno = {"abc", 100};
std::cout<<"strno:\t"<<strno.first<<'\t'<<strno.second<<std::endl;
std::cout<<"test template alias done.\n"<<std::endl;
```
可以看到，在声明str_int的时候，还可以用某一类型部分实例化模板。这是一个挺好用的特性。
# 默认参数
新标准中，可以像为函数提供默认参数一样，为模板参数提供默认值。


```
template<typename T, typename F=std::less<T>>
int compare(const T &v1, const T &v2, F f=F())
{
	if(f(v1, v2)) return -1;
	if(f(v2, v1)) return 1;
	return 0;
}
std::cout<<"test default template parameter:\n";
std::cout<<"compare int 1 2:\t"<<compare(1, 2)<<std::endl;
std::cout<<"compare int 2.0 1.0:\t"<<compare(2.0, 1.0)<<std::endl;
//std::cout<<"compare int 2.0 1:\t"<<compare(2.0, 1)<<std::endl; // wrong. can't determine which type is T
std::cout<<"test default template parameter done.\n"<<std::endl;
```
# 尾置返回类型
当时用模板定义一个函数时，有时函数的返回类型是和模板参数相关的，这时可以通过decltype获得返回类型。


```
template<typename It>
decltype(*beg) get_begin(It beg)
{
	return *beg;
}
```
但是这样写是无法通过编译的，因为decltype进行类型推断时，beg还不存在。所以这时需要使用尾置返回类型。


```
template<typename It>
auto get_begin(It beg) -> decltype(*beg)
{
	return *beg;
}
std::cout<<"test tail return type of template:\n";
std::vector<int> numbers = {1, 2, 3, 4, 5};
std::cout<<"get_begin:\t"<<get_begin(numbers.begin())<<std::endl;
std::cout<<"test tail return type of template done.\n";
```
# 不定长模板
新标准中，可以定义一个不定长度的模板参数列表。这种形式，一般和递归结合使用。


```
template<typename T>
std::ostream &print_variadic(std::ostream &os, const T &t)
{
	return os<<t<<std::endl;
}
template<typename T, typename... Args>
std::ostream &print_variadic(std::ostream &os, const T &t, const Args&... rest)
{
	os<<t<<"(remain size: "<<sizeof...(Args)<<"), ";
	return print_variadic(os, rest...);
}
std::cout<<"test variadic template:\n";
print_variadic(std::cout, 100, "s", 56.76, 101);
std::cout<<"test variadic template done.\n";
```
第一次为print_variadic传入了4个需要打印的对象，则实例化第二个不定长模板函数，将100赋值给t，并将剩余的3个参数打包成rest。
在内部递归中，将不断的将rest包中的第一个参数拿出来付给t，剩余参数打包进行下一次递归调用。
最后只剩一个参数时，两种形式的print_variadic都可以匹配，但是第一种没有模版参数包的版本更加特例化，因此将调用第一种形式的print_variadic，结束递归。
# 总结
- 提供了一个名为function的标准库类，可以用function来表示多个不同形式的函数调用，只要这些函数调用的参数类型和返回类型是一致的
- 在新标准中，可以声明一个类的模板参数类型为类的友元。
- 新标准中，可以使用using为模板声明别名。
- 新标准中，可以像为函数提供默认参数一样，为模板参数提供默认值。
- 部分模板函数返回值需要使用尾置返回类型。
- 新标准中，可以定义一个不定长度的模板参数列表，将参数打包。这种形式，一般和递归结合使用。
完整代码详见[template.cpp](https://github.com/yubo1911/saber/blob/master/learncpp11/template.cpp)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/08/a41e08fcaca939c581c931c115da0a8d.png)![](http://jbcdn2.b0.upaiyun.com/2016/08/c07df7ab8dad3a49f6ce602eb7d62949.jpg)
