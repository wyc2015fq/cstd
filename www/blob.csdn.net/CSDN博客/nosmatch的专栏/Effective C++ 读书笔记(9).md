# Effective C++ 读书笔记(9) - nosmatch的专栏 - CSDN博客
2013年07月10日 00:54:36[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：513
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款九：绝对不要在构造函数或者析构函数中调用virtual函数
> 
首先上例子：
```cpp
class People{
public:
	People();
	virtual void task() const = 0;
	....
};
People::People(){
	....
	task();
}
class Studeng : public People{
public:
	virtual void task() const;
	.....
};
```
在People的构造函数中，调用了虚函数task,当我们定义一个student对象的时候，student的构造函数将会调用，在初始化
student的成员函数之前将首先调用父类的构造函数，在父类的构造函数中调用了pure virtual函数，此时本意想通过虚函数来调用
子类的对应函数，但是此时子类的构造函数还没有构造成功，因此此时调用的还是父类的pure virtual task,报错！即使task不是纯虚
函数，不会报错，但是会出现我们不想要的情况！
> 
同样的如果再析构函数中调用virtual函数，当在子类中调用virtual函数的时候，子类中的成员变量变量将呈现未定义的状
态，进入父类的析构函数的时候，此时将对象单纯的看作是父类的对象，此时的所有virtual函数调用或者dynamic_cast等都会那么看待！
> 
既然不能通过父类的虚函数在构造函数中调用子类中的函数，可以通过在子类调用父类的构造函数的时候将子类的信息
传递给父类来执行！
> 
```cpp
class People{
public:
	People();
	void task(std::string info) const;
	....
};
People::People(std::sting info){
	....
	task(info);
}
class Student : public People{
public:
	Student():People(createinfo(parame)){....}
	.....
private:
	static std::string createinfo(std::string parame){....}
}
```
请记住：
- 在构造函数和析构函数中不要调用虚函数，因为此时虚函数不会下降为对应的子类
> 
