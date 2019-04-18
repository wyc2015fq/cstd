# Effective C++ 读书笔记(24) - nosmatch的专栏 - CSDN博客
2013年07月14日 22:01:09[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：609
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款24：若所有参数皆需要类型转换，请为此采用non-member函数
> 
考虑一个有理数的类：
```cpp
class Ration{
public:
	Ration(int numerator = 0, int denominator = 1);
	int numerator() const;
	int denominator() const;
private:
	int numerator;
	int denominator;
};
```
对于不同类型的数中我们希望能够进行隐式的类型转化，例如int型与double类型进行操作时int型会隐式的转化为double类
型这里我们也希望能将int型和Ration类型进行类型转化，首先观察下operator*函数：
> 
```cpp
class Ration{
public:
	...
	const Ration operator* (const Ration& lhs,const Ration& rhs) const;
};
```
下面的操作：
```cpp
Ration oneEight(1,8);
Ration oneHalf(1,2);
Ration result = oneEight * oneHalf;
result = result * oneEight;
```
在当前的这种情况下上面的操作能进行的很好，接下来看下面的操作：
```cpp
result = oneEight * 2;
result = 2 * oneEight;
```
第一个能执行的很好，但是第二个却不能编译通过，对于内置类型中的交换律该类不能很好的支持，如果我们修改成下
面的形式我们就能很好的看出来为什么第二个不能支持：
> 
```cpp
result = oneEight.operator*(2);
result = 2.operator*(oneEight);
```
明显的对于第二个，2不是一个Ration类，想对应的也就没有operator*函数，所以失败，在第一个中，在oneEight的
operator*操作中需要的是一个Ration的对象，在这里2隐式的转化为了一个Ration对象，因为Raion构造函数有默认值并且不是
explicit修饰的！为了能保证支持下面那种情况，我们要把第二种情况下的2也能转化为一个Ration类型，因为只有参数中才能
进行隐式类型转化，因此我们也就是将第二种情况下的2也作为参数传入，此时我们将member函数转化为一个non-member函数，
如下：
> 
```cpp
const Ration operator*(const Ration& lhs,const Ration& rhs){
    ...
}
```
因为此时不是做为一个class的member函数，因此第一个参数也就不会是一个this指针的Ration对象，而是作为一个
operator*的第一个参数存在，所以上面的两种情况都能很好的做到支持！
> 
还有人可能想到需不需要把该函数作为该class的friend函数，记住如果non-member和non-friend函数能解决的就不要应用
对应的member和friend函数，这个在上一条款中已经说过了！
请记住：
- 如果你需要为一个函数的所有参数进行类型转化(包括this指针所指向的那个隐喻参数)，那么这个函数为non-member函数！
> 
