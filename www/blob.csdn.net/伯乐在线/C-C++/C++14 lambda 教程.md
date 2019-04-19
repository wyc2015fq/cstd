# C++14 lambda 教程 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [sunbiaobiao](http://www.jobbole.com/members/sunbiaobiao) 翻译。未经许可，禁止转载！
英文出处：[solarianprogrammer](https://solarianprogrammer.com/2014/08/28/cpp-14-lambda-tutorial/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
C++ 14刚在上个月通过，C++14给C++11带来了一些令人期待的改变，[例如允许auto作为一个函数的返回类型](http://blog.jobbole.com/76744/)，还有泛型lambdas，这也是我们这篇文章的主题。
C++11 引入了 Lambdas，它意味着你可以把代码写的更短，匿名的函数，而不用一个函数，避免了创建一个单独的类和单独的函数定义，这里有一个典型的C++11中lambda的使用例子，返回一个数字的平方，


```
int result = [](int input) { return input * input; }(10);
cout << result << endl;
```
如果你想在很多地方使用这个代码片段，你可以把它保存到一个变量中


```
auto func = [](int input) { return input * input; };
// first use
std::cout << func(10) << std::endl;
// second use
std::cout << func(23) << std::endl;
```
你可以想到上面的代码会出什么问题了吗，它只是针对整数类型，如果你想把这个函数用在double或者复数类型的数字，像这样


```
// square of an int
std::cout << func(10) << std::endl;
// square of a double
std::cout << func(2.345) << std::endl;
// square of a complex number
std::cout << func(std::complex<double>(3, -2)) << std::endl;
```
这里明显只能使用模板


```
template <typename T>
T func(T z) {
    return z * z;
}
```
很显然，我们这里不是寻求这种解决方案，上面的这个函数不是匿名的，而是一个全局的函数，
C++14 标准中引入了泛型lambda的概念，允许我们使用auto来定义lambda的参数类型，我们可以写一个更短的更优雅的代码


```
auto func = [](auto input) { return input * input; };
```
下面是完整一点的代码


```
#include<iostream>
#include<complex>
int main() {
    // Store a generalized lambda, that squares a number, in a variable
    auto func = [](auto input) { return input * input; };
    // Usage examples:
    // square of an int
    std::cout << func(10) << std::endl;
    // square of a double
    std::cout << func(2.345) << std::endl;
    // square of a complex number
    std::cout << func(std::complex<double>(3, -2)) << std::endl;
    return 0;
}
```
上面的代码可以使用主流的C++编译器编译，像GCC4.9x, 或者Clang，下面是一个在OSX上使用GCC和Clang编译运行的结果


```
$ clang++ -std=c++1y -pedantic -Wall -stdlib=libc++ test_01.cpp -o test_01
$ ./test_01
100
5.49903
(5,-12)
$ g++-4.9.1 -std=c++14 -pedantic -Wall test_01.cpp -o test_01
$ ./test_01
100
5.49903
(5,-12)
$
```
然而，真正泛型lambda的发光点是和STL结合使用的时候，如果你想以递减的顺序对一个vector排序，使用泛型lambda，我们可以这样写：


```
std::sort(V.begin(), V.end(), [](auto i, auto j) { return (i > j); });
```
下面是完整的代码


```
#include<iostream>
#include<vector>
#include<numeric>
#include<algorithm>
int main() {
    std::vector<int> V(10);
    // Use std::iota to create a sequence of integers 0, 1, ...
    std::iota(V.begin(), V.end(), 1);
    // Print the unsorted data using std::for_each and a lambda
    std::cout << "Original data" << std::endl;
    std::for_each(V.begin(), V.end(), [](auto i) { std::cout << i << " "; });
    std::cout << std::endl;
    // Sort the data using std::sort and a lambda
    std::sort(V.begin(), V.end(), [](auto i, auto j) { return (i > j); });
    // Print the sorted data using std::for_each and a lambda
    std::cout << "Sorted data" << std::endl;
    std::for_each(V.begin(), V.end(), [](auto i) { std::cout << i << " "; });
    std::cout << std::endl;
    return 0;
}
```
下面是运行编译的结果


```
$ g++-4.9.1 -std=c++14 -pedantic -Wall test_02.cpp -o test_02
$ ./test_02
Original data
1 2 3 4 5 6 7 8 9 10
Sorted data
10 9 8 7 6 5 4 3 2 1
$
```
