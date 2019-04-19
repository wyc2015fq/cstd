# Qt一些源码实现细节及内部实现简析 - fanyun的博客 - CSDN博客
2017年10月29日 19:06:56[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1687
      Qt中，最重要的机制，莫过于信号和槽了，现在我们谈谈关于信号（emit）和槽（slots）编译器幕后的故事。当我们写下一下emit signal代码的时候，与这个signal相连接的slot就会被调用，那么这个调用是如何发生的？
      首先，让我们来看一段例子代码：
```cpp
class ZMytestObj : public QObject
{
Q_OBJECT
signals:
void sigMenuClicked();
void sigBtnClicked();
};
```
      MOC编译器在做完预处理之后的代码如下：
```cpp
// SIGNAL 0
void ZMytestObj::sigMenuClicked()
{
QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
 
// SIGNAL 1
void ZMytestObj::sigBtnClicked()
{
QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
```
       每一个signal都会被转换为一个与之相对应的成员函数。也就是说，当我们写下这样一行代码：
```cpp
emit sigBtnClicked();
```
      当程序运行到这里的时候，实际上就是调用了void ZMytestObj::sigBtnClicked() 这个函数。大家注意比较这两个函数的函数体，
```cpp
void ZMytestObj::sigMenuClicked()  void ZMytestObj::sigBtnClicked()，
```
      它们唯一的区别就是调用 QMetaObject::activate 函数时给出的参数不同，一个是0，一个是1，它们的含义是什么呢？它们表示是这个类中的第几个signal被发送出来了，回头再去看头文件就会发现它们就 是在这个类定义中，signal定义出现的顺序，这个参数可是非常重要的，它直接决定了进入这个函数体之后所发生的事情。
当执行流程进入到QMetaObject::activate函数中后，会先从connectionLists(这个变量根据传入的signal的index将每个signal对应的connectionList都包含进来)这个变量中取出与这个signal相对应的 connection list，它根据的就是刚才所传入进来的signal index。这个connectionlist中保存了所有和这个signal相链接的slot的信息，每一对connection(即：signal 和 slot 的连接)是这个list中的一项。
      在每个一具体的链接记录(connectionList)中，还保存了这个链接的类型，是自动链接类型，还是队列链接类型，或者是阻塞链接类型，不同的类型处理方法还不一样的。这里，我们就只说一下直接调用的类型。
对于直接链接的类型，先通过connectionList找到接收这个signal的对象的指针，然后通过这个对象指针找到处理这个signal的slot的index(这个index表示是这个对象中slot的索引)，已经是否有需要处理的参数，然后就使用这些信息去调用receiver的qt_metcall 方法。
      在qt_metcall方法中就简单了，根据slot的index，一个大switch语句，调用相应的slot函数就可以了。connect，幕后的故事。我们都知道，把一个signal和slot连接起来，需要使用QObject类的connect方法，它的作用就是把一个object的signal和另外一个object的slot连接起来，以达到对象间通讯的目的。
       connect 在幕后到底都做了些什么事情？为什么emit一个signal后，相应的slot都会被调用？
       SIGNAL 和 SLOT 宏定义：
我们在调用connect方法的时候，一般都会这样写：
      obj.connect(&obj, SIGNAL(destroyed()), &app, SLOT(aboutQt()));
我们看到，在这里signal和slot的名字都被包含在了两个大写的SIGNAL和SLOT中，这两个是什么呢？原来SIGNAL 和 SLOT 是Qt定义的两个宏。好了，让我们先来看看这两个宏都做了写什么事情：
这里是这两个宏的定义：
# define SLOT(a)      '1'#a
# define SIGNAL(a)    '2'#a
      原来Qt把signal和slot都转化成了字符串，并且还在这个字符串的前面加上了附加的符号，signal前面加了'2'，slot前面加了'1'。也就是说，我们前面写了下面的connect调用，在经过moc编译器转换之后，就便成了：
obj.connect(&obj, '2destroyed()', &app, '1aboutQt()'));
      当connect函数被调用了之后，都会去检查这两个参数是否是使用这两个宏正确的转换而来的，它检查的根据就是这两个前置数字，是否等于1或者是2，如果不是，connect函数当然就会失败啦！
      然后，会去检查发送signal的对象是否有这个signal，方法就是查找这个对象的class所对应的staticMetaObject对象中所包含的d.stringdata所指向的字符串中是否包含这个signal的名字，在这个检查过程中,就会用到d.data所指向的那一串整数，通过这些整数值来计算每一个具体字符串的起始地址。同理，还会使用同样的方法去检查slot，看响应这个signal的对象是否包含有相应的slot。这两个检查的任 何一个如果失败的话，connect函数就失败了，返回false.
     前面的步骤都是在做一些必要的检查工作，下一步，就是要把发送signal的对象和响应signal的对象关联起来。在QObject的私有数据类QObjectPrivate中，有下面这些数据结构来保存这些信息：
```cpp
class QObjectPrivate : public QObjectData
{
   struct Connection
   {
     QObject *receiver;
     int method;
     uint connectionType : 3; // 0 == auto, 1 == direct, 2 == queued, 4 == blocking
     QBasicAtomicPointer argumentTypes;
   };
 
   typedef QList ConnectionList;
 
   QObjectConnectionListVector *connectionLists;
 
   struct Sender
   {
      QObject *sender;
      int signal;
      int ref;
   };
 
   QList senders;
}
```
        在发送signal的对象中，每一个signal和slot的connection，都会创建一个QObjectPrivate::Connection对象，并且把这个对象保存到connectionList这个Vector里面去。在响应signal的对象中，同样，也是每一个signal和slot的connection，都会一个创建一个Sender对象，并且把这个对象附加在Senders这个列表中。
       Qt对象之间的父子关系及内存泄露
       很多C/C++初学者常犯的一个错误就是，使用malloc、new分配了一块内存却忘记释放，导致内存泄漏。Qt的对象模型提供了一种Qt对象之间的父 子关系，当很多个对象都按一定次序建立起来这种父子关系的时候，就组织成了一颗树。当delete一个父对象的时候，Qt的对象模型机制保证了会自动的把 它的所有子对象，以及孙对象，等等，全部delete，从而保证不会有内存泄漏的情况发生。任何事情都有正反两面作用，这种机制看上去挺好，但是却会对很多Qt的初学者造成困扰，我经常给别人回答的问题是：
1，new了一个Qt对象之后，在什么 情况下应该delete它？
2，Qt的析构函数是不是有bug？
3，为什么正常delete一个Qt对象却会产生segment fault？等等诸如此类的问题，这篇文章就是针对这个问题的详细解释。
       在每一个Qt对象中，都有一个链表，这个链表保存有它所有子对象的指针。当创建一个新的Qt对象的时候，如果把另外一个Qt对象指定为这个对象的父对象， 那么父对象就会在它的子对象链表中加入这个子对象的指针。
       另外，对于任意一个Qt对象而言，在其生命周期的任何时候，都还可以通过setParent函数 重新设置它的父对象。当一个父对象在被delete的时候，它会自动的把它所有的子对象全部delete。当一个子对象在delete的时候，会把它自己 从它的父对象的子对象链表中删除。
       QWidget是所有在屏幕上显示出来的界面对象的基类，它扩展了Qt对象的父子关系。一个Widget对象也就自然的成为其父Widget对象的子 Widget，并且显示在它的父Widget的坐标系统中。例如，一个对话框(dialog)上的按钮(button)应该是这个对话框的子 Widget。
       关于Qt对象的new和delete，下面我们举例说明。
       例如，下面这一段代码是正确的：
```cpp
int main()
{
QObject* objParent = new QObject(NULL);
QObject* objChild = new QObject(objParent);
QObject* objChild2 = new QObject(objParent);
delete objParent;
}
```
我们用一张图来描述这三个对象之间的关系：
qt.object.parent.child
       在上述代码片段中，objParent是objChild的父对象，在objParent对象中有一个子对象链表，这个链表中保存它所有子对象的指针，在 这里，就是保存了objChild和objChild2的指针。在代码的结束部分，就只有delete了一个对象objParent，
      在 objParent对象的析构函数会遍历它的子对象链表，并且把它所有的子对象(objChild和objChild2)一一删除。所以上面这段代码是安 全的，不会造成内存泄漏。
      如果我们把上面这段代码改成这样，也是正确的：
```cpp
int main()
{
QObject* objParent = new QObject(NULL);
QObject* objChild = new QObject(objParent);
QObject* objChild2 = new QObject(objParent);
delete objChild;
delete objParent;
}
```
      在这段代码中，我们就只看一下和上一段代码不一样的地方，就是在delete objParent对象之前，先delete objChild对象。在delete objChild对象的时候，objChild对象会自动的把自己从objParent对象的子对象链表中删除，也就是说，在objChild对象被 delete完成之后，objParent对象就只有一个子对象(objChild2)了。然后在delete objParent对象的时候，会自动把objChild2对象也delete。所以，这段代码也是安全的。
      Qt的这种设计对某些调试工具来说却是不友好的，比如valgrind。比如上面这段代码，valgrind工具在分析代码的时候，就会认为objChild2对象没有被正确的delete，从而会报告说，这段代码存在内存泄漏。哈哈，我们知道，这个报告是不对的。
      我们在看一看这一段代码：
```cpp
int main()
{
QWidget window;
QPushButton quit('Exit', &window);
}
```
     在这段代码中，我们创建了两个widget对象，第一个是window，第二个是quit，他们都是Qt对象，因为QPushButton是从 QWidget派生出来的，而QWidget是从QObject派生出来的。这两个对象之间的关系是，window对象是quit对象的父对象，由于他们 都会被分配在栈(stack)上面，那么quit对象是不是会被析构两次呢？我们知道，在一个函数体内部声明的变量，在这个函数退出的时候就会被析构，那 么在这段代码中，window和quit两个对象在函数退出的时候析构函数都会被调用。那么，假设，如果是window的析构函数先被调用的话，它就会去
 delete quit对象；然后quit的析构函数再次被调用，程序就出错了。事实情况不是这样的，C++标准规定，本地对象的析构函数的调用顺序与他们的构造顺序相 反。那么在这段代码中，这就是quit对象的析构函数一定会比window对象的析构函数先被调用，所以，在window对象析构的时候，quit对象已 经不存在了，不会被析构两次。
如果我们把代码改成这个样子，就会出错了，对照前面的解释，请你自己来分析一下吧。
```cpp
int main()
{
QPushButton quit('Exit');
QWidget window;
quit.setParent(&window);
}
```
但是我们自己在写程序的时候，也必须重点注意一项，千万不要delete子对象两次，就像前面这段代码那样，程序肯定就crash了。
最后，让我们来结合Qt source code，来看看这parent/child关系是如何实现的。
在本专栏文章的第一部分“对象数据存储”，我们说到过，所有Qt对象的私有数据成员的基类是QObjectData类，这个类的定义如下：
```cpp
typedef QList QObjectList;
class QObjectData
{
public:
QObject *parent;
QObjectList children;
// 忽略其它成员定义
};
```
      我们可以看到，在这里定义了指向parent的指针，和保存子对象的列表。其实，把一个对象设置成另一个对象的父对象，无非就是在操作这两个数据。
把子对 象中的这个parent变量设置为指向其父对象；而在父对象的children列表中加入子对象的指针。
            
