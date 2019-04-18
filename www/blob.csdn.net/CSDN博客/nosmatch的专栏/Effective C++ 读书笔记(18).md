# Effective C++ 读书笔记(18) - nosmatch的专栏 - CSDN博客
2013年07月13日 15:06:31[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：485
条款18:让接口更容易的被使用，不易误用
> 
接口设计主要是给应用接口的人使用的，他们可能不是接口的设计者，这样作为接口的设计者就要对接口的定义更加易懂
，让使用者不宜发生误用，例如对于一个时间类：
> 
```cpp
class Date{
public:
    Data(int month, int day, int year){
    ....
    }
}；
```
在应用Date类的时候，对于Date的三个参数我们很容易用错，因为她们的类型相同，我们可能会将实参20传给month等，
我们在设定接口的时候要保证接口使用过程中不管怎么用都会不出错，例如我们可以定义三个类型：
> 
```cpp
struct month{
explicit month(int i):val(i){}
int val;
};
struce day{
explicit day(int i):val(i){}
int val;
};
struct year{
explicit year(int i):val(i){}
int val;
};
```
这样我们在使用的时候就不会出现将day参数和month参数用错的情况，例如:
```cpp
Date d(month(12),day(19),year(2013));
```
但是此时我们可能经month的参数用错，例如设置为>12,或者将day的参数大于该月的天数，这样我们可以经一步设置：
```cpp
class month{
public:
	static Month Jan(){return Month(1);}
	static Month Feb(){return Month(2);}
	.....
	static Month Dec(){return Month(12);}
private:
	explicit month(int m):val(m){}
	int val;
};
```
这样我们就可以调用：
```cpp
Date d(month::Jan(),day(18),year(2013));
```
我们用类的静态函数来替换对象来制定月份，相关的我们也可以设定day和year，这样我们就不会出现Month用错的情况
了。
> 
我们设定接口的一个原则就是限定类型内什么事情可以做，什么事情不可以做，我们在设定一个类型的时候，我们要让
它与内置的类型有相同的表现形式。
> 
当我们的接口返回的是指针的类型的时候，应用接口的客户可能会出现对指针所指向资源没有释放的情况，我们此时可
以使用前面介绍过的资源管理类来对资源进行管理，常用的是std::tr1::shared_ptr智能指针，它可以自己制定其所指资源释放的时候
所调用的函数，具体使用方式请参看相关资料。
请记住：
- 好的接口要容易被正确使用而不是误用
- "促进正确使用"的方法包括接口的一致性，与内置类型的行为兼容等
- "阻止误用"的方法包括建立新类型、限制类型上的操作、束缚对象值，以及消除客户对资源的管理责任。
- tr1::shared_ptr支持定制型删除器；

