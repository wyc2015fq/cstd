# python QQTableView中嵌入复选框CheckBox四种方法 - _天枢 - 博客园
## [python QQTableView中嵌入复选框CheckBox四种方法](https://www.cnblogs.com/yhleng/p/6922860.html)
2017-05-31 10:17 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=6922860)
搜索了一下，QTableView中嵌入复选框CheckBox方法有**四种**：
        第一种不能之前显示，必须双击/选中后才能显示，不适用。
        第二种比较简单，通常用这种方法。
        第三种只适合静态显示静态数据用
        第四种比较适合扩展，它除了可以嵌入复选框，还可以通过paint()绘制其它控件，图片等自定义风格。
**第一种方法是：编辑委托法**
这种方法直接利用委托中重载createEditor()，激活QCheckBox，这个缺点是必须双击/选中，才能显示CheckBox控件。一般不满足我们实际中的直接显示的需要。可以参考Qt中的QSpinBoxDelegate例子。
**第二种方法是：[设置QAbstractTableModel的flags()函数法。](http://blog.csdn.net/gong_xucheng/article/details/21523485)**
通过Delegate创建QCheckBox来实现的Check列，只有在该列进入编辑模式时才能够Check/Uncheck。这显然不是我们想要的，网上翻来翻去，在一个国外论坛中看到了无需Delegate的实现方法，只需重写Model即可：
主要是修改两个函数：
//设置某一列为可选角色，绘画出QCheckBox
Qt::ItemFlags flags(const QModelIndex &index) const; 
//根据界面选择QCheckbox，修改Model中的数据
 bool setData(const QModelIndex &index, const QVariant &value, int role);
**[cpp]**[view plain](http://blog.csdn.net/gong_xucheng/article/details/21523485#)[copy](http://blog.csdn.net/gong_xucheng/article/details/21523485#)
[print](http://blog.csdn.net/gong_xucheng/article/details/21523485#)[?](http://blog.csdn.net/gong_xucheng/article/details/21523485#)
- 2.在StudentInfoModel .h头文件中的主要代码：    
- class StudentInfoModel : public QAbstractTableModel     
- {    
-     Q_OBJECT    
- public:    
-     StudentInfoModel(const int totalColumn, const int aColumnNumWithChechBox = 0, QObject *parent = 0)    
-     :totalColumn(totalColumn),colNumberWithCheckBox(aColumnNumWithChechBox),    
- 
-     QAbstractTableModel(parent) {rowCheckStateMap.clear();};    
- public:    
- int rowCount(const QModelIndex &parent = QModelIndex()) const;    
- int columnCount(const QModelIndex &parent = QModelIndex()) const;    
-     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;    
-     QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;    
-     Qt::ItemFlags flags(const QModelIndex &index) const;    
- bool setData(const QModelIndex &index, const QVariant &value, int role);    
- 
- 
- public:    
- void AddStudentInfo(const StudentInfo &studentInfo);    
- 
- 
-     signals:    
- void StudentInfoIsChecked(const StudentInfo &studentInfo);    
- 
- 
- private:    
- typedef QVector<StudentInfo> StudentInfos;    
-     StudentInfos studentInfos;    
- int totalColumn;    
- int colNumberWithCheckBox;    
-     QMap<int, Qt::CheckState> rowCheckStateMap;    
- };    
- 
- 
- 3.在StudentInfoModel.cpp文件中的主要代码如下：    
- QVariant StudentInfoModel::data( const QModelIndex &index, int role ) const    
- {    
- if (role == Qt::DisplayRole)     
-     {     
- if (index.column() == 0)     
- return QString::number(index.row()+1);     
- if (index.column() == 1)     
- return studentInfos[index.row()].stuNumber;     
- if (index.column() == 2)    
- return studentInfos[index.row()].stuName;     
- if (index.column() == 3)    
- return studentInfos[index.row()].stuID;     
- if (index.column() == 4)    
- return studentInfos[index.row()].stuPhoneNumber;    
- if (index.column() == 5)     
- return studentInfos[index.row()].department;     
- if (index.column() == 6)     
- return studentInfos[index.row()].stuDescription;     
-     }     
- if (role == Qt::CheckStateRole)     
-     {     
- if (index.column() == colNumberWithCheckBox)     
-         {     
- if (rowCheckStateMap.contains(index.row()))     
- return rowCheckStateMap[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked; return Qt::Unchecked;     
-         }     
-     }     
- return QVariant();    
- }    
- 
- 
- Qt::ItemFlags StudentInfoModel::flags( const QModelIndex &index ) const    
- {    
- if    
-     (!index.isValid())    
- return 0;    
- 
- 
- if (index.column() == colNumberWithCheckBox)    
- return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;    
- 
- 
- return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;    
- }    
- 
- 
- bool StudentInfoModel::setData( const QModelIndex &index, const QVariant &value, int role )    
- {    
- if(!index.isValid())    
- return false;    
- if (role == Qt::CheckStateRole && index.column() == colNumberWithCheckBox)    
-     {    
- if (value == Qt::Checked) //    
-         {    
-             rowCheckStateMap[index.row()] = Qt::Checked;     
- if(studentInfos.size() > index.row())    
-             emit StudentInfoIsChecked(studentInfos[index.row()]);    
-         }    
- else    
-         {    
-             rowCheckStateMap[index.row()] = Qt::Unchecked;    
-         }     
-     }    
- return true;    
- }    
![](http://dl.iteye.com/upload/attachment/0068/5173/3b52b279-3f73-3744-8d9b-6b7177c8e853.jpg)
**第三种方法是：用QTableView中的方法void setIndexWidget(const QModelIndex &index, QWidget *widget)来设置QCheckBox。**
代码：setIndexWidget(index, new QTextEdit);
 Qt Assistant 写道
The items shown in a table view, like those in the other item views, are rendered and edited using standard delegates. However, for some tasks it is sometimes useful to be able to insert widgets in a table instead. Widgets are set for particular indexes with the setIndexWidget() function, and later retrieved with indexWidget().
    Qt Assistant **写道关于setIndexWidget()**
Sets the given widget on the item at the given index, passing the ownership of the widget to the viewport.
If index is invalid (e.g., if you pass the root index), this function will do nothing.
The given widget's autoFillBackground property must be set to true, otherwise the widget's background will be transparent, showing both the model data and the item at the given index.
If index widget A is replaced with index widget B, index widget A will be deleted. For example, in the code snippet below, the QLineEdit object will be deleted.
setIndexWidget(index, new QLineEdit);
...
setIndexWidget(index, new QTextEdit);
This function should only be used to display static content within the visible area corresponding to an item of data. If you want to display custom dynamic content or implement a custom editor widget, subclass QItemDelegate instead.
此功能只应该用来显示可视区域内对应一个数据项的静态内容。如果你想显示自定义的动态内容或执行自定义编辑器部件，子类化QItemDelegate代替。
就是说这个只适合用来做不变数据的显示,而不适合做一些会产生插入,更新,删除这样的操作的数据显示.
**第四种方法是：[实现QAbstractItemDelegate的paint()函数](http://blog.csdn.net/gong_xucheng/article/details/21523565)。**
