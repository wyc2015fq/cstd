# Effective C++ 读书笔记(12) - nosmatch的专栏 - CSDN博客
2013年07月10日 22:14:43[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：397
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款12：复制对象时勿忘其每一个成分
> 
C++中类会将成员都封装起来，类的copy操作只能通过赋值函数和复制构造函数，在进行对应的copy操作的时候注意要将成员
的每一个成员进行对应的复制！例如:
> 
```cpp
class People{
private:
	string name;
public:
	People(const People& rhs):name(rhs.name){
	}
	People& operator=(const People& rhs){
		name = rhs.name;
		return *this;
	}
};
```
在上面的例子中只有一个成员没有任何的问题，但是当加入另一个成员变量的时候也要注意对应的修改连个copy函数！
否则虽然编译器不会警告任何错误，在后面可能去出现意想不到的问题！
> 
在继承体系中，主要子类的copy函数要主动的去调用父类的相关copy函数，例如：
```cpp
class Student:public People{
private:
	string school;
public:
	Student(const Student& rhs):school(rhs.school){}
	Student& operator=(const Student& rhs){
		school = rhs.school;
		return *this;
	}
};
```
在上面的例子中，复制了子类中的所有成员，但是相关父类中的name成员变量缺没有复制，此时name变量不能被复制为
rhs的name值，取代的是一个默认的值，正确的方式如下：
> 
```cpp
Student::Student(const Student& rhs):People(rhs),school(rhs.school){}
Student& Student::operator=(const Student& rhs){
	Student::operator=(rhs);
	school = rhs.school;
	return *this
}
```
当编写一个copy函数的，必须要确保(1)复制所有的local变量(2)调用父类的对应的copy函数。
请记住：
- Copying 函数应该确保复制“对象内的所有成员函数”及“所有base class成分”
- 不要尝试以某个copying函数去实现另一个copying函数，应将共同的机制放进第三个函数中，并由两个copying函数共同调用
