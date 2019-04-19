# Qt UserRole详解 - fanyun的博客 - CSDN博客
2017年11月26日 21:49:34[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3458
       Qt UserRole就是组件角色，在复杂系统中使用的比较多，原因也很简单，系统复杂，为了组件区分方便。   
       在Qt中很多类是可以给他添加角色值的，比如说QComboBox中的setItemData()与QStandardItemModel中的setData()这两个函数，都是在Index位置上添加角色值。这两个函数原型如下：
```cpp
void QComboBox::setItemData ( int index, const QVariant & value, int role = Qt::UserRole );
bool QStandardItemModel::setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
```
       如果想在同一Index上添加多个角色值，我们可以利用下面的方法添加：
```cpp
QStandardItemModel *pModel = new QStandardItemModel;
pModel->setData(pModel->index(0, 0), 1, Qt::UserRole);
pModel->setData(pModel->index(0, 0), 2, Qt::UserRole + 1);
```
       这种方法多于表格处理，或者是QComboBox中需要获取其他信息。
       在默认情况下，我们在利用这样的函数时我们并不加添加角色的，如果想利用这个功能我们就可以使用上面的方法来做。这里给大家提一个特例，QComboBox中的addItem()函数，他默认情况下是直接给Qt::UserRole这个角色添加值，而函数的原型中却没有给出这个角色，原型如下：
```cpp
void QComboBox::addItem ( const QString & text, const QVariant & userData = QVariant() );
Adds an item to the combobox with the given text, and containing the specified userData (stored in the Qt::UserRole). The item is appended to the list of existing items.
```
