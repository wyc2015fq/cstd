
# C++学习之三、探索继承技术 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月27日 21:42:59[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1235标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[扩展																](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=c&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
																								](https://so.csdn.net/so/search/s.do?q=c&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=class&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
**探索继承技术**
**本文是基于大家已经知道继承技术的基础上强化一些知识**
**继承的客户视图：**
Super
↑     Sub类型的对象也是Super类型的对象，因为Sub是从Super继承而来的。
Sub
指向对象的指针或者引用可以引用所声明类的对象或其任何子类对象。比如，指向Super的指针可以实际上指向Sub对象，对于引用也是这样。客户代码仍然只访问Super中的方法和数据成员，但是通过这种机制，对Super进行操作的代码也可以对Sub进行操作。
Super *SuperPoint = new Sub();
**继承的子类视图：**
子类（public继承）可以访问超类的public和protected方法和数据成员。
**覆盖方法：**
超类中只有声明为virtual的方法才能被子类正确覆盖。
一个好的经验就是所有方法都声明为virtual（构造函数除外），这样就不需要担心覆盖方法是否有效了。唯一的不足就是牺牲了性能。
语法：在子类的定义中重新声明。在子类的实现文件中，重新定义。超类和子类中，此函数的定义之前都不需要加上virtual关键字，在类定义的声明处加上就可以了。
如果子类中不想继续被子类的子类中的方法覆盖，在子类中方法的声明处就不需要加上virtual关键字了，但经验就是，最好加上，以备子类继续被扩展。
如：
class Super
{
public：
...code...
virtual void someMethod();
...code...
}；
void Super:: someMethod()
{
cout<<”Super’s method.”<<endl;
}
class Sub:public Super
{
public：
...code...
virtual void someMethod();
virtual void otherSomeMethod();
...code...
};
void Sub:: someMethod();
{
cout<<”Sub’s method.”<<endl;
}

**覆盖方法的客户视图：**
经过前面的覆盖之后，对于someMethod()方法，Super与Sub对象都可以调用，只是行为有所不同。对于指针或引用可以引用类的对象或任意子类的对象。对象自身知道自己实际是哪个类的对象，所以只要方法用virtual声明，就会调用正确的方法。
考虑下面：
Super mySuper；
Sub mySub；
Super *superPoint = &mySub；
Super &ref = mySub；
Super obj = mySub;
mySuper.someMethod();
mySub.someMethod();
superPoint->someMethod();
ref.someMethod();
obj.someMethod();
依次输出为：
Super’s method.”
Sub’s method.
Sub’s method.
Sub’s method.
Super’s method.”
注意：即使超类的指针和引用知道实际上它是一个子类对象，也不能调用在超类中未定义的子类方法或者成员。
对于：
ref.otherSomeMethod(); //bug
对于非指针、非引用的对象，它不知道自己到底是哪个类的对象，这样，下面的aObj对象就会失去子类中的一些知识。
Sub mySub；
Super aObj = mySub；
aObj.someMethod();
输出：
Super’s method.”
综上：就相当于把Super对象看做一个盒子，Sub看做另一个更大的盒子（因为子类添加了一些自己的内容）。在使用Sub的引用或指针时，盒子不会改变，只是采用一种新的办法来访问。然而，在把Sub强制转换为Super时，就会扔掉一些Sub自己独有的内容，才能把它放进一个较小的盒子中。
**考虑父类：**
**1． 父构造函数：**
C++定义的对象的创造顺序为：
a． 如果有的话，首先构造基类。
b． 非static数据成员按照声明顺序构造。
c． 执行构造函数。
注意其父类构造函数是系统自动调用的。如果其父类存在默认构造函数，C++会自动调用。如果父类没有默认构造函数，或者尽管有，但是希望使用另外一个构造函数，则可以把构造函数用链串起来，就像初始化列表中初始化数据成员一样。
如：
class Super
{
public：
Super(int i);
}；
class Sub:public Super
{
public:
Sub();
};
Sub::Sub():Super(7)
{
...code...
}
如果向父类构造函数传递自己的数据成员作为参数则是不行的，因为先调用父类构造函数，后再初始化自己的数据成员，如果这样，则传递的数据成员是未初始化的。
**2． 父析构函数**
因为析构函数不能包含参数，所以C++会给父类自动调用其析构函数。撤销的顺序与构造顺序恰好相反。
a． 调用析构函数体
b． 按照构造的逆序删除数据成员
c． 如果有父类，析构父类
注意，作为经验，所有析构函数都应该使用virtual关键字声明。如果不这样，就可能会发生错误。考虑：如果代码可能对一个超类指针调用删除操作，但这个超类指针实际指向一个子类对象，那么析构链的开始位置就不对了。
**3． 引用父类的数据**
在子类中函数和数据成员的名称可能会产生二义性，进行多重继承尤其如此。C++提供了一种机制来消除二个类之间的名字二义性：作用域解析操作符。
在子类中覆盖方法时，实际上是替换其他代码感兴趣的原始代码？不过父类中的方法依然存在，可能还会用到。在子类中的方法要调用父类中的方法（该方法子类覆盖了），则需加上父类名加上作用域解析操作符。
如：
class Super
{
public：
...code...
virtual string doSomething(){ return “Super”;}
...code...
}；
class Sub:public Super
{
public：
...code...
virtual string doSomething(){ return “Sub”+ Super:: doSomething() ;}
virtual void otherSomeMethod();
...code...
};
**4． 向上类型强制转换和向下类型强制转换**
**向上类型强制转换：**
下面会造成切割
Super mySuper = mySub；
下面就不会发生切割了
Super *superPoint = &mySub；
Super &ref = mySub；
总结：进行向上类型强制转换时，要使用指向超类的指针或引用来避免切割问题。
**向下类型强制转换：**
考虑下面：
void presumptuous（Super* inSuper）
{
Sub * mySub = static_cast<Sub*>(inSuper);
...other code...
}
如果编写此方法的人来调用这个函数，可能没什么问题，因为他知道此函数希望参数类型为Sub*。单数如果别人来调用，就会给它传递一个Super*，并不会完成编译时检查来对参数进行类型转换，函数会盲目地假定inSuper实际上是指向Sub对象的指针。
向下类型转换有时是必须的，在可控的环境中可以有效使用向下类型强制转换。此时应该利用dynamic_cast,它使用该类型对象的内置知识来防止无意义的类型转换。如果对指针不能进行动态类型转换，指针值为NULL而不是指向无意义的数据。如果未能对对象的引用使用dynamic_cast，则会抛出std：：bad_cast异常。
总结：只有在必要并且保证使用动态类型转换的时候使用向下类型强制转换。
**继承以实现多态**
**纯虚方法与抽象基类**：
纯虚方法是指在类定义中显式未定义的方法。包含纯虚方法的类称为抽象类，抽象类不能实例化,但是仍然可以使用抽象类类型的指针和引用。
语法：virtual string getString() const = 0;
如果二个要实现二个兄弟类的互相转换，可以在兄弟类中增加一个类型构造函数，它看起来和复制构造函数很像，但是复制构造函数引用同一类型的对象，而类型构造函数引用的是兄弟类的对象。
如：
class SpreadsheetCell
{code};
class StringSpreadsheetCell:public SpreadsheetCell
{
public:
StringSpreadsheetCell();
StringSpreadsheetCell(const DoubleSpreadsheetCell& inDoubelCell);
};
class DoubleSpreadsheetCell:public SpreadsheetCell
{code};
使用类型构造函数，给定DoubleSpreadsheetCell很容易构造StringSpreadsheetCell。但是不要被类型转换所迷惑。从一个兄弟类转换到另一个兄弟类是不行的。除非重载类型转换操作符。
这样实现二个StringSpreadsheetCel、一个StringSpreadsheetCell
一个DoubleSpreadsheetCell、二个DoubelSpreadsheetCel相加就可以写一个公共的operator+重载了。
const StringSpreadsheetCell operator+( const StringSpreadsheetCell& lhs, const StringSpreadsheetCell& rhs)
{code}
**多重继承：**
**从多个类中继承：**
class A{code}；
class B{code}；
class C：public A，public B{}；
C对象支持A和B中所有public方法和数据成员。
类C的方法可以访问A和B中的protected数据和方法。
C对象可以转换为A和B对象。
创建C对象时，会自动调用A和B的默认构造函数，调用顺序在类定义中这二个类的列出顺序。
撤销C对象时，会自动调用A和B的析构函数，调用顺序与类定义中这二个类的列出顺序相反。
**命名冲突与二义基类:**
**名字二义性：**
如果A和B都有一个public eat()方法，一旦C对象调用eat()方法，就会产生二义性（也可是是同名的数据成员）。
解决此二义性：
1． static_cast<A>(myC.eat()); //向上转换产生切割调用A的eat()方法
2． myC.A::eat(); //使用作用域解析操作符调用A的eat()方法
引起二义性问题的另外一种原因就是：从同一个类继承二次。
class A{code}；
class B：public A{code}；
class C：public A，public B{}；
**二义基类：**
多个父类自身有共同的父类。
使用这种继承体系，最好是使上层的共同父类成为抽象基类，所有方法都声明为纯虚方法。


