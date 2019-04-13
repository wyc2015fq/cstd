
# C++学习之一、掌握类和对象 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月26日 21:28:20[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1161标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[编译器																](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=string&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
																								](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=c++&t=blog)

**掌握类和对象**
**---本文是基于大家已经知道类和对象的基础上强化一些知识**
**对象构造：**
在栈上使用构造函数唯一正确的方法：
ClassName myclass(5);
在堆上使用构造函数：
ClassName *myclass = new ClassName（5）;
在栈上声明对象就会调用其构造函数，而在堆上可以声明一个类的对象指针，不必立即调用构造函数。
在栈上调用默认构造函数要去掉构造函数的小括号。
一旦自己定义了构造函数，系统就不会自动生成默认构造函数了。
即如果没有显式得定义构造函数，系统会自动生成一个默认构造函数（0参数构造函数）
如果没有显式得定义拷贝构造函数，系统会自动生成一个按值传递的拷贝构造函数。
定义构造函数时，尽量使用初始化列表，警告：数据成员会按其出现在类定义中的顺序得到初始化，而不是按照初始化列表中的顺序。
如果函数或类方法中不会改变参数的值，则尽量使用const 引用来传递参数。
**对象撤销：**
如果没有析构函数，系统会自动生成一个析构函数。
对于栈上的对象，一旦出了作用域，则对象会调用析构函数撤销对象。
所以不要返回函数或者类方法中定义的对象的引用。
**对象赋值：**
如下面：
SpreadsheetCell myCell(5),antherCell;
antherCell = myCell;
你可能想说myCell被“复制”到antherCell。不过，在C++世界中，“复制”只会在对象初始化才会出现。如果一个对象已经有值了，而这个值要被重写或覆盖。，更准确地说是“赋值”。C++为复制操作提供的是拷贝构造函数。由于拷贝构造函数是一个构造函数，因此只能用于对象创建，而不能用于以后对对象的赋值操作了。因此C++为赋值操作提供的是赋值操作符=重载。如果你没有自己定义一个赋值操作符，则系统会自动生成一个默认的赋值操作符=，它与默认拷贝行为几乎完全等同。不过不同与拷贝构造函数的是，赋值操作符会返回对象的一个引用，原因是赋值可以串链。
myCell = antherCell = otherCell;
**区分赋值和复制：**
SpreadsheetCell myCell(5);
SpreadsheetCell antherCell(myCell);   //复制（拷贝）构造函数
//也是复制构造函数构造的，没有调用operator=！，相当于SpreadsheetCell aThirdCell(myCell);
SpreadsheetCell aThirdCell = myCell;
antherCell = myCell; //调用operator=！因为anther已经构造。
综上：=不一定是调用operator=表示赋值，当用在变量声明的同一行上时，是复制构造函数的简写。
**对象作为返回值：**
SpreadsheetCell myCell(5);
string s1;
s1 = myCell.getString();
解析：当getString()返回对象myCell的mstringl时，编译器调用了一个string拷贝构造函数，创建一个匿名的string对象。将这个结果赋值给s1时，会以这个临时string作为参数，对s1调用赋值操作符。然后这个临时string对象会被撤销。
而对于：
SpreadsheetCell myCell(5);
string s1 = myCell.getString();
解析：当getString()返回对象myCell的mstringl时，编译器调用了一个string拷贝构造函数，创建一个匿名的string对象。然后s1调用拷贝构造函数，而不是赋值操作符。
**拷贝构造函数和对象成员：**
如果一个对象的成员中还包含其他对象时，，编译器生成的拷贝构造函数就会递归地调用所包含的各个对象的拷贝构造函数。如果你编写了自己的拷贝构造函数，可以利用初始化列表提供同样的语义。如果在初始化列表中没有包含某个数据成员，编译器会对其完成默认初始化（即调用该对象的0参数默认构造函数），然后才执行构造函数体中的代码。因此，一旦使用初始化列表，等到执行构造函数体时，所有对象成员已经得到了初始化。
也可以不用初始化列表来编写拷贝构造函数，如下：
SpreadsheetCell：：SpreadsheetCel(const SpreadsheetCel & src)
{
mValue = src.mValue;
mString = src.mString;
}
此时拷贝构造函数体中为数据成员赋值时，调用的是赋值操作符，而不是数据成员的拷贝构造函数。


