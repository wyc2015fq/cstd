# Qt:对象树 - xqhrs232的专栏 - CSDN博客
2016年10月14日 22:43:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：193
原文地址::[http://blog.chinaunix.net/uid-374124-id-4121870.html](http://blog.chinaunix.net/uid-374124-id-4121870.html)
相关文章
1、[QT中的对象树与对象拥有权](http://blog.csdn.net/joliny/article/details/3306363)----[http://blog.csdn.net/joliny/article/details/3306363](http://blog.csdn.net/joliny/article/details/3306363)
QObjects类以对象树组织。创建的QObjects会自动添加到它的父对象的childern()列表中，并且父对象被删除时，子对象也会被删除。这种方法很适合GUI对象。例如：QShortcut是某个窗口的子对象，当用户关闭这个窗口时，QShortcut对象就会被删掉。
        QQuickItem是Qt Quick模块的基础图像元素，派生自QObject，但也有父窗口的概念。一个item的父窗口并不一定是它的父对象。
QWidget作为Qt Widget模块的窗口基础类型，扩展了父子关系。在这样的关系中，子对象通常也是子窗口，例如：它处于父窗口坐标系统中并被父窗口的边界夹住。当一个对话框被关闭时，应用程序便把它删除掉，对话框的按钮和标签也同样被删除掉了，因为按钮和标签都是对话框的子窗口。
也可以手动删除子窗口，它们将会从父窗口中移除。例如，当用户移除工具栏时会导致应用程序删掉QToolBar对象中的一个，同时工具栏的QMainWindow父窗口会检测到变化并重置屏幕间距。
当应用程序看起来很奇怪时，QObject::dumpObjectTree() and QObject::dumpObjectInfo()是有用的工具。
对象构造函数和析构函数的顺序
在堆上创建QObjects（使用new创建）时，对象树会以任何顺序构造，并之后也以任何顺序销毁。当对象树中有一个QObject被删除时，如果对象有父亲，析构函数会自动地从它的父亲移除该对象。如果对象有孩子，析构函数自动删除每一个孩子。不管析构函数的顺序如何，没有一个QObject会被删掉两次。
当对象在栈上创建时，行为相同。正常情况下，析构函数的顺序不会产生任何问题。
int main()
{
    QWidget window;
    QPushButton quit("Quit", &window);
    ...
}
Window和quit都是QObject对象。Quit的析构函数并不会被调用两次，因为C++语言标准规定了现场对象的析构函数以它们的构造函数的相反顺序被调用。Quit的析构函数会被先调用，被它的父亲删除，window的析构函数随后被调用。
但是如果交换构造函数的顺序，如下所示：
int main()
{
    QPushButton quit("Quit");
    QWidget window;
    quit.setParent(&window);
    ...
}
在这个例子中，析构函数的顺序就会造成问题。父亲的析构函数首先被调用因为它后创建。它会先调用孩子quit的析构函数，这是不正确的，因为quit是一个现场变量。随后quit会超出范围，它的析构函数再次被调用，这次是正确的，但破坏已经发生了。
