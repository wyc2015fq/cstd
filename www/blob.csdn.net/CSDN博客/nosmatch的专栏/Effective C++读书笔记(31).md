# Effective C++读书笔记(31) - nosmatch的专栏 - CSDN博客
2013年07月21日 11:19:04[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：583
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款31：将文件间的编译依赖关系降到最低
> 
在C++中我们要将接口与实现分离开来，这样只要接口没有改变，对接口应用的客户就可以保持不变，例如:
```cpp
class Person{
public:
	Person(const std::string& name, const Date& birthday,
		   const Address& addr);
	std::string name() const;
	std::string birthday() const;
	std::string address() const;
	...
private:
	std::string theName; //实现细节
	Date theBirthday;    //实现细节
	Address theAddress;  //实现细节
};
```
在上面的例子中，如果对类Person的实现细节做了修改，则在在编译的时候所有应用到接口的客户都要重新的编译，这
样效率是很低的，并且我们编译的时候通常上面都是包含#include "date.h"，#define "address.h"等这样无疑就在这些文件中形成了
很强的依赖关系，我们可以将Person的实现与接口分离开来，如下：
> 
```cpp
class PersonImpl；
class Date；
Class Address；
class Person{
public:
	Person(const std::string& name, const Date& birthday,
		   const Address& addr);
	std::string name() const;
	std::string birthday() const;
	std::string address() const;
	...
private:
	std::tr1::shared_ptr<PersonImpl> pImpl;
};
```
其中类PersonImpl是Person类的具体细节的实现类：
```cpp
class PersonImpl；
class Date；
Class Address；
class PersonImpl{
public:
	PersonImpl(const std::string& name, const Date& birthday,
		   const Address& addr);
	std::string name() const;
	std::string birthday() const;
	std::string address() const;
	...
private:
	std::string theName;
	Date theBirthday;
	Address theAddress;
};
```
其中
```cpp
Person::Person(const std::string& name, const Date& birthday, const Address& addr):pImpl(new PersonImpl(name,birthday,addr)){
}
std::string Person::name() const{
	pImpl->name();
	
}
```
对应的Person类的接口可以通过PersonImpl的接口来实现！在上面中我们将接口实现与接口的具体的细节分离成具体的
两个独立的类，在Person类中，对于Date/Address等类型我们不必在包含其类的具体实现，我们只需在前面做前置声明即可
这种情况适合于文件中对该类的应用是通过指针来进行的或者该类作为函数的形式参数！在程序中我们如果使用object reference
或者object pointers可以完成的任务，就不要使用object，这样我们就可以通过一个类的声明式就可以定义出指向该类型的reference
和points，但是如果定义某类型的object就需要用到该类型的定义式。还有如果能够尽量的用class声明式来替换class定义式！我们可
以将为声明式和定义式提供不同的头文件，例如#include "datefwd.h" 注意命名的方式。
> 
对于接口和实现的分离我们还可以用过abstract class的形式来实现：
```cpp
class Person{
public:
	virtual ~Person();
	virtual std::string name() const = 0;
	virtual std::string birthdayDate() const = 0;
	virtual std::string address() const = 0;
         ...
    static std::tr1::shared_ptr<Person> create(const std::string& name, const Date birthday,
    const Address& addr);
 };
```
上面的Person类只是个抽象类，我们应用的时候必须实现一个具体的类才可以，因为抽象的类是不能生成对象的：
> 
```cpp
class RealPerson : public Person{
public:
	RealPerson(const std::string& name, cosnt Date& birthday, 
			   const Address& addr):theName(name),theBirthdayDate(birthday),theAddress(addr)
	{}
	virtual ~RealPerson(){}
	std::string name() const;
	std::string birthday const;
	std::string address() const;
private:
	std::string theName;
	Date theBirthdayDate;
	Address theAddress;
};
```
这样就可以在Person类中添加一个生成对象的接口，由于Person是抽象类是不能生成对象，因此我们这个接口必须是
static的成员：
> 
```cpp
std::tr1::shared_ptr<Person> Person::create(const std::string& name, const Date& birthday, const Address& addr){
	return std::tr1::shared_ptr<Person>(new RealPerson(name,birthday,addr));
}
```
此外我们通过该Person抽象类我们还可以创建其他的对象！这样也可以实现类的接口与具体实现的分离！
请记住：
- 支持"编译依赖性最小"的一般构想是：相依于声明式，不要相依于定义式。
- 程序库头文件应该以"完全且仅有声明式"的形式存在，这种做法不论是否涉及template都适合
> 
