# Qt Model/View使用模型/视图（二） - fanyun的博客 - CSDN博客
2019年03月31日 22:27:50[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：33
         Qt提供了两个标准的模型：QStandardItemModel和QFileSystemModel。
         QStandardItemModel是一个多用途的模型，可用于表示list,table,tree views所需要的各种不同的数据结构。这个模型也持有数据。
       QFileSystemModel维护相关的目录内容的信息，它本身不持有数据，仅是对本地文件系统中的文件与目录的描述。QFileSystemModel是一个现成的模型，很容易进行配置以利用现有的数据。使用这个模型，可以很好地展示如何给一个现成的视图设定模型，研究如何用model indexes来操纵数据。
**model与views的搭配使用：**
        QListView与QTreeView很适合与QFileSystemModel进行搭配。下面的例子在tree view与list view显示了相同的信息，QFileSystemModel提供了目录内容数据。这两个Views共享用户选择，因此每个被选择的项在每个view中都会被高亮。
```cpp
QApplication app(argc, argv);  
   QSplitter *splitter = new QSplitter;  
   /*此模型model的创建是为了使用特定文件系统的数据。setRootPath()调用告诉驱动此文件系统的model将数据呈现给哪些视图（Views）*/  
   QFileSystemModel *model = new QFileSystemModel;  
   model->setRootPath(QDir::currentPath());  
   /*创建两个视图（Views），分别是TreeView和ListView*/  
   /*配置一个View以显示模型model中的数据，只需简单调用setModel()函数，并将目录model作为参数。setRootIndex()告诉views显示哪个目录的信息，这需要提供一个model index,然后用这个model index去model中去获取数据。index()函数是QFileSystemModel特有的，通过把一个目录做为参数，得到了需要的model index。*/  
   QTreeView *tree = new QTreeView(splitter);  
   tree->setModel(model);  
   tree->setRootIndex(model->index(QDir::currentPath()));  
   QListView *list = new QListView(splitter);  
   list->setModel(model);  
   list->setRootIndex(model->index(QDir::currentPath()));  
   /*将所有的widgets置于splitter中*/  
   splitter->setWindowTitle("Two views onto the same file system model");  
   splitter->show();  
   return app.exec();
```
显示效果图如图1所示：
![](https://img-blog.csdn.net/2018031310402348)
