# QT 信号机制 - 三少GG - CSDN博客
2013年01月04日 03:03:59[三少GG](https://me.csdn.net/scut1135)阅读数：613
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
# [详解QT 信号机制 （上篇）](http://mobile.51cto.com/symbian-273544.htm)
**QT 信号机制 **是本文要介绍的内容，**Qt**用预编译器和宏来保证强大的跨平台能力，**信号机制**则是其中最精妙之处。本文分析了几种常见的**信号**处理**机制**，然后详细介绍了**Qt**的Signal/Slot**机制**。
首先要说明，这里所说的**信号**不是Unix中进程间通信的**信号。**这里的**信号**更多地与图形界面的输入输出联系在一起（当然也可以是不可见的操作）。自从计算机程序从字符界面转为图形界面，用户的输入一下子变得繁杂和丰富起来，不同的输入位置、不同的输入设备、不同的焦点位置、不同的输入值组合起来构成了许许多多的**信号**。一下子，这个世界变得五彩缤纷。当前的三大主流操作系统??Windows、Unix和MAC都提供了令人赏心悦目的图形界面。
虽然它们出自不同公司，自身还有很多分支，但是在图形操作与管理上还是大致类同的：都有桌面、有图标，有大大小小规则或不规则的窗口，窗口上有标题、边框、菜单以及按钮等各种控件，用户可以用键盘在当前焦点输入内容，可以用鼠标点击任意的窗口和控件。就能动性来说，是由用户主导程序下一步作何操作，而不象字符时代那样由程序来主导用户。这也就是所谓的“事件驱动”。在一个事件驱动的系统中，不论是Windows，还是Unix，都脱离不了以下的处理框架：
当某个应用程序收到操作系统发送的事件时，它就要判断这个事件该由谁处理。处理过程本身又可能引起新的事件发生，这就要告诉操作系统我发出了什么信号。如此这般循环往复，青山之水常流。那么，一个具体的**信号**究竟是如何触发与它对应的函数呢？绝大部分的系统都是采用了回调的机制，所谓“回调”其实就是指向某一个函数的指针。
在C语言中函数名其实也是一个指针，因此回调其实是一个指向指针的指针。在不同的开发框架或开发包中，对于回调的实现有着一些细微的差别。初接触Qt时，我一直在想它是如何处理各种平台的信号调用。虽然C语言本身是平台无关的，但具体到某一个操作系统、某一个开发包，信号机制会有些不同。
而信号是面向对象的开发环境中一个很重要的环节，如果要设计一个类库或程序框架，就必须很好地考虑不同平台间的差异。接触了Qt之后，感觉Qt选择了一条颇具特色的处理途径??Signals/Slot，中文名暂定为“信号/反应槽”。在Qt的内部设计中，通过信号/反应槽（signals/slot）的使用对回调进行了很好的封装。为了更好地了解该机制我们先看一下其他几种常用的信号相关程序。
1.Win32
Win32的程序总是从WinMain开始执行。在WinMain的代码中，主要功能一般有三个：一是注册窗口类，二是在屏幕上显示窗口，三是实现消息环。消息环的作用就是从应用程序队列中取出操作系统放入的消息，从而实现用户和程序之间的交互（也包括象定时器之类的非用户输入的消息）。应用程序不定期地在消息环中等待消息的到来。如下所示：// 消息环
- while(GetMessage(&msg, NULL, 0, 0))  
- {  
- TranslateMessage(&msg);  
- DispatchMessage(&msg);  
- } 
这一段程序包括了形成一个标准消息环的三个基本API：GetM essage()、TranslateMessage()和ispatchMessage()
，采用加速键和非模式对话框时将相应改变消息环的结构。在Windows中，GetMessage()是多任务的核心。在应用程序的消息队列中出现一条消息之前，该函数并不返回任何东西。GetMessage()的等待阻塞了当前进程，因而为正在运行的其他应用程序提供了检查私有消息环的机会。出现一条消息后，GetMessage()将取出该消息，并将信息存储在一个MSG数据结构中。对于每一条迫使退出消息环、进程终止的消息（WM_QUIT除外），GetMessage()返回TRUE。
通常在消息环后面跟一个返回语句，迫使WinMain()返回系统。紧跟着GetMessage()的TranslateMessage()对msg进行处理并修改该数据块的内容。DispatchMessage()负责查找应调用哪一个窗口过程，这种选择是根据msg中hwnd所标识的窗口进行决策。窗口过程对消息进行处理， 完毕后即返回到消息环， 再次执行GetMessage()。如下图所示：
为了对所关心的消息做出处理，窗口在创建时一定要提供一个消息回调函数，不管该创建过程是显式调用还是其他API函数隐式生成。用户在该回调函数中要对每一个关心的消息做出判断与处理，从C语言的观点来看，一个窗口过程（回调函数）就是这样一个函数：接受四个参数，返回一个LRESULT值，一个switch语句在过程内占用了大量的代码以完成各个行为动作。
2.MFC
虽然直接用Win32 API开发的程序运行效率高、条理分明，但开发起来却较为复杂，维护工时也耗用较多，因此现在Windows环境中大部分用C++开发的应用程序使用了微软提供的MFC类库。它是面向对象设计的，虽然乍一看其编程风格与Win32迥然不同，但那是高度封装的结果，其内部的实现与Win32没有区别。
MFC的一个主导设计思想就是程序框架下（CFrameWnd）的视图/文档模型，同时定义了许多宏来简化编程，其消息的传递也与宏息息相关（有关MFC的解剖可看侯捷先生的《深入浅出MFC》第二版）。通过使用这些宏，应用程序自身将维护这一张可能为数不菲的消息映射表。对于程序员来说，只需要点击鼠标就可完成以上的工作，开发效率有了很大的提高。
3.Linux
Linux（包括其他的Unix）和Windows的一个很大不同点在于其图形界面的管理是与内核分开的，负责图形操作（还包括键盘、鼠标等事件捕获）的模块是X Window。请注意，此处的“Window”与微软的Windows毫无亲戚关系。X Window包括三大部分：服务端（XServer）、客户端（X Client）和协议（X protocol），示意图如下：我们平时在Linux下开发的有图形界面的程序一般就是X Window中的客户端程序，相对应的库就是X Lib。
X Lib是X Window中最低层的接口库，相当于微软Windows中的 API。这个库封装了对X protocol的存取，提供了超过610个函数。由于X protocol可以在网络上传播，因此X Window中服务器端和客户端可以不在一台机器上，这一点和微软Windows有着很大的区别。对比X Lib与Win32 API的处理方式，可以发现虽然两者框架不一、风格不一，但在流程处理上都有异曲同工之妙。
4.Qt
Qt中的类库有接近一半是从基类QObject上继承下来，信号与反应槽（signals/slot）机制就是用来在QObject类或其子类间通讯的方法。作为一种通用的处理机制，信号与反应槽非常灵活，可以携带任意数量的参数，参数的类型也由用户自定。
同时其本身也是类型安全的，任何一个从QObject或其子类继承的用户类都可以使用信号与反应槽。信号的作用如同Windows系统中的消息。在Qt中，对于发出信号的对象来说，它并不知道是谁接收了这个信号。这样的设计可能在某些地方会有些不便，但却杜绝了紧耦合，于总体设计有利。反应槽是用来接收信号的， 但它实际上也是普通的函数，程序员可以象调用普通函数一样来调用反应槽。与信号类似的是，反应槽的拥有者也不知道是谁向它发出了信号。
在程序设计过程中，多个信号可以连接至一个反应槽，类似的，一个信号也可以连接至多个反应槽，甚至一个信号可以连接至另一个信号。在Windows中，如果我们需要多个菜单都激发一个函数，一般是先写一个共用函数，然后在每个菜单的事件中调用此函数。在Qt中如果要实现同样的功能，就可以把实现部分写在一个菜单中，然后把其他菜单与这个菜单级联起来。
虽然信号/反应槽机制有很多优点，使用也很方便，但它也不是没有缺点。最大的缺点在于要稍微牺牲一点性能。根据Trolltech公司的自测，在CPU为Intel PentiumII 500 Mhz的PC机上，对于一个信号对应一个反应槽的连接来说，一秒钟可以调用两百万次；对于一个信号对应两个反应槽的连接来说，一秒钟可以调用一百二十万次。
这个速度是不经过连接而直接进行回调的速度的十分之一。请注意这里的十分之一速度比是调用速度的比较，而不是一个完整函数执行时间的比较。事实上一般情况下一个函数的总执行时间大部分是在执行部分，只有小部分是在调用部分，因些这个速度是可以接受的。这就象面向对象的编程和早些年的结构化编程相比一样：程序的执行效率并没有提高，反而是有所下降的，但现在大家都在用面向对象的方法编写程序。用一部分执行效率换回开发效率与维护效率是值得的，况且现在已是P4为主流的时代。我们先来看一个简单的样例：
- class Demo : public QObject  
- {  
- Q_OBJECT  
- public:  
- Demo();  
- int value() const { return val; }；  
- public slots:  
- void setValue( int );  
- signals:  
- void valueChanged( int );  
- private:  
- int val;  
- }; 
由样例可看到，类的定义中有两个关键字slots和signals，还有一个宏Q_OBJECT。在Qt的程序中如果使用了信号与反应槽就必须在类的定义中声明这个宏，不过如果你声明了该宏但在程序中并没有信号与反应槽，对程序也不会有任何影响，所以建议大家在用Qt写程序时不妨都把这个宏加上。使用slots定义的就是信号的功能实现，即反应槽，例如：
- void Demo::setValue( int v )  
- {  
- if ( v != val ) {  
- vval = v;  
- emit valueChanged(v);  
- }  
- } 
这段程序表明当setValue执行时它将释放出valueChanged这个信号。以下程序示范了不同对象间信号与反应槽的连接。
- Demo a, b;  
- connect(&a, SIGNAL(valueChanged(int)), &b, SLOT(setValue(int)));  
- b.setValue( 11 );  
- a.setValue( 7Array );  
- b.value(); // b的值将是7Array而不是原先设的11 
在以上程序中，一旦信号与反应槽连接，当执行a.setValue(7Array)时就会释放出一个valueChanged(int)的信号，对象b将会收到这个信号并触发setValue(int)这个函数。当b在执行setValue(int)这个函数时，它也将释放valueChanged(int)这个信号，当然b 的信号无人接收，因此就什么也没干。示意图如下：请注意，在样例中我们仅当输入变量v不等于val时才释放信号，因此就算对象
a与b进行了交叉连接也不会导致死循环的发生。由于在样例中使用了Qt特有的关键字和宏，而Qt本身并不包括C++的编译器，因此如果用流行的编译程序（如Windows下的Visual C++或Linux下的gcc）是不能直接编译这段代码的，必须用Qt的中间编译工具moc.exe把该段代码转换为无专用关键字和宏的C++代码才能为这些编译程序所解析、编译与链接。
以上代码中信号与反应槽的定义是在类中实现的。那么，非类成员的函数，比如说一个全局函数可不可以也这样做呢？答案是不行，只有是自身定义了信号的类或其子类才可以发出该种信号。一个对象的不同信号可以连接至不同的对象。
当一个信号被释放时，与之连接的反应槽将被立刻执行，就象是在程序中直接调用该函数一样。信号的释放过程是阻塞的，这意味着只有当反应槽执行完毕后该信号释放过程才返回。如果一个信号与多个反应槽连接，则这些反应槽将被顺序执行，排序过程则是任意的。因此如果程序中对这些反应槽的先后执行次序有严格要求的话，应特别注意。使用信号时还应注意：信号的定义过程是在类的定义过程即头文件中实现的。
为了中间编译工具moc的正常运行，不要在源文件(.cpp)中定义信号，同时信号本身不应返回任何数据类型，即是空值(void)。如果你要设计一个通用的类或控件，则在信号或反应槽的参数中应尽可能使用常规数据以增加通用性。如上例代码中valueChanged的参数为int型，如果它使用了特殊类型如QRangeControl::Range，那么这种信号只能与RangeControl中的反应槽连接。如前所述，反应槽也是常规函数，与未定义slots的用户函数在执行上没有任何区别。
但在程序中不可把**信号**与常规函数连接在一起，否则**信号**的释放不会引起对应函数的执行。要命的是中间编译程序moc并不会对此种情况报错，C++编译程序更不会报错。初学者比较容易忽略这一点，往往是程序编好了没有错误，逻辑上也正确，但运行时就是不按自己的意愿出现结果，这时候应检查一下是不是这方面的疏忽。**Qt**的设计者之所以要这样做估计是为了**信号**与反应槽之间匹配的严格性。既然反应槽与常规函数在执行时没有什么区别，因此它也可以定义成公共反应槽（public
 slots）、保护反应槽（protected slots）和私有反应槽（private slots）。如果需要，我们也可以把反应槽定义成虚函数以便子类进行不同的实现，这一点是非常有用的。
只讨论一下**信号**与反应槽的使用好象还不过瘾，既然**Qt**的X11 Free版提供了源代码，我们就进去看一下在QObject中connect的实现。由于**Qt**是一个跨平台的开发库，为了与不同平台上的编译器配合，它定义了一个中间类QMetaObject，该类的作用是存放有关信号/反应槽以及对象自身的信息。这个类是**Qt**内部使用的，用户不应去使用它。
# 详解QT 信号机制 （下篇）
继续 详解[QT 信号机制 （上篇）](http://www.51cto.com/php/viewart.php?artID=273544) 的内容接续介绍，本节介绍的是详解**QT 信号机制** （下篇），以下是QMetaObject的定义（为了浏览方便，删除了一部分次要代码）：
- class Q_EXPORT QMetaObject  
- {  
- public:  
- QMetaObject( const char * const class_name, QMetaObject *superclass,  
- const QMetaData * const slot_data, int n_slots,  
- const QMetaData * const signal_data, int n_signals);  
- virtual ~QMetaObject();  
- int numSlots( bool super = FALSE ) const; /* 反应槽的数量 */  
- int numSignals( bool super = FALSE ) const; /* 信号的数量 */  
- int findSlot( const char *, bool super = FALSE ) const;  
- /* 根据反应槽的名称找到其在列表中的索引 */  
- int findSignal( const char *, bool super = FALSE ) const;  
- /* 根据信号的名称找到其在列表中的索引 */  
- const QMetaData *slot( int index, bool super = FALSE ) const;  
- /* 根据索引取得反应槽的数据 */  
- const QMetaData *signal( int index, bool super = FALSE ) const;  
- /* 根据索引取得信号的数据 */  
- QStrList slotNames( bool super = FALSE ) const;  
- /* 取得反应槽列表 */  
- QStrList signalNames( bool super = FALSE ) const;  
- /* 取得信号列表 */  
- int slotOffset() const;  
- int signalOffset() const;  
- static QMetaObject *metaObject( const char *class_name );  
- private:  
- QMemberDict *init( const QMetaData *, int );  
- const QMetaData *slotData; /* 反应槽数据指针 */  
- QMemberDict *slotDict; /* 反应槽数据字典指针 */  
- const QMetaData *signalData; /* 信号数据指针*/  
- QMemberDict *signalDict; /* 信号数据字典指针*/  
- int signaloffset;  
- int slotoffset;  
- }; 
再看一下QObject中connect的实现。剥去粗枝，函数中便露出一个更细化的函数：connectInternal，它又做了哪些工作呢？让我们看一下：
- void QObject::connectInternal( const QObject *sender, int signal_index,  
- const QObject *receiver,  
- int membcode, int member_index )  
- {  
- QObject *s = (QObject*)sender;  
- QObject *r = (QObject*)receiver;  
- if ( !s->connections ) {  
- /* 如果某个对象有信号或反应槽但没有建立相互连接是不会建立连接列表的，这样可减少一些无谓的资源消耗 */  
- s->connections = new QSignalVec( 7 );  
- s->connections->setAutoDelete( TRUE );  
- /* 无连接时，连接列表将被自动删除 */  
- }  
- QConnectionList *clist = s->connections->at( signal_index );  
- if ( !clist ) {  
- /* 建立与信号源对象中某一个信号所对应的接收对象的列表 */  
- clist = new QConnectionList;  
- clist->setAutoDelete( TRUE );  
- s->connections->insert( signal_index, clist );  
- }  
- QMetaObject *rrmeta = r->metaObject();  
- switch ( membcode ) {  
- /* 取得信号或反应槽的数据指针 */  
- case QSLOT_CODE:  
- rm = rmeta->slot( member_index, TRUE );  
- break;  
- case QSIGNAL_CODE:  
- rm = rmeta->signal( member_index, TRUE );  
- break;  
- }  
- QConnection *c = new QConnection( r, member_index,  
- rm ? rm->name : "qt_invoke", membcode );  
- /* 创建一个新的信号/反应槽连接 */  
- clist->append( c ); /* 信号源端加入这一对连接 */  
- if ( !r->senderObjects ) {  
- /* 类似于信号源端，反应槽端的连接列表也是动态创建的 */  
- r->senderObjects = new QObjectList;  
- }  
- r->senderObjects->append( s ); /* 反应槽端加入这一对连接 */  
- } 
到此，信号与反应槽的连接已建立完毕，那么信号产生时又是如何触发反应槽的呢？从QObject的定义中可以看出其有多个activate_signal的成员函数，这些函数都是protected的，也即只有其自身或子类才可以使用。看一下它的实现：
- void QObject::activate_signal( QConnectionList *clist, QUObject *o )  
- {  
- if ( !clist ) /* 有效性检查 */  
- return;  
- QObject *object;  
- QConnection *c;  
- if ( clist->count() == 1 ) {  
- /* 对某一个对象的一个具体信号来说，一般只有一种反应槽与之相连，这样事先判断一下可以加快处理速度 */  
- c = clist->first();  
- object = c->object();  
- sigSender = this;  
- if ( c->memberType() == QSIGNAL_CODE )  
- object->qt_emit( c->member(), o ); /* 信号级连 */  
- else  
- object->qt_invoke( c->member(), o );/* 调用反应槽函数 */  
- } else {  
- QConnectionListIt it(*clist);  
- while ( (c=it.current()) ) { /* 有多个连接时，逐一扫描 */  
- ++it;  
- object = c->object();  
- sigSender = this;  
- if ( c->memberType() == QSIGNAL_CODE )  
- object->qt_emit( c->member(), o ); /* 信号级连 */  
- else  
- object->qt_invoke( c->member(), o ); /* 调用反应槽函数 */  
- }  
- }  
- } 
至此我们已经可以基本了解Qt中信号/反应槽的流程。我们再看一下Qt为此而新增的语法：三个关键字：slots、signals和emit，三个宏：SLOT()、SIGNAL()和Q_OBJECT。在头文件qobjectdefs.h中，我们可以看到这些新增语法的定义如下：
- #define slots // slots: in class  
- #define signals protected // signals: in class  
- #define emit // emit signal  
- #define SLOT(a) "1"#a  
- #define SIGNAL(a) "2"#a 
由此可知其实三个关键字没有做什么事情，而SLOT()和SIGNAL()宏也只是在字符串前面简单地加上单个字符，以便程序仅从名称就可以分辨谁是信号、谁是反应槽。中间编译程序moc.exe则可以根据这些关键字和宏对相应的函数进行“翻译”，以便在C++编译器中编译。剩下一个宏Q_OBJECT比较复杂，它的定义如下：
- #define Q_OBJECT ＼  
- publi ＼  
- virtual QMetaObject *metaObject() const { ＼  
- return staticMetaObject(); ＼  
- }  
- ＼  
- virtual const char *className() const; ＼  
- virtual void* qt_cast( const char* ); ＼  
- virtual bool qt_invoke( int, QUObject* ); ＼  
- virtual bool qt_emit( int, QUObject* ); ＼  
- QT_PROP_FUNCTIONS  
- ＼  
- static QMetaObject* staticMetaObject(); ＼  
- QObject* qObject() { return (QObject*)this; } ＼  
- QT_TR_FUNCTIONS  
- ＼  
- private: ＼  
- static QMetaObject *metaObj; 
从定义中可以看出该宏的作用有两个：一是对与自己相关的QMetaObject中间类操作进行声明，另一个是对信号的释放操作和反应槽的激活操作进行声明。当moc.exe对头文件进行预编译之后，将会产生一个可供C++编译器编译的源文件。以上述的Demo类为例，假设它的代码文件分别为d e m o . h和d e m o . c p p ，预编译后将产生
moc_demo.cpp，其主要内容如下：
- QMetaObject *Demo::metaObj = 0;  
- void Demo::initMetaObject()  
- {  
- if ( metaObj )  
- return;  
- if ( strcmp(QObject::className(), "QObject") != 0 )  
- badSuperclassWarning("Demo","QObject");  
- (void) staticMetaObject();  
- }  
- QMetaObject* Demo::staticMetaObject()  
- {  
- if ( metaObj )  
- return metaObj;  
- (void) QObject::staticMetaObject();  
- typedef void(Demo::*m1_t0)(int);  
- m1_t0 v1_0 = Q_AMPERSAND Demo::setValue; /* 定位反应槽的入口 */  
- QMetaData *slot_tbl = QMetaObject::new_metadata(1);  
- /* 新建一个反应槽数据 */  
- QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);  
- slot_tbl[0].name = "setValue(int)"; /* 反应槽名称 */  
- slot_tbl[0].ptr = *((QMember*)&v1_0);  
- /* 通过反应槽名称可以找到反应槽的入口指针 */  
- slot_tbl_access[0] = QMetaData::Public; /* 权限类型 */  
- typedef void(Demo::*m2_t0)(int);  
- m2_t0 v2_0 = Q_AMPERSAND Demo::valueChanged; /* 定位信号的入口 */  
- QMetaData *signal_tbl = QMetaObject::new_metadata(1); /* 新建信号数据 */  
- signal_tbl[0].name = "valueChanged(int)"; /* 信号名称 */  
- signal_tbl[0].ptr = *((QMember*)&v2_0);  
- /* 通过信号名称可以找到信号的入口指针 */  
- metaObj = QMetaObject::new_metaobject(  
- /* 创建一个与demo类相关的QMetaObject对象 */  
- "Demo", "QObject",  
- slot_tbl, 1,  
- signal_tbl, 1,  
- 0, 0 );  
- metaObj->set_slot_access( slot_tbl_access ); /* 设置权限 */  
- return metaObj;  
- }  
- // 有信号时即激活对应的反应槽或另一个信号  
- void Demo::valueChanged( int t0 )  
- {  
- activate_signal( "valueChanged(int)", t0 );  
- } 
该文件中既没有**Qt**特有的关键字，也没有特殊的宏定义，完全符合普通的[C++](http://developer.51cto.com/col/1523/)语法，因此可以顺利编译和链接。
