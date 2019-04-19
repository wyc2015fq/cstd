# std::transform || inner_product || accumulate - 三少GG - CSDN博客
2011年12月16日 10:39:42[三少GG](https://me.csdn.net/scut1135)阅读数：1111标签：[class																[iterator																[function																[exchange																[parameters																[input](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=exchange&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[重构C/C++																[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)](https://blog.csdn.net/scut1135/article/category/621651)
该问题归结为std::transform函数的使用
函数原型
template < class InputIterator, class OutputIterator, class UnaryOperator > OutputIterator transform ( InputIterator first1, InputIterator last1, OutputIterator result, UnaryOperator op ); template < class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperator > OutputIterator transform ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryOperator binary_op );
说明：
对于第一个原型：**函数将对从输入参数的first1-last1的全部变量做op函数操作。结果保存到result中，或是通过返回值返回。**
对于原形二：**这个是对一的一个扩展，对于1这个只能对单个元素队列进行op操作，而第二个原形则是对对first1-last1上的元素和first2开始序列的元素逐个binary_op计算再保存到结果**
示例
int op_inc(int num) { return ++i; }
int op_sum(int numa,int numb) { return a+b; }
int main()
{
> 
vector<int >first;
vector<int >second;
vector<int >result;
.........//初始化过程省略,不过要注意当调用第二个原型的时候，保持队列二的长度要大于等于1
transform(first.begin(), first.end(), result.begin(), op_inc);
transform(first.begin(), first.end(), second.begin(), result.begin(), op_sum);
........//输出结果
return 0;
}
以上是transform的基本使用，接下来说明如何用它来处理字符串的大小写转换
事实上很简单，主要用到了，单个字符的大小写转换函数：tolower(),toupper()
不过对于大写转小写，同时小写转大写的要自己单独处理，函数如下
char exchange(char c)
{
> 
if(c <= 'Z' && c >= 'A')
> 
> 
c = tolower(c);
> 
else if(c >= 'a' && c <= 'z')
> 
> 
c = toupper(c);
> 
return c;
}
示例
std::string str = "Http";
transform(str.begin(), str.end(), str.begin(), ::tolower); //将大写的都转换成小写
transform(str.begin(), str.end(), str.begin(), ::toupper); //将小写的都转换成大写
transform(str.begin(), str.end(), str.begin(), exchange);  //大小写切换
注以上结果都保存在str中。
+++
```cpp
Code highlighting produced by Actipro CodeHighlighter (freeware)http://www.CodeHighlighter.com/-->#include <vector>
#include <deque>
#include <list>
#include <iterator>
#include <algorithm>
using std::vector;
using std::deque;
using std::list;
// Calculate the square of value.
inline unsigned CalculateSquare(unsigned p_uiNumber)
{
    return p_uiNumber*p_uiNumber;
}
// Entrance
int main()
{
    vector<unsigned> vecUint;
    const unsigned uiElementNumber = 6U;
    for (unsigned i = 0U; i < uiElementNumber; ++i)
    {
        vecUint.push_back(i);
    }
    deque<unsigned> deqUint;
    transform(vecUint.begin(), vecUint.end(), back_inserter(deqUint), CalculateSquare);
    list<unsigned> lstUint;
    transform(vecUint.begin(), vecUint.end(), deqUint.begin(), back_inserter(lstUint), std::multiplies<unsigned>());
    return EXIT_SUCCESS;
}
```
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# inner_product
**Visual Studio 2008**
[其他版本](http://msdn.microsoft.com/zh-cn/library/kkkz3679(v=VS.90).aspx)
![](http://i3.msdn.microsoft.com/Hash/c764fa3d6b8a84bd40aa131355215c98.png)
Computes the sum of the element-wise product of two ranges and adds it to a specified initial value or computes the result of a generalized procedure where the sum and product binary operations are replaced by other specified binary operations.

template<class InputIterator1, class InputIterator2, class Type>
   Type inner_product(
      InputIterator1 _First1, 
      InputIterator1 _Last1,
      InputIterator2 _First2, 
      Type _Val
   );
template<class InputIterator1, class InputIterator2, class Type,
   class BinaryOperation1, class BinaryOperation2>
   Type inner_product(
      InputIterator1 _First1, 
      InputIterator1 _Last1,
      InputIterator2 _First2, 
      Type _Val, 
      BinaryOperation1 _Binary_op1, 
      BinaryOperation2 _Binary_op2
   );
![](http://i.msdn.microsoft.com/Hash/030c41d9079671d09a62d8e2c1db6973.gif)Parameters
- _First1
An input iterator addressing the first element in the first range whose inner product or generalized inner product with the second range is to be computed.
- _Last1
An input iterator addressing the last element in the first range whose inner product or generalized inner product with the second range is to be computed.
- _First2
An input iterator addressing the first element in the second range whose inner product or generalized inner product with the first range is to be computed.
- _Val
An initial value to which the inner product or generalized inner product between the ranges is to be added.
- *_Binary_op1*
The binary operation that replaces the inner product operation of sum applied to the element-wise products in the generalization of the inner product.
- *_Binary_op2*
The binary operation that replaces the inner product element-wise operation of multiply in the generalization of the inner product.
![](http://i.msdn.microsoft.com/Hash/030c41d9079671d09a62d8e2c1db6973.gif)Return
 Value
The first member function returns the sum of the element-wise products and adds to it the specified initial value. So for ranges of values *a*i and *b*i, it returns:
_Val + ( *a*1 * *b*1 ) + ( *a*2 * *b*2 ) +
by iteratively replacing _Val with _Val + (**a*i * **b*i ).
**The second member function returns:**
**_Val _*Binary_op1* ( *a*1 _*Binary_op2 b*1 ) _*Binary_op1* ( *a*2 _*Binary_op2 b*2 ) _*Binary_op1***
**by iteratively replacing _Val with _Val _*Binary_op1* (**a*i _*Binary_op2* **b*i ).**
难道刚好重载了上面这个？？？~~_Val** _Binary_op1 ( a**1** _Binary_op2 b**1** ) _Binary_op1**~~
关于重载():
在这个例子中，二元函数对象divides()用第二个序列的元素除第一个序列中相对应的元素。因此，divides()接受的参考顺序为：
divides(*first*, *second*)
这种可以进行推广：当使用二元函数对象时，它的参数顺序是*first*，*second*。
创建函数对象Creating a Function Object
除了使用内置对象之外，你也可以创建自己的函数对象。要这样做，你仅需重载一个类的operator()函数。然而，出于最大灵活性考虑，你要使用下面由STL定义的一个类作为你的函数对象的基类。
template <class Argument, class Result> struct unary_function {
typedef Argument argument_type;
typedef Result result_type;
};
template <class Argument1, class Argument2, class Result>
struct binary_function {
typedef Argument1 first_argument_type;
typedef Argument2 second_argument_type;
typedef Result result_type;
};
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# accumulate
**Visual Studio 2008**
[其他版本](http://msdn.microsoft.com/zh-cn/library/aawk6wsh(v=VS.90).aspx)
![](http://i3.msdn.microsoft.com/Hash/c764fa3d6b8a84bd40aa131355215c98.png)
Computes the sum of all the elements in a specified range including some initial value by computing successive partial sums or computes the result of successive partial results similarly obtained from using a specified binary operation other than the sum.

template<class InputIterator, class Type>
   Type accumulate(
      InputIterator _First, 
      InputIterator _Last, 
      Type _Val
   );
template<class InputIterator, class Type, class BinaryOperation>
   Type accumulate(
      InputIterator _First, 
      InputIterator _Last, 
      Type _Val, 
      BinaryOperation _Binary_op
   );
![](http://i.msdn.microsoft.com/Hash/030c41d9079671d09a62d8e2c1db6973.gif)Parameters
- _First
An input iterator addressing the first element in the range to be summed or combined according to a specified binary operation.
- _Last
An input iterator addressing the last element in the range to be summed or combined according to a specified binary operation that is one position beyond the final element actually included in the iterated accumulation.
- _Val
An initial value to which each element is in turn added or combined with according to a specified binary operation.
- _Binary_op
The binary operation that is to be applied to the each element in the specified range and the result of its previous applications.
