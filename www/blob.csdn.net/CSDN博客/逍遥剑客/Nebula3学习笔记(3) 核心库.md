# Nebula3学习笔记(3): 核心库 - 逍遥剑客 - CSDN博客
2008年07月03日 14:49:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3630
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
**核心子系统**
核心库(Core namespace)实现了这些特性:
- 一个实现了引用计数的RefCounted基类
- 一个运行时类型信息系统(RTTI) 
- 一个模板智能指针, 用于处理RefCounted对象的生命周期 
- 一个由类名创建C++对象实例的工厂机制 
- 一个中央Server对象用于建立基本的Nebula3运行环境
**对象模型**
Nebula3在C++对象模型的基础之上实现了下面这些新特性:
- 基于引用计数和智能指针的生命周期管理 
- 基于类名或四字符编码的对象创建 
- 一个运行时类型信息系统 
**实现一个新的Nebula3类**
当实现一个新的类时首先要考虑它是一个传统的C++类还是要从Core::RefCounted继承. 以下几点可以帮你找到答案:
- 如果这个类需要使用Nebula3的扩展对象特性, 如引用计数, RTTI等, 则它必须从Core::RefCounted继承. 
- 如果这个类是一个典型的小工具类, 如动态数组, 数学向量, 或其它相似的东西, 那么它从Core::RefCounted 继承也没有什么意义.
从Core::RefCounted类继承有一些限制:
- RefCounted派生类不应该在栈上创建对象, 因为栈对象的生命周期是由C++来管理的(他们会在离开当前上下文时被销毁, 从而绕过了Nebula3的引用计数生命周期 管理) 
- RefCounted的派生类只有一个默认的构造函数. 
- RefCounted的派生类必须有一个虚析构函数. 
- RefCounted的派生类不能进行拷贝, 因为这样会造成引用计数机制混乱.
要使用Nebula3的对象模型特性, 除了需要从Core::RefCounted继承外, 还需要在头文件新类的声明中进行额外的标注:
一个标准的RefCounted派生类一般这样声明:
 1: namespace MyNamespace
 2: **{**
 3: **class** MyClass :**public** Core::RefCounted
 4: **{**
 5: DeclareClass**(**MyClass**);**
 6: **public**:
 7: */// constructor*
 8:     MyClass**();**
 9: */// destructor*
10: **virtual**~MyClass**();**
11: **.**..
12: **};**
13: RegisterClass**(**MyClass**);**
注意DeclareClass()宏, 构造函数, 析构函数还有类外面的RegisterClass()宏. DeclareClass()宏加入了RTTI和工厂机制所需的最小代价的信息, 它隐藏了Nebula3的对象模型, 希望可以在不影响已有类的基础进上进行内部机制的变更. RegisterClass()宏是可选的, 它把当前类在中央工厂进行注册. 如果你知道这个类永远不会由类名或四字符编码进行创建, 这个宏可以省略. 
在这个类的.cpp文件里需要包含Nebula3特有的信息:
1: namespace MyNamespace
2: **{**
3: ImplementClass**(**MyNamespace::MyClass**,**'MYCL'**,** Core::RefCounted**);**
4: 
5: **}**
ImplementClass()宏注册类的RTTI机制, 第一个参数描述了类的名字(注意命名空间必须包含). 第二个参数是类的四字符编码, 它必须是所有类中唯一的(如果有重复, 你会在启动程序时得到一个错误提示). 第三个参数是父类的名字, 用于RTTI系统去构造类的关系树.
**引用计数和智能指针**
Nebula3使用传统的引用计数来管理对象的生命周期. 一个模板智能指针类Ptr<>对程序员隐藏了引用计数的实现细节. 一般来说, 应该一直使用智能指针指向RefCounted的派生对象, 除非你能肯定在给出的代码块中这个对象的引用计数不会发生变化. 
智能指针相对于一般指针有很多好处:
- 访问一个空指针会给你一个断言警告而不是一个内存错误 
- 你不需要对引用计数的对象调用AddRef()或Release() (事实上如果你调了, 会了发生严重的错误) 
- 智能指针可以在容器类里良好地工作, 一个智能指针的数组会消除所有的一般指针需要的生命周期管理, 你永远不需要考虑去释放指针所指针的对象, 数组包含的像是真正的C++对象一样 
- 用智能指针不需要考虑指针的所属, 不需要为谁delete对象而烦恼
智能指针也有一些缺点:
- 性能: 拷贝和赋值会引起对象的引用计数的变化, 解除引用会引起指针的断言检查. 这导致的性能消耗一般是可以忽略的, 但是你最好保证它不在内部循环中发生. 
- 应该销毁的对象还存在: 因为智能指针管理的对象只有在最后一个引用放弃时才会销毁, 这样会使对象存在超过预订的时间. 这经常会导致一个BUG的产生. 不过引用计数泄露(程序退出时还仍然存在的对象)时Nebula3会提醒你. 
**创建Nebula3对象**
从Core::RefCounted继承的类可以通过3种不同的方式进行创建:
直接通过静态的Create方法:
1: Ptr<MyClass> myObj = MyClass::Create**();**
静态的Create()方法是之前提到的DeclareClass()宏加入的, 相对于new操作符来说, 它并没有多做什么. 注意正确使用智能指针来保存新建的对象. 
另一种创建方式是通过类名:
1: using namespace Core**;**
2: Ptr<MyClass> myObj =**(**MyClass***)**Factory::Instance**()**->Create**(**"MyNamespace::MyClass"**);**
当你在运行时通过类名来创建十分有用, 特别是对象的反序列化和脚本接口的使用. 注意类型转换是必须的, 因为工厂的Creat()方法返回的是RefCounted指针. 
由类名创建的变种是根据四字符编码进行创建:
1: using namespace Core**;**
2: using namespace Util**;**
3: Ptr<MyClass> myObj =**(**MyClass***)** Factory::Instance**()**->Create**(**FourCC**(**'MYCL'**));**
这个方法看上去没有那个直观, 但是它比类名创建快得多. 并且四字符编码比类名占用的空间更少, 这更利于对象写入二进制流或从中读取. 
**运行时类型信息系统**
Nebula3的RTTI系统可以让你在运行时访问对象的类型, 检查一个对象是不是某个类的实例, 或者某个派生类的实例. 你也可以直接获得一个对象的类名和四字符编码. 所有这些功能是由DeclareClass() 和 ImplementClass() 宏在背后实现的. 
这时有示例程序:
 1:     using namespace Util**;**
 2:     using namespace Core**;**
 3: 
 4: *// check whether an object is instance of a specific class*
 5: **if****(**myObj->IsInstanceOf**(**MyClass::RTTI**))**
 6: **{**
 7: *// it's a MyClass object*
 8: **}**
 9: 
10: *// check whether an object is instance of a derived class*
11: **if****(**myObj->IsA**(**RefCounted::RTTI**))**
12: **{**
13: *// it's a RefCounted instance or some RefCounted-derived instance*
14: **}**
15: 
16: *// get the class name of my object, this yields "MyNamespace::MyClass"*
17: **const** String& className = myObj->GetClassName**();**
18: 
19: *// get the fourcc class identifier of my object, this yields 'MYCL'*
20: **const** FourCC& fourcc = myObj->GetClassFourCC**();**
你也可以向中央工厂查询一个类是否已经注册:
 1:     using namespace Core**;**
 2: 
 3: *// check if a class has been registered by class name*
 4: **if****(**Factory::Instance**()**->ClassExists**(**"MyNamespace::MyClass"**))**
 5: **{**
 6: *// yep, the class exists*
 7: **}**
 8: 
 9: *// check if a class has been registered by class fourcc code*
10: **if****(**Factory::Instance**()**->ClassExists**(**FourCC**(**'MYCL'**)))**
11: **{**
12: *// yep, the class exists*
13: **}**
**Nebula3单件**
很多Nebula3的核心对象都是单件, 就是只存在一个实例, 并且所有其它对象都知道它.
你可以通过静态方法Instance()来访问单件, 它返回唯一实例的一个指针. 返回的指针保证是合法的. 如果在调用Instance()方法时对象实例不存在, 一个断点会被抛出:
1: *// obtain a pointer to the Core::Server singleton*
2:     Ptr<Core::Server> coreServer = Core::Server::Instance**();**
你也可以检查单件是否存在:
1: *// does the Core::Server object exist?*
2: **if****(**Core::Server::HasInstance**())**
3: **{**
4: *// yep, the core server exists*
5: **}**
Nebula3提供了一些辅助的宏来实现单件:
 1: *// declare a singleton class*
 2: **class** MySingletonClass :**public** Core::RefCounted
 3: **{**
 4: DeclareClass**(**MySingletonClass**);**
 5: DeclareSingleton**(**MySingletonClass**);**
 6: **public**:
 7: */// constructor*
 8:     MySingletonClass**();**
 9: */// destructor*
10: **virtual**~MySingletonClass**();**
11: **.**..
12: **};**
13: 
14: *// implement the singleton class*
15: ImplementClass**(**MyNamespace::MySingletonClass**,**'MYSC'**,** Core::RefCounted**);**
16: ImplementSingleton**(**MyNamespace::MySingletonClass**);**
17: 
18: *//------------------------------------------------------------------------------*
19: */***
20: *    Implements the Singleton constructor.*
21: **/*
22: MySingletonClass::MySingletonClass**()**
23: **{**
24: ConstructSingleton**;**
25: **}**
26: 
27: *//------------------------------------------------------------------------------*
28: */***
29: *    Implements the Singleton destructor.*
30: **/*
31: MySingletonClass:~MySingletonClass**()**
32: **{**
33: DestructSingleton**;**
34: **}**
DeclareSingleton()和ImplementSingleton()宏跟DeclareClass()和ImplementClass()宏差不多.它们在类中添加了一些静态方法(也就是Instance()和HasInstance()). 类的构造函数和析构函数必须包含**ConstructSingleton**和**DestructSingleton**宏. ContructSingleton初始化了一个私有的单件指针并保证没有其它的类实例存在(如果不是, 会抛出断言). DestructSingleton让私有的单件指针无效化. 
单件的访问默认是只有本地线程. 这意味着在一个线程中创建的单件无法被其他线程访问. 这使得”并行Nebula”大大简化了多线程编程. “并行Nebula”的基本思想是, 一个典型的Nebula3应用程序包含一些”Fat线程”, 每一个Fat线程都是运行在一个单独的CPU核心上. Fat线程可以用于实现异步IO, 渲染, 物理等等. 每一个Fat线程都初始化了它们自己的Nebula3运行环境, 它们执行特性任务所需的最少依赖. 这基本上消除了大部分Nebula3代码的同步问题, 并且把线程相关的代码集中到一个明确定义的代码区域中. “并行Nebula”的另一个好处就是, 程序员在多线程环境中编程时不需要关心太多. 大多数Nebula3代码看起来就像单线程代码一样, 但是它们却运行在各自的Fat线程中. 
**性能与内存占用的考虑**
Nebula3核心层的一个设计目标就是减少底层代码的内存占用, 来更好的适应微型平台, 像手持设备. 这里有一些已经完成的目标:
- RefCounted 类在每个实例中只增加了4byte用于引用计数. 
- RTTI机制在开头增加了30 到 60 byte, 但是这是对于每个类来说的, 而是不是每个实例. 
- 一个智能指针仅仅4 byte, 就像普通指针一样. 
- 一些监控结构只会在debug模型下创建，　特别是用来检测引擎计数泄露的RefCountedList.
这里一些用三种不种的创建方法创建一百万个RefCounted 对象所需的时间信息. 这些时间信息是在台Intel Pentium 800 MHz的笔记本上得出的.  
- Create(): 0.29 seconds 
- FourCC: 0.65 seconds 
- 类名: 1.45 seconds 
