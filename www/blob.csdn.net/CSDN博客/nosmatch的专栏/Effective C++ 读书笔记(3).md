# Effective C++ 读书笔记(3) - nosmatch的专栏 - CSDN博客
2013年07月07日 23:05:26[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：491
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款三：尽量的使用const
> 
条款二对const关键字进行了简单的介绍，主要是介绍了const对一些预处理定义的替换使用，本条款主要
是讲const作为一个常量修饰的一个寓意的约束，总之就是一句话：在能使用const的地方尽量使用const！
> 
1.const对于基本内置类型的约束，主要指的是指针类型。
对于指针类型有下面两种约束的方式：
```cpp
const int * pi；
int * const pi；
```
很多人对这两种方式搞混，这里有一种简单的方法可以记住，遇到这样的const指针的时候，你把指针指向的
类型的关键词去掉，然后看const后面是什么就是修饰的什么，例如const int* pi；去掉int，就剩下const * int；因此
此时const是修饰*int，即指针指向的数是不能修改的，对应的可以看出int* const pi的意义，根据这种方式前者也可
以写作int const * pi；
> 
在STL中，迭代器也是一种指针，可能称之为智能指针，const可以对迭代器进行修饰，对应的也有两种修饰的方式：
```cpp
const std::vector<int>::iterator iter;
std::vector<int>::const_iterator iter;
```
其中后者是STL中定义的一种迭代器的方式，叫做常量迭代器，也可以又上面的那种方法看出这两种迭代器的不同！
此外在函数的应用中，也常用到const关键字，主要是两点，一是对于函数的形式参数的修饰上，如果参数在函数
内部不被修改那么一般情况下都要用const将参数修饰下，这样当函数接口暴露出去的时候，别人能很容的看清楚，还有一
种比较少见的用法就是对函数返回值得修饰上，这个主要是防止最函数返回值进行赋值操作，例如下面：
> 
```cpp
class Ration{};
const Ration operator*(const Ration& lhs，const Ration* rhs)；
```
> 
2.const成员函数
```cpp
class Ration{
private:
	int n_;
public:
	int getn(){
		return n;
	}
	int getn() const{
		return m;
	}
	void setn(int i) const{
		n = i;
	}
};
```
在C++中有函数重载的概念，对同名的函数，如果函数的参数类型或者个数不相同，就可以作为不同的函数，这个
主要是通过在编译源代码过程中对不同的函数重新命名来实现，对于const修饰的函数，不能对对象的任何成员进行修改，
并且const修饰的对象只能调用对象的const成员函数，其中const对象主要是作为函数的参数进行传递的！
> 
```cpp
class Ration{
private:
	int n_;
public:
	int getn(){
		return n;
	}
	int getn() const{
		return m;
	}
	void setn(int i) const{
		n = i;
	}
};
```
定义一个const Ration test，则test调用getn()函数的时候只能调用const的get函数，不能调用普通的成员函数，如果
没有const的成员函数，将报错，对应的例子中的setn函数定义为const函数，但是它却对成员n进行了赋值，因此是不允许的，
编译也不会通过，如果想让setn()函数编译通过，我们可以借助关键词mutable,将n定义为mutable int n，这样即使在const函
数中也可以对n进行修改！咱们一般的应用中很少直接定义一个const的对象，一般const的对象是用在函数的形式参数中出现的！
> 
说两点注意点：1.在前面中已经说明了const成员函数是不能够修改任何成员变量的，但是有时候确实可能出现隐式的修
改，如下：
> 
```cpp
class WHZString{
private: 
	char* ptest_;
public:
	.....
	
	void setchar(char p){
		ptest_[0] = p;
	}
}
```
这样就隐式的对类的成员做了修改！
2.当const成员函数与非const成员函数功能相同的时候，我们一般不会定义两个成员函数，其中一个只是比另一个多了一个
const的修饰，我们的解决办法是让非const的成员函数调用const的成员函数，其中const的成员函数正常定义，此时可能用到C++中
的强制类型转换例如static_cast/const_cast等！
