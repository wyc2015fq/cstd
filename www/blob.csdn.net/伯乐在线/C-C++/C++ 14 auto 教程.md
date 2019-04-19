# C++ 14 auto 教程 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [sunbiaobiao](http://www.jobbole.com/members/sunbiaobiao) 翻译。未经许可，禁止转载！
英文出处：[Solarian Programmer](https://solarianprogrammer.com/2014/08/21/cpp-14-auto-tutorial/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
C++14标准最近刚被通过，像以前一样，没有给这个语言带来太大变化，C++14标准是想通过改进C++11 来让程序员更加轻松的编程，C++11引入auto关键字（严格来说auto从C++ 03 开始就有了，只是C++11改变了auto的含义），auto让你的代码更加干净，更容易让你避免错误，举个例子
原来你必须这样写


```
int i = 1;
    std::complex<double> c = return_a_complex_number();
```
你现在可以这样写


```
auto i = 1;
    auto c = return_a_complex_number();
```
声明为auto的变量在编译时期就分配了内存，而不是到了运行时期，所以使用auto不再引发任何速度延迟，这也意味着使用auto的时候，这个变量不初始化会报错。因为编译器无法知道这个变量的类型。


```
auto i;		// this will rise a compilation error
```
C++11中一个做的不好的地方是，你不能使用auto来定义一个函数类型，在新的标准中你就可以了：


```
// Error in C++11, works in C++14
    auto my_function() {
        ...
        return value;
    }
```
只要你的函数返回一个值，编译器就知道怎么解释这个auto关键词。现在，你可以使用最新版本的Clang和GCC，


```
g++-4.9.1 -Wall -std=c++14 -pedantic my_prog_name.cpp -o my_prog_name
```
为了更好地使用auto来简化你的代码，让我们分别使用C++98 、C++11 和C++14 来实现同一段代码，为了说明，我们使用两个函数来改变一个vector变量


```
multiply_by_two(add_one(my_vector));
```
很明显，这个循环给一个vector变量的每一个值加一再乘以二 你可以写一个函数，而不是两个。这里我们不是为了追求性能，而是说明auto的用法。
在C++98中你要这样写

C++
```
std::vector<int>& add_one(std::vector<int> &v) {
        for(std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
            *it += 1;
        }
        return v;
    }
    void multiply_by_two(std::vector<int> &v) {
        for(std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
            *it *= 2;
        }
    }
```
上面的代码显得很啰嗦
在C++11中你可以这样写


```
std::vector<int>& add_one(std::vector<int> &v) {
        for(auto& it : v) {
            it += 1;
        }
        return v;
    }
    void multiply_by_two(std::vector<int> &v) {
        for(auto& it : v) {
            it *= 2;
        }    
    }
```
C++11 中显然有了进步，我们这是可以使用auto来简化循环时候的一点代码。但仍显啰嗦。
在C++14中你可以使用auto来定义一个函数类型，代码可以简化为这样：


```
auto& add_one(std::vector<int>& v) {
        for(auto& it : v) {
            it += 1;
        }
        return v;
    }
    void multiply_by_two(std::vector<int>& v) {
        for(auto& it : v) {
            it *= 2;
        }    
    }
```
这里是完整代码


```
// C++14 "auto" demo code	
    #include <iostream>
    #include <vector>
    auto& add_one(std::vector<int>& v) {
        for(auto& it : v) {
            it += 1;
        }
        return v;
    }
    void multiply_by_two(std::vector<int>& v) {
        for(auto& it : v) {
            it *= 2;
        }    
    }
    void print_vec(const std::vector<int>& v) {
        for(const auto& e: v) {
            std::cout << e << std::endl;
        }   
        std::cout << std::endl;     
    }
    int main() {
        // Add one and multiply by two a vector of integers
        std::vector<int> my_vector{-1, 2, 3, 5};
        multiply_by_two(add_one(my_vector));    
        print_vec(my_vector);
        return 0;
    }
```
**你可以清晰的对比出，C++14比C++11 有了一点进步。**
