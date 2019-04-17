# C++中的函数对象（Function Object）（一） - BonChoix的专栏 - CSDN博客





2012年10月09日 08:24:32[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：30285








                
       一个函数对象，即一个重载了括号操作符“（）”的对象。当用该对象调用此操作符时，其表现形式如同普通函数调用一般，因此取名叫函数对象。举个最简单的例子：
```cpp
class FuncObjType
{
public:
	void operator() ()
	{
		cout<<"Hello C++!"<<endl;
	}
};
```


类FuncObjType中重载了“（）”操作符，因此对于一个该类的对象FuncObjType val，可以这样调用该操作符：val()。调用结果即输出以上代码中的内容。该调用语句在形式上跟以下函数的调用完全一样：
```cpp
void val()
{
	cout<<"Hello C++!"<<endl;
}
```



       既然用函数对象与调用普通函数有相同的效果，为什么还有搞这么麻烦定义一个类来使用函数对象？主要在于函数对象有以下的优势：

       1. 函数对象可以有自己的状态。我们可以在类中定义状态变量，这样一个函数对象在多次的调用中可以共享这个状态。但是函数调用没这种优势，除非它使用全局变量来保存状态。呃，面向对象编程。。。全局变量。。。。![惊讶](http://static.blog.csdn.net/xheditor/xheditor_emot/default/ohmy.gif)

       2. 函数对象有自己特有的类型，而普通函数无类型可言。这种特性对于使用C++标准库来说是至关重要的。这样我们在使用STL中的函数时，可以传递相应的类型作为参数来实例化相应的模板，从而实现我们自己定义的规则。比如自定义容器的排序规则。



       举几个使用函数对象的典型的例子：

       第一个即自定义容器set对字符串string的排序规则。首先来定义相应的类并重载 “（）” 来规定排序（这里我们按从大到小排序），

```cpp
class StringSort
{
public:
	bool operator() (const string &str1, const string &str2) const
	{
		return str1 > str2;
	}
};
```

然后我们可以用这个类作为参数来初始化set容器：

```cpp
set<string,StringSort> myset;
myset.insert("A");
myset.insert("B");
```

这样容器内容输出为：“B“，”A"，而不是默认情况下的"A"，"B"。

       再举一个实用到内部状态的函数对象的例子。设想我们通过一个函数对象来产生一串连续的数字，可以任意规定起始数字，每调用一次，将返回下一个数字。相应的类可以如下设计：

```cpp
class SuccessiveNumGen
{
public:
	SuccessiveNumGen(int origin = 0):m_origin(origin){}
	int operator() ()
	{
		return m_origin++;
	}
private:
	int m_origin;
};
```

为了测试，我们用到一个标准库函数：generate_n。这个函数自动调用我们传进去的函数对象n次，并将结果保存到指定的起始迭代器处。如下：

```cpp
vector<int> dest;
generate_n(back_inserter(dest),10,SuccessiveNumGen(3));
```

这里我们用dest来存放数字，初始化了一个函数对象SuccessiveNumGen(3)，即起始数字为3。generate_n函数自动调用10次，结果存放在dest容器中。这里还用到了back_inserter函数，这是个函数适配器，它返回一个函数对象，能保证每次将结果放到容器dest的尾部。 函数适配器在后面会介绍，这里不再多说。我们只当它提供了我们存放数字的位置即可（切不可直接使用dest.end()！）。这样，最终容器中的内容为 3，4，5，6，……，12。通过使用不同的起始数字来初始化不同的函数对象，可以生成不同的数字序列。


       此外，函数对象还有一个非常重要的用途，即作为谓词函数（Predicate）。谓词函数通常用来对传进来的参数进行判断，并返回布尔值。标准库中有大量的函数都定义了多个重载版本，其中包含由用户提供谓词函数的，比如：find_if，remove_if，等等。现在假设我们有一串数字，要从中找出第一个不小于10的数字。可以定义如下相应的类：

```cpp
class NoLess
{
public:
	NoLess(int min = 0):m_min(min){}
	bool operator() (int value) const
	{
		return value >= m_min;
	}
	
private:
	int m_min;
};
```

从而这样调用 find_if函数：find_if(dest.begin(),dest.end(),NoLess(10))即可，该函数返回第一个令函数对象返回true的值的位置（在这个例子中即10对应的迭代器位置）。



       有一点需要指出的是，在调用用到函数对象的标准库算法时，除非显式地指定模板类型为“传引用”，否则默认情况下函数对象是”按值传递“的！因此，如果传递的是一个具有内部状态的函数对象，则被改变状态的是函数内部被复制的临时对象，函数结束后随之消失。真正传进来的函数对象状态并为改变。我们写个简单的测试程序来验证下，这个例子中我们来查找数字序列中的第3个数字：

```cpp
class Nth
{
public:
	Nth(int n=0):m_nth(n),m_count(1){}
	bool operator() (int)
	{
		return m_count++ == m_nth;
	}
	
	int GetCount()const
	{
		return m_count;
	}
	
private:
	int m_nth;
	int m_count;
};
```

测试如下：

```cpp
Nth nth(3);
vector<int>::iterator nthItr = find_if(dest.begin(),dest.end(),nth);  //dest内容为连续数字：3,4,5,6,……,12
cout<<"3rd:"<<*nthItr<<endl;
cout<<"State:"<<nth.GetCount()<<endl;
```

输出结果为，确实能找到第三个数字（5），但查看nth的状态时，返回的m_count依然为0。说明nth确实未被修改。


       一般情况下，至于是传值还是传引用，不会对我们造成太多影响。但还是有必要清楚这一点，困为有时候针对特定的实现会出现一些莫名其妙的问题，以下面的例子来说明：

       我们通过remove_if函数来删除一个数字序列中的第3个数字，如下：

```cpp
vector<int> vec;
for(vector<int>::size_type i=0; i<10; ++i)
	vec.push_back(i+1);

Nth nth(3);

remove_if(vec.begin(),vec.end(),nth);
	
for(vector<int>::size_type i=0; i<10; ++i)
{
	cout<<vec[i]<<endl;
}
```

按正常思路来讲，删除第3个元素后输出应该是：1,2,4,5,6,7,8,9,10,10(至于最后为什么会出现两次10，这跟标准算法库设计思想有关：算法库绝不改变容器的大小！所谓的remove只是概念上的remove，被删的元素被丢到了后面，可以通过返回值来确定位置。标准算法库以后会进行说明，不是这部分的重点）。但实际情况却令人吃惊：1,2,4,5,7,8,9,10,9,10。

造成这种现象的原因与标准库的实现有关，在《The C++ Standard Library, A tutorial and reference》这本书上，作者给出一种造成这种情况的可能的实现：

```cpp
template <class ForwIter, class Predicate>
ForwIter remove_if(ForwIter beg, ForwIter end, Predicate op)
{
	beg = std::find_if(beg, end, op);
	if (beg == end) 
	{
		return beg;
	}
	else 
	{
		ForwIter next = beg;
		return remove_copy_if(++next, end, beg, op);
	}
}
```

在这个实现中，我们清晰地看出，函数对象op两次作为参数传递到find_if和remove_copy_if函数中，正是由于标准库默认的"按值传递”，导致两次传递进来的op都是最初始的状态，即m_count为0！这样一来，传递到remove_if函数中的op将从+next位置(next是第一次找到的将被删除的第三个位置）算起再次删除第三个元素，于是第六个元素也就被删除了。

此外，C++参考手册网站[C++参考手册](http://en.cppreference.com/w/)上提供的是这样一种可能的实现：

```cpp
template<class ForwardIt, class UnaryPredicate>
ForwardIt remove_if(ForwardIt first, ForwardIt last, 
                          UnaryPredicate p)
{
    ForwardIt result = first;
    for (; first != last; ++first) {
        if (!p(*first)) {
            *result++ = *first;
        }
    }
    return result;
}
```

显然，这种实现则不会造成删除两次的情况，经过我的测试，结果确实是我们想要的：1,2,4,5,6,7,8,9,10,10。

       可见，不是所有的返回布尔值的函数对象都适合作为谓词函数，比如这个例子，不同标准库具体的实现会造成不同的结果。因此，作为一个好的编程习惯，用作谓词函数的函数对象，其判断结果最好不要依赖内部变动的状态。 

        （待续）           



