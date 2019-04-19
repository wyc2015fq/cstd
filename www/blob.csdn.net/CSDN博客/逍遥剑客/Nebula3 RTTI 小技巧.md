# Nebula3 RTTI 小技巧 - 逍遥剑客 - CSDN博客
2009年06月16日 23:59:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2562
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
注意: 以下代码省略了命名空间前缀和”using namespace xxx”声明来改善可读性. 另外, 因为我没有通过编译器运行这些代码, 不保证有些手误.
**不要把****Rtti****跟****RTTI****搞混****:**
Rtti 是类名, MyClass::RTTI 是类的Rtti对象的名字. 每个RefCounted派生的类都有一个Core::Rtti的静态实例, 它在main()之前初始化.
**检查一个对象是否为特定类或者其派生类****:**
这是Nebula3 RTTI系统的标准特性, 可以检查一个对象是否可以安全地转换为特定类接口: 
// check whether obj is instance of a specific class: 
if (obj->IsInstanceOf(MyClass::RTTI)) …
// check whether obj is instance of class, or a derived class: 
if (obj->IsA(MyClass::RTTI))…
与Nebula2相比, N3的RTTI检查是非常快的(N2中, 这需要先把一个类名字符串转换成一个指针). 在N3中, RTTI检查只是简单的指针比较. IsA()在类不匹配时可能会慢些, 因为它需要在继承树中遍历到根部. 这样一来, 最好使用IsInstanceOf()方法, 因为它只是一个指针比较.
尽管这两个方法都有类名和类四字符码(fourcc)的版本, 显然它们比直接使用RTTI对象要慢:
**if (obj->IsInstanceOf(“MyNamespace::MyClass”)) … if (obj->IsInstanceOf(FourCC(‘MYCL’))…if (obj->IsA(“MyNamespace::MyClass”))… if (obj->IsA(FourCC(‘MYCL’))…**
**使用****Ptr<> cast ****方法来进行安全的转换****:**
Ptr<> 类有3个转换方法, 其中2个是安全的向上和向下转换, 还有一个不安全但快速的C风格的强制转换. 要进行向下转换(从一父类转换为特定子类)可以这么做:
// assume that res is a Ptr<Resource>, and safely down-cast 
// it to a Ptr<D3D9Texture> (D3D9Texture is a subclass of Resource): 
const Ptr<D3D9Texture>& d3d9Tex = res.downcast<D3D9Texture>();
如果tex不是D3D9Texture对象的话, 这会产生一个运行时错误.
安全的向上转换也差不多:
const Ptr<Resource>& res = d3d9Tex.upcast<Resource>();
一个不安全的C风格转换是这样的:
const Ptr<Resource>& res = d3d9Tex.cast<Resource>();
不安全的转换是最快的(在release模式中, 编译器会把这个调用优化掉), 但是很显然它会让你自作自受. 那两个安全的转换方法会调用Rtti::IsDerivedFrom()方法, 而且不会有临时的Ptr<>对象产生, 因为返回的是一个 const引用.
**直接查询****RTTI ****对象****:**
你可以在没有实际对象的情况下直接查询许多类属性:
// get the name of a class: 
const String& className = MyClass::RTTI.GetName();
// get the FourCC identifier of aclass: 
FourCC classFourCC = MyClass::RTTI.GetFourCC();
// get a pointer to the Rtti object of the parent class 
// (returns 0 when called on RefCounted::RTTI) 
Rtti* parentRtti = MyClass::RTTI.GetParent();
// check if a class is derived from this class: 
// by Rtti object: 
if (MyClass::RTTI.IsDerivedFrom(OtherClass::RTTI)) … 
// by class name: 
if (MyClass::RTTI.IsDerivedFrom(“MyNamespace::OtherClass”)) … 
// by class fourcc: 
if (MyClass::RTTI.IsDerivedFrom(FourCC(‘OTHR’))…
你可以检查两个Rtti对象是否相等:
const Rtti& otherRtti = …; 
if (MyClass::RTTI == otherRtti)… 
if (MyClass::RTTI != otherRtti)…
由于保证了每个类只有一个Rtti对象, 所以这个比较跟比较2个Rtti对象的指针是等价的(事实上相等和不等运算符就是这么干的). 
**直接从****RTTI****对象创建实例****:**
Ptr<MyClass> myObj = (MyClass*) MyClass::RTTI.Create();
这个过时的C风格转换看起来格格不入, 但在这里是必须的, 因为Rtti::Create()方法只是一个普通指针, 而不是一个智能指针.
通过RTTI对象创建实例而不是MyClass::Create()静态方法对于把对象类型做为方法调用参数时非常有用:
Ptr<RefCounted> CreateObjectOfAnyClass(const Rtti& rtti) 
{ 
    return rtti.Create(); 
}
这比其它两种创建方法要快: 通过类名创建和类的四字符码(fourcc)创建.
**通过类名或****FourCC****标识符创建对象**
你可以用Core::Factory 单件以类名和FourCC标识符创建从RefCounted派生的对象:
Ptr<MyClass> obj = (MyClass*) Factory::Instance()->Create(“MyNamespace::MyClass”);
Ptr<MyClass> obj = (MyClass*) Factory::Instance()->Create(FourCC(‘MYCL’));
这主要用于序列化代码, 或者对象类型需要经过网络连接进行转达.
**用****Core::Factory****单件查找类的****RTTI****对象**
你可以通过类名或类的FourCC标识符来获得RTTI静态对象的指针:
const Rtti* rtti = Factory::Instance()->GetClassRtti(“MyNamespace::MyClass”);
const Rtti* rtti = Factory::Instance()->GetClassRtti(FourCC(‘MYCL’));
如果类不存在, 调用会失败. 你可以用ClassExsits()方法检查类是否已经注册给工厂(factory):
**bool classExists = Factory::Instance()->ClassExists(“MyNamespace::MyClass”);bool classExists = Factory::Instance()->ClassExists(FourCC(‘MYCL’));**
**疑难解答**
通常在使用Nebula3的RTTI系统时有两个问题.
当编写一个新类时, 可能会发生FourCC已经被占用的情况. 这时, 启动程序时会弹出一个这样的错误对话框:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090616/N3_Class_FourCC.JPG)
修正冲突的办法是改变其中的一个FourCC代码并重新编译.
另一个问题就是有些类没有在程序启动时注册, 因为它的静态RTTI对象的构造方法被链接器(linker)给”优化没了”. 这通常是因为没有实际的C++代码直接用到这个类. 例如一个对象只通过类名(或FourCC)进行创建并且只通过虚函数进行访问. 
这种条件下, 链接器会完全地抛弃这个类的.obj单元, 因为外部没有调用到这个对象单元. 这对于减于可执行文件的体积很有效, 并且跟C++的静态对象模型工作得很好. 但是对于Nebula3的动态对象模型来说, 我们需要欺骗链接器来链接”没有使用”的类到可执行文件中去. 幸亏我们不需要为每个RefCounted派生的类做这件事, 只需要针对某些继承树中的类(如渲染层中的ModeNode的子类和ModeNodeInstance的子类, 还有应用程序层中的Property的子类)
为了防止链接器抛弃某个类, 建议进行下面的处理: 
- 在一个.h文件中集中加入__RegisterClass(MyClass)宏
- 在.cc文件中包含这个头文件. 这样就可以保证不被链接器所抛弃了
头文件**/nebula3/code/render/render_classregistry.h**是一个进行集中类注册的典型例子.
