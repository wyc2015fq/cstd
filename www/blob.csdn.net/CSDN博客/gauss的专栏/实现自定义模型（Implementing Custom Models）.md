# 实现自定义模型（Implementing Custom Models） - gauss的专栏 - CSDN博客
2013年01月06日 23:10:28[gauss](https://me.csdn.net/mathlmx)阅读数：222
转自：http://blog.csdn.net/iamdbl/article/details/3148135
Qt预定义的类已经提供了方便处理和显示数据的方式。然而，一些数据源不能直接使用这些定义好的模型，这就需要创建自定义的模型，优化对这些数据的处理。
在创建自定义模型之前，让我们首先回顾一下Qt的model/view结构的主要概念。在一个模型中每一个数据都有一个模型的索引（a model index）和一组属性，这些属性称为角色（roles），这些属性能够用任何类型的数据保存。在这一章的前几节最常用的角色为：Qt::DisplayRole和Qt::EditRole。其它角色有些是用来辅助性的数据，例如Qt::ToolTipRole，Qt::StatusTipRole和Qt::WhatsThisRole，还有一些是对显示属性的控制，如Qt::FontRole，Qt::TextAlignmentRole，Qt::TextColorRole，Qt::BackgroundColorRole。
Figure 10.9. Schematic view of Qt's models
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/EntryImages/20081026/10-9.JPG)
对一个列表模型（list model），用到的索引属性只有行号，QModelIndex::row()能够得到。对一个表格式模型（table model），用到的索引属性为行号和列号，调用函数QModelIndex::row()，QModelIndex::column()得到。不论式列表模型还是表格模型，每一个项目（item）的父都是根项目（root），这个根项目用一个非法的QModelIndex得到。这一节的前两个例子说明怎么样创建一个自定义的表格模型。
一个树性模型（tree model）和一个表格模型相似，但也略有不同。和表格模型一样，顶层父项目用一个非法的QModelIndex代替，但是每一个项目还能够有子项目，父项目能够用QModelIndex::parent()得到。每一个项目都有自己的角色数据，没有或者有多个子项目。由于项目能够把其它项目作为子项目，因此能够表示递归模型，最后一个例子说明了整个模型的使用。
本节的第一个例子是一个只读的模型，显示不同的货币对比。
Figure 10.10. The Currencies application
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/EntryImages/20081026/10-10.JPG)
这个程序也可以使用一个简单的表格实现，但是我们想自定义一个模型对数据的存贮进行优化。如果我们想在一个表格中保存162中货币之间的比例关系，我们就需要保存162×162＝26224个数据。如果使用下面的模型，我们只需要保存162个数据（每一种货币的对美元的比例）。
类CurrencyModel用一个标准的QTableView表示。模型中的数据类型为QMap<QString,double>；每一个key是一个货币代码，double型的数据表示为相对美元的比例。下面是一个代码段：
QMap<QString, double> currencyMap;currencyMap.insert("AUD", 1.3259);currencyMap.insert("CHF", 1.2970);...currencyMap.insert("SGD", 1.6901);currencyMap.insert("USD", 1.0000);CurrencyModel currencyModel;currencyModel.setCurrencyMap(currencyMap);QTableView tableView;tableView.setModel(¤cyModel);tableView.setAlternatingRowColors(true);
现在我们看一下类的定义，首先看头文件
class CurrencyModel : public QAbstractTableModel{public:    CurrencyModel(QObject *parent = 0);    void setCurrencyMap(const QMap<QString, double> &map);    int rowCount(const QModelIndex &parent) const;    int columnCount(const QModelIndex &parent) const;    QVariant data(const QModelIndex &index, int role) const;    QVariant headerData(int section, Qt::Orientation orientation,                        int role) const;private:    QString currencyAt(int offset) const;    QMap<QString, double> currencyMap;};
我们选择QAbstractTableModel作为基类，因为QAbstractTableModel和我们的数据源最为匹配。Qt提供了一些基本的模型类，包括QAbstractListModel，QAbstractTableModel，QAbstractItemModel。QAbstractItemModel类用来支持广泛类型的模型，而QAbstractListModel和QAbstractTableModel类对一维数据和二维数据提供支持。
Figure 10.11. Inheritance tree for the abstract model classes
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/EntryImages/20081026/10-11.JPG)
对一个只读的表格模型，我我们必须重写三个函数rowCount()，columnCount()和data()。在这个例子中，我们还实现了headerData()，提供一个函数setCurrencyMap()来初始化数据。
CurrencyModel::CurrencyModel(QObject *parent)    : QAbstractTableModel(parent){}
在构造函数中，把父参数传递给基类。
int CurrencyModel::rowCount(const QModelIndex & /* parent */) const{    return currencyMap.count();}int CurrencyModel::columnCount(const QModelIndex & /* parent */) const{    return currencyMap.count();}
对于本例中的表格模型，行数和列数为货币映射中的货币个数。父参数对表格模型来说没有意义，传递这个参数是因为rowCount()和columnCount()从QAbstractItemModel继承而来，基类是支持继承的。
QVariant CurrencyModel::data(const QModelIndex &index, int role) const{    if (!index.isValid())        return QVariant();    if (role == Qt::TextAlignmentRole) {        return int(Qt::AlignRight | Qt::AlignVCenter);    } else if (role == Qt::DisplayRole) {        QString rowCurrency = currencyAt(index.row());        QString columnCurrency = currencyAt(index.column());        if (currencyMap.value(rowCurrency) == 0.0)            return "####";        double amount = currencyMap.value(columnCurrency)                        / currencyMap.value(rowCurrency);        return QString("%1").arg(amount, 0, 'f', 4);    }    return QVariant();}
函数data()返回任何一个项目中一个角色的数据值。这个项目由一个QModelIndex确定。对一个表格模型，需要知道的是行号和列号，通过函数row()和column()可以得到。
如果角色是Qt::TextAlignmentRole，返回一个适合显示数字的对齐方式。如果是显示角色Qt::DisplayRole，我们得到每一个货币关联的值，然后计算汇率。
我们把返回数值作为一个double处理，但是我们对数字的显示没有控制。因此，我们返回一个字符串，对它进行格式化处理。
QVariant CurrencyModel::headerData(int section,                                   Qt::Orientation /* orientation */,                                   int role) const{    if (role != Qt::DisplayRole)        return QVariant();    return currencyAt(section);}
函数headData()是视图（view）调用，显示水平方向和垂直方向的标题。参数section表示行号或者列号（根据方向可以确定）。由于在这个例子中行和列都显示货币代码，我们不需要关注方向，只要返回给定行号或者列号的货币代码就可以。
void CurrencyModel::setCurrencyMap(const QMap<QString, double> &map){    currencyMap = map;    reset();}
调用函数setCurrencyMap()能够改变货币种类。QAbstractItemModel::reset()通知视图，它们正在使用的模型数据是非法的，需要对可视部分数据进行更新。
QString CurrencyModel::currencyAt(int offset) const{    return (currencyMap.begin() + offset).key();}
函数currencyAt()返回给定偏移的键值（即货币代码），我们使用一个STL风格的遍历器得到对应的数据。
如上所述，我们发现创建一个只读的模型很简单，只需关注数据源的特点就可以，需要考虑数据在内存中的保存格式和读取的速度。下一个例子也是基于表格的，但是这里所有的数据都是用户输入的。
这个应用程序用来存贮任何两个城市之间的距离数据。和前面的例子一样，我们使用QTableWidget控件，每一个项目中存储每一个城市对的距离数值。然而，一个自定义的模型能够更加有效，因为一个城市A到城市B的距离和城市B到城市A的距离是一样的，因此，数据沿主对角线是镜像的。
为了说明自定义数据模型和普通表格的不同，我们假设有三个城市A，B，C，如果我们为每一个组合保存一个值，我们需要存储9个值。然而，一个自定义模型只需要三个值（A,B）,(A,C),(B,C)。
Figure 10.12. The Cities application
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/EntryImages/20081026/10-12.JPG)
现在我们开始实现这个例子：
QStringList cities;cities << "Arvika" << "Boden" << "Eskilstuna" << "Falun"       << "Filipstad" << "Halmstad" << "Helsingborg" << "Karlstad"       << "Kiruna" << "Kramfors" << "Motala" << "Sandviken"       << "Skara" << "Stockholm" << "Sundsvall" << "Trelleborg";CityModel cityModel;cityModel.setCities(cities);QTableView tableView;tableView.setModel(&cityModel);tableView.setAlternatingRowColors(true);
像在前一个例子中一样，我们必须重新实现一些函数，此外，我们必须重新实现函数setData()和flags()，让这个数据模型能够编辑，下面是它的类定义：
class CityModel : public QAbstractTableModel{    Q_OBJECTpublic:    CityModel(QObject *parent = 0);    void setCities(const QStringList &cityNames);    int rowCount(const QModelIndex &parent) const;    int columnCount(const QModelIndex &parent) const;    QVariant data(const QModelIndex &index, int role) const;    bool setData(const QModelIndex &index, const QVariant &value,                 int role);    QVariant headerData(int section, Qt::Orientation orientation,                        int role) const;    Qt::ItemFlags flags(const QModelIndex &index) const;private:    int offsetOf(int row, int column) const;    QStringList cities;    QVector<int> distances;};
在这个模型中，我们使用两个数据结构：QStringList代表城市的名称，QVector<int>存储城市之间的距离值。
CityModel::CityModel(QObject *parent)    : QAbstractTableModel(parent){}
这个构造函数给基类传递了parent参数，其它什么都没有做。
int CityModel::rowCount(const QModelIndex & /* parent */) const{    return cities.count();}int CityModel::columnCount(const QModelIndex & /* parent */) const{    return cities.count();}
我们把城市放在一个正方形的网格中，行数和列数就是城市的数量。
QVariant CityModel::data(const QModelIndex &index, int role) const{    if (!index.isValid())        return QVariant();    if (role == Qt::TextAlignmentRole) {        return int(Qt::AlignRight | Qt::AlignVCenter);    } else if (role == Qt::DisplayRole) {        if (index.row() == index.column())            return 0;        int offset = offsetOf(index.row(), index.column());        return distances[offset];    }    return QVariant();}
函数data()的实现与CurrencyModel里的实现相似。如果行列一样，此时两个城市是一样的，返回0，否则，它根据行数列数得到对应的城市，计算出距离。
QVariant CityModel::headerData(int section,                               Qt::Orientation /* orientation */,                               int role) const{    if (role == Qt::DisplayRole)        return cities[section];    return QVariant();}
函数headerData()返回行，列标题，在这个例子中表为正方形表，行数列数相等，这里我们返回对应的城市名称。
bool CityModel::setData(const QModelIndex &index,                        const QVariant &value, int role){    if (index.isValid() && index.row() != index.column()            && role == Qt::EditRole) {        int offset = offsetOf(index.row(), index.column());        distances[offset] = value.toInt();        QModelIndex transposedIndex = createIndex(index.column(),                                                  index.row());        emit dataChanged(index, index);        emit dataChanged(transposedIndex, transposedIndex);        return true;    }    return false;}
函数setData()在编辑表中项目时调用。只要模型的索引有效，两个城市不同，要修改的是数据的Qt::EditRole属性，函数就保存用户输入的距离值。
函数createIndex()生成指定行，列的模型索引。用这个函数我们得到与对角线对称的另一个项目的模型索引，这两个距离是相等地。我们用当前列数表示行数，用行数表示列数，就得到对应的项目。
如果我们改变了一个项目，就发送dataChanged()信号。由于一个项目改变后可能会引起不止一行一列改变，因此信号函数中有两个索引，由这两个索引表示一个由行列组成的矩形区域，一个代表变化的左上角项目索引，另一个代表变化右下角项目索引。同时对角位置的项目也改变了，也发出dataChanged()信号。函数返回值为true或者false，表示编辑是否成功。
Qt::ItemFlags CityModel::flags(const QModelIndex &index) const{    Qt::ItemFlags flags = QAbstractItemModel::flags(index);    if (index.row() != index.column())        flags |= Qt::ItemIsEditable;    return flags;}
函数flags()用来表示模型的项目能做的工作（如是否可以编辑）。抽象类QAbstractTableModel中这个函数的返回的是Qt::ItemIsSelectable和Qt::ItemIsEnabled。在CityModel中增加了Qt::ItemIsEditable。对角线上的值总是0，不能编辑。
void CityModel::setCities(const QStringList &cityNames){    cities = cityNames;    distances.resize(cities.count() * (cities.count() - 1) / 2);    distances.fill(0);    reset();}
如果重新设置的城市的列表，我们用新的列表代替原城市列表，重新计算距离数组distances，调用QAbstactItemModel::reset()，这个函数来通知这个模型的所有视图重新获取数据。
int CityModel::offsetOf(int row, int column) const{    if (row < column)        qSwap(row, column);    return (row * (row - 1) / 2) + column;}
私有函数offsetOf()计算给定行，列的一个城市对的距离值在distances数组中的位置。例如，我们有四个城市A,B,C,D，如果用户改变了第3行第一列，在distances数组中的索引为3*(3-1)/2+1=4。如果用户改变了第一行第3列，调用qSwap()后，可以得到同样的偏移值。
Figure 10.13. The
cities and
distances data structures and the table model
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/EntryImages/20081026/10-13.JPG)
本节的最后一个例子是一个正则表达式的解析树模型。一个正则表达式包涵一个或者多个项（terms），项与项之间用“|”分开。因此表达式“alpha|bravo|Charlie”包含3个项。每一个项有包含多个因数（factors），例如bravo包含5个因数（每一个字母代表一个因数）。因数还可以进一步分解为原子（atom）和一个符号，如‘*’，‘+’，‘?’。一个正则表达式还可能包含表达式，解析可以是递归的
Figure 10.14. The Regexp Parser application
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/EntryImages/20081026/10-14.JPG)
RegExpParsery应用程序包含四个类:
·         RegExpWindow is 一个窗口，用户可以输入表达式，显示表达式的解析结构.
·         RegExpParser ，根据一个表达式生成一个解析树。
·         RegExpModel 用来表示解析树的一个树型模型。
·         Node 代表解析树中的一个项。
Node类定义如下:
class Node{public:    enum Type { RegExp, Expression, Term, Factor, Atom, Terminal };    Node(Type type, const QString &str = "");    ~Node();    Type type;    QString str;    Node *parent;    QList<Node *> children;};
每一个Node都有一个类型，一个字符串（可以为空），一个父节点（可以为空）和一个子节点列表（可以为空）。
Node::Node(Type type, const QString &str){    this->type = type;    this->str = str;    parent = 0;}
在构造函数中初始化成员变量type和str。Node中所有的成员变量都是公有的，可以对type，string，parent，children直接操作。
Node::~Node(){    qDeleteAll(children);}
函数qDeleteAll()遍历容器里所有的指针，然后进行删除。但是删除后它并不把指针设置为0，如果容器在析构函数以外使用，一般在语句的后面调用容器的clear()函数，清空所有的成员。
现在我们开始定义我的数据项（每一个数据项代表一个Node），首先我们创建一个模型：
class RegExpModel : public QAbstractItemModel{public:    RegExpModel(QObject *parent = 0);    ~RegExpModel();    void setRootNode(Node *node);    QModelIndex index(int row, int column,                      const QModelIndex &parent) const;    QModelIndex parent(const QModelIndex &child) const;    int rowCount(const QModelIndex &parent) const;    int columnCount(const QModelIndex &parent) const;    QVariant data(const QModelIndex &index, int role) const;    QVariant headerData(int section, Qt::Orientation orientation,                        int role) const;private:    Node *nodeFromIndex(const QModelIndex &index) const;    Node *rootNode;};
这次我们需要一个有继承关系的模型，模型类没有从QAbstractTableModel继承，而是从QAbstractItemModel继承。我们必需要重新实现的函数基本一样，在这个类中我们必须重新实现的函数还有index()和parent()。为了设置模型的数据，我们调用setRootNode()函数，作为解析树的根节点。
RegExpModel::RegExpModel(QObject *parent)    : QAbstractItemModel(parent){    rootNode = 0;}
在构造函数中，我们只是把根节点设置为0，把参数parent传递给基类。
RegExpModel::~RegExpModel(){    delete rootNode;}
在析构函数中我们删除根节点。如果根节点还有子节点，每一个子节点也会被Node的析构函数删除。
void RegExpModel::setRootNode(Node *node){    delete rootNode;    rootNode = node;    reset();}
从新设置根节点前，我们删除原来的根节点（所有的子节点也一同删除）。然后设置新的根节点并调用call()函数，通知视图类刷新数据显示。
QModelIndex RegExpModel::index(int row, int column,                               const QModelIndex &parent) const{    if (!rootNode)        return QModelIndex();    Node *parentNode = nodeFromIndex(parent);    return createIndex(row, column, parentNode->children[row]);}
函数index()是QAbstractItemModel的重新实现。由模型类或者视图类需要得到一个子节点时调用（最顶层节点是一个无效的QModelIndex）。对于表格式和列表式的模型，QAbstractTableModel和QAbstractListModel的实现就是正确的，我们就不需要重新四实现这个函数。
在我们重新实现的index()函数中，如果没有根节点，我们返回一个无效的QModelIndex。如果有根节点，我们用行数，列数和子节点Node参数创建一个QModelIndex。对于有继承层次的模型，只知道行数，列数还不足以确定一个子节点。我们必须知道它的父节点。为了得到父节点，我们在QModelIndex中保存一个指向Node的指针。QModelIndex中除了可以保存一个行数，列数外，还可以保存一个void*指针或者一个int型数值。
子节点的Node*通过父节点的子节点列表得到。父节点由其QModelIndex索引和私有函数nodeFromIndex()得到：
Node *RegExpModel::nodeFromIndex(const QModelIndex &index) const{    if (index.isValid()) {        return static_cast<Node *>(index.internalPointer());    } else {        return rootNode;    }}
函数nodeFromIndex()将给定索引的void*转换为一个Node*，如果给定的索引是无效的，就返回根节点rootNode，在模型类中，一个无效的模型索引指向的就是根节点。
int RegExpModel::rowCount(const QModelIndex &parent) const{    Node *parentNode = nodeFromIndex(parent);    if (!parentNode)        return 0;    return parentNode->children.count();}
对于一个给定的节点，行数就是它的子节点的个数。
int RegExpModel::columnCount(const QModelIndex & /* parent */) const{    return 2;}
列数在这个为2，第一列为节点的类型，第二列为节点的值。
QModelIndex RegExpModel::parent(const QModelIndex &child) const{    Node *node = nodeFromIndex(child);    if (!node)        return QModelIndex();    Node *parentNode = node->parent;    if (!parentNode)        return QModelIndex();    Node *grandparentNode = parentNode->parent;    if (!grandparentNode)        return QModelIndex();    int row = grandparentNode->children.indexOf(parentNode);    return createIndex(row, child.column(), parentNode);}
通过一个子节点的QModelIndex得到父节点要比通过父节点得到子节点困难些。调用nodeFromIndex()可以得到父节点Node指针，但是要想得到父节点所在的行数，我们需要得到父节点的父节点，
QVariant RegExpModel::data(const QModelIndex &index, int role) const{    if (role != Qt::DisplayRole)        return QVariant();    Node *node = nodeFromIndex(index);    if (!node)        return QVariant();    if (index.column() == 0) {        switch (node->type) {        case Node::RegExp:            return tr("RegExp");        case Node::Expression:            return tr("Expression");        case Node::Term:            return tr("Term");        case Node::Factor:            return tr("Factor");        case Node::Atom:            return tr("Atom");        case Node::Terminal:            return tr("Terminal");        default:            return tr("Unknown");        }    } else if (index.column() == 1) {        return node->str;    }    return QVariant();}
在data()函数中，我们得到给定索引的Node*，利用这个指针得到数据。如果编辑方式不是Qt::DisplayRole，或者不能得到一个有效的指针，我们返回一个无效的QVariant类型的数据。如果是第0列，我们根据节点的类型设置显示的名称，如果是第1列，我们返回节点的值。
QVariant RegExpModel::headerData(int section,                                 Qt::Orientation orientation,                                 int role) const{    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {        if (section == 0) {            return tr("Node");        } else if (section == 1) {            return tr("Value");        }    }    return QVariant();}
在函数headerData()中，我们返回水平方向的标题。QTreeView类在显示有层次结构的模型时没有竖直方向的标题，在函数中没有进行处理竖直方向的标题。
现在我们已经完成了Node和RegExpModel，当用户改变文本时，根节点就需要重新创建。
void RegExpWindow::regExpChanged(const QString ®Exp){    RegExpParser parser;    Node *rootNode = parser.parse(regExp);    regExpModel->setRootNode(rootNode);}
当用户改变输入的表达式时，主窗口的regExpChanged()就被调用。在这个函数中，解析器解析用户输入的表达式，解析器返回的指针作为解析树的根节点。
在这里没有介绍RegExpParser类，这个类不设计GUI和模型视图编程。完整的代码在书的CD上。
在这一节中，我们介绍了三个不同的自定义模型的创建。很多模型的项目和模型索引之间是一对一的关系，要比这里提到的要简单。更多的有关模型视图编程的例子可以参考Qt提供的文档。
RegExpParsery应用程序包含四个类:
·         RegExpWindow is 一个窗口，用户可以输入表达式，显示表达式的解析结构.
·         RegExpParser ，根据一个表达式生成一个解析树。
·         RegExpModel 用来表示解析树的一个树型模型。
·         Node 代表解析树中的一个项。
Node类定义如下:
class Node{public:    enum Type { RegExp, Expression, Term, Factor, Atom, Terminal };    Node(Type type, const QString &str = "");    ~Node();    Type type;    QString str;    Node *parent;    QList<Node *> children;};
每一个Node都有一个类型，一个字符串（可以为空），一个父节点（可以为空）和一个子节点列表（可以为空）。
Node::Node(Type type, const QString &str){    this->type = type;    this->str = str;    parent = 0;}
在构造函数中初始化成员变量type和str。Node中所有的成员变量都是公有的，可以对type，string，parent，children直接操作。
Node::~Node(){    qDeleteAll(children);}
函数qDeleteAll()遍历容器里所有的指针，然后进行删除。但是删除后它并不把指针设置为0，如果容器在析构函数以外使用，一般在语句的后面调用容器的clear()函数，清空所有的成员。
现在我们开始定义我的数据项（每一个数据项代表一个Node），首先我们创建一个模型：
class RegExpModel : public QAbstractItemModel{public:    RegExpModel(QObject *parent = 0);    ~RegExpModel();    void setRootNode(Node *node);    QModelIndex index(int row, int column,                      const QModelIndex &parent) const;    QModelIndex parent(const QModelIndex &child) const;    int rowCount(const QModelIndex &parent) const;    int columnCount(const QModelIndex &parent) const;    QVariant data(const QModelIndex &index, int role) const;    QVariant headerData(int section, Qt::Orientation orientation,                        int role) const;private:    Node *nodeFromIndex(const QModelIndex &index) const;    Node *rootNode;};
这次我们需要一个有继承关系的模型，模型类没有从QAbstractTableModel继承，而是从QAbstractItemModel继承。我们必需要重新实现的函数基本一样，在这个类中我们必须重新实现的函数还有index()和parent()。为了设置模型的数据，我们调用setRootNode()函数，作为解析树的根节点。
RegExpModel::RegExpModel(QObject *parent)    : QAbstractItemModel(parent){    rootNode = 0;}
在构造函数中，我们只是把根节点设置为0，把参数parent传递给基类。
RegExpModel::~RegExpModel(){    delete rootNode;}
在析构函数中我们删除根节点。如果根节点还有子节点，每一个子节点也会被Node的析构函数删除。
void RegExpModel::setRootNode(Node *node){    delete rootNode;    rootNode = node;    reset();}
从新设置根节点前，我们删除原来的根节点（所有的子节点也一同删除）。然后设置新的根节点并调用call()函数，通知视图类刷新数据显示。
QModelIndex RegExpModel::index(int row, int column,                               const QModelIndex &parent) const{    if (!rootNode)        return QModelIndex();    Node *parentNode = nodeFromIndex(parent);    return createIndex(row, column, parentNode->children[row]);}
函数index()是QAbstractItemModel的重新实现。由模型类或者视图类需要得到一个子节点时调用（最顶层节点是一个无效的QModelIndex）。对于表格式和列表式的模型，QAbstractTableModel和QAbstractListModel的实现就是正确的，我们就不需要重新四实现这个函数。
在我们重新实现的index()函数中，如果没有根节点，我们返回一个无效的QModelIndex。如果有根节点，我们用行数，列数和子节点Node参数创建一个QModelIndex。对于有继承层次的模型，只知道行数，列数还不足以确定一个子节点。我们必须知道它的父节点。为了得到父节点，我们在QModelIndex中保存一个指向Node的指针。QModelIndex中除了可以保存一个行数，列数外，还可以保存一个void*指针或者一个int型数值。
子节点的Node*通过父节点的子节点列表得到。父节点由其QModelIndex索引和私有函数nodeFromIndex()得到：
Node *RegExpModel::nodeFromIndex(const QModelIndex &index) const{    if (index.isValid()) {        return static_cast<Node *>(index.internalPointer());    } else {        return rootNode;    }}
函数nodeFromIndex()将给定索引的void*转换为一个Node*，如果给定的索引是无效的，就返回根节点rootNode，在模型类中，一个无效的模型索引指向的就是根节点。
int RegExpModel::rowCount(const QModelIndex &parent) const{    Node *parentNode = nodeFromIndex(parent);    if (!parentNode)        return 0;    return parentNode->children.count();}
对于一个给定的节点，行数就是它的子节点的个数。
int RegExpModel::columnCount(const QModelIndex & /* parent */) const{    return 2;}
列数在这个为2，第一列为节点的类型，第二列为节点的值。
QModelIndex RegExpModel::parent(const QModelIndex &child) const{    Node *node = nodeFromIndex(child);    if (!node)        return QModelIndex();    Node *parentNode = node->parent;    if (!parentNode)        return QModelIndex();    Node *grandparentNode = parentNode->parent;    if (!grandparentNode)        return QModelIndex();    int row = grandparentNode->children.indexOf(parentNode);    return createIndex(row, child.column(), parentNode);}
通过一个子节点的QModelIndex得到父节点要比通过父节点得到子节点困难些。调用nodeFromIndex()可以得到父节点Node指针，但是要想得到父节点所在的行数，我们需要得到父节点的父节点，
QVariant RegExpModel::data(const QModelIndex &index, int role) const{    if (role != Qt::DisplayRole)        return QVariant();    Node *node = nodeFromIndex(index);    if (!node)        return QVariant();    if (index.column() == 0) {        switch (node->type) {        case Node::RegExp:            return tr("RegExp");        case Node::Expression:            return tr("Expression");        case Node::Term:            return tr("Term");        case Node::Factor:            return tr("Factor");        case Node::Atom:            return tr("Atom");        case Node::Terminal:            return tr("Terminal");        default:            return tr("Unknown");        }    } else if (index.column() == 1) {        return node->str;    }    return QVariant();}
在data()函数中，我们得到给定索引的Node*，利用这个指针得到数据。如果编辑方式不是Qt::DisplayRole，或者不能得到一个有效的指针，我们返回一个无效的QVariant类型的数据。如果是第0列，我们根据节点的类型设置显示的名称，如果是第1列，我们返回节点的值。
QVariant RegExpModel::headerData(int section,                                 Qt::Orientation orientation,                                 int role) const{    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {        if (section == 0) {            return tr("Node");        } else if (section == 1) {            return tr("Value");        }    }    return QVariant();}
在函数headerData()中，我们返回水平方向的标题。QTreeView类在显示有层次结构的模型时没有竖直方向的标题，在函数中没有进行处理竖直方向的标题。
现在我们已经完成了Node和RegExpModel，当用户改变文本时，根节点就需要重新创建。
void RegExpWindow::regExpChanged(const QString ®Exp){    RegExpParser parser;    Node *rootNode = parser.parse(regExp);    regExpModel->setRootNode(rootNode);}
当用户改变输入的表达式时，主窗口的regExpChanged()就被调用。在这个函数中，解析器解析用户输入的表达式，解析器返回的指针作为解析树的根节点。
在这里没有介绍RegExpParser类，这个类不设计GUI和模型视图编程。完整的代码在书的CD上。
在这一节中，我们介绍了三个不同的自定义模型的创建。很多模型的项目和模型索引之间是一对一的关系，要比这里提到的要简单。更多的有关模型视图编程的例子可以参考Qt提供的文档。
