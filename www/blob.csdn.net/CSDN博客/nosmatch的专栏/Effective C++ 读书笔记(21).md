# Effective C++ 读书笔记(21) - nosmatch的专栏 - CSDN博客
2013年07月14日 11:42:45[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：636
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款21：必须返回对象时别妄想返回reference
> 
上一个条款讲解了以pass-by-reference来替代pass-by-value可以很好的增加效率，但是pass-by-reference也是不能乱用的
本条款主要讲在函数返回对象的时候不应该让他返回一个reference！上例子：
> 
```cpp
class Ration{
public:
	Ration(int numerator,int denominator);
	...
private:
	int n,b;
	friend const Ration operator*(const Ration& lhs,const Ration& rhs);
};
const Ration operator*(const Ration& lhs, const Ration& rhs){
      Ration result(lhs.n * rhs.n, lhs.b * rhs.b);
      return result;
}
```
> 
上面是个有理数的类，在操作符*中我们返回值一个pass-by-value的形式进行的，下面我们看下如果pass-by-reference
会出现什么情况：
> 
```cpp
const Ration& operator*(const Ration& lhs,const Ration& rhs){
	Ration result(lhs.n * rhs.n, lhs.b * rhs.b);
	return result;
}
```
上面的例子中，在函数内result的对象是在函数的堆栈中申请的，如果函数结束那么对象的空间将会自动的适当，此时返
回的引用是一个指向无用空间的引用，一旦对这个引用进行使用将会出现不确定的结果！还有一种情况是返回的引用的对象是
在函数的堆中申请的，如下：
> 
```cpp
const Ration& operator*(const Ration& lhs,const Ration& rhs){
	Ration result = new Ration(lhs.n * rhs.n, lhs.b * rhs.b);
	return result;
}
```
> 
在这种情况下如果不对new的空间进行内存的释放，返回的引用指向的空间都是有效的，但是容易产生的一个问题是内存
泄露的情况，如果有计算：Ration result = (a*b)*c,其中a、b、c都是Ration类型的对象，此时就会产生内存泄露的问题！
> 
书中还介绍了一种情况就是返回一种static的对象，例如：
```cpp
const Ration& operator*(const Ration& lhs,const Ration& rhs){
	static Ration result(lhs.n * rhs.n, lhs.b * rhs.b);
	return result;
}
```
此时在函数内部返回的是static的对象，这个对于多线程情况下容易发生错误，此外由于对象是函数local static，则这个对
像在一个函数内部是只保留一份copy的，那么我们对这个函数的多次调用中指向的result对象都是相同的！例如
> 
```cpp
if(a * b == b * c){
...
}
```
上面这个例子中，不管a/b/c是什么样子的数，返回的结果都是真，因为*返回的是一个静态的成员，而该成员在函数中只
保留一份！
请记住：
- 绝对不要返回一个ponit或者reference指向一个local stack对象、一个heap对象或者static对象，虽然pass-by-value会造成函数构造和析构的成本，但是至少能保证正确性！
