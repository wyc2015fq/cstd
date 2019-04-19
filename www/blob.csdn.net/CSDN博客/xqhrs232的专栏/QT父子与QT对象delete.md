# QT父子与QT对象delete - xqhrs232的专栏 - CSDN博客
2016年10月14日 22:56:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：190
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/caonima123141223/article/details/39968705](http://blog.csdn.net/caonima123141223/article/details/39968705)
相关文章
1、[QT父子与QT对象delete](http://www.cnblogs.com/findumars/p/4982755.html)----[http://www.cnblogs.com/findumars/p/4982755.html](http://www.cnblogs.com/findumars/p/4982755.html)
2、[Qt父－子对象机制](http://blog.csdn.net/yueyansheng2/article/details/50495955)----[http://blog.csdn.net/yueyansheng2/article/details/50495955](http://blog.csdn.net/yueyansheng2/article/details/50495955)
很多C/C++初学者常犯的一个错误就是，使用malloc、new分配了一块内存却忘记释放，导致内存泄漏。Qt的对象模型提供了一种Qt对象之间的父 子关系，当很多个对象都按一定次序建立起来这种父子关系的时候，就组织成了一颗树。当delete一个父对象的时候，Qt的对象模型机制保证了会自动的把 它的所有子对象，以及孙对象，等等，全部delete，从而保证不会有内存泄漏的情况发生。
任何事情都有正反两面作用，这种机制看上去挺好，但是却会对很多Qt的初学者造成困扰，我经常给别人回答的问题是：1，new了一个Qt对象之后，在什么 情况下应该delete它？2，Qt的析构函数是不是有bug？3，为什么正常delete一个Qt对象却会产生segment fault？等等诸如此类的问题，这篇文章就是针对这个问题的详细解释。
在每一个Qt对象中，都有一个链表，这个链表保存有它所有子对象的指针。当创建一个新的Qt对象的时候，如果把另外一个Qt对象指定为这个对象的父对象， 那么父对象就会在它的子对象链表中加入这个子对象的指针。另外，对于任意一个Qt对象而言，在其生命周期的任何时候，都还可以通过setParent函数 重新设置它的父对象。当一个父对象在被delete的时候，它会自动的把它所有的子对象全部delete。当一个子对象在delete的时候，会把它自己 从它的父对象的子对象链表中删除。
QWidget是所有在屏幕上显示出来的界面对象的基类，它扩展了Qt对象的父子关系。一个Widget对象也就自然的成为其父Widget对象的子 Widget，并且显示在它的父Widget的坐标系统中。例如，一个对话框(dialog)上的按钮(button)应该是这个对话框的子 Widget。
关于Qt对象的new和delete，下面我们举例说明。
例如，下面这一段代码是正确的：
int main()
{
QObject* objParent =new QObject(NULL);
QObject* objChild =new QObject(objParent);
QObject* objChild2 =new QObject(objParent);
delete objParent;
}
我们用一张图来描述这三个对象之间的关系：
![clip_image001](http://images.cnblogs.com/cnblogs_com/foxhengxing/201012/201012242124583186.jpg)
在上述代码片段中，objParent是objChild的父对象，在objParent对象中有一个子对象链表，这个链表中保存它所有子对象的指针，在 这里，就是保存了objChild和objChild2的指针。在代码的结束部分，就只有delete了一个对象objParent，在 objParent对象的析构函数会遍历它的子对象链表，并且把它所有的子对象(objChild和objChild2)一一删除。所以上面这段代码是安全的，不会造成内存泄漏。
如果我们把上面这段代码改成这样，也是正确的：
int main()
{
QObject* objParent =new QObject(NULL);
QObject* objChild =new QObject(objParent);
QObject* objChild2 =new QObject(objParent);
delete objChild;
delete objParent;
}
在这段代码中，我们就只看一下和上一段代码不一样的地方，就是在delete objParent对象之前，先delete objChild对象。在delete objChild对象的时候，objChild对象会自动的把自己从objParent对象的子对象链表中删除，也就是说，在objChild对象被 delete完成之后，objParent对象就只有一个子对象(objChild2)了。然后在delete objParent对象的时候，会自动把objChild2对象也delete。所以，这段代码也是安全的。
Qt的这种设计对某些调试工具来说却是不友好的，比如valgrind。比如上面这段代码，valgrind工具在分析代码的时候，就会认为objChild2对象没有被正确的delete，从而会报告说，这段代码存在内存泄漏。哈哈，我们知道，这个报告是不对的。
我们在看一看这一段代码：
int main()
{
QWidget window;
QPushButton quit("Exit", &amp;window);
}
在这段代码中，我们创建了两个widget对象，第一个是window，第二个是quit，他们都是Qt对象，因为QPushButton是从 QWidget派生出来的，而QWidget是从QObject派生出来的。这两个对象之间的关系是，window对象是quit对象的父对象，由于他们 都会被分配在栈(stack)上面，那么quit对象是不是会被析构两次呢？我们知道，在一个函数体内部声明的变量，在这个函数退出的时候就会被析构，那 么在这段代码中，window和quit两个对象在函数退出的时候析构函数都会被调用。那么，假设，如果是window的析构函数先被调用的话，它就会去 delete
 quit对象；然后quit的析构函数再次被调用，程序就出错了。事实情况不是这样的，C++标准规定，本地对象的析构函数的调用顺序与他们的构造顺序相 反。那么在这段代码中，这就是quit对象的析构函数一定会比window对象的析构函数先被调用，所以，在window对象析构的时候，quit对象已 经不存在了，不会被析构两次。
如果我们把代码改成这个样子，就会出错了，对照前面的解释，请你自己来分析一下吧。
int main()
{
QPushButton quit("Exit");
QWidget window;
quit.setParent(&amp;window);
}
但是我们自己在写程序的时候，也必须重点注意一项，千万不要delete子对象两次，就像前面这段代码那样，程序肯定就crash了。
最后，让我们来结合Qt source code，来看看这parent/child关系是如何实现的。
在本专栏文章的第一部分“对象数据存储”，我们说到过，所有Qt对象的私有数据成员的基类是QObjectData类，这个类的定义如下：
typedef QList&lt;QObject*&gt; QObjectList;
class QObjectData
{
public:
QObject *parent;
QObjectList children;
// 忽略其它成员定义
};
我们可以看到，在这里定义了指向parent的指针，和保存子对象的列表。其实，把一个对象设置成另一个对象的父对象，无非就是在操作这两个数据。把子对 象中的这个parent变量设置为指向其父对象；而在父对象的children列表中加入子对象的指针。当然，我这里说的非常简单，在实际的代码中复杂的 多，包含有很多条件判断，有兴趣的朋友可以自己去读一下Qt的源代码。
