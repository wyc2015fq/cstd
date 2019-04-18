# 【设计模式】设计模式C++编程实现之单例模式（Singleton Pattern） - 江南烟雨 - CSDN博客
2013年04月17日 14:57:58[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3016标签：[C++																[设计模式](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[设计模式](https://blog.csdn.net/xiajun07061225/article/category/1174429)
有一些对象我们只需要一个，比如说线程池（threadpool）缓存（cache）、对话框、处理偏好设置和注册表的对象、日志对象，充当打印机、显卡等设备的驱动程序的对象。如果制造出多个实例，则会导致许多问题的产生。
许多时候的确可以通过程序员之间的阅读就可以办到，但是利用单例模式是更好的办法。
单例模式给我们一个全局的访问点，和全局变量一样方便，有没有全局变量的缺点（必须在程序开始就创建，如果后面没有用到，会造成资源的浪费）。
单件模式的定义：
确保一个类只有一个实例，并提供一个全局访问点。
UML图如下：
![](https://img-my.csdn.net/uploads/201304/17/1366182140_5986.png)
图一 单例模式UML图
单件模式实现方式：
将构造函数声明为私有的，在类中定义一个静态成员变量记录类的唯一实例。定义一个静态public方法来实例化对象，并返回这个实例。在这个方法中实行延迟化策略。如果我们不需要这个实例，就永远不会实例化它。在外部只能通过调用这个方法来实例化对象。
经典单件模式C++代码实现：
```cpp
//单例模式
#include <iostream>
using namespace std;
//经典单例模式实现
class Singleton{
private:
	static Singleton *uniqueInstance;//利用一个静态变量来记录Singleton的唯一实例
	Singleton(){};
public:
	//实例化对象的方法
	static Singleton* getInstance()
	{
		if(NULL == uniqueInstance)
			uniqueInstance = new Singleton();
		return uniqueInstance;
	}
};
Singleton* Singleton::uniqueInstance = NULL;//静态成员变量初始化
int main()
{
	Singleton *singletonObj = Singleton::getInstance();
	delete singletonObj;
	return 0;
}
```
当在多线程环境下使用单件模式时，可能会出现问题。问题出在函数getInstance(),没有实现同步。
解决方法：
（1）加同步锁
加锁来实现多个线程判断instance是否为空以及创建实例正常进行。代码如下：
```cpp
class SingletonPattern{
private:
static SingletonPattern *instance;//静态类型的实例
SingletonPattern(){}//构造函数
public:
static SingletonPattern *getInstance(){
Lock();//加锁函数
if (NULL == instance)//延迟初始策略
{
instance = new SingletonPattern();
}
UnLock();//释放锁函数
return instance;
}
};
```
但是可能会造成程序运行效率的急剧下降。
（2）双重检查加锁
我们可以针对上面一种加同步锁的方法进行优化。
因为只有当实例还没有创建之前需要进行加锁操作，以保证只有一个线程创建出实例。而当实例已经被创建出之后，就不需要再做加锁操作了。
优化后的代码如下：
```cpp
class SingletonPattern{
private:
static SingletonPattern *instance;//静态类型的实例
SingletonPattern(){}//构造函数
public:
static SingletonPattern *getInstance(){
if (NULL == instance)//延迟初始策略
{
Lock();//加锁函数
if (NULL == instance)
{
instance = new SingletonPattern();
}
UnLock();//释放锁函数
}
return instance;
}
};
```

