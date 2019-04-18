# ptf_fun - ljx0305的专栏 - CSDN博客
2012年08月23日 11:13:57[ljx0305](https://me.csdn.net/ljx0305)阅读数：703标签：[fun																[function																[each																[pointers																[character																[search](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=pointers&t=blog)](https://so.csdn.net/so/search/s.do?q=each&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=fun&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
                
关于ptf_fun的一些资料：
http://www.cnblogs.com/shootingstars/archive/2008/11/14/860042.html
以前使用bind1st以及bind2nd很少，后来发现这两个函数还挺好玩的，于是关心上了。
在C++ Primer对于bind函数的描述如下：
“
绑定器binder通过把二元函数对象的一个实参绑定到一个特殊的值上将其转换成一元函数对象
C++标准库提供了两种预定义的binder 适配器bind1st 和bind2nd 正如你所预料的bind1st 把值绑定到二元函数对象的第一个实参上bind2nd 把值绑定在第二个实参上
例如
为了计数容器中所有小于或等于10 的元素的个数我们可以这样向count_if()传递
count_if( vec.begin(), vec.end(), bind2nd( less_equal<int>(), 10 ));
”
哦，这倒是挺有意思的。于是依葫芦画瓢：
Helper template functions used to convert unary and binary function pointers, respectively, into unary and binary adaptable functions.
template<class Arg, class Result>
   pointer_to_unary_function<Arg, Result, Result (*)(Arg)>
      ptr_fun(Result (*_pfunc)(Arg));
template<class Arg1, class Arg2, class Result>
   pointer_to_binary_function<Arg1, Arg2, Result, Result (*)(Arg1, Arg2)>
      ptr_fun(Result (*_pfunc)(Arg1, Arg2));
#### Parameters
- _pfunc
The unary or binary function pointer to be converted to an adaptable function.
![](http://i.msdn.microsoft.com/Global/Images/clear.gif)
  Return Value 
The first template function returns the unary function [pointer_to_unary_function](http://msdn.microsoft.com/en-us/library/047wx24c%28VS.80%29.aspx) <**Arg**, **Result**>(*_pfunc).
The second template function returns binary function [pointer_to_binary_function](http://msdn.microsoft.com/en-us/library/a189f69f%28VS.80%29.aspx) <**Arg1**, **Arg2**, **Result**>(*_pfunc).
![](http://i.msdn.microsoft.com/Global/Images/clear.gif)
  Remarks 
A function pointer is a function object and may be passed to any Standard Template Library algorithm that is expecting a function as a parameter, but it is not adaptable. To use it with an adaptor, such as binding a value to it or using it with a negator,
 it must be supplied with the nested types that make such an adaptation possible. The conversion of unary and binary function pointers by the
**ptr_fun** helper function allows the function adaptors to work with unary and binary function pointers.
![](http://i.msdn.microsoft.com/Global/Images/clear.gif)
  Example 
// functional_ptr_fun.cpp
// compile with: /EHsc
#include <vector>
#include <algorithm>
#include <functional>
#include <cstring>
#include <iostream>
int main( )
{
   using namespace std;
   vector <char*> v1;
   vector <char*>::iterator Iter1, RIter;
   v1.push_back ( "Open" );
   v1.push_back ( "up" );
   v1.push_back ( "the" );
   v1.push_back ( "pearly" );
   v1.push_back ( "gates" );
   cout << "Original sequence contains: " ;
   for ( Iter1 = v1.begin( ) ; Iter1 != v1.end( ) ; Iter1++ )
      cout << *Iter1 << " ";
   cout << endl;
   // To search the sequence for "pearly"
   // use a pointer_to_function conversion
   RIter = find_if( v1.begin( ), v1.end( ),
      not1 ( bind2nd (ptr_fun ( strcmp ), "pearly" ) ) );
   if ( RIter != v1.end( ) )  
   {
      cout << "The search for 'pearly' was successful.\n";
      cout << "The next character string is: " 
           << *++RIter << "." << endl;
   }
}
![](http://i.msdn.microsoft.com/Global/Images/clear.gif)
  Output 
Original sequence contains: Open up the pearly gates 
The search for 'pearly' was successful.
The next character string is: gates.
![](http://i.msdn.microsoft.com/Global/Images/clear.gif)
  Requirements 
**Header:** <functional>
![](http://i.msdn.microsoft.com/Global/Images/clear.gif)
  See Also 
#### Reference
[Standard Template Library](http://msdn.microsoft.com/en-us/library/c191tb28%28VS.80%29.aspx)
#### Other Resources
[<functional> Members](http://msdn.microsoft.com/en-us/library/4y7z5x4b%28VS.80%29.aspx)
http://stl.winterxy.com/html/item_41.html
## 条款41：了解使用ptr_fun、mem_fun和mem_fun_ref的原因
ptr_fun/mem_fun/mem_fun_ref系列是什么意思的？有时候你必须使用这些函数，有时候不用，总之，它们是做什么的？它们似乎只是坐在那里，没用地挂在函数名周围就像不合身的外衣。它们不好输入，影响阅读，而且难以理解。这些东西是STL古董的附加例子（正如在[条款10](http://stl.winterxy.com/html/item_10.html)和[18](http://stl.winterxy.com/html/item_18.html)中描述的那样），或者只是一些标准委员会的成员因为太闲和扭曲的幽默感而强加给我们的语法玩笑？
冷静一下。虽然ptr_fun、mem_fun和mem_fun_ref的名字不太好听，但它们做了很重要的工作，而不是语法玩笑，这些函数的主要任务之一是覆盖C++固有的语法矛盾之一。
如果我有一个函数f和一个对象x，我希望在x上调用f，而且我在x的成员函数之外。C++给我三种不同的语法来实现这个调用：
f(x);			// 语法#1：当f是一个非成员函数
x.f();			// 语法#2：当f是一个成员函数
			// 而且x是一个对象或一个对象的引用
p->f();			// 语法#3：当f是一个成员函数
			// 而且p是一个对象的指针
现在，假设我有一个可以测试Widget的函数，
void test(Widget& w);		// 测试w，如果没通过
				// 就标记为“failed”
而且我有一个Widget的容器：
vector<Widget> vw;			// vw容纳Widget
要测试vw中的每个Widget，我很显然可以这么使用for_each：
for_each(vw.begin(), vw.end(), test);	// 调用#1（可以编译）
但想象test是一个Widget的成员函数而不是一个非成员函数，也就是说，Widget支持自我测试：
class Widget {
public:
	void test();		// 进行自我测试；如果没通过
				// 就把*this标记为“failed”
};
在一个完美的世界，我也将能使用for_each对vw中的每个对象调用Widget::test：
for_each(vw.begin(), vw.end(),
		&Widget::test);		// 调用#2（*不能编译*）
实际上，如果世界真的完美，我将也可以使用for_each来在Widget*指针的容器上调用Widget::test：
list<Widget*> lpw;				// lpw容纳Widget的指针
for_each(lpw.begin(), lpw.end(),
		&Widget::test);		// 调用#3（*也不能编译*）
但是想想在这个完美的世界里必须发生的。在调用#1的for_each函数内部，我们以一个对象为参数调用一个非成员函数，因此我们必须使用语法#1。在调用#2的for_each函数内部，我们必须使用语法#2，因为我们有一个对象和一个成员函数。而调用#3的for_each函数内部，我们需要使用语法#3，因为我们将面对一个成员函数和一个对象的指针。因此我们需要三个不同版本的for_each，而那个世界将有多完美？
在我们拥有的世界上，我们只有一个版本的for_each。想一种实现不难：
template<typename InputIterator, typename Function>
Function for_each(InputIterator begin, InputIterator end, Function f)
{
	while (begin != end) f(*begin++);
}
这里，我强调当调用时for_each是用语法#1这个事实。这是STL里的一个普遍习惯：函数和函数对象总使用用于非成员函数的语法形式调用。这解释了为什么调用#1可以编译而调用#2和#3不。这是因为STL算法（包括for_each）牢牢地绑定在句法#1上，而且只有调用#1与那语法兼容。
也许现在清楚为什么mem_fun和mem_fun_ref存在了。它们让成员函数（通常必须使用句法#2或者#3来调用的）使用句法1调用。
mem_fun和mem_fun_ref完成这个的方式很简单，虽然如果你看一下这些函数之一的声明会稍微清楚些。它们是真的函数模板，而且存在 mem_fun和mem_fun_ref模板的几个变体，对应于它们适配的不同的参数个数和常量性（或缺乏）的成员函数。看一个声明就足以理解事情怎样形成整体的：
template<typename R, typename C>		// 用于不带参数的non-const成员函数
mem_fun_t<R,C>				// 的mem_fun声明。
mem_fun(R(C::*pmf)());			// C是类，R是被指向
					// 的成员函数的返回类型
mem_fun带有一个到成员函数的指针，pmf，并返回一个mem_fun_t类型的对象。这是一个仿函数类，容纳成员函数指针并提供一个operator()，它调用指向在传给operator()的对象上的成员函数。例如，在这段代码中，
list<Widget*> lpw;				// 同上
...
for_each(lpw.begin(), lpw.end(),
		mem_fun(&Widget::test));	// 这个现在可以编译了
for_each接受一个mem_fun_t类型的对象，持有一个Widget::test的指针。对于在lpw里的每个Widget*指针，for_each使用语法#1“调用”mem_func_t，而那个对象立刻在Widget*指针上使用句法#3调用Widget::test。
总的来说，mem_fun适配语法#3——也就是当和Widget*指针配合时Widget::test要求的——到语法1，也就是for_each用的。因此也不奇怪像mem_fun_t这样的类被称为函数对象适配器。知道这个不应该使你惊讶，完全类似上述的，mem_fun_ref函数适配语法#2到语法#1，并产生mem_fun_ref_t类型的适配器对象。
mem_fun和mem_fun_ref产生的对象不仅允许STL组件假设所有函数都使用单一的语法调用。它们也提供重要的typedef，正如ptr_fun产生的对象一样。[条款40](http://stl.winterxy.com/html/item_40.html)告诉了你这些typedef后面的故事，所以我将不在这里重复它。但是，这使我们能够理解为什么这个调用可以编译。
for_each(vw.begin(), vw.end(), test);		// 同上，调用#1；
						// 这个可以编译
而这些不能：
for_each(vw.begin(), vw.end(), &Widget::test);	// 同上，调用#2；
						// 不能编译
for_each(lpw.begin(), lpw.end(), &Widget::test);	// 同上，调用#3；
						// 不能编译
第一个调用（调用#1）传递一个真的函数，因此用于for_each时不需要适配它的调用语法；这个算法将固有地使用适当的语法调用它。而且，for_each没有使用ptr_fun增加的typedef，所以当把test传给for_each时不必使用ptr_fun。另一方面，增加的 typedef不会造成任何损伤，所以这和上面的调用做相同的事情：
for_each(vw.begin(), vw.end(), ptr_fun(test));	// 可以编译，行为
						// 就像上面的调用#1
如果你关于什么时候使用ptr_fun什么时候不使用而感到困惑，那就考虑每当你传递一个函数给STL组件时都使用它。STL将不在乎，并且没有运行期的惩罚。可能出现的最坏的情况就是一些读你代码的人当看见不必要的ptr_fun使用时，可能会扬起眉毛。我认为，那有多让你操心依赖于你对扬起眉毛的敏感性。
一个与ptr_fun有关的可选策略是只有当你被迫时才使用它。如果当typedef是必要时你忽略了它，你的编译器将退回你的代码。然后你得返回去添加它。
mem_fun和mem_fun_ref的情况则完全不同。只要你传一个成员函数给STL组件，你就必须使用它们，因为，除了增加 typedef（可能是或可能不是必须的）之外，它们把调用语法从一个通常用于成员函数的适配到在STL中到处使用的。当传递成员函数指针时如果你不使用它们，你的代码将永远不能编译。
现在只留下成员函数适配器的名字，而在这里，最后，我们有一个真实的STL历史产物。当对这些种适配器的需求开始变得明显时，建立STL的人们正专注于指针的容器。（这种容器的缺点在[条款7](http://stl.winterxy.com/html/item_07.html)、[20](http://stl.winterxy.com/html/item_20.html)和[33](http://stl.winterxy.com/html/item_33.html)描述，这看起来可能很惊人，但是记住指针的容器支持多态，而对象的容器不支持。）他们需要用于成员函数的适配器，所以他们选择了mem_fun。但很快他们意识到需要一个用于对象的容器的另一个适配器，所以他们使用了mem_fun_ref。是的，它非常不优雅，但是这些事情发生了。告诉我你从未给你的任何组件一个你过后意识到，呃，很难概括的名字……
引用:[http://blog.c114.net/html/87/367587-56559.html](http://blog.c114.net/html/87/367587-56559.html)
