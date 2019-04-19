# Qt Model/View模型/视图使用总结（一） - fanyun的博客 - CSDN博客
2019年03月31日 22:27:39[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：32
       在我们Qt项目的数据实时更新显示中，我们经常会用到MVC框架；Qt也提供了一些MVC（模型/视图结构的类），可以用来管理数据并呈现给用户。MVC的体系结构能使数据和模型分离，使得我们更加灵活的构建模块组件，而且它提供了一个标准模型接口，以允许更多类型的数据显示到现有结构的视图中。
       MVC由三种对象组成。Model是应用程序对象，View是它的屏幕表示，Controller定义了用户界面如何对用户输入进行响应。在MVC之前，用户界面设计倾向于三者结合在一起，MVC对它们进行了解耦，提高了灵活性与重用性。MVC之前，用户界面的设计往往是将这些对象组合在一起。MVC的解耦带来了灵活性和重用性。
      MVC（视图/模型结构）把数据和视图组件分离，这使得我们可以在几个不同的试图组件中显示相同的数据，并且实现新类型的视图，并且不改变底层的数据结构。也可以根据控制响应的不同命令，在同一个组件区创建不同的视图样式，来加载不同的输入数据。为了更灵活的对用户输入进行处理，引入了delegate这个概念。它的好处是，数据项的渲染与编程可以进行定制。
![](https://img-blog.csdn.net/20180313101147569)
                                                               图 1  MVC模型框架图
        如上图1所示，model与数据源通讯，并提供接口给结构中的其他组件使用。通信的性质依赖于数据源的种类与model实现的方式。view从model获取model indexes，后者是数据项的引用。通过把model indexes提供给model，view可以从数据源中获取数据。在标准的views中，delegate会对数据项进行渲染，当某个数据项被选中时，delegate通过model indexes与model直接进行交流。
       总的来说，model/view相关类可以被分成上面所提到的三组：models、views、delegates。这些组件通过抽象类来定义，它们提供了共同的接口，在某些情况下，还提供了缺省的实现。抽象类意味着需要子类化以提供完整的其他组件希望的功能。这也允许实现定制的组件。
        Models、views、delegates之间通过信号—槽机制来进行通信：
(1).从model发出的信号通知view数据源中的数据发生了改变。
(2).从view发出的信号提供了有关被显示的数据项与用户交互的信息。
(3).从delegate发生的信号被用于在编辑时通知model和view关于当前编辑器的状态信息。
**Models（模型）：**
        所有的item models都基于QAbstractItemModel类，这个类定义了用于views和delegates访问数据的接口。数据本身不必存储在model，数据可被置于一个数据结构或另外的类、文件、数据库、或其它的程序组件中。
       QAbstractItemModel提供给数据一个接口，它非常灵活，基本满足views的需要，无论数据用以下任何形式表现，如table、list、trees。然而，当你重新实现一个model时，如果它基于table或list形式的数据结构，最好从QAbstractListModel、QAbstractTableModel开始做起，因为它们提供了适当的常规功能的缺省实现。这些类可以被子类化以支持特殊的定制需求。
**        Qt提供了一些现成的models用于处理数据项：**
(1).QStringListModel 用于存储简单的QString项目列表。
(2).QStandardItemModel管理更复杂的树型结构数据项，每项都可以包含任意数据。
(3).QFileSystemModel提供本地文件系统中的文件与目录信息。
(4).QSqlQueryModel、QSqlTableModel、QSqlRelationTableModel用来访问数据库。
       如果这些标准模型不能满足需要，可以使用子类化QAbstractItemModel、QAbstractListModel、QAbstractTableModel类来定制模型。
**Views（视图）：**
       不同的view都完整实现了各自的功能，QListView把数据显示为一个列表，QTableView把Model 中的数据以table的形式表现，QTreeView用具有层次结构列表来显示model中的数据。这些类都基于QAbstractItemView抽象基类。尽管这些类都已经完整地得到了实现，但它们仍然可以用于子类化以满足定制需求。
**Delegates（委托）：**
      QAbstractItemDelegate是model/view架构中的用于delegate的抽象基类。缺省的delegate实现在QStyledItemDelegate类中提供，它可以用于Qt标准views的缺省 delegate。但是，QStyledItemDelegate和QItemDelegate是相互独立的用于实现视图（views）中items的描绘和编辑功能的方法。它们两者的不同在于，QStyledItemDelegate使用当前的样式（style）来描绘items。因此，当我们实现定制委托（delegate）或者使用Qt Style Sheets时，我们建议使用QStyledItemDelegate作为基类。
**Sorting（排序）：**
       在model/view架构中，有两种方法进行排序，选择哪种方法依赖于你的底层Model。假如你的model是可排序的，也就是它重新实现了QAbstractItemModel::sort()函数，QTableView与QTreeView都提供了API，允许你以编程的方式对model数据进行排序。另外，你也可以进行交互方式下的排序（例如，允许用户通过点击view表头的方式对数据进行排序），具体方法是：把QHeaderView::sectionClicked()信号与QTableView::sortByColum()槽或QTreeView::sortByColumn()槽进行关联。
       另一种方法是，假如你的model没有提供需要的接口或是你想用list view表示数据，可以用一个代理model在用view表示数据之前对你的model数据结构进行转换。
**Convenience classes（便利类）：**
       许多便利类都源于标准的view类，它们方便了那些使用Qt中基于项的view与table类，它们不应该被子类化，它们只是为Qt 3的等价类提供一个熟悉的接口。这些类有QListWidget、QTreeWidget、QTableWidget，它们提供了如Qt 3中的QListBox、 QlistView、QTable相似的行为。这些类比View类缺少灵活性，不能用于任意的models，推荐使用model/view的方法处理数据。
