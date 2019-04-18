# 【C++深入探索】Copy-and-swap idiom详解和实现安全自我赋值 - 江南烟雨 - CSDN博客
2012年08月30日 21:40:30[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：6286
任何管理某资源的类比如智能指针需要遵循一个规则（The Rule of Three）：
如果你需要显式地声明一下三者中的一个：析构函数、拷贝构造函数或者是拷贝赋值操作符，那么你需要显式的声明所有这三者。
拷贝构造函数和析构函数实现起来比较容易，但是拷贝赋值操作符要复杂许多。
它是怎么实现的？我们需要避免那些误区？
那么Copy-and-swap就是完美的解决方案。而且可以很好地帮助拷贝赋值操作符达到两个目标：避免代码重复、提供强烈的异常安全保证。
**1、  怎么工作**
概念上讲，它是利用拷贝构造函数生成一个临时拷贝，然后使用swap函数将此拷贝对象与旧数据交换。然后临时对象被析构，旧数据消失。我们就拥有了新数据的拷贝。
为了使用copy-and-swap，我们需要拷贝构造函数、析构函数以及swap交换函数。
一个交换函数是一个non-throwing函数，用来交换某个类的两个对象，按成员交换。我们可能会试着使用std:swap，但是这不可行。因为std:swap使用自己的拷贝构造函数和拷贝赋值操作符。而我们的目的是定义自己的拷贝赋值操作符。
**2、  目的**
让我们看一个具体的实例。我们需要在一个类中管理一个动态数组。我们需要实现构造函数、拷贝赋值操作符、析构函数。
```cpp
#include <algorithm> // std::copy
#include <cstddef> // std::size_t
class dumb_array
{
public:
	// (default) constructor
	dumb_array(std::size_t size = 0) :
	  mSize(size),
		  mArray(mSize ? new int[mSize]() : 0)
	  {}
	  // copy-constructor
	  dumb_array(const dumb_array& other) :
	  mSize(other.mSize),
		  mArray(mSize ? new int[mSize] : 0),
	  {
		  // note that this is non-throwing, because of the data
		  // types being used; more attention to detail with regards
		  // to exceptions must be given in a more general case, however
		  std::copy(other.mArray, other.mArray + mSize, mArray);
	  }
	  // destructor
	  ~dumb_array()
	  {
		  delete [] mArray;
	  }
private:
	std::size_t mSize;
	int* mArray;
};
```
这个类几乎可以说是成功的实现了管理动态类的功能，但是还需要opeator=才能正常工作。
下面是一个不怎么好的实现：
```cpp
// the hard part
dumb_array& operator=(const dumb_array& other)
{
    if (this != &other) // (1)
    {
        // get rid of the old data...
        delete [] mArray; // (2)
        mArray = 0; // (2) *(see footnote for rationale)
        // ...and put in the new
        mSize = other.mSize; // (3)
        mArray = mSize ? new int[mSize] : 0; // (3)
        std::copy(other.mArray, other.mArray + mSize, mArray); // (3)
    }
    return *this;
}
```
上述代码有三个问题，分别是括号所注明的。
**（1）需要进行自我赋值判别。**
这个判别有两个目的：是一个阻止冗余代码的一个简单的方法；可以防止出现bug（删除数组接着又进行复制操作）。在其他时候不会有什么问题，只是使得程序变慢了。自我赋值在程序中比较少见，所以大部分情况下这个判别是多余的。这样，如果没有这个判别也能够正常工作就更好了。
**（2）只提供了基本异常安全保证。**
如果new int[mSize]失败，那么*this就被修改了（数组大小是错误的，数组也丢失了）。为了提供强烈保证，需要这样做：
```cpp
dumb_array& operator=(const dumb_array& other)
{
    if (this != &pOther) // (1)
    {
        // get the new data ready before we replace the old
        std::size_t newSize = other.mSize;
        int* newArray = newSize ? new int[newSize]() : 0; // (3)
        std::copy(other.mArray, other.mArray + newSize, newArray); // (3)
        // replace the old data (all are non-throwing)
        delete [] mArray;
        mSize = newSize;
        mArray = newArray;
    }
    return *this;
}
```
代码膨胀了！这就导致了另外一个问题：
**（3）代码冗余。**
核心代码只有两行即分配空间和拷贝。如果要实现比较复杂的资源管理，那么代码的膨胀将会导致非常严重的问题。
**3、一个成功的解决方案**
就像前面所提到的，copy-and-swap可以解决所有这些问题。但是现在，我们还需要完成另外一件事：swap函数。规则“The rule of three”指明了拷贝构造函数、赋值操作符以及析构函数的存在。其实它应该被称作是“The Big And Half”：任何时候你的类要管理一个资源，提供swap函数是有必要的。
我们需要向我们的类添加swap函数，看以下代码：
```cpp
class dumb_array
{
public:
    // ...
    friend void swap(dumb_array& first, dumb_array& second) // nothrow
    {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap; 
        // by swapping the members of two classes,
        // the two classes are effectively swapped
        swap(first.mSize, second.mSize); 
        swap(first.mArray, second.mArray);
    }
    // ...
};
```
现在我们不仅可以交换dumb_array，而且交换是很有效率的进行：它只是交换指针和数组大小，而不是重新分配空间和拷贝整个数组。
这样，我们可以如下实现拷贝赋值操作符：
```cpp
dumb_array& operator=(dumb_array other) // (1)
{
    swap(*this, other); // (2)
    return *this;
}
```
就是这样！以上提到的三个问题全部获得解决。
**4、为什么可以正常工作**
我们注意到一个很重要的细节：参数是按值传递的。
某些人可能会轻易地这样做（实际上，很多失败的实现都是这么做的）：
```cpp
dumb_array& operator=(const dumb_array& other)
{
    dumb_array temp(other);
    swap(*this, temp);
    return *this;
}
```
这样做我们会失去一个重要的优化机会（参考[Want
 Speed? Pass by Value](http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/)）。而在C++11中，它备受争议。
**通常，我们最好遵循比较有用的规则是：不要拷贝函数参数。你应该按值传递参数，让编译器来完成拷贝工作。**
这种管理资源的方式解决了代码冗余的问题，我们可以用拷贝构造函数完成拷贝功能，而不用按位拷贝。拷贝功能完成后，我们就可以准备交换了。
注意到，上面一旦进入函数体，所有新数据都已经被分配、拷贝，可以使用了。这就提供了强烈的异常安全保证：如果拷贝失败，我们不会进入到函数体内，那么this指针所指向的内容也不会被改变。（在前面我们为了实施强烈保证所做的事情，现在编译器为我们做了）。
swap函数时non-throwing的。我们把旧数据和新数据交换，安全地改变我们的状态，旧数据被放进了临时对象里。这样当函数退出时候，旧数据被自动释放。
因为copy-and-swap没有代码冗余，我们不会在这个而操作符里面引入bug。我们也避免了自我赋值检测。
参考资料：
[http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom](http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)
