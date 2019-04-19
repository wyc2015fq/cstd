# C# OOP：继承，接口和抽象类 - 文章 - 伯乐在线
原文出处： [老张的菠菜](http://blog.csdn.net/zhangchen124/article/details/51276665)
**继承**
在现实生活中有所谓的“种瓜得瓜、种豆得豆”的说法，在生物学概念上就是“遗传”的概念，在面向对象中“继承”就类似于生物学中的遗传，通过继承，可以更好的实现代码的重用（复用，多次使用的意思）、增加扩展性、简化代码等。
下面是现实生活中的引用：
![1](http://jbcdn2.b0.upaiyun.com/2016/06/28c8edde3d61a0411511d3b1866f06363.jpg)
定义与使用继承
继承一次最常用的就是通过合法程序（比如遗嘱）从别人那里接受财产或爵位等，有点类似于世袭制。
另外一个含义，是人从父母那里继承一些特征，如果孩子从父母继承的身高、相貌等特征。那么在什么情况下使用继承，以及在程序中应该如何定义呢？
首先,继承一般使用在类之间有一定的层次关系，即一个类已经包含了其他类的一些属性与方法，比如，雇员类包含了技术员工、客服员工、软件支持、经理等的属性和方法，这时，我们把雇员类成为父类，技术员工等类成为子类，子类可以通过继承机制来使用父类的属性和方法。
概括来说：继承需要is a的关系（技术员工 is a 雇员）
在C#中，使用“：”来表示一个类继承自另外一个类。格式如下：

C#
```
class   技术员工：雇员
{
//技术员工的属性定义
// 技术员工的方法定义
}
```
现在需要你为一家卡通影视公司开发商业软件，其中涉及一个会说话的鸭子（比如唐老鸭），在此之前已经有了，普通鸭子类了。代码如下：

C#
```
Class Duck
{
private    string name;
public string Name
{
get{return name;}
set{name=value;}
}
public void Quck()
{
Console.WriteLine(“嘎嘎嘎”);
}
public void Swimming()
{
Console.WriteLine(“游啊游”);
}
}
```
那么，现在唐老鸭会说人话，怎么办呢？同时还会“嘎嘎嘎”叫，原来的方式是在Duck类里增加方法

C#
```
public void Speak()
{
Console.WriteLine(“说人话了^.^”);
}
```
然后在Main()方法中调用，使用继承后的方法：

C#
```
class TangDuck:Duck
{
public void Speak()
{
Console.WriteLine(“说人话了^.^”);
}
}
```
同理可以在Main()中进行类的声明与调用

C#
```
static void Main(string[]args)
{
TangDuck tanglaoya=newTangDuck();
tanglaoya.Speak();              //可以实现说话功能
}
```
我们看到类TangDuck在使用继承实现Speak时，在类名后跟了个“：”，冒号在此处的含义为“继承自”的意思。继承特点如下：
1.有助于代码的重用；
2.代码维护和书写都简单得多；
3.父类的可继承数据成员和方法可用于子类；
4.子类可以轻易的获得数据成员和方法；
> 
编码实践：试完成作业1
深入理解继承
 1.访问修饰符在继承中的使用范围
![2](http://jbcdn2.b0.upaiyun.com/2016/06/665f644e43731ff9db3d341da5c827e13.jpg)
3.继承的单根性
![](http://jbcdn2.b0.upaiyun.com/2016/06/525d9469a64c9613a517b3c5b4564ce6.jpeg)
4.继承自上而下是一种逐层具体化的过程，而自下向上是一种逐层抽象化得过程，这种抽象化关系反应为上下层之间的继承关系。例如：最高层的动物具有最普遍的特征，而最底层的人则具有较具体的特征。
> 
项目1：使用访问修饰符深入学习继承
项目背景：实现雇员类对人类的代码重用。
解决方案：学习如何利用继承机制来实现代码重用。
任务1：编写代码

C#
```
using System;
usingSystem.Collections.Generic;
usingSystem.Text;
namespaceExtends
{
class Person
{
//字段
private stringname;
private stringaddress;
private stringage;
private stringemail;
//属性
public stringName
{
get{ return name; }
set{ name = value; }
}
protected stringAddress
{
get{ return address; }
set{ address = value; }
}
public stringAge
{
get{ return age; }
set{ age = value; }
}
internal stringEmail
{
get{ return email; }
set{ email = value; }
}
//方法
public voidShowName()
{
Console.WriteLine("姓名：" + name);
}
protected voidShowAddress()
{
Console.WriteLine("地址：" + address);
}
private voidShowAge()
{
Console.WriteLine("年龄:" + age);
}
internal voidShowEmail()
{
Console.WriteLine("邮件:" + email);
}
//构造函数
public Person()
{
name = "活人";
address = "人世间 ";
age = "1";
email = "haoren@163.com";
}
}
class Employee:Person    //Employee类继承自Person类
{
//字段：部门
private floatdepartment;
//Employee类的方法
public voidShow()
{
Console.WriteLine(Name+ Address + Email);
}
}
class Program
{
static voidMain(string[]args)
{
EmployeetangJun = new Employee();
tangJun.ShowName();
tangJun.Name = "唐峻 ";
tangJun.ShowEmail();
tangJun.Show();
}
}
}
```
任务2：编译程序:
任务3：运行效果
> 
base和this关键字
通过前面的学习，我们知道子类只能继承父类非private的属性和方法，系统在创建对象时必须调用类的构造方法，那么在创建子类对象时父类的构造方法是怎样被调用的呢？示例如下：

C#
```
using System;
namespace ExtendsExam
{
classMyBase
{
publicMyBase()
{
Console.WriteLine(“基类对象被创建”);
}
~MyBase()
{
Console.WriteLine(“基类对象被销毁”);
}
}
class SubClass ：MyBase()
{
public SubClass()
{
Console.WriteLine(“子类对象被创建”);
}
~SubClass()
{
Console.WriteLine(“子类对象被销毁”);
}
}
classProgram
{
static void Main(string[]args)
{
SubClass sc=new SubClass();
}
}
}
```
运行效果：
基类对象被创建
子类对象被创建
子类对象被销毁
基类对象被销毁
通过上述的案例，我们会发现子类自动调用了父类的构造函数和析构函数。如果父类有多个构造函数（即父类构造函数重载的情况），子类如何知道调用父类的那个构造函数呢？此时，用base关键字指定调用父类的具体构造函数就比较有用。
> 
base的使用：
n       调用父类的属性和方法

C#
```
public voidDoWork()
{
string message = string.Format(
" {0}上班了，时间{1}",
base.Name, base.Time);
}
```
n       调用父类的构造函数

C#
```
publicManager(string name, Genders gender,
int age,string position,int allowance)
:base(name,gender,age)
{
//经理类扩展的属性
this.position = position;
this.allowance = allowance;
}
```
案例：

C#
```
usingSystem;
usingSystem.Collections.Generic;
usingSystem.Text;
namespace Test
{
class Person
{
protected stringname;
public stringName
{
get{ return name; }
}
public Person(string_name)
{
name = _name;
}
public voidShowName()
{
Console.WriteLine("name={0}", name);
}
}
//Person类的子类
class Employee:Person
{
public stringid;
public Employee(stringsname,string sid):base(sname)
{
id = sid;
}
public voidShow()
{
base.ShowName();        //利用base调用方法
Console.WriteLine("name={0}",base.name);  //利用base调用父类字段
Console.WriteLine("id={0}", id);
}
}
class Program
{
static voidMain(string[]args)
{
Employeeemp = new Employee("聂亚龙","MS007");
emp.Show();
}
}
}
```
> 
**this关键字：用来调用本类成员**
通常也可以隐藏this，MSDN总结如下：
限定被相似的名称隐藏的成员，例如：

C#
```
public Employee(string name, string alias)
{
this.name = name;
this.alias = alias;
}
```
将对象作为参数传递到其他方法，例如：
this指代对象本身，用于访问本类的常量、字段、属性和方法，而且不管访问元素师任何访问级别。另外，静态成员不是对象的一部分，因此不能在静态方法中引用this。
抽象类定义及其使用
在理解抽象类和抽象方法之前，我们先来看一下“抽象”是什么意思，汉语词典“抽象”含义如下：
1. 将复杂物体的一个或几个特性抽出去，而只去注意其他特性的行动或过程（主要看是否与系统研究的目标一致）
2. 将几个有区别的物体的共同性质或特性形象地抽取出来或孤立地进行考虑的行动或过程。
3. 抽象对于将东西分类是必需的。
4. 摘要、提炼，抽象化。
从词典解释，我们会发现程序里的抽象理论实际上是在仿效现实中的抽象理论和方法。
**抽象类**：好比一篇摘要，摘要中涉及的很多要点都要在文章的各个部分实现，同样抽象类功能需要子类来实现。
我们在查找某篇文章时往往先看摘要，摘要可以代表这篇文章，也同样通过查看抽象类你可以明晰子类提供的功能，并且抽象类对象也可以代表子类实例行使功能。
**抽象类最重要的三个描述**：
n      抽象类是子类的一个描述
n      抽象类不能自己实例化，但可以代表子类实例
n      抽象类和接口都是用来实现抽象的。
定义如下：

C#
```
访问修饰符  abstract class 类名
{
抽象类体
}
```
抽象类定义示例：

C#
```
publicabstract class Employee
{
protected string nid;
public abstract int GetSalary();
public abstract int Name{get;set;}
}
```
抽象类的定义和普通类的定义非常相似，只是在class和类的访问修饰符中间加了一个abstract关键字。抽象类面可以定义抽象方法、抽象属性等。抽象类只是用来描述功能，所以这些抽象的方法、属性等不需要去实现它，只写个空壳就可以了。
**抽象类的使用**：
抽象类不能自己实例化，需要使用其子类来实例化。那么抽象实例怎样才能代表子类实例行使子类功能呢？C#中提供了一种重写的机制来完成这个功能。重写关键字为override,子类使用该关键字来重写基类的抽象方法、抽象属性等（注意：凡是子类继承了抽象类，就必须重写抽象类里的抽象方法，否则编译出错）。

C#
```
/*抽象类案例*/
usingSystem;
abstractclass BuRuDongWu   //抽象类：哺乳动物类
{
//成员变量
protected int legs;
protected BuRuDongWu(int legs)
{
this.legs=legs;
}
//抽象方法
public abstract void eat();
//Console.WriteLine("我有方法体");
//抽象方法做实现了吗？  抽象方法 体现哺乳动物有一个吃的行为就行了，已经代表了所有子类的吃的行为...
//抽象方法没有方法体（不允许有方法体）;可以常规方法
public int getLegs()  //常规方法,获得腿数.
{
return legs;
}
}
classDog:BuRuDongWu            //狗类继承自抽象类：哺乳动物类
{
public Dog(int legs):base(legs)
{}
//子类必须实现所有的抽象方法,通过重写.
public override void eat()           //注意，重写方法相当于子类实现了父类的抽象方法
{
Console.WriteLine("狗吃肉");
}
}
classCat:BuRuDongWu
{
public Cat(int legs):base(legs)
{}
public override void eat()           //注意，重写方法相当于子类实现了父类的抽象方法
{
Console.WriteLine("猫吃老鼠");
}
}
classTest
{
static void Main(string[]args)
{
//BuRuDongWu  lv=new BuRuDongWu();    实践证明抽象类不能被实例化;
//抽象类必须被子类继承实现，才能体现功能; --->更好的体现,继承特性；
Dog underDog=new Dog(4);
Cat  lanMao=newCat(4);
underDog.eat();
lanMao.eat();
Console.WriteLine("狗有{0}条腿,猫有{1}条腿",underDog.getLegs(),lanMao.getLegs());
}
}
```
为什么要使用父类对象指向子类对象实例呢？留给大家思考。
> 
接口—概念、理解、本质
在讲接口概念之前，首先来让我们看一下现实生活中都有哪些是接口。
![](http://jbcdn2.b0.upaiyun.com/2016/06/0756308b97487b4ad7a713e55c97c8e0.jpeg)
从现实生活中可以知道，接口是一套规范，遵守这个规范就可以实现功能(比如、用U盘来回拷贝东西、用鼠标操作电脑、用摄像头和电脑建立连接)。
例如，将电器的“开”、“关”行为单独定义为一个接口，这样凡是继承自这个接口的电器就都有了“开”、“关”的方法。让电灯、电视、电扇、电冰箱都继承并实现“开关”接口，这样，对于所有电器来说都有了自己的开关，并且可以针对本身做相应开关的定义。
**接口的定义**：

C#
```
访问修饰符  interface 接口名   //名称通常以I开头
{
接口内容
}
```
- 与抽象类一样，接口里面的方法成员也不能有方法体；
- 类可以继承自接口，继承之后必须实现里面的所有方法；
- 接口不能被实例化；
- 类在实现接口里的方法的时，不需要使用override关键字，直接实现即可。
- C#中接口不能包含任何数据成员；
- 接口内的访问修饰符不需要些，默认为public。

C#
```
//接口示例
using System;
interface OnOff  //开关接口
{
void On();
void Off();
}//实现接口的类必须实现接口的所有成员(成员变量、成员方法....)；
class Light:OnOff    //灯
{
public void On()     //Light类继承了OnOff接口，在自己的类中对接口中的2个方法来做实现.
{
Console.WriteLine("电灯亮了");
}
public void Off()
{
Console.WriteLine("电灯关了");
}
}
class TV:OnOff
{
public void On()     //TV类继承了OnOff接口，在自己的类中对接口中的2个方法来做实现.
{
Console.WriteLine("电视亮了");
}
public void Off()
{
Console.WriteLine("电视关了");
}
}
class Test
{
static void Main(string[]args)
{
Light meDeng=new Light();
meDeng.On();
meDeng.Off();
TV meTv=new TV();
meTv.On();
meTv.Off();
}
}
```
接口的使用基本上和抽象类一样，有人把接口比做双节棍中的一节，另外一节就是继承并实现接口的类，只有两节联合起来使用才会发生效力。
另外，在C#中接口是多继承的，接口之间可以互相继承与多继承、普通类或抽象类也可以继承自接口（注意接口不能继承自类或抽象类），一个类可以同时继承自类和多个接口。
u    接口与接口之间
接口与接口之间可以互相继承，继承的规则和类一样，即子接口将获得父接口的内容。

C#
```
interface Irun{void run();}    //跑动
interface Iswim{void swim();}   //游泳
interface Itwo:Ifly,Iswim{}    //同时实现跑动与游泳
class TwoTank:Itwo
{       //自己的两栖坦克
public void run(){…}
public void swim(){…}
}
```
u    接口、抽象类和类

C#
```
interface IA{void playA{};}
interface IB{void playB{};}
abstract class TC
{
public abstract  void playC();
}
class TD:TC,IA,IB
{…}
```
**面向接口的编程**：
设计模式的祖师GoF，有句名言：Program to an interface,not an implementation,表示对接口编程而不要对实现编程，更通俗的说法是对抽象编程而不是对具体编程。
微软的FCL(基础类库)，是微软的一帮牛人已经定义好了的东西，如果你理解了接口，就会发现FCL类库中使用到了很多接口技术。比如数组Array类:
publicabstractclass Array :ICloneable,IList,ICollection,IEnumerable,实现了System.Collections命名空间下的Ienumerable接口，那么我们可以用此数组来操作数组元素：

C#
```
int []a=new int[]{1,2,3,4,5};
System.Collections.Ienumerator it=a.GetEnumerator();
while(it.MoveNext())
{
Console.WriteLine(it.Current.ToString());
}
```
**抽象类与接口的异同：**![](http://jbcdn2.b0.upaiyun.com/2016/06/b678032f5e98b0bf4e4330ca4767ad63.jpeg)
小结：
在本章中，我们主要学习了：
u      继承的定义及其使用
u      什么是抽象类及重写
u      接口的定义与使用
英语词汇：
英文                         中文
Base                       基础的，基类
Derived                 派生的，继承的
Point                      点
Abstract                抽象的
Virtual                    虚的
Animal                   动物
Sealed                   封装的
Method                 方法
Interface                接口
Declare                  声明
Update                  更新
Item                        项
Override                重载
Furniture               家具
Bookshelf              书柜
> 
练习项目：
1请编码实现某软件公司员工的继承关系：
雇员（Animal）具有行为：上班、工作、下班
雇员包括：技术员工，客服、销售
这些员工工作的行为各不相同；但上班、下班的行为是一致的。
