# Effective C++：类与函数的设计和申明 - 文章 - 伯乐在线
来源：[陈太汉](http://www.cnblogs.com/hlxs/archive/2012/07/22/2603538.html)
在看《Effective C++》这本书的过程中，我无数次的发出感叹，写得太好了，句句一针见血，直接说到点上。所以决定把这本书的内容加上自己的理解写成5篇博客，我觉得不管你是否理解这些条款，都值得你先记下来。下面的索引对应的是书中的章节。
**18：努力让接口完美且最小化**
**19：区分member functions，non-member functions和friend functions三者**
**20：避免将data member放在公开接口中**
**21：尽量使用const**
**22：尽量使用 pass-by-refernece,少用pass-by-value**
![《Effective C++》构造函数析构函数Assignment运算符](http://jbcdn2.b0.upaiyun.com/2012/07/Effective-C++.jpg)
**18：努力让接口完美且最小化**
为了客户端的方便调用，接口中可能会定义很多方法，而其中可能右很多方法是多余或是重复的，这样会导致接口中方法太多，让用户迷失在一堆的方法中，而且大型接口不易维护，长长的class定义导致头文件很长，会增大编译的时间。但是也不必太过吝啬方法的个数，如果加入一个member function会是class更好用，会是增加一个member function能减少客户端的错误，那都是这些方法都是成为接口一份子的理由。
**19：区分member functions，non-member functions和friend functions三者**
member function可以是虚函数而non-member function不可以，如果一个函数必须是动态绑定的那么他就必须是虚函数，就必须是memberfunction，虚函数能实现动态绑定是因为子类可以根据自己的需要重写父类的虚方法实现动态绑定，而non-member function不可能被重写。Friend function是独立于class的，他只是可以访问class的私有成员，如果一个方法不需要访问一个class的私有成员，就不应该让这个方法称为这个类的friend function。

C++
```
class Rational
{
   public:
       Rational(int numerator=0,int denominator=1);
       int numerator()const;
       int denominator() const;
       const Rational operator*(const Rational& rhs)const;
  private:
        ...
};
```
上面这个类表示一个分数，分数的加减乘除的方法都没有提供，那我们该以什么样的方式实现这些操作呢，是member function还是non-member function还是friend function呢？
第一直觉就是这些操作是属于Rational的应该是member function，那么我们就新增一个关于乘法的public member function，就是下面这个样子：
const Rational operator*(const Rational& rhs)const;
简单的介绍为什么是这个样子。先解释3个const，第一个const表示方法的返回为const，就是禁止我们对一个乘法赋值，如禁止a*b=3；第二个const表示在这个方法中不能修改rhs中任何成员的值，第三个const表示这个方法是const方法，在这个方法中不能修改调用这个方法的对象的数据成员。Const还有其他很多作用将会在下一个条款中介绍。
返回值为什么是by value？首先我们必须用一个变量来存乘法的结果值，我们不能在方法中构造一个局部变量，然后返回他的引用，因为这个方法执行完后，局部变量会被自动回收。
参数为什么为引用类型？一句话尽量用by reference代替by value，条款22专门讲述这个问题。这个返回值返回使用by value是没有其他办法了，你必须用一个变量来存放结果值。有了这个方法我们就可以进行乘法操作了。
Rational oneHalf(1,2),twoFive(2,5);
Rational result=oneHalf*twoFive;//没有问题
Result=oneHalf*3;//没有问题，在类型不匹配的时候编译器会一直寻找隐式类型转换的方法，直到找不到报错，由于构造函数的两个参数都有默认值，所以可以发生隐式类型转换，3相当于Rational(3,1),于是不会出现任何问题。
乘法的交换律告诉我们：a*b=b*a;于是我想oneHalf*3可以写成3*oneHalf，但是对不起不行，
3*oneHalf相当于3.operator(oneHalf)，在这个3是操作对象，不会发生任何类型转换，而oneHalf是参数，于是编译器寻找将Rational转换成int（假设3为int类型）的方法，当然是没有啦。为了实现Rational和int类型的任意操作用member function是不可能啦，于是用non-member function，于是写下乘法的方法如下：


C++
```
const Rational operator*(const Rational& lhs,const Rational& rhs)
{
    return Rational(lhs.numerator()*rhs.numerator(),lhs.denominator()*rhs. denominator());
}
```
Non-member function当然不需要用cosnt修饰，将乘法结果存在一个匿名变量中，如果编译有优化的话，这个结果将直接存在接收这个方法的变量中。有了这个方法，3*oneHalf就没有任何问题了，现在3被隐式转换成Rational(3,1)了，不会出现任何问题，如果你不想出现隐式类型转换，就在构造函数的前面加上explicit。最后要考虑的是是否需要将这个方法称为Rational的friend function，当然是不需要，因为让他称为friend function没有任何帮助，多一个没有任何帮助的朋友有这个必要吗？
**20：避免将data member放在公开接口中**
将data member设为private，然后用member function实现读写操作，对于用过面向对象语言的朋友都知道，就不废话了。如果这些方法只是返回data member的话，可以让这些方法称为inline，就可以节省方法调用带来的性能损失。
**21：尽量使用const**
关于这一条上面也提到一些。
Const修饰方法的返回值表示不可以直接对这个方法进行赋值；
Const修饰方法表示在这个方法中不能修改data member；
Const修饰参数表示这个参数在这个方法中不能修改；
常量性的不同也可以实现方法的重载，常量对象只能调用对应的常量方法。非常量对象可以调用常量方法。
**22：尽量使用 pass-by-refernece,少用pass-by-value**
先看一个类，然后看两个方法的对比，其他的废话就多说了，因为C#引用类型默认的是pass-by-reference，而C++任何类型默认的都是pass-by-value。

C++
```
class DataItem
{
public:
DataItem()
       {
           cout<<" constructor DataItem"<<endl;
       }
       ~DataItem()
       {
            cout<<" ~destructor DataItem"<<endl;
        }
        DataItem(const DataItem& item)
        {
            cout<<" constructor DataItem"<<endl;
            value=item.value;
            text=item.text;
            //*this=item;//这句的作用等同于上面两句，但是它会调用operator= ，就多了一次方法调用 
        }
        const DataItem& operator=(const DataItem& item)
        {
            cout<<" operator= DataItem"<<endl;
            text=item.text;
            value=item.value;
            return *this;
        }
        DataItem* operator&()
        {
            return this;
        }
        const DataItem* operator&()const
        {
            return this;
       }
        int GetValue()
        {
           return value;
       }
       void SetValue(int val)
       {
           value=val;
       }
string& GetText()
        {
            return *text;
        }
void SetText(string* txt)
        {
            text=txt;
        }
    private : 
        int value;
        string* text;
};
```
两个对比方法及测试代码：

C++
```
DataItem getDataItemByValue(DataItem item)
{
    return item;
}
const DataItem& getDataItemByReference(const DataItem& item)
{
    return item;
}
void TestDataItem()
{ 
     DataItem item; 
     cout<<"getDataItemByValue start:"<<endl;
     getDataItemByValue(item);
     cout<<"getDataItemByValue end"<<endl;
     cout<<endl;
     cout<<"getDataItemByReference start:"<<endl;
     getDataItemByReference(item);
     cout<<"getDataItemByReference end"<<endl; 
}
```
结果截图：
[](http://jbcdn2.b0.upaiyun.com/2012/07/Effective-C++.jpg)![类与函数的设计和申明](http://jbcdn2.b0.upaiyun.com/2012/07/Classes-and-functions-and-affirm.jpg)
从结果中我们看到pass-by-value多调用两次构造函数，两次析构函数，还有对象的数据成员的构造和析构，损失的确是很惨重。
