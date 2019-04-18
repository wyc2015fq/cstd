# [译]C++17,optional, any, 和 variant 的更多细节 - tkokof1的专栏 - CSDN博客

2018年09月12日 14:19:35[tkokof1](https://me.csdn.net/tkokof1)阅读数：511


> 
看到一个介绍 C++17 的系列博文([原文](http://www.modernescpp.com/index.php/c-17-constructed-in-place)),有十来篇的样子,觉得挺好,看看有时间能不能都简单翻译一下,这是第六篇~

std::optional, std::any, 和 std::variant 有一个共同特点:他们都支持就地构造.另外的,std::variant 还支持访问者模式.

首先,我们要了解一下这3种数据类型的功能作用.
- std::optional 是一种可能包含也可能不包含某一类型对象的类型.
- std::variant 是一种类型安全的联合体
- std::any 是一种可以包含任意类型(指可复制类型)对象的类型

我在之前的[文章](http://www.modernescpp.com/index.php/c-17-what-s-new-in-the-library)中讲解了这3个数据类型的一些细节,不了解的朋友可以先去看看,相关内容这里就不再赘述了.

![图](https://img-blog.csdn.net/201809121418338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### Construct in-place

什么是就地构造呢?以 std::optional<std::string> 为例来说明就是: **所谓就地构造,就是你可以直接使用 std::string 的构造参数来构造 std::optional<std::string>**.

下面是一个简短的示例.

```
#include <optional>
#include <iostream>
#include <string>

int main() 
{
	std::cout << std::endl;

	// C string literal
	std::optional<std::string> opt1(std::in_place, "C++17");                        // 1

	// 5 characters 'C'
	std::optional<std::string> opt2(std::in_place, 5, 'C');                          // 2

	// initializer list
	std::optional<std::string> opt3(std::in_place, { 'C', '+', '+', '1', '7' });      // 3

	// Copy constructor
	std::optional<std::string> opt4(opt3);                                          // 4

	std::cout << *opt1 << std::endl;
	std::cout << *opt2 << std::endl;
	std::cout << *opt3 << std::endl;
	std::cout << *opt4 << std::endl;

	std::cout << std::endl;
	
	return 0;
}
```

代码中的 opt1(第10行), op2(第13行) 和 op3(第16行) 都使用了 std::in_place 标记来进行构造,这意味着 std::optional 的构造参数将直接用于调用 std::string 的构造函数.所以在上述代码中, opt1 中 std::string 的构造函数参数即为 C 风格字符串(“C++17”), op2 中是5个单字符’C’, op3 中则是初始化列表({ ‘C’, ‘+’, ‘+’, ‘1’, ‘7’ }).另外,代码中的 opt4(第19行)并未使用就地构造方法,而是调用了 std::optional 的复制构造函数(复制了op3).

程序的输出如下:

![图](https://img-blog.csdn.net/20180912141848538?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上述的就地构造是不是觉得有些熟悉?其实早在 C++11 中,标准库容器就引入很多用于增加容器元素的接口方法,这些方法都以 emplace 开头,功能上就是提供了就地构造的方法.以 std::vector<int> vec 为例,借助其支持的 emplace_back 方法,我们可以直接调用 vec.emplace_back(5) 来增加 vec 的末尾元素,这等同于下面代码: vec.push_back(int(5)).

std::variant 还支持 std::visit 方法(即精典的设计模式:[访问者](https://en.wikipedia.org/wiki/Visitor_pattern)).

#### Visit a list of variants

std::visit 方法允许你对一个 std::variants 列表应用访问者模式,而相应的访问者必须是一个[callable](http://en.cppreference.com/w/cpp/concept/Callable)类型,所谓 callable 类型,是一种可以被调用的类型,通常是一个函数,一个函数对象或者一个 lambda 函数.简单起见,这里我仅使用 lambda 函数来举例说明.

```
#include <iostream>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <variant>

int main()
{
	std::cout << std::endl;

	std::vector<std::variant<char, long, float, int, double, long long>>      // 1
		vecVariant = { 5, '2', 5.4, 100ll, 2011l, 3.5f, 2017 };

	// display each value                                                             
	for (auto& v : vecVariant) {
		std::visit([](auto&& arg) {std::cout << arg << " "; }, v);                // 2
	}

	std::cout << std::endl;

	// display each type
	for (auto& v : vecVariant) {
		std::visit([](auto&& arg) {std::cout << typeid(arg).name() << " "; }, v); // 3
	}

	std::cout << std::endl;

	// get the sum
	std::common_type<char, long, float, int, double, long long>::type res{};  // 4

	std::cout << "typeid(res).name(): " << typeid(res).name() << std::endl;

	for (auto& v : vecVariant) {
		std::visit([&res](auto&& arg) {res += arg; }, v);                          // 5
	}
	std::cout << "res: " << res << std::endl;

	// double each value
	for (auto& v : vecVariant) {
		std::visit([](auto&& arg) {arg *= 2; }, v);                           // 6
		std::visit([](auto&& arg) {std::cout << arg << " "; }, v);
	}

	std::cout << std::endl;
	
	return 0;
}
```

代码中我创建了 std::variants 的列表(代码第11行).每个 variant 都可以包含以下的任一类型:char, long, float, int, double, long long.遍历 variant 列表并对每一个 variant 应用 lambda 函数非常简单(代码第15行到17行).借助 typeid 函数,我便可以获得 variant 的实际类型(代码第22行到24行).到这里,我想你应该已经看出了代码中的访问者模式, std::vector<std::variant> 就是我应用各种函数(即访问者)的被访问数据结构.

现在,我想将各个 variant 的元素求和.求和之前,我需要在编译期确定所求和的结果类型,为此我使用了 std::common_type (代码第29行), std::common_type 可以给出 char, long, float, int, double, 和 long long 都可以进行隐式转换的类型(double类型).代码中的 res{} 定义将 res(求和结果) 初始化为了 0.0,并在第33行到35行执行了真正的求和操作.我甚至使用访问者动态的修改了 variant 中的元素(代码第40行).

程序的输出如下.Visual C++ 中的运行时类型信息(std::type_info)给出了非常易读的类型名称.

![图](https://img-blog.csdn.net/20180912141905919?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

