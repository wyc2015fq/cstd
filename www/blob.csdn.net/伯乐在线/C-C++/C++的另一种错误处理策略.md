# C++的另一种错误处理策略 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [周昌鸿](http://www.jobbole.com/members/zzzworm) 翻译。未经许可，禁止转载！
英文出处：[Zenol](http://zenol.fr/site/2013/08/27/an-alternative-error-handling-strategy-for-cpp/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
这篇短文是讨论一个大多数程序员都感兴趣的一个话题：错误处理。错误处理是编程的一个“黑暗面”。它既是应用程序的“现实世界”的关键点，也是一个你想隐藏的复杂业务。
在早期的C编程生涯中，我知道三种错误处理的方式。
## C语言的方式：返回错误码
C语言风格的错误处理是最简单的，但是并不完美。
C语言风格的错误处理依赖于“当程序遇到错误时返回一个错误码”。这里是一个简单的例子：

C++
```
int find_slash(const char *str)
{
	int i = 0;
 
	while (str[i] && str[i] != '/')
		  i++;
 
	if (str[i] == '\0')
		return -1; //Error code
 
	//True value
	return i;
}
 
// . . .
 
if (find_slash(string) == -1)
{
		//error handling
}
```
## 使用这种方式的有什么好处？
你可以在调用函数之后直接处理错误码（在C语言中，你也会这样处理），显示一个错误消息或者直接终止程序。或者仅仅恢复程序最近的一个状态，终止计算。
当你找不到错误处理在哪里的时候,你只需要后头看看函数调用，错误处理就在那个附近。
## 使用这种方式有什么不好？
有人可能会告诉你，这种异常/错误处理方式和“执行逻辑”混在了一起。当你顺序地阅读这些代码的时候就行程序执行一样，你看到了一会错误处理，一会程序执行。这样很糟糕，你可能更喜欢只读程序执行逻辑或者错误处理逻辑。
并且你被限定使用错误码，如果你想要提供更多的信息，你需要创建一些功能函数比如：errstr或者提供全局变量。
## 使用C++的方式
C++作为对C的增强，引入了一种新的错误处理方式——异常。异常通过抛出一个错误的方式来中断正常代码执行逻辑，并可以被其他地方所捕获。下面是一个简单的例子：

C++
```
int find_slash(const char *str)
{
	int i = 0;
 
	while (str[i] && str[i] != '/')
		  i++;
 
	if (str[i] == '\0')
		throw AnException("Error message");
 
	//True value
	return i;
}
 
// . . .
 
try
{
	find_slash(string);
}
catch(AnException& e)
{
   //Handle exception
}
```
## 这样做的好处？
程序逻辑和错误处理分离了。一边你可以看到函数是如何工作的，而另一边你可以看到函数失败时候是怎么处理的。这样做很完美，可以很容易看出错误处理和正常程序逻辑。
另外，现在你可以为你的错误提供你需要的尽可能多的信息，因为你可以将需要的内容填充在自定义异常对象里。
## 这样做的坏处
编写详尽的异常处理变得很冗。你需要一个异常树，但是最好不要太大,这样,你可以选择捕获感兴趣的异常。同时,内部需要提供错误码,来获知究竟发生了什么,同时需要检索一些错误消息,等等。编写写异常类通常都是冗长,这是将信息嵌入到错误里来灵活处理更多的信息的成本。
这里的错误处理哲学是将错误尽可能推迟到需要处理的地方再处理，当你不知道程序执行过程究竟哪里会产生一个错误，你需要跳过不同的文件和功能函数来查找，这通常都是困难的，如果你在一个很深的调用树（这里意思是当你将函数调用绘制出一个图形，其形状类似一棵树）上引发了一个异常，你需要指定在哪里来处理这个异常，当它被处理的时候，它又是在哪里发生的。特别是当你的程序很大，又是很早之前编写，有恰巧设计不够良好的时候，就更加显得困难。而大多数商业项目都是这样。
所以我觉得“异常是危险的”。虽然它提供了一种良好的方式来处理错误——仅限于一些小项目，并且这里的调用图简单且易于掌握时候。
## 错误封装的模式
我这里把它叫做一种模式，所以人们不必害怕担心。后面，我会给它一种更好的命名，所以请不要着急。
错误封装的主旨是创建一种封装来包含错误消息或者错误的返回值。我们通常会选择字符串而不是其他，因为这也并不容易实现。我们尽力保证语法的可读性，可理解，并且容易应用。我们不处理拷贝构造或者多参数函数及返回值，这里仅给出一个尽可能简单的例子。
让我们以下面的例子开始：

C++
```
E<int> find_slash(const char* str)
{
	int i = 0;
 
	while (str[i] && str[i] != '/')
		  i++;
 
	if (str[i] == '\0')
		return fail<int>("Error message");
 
	//True value
	return ret(i);
}
 
// . . .
 
auto v = find_slash(string);
if(!v)
{
	//Handle exception
}
```
乍一看，这里有点类似C语言的风格，但是不是，为表明这一点，请看接下来的多个函数调用例子：

C++
```
E<int> find_slash(const char*);
E<int> do_some_arithmetic(int);
E<std::string> format(int);
E<void> display(std::string);
 
auto v = ret(string)
		 .bind(find_slash)
		 .bind(do_some_arithmetic)
		 .bind(format)
		 .bind(display);
 
if(!v)
{
	//Handle error
}
```
好了，这里发生了什么？bind是一个成员函数来绑定你的函数调用，试着去应用它。如果错误装箱里面含有一个值，那么它就应用于函数调用，继续返回一个错误装箱（编译器不允许你返回一个不带错误装箱的函数）。
所以，我们链式调用了find_slashe，do_some_arithmetic, format和display.它们都不处理错误装箱，由于bind函数的作用，我们将函数`E<something_out> f(something_in)``返回结果给E<something_out> f(E<something_in>)`函数做参数。
## 这里的好处是什么？
再一次，函数逻辑（调用链）和错误处理分离了。和异常一样，我们可以简单读一下函数调用链来了解代码逻辑，而不用关心执行是在哪里被中断的。事实上，函数调用链可以在任何调用时被中断。但是我们可以认为没有错误发生，如果我们的逻辑是正确的，可以很快速检查。
当然，类型推导会阻止你在调用display之后继续进行绑定。所以我们也没有失去类型能力。
注意，我们没有在其他地方调用这些函数，我们在最后将这些方法组装在一起。这里是关键，你应该编写一些小的模块函数（另外，注意：你应该编写模板函数使其工作）接收一个值，然后计算一个新值或者返回失败。在每一步中，你都不需要考虑可能出现错误导致你的控制流中断，并且校验你是否在一个有效的状态上（异常安全基于查询每个函数调用，指出函数是否中断你的控制流程，如果出现异常会发生什么），基于这一点，这样做更安全。
和异常一样，我们可以处理很详细的信息，尽管这里我们编写的是一个偏模板函数，所以也容易理解一些。
我们可以很容易放置异常处理逻辑，把它放在函数调用链之后（除非这个返回值还需要进一步被链接）。现在，我们有一个大的的执行流，没有中断，使用小的函数处理流程，容易定位。当需要添加一个新的错误时，你只需找到那些函数，通过函数调用链，你可以直接定位到处理位置，并根据需要添加。大型项目变得更加的线性化，并且更易读。
## 这样做有什么不足？
首先，这是一个新的处理方式，并且和C++的方式不兼容。这不是一个标准处理方法，当你使用stl时，你仍然需要使用异常。
对于我来说，这样做还是有点冗长。需要显式编写fail<int>（…）的模板推导显得有点怪异，如果你有个多态错误类型就更糟了，你不得不这样写`fail<return_type, error_type>("...")`.
当函数有多个参数时编写也很困难，在其他一些语言中，可以使用适用类型和抽象类型很好地解决这个问题，不过这在C++中不会提供。我想更适合使用`bind2(E<a>, E<b>, f)`和`bind3(E<a>, E<b>, E<c>, f)``，可变模板参数功能更有用。`
`为获取封装错误中的值，我们需要检查这个值是否是有效值，接着调用一个“to_value”方法。我们没办法不通过检查来做到这一点。我们希望的是“解构”一个对象，不过这在C++中不支持，这也不是一些可以说“我们把它加入到下一个标准”的特性。`
`目前为止，我不知道读者是否有方法将其适配到成员函数中，如果你有想法，请测试一下，如果可以，请告知我们。`
## 实现原子错误处理
`我实现了它，我定义了这个黑魔法的名字——“原子化”，你可以认为“原子化”是一个对值和错误上下文的装箱，比如，一个box包含一个值或者什么也不包含是一个原子组（这里作为一个练习，你可以试着实现一下）。`
`有点奇怪的是，从某个角度来说队列是一个原子组，他们拥有一个上下文的值。`
`让我们从上面的E模版类实现开始，这里使用了C++11标准中的decltype和auto -> decltype类型，允许自动推导得到表达式的类型，这非常有用。`
`这里的bind函数有点怪异，但是他实现了我刚才提到的内容。`

C++
```
/*
This is the "Either String" monad, as a way to handle errors.
*/
 
template
<typename T>
class E
{
private:
	//The value stored
	T m_value;
	//The error message stored
	std::string m_error;
	//A state. True it's a value, false it's the message.
	bool m_valid;
 
	E()
	{}
 
public:
	//Encapsulate the value
	static E ret(T v)
	{
		E box;
		box.m_value = v;
		box.m_valid = true;
		return box;
	}
 
	//Encapsulate an error
	static E fail(std::string str)
	{
		E box;
		box.m_error = str;
		box.m_valid = false;
		return box;
	}
 
	//True if it's a valid value
	operator bool() const
	{
		return m_valid;
	}
 
	//Deconstruct an E to a value
	T to_value() const
	{
		//It's a programmer error, it shouldn't happen.
		if (!*this)
		{
			std::cerr << "You can't deconstruct to a value from an error" << std::endl;
			std::terminate();
		}
		return m_value;
	}
 
	//Deconstruct an E to an error
	std::string to_error() const
	{
		//It's a programmer error, it shouldn't happen.
		{
			std::cerr << "You can't deconstruct to an error from a value" << std::endl;
			std::terminate();
		}
		return m_error;
	}
 
	friend std::ostream& operator<< (std::ostream& oss, const E<T>& box)
	{
		if (box)
			oss << box.m_value;
		else
			oss << box.m_error;
		return oss;
	}
 
	template<typename F>
	inline
	auto bind(F f) -> decltype(f(m_value))
	{
		using type = decltype(f(m_value));
		if (*this)
			return f(m_value);
		else
			return type::fail(m_error);
	}
};
```
``这里，我重载了<<运算符，所以导出装箱中的内容更容易一些。我们并不是一定需要它，在“真”值时去掉这一点也更好一些。
这里的例子，我们需要一个“E<void>”类型，但是它可能不一定使用。我们需要为void实现一个特别的重载，这里其实也是一样的，只不过期望的值是一个“空箱”。

C++
```
/*
	Special instance for void
*/
 
template<>
class E<void>
{
private:
	std::string m_error;
	bool m_valid;
 
	E()
	{}
 
public:
	//Encapsulate the value
	static E ret()
	{
		E box;
		box.m_valid = true;
		return box;
	}
 
	//Encapsulate an error
	static E fail(std::string str)
	{
		E box;
		box.m_error = str;
		box.m_valid = false;
		return box;
	}
 
	//True if it's a valid value
	operator bool() const
	{
		return m_valid;
	}
 
	//Déconstruct an E to a value
	void to_value() const
	{
		//It's a programmer error, it shouldn't happen.
		if (!*this)
		{
			std::cerr << "You can't deconstruct to a value from an error" << std::endl;
			std::terminate();
		}
	}
 
	//Deconstruct an E to an error
	std::string to_error() const
	{
		//It's a programmer error, it shouldn't happen.
		if (*this)
		{
			std::cerr << "You can't deconstruct to an error from a value" << std::endl;
			std::terminate();
		}
		return m_error;
	}
 
	friend std::ostream& operator<< (std::ostream& oss, const E<void>& box)
	{
		if (box)
			oss << "()";
		else
			oss << box.m_error;
		return oss;
	}
 
	template<typename F>
	inline
	auto bind(F f) -> decltype(f())
	{
		using type = decltype(f());
		if (*this)
			return f();
		else
		return type::fail(m_error);
	}
};
```
我们没有提到ret和fail方法，事实上，它们只是对xxx::fail和xxx::ret函数的封装。

C++
```
/*
   Then, I introduced those simple functions, to reduce the
   call to something readable/writable
 */
template <typename T>
inline
E<T> ret(T v)
{
	return E<T>::ret(v);
}
 
template <typename T>
inline
E<T> fail(std::string err)
{
	return E<T>::fail(err);
}
```
这里，你可以编译并执行一下上面的代码。
如果你想要更多的，可以试试下面这个更具体一点的例子：

C++
```
/*
	Here come a case of use.
*/
 
// What a user would see:
 
//Return a value in an error context
template <typename T> inline
E<T> ret(T v);
//Fail in an error context of type T
template <typename T> inline
E<T> fail(std::string err);
 
// What a user would write:
 
typedef std::vector<std::string> vs;
typedef std::string str;
 
//Parse a +- formated string.
//If a letter is prefixed by +, then the function toupper is applied.
//''											  tolower is applied.
//Non alphabetical (+ and - excepted) aren't alowed.
//Words are cut on each space ' '. Other blank characters aren't alowed.
E<std::vector<std::string>> parse(std::string str)
{
	int mode = 0;
	vs vec;
 
	if (str.empty())
		return fail<vs>("Empty string aren't allowed");
 
	std::string stack;
	for(int i = 0; str[i] != '\0'; i++)
	{
		switch(str[i])
		{
		case '-':
			mode = 1;
			break;
		case '+':
			mode = 2;
			break;
		case ' ':
		{
			if(!stack.empty())
				vec.push_back(stack);
			stack.resize(0);
			mode = 0;
			break;
		}
		default:
		{
			if (!isalpha(str[i]))
				return fail<vs>("Only alpha characters are allowed");
			if (mode == 1)
				stack.push_back(tolower(str[i]));
			else if (mode == 2)
				stack.push_back(toupper(str[i]));
			else
				stack.push_back(str[i]);
			mode = 0;
			break;
		}
		}
	}
	if(!stack.empty())
		vec.push_back(stack);
 
	return ret(vec);
}
 
//Take the first word and append it to the begining of all other words.
//Vec should contain at least one element.
E<std::vector<std::string>> prefixy(std::vector<std::string> vec)
{
	if (vec.empty())
		return fail<vs>("Can't add prefixes on an empty table");
 
	std::string prefix = vec.front();
	vs out;
 
	for (auto s : vec)
	{
		if (prefix == s)
			continue;
		out.push_back(prefix + s + "^");
	}
 
	return ret(out);
}
 
//Concatenate all strings as a big string. Vec should contain data.
E<std::string> concat(std::vector<std::string> vec)
{
	std::string output;
 
	if (vec.empty())
		return fail<str>("Empty vectors aren't allowed");
 
	for (auto s : vec)
		output += s;
 
	if (output.empty())
		return fail<str>("No data found");
	return ret(output);
}
 
int main()
{
	typedef std::string s;
 
	//Parse some string, show how error interrupt computation of the "chain".
	std::cout << ret((s)"+hello	-WORLD").bind(parse).bind(prefixy).bind(concat) << std::endl;
	std::cout << ret((s)"+hello Hello  Hello").bind(parse).bind(prefixy).bind(concat) << std::endl;
	std::cout << ret((s)"+   ").bind(parse).bind(prefixy).bind(concat) << std::endl;
	std::cout << ret((s)"+hi").bind(parse).bind(prefixy).bind(concat) << std::endl;
 
	//Play with lambda to "replace" a value if it's not an error.
	std::cout << ret((s)"Some string").bind([](const std::string&) {return fail<s>("Failed");});
	std::cout << ret(23).bind([](const int) {return ret(42);});
	std::cout << fail<int>("NaN").bind([](const int) {return ret(42);});
 
	return 0;
}
```
