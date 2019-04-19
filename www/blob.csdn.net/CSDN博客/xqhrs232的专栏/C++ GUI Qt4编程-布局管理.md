# C++ GUI Qt4编程-布局管理 - xqhrs232的专栏 - CSDN博客
2018年06月11日 14:18:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：201
原文地址::[https://blog.csdn.net/u011014707/article/details/18602773](https://blog.csdn.net/u011014707/article/details/18602773)
相关文章
1、C++ GUI with Qt4编程-完整书签.pdf----[https://download.csdn.net/download/mylovermsn/9862901](https://download.csdn.net/download/mylovermsn/9862901)
2、[Qt布局Layout设置完全填充（设置Layout的Margin值）](https://www.cnblogs.com/craigtao/p/6386215.html)----[https://www.cnblogs.com/craigtao/p/6386215.html](https://www.cnblogs.com/craigtao/p/6386215.html)
3、[Qt Creator 窗体控件自适应窗口大小布局](https://www.cnblogs.com/emouse/archive/2013/05/19/3087708.html)----[https://www.cnblogs.com/emouse/archive/2013/05/19/3087708.html](https://www.cnblogs.com/emouse/archive/2013/05/19/3087708.html)
 放置在窗体中的每一个窗口部件都必须给定一个合适的大小和位置。Qt提供了多个用于在窗体中摆放窗口部件的类：QHBoxLayout，QVBoxLayout，QGridLayout和QStackLayout。这些类简单易用，几乎每个Qt开发人员都会用到它们。
        使用Qt布局类的另外一个原因是：它们可以确保窗体能够自动适应于不同的字体，语言和系统平台。如果用户改变了系统的字体设置，那么该应用程序的窗体将能够立刻做出响应，并在必要的情况下重新改变自己的大小。并且如果将应用程序的用户接口翻译成另外一种语言，那么这些布局类就会考虑窗口部件中翻译的内容，以尽量避免文本被截断的现象发生。
        可以执行布局管理功能的其他类还有QSplitter，QScrollArea，QMainWindow和WMdiArea。这些类所拥有的共同点在于它们提供了一种用户可以灵活掌控的布局方式。例如，QSplitter就提供了一个切分窗口拖动条（splitter bar），通过拖拽它，用户可以改变窗口部件的大小。因为它们经常适合用作布局类的替换方式。
1.在窗体中摆放窗口部件
        有三中基本方法用于管理窗体上子窗口部件的布局：绝对位置法，人工布局法和布局管理器法。
        (1)绝对位置法
                绝对位置法是一种最原始的摆放窗口部件的方法。这可以通过对窗体的各个子窗口部件分配固定的大小和位置以及对窗体分配固定的大小实现。
               FindFileDialog::FindFileDialog(QWidget *parent):QDialog(parent)
               {
                        nameLabel->setGeometry(9, 9, 50, 25);
                        setWindowTitle(tr("Find Files or Folders"));
                        setFixedSize(365, 240);
               }
               绝对位置法的缺点：
               1)用户无法改变窗口的大小。
               2)如果用户选择了一种不常用的大字体，或者当应用程序被翻译成另外一种语言时，也许会把一些文本截断。
               3)对于某些风格的平台，这些窗口部件可能会具有并不合适的尺寸大小。
               4)必须人工计算这些位置和大小。这样做不仅非常枯燥乏味且极易出错，并且还会让后期的维护工作变得痛苦万分。
        (2)人工布局法
               人工布局法也还是需要给定窗口部件的绝对位置，但是利用人工布局方法给定的大小尺寸总是可以和窗口的大小成比例，这比完全依靠手写代码要好得多了。通过重新实现窗体的resizeEvent()函数，该函数可以设置窗体中子窗口部件的几何形状，就可以实现人工布局法。
               FindFileDialog::FindFileDialog(QWidget *parent):QDialog(parent)
               {
                       //设置窗体的最小大小设置为265×190
                       setMinimumSize(265, 190);
                       //设置窗体的初始化大小为365×240
                       resize(365, 240);
               }
               void FindFileDialog::resizeEvent(QResizeEvent event)
               {
                       //获取窗体的额外空间
                       int extraWidth = width() - minimumWidth();
                       int extraHeight = height() - minimumHeight();
                       namedLabel->setGeometry(9, 9, 50, 25);
                       //根据额外的空间设置控件的大小
                       namedLineEdit->setGeometry(65, 9, 100 + extraWidth, 25);
               }
               人工布局法的缺点：
               1)和人工布局法一样需要对很多手写代码中的常量进行计算。
               2)和人工布局法一样存在文本被截断的危险。
        (3)布局管理器法
               布局管理器会为每种类型的窗口部件提供一些合理的默认值，并且也会考虑每一个窗口部件的大小提示，这些大小提示又通常会取决与该窗口部件的字体，风格和内容。布局管理器也会充分考虑其最小和最大尺寸，并且会自动通过调整布局来响应字体的变化，内容的改变以及窗口大小的调整。
               最为重要的三种布局管理器是：QHBoxLayout，QVBoxLayout和QGridLayout。这三个类从QLayout中派生出来，而QLayout类为布局提供了基本框架。这三个类可以得到Qt设计师的完全支持，并且也可以直接在代码中使用它们。
               FindFileDialog::FindFileDialog(QWidget *parent):QDialog(parent)
               {
                       QGridLayout *leftLayout = new QGridLayout;
                       leftLayout->addWidget(nameLabel, 0 , 0);
                       QVBoxLayout *rightLayout = new QVBoxLayout;
                       rightLayout->addStretch();
                       setLayout(leftLayout);
               }
               对话框周围的边白和两个子窗口部件之间的间隔均被设置为默认值，该值取决于当前窗口部件的风格，但通过QLayout::setContentsMargins()和QLayout::setSpacing()，可以对它们的值进行修改。
               在Qt设计师中，也可以通过可视化的方式创建出同样的对话框，即：通过把这些子窗口部件摆放在大致的地方，选择那么需要摆放在一起的窗口部件，然后再单击Form->lay out Horizontally，Form->Lay out Vertically，或者Form->Lay Out in a Grid即可。
               QGridLayout::addWidget()的调用语法：layout->addWidget(widget, row, colunm, rowSpan, columnSpan)；其中，widget是要插入到布局中的子窗口部件，（row，column）是由该窗口部件所占用的左上角单元格，rowSpan是该窗口部件要占用的行数，而columnSpan是该窗口部件要占用的列数，如果省略了这些参数，则参数rowSpan和columnSpan将会取默认值1.
               addStretch()调用告诉垂直布局管理器，它会占满布局中这一处的空间。
               使用布局管理器的确为我们提供了很多额外的好处，如果往布局中添加一个窗口部件或者从布局中移除一个窗口部件，布局都会自动适应所产生的这些新情况，如果对一个子窗口部件调用了hide()或者show()，也同样能够做到自动适应，如果子窗口部件的大小提示发生了变化，部件将会自动进行调整，从而把新的大小提示考虑进去，还有，布局管理器也会自动根据窗体中子窗口部件的最小大小提示和大小提示，从总体上为这个窗体设置一个最小尺寸。
                窗口部件的大小策略会告诉布局系统应该如何对它进行拉伸或者压缩。Qt为它所有的内置窗口部件都提供了合理的默认大小策略值，但是由于不可能为每一种可能产生的布局都提供唯一的默认值，所以在窗体中，开发人员改变它上面的一个或两个窗口部件的大小策略是非常普遍的现象，一个QSizePolicy既包含一个水平分量也包含一个垂直分量，以下是一些最为常用的取值：
                1)Fixed 该窗口部件不能被拉伸或者压缩。窗口部件的大小尺寸总是保持为其大小提示的尺寸。
                2)Minimum 该窗口部件的大小提示就是它的最小大小。再不能把窗口部件压缩到比这个大小提示还要小的大小，但是如有必要，可以拉伸它来填充尽可能多的空间。
                3)Maximum 该窗口部件的大小提示就是它的最大大小。但是可以把该窗口部件压缩成它的最小大小提示的尺寸。
                4)Preferred 该窗口部件的大小提示就是它比较合适的大小。但是如果需要，还是可以对该窗口部件进行拉伸或者压缩。
                5)Expanding 可以拉伸或者压缩该窗口部件，并且它特别希望能够变成变高。
                QSizePolicy大小规则中包含的水平方向和垂直方向两个分量之外，还保存了水平方向和垂直方向的一个拉伸因子，这些拉伸因子可以用来说明在增大窗体时，对不同的子窗口部件应使用的不同放大比例。
                影响布局方式的另一种方法是设置它的子窗口部件的最小大小，最大大小和固定大小。当布局管理器在摆放这些窗口部件的时候，它就会考虑这些约束条件，并且如果这样还不够的话，还可以对子窗口部件的类进行派生并且重新实现sezeHint()函数，由此获得所需的大小提示。
2.分组布局
        QStackedLayout类可以对一组子窗口部件进行摆放，或者对它们进行分页，而且一次只显示其中一个，而把其他的子窗口部件或者分页都隐藏起来。QStackedLayout本身并不可见，并且对于用户改变分页也没有提供其他特有的方法。为方便起见，Qt还提供了QStackedWidget类，这个类提供了一个带内置QStackedLayout的QWidget。
        分页是从0开始编号的，要使某个特定的子窗口部件可见，可以用一个页号来调用setCurrentIndex()。使用indexOf()可以获取子窗口部件的页号。
        PreferenceDialog::PreferenceDialog(QWidget *parent)
        {
                listWidget = new QListWidget;
                listWidget->addItem(tr("Appearance"));
                stackedLayout = new QStackedLayout;
                stackedLayout->addWidget(appearancePage);
                connect(listWidget, SIGNAL(currentRowChanged(int)), stackedLayout, SLOT(setCurrentIndex(int)));
                listWidget->setCurrentRow(0);
        }
        对于页数较少或者可能会保持较小的一些情况，一种比使用QStackedWidget和WListWidget更为简单的替换方法是使用QTabWidget。
3.切分窗口
        QSplitter就是一个可以包含一些其他窗口部件的窗口部件。在切分窗口(splitter)中的这些窗口部件会通过切分条(splitter handle)而分隔开来。用户可以通过拖动这些切分条来改变切分窗口中子窗口部件的大小。切分窗口常常可以用作布局管理器的替代品，从而可以把更多的控制权交给用户。
        QSplitter中的子窗口部件将会自动按照创建时的顺序一个挨一个地(或者一个在另外一个的下面)放在一起，并以切分窗口拖动条(splitter bar)来分隔相邻窗口部件。
        int main(int argc, char *argv[])
        {
                QApplication app(argc, argv);
                QTextEdit *editor1 = new QTextEdit;
                QTextEdit *editor2 = new QTextEdit;
                QSplitter splitter(Qt::Horizontal);
                splitter.addWidget(editor1);
                //设置伸展因子
                splitter.setStretchFactir(1, 1);
                splitter.addWidget(editor2);
                splitter.show();
                return app.exec();              
        }
        与布局管理器不同之处在于，布局管理器只是简单地摆放一个窗体中的子窗口部件并且也没有可见的外形，但QSplitter是从QWidget派生的，并且在使用的时候，也可以像任何其他窗口部件一样使用。通过对多个QSplitter进行水平或者垂直方向的嵌套，就可以获得更为复杂的一些布局。
         当用户重新改变窗口的大小时，QSplitter通常会重新分配空间，以便能够使所有的子窗口部件的相对大小能够与先前保持相同的比例。
         Qt设计师完全支持QSplitter。要把多个窗口部件放到一个切分窗口中，可以先把这些子窗口部件放置在期望的大致位置，选中它们，然后再单击Form->Lay Out Horizontally in Splitter或者Form->Lay Out Vertically in Splitter即可。
4.滚动区域
        QScrollArea类提供了一个可以滚动的视口和两个滚动条。如果想给一个窗口部件添加一个滚动条，则可以使用一个QScrollArea类来实现，这可能要比我们自己通过初始化QScrollBar，然后再实现它的滚动等功能简单得多。
        QScrollArea的使用方法，就是以我们想要添加滚动条的窗口部件为参数调用setWidget()。如果这个窗口部件的父对象不是视口，QScrollArea会自动把这个窗口部件的父对象重定义为该视口(可以通过QScrollArea::viewport()来访问)，并且让它成为视口的字对象。
         int main(int argc, char *argv[])
         {
                  QApplication app(argc, argv);
                  IconEditor *iconEditor = new IconEditor;
                  iconEditor->setIconImage(QImage(":/images/mouse.png"));
                  QScrollArea scrollArea;
                  scrollArea.setWidget(iconEditor);
                  scrollArea.viewport()->setBackgroundRole(QPalette::Dark);
                  scrollArea.viewport()->setAutoFillBackground(true);
                  scrollArea.setWindowTitle(QObject::tr("Icon Editor"));
                  scrollArea.show();
                  return app.exec();
         }
         默认情况下，只有在视口的大小小于子窗口部件的大小时，才会把滚动条显示出来。但是通过设置滚动条的策略，可以强制滚动条总是可见：
         scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
         scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
         QScrollArea从QAbstractScrollArea继承了它的许多功能，像QTextEdit和QAbstractItemView(Qt项视图类的基类)这样的一些类，由于他们是从QAbstractScrollArea中派生出来的，所以为了获得滚动条，就没有必要再把它们封装在QScrollArea中。
5.停靠窗口和工具栏
         停靠窗口(dock window)是指一些可以停靠在QMainWindow中或是浮动为独立窗口的窗口。QMainWindow提供了4个停靠窗口区域，分别在中央窗口部件的上部/下部/左侧/右侧。在Qt中，各个停靠窗口都是QDockWidget的实例。
         每一个停靠窗口都有自己的标题栏，即使它处于停靠时也是如此，通过拖拽这一标题栏，用户可以把停靠窗口从一个停靠区域移动到另外一个停靠区域。通过把这个停靠窗口拖动到其他停靠区域的外面，就可以把停靠窗口从一个停靠区域中分离出来，让它成为一个独立的窗口。自由浮动的停靠窗口总是显示在它们的主窗口的上面。通过点击窗口部件标题栏上的关闭按钮，就可以关闭QDockWidget。通过调用QDockWidget::setFeatures()，就可以禁用所有这些特性以及它们的任意组合。
        QDockWidget *shapesDockWidget = new QDockWidget(tr("Shapes"));
        //设定窗口部件的名称，对于保存/恢复停靠窗口和工具栏的几何形状及状态的话，给定窗口部件的名字就很有必要了。
        shapesDockWidget->setObjectName("shapesDockWidget");
        shapesDockWidget->setWidget(treeWidget);
        //对停靠窗口区域加以限定即可以接受停靠窗口，只允许把停靠窗口拖拽到左侧和右侧的停靠区域，如果没有明确地设置所允许的区域，那么用户就可能把该停靠窗口拖动到这四个可停靠区域中的任何一个。
        shapesDockWodget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        addDockWidget(Qt::RightDockWidgetArea, shapesDockWidget);
        QToolBar *fontToolBar = new QToolBar(tr("Font"));
        fontToolBar->setObjectName("fontToolBar");
        fontToolBar->addWidget(familyComBox);
        fontToolBar->addAction(boldAction);
        fontToolBar->setAllowedAreas(Qt::TopToolArea | Qt::BottomToolBarTool);
        addToolBar(fontToolBar);
        QMainWindow提供了一个上下文菜单，其中列出了所有的停靠窗口和工具栏，用户可以使用这个菜单关闭和恢复停靠窗口，也可以用它隐藏和恢复工具栏。
6.多文档界面
        在主窗口的中央区域能够提供多个文档的那些应用程序就称为多文档界面(Multiple Document Interface, MDI)应用程序，或者称为MDI应用程序。在Qt中，通过把QMdiArea类作为中央窗口部件，并且通过让每一个文档窗口都成为这个QMdiArea的子窗口部件，就可以创建一个多文档界面的应用程序了。
        对于多文档界面应用程序有一个惯例，就是为它提供一个Window菜单，这个菜单中包含一些管理这些窗口以及这些窗口列表的命令。激活窗口会使用一个选择标记标识出来。用户通过在Window菜单中单击代表特定窗口的一项，就可以激活任何窗口。
        MainWindow::MainWindow()
        {
                //构造中央窗口部件
                mdiArea = new QMdiArea;
                setCentralWidget(mdiArea);
                connect(mdiArea, SIGNAL(subWindowActivated(QMdisubWindow*)), this, SLOT(updateActions()));
                createActions();
                createMenus();
                createToolBars();
                createStatusBar();
                setWindowIcon(QPixmap(":/images/icon.png"));
                setWindowTitle(tr("MDI Editor"));
                //定时器
                QTimer::singleShot(0, this, SLOT(loadFiles()));
        }
