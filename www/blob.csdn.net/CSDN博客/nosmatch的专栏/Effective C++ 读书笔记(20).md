# Effective C++ 读书笔记(20) - nosmatch的专栏 - CSDN博客
2013年07月13日 16:57:51[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：493
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款20：宁以pass-by-reference-to-const替换pass-by-value
> 
考虑下面的例子：
```cpp
class People{
public:
	Person();
	virtual ~People();
	...
private:
	std::string name;
	std::string address;
};
class Student:public People{
public:
	Studenet();
	~Student();
	...
private:
	std::string schoolName;
	std::string shoolAddress;
};
```
考虑下面的调用：
> 
```cpp
bool validateStudent(Student s);
Student plato;
bool platoIsOk = validateStudent(plato);
```
> 
在函数validateStudent函数调用的时候会发生什么事情？首先会调用Student的copy构造函数从plato copy构造对象s，在
函数validateStudent函数执行结束后，调用对象s的析构函数，这样总共调用了一次copy 构造函数和一次析构函数，但是在Student
类内部，有两个string成员对象，因此在Student构造函数的时候也会调用string类的两个构造函数，相应结束的时候也会调用string
的析构函数，又因为Student类继承子People，因此Student类的构造会引起People类对象的构造，对应的People对象两个string对象
的构造和对应的析构函数，这样一个函数的执行就涉及到了六次的构造函数和六次的析构函数，虽然调用时正确的，但是调用的效
率是太低了，我们可以通过pass-by-reference-to-const来调用，
> 
```cpp
bool validateStudent(const Student& s);
```
通过引用的调用可以防止对应的构造函数与析构函数的调用，const关键字主要是用来说明实参的对象是不能改变的，在
pass-by-value的调用方式中，实参的对象也是不能改变的，改变的是复制的临时对象！在函数返回的对象也是有相似的情况！
> 
在pass-by-value还可能造成一种对象切割的情况，例如：
```cpp
class Window{
public:
	...
	std::string name() const;
	virtual void display() const;
};
class WindowWithScrollBars:public Window{
public:
	...
	virtual void display() const;
	....
};
void printNameAndDisplay(Window w){
	std::cout << w.name()<<endl;
	w.display();
}
WindowWithScrollBars wwsb;
printNameAndDisplay(wwsb);
```
在上面的这种情况下，当wwsb传递给函数的参数w时，发生的是pass-by-value,此时调用的Window的copy构造函数，因
此，copy 构造函数构造的对象是Window类型的，不管实参类型是Window类型还是Window的子类型，这种情况叫对象的切割，对应
的如果我们将printNameAndDisply函数的参数修改为const Window& w,此时我们就可以在函数内部调用到子类的函数，这就是多态的
应用！需要注意！
> 
在C++中并不是所有的对象都是要以pass-by-reference-to-const的形式，对于内置的类型和stl中的迭代器类型和函数对象
还是要采用pass-by-value的形式！
请注意：
- 尽量以pass-by-reference-to-const来替换pass-by-value,前者不仅高效还能防止对象的切割问题
- 对于内置类型和STL中的迭代器和函数对象还是要采用pass-by-value的形式比较高效
> 
