# STL运用的C++技术（6）——函数对象 . - ljx0305的专栏 - CSDN博客
2013年01月30日 15:43:51[ljx0305](https://me.csdn.net/ljx0305)阅读数：574
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
STL是C++标准库的重要组成部分之一，它不仅是一个可复用的组件库，更是一个包含算法与数据结构的软件框架，同时也是C++泛型编程的很好例子。STL中运用了许多C++的高级技术。本文介绍函数对象，其实是接上一篇的话题，因为函数对象本质上还是重载操作符。主要参考了《C++
 Primer》和《STL源码剖析》。
      可以为类类型的对象重载函数调用操作符，定义了调用操作符的类，其对象称之为函数对象（function object），即它们的行为类似函数对象。这是《C++ Primer》中的定义。下面通过一个例子来引出函数对象的使用。在我的解题笔记系列中，有一篇文章 [解题笔记（16）——几个数字的问题](http://blog.csdn.net/wuzhekai1985/article/details/6638718)，其中第四个问题是调整数组顺序使奇数位于偶数前面（数组）。输入一个整数数组，调整数组中数字的顺序，使得所有奇数位于数组的前半部分。所有偶数位于数组的后半部分。要求时间复杂度为O(n)。这个问题不难，其中用到了函数指针。代码如下：
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- bool IsOdd(int x)    
- {    
- return (x & 1)? true: false;    
- }    
- //函数功能 ： 调整数组顺序使奇数位于偶数前面   
- //函数参数 ： pArray指向数组的指针，nLen为数组长度   
- //返回值 ：   无   
- void PartionArray(int *pArray, int nLen, bool (*func)(int))    
- {    
- int i = -1;    
- for(int j = 0; j < nLen; j++)    
-     {    
- if(func(pArray[j])) //满足调整条件  
-         {    
-             i++;    
- int tmp = pArray[j];    
-             pArray[j] = pArray[i];    
-             pArray[i] = tmp;    
-         }    
-     }    
- }   
```cpp
bool IsOdd(int x)  
{  
    return (x & 1)? true: false;  
}  
//函数功能 ： 调整数组顺序使奇数位于偶数前面  
//函数参数 ： pArray指向数组的指针，nLen为数组长度  
//返回值 ：   无  
void PartionArray(int *pArray, int nLen, bool (*func)(int))  
{  
    int i = -1;  
    for(int j = 0; j < nLen; j++)  
    {  
        if(func(pArray[j])) //满足调整条件  
        {  
            i++;  
            int tmp = pArray[j];  
            pArray[j] = pArray[i];  
            pArray[i] = tmp;  
        }  
    }  
}
```
      传递一个函数指针的好处是，可以很方便的修改调整的条件，这个问题希望将奇数放在前面，偶数放在后面。如果有另外一个问题，希望将正数放前面，负数放后面，那么只需定义新的调整函数，类似 IsOdd，然后将它作为参数传递给PartionArray函数即可。
      这里其实也可以使用函数对象，代码如下：
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- #include <iostream> 
- usingnamespace std;  
- 
- //函数对象 
- template<class T>  
- struct IsOdd  
- {  
- bool operator() (T x){  
- return (x & 1)?true: false;  
-     }  
- };  
- //函数功能 ： 调整数组顺序使奇数位于偶数前面   
- //函数参数 ： pArray指向数组的指针，nLen为数组长度  
- //返回值 ：   无   
- template <class T, class F>  
- void PartionArray(T *pArray, int nLen, F func)  
- {    
- int i = -1;    
- for(int j = 0; j < nLen; j++)    
-     {    
- if(func(pArray[j])) //满足调整条件  
-         {    
-             i++;    
-             T tmp = pArray[j];    
-             pArray[j] = pArray[i];    
-             pArray[i] = tmp;    
-         }    
-     }    
- }    
- //测试用例 
- int main()  
- {  
- short a[] = {1,2,3,4,5,6};  
- long b[] = {1,2,3,4,5,6};  
-     PartionArray(a, 6, IsOdd<short>());   
-     PartionArray(b, 6, IsOdd<long>());   
- return 0;  
- }  
```cpp
#include <iostream>
using namespace std;
//函数对象
template<class T>
struct IsOdd
{
	bool operator() (T x){
		return (x & 1)?true: false;
	}
};
//函数功能 ： 调整数组顺序使奇数位于偶数前面  
//函数参数 ： pArray指向数组的指针，nLen为数组长度  
//返回值 ：   无  
template <class T, class F>
void PartionArray(T *pArray, int nLen, F func)
{  
    int i = -1;  
    for(int j = 0; j < nLen; j++)  
    {  
        if(func(pArray[j])) //满足调整条件  
        {  
            i++;  
            T tmp = pArray[j];  
            pArray[j] = pArray[i];  
            pArray[i] = tmp;  
        }  
    }  
}  
//测试用例
int main()
{
	short a[] = {1,2,3,4,5,6};
	long b[] = {1,2,3,4,5,6};
	PartionArray(a, 6, IsOdd<short>()); 
	PartionArray(b, 6, IsOdd<long>()); 
	return 0;
}
```
       相比函数指针，函数对象的优势在于能很好的满足STL的抽象要求，同时更为灵活。上面这个例子算是一个引子，引出STL中函数对象的运用。
       STL中的函数对象并不是孤立的，而是与STL的算法紧密联系在一起。举个简单的例子就明白了，比如我们希望利用STL的算法为一个数组排序，我们可以这样做。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- #include <iostream> 
- #include <algorithm> 
- usingnamespace std;  
- int main()  
- {  
- int a[] = {1,3,2,4,5,7};  
-     sort(&a[0], &a[6]);  
- for(int i = 0; i < 6; i++)  
-         cout<<a[i]<<' ';  
-     cout<<endl;  
- return 0;  
- }  
```cpp
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
	int a[] = {1,3,2,4,5,7};
	sort(&a[0], &a[6]);
	for(int i = 0; i < 6; i++)
		cout<<a[i]<<' ';
	cout<<endl;
	return 0;
}
```
       这个排序算法默认是递增排序，那么如果希望是递减排序呢？很方便，在函数实参中加入一个函数对象即可。记住需包含头文件 #include <functional>
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- #include <iostream> 
- #include <functional> 
- #include <algorithm> 
- usingnamespace std;  
- int main()  
- {  
- int a[] = {1,3,2,4,5,7};  
-     sort(&a[0], &a[6], greater<int>());  
- for(int i = 0; i < 6; i++)  
-         cout<<a[i]<<' ';  
-     cout<<endl;  
- return 0;  
- }  
```cpp
#include <iostream>
#include <functional>
#include <algorithm>
using namespace std;
int main()
{
	int a[] = {1,3,2,4,5,7};
	sort(&a[0], &a[6], greater<int>());
	for(int i = 0; i < 6; i++)
		cout<<a[i]<<' ';
	cout<<endl;
	return 0;
}
```
可以说在STL中，函数对象扮演着重要的角色，发挥着巨大的作用。下面列出了几个常用的函数对象，摘自HP的STL源码，做了部分修改。STL定义了两个类，作为一元操作和二元操作的基类，这两个基类仅仅是使用了内嵌型别技术，为什么要这样子呢？后面介绍函数适配器时有说明，可以看到它的强大之处。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- //用来定义一元操作的参数类别和返回值类别 
- template <class Arg, class Result>  
- struct unary_function {  
- typedef Arg argument_type;  //内嵌型别技术
- typedef Result result_type;  
- };  
- //用来定义二元操作的参数类别和返回值类别 
- template <class Arg1, class Arg2, class Result>  
- struct binary_function {  
- typedef Arg1 first_argument_type;  
- typedef Arg2 second_argument_type;  
- typedef Result result_type;  
- };  
- 
- //一元操作，就两个 
- template <class T>  
- struct negate : public unary_function<T, T> {  
-     T operator()(const T& x) const { return -x; }  
- };  
- template <class T>  
- struct logical_not : public unary_function<T,bool> {  
- bool operator()(const T& x) const { return !x; }  
- };  
- //加减乘除取模 
- template <class T>  
- struct plus : public binary_function<T, T, T> {  
-     T operator()(const T& x, const T& y) const { return x + y; }  
- };  
- template <class T>  
- struct minus : public binary_function<T, T, T> {  
-     T operator()(const T& x, const T& y) const { return x - y; }  
- };  
- template <class T>  
- struct multiplies : public binary_function<T, T , T> {  
-     T operator()(const T& x, const T& y) const { return x * y; }  
- };  
- template <class T>  
- struct divides : public binary_function<T ,T, T> {  
-     T operator()(const T& x, const T& y) const { return x / y; }  
- };  
- template <class T>  
- struct modulus : public binary_function<T, T, T> {  
-     T operator()(const T& x, const T& y) const { return x % y; }  
- };  
- //关系运算 
- template <class T>  
- struct equal_to : public binary_function<T, T, bool> {  
- bool operator()(const T& x, const T& y) const { return x == y; }  
- };  
- template <class T>  
- struct not_equal_to : public binary_function<T, T,bool> {  
- bool operator()(const T& x, const T& y) const { return x != y; }  
- };  
- template <class T>  
- struct greater : public binary_function<T, T, bool> {  
- bool operator()(const T& x, const T& y) const { return x > y; }  
- };  
- template <class T>  
- struct less : public binary_function<T, T, bool> {  
- bool operator()(const T& x, const T& y) const { return x < y; }  
- };  
- template <class T>  
- struct greater_equal : public binary_function<T, T, bool> {  
- bool operator()(const T& x, const T& y) const { return x >= y; }  
- };  
- template <class T>  
- struct less_equal : public binary_function<T, T, bool> {  
- bool operator()(const T& x, const T& y) const { return x <= y; }  
- };  
- //逻辑运算 
- template <class T>  
- struct logical_and : public binary_function<T, T, bool>{  
- bool operator()(const T& x, const T& y) const { return x && y; }  
- };  
- template <class T>  
- struct logical_or : public binary_function<T, T, bool>  
- {  
- bool operator()(const T& x, const T& y) const { return x || y; }  
- };  
```cpp
//用来定义一元操作的参数类别和返回值类别
template <class Arg, class Result>
struct unary_function {
	typedef Arg argument_type;  //内嵌型别技术
	typedef Result result_type;
};
//用来定义二元操作的参数类别和返回值类别
template <class Arg1, class Arg2, class Result>
struct binary_function {
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};
//一元操作，就两个
template <class T>
struct negate : public unary_function<T, T> {
	T operator()(const T& x) const { return -x; }
};
template <class T>
struct logical_not : public unary_function<T,bool> {
	bool operator()(const T& x) const { return !x; }
};
//加减乘除取模
template <class T>
struct plus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x + y; }
};
template <class T>
struct minus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x - y; }
};
template <class T>
struct multiplies : public binary_function<T, T , T> {
	T operator()(const T& x, const T& y) const { return x * y; }
};
template <class T>
struct divides : public binary_function<T ,T, T> {
	T operator()(const T& x, const T& y) const { return x / y; }
};
template <class T>
struct modulus : public binary_function<T, T, T> {
	T operator()(const T& x, const T& y) const { return x % y; }
};
//关系运算
template <class T>
struct equal_to : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x == y; }
};
template <class T>
struct not_equal_to : public binary_function<T, T,bool> {
	bool operator()(const T& x, const T& y) const { return x != y; }
};
template <class T>
struct greater : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x > y; }
};
template <class T>
struct less : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T>
struct greater_equal : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x >= y; }
};
template <class T>
struct less_equal : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x <= y; }
};
//逻辑运算
template <class T>
struct logical_and : public binary_function<T, T, bool>{
	bool operator()(const T& x, const T& y) const { return x && y; }
};
template <class T>
struct logical_or : public binary_function<T, T, bool>
{
  bool operator()(const T& x, const T& y) const { return x || y; }
};
```
      上面这些函数对象都比较简单，接下来介绍几个稍微复杂点的，它们被称为函数适配器（function adapter），用于特化和扩展一元和二元函数对象。分为两类，第一是绑定器（binder），它通过将一个操作数绑定到给定值而将二元函数对象转换为一元函数。第二是求反器（negator），它将谓词函数对象的真值求反。这些定义来自《C++ Primer》一书。书上还给了两个例子，这里罗列一下：
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- #include <iostream> 
- #include <functional> 
- #include <vector> 
- #include <algorithm> 
- usingnamespace std;  
- int main()  
- {  
-     vector<int> vec(10, 1);  
- int count1 = count_if(vec.begin(), vec.end(), bind2nd(less_equal<int>(), 10));       //求容器中小于等于10的元素个数
- int count2 = count_if(vec.begin(), vec.end(), not1(bind2nd(less_equal<int>(), 10))); //求容器中不小于等于10的元素个数，正好是上面结果的取反
-     cout<<count1<<' '<<count2<<endl;  //10 0
- return 0;  
- }  
```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
	vector<int> vec(10, 1);
	int count1 = count_if(vec.begin(), vec.end(), bind2nd(less_equal<int>(), 10));       //求容器中小于等于10的元素个数
	int count2 = count_if(vec.begin(), vec.end(), not1(bind2nd(less_equal<int>(), 10))); //求容器中不小于等于10的元素个数，正好是上面结果的取反
	cout<<count1<<' '<<count2<<endl;  //10 0
	return 0;
}
```
        下面分析一下STL是如何实现函数适配器的，先给出STL的源码。已经整理过了。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- //绑定第一个参数 
- template <class Operation>   
- class binder1st: public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {  
- public:  
-     binder1st(const Operation& x, consttypename Operation::first_argument_type& y) : op(x), value(y) {} //构造函数
- typename Operation::result_type operator()(consttypename Operation::second_argument_type& x) const {  
- return op(value, x);  //固定第一个参数
-     }  
- protected:  
-     Operation op;  
- typename Operation::first_argument_type value;  
- };  
- //绑定第二个参数 
- template <class Operation>   
- class binder2nd: public unary_function<typename Operation::first_argument_type,typename Operation::result_type> {  
- public:  
-     binder2nd(const Operation& x, consttypename Operation::second_argument_type& y) : op(x), value(y) {}  
- typename Operation::result_type operator()(consttypename Operation::first_argument_type& x) const {  
- return op(x, value);  //固定第二个参数
-     }  
- protected:  
-     Operation op;  
- typename Operation::second_argument_type value;  
- };  
- //外部接口 
- template <class Operation, class T>  
- inline binder1st<Operation> bind1st(const Operation& fn, const T& x) {  
- typedeftypename Operation::first_argument_type Arg1_type;  
- return binder1st<Operation>(fn,Arg1_type(x));  
- }  
- //外部接口 
- template <class Operation, class T>  
- inline binder2nd<Operation> bind2nd(const Operation& fn, const T& x) {  
- typedeftypename Operation::second_argument_type Arg2_type;  
- return binder2nd<Operation>(fn, Arg2_type(x));  
- }  
```cpp
//绑定第一个参数
template <class Operation> 
class binder1st: public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {
public:
	binder1st(const Operation& x, const typename Operation::first_argument_type& y) : op(x), value(y) {} //构造函数
	typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {
		return op(value, x);  //固定第一个参数
	}
protected:
	Operation op;
	typename Operation::first_argument_type value;
};
//绑定第二个参数
template <class Operation> 
class binder2nd: public unary_function<typename Operation::first_argument_type,typename Operation::result_type> {
public:
	binder2nd(const Operation& x, const typename Operation::second_argument_type& y) : op(x), value(y) {}
	typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
		return op(x, value);  //固定第二个参数
	}
protected:
	Operation op;
	typename Operation::second_argument_type value;
};
//外部接口
template <class Operation, class T>
inline binder1st<Operation> bind1st(const Operation& fn, const T& x) {
	typedef typename Operation::first_argument_type Arg1_type;
	return binder1st<Operation>(fn,Arg1_type(x));
}
//外部接口
template <class Operation, class T>
inline binder2nd<Operation> bind2nd(const Operation& fn, const T& x) {
	typedef typename Operation::second_argument_type Arg2_type;
	return binder2nd<Operation>(fn, Arg2_type(x));
}
```
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- //一元操作求反 
- template <class Predicate>  
- class unary_negate: public unary_function<typename Predicate::argument_type, bool> {  
- protected:  
-     Predicate pred;  
- public:  
- explicit unary_negate(const Predicate& x) : pred(x) {}  
- bool operator()(consttypename Predicate::argument_type& x) const {  
- return !pred(x);  
-   }  
- };  
- //二元操作求反 
- template <class Predicate>   
- class binary_negate : public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type,bool> {  
- protected:  
-     Predicate pred;  
- public:  
- explicit binary_negate(const Predicate& x) : pred(x) {}  
- bool operator()(consttypename Predicate::first_argument_type& x, consttypename Predicate::second_argument_type& y) const {  
- return !pred(x, y);   
-     }  
- };  
- //外部接口 
- template <class Predicate>  
- inline unary_negate<Predicate> not1(const Predicate& pred)  
- {  
- return unary_negate<Predicate>(pred);  
- }  
- //外部接口 
- template <class Predicate>  
- inline binary_negate<Predicate> not2(const Predicate& pred)  
- {  
- return binary_negate<Predicate>(pred);  
- }  
```cpp
//一元操作求反
template <class Predicate>
class unary_negate: public unary_function<typename Predicate::argument_type, bool> {
protected:
	Predicate pred;
public:
	explicit unary_negate(const Predicate& x) : pred(x) {}
	bool operator()(const typename Predicate::argument_type& x) const {
    return !pred(x);
  }
};
//二元操作求反
template <class Predicate> 
class binary_negate : public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type,bool> {
protected:
	Predicate pred;
public:
	explicit binary_negate(const Predicate& x) : pred(x) {}
	bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const {
		return !pred(x, y); 
	}
};
//外部接口
template <class Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred)
{
	return unary_negate<Predicate>(pred);
}
//外部接口
template <class Predicate>
inline binary_negate<Predicate> not2(const Predicate& pred)
{
	return binary_negate<Predicate>(pred);
}
```
          到这里，我们可以回答之前的一个问题，就是STL为什么要定义两个基类，里面仅仅是内嵌型别。通过上面代码，不难发现，原来是用来萃取函数对象所操作的数据类型。比如 binder1st 这个类，它的模板中只有一个形参，它继承自unary_function，而unary_function的模板有两个形参。如何实现这种继承关系呢？内嵌型别技术，因为binder1st 的模板实参是个函数对象，继承自unary_function或binary_function，通过内嵌型别技术很容易萃取出数据类型。
         再进一步，函数适配器是如何工作的呢？
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- include <iostream>  
- #include <functional> 
- #include <vector> 
- #include <algorithm> 
- usingnamespace std;  
- int main()  
- {  
-     vector<int> vec(10, 1);  
- int count1 = count_if(vec.begin(), vec.end(), bind2nd(less_equal<int>(), 10));       //求容器中小于等于10的元素个数
- int count2 = count_if(vec.begin(), vec.end(), not1(bind2nd(less_equal<int>(), 10))); //求容器中不小于等于10的元素个数，正好是上面函数的取反
-     cout<<count1<<' '<<count2<<endl;  //10 0
- return 0;  
- }  
```cpp
include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
	vector<int> vec(10, 1);
	int count1 = count_if(vec.begin(), vec.end(), bind2nd(less_equal<int>(), 10));       //求容器中小于等于10的元素个数
	int count2 = count_if(vec.begin(), vec.end(), not1(bind2nd(less_equal<int>(), 10))); //求容器中不小于等于10的元素个数，正好是上面函数的取反
	cout<<count1<<' '<<count2<<endl;  //10 0
	return 0;
}
```
        还是以这个程序为例，介绍一下count_if(vec.begin(), vec.end(), bind2nd(less_equal<int>(), 10))具体执行。首先执行bind2nd(less_equal<int>(), 10)这个函数，这个函数会返回一个binder2nd的函数对象，注意构造这个函数对象的时候，binder2nd(const Operation& x, const typename
 Operation::second_argument_type& y) : op(x), value(y) {} 。第二个参数value的值设置为10，而第一个参数op设置成less_equal<int>()这个函数对象。
       接着count_if 执行时，下面是它的源码。执行时，pred(*first)其实就是binder2nd中的运算，而这个运算的第二个参数是固定的，也就是10，所以pred只传递了一个实参进去就可以了。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- template <class InputIter, class Predicate, class Size>  
- void count_if(InputIter first, InputIter last, Predicate pred, Size& n) {  
- for ( ; first != last; ++first)  
- if (pred(*first))  
-             ++n;  
- }  
- template <class InputIter, class Predicate>  
- typename iterator_traits<InputIter>::difference_type count_if(InputIter first, InputIter last, Predicate pred) {  
- typename iterator_traits<InputIter>::difference_type n = 0;  
- for ( ; first != last; ++first)  
- if (pred(*first))  
-             ++n;  
- return n;  
- }  
```cpp
template <class InputIter, class Predicate, class Size>
void count_if(InputIter first, InputIter last, Predicate pred, Size& n) {
	for ( ; first != last; ++first)
		if (pred(*first))
			++n;
}
template <class InputIter, class Predicate>
typename iterator_traits<InputIter>::difference_type count_if(InputIter first, InputIter last, Predicate pred) {
	typename iterator_traits<InputIter>::difference_type n = 0;
	for ( ; first != last; ++first)
		if (pred(*first))
			++n;
	return n;
}
```
        下面把上述综合起来，通过修改STL，写个完整的测试程序。如下所示，注意这里用的都是自定义的代码，没有包含STL的函数对象和函数。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6658940#)
- #include <iostream> 
- #include <vector> 
- usingnamespace std;  
- 
- //count_if函数 
- template <class InputIter, class Predicate, class Size>  
- void count_if(InputIter first, InputIter last, Predicate pred, Size& n) {  
- for ( ; first != last; ++first)  
- if (pred(*first))  
-             ++n;  
- }  
- //用来定义一元操作的参数类别和返回值类别 
- template <class Arg, class Result>  
- struct unary_function {  
- typedef Arg argument_type;  
- typedef Result result_type;  
- };  
- //用来定义二元操作的参数类别和返回值类别 
- template <class Arg1, class Arg2, class Result>  
- struct binary_function {  
- typedef Arg1 first_argument_type;  
- typedef Arg2 second_argument_type;  
- typedef Result result_type;  
- };  
- //本测试之用到关系函数对象 
- template <class T>  
- struct less_equal : public binary_function<T, T, bool> {  
- bool operator()(const T& x, const T& y) const { return x <= y; }  
- };  
- //绑定第二个参数 
- template <class Operation>   
- class binder2nd: public unary_function<typename Operation::first_argument_type,typename Operation::result_type> {  
- public:  
-     binder2nd(const Operation& x, consttypename Operation::second_argument_type& y) : op(x), value(y) { cout<<"binder2nd Constructor"<<endl; }  
- typename Operation::result_type operator()(consttypename Operation::first_argument_type& x) const {  
-         cout<<"binder2nd's operator()"<<endl;  
- return op(x, value);  //固定第二个参数
-     }  
- protected:  
-     Operation op;  
- typename Operation::second_argument_type value;  
- };  
- //外部接口 
- template <class Operation, class T>  
- inline binder2nd<Operation> bind2nd(const Operation& fn, const T& x) {  
- typedeftypename Operation::second_argument_type Arg2_type;  
- return binder2nd<Operation>(fn, Arg2_type(x));  
- }  
- //一元操作求反 
- template <class Predicate>  
- class unary_negate: public unary_function<typename Predicate::argument_type, bool> {  
- protected:  
-     Predicate pred;  
- public:  
- explicit unary_negate(const Predicate& x) : pred(x) { cout<<"unary_negate Constructor"<<endl; }  
- bool operator()(consttypename Predicate::argument_type& x) const {  
-     cout<<"unary_negate's operator()"<<endl;  
- return !pred(x);  
-   }  
- };  
- //外部接口 
- template <class Predicate>  
- inline unary_negate<Predicate> not1(const Predicate& pred)  
- {  
- return unary_negate<Predicate>(pred);  
- }  
- //测试程序 
- int main()  
- {  
-     vector<int> vec(10, 1);  
- int count1 = 0, count2 = 0;  
-     count_if(vec.begin(), vec.end(), bind2nd(less_equal<int>(), 10),count1);       //求容器中小于等于10的元素个数
-     count_if(vec.begin(), vec.end(), not1(bind2nd(less_equal<int>(), 10)),count2); //求容器中不小于等于10的元素个数，正好是上面函数的取反
-     cout<<count1<<' '<<count2<<endl;  //10 0
- return 0;  
- }  
```cpp
#include <iostream>
#include <vector>
using namespace std;
//count_if函数
template <class InputIter, class Predicate, class Size>
void count_if(InputIter first, InputIter last, Predicate pred, Size& n) {
	for ( ; first != last; ++first)
		if (pred(*first))
			++n;
}
//用来定义一元操作的参数类别和返回值类别
template <class Arg, class Result>
struct unary_function {
	typedef Arg argument_type;
	typedef Result result_type;
};
//用来定义二元操作的参数类别和返回值类别
template <class Arg1, class Arg2, class Result>
struct binary_function {
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};
//本测试之用到关系函数对象
template <class T>
struct less_equal : public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y) const { return x <= y; }
};
//绑定第二个参数
template <class Operation> 
class binder2nd: public unary_function<typename Operation::first_argument_type,typename Operation::result_type> {
public:
	binder2nd(const Operation& x, const typename Operation::second_argument_type& y) : op(x), value(y) { cout<<"binder2nd Constructor"<<endl; }
	typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
		cout<<"binder2nd's operator()"<<endl;
		return op(x, value);  //固定第二个参数
	}
protected:
	Operation op;
	typename Operation::second_argument_type value;
};
//外部接口
template <class Operation, class T>
inline binder2nd<Operation> bind2nd(const Operation& fn, const T& x) {
	typedef typename Operation::second_argument_type Arg2_type;
	return binder2nd<Operation>(fn, Arg2_type(x));
}
//一元操作求反
template <class Predicate>
class unary_negate: public unary_function<typename Predicate::argument_type, bool> {
protected:
	Predicate pred;
public:
	explicit unary_negate(const Predicate& x) : pred(x) { cout<<"unary_negate Constructor"<<endl; }
	bool operator()(const typename Predicate::argument_type& x) const {
	cout<<"unary_negate's operator()"<<endl;
    return !pred(x);
  }
};
//外部接口
template <class Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred)
{
	return unary_negate<Predicate>(pred);
}
//测试程序
int main()
{
	vector<int> vec(10, 1);
	int count1 = 0, count2 = 0;
	count_if(vec.begin(), vec.end(), bind2nd(less_equal<int>(), 10),count1);       //求容器中小于等于10的元素个数
	count_if(vec.begin(), vec.end(), not1(bind2nd(less_equal<int>(), 10)),count2); //求容器中不小于等于10的元素个数，正好是上面函数的取反
	cout<<count1<<' '<<count2<<endl;  //10 0
	return 0;
}
```
本人享有博客文章的版权，转载请标明出处[http://blog.csdn.net/wuzhekai1985](http://blog.csdn.net/wuzhekai1985)
