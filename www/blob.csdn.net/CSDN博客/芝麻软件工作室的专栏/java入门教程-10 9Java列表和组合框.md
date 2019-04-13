
# java入门教程-10.9Java列表和组合框 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:48:24[seven-soft](https://me.csdn.net/softn)阅读数：672个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



列表和组合框是又一类供用户选择的界面组件，用于在一组选择项目选择，组合框还可以输入新的选择。
## 列表
列表(JList)在界面中表现为列表框，是JList类或它的子类的对象。程序可以在列表框中加入多个文本选择项条目。列表事件的事件源有两种：一是鼠标双击某个选项：双击选项是动作事件，与该事件相关的接口是ActionListener，注册监视器的方法是addActionListener()，接口方法是actionPerformed(ActionEvent e)。
二是鼠标单击某个选项：单击选项是选项事件，与选项事件相关的接口是ListSelectionListener,注册监视器的方法是addListSelectionListener,接口方法是valueChanged(ListSelectionEvent e)。
JList 类的常用构造方法：JList()：建立一个列表。
JList(String list[])：建立列表，list是字符串数组，数组元素是列表的选择条目。
JList类的常用方法：getSelectedIndex()：获取选项的索引。返回最小的选择单元索引；只选择了列表中单个项时，返回该选择。
getSelectedValue()：获取选项的值。
getSelectedIndices()：返回所选的全部索引的数组（按升序排列）。
getSelectedValues(),：返回所有选择值的数组，根据其列表中的索引顺序按升序排序。
getItemCount()：获取列表中的条数。
setVisibleRowCount(int n)：设置列表可见行数。
setSelectionMode(int seleMode)：设置列表选择模型。选择模型有单选和多选两种。单选：ListSelectionModel.SINGLE_SELECTION.
多选：ListSelectionModel.MULTIPLE.INTERVAL_SELECTION.
remove(int n)：从列表的选项菜单中删除指定索引的选项。
removeAll()：删除列表中的全部选项。
列表可以添加滚动条，列表添加滚动条的方法是先创建列表，然后再创建一个JScrollPane滚动面板对象，在创建滚动面板对象时指定列表。以下代码示意为列表list2添加滚动条：
JScrollPane jsp = new JScrollPane(list2);
【例 11-13】小应用程序有两个列表，第一个列表只允许单选，第二个列表允许多选（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-13.txt)）。
## 组合框
组合框(JComboBox)是文本框和列表的组合，可以在文本框中输入选项，也可以单击下拉按钮从显示的列表中进行选择。
组合框的常用构造方法：JComboBox()：建立一个没有选项的JComboBox对象。
JComboBox(JComboBoxModel aModel)：用数据模型建立一个JComboBox对象。
JComboBox(Object[]items)：利用数组对象建立一个JComboBox对象。
组合框的其他常用方法有以下几个：addItem(Object obj)：向组合框加选项。
getItemCount()：获取组合框的条目总数。
removeItem(Object ob)：删除指定选项。
removeItemAt(int index)：删除指定索引的选项。
insertItemAt(Object ob,int index)：在指定的索引处插入选项。
getSelectedIndex()：获取所选项的索引值(从0开始)。
getSelectedItem()：获得所选项的内容。
setEditable(boolean b)：设为可编辑。组合框的默认状态是不可编辑的，需要调用本方法设定为可编辑，才能响应选择输入事件。
在JComboBox对象上发生事件分为两类。一是用户选定项目，事件响应程序获取用户所选的项目。二是用户输入项目后按回车键，事件响应程序读取用户的输入。第一类事件的接口是ItemListener;第二类事件是输入事件，接口是ActionListener。
【例 11-14】一个说明组合框用法的应用程序。程序中声明的组合框子类实现ItemLister接口和ActionListener接口。组合框子类的窗口中设置了一个文本框和一个组合框，组合框中有三个选择。实现接口的监视方法将组合框的选择结果在文本框中显示（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-14.txt)）。

