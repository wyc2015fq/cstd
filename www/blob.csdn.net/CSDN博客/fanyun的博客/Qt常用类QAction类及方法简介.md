# Qt常用类QAction类及方法简介 - fanyun的博客 - CSDN博客
2016年12月30日 17:02:42[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3767
       QAction类提供了一个可以同时出现在菜单和工具条上的抽象用户界面操作。
       在图形用户界面应用程序中很多命令可以通过菜单选项、工具条按钮和键盘快捷键调用。因为同一个操作将会被执行，而与它的调用方法无关，并且因为菜单和工具条必须保持同步，所以提供一个操作这样的命令很有用。一个操作可以被添加到菜单和工具条中并且将会自动使它们同步。例如，如果用户按下“加粗”工具条按钮，“加粗”菜单项将会自动被选中。
        QAction可以包含图标、菜单文本、快捷键、状态条文本、这是什么文本和工具提示。它们可以分别通过setIconSet()、setText()、setMenuText()、setToolTip()、setStatusTip()、setWhatsThis()和setAccel()来设置。
        操作可以是一个切换操作，例如，一个“加粗”工具条按钮，或者一个命令操作，例如“打开文件”来调用打开文件对话框。当切换操作的状态改变了，它发射toggled()信号。当命令操作和切换按钮被调用时，它们都发射activated()信号。使用setToggleAction()来设置操作的切换状态。如果要看操作是否是切换操作，请使用isToggleAction()。切换操作可以是“开”，isOn()返回真，或者“关”，isOn()返回假。
        操作可以使用addTo()被添加到窗口部件（菜单或者工具条）中，并且使用removeFrom()来移除。
一旦QAction已经被创建，它应该被添加到相应的菜单或者工具条重并且然后连接到执行这个操作的槽。例如：
       fileSaveAction = new QAction( "Save File", QPixmap( filesave),
                                     "&Save", CTRL+Key_S, this, "save" );
       connect( fileSaveAction, SIGNAL( activated() ) , this, SLOT( save() ) );
       我们创建一个菜单文本为“保存(&S)”并且Ctrl+S为键盘快捷键的“文件保存”操作。我们连接fileSaveAction的activated()信号到我自己的save()槽。注意在这时还没有菜单或者工具条操作，接下来我们将添加它们：
       QToolBar * fileTools = new QToolBar( this, "file operations");
       fileSaveAction->addTo( fileTools );
       QPopupMenu * file = new QPopupMenu( this );
       menuBar()->insertItem( "&File", file );
       fileSaveAction->addTo( file );
      我们创建了一个工具条并把我们的fileSaveAction添加进去。类似的，我们创建一个菜单，添加一个顶级菜单项并且把我们的fileSaveAction添加进去。
（对于一个更详细的实例请参考有QAction特征的简单应用程序排演。）
     我们建议把操作为它所在的窗口的子对象来创建。在绝大多数情况下操作是应用程序主窗口的子对象。
为了防止循环，不要在一个稍后要添加操作的窗口部件的子对象中创建操作。
也可以参考主窗口和相关类和基本窗口部件。
常用的方法总结如下：
1.  QAction::QAction( const QString & text, QObject * parent )
    QAction类的构造函数之一，利用text，parent创建QAction对象。QAction对象一般为菜单中的菜单项，比如 "文件"菜单中"新建"选项就是一个QAction对象，上述构造函数中text成员变量即为菜单项所表示的内容。
    用法：QAction *newAction = new QAction(tr("&New"),this);
2.  void QAction::setIcon ( const QIcon& icon )
    该函数可设置菜单项名称前的图标
    用法：newAction->setIcon(QIcon(":/images/new.png"));
3. void QAction::setShortcut ( const QKeySequence & shortcut );
    设置QAction对象执行的快捷键
    用法：newAction->setShortcut(tr("Ctrl+N"));
4. void QAction::setStatusTip ( const QString & statusTip );
    设置当鼠标移动到“动作”上时，状态栏显示的提示语。
5. void QAction::setVisible ( bool );
    设置“动作”显示与否，当形参为true是，“动作”表示。
7. void QAction::triggered ( bool checked = false ) [signal]
    此函数为信号，当用户触发此“动作”时，此信号发射。例如用户点击了菜单中的菜单项等。
    此函数一般用法为：在QObject::connect（）函数中作为信号参数，用于触发“动作”所对应执行的槽函数——实现“动作”的功能函数，例如：“新建”按钮被用户按下，所需要的功能可能是新建一个文档，那么新建一个文档的动作就在这个槽函数中实现。至于此信号函数中的形参暂时可以忽略。以后会在介绍QObject类中的静态方法connect()函数时详细解说。
8 . void QAction::setCheckable ( bool );
    此函数用于设置QAction类中的私有变量： bool checkable ，此属性用以提供“动作”是否为复选动作，例如Qt Creator中“控件”菜单中的“全屏”菜单项即为复选动作菜单
