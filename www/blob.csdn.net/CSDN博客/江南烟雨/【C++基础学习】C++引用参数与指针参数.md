# 【C++基础学习】C++引用参数与指针参数 - 江南烟雨 - CSDN博客
2011年08月23日 08:48:44[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：4390
引用和指针参数，这两种参数都允许函数修改实参，也都允许有效地向函数传递大型类对象。那么，怎么样决定把函数参数声明成引用还是指针呢？
引用必须被初始化为指向一个对象，而且一旦初始化了，就不能指向其他对象。而指针可以指向一系列不同的对象也可以什么都不指向。
因为指针可能指向一个对象或者没有任何对象，所以函数在确定指针实际指向一个有效地对象之前不能安全地解引用一个指针。例如：
```cpp
class X;
void manip(X *px){
	//在解引用之前确定它非0
	if(px != 0)
		//解引用指针
}
```
另一方面，对于引用参数，函数不需要保证它指向一个对象。例如：
```cpp
class Type{};
void operate(const Type& p1,const Type& p2);
int main(){
	Type obj1;
	//错误：引用参数的实参不能为0
	Type obj2 = operate(obj1,0);
}
```
如果一个参数可能在函数中指向不同的对象，或者这个参数可能不指向任何对象，则必须使用指针参数。
引用参数的一个重要用法是，它允许我们在有效实现重载操作符的同事，还能保证用法的直观性。
看个实例，它使用了Matrix类类型。我们想支持两个Matrix对象的加法和赋值操作符，使他们的用法同内置类型一样“自然”：
```cpp
Matrix a,b,c;
c = a + b;
```
Matrix类对象的加法和赋值操作符用重载操作符来实现。
为重载操作符提供一个定义：
```cpp
Matrix operator+(Matrix m1,Matrix x2){
	Matrix result;
	//do the computation in result
	return result;
}
```
实参按值传递，效率很低。
为提高效率，假设我们将参数声明为指针。operator+()新的实现代码：
```cpp
Matrix operator+(Matrix *m1,Matrix *x2){
	Matrix result;
	//do the computation in result
	return result;
}
```
这样有一个缺点：失去了加法操作符用法的直观性。现在指针参数要求我们传递地址作为实参，指向Matrix对象。现在，我们的加法操作必须如下编程：
```cpp
&a + &b;
```
但是这样比较难看，接口不友好。而且，在一个复合表达式中加三个对象变得很困难：
```cpp
//这无法工作
//&a+&b的返回类型是Matrix对象
&a + &b + &c;
```
程序必须这样写：
```cpp
&(&a + &b) + &c;
```
但是，引用参数提供了我们需要的方案。此时函数接收到的是实参的左值而不是值得拷贝。引用参数的实参是Matrix对象本身，这允许我们像对内置数据类型的对象一样自然地使用加法操作符。
operator+()实现代码：
```cpp
Matrix operator+(Matrix &m1,Matrix &x2){
	Matrix result;
	//do the computation in result
	return result;
}
```
它支持如下形式的Matrix对象的加法：
```cpp
a + b + c;
```
