# Qt的句柄类和实体类-QObject和QOjectPrivate - xqhrs232的专栏 - CSDN博客
2016年12月05日 17:04:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：348
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/newthinker_wei/article/details/22789885](http://blog.csdn.net/newthinker_wei/article/details/22789885)
相关文章
1、[QT:QObject
 简单介绍](http://blog.chinaunix.net/uid-374124-id-4121508.html) ----[http://blog.chinaunix.net/uid-374124-id-4121508.html](http://blog.chinaunix.net/uid-374124-id-4121508.html)
2、
# [QObject是QT所有类的基类](http://blog.csdn.net/imxiangzi/article/details/47979285)----[http://blog.csdn.net/imxiangzi/article/details/47979285](http://blog.csdn.net/imxiangzi/article/details/47979285)
原文来自：[http://mobile.51cto.com/symbian-271989.htm](http://mobile.51cto.com/symbian-271989.htm)
**详解 Qt 源码分析 QObject**
**Qt **源码分析 **QObject**是本文要介绍的内容，很详细的去解析，先来看内容。**Qt**的**QObject**
1.试验代码：
- #include <QApplication>
- #include <QtCore>
- #include <QtGui>
- int main(int argc, char *argv[])  
- {  
-  QApplication app(argc, argv);  
-  int size = sizeof(QObject);  
-  QPushButton* quit = new QPushButton("Quit");  
-  delete quit;  
-  return app.exec();  
- } 
QObject是Qt类体系的唯一基类,就象MFC中的CObject和Dephi中的TObject,是Qt各种功能的源头活水,因此Qt源码分析的第一节就放在这个QObject上
- int size = sizeof(QObject); 
QObject的大小是8,除了虚函数表指针需要的4个字节以外,另外的4个字节是:
- QObjectData *d_ptr; 
QObject中的数据被封装在QObjectData类中了,为什么要封装数据呢?
原因是Qt中有一个很重要的设计模式就是句柄实体模式,也就是以QObject为基类的类一般都是句柄类,一般只有一个指针指向一个实体类,在实体类中保存全部的数据
而且一般情况下这个指针还是私有的,方便以后修改句柄类的实现细节
因此,也可以说和句柄类继承关系平行的也有一套实体类派生体系,因此,准确的说,Qt的基类其实有两个,一个是QObject,这是句柄类的唯一基类,另一个是QObjectData,这是实体
类的基类
- QObjectData类定义如下:  
- class QObjectData {  
- public:  
-     virtual ~QObjectData() = 0;  
-     QObject *q_ptr;  
-     QObject *parent;  
-     QObjectList children;  
- 
-     uint isWidget : 1;  
-     uint pendTimer : 1;  
-     uint blockSig : 1;  
-     uint wasDeleted : 1;  
-     uint ownObjectName : 1;  
-     uint sendChildEvents : 1;  
-     uint receiveChildEvents : 1;  
-     uint unused : 25;  
-     int postedEvents;  
- #ifdef QT3_SUPPORT  
-     int postedChildInsertedEvents;  
- #else  
-     int reserved;  
- #endif  
- };  
- QObject *q_ptr; 
这个指针指向实体类对应的句柄类,这和上面的代码
- QObjectData *d_ptr; 
遥相呼应,使得句柄类和实体类可以双向的引用,为什么是这样的命名方式呢?可能q指的是Qt接口类,d指的是Data数据类,这当然是猜测了,但是或许可以方便你记忆,在Qt中,
这两个指针名字是非常重要的,必须记住
但是仅仅如此还是不容易使用这两个指针,因为它们都是基类的类型,难道每次使用都要类型转换吗?为了简单起见,Qt在这里声明了两个宏
- #define Q_DECLARE_PRIVATE(Class) \  
-     inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(d_ptr); } \  
-     inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(d_ptr); } \  
-     friend class Class##Private;  
- 
- #define Q_DECLARE_PUBLIC(Class) \  
-     inline Class* q_func() { return static_cast<Class *>(q_ptr); } \  
-     inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \  
-     friend class Class; 
只要在类的头文件中使用这两个宏,就可以通过函数直接得到实体类和句柄类的实际类型了,而且这里还声明了友元,使得数据类和句柄类连访问权限也不用顾忌了
而且为了cpp文件中调用的方便,更是直接声明了以下两个宏
- #define Q_D(Class) Class##Private * const d = d_func()  
- #define Q_Q(Class) Class * const q = q_func() 
好了,使用起来倒是方便了,但是以后局部变量可千万不能声明为d和q了
这里的d_func和q_func函数是非常常用的函数,可以理解为一个是得到数据类,一个是得到Qt接口类
- QObject *parent; 
这里指向QObject的父类
- QObjectList children; 
这里指向QObject相关的子类列表
这确实是个大胆的设计,如果系统中产生了1000000个QObject实例(对于大的系统,这个数字很容易达到吧),每个QObject子类平均下来是100(这个数字可能大了),
光这些指针的开销就有1000000*100*4=400M,是够恐怖的,如果我们必须在灵活性和运行开销之间做一个选择的话,无疑Qt选择了前者,对此我也很难评论其中的优劣,
还是祈求越来越强的硬件水平和Qt这么多年来得到的赫赫威名保佑我们根本就没有这个问题吧,呵呵
总之,Qt确实在内存中保存了所有类实例的树型结构
- uint isWidget : 1;  
- uint pendTimer : 1;  
- uint blockSig : 1;  
- uint wasDeleted : 1;  
- uint ownObjectName : 1;  
- uint sendChildEvents : 1;  
- uint receiveChildEvents : 1;  
- uint unused : 25; 
这些代码就简单了,主要是一些标记位,为了节省内存开销,这里采用了位域的语法,还保留了25位为unused,留做以后的扩充
- #ifdef QT3_SUPPORT  
-     int postedChildInsertedEvents;  
- #else  
-     int reserved;  
- #endif 
这里或许是为了兼容Qt3下序列化的数据吧,即使没有定义QT3_SUPPORT,还是保留了一个数据reserved,以保证整个QObjectData的大小不变
具体看一个例子吧,对这种句柄实体模式加深认识,这就是Qt中的按钮类QPushButton
- QPushButton的句柄类派生关系是:  
- QObject  
-  QWidget  
-   QAbstractButton  
-    QPushButton  
- 
- QPushButton的实体类派生关系是:  
- QObjectData  
-  QObjectPrivate  
-   QWidgetPrivate  
-    QAbstractButtonPrivate  
-     QPushButtonPrivate 
可以看出,这里确实是一个平行体系,只不过实体类派生关系中多了一个QObjectPrivate,这个类封装了线程处理,信号和槽机制等具体的实现,可以说它才是Qt实体类中
真正起作用的基类,而QObjectData不过是一层浅浅的数据封装而已
先不忙了解QObjectPrivate类中的接口和实现,我们先看看在Qt中,句柄类和实体类这两条体系是如何构造的?
- QPushButton* quit = new QPushButton("Quit"); 
创建一个Qt的按钮,简简单单一行代码,其实背后大有玄机
- QPushButton::QPushButton(const QString &text, QWidget *parent)  
-     : QAbstractButton(*new QPushButtonPrivate, parent) 
首先QPushButton的构造函数中调用了QAbstractButton的构造函数,同时马上new出来一个QPushButtonPrivate实体类,然后把指针转换为引用传递给QAbstractButton
- QAbstractButton::QAbstractButton(QAbstractButtonPrivate &dd, QWidget *parent)  
-     : QWidget(dd, parent, 0) 
QAbstractButton的构造函数中继续调用基类QWidget的构造函数,同时把QPushButtonPrivate实体类指针继续传给基类
- QWidget::QWidget(QWidgetPrivate &dd, QWidget* parent, Qt::WFlags f)  
-     : QObject(dd, ((parent && (parent->windowType() == Qt::Desktop)) ? 0 : parent)), QPaintDevice() 
QWidget继续坐着同样的事情
- QObject::QObject(QObjectPrivate &dd, QObject *parent)  
-     : d_ptr(&dd) 
终于到了基类QObject,这里就直接把QPushButtonPrivate的指针赋值给了d_ptr(还记得这个变量名称吧)
最终在QPushButton构造时同时产生的new QPushButtonPrivate被写到了QObject中的d_ptr中
- QObject::QObject(QObjectPrivate &dd, QObject *parent)  
-     : d_ptr(&dd)  
- {  
-     Q_D(QObject);  
-     ::qt_addObject(d_ptr->q_ptr = this);  
-     QThread *currentThread = QThread::currentThread();  
-     d->thread = currentThread ? QThreadData::get(currentThread)->id : -1;  
-     Q_ASSERT_X(!parent || parent->d_func()->thread == d->thread, "QObject::QObject()",  
-                "Cannot create children for a parent that is in a different thread.");  
-     if (parent && parent->d_func()->thread != d->thread)  
- parent = 0;  
-     if (d->isWidget) {  
-         if (parent) {  
-             d->parentparent = parent;  
-             d->parent->d_func()->children.append(this);  
-         }  
-         // no events sent here, this is done at the end of the QWidget constructor  
-     } else {  
-         setParent(parent);  
-     }  
- } 
然后执行QObject的构造函数,这里主要是一些线程的处理,先不理它
- QWidget::QWidget(QWidgetPrivate &dd, QWidget* parent, Qt::WFlags f)  
-     : QObject(dd, ((parent && (parent->windowType() == Qt::Desktop)) ? 0 : parent)), QPaintDevice()  
- {  
-     d_func()->init((parent && parent->windowType() == Qt::Desktop ? parent : 0), f);  
- } 
然后是QWidget的构造函数,这里调用了数据类QWidgetPrivate的init函数,这个函数不是虚函数,因此静态解析成QWidgetPrivate的init函数调用
- QAbstractButton::QAbstractButton(QAbstractButtonPrivate &dd, QWidget *parent)  
-     : QWidget(dd, parent, 0)  
- {  
-     Q_D(QAbstractButton);  
-     d->init();  
- } 
然后是QAbstractButton的构造函数,这里调用了数据类QAbstractButton的init函数,这个函数不是虚函数,因此静态解析成QAbstractButton的init函数调用
- QPushButton::QPushButton(const QString &text, QWidget *parent)  
-     : QAbstractButton(*new QPushButtonPrivate, parent)  
- {  
-     Q_D(QPushButton);  
-     d->init();  
-     setText(text);  
- } 
然后是QPushButton的构造函数,这里调用了数据类QPushButton的init函数,这个函数不是虚函数,因此静态解析成QPushButton的init函数调用。
现在的事情很清楚了,总结一下:
QPushButton在构造的时候同时生成了QPushButtonPrivate指针,QPushButtonPrivate创建时依次调用数据类基类的构造函数
QPushButton的构造函数中显示的调用了基类的构造函数并把QPushButtonPrivate指针传递过去,QPushButton创建时依次调用接口类基类的构造函数
在接口类的构造函数中调用了平行数据类的init函数,因为这个函数不是虚函数,因此就就是此次调用了数据类的init函数
需要指出的是,为什么QPushButtonPrivate实体类指针要转换为引用呢?为什么不是直接传递指针?结论是人家喜欢这样写,就是不传指针传引用,而且要用一个*new之类的怪异语法,真叫人没有办法,其实这里用指针是一样的,代码看起来也自然一些.
- delete quit; 
说完了构造,再说说析构
- QPushButton::~QPushButton()  
- {  
- } 
这里当然会调用QPushButton的析构函数了
-  QAbstractButton::~QAbstractButton()  
- {  
- #ifndef QT_NO_BUTTONGROUP  
-     Q_D(QAbstractButton);  
-     if (d->group)  
-         d->group->removeButton(this);  
- #endif  
- } 
然后是QAbstractButton的析构函数
- QWidget::~QWidget()  
- {  
-     Q_D(QWidget);  
- ...  
- } 
然后是QWidget的析构函数,这里洋洋洒洒一大堆代码,先不管它
- QObject::~QObject()  
- {  
- ...  
- } 
最后是QObject的析构函数,这里也是洋洋洒洒的一大堆
-     Q_D(QObject);  
-     if (d->wasDeleted) {  
- #if defined(QT_DEBUG)  
-         qWarning("Double QObject deletion detected");  
- #endif  
-         return;  
-     }  
-     d->wasDeleted = true; 
这些没有什么好说的,就是设一个wasDeleted的标志,防止再被引用,对于单线程情况下,马上就要被删除了,还搞什么标记啊,根本没用,但是对于多线程情况下,这个标记应该是有用的
- // set all QPointers for this object to zero  
- GuardHash *hash = ::guardHash();  
- if (hash) {  
-     QWriteLocker locker(guardHashLock());  
-     GuardHash::iterator it = hash->find(this);  
-     const GuardHash::iterator end = hash->end();  
-     while (it.key() == this && it != end) {  
-         *it.value() = 0;  
- it = hash->erase(it);  
-     }  
- } 
这里是支持QPointers的实现代码,我们以后再说
- emit destroyed(this); 
Qt的一个指针删除时要发送destroyed信号,一般情况下是没有槽来响应的
- QConnectionList *list = ::connectionList();  
- if (list) {  
-     QWriteLocker locker(&list->lock);  
-     list->remove(this);  
- } 
这里清除了信号槽机制中的记录
- if (d->pendTimer) {  
-     // have pending timers  
-     QThread *thr = thread();  
-     if (thr || d->thread == 0) {  
-         // don't unregister timers in the wrong thread  
-         QAbstractEventDispatcher *eventDispatcher = QAbstractEventDispatcher::instance(thr);  
-         if (eventDispatcher)  
-             eventDispatcher->unregisterTimers(this);  
-     }  
- } 
这里清除定时器
- d->eventFilters.clear(); 
这里清除事件过滤机制
- // delete children objects  
- if (!d->children.isEmpty()) {  
-     qDeleteAll(d->children);  
-     d->children.clear();  
- } 
这里清除所有子类指针,当然每个子类指针清除时又会清除它的所有子类,因此Qt中new出来的指针很少有显示对应的delete,因为只要最上面的指针被框架删除了,
它所连带的所有子类都被自动删除了
- {  
-     QWriteLocker locker(QObjectPrivate::readWriteLock());  
-     ::qt_removeObject(this);  
- 
-     /*  
-       theoretically, we cannot check d->postedEvents without  
-       holding the postEventList.mutex for the object's thread,  
-       but since we hold the QObjectPrivate::readWriteLock(),  
-       nothing can go into QCoreApplication::postEvent(), which  
-       effectively means noone can post new events, which is what  
-       we are trying to prevent. this means we can safely check  
-       d->postedEvents, since we are fairly sure it will not  
-       change (it could, but only by decreasing, i.e. removing  
-       posted events from a differebnt thread)  
-     */  
-     if (d->postedEvents > 0)  
-         QCoreApplication::removePostedEvents(this);  
- }  
- if (d->parent)        // remove it from parent object  
-     d->setParent_helper(0);  
- delete d;  
- d_ptr = 0; 
这里要删除相关的数据类指针了
小结：**Qt **源码分析 **QObject**的内容介绍完了，希望本文对你有所帮助！

