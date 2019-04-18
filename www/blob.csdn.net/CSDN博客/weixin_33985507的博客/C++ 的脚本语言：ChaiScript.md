# C++ 的脚本语言：ChaiScript - weixin_33985507的博客 - CSDN博客
2016年06月15日 20:37:42[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
[ChaiScript](https://github.com/ChaiScript/ChaiScript) 是一个可以方便的嵌在 C++ 程序里的脚本语言，相比于 V8（Google JavaScript）和 Lua 来说，它的用法要简单得多。
ChaiScript 和 STL 一样只有头文件，缺点是编译慢，而且因为大量使用模板，编译就更慢。
**说明**：
- 
本文示例代码一律假定已经 `using namespace chaiscript`。
- 
本文已经有些年头了，不代表 ChaiScript 最新特性。
## 函数
导出（expose）函数到 ChaiScript：
```
int echo (int i) {
  return i;
}
ChaiScript chai;
chai.add(fun(&echo), "echo");  // 导出到ChaiScript
int i = chai.eval<int>("echo(1)");  // 在ChaiScript里调用这个函数
```
导出重载函数时，需要指定具体类型：
```
string echo(const string& s) {
  return s;
}
ChaiScript chai;
chai.add(fun<int (int)>(&echo), "echo");
chai.add(fun<string (const string&)>(&echo), "echo");
int i = chai.eval<int>("echo(1)");
string s = chai.eval<string>("echo(\"string\")");
```
## 强类型
> 
ChaiScript is very strongly typed and does not do any int to unsigned int conversions automatically.
[http://chaiscript.com/node/126](http://chaiscript.com/node/126)
给定：
```
unsigned int fac(unsigned int n) {
  return n == 0 ? 1 : n * fac(n - 1);
}
chai.add(fun(&fac), "fac");
```
这样调用是不行的：
```
unsigned int fac = chai.eval<unsigned int>("fac(3)"); // 错！
```
会有 `bad_boxed_cast` 异常。应该在调用时显式的转成 `unsigned int`：
```
unsigned int fac = chai.eval<unsigned int>("fac(unsigned_int(3))");
```
或者，直接以 `signed int` 来导出这个函数：
```
chai.add(fun<int (int)>(&fac), "fac");
int fac = chai.eval<int>("fac(3)");
```
C++ 虽然也是强类型，但是允许 `signed` 和 `unsigned` 之间隐式转换。如果 `fac` 函数只有 `unsigned` 实现，`fac(3)` 也能调用，`3` 是 `signed`（`3u` 表示 `unsigned`），但是 C++ 可以将它隐式转换成 `unsigned`。ChaiScript 则不行，`signed` 就是 `signed`，`unsigned` 就是 `unsigned`，需要显式转换（`3u` 这种写法 ChaiScript 也不支持）。
对 ChaiScript 来说，`int` 和 `double` 都是 POD（plain object data）类型，`bool` 和 `std::string` 也是，完整的列表详见 ChaiScript bootstrap（自举，引导）的那段代码。
## 数组
ChaiScript 提供的 `Vector` 类似于 STL 的 `vector`：
```
var v := Vector()
v.push_back(1)
v.push_back(2)
print(v) // [1, 2]
```
`Vector` 的构造和初始化可以简化（类似于 Python 的列表）：
```
var v := [1, 2]
```
在 STL 的 `vector` 和 ChaiScript 的 `Vector` 之间，是不可以“直接”转换的：
```
vector<int> ints = chai.eval<vector<int> >("[1, 2, 3]"); // 错！
```
这样会有 `bad_boxed_cast` 异常。右边 `eval` 返回的是 `vector<Boxed_Value>`，不能自动转成 `vector<int>`。也可以简单理解成：自动 Box 和 Unbox 只对 POD 类型有效。这是一种设计上的折中吧，作者是这样解释的：
> 
There's no built in way to do conversions between typed vectors and vectors of Boxed_Value. We tried to implement that, but it introduced too much overhead in the code.
[http://chaiscript.com/node/118](http://chaiscript.com/node/118)
下面说说细节。首先是 `eval` 的结果可以引用：
```
vector<Boxed_Value>& ints = chai.eval<vector<Boxed_Value> >("[1, 2, 3]");
```
这就避免了一次拷贝构造。但是注意，`Boxed_Value` 里具体的数据是引用计数的：
```
class Boxed_Value {
private:
  boost::shared_ptr<Data> m_data;
};
```
所以 `vector` 本身引用与否，不影响内部的数据。
假如想在 C++ 这边改 ChaiScript 里的这个数组，也可以：
```
vector<Boxed_Value>& ints = chai.eval<vector<Boxed_Value> >("var a := [1, 2, 3]; a;"); // 声明变量a方便后续访问
ints[0].assign(Boxed_Value(4));
chai.eval("print(a[0])"); // 4
```
这里有两点值得注意。首先，变量 `ints` 不用引用也可以达到相同效果，因为如前所述，`Boxed_Value` 内部的数据有引用计数。其次，`assign` 不能替换成 `=`：
```
ints[0] = Boxed_Value(4);
```
用 `=`，之前的那个 `Boxed_Value` 就被替换掉了。
## 引用
下面这两种写法的差别在于，第一种多一次拷贝构造：
```
var a = Vector()
var a := Vector()
```
看两个例子。
例一：
```
var a := Vector()
var b = a // 拷贝构造了b
b.push_back(1) // b改了，a仍为[]
```
例二：
```
var a := Vector()
var b := a // b引用a
b.push_back(1) // a和b都改了
```
发现一个 bug，用快捷方式创建的 `Vector`，不能再被其他变量引用：
```
var a := [1, 2, 3] // 用快捷方式创建
var b := a
b.push_back(1) // Crash!
```
可见 ChaiScript 还有很多问题。不够成熟是我对它最大的顾虑。
## 类
```
class Buffer {
public:
  size_t LineCount() const { return lines_.size(); }
  string& GetLine(size_t line_index) {
    return lines_[line_index];
  }
  const string& GetLine(size_t line_index) const {
    return lines_[line_index];
  }
  void AddLine(const string& line) {
    lines_.push_back(line);
  }
private:
  vector<string> lines_;
};
```
```
chai.add(user_type<Buffer>(), "Buffer");
// Default constructor
chai.add(constructor<Buffer ()>(), "Buffer");
// Copy constructor
chai.add(constructor<Buffer (const Buffer &)>(), "Buffer");
chai.add(fun(&Buffer::AddLine), "AddLine");
chai.add(fun(&Buffer::LineCount), "LineCount");
```
## 成员变量
成员变量的导出方法和成员函数相同。
```
class Option {
public:
  std::string cjk;
  std::string file_encoding;
  bool show_space;
  bool show_number;
};
```
```
chai_->add(user_type<Option>(), "Option");
chai_->add(fun(&Option::cjk), "cjk");
chai_->add(fun(&Option::file_encoding), "file_encoding");
chai_->add(fun(&Option::show_number), "show_number");
chai_->add(fun(&Option::show_space), "show_space");
```
## 一些补充
### 传值，传引用
- 
Basic data types are passed by value.
- 
Class data types are passed by reference.
- 
There is not specific syntax for one or the other.
- 
Use wrapper classes for basic data types, if you want to pass them by reference (like Java.)
### 关于 `use`
如果导出一个变量，然后 `eval` 一个文件，这个文件又 `use` 了另一个文件，那么被 `use` 的那个文件是看不到这个变量的。
`use` 就相当于 C++ 的 `include`，被 `use` 的文件一般定义了一些公共的函数和类。
