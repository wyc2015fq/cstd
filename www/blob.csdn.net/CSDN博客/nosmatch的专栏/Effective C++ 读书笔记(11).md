# Effective C++ 读书笔记(11) - nosmatch的专栏 - CSDN博客
2013年07月10日 10:18:48[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：545
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款11：在operator=中处理“自我赋值”
> 
自我赋值是指将对象自己赋值给自己；例如
```cpp
People p；
...
p = p;
```
虽然这看起来比较好笑，但是这种情况很可能会出现的，例如当你对两个指针进行赋值操作的时候，两个指针指向的对象就可能
是同一个，在C++中，由于多态的出现，类型不同的指针或者引用也可能指向同一个对象，例如：
> 
```cpp
class People{...};
class Student : public People{...};
void doSomething(const People& p, const Student& s){
    ....
}
```
其中p和s就可能指向的是同一个对象，当对象进行自我赋值的时候会出现什么问题的呢？看下面的例子:
```
class Bitmap{.....};
class Wdiget{
public:
	Wdiget& operator=(const Wdiget& rhs){
		delete p;
		p = new Bitmap(*rhs.p);
		return *this;
	}
	...
private:
	Bitmap* p;
};
```
在这个例子中，如果rhs与this指向的是同一个对象，那么当delete p以后，就包含一个空的对象，因此最终的p指向一个被
删除的对象！为解决这个问题我们可以在delete之前首先做判同操作，如下：
> 
```cpp
Wdiget& operator=(const Wdiget& rhs){
    if(&rhs == this){
        return *this;
	}
    delete p;
    p = new Bitmap(*rhs.p);
    return *this;
}
```
这样在正常情况下是不会出现自我赋值情况，但是如果在new Bitmap的时候出现异常怎么办呢？此时this的p已经被释放，下面
是另外的一种解决办法：
> 
```cpp
Wdiget& Wdiget::operator=(const Wdiget& rhs){
	Bitmap* tmp = p;
	p = new Bitmap(*rhs.p);
	delete tmp;
	return *this;
};
```
这样当在new Bitmap出现异常的情况下，this还是保持原先的不变！
此外书中还提到一种copy and wap的技术，这个在后面会详细的介绍。
请记住：
- 确保当对象进行自我赋值的时候有良好的行为，其中包括来源对象和目标对象的比较，设计良好的赋值顺序，以及copy and swap技术
- 要确保当一个函数操作对个对象，并且多个对象可能是同一个对象的时候行为也是准确的
> 

