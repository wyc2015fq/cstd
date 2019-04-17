# 在Pocket PC应用程序中使用DataGrid控件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年12月11日 09:29:33[boonya](https://me.csdn.net/boonya)阅读数：831标签：[.net compact framewo																[datagrid](https://so.csdn.net/so/search/s.do?q=datagrid&t=blog)](https://so.csdn.net/so/search/s.do?q=.net compact framewo&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)








原文地址:[http://blog.csdn.net/aawolf/article/details/7537](http://blog.csdn.net/aawolf/article/details/7537)



# 在Pocket PC应用程序中使用DataGrid控件



Christian Forsberg

businessanyplace.net

September 2003

Applies to:

    Microsoft® .NET Compact Framework 1.0

    Microsoft Visual Studio® .NET 2003

**摘要：**学习如何在你的Pocket PC应用程序中如何有效地使用DataGrid控件进行数据查看和更新。这篇文章将解释，为什么这是重要的，并向你演示如何做。

Download [datagrid_control.exe](http://download.microsoft.com/download/f/9/0/f90b348a-5a35-43ea-9e51-269cb03f6aa9/datagrid_control.exe)

#### Contents

[Table
 Data](http://msdn.microsoft.com/library/en-us/dnppc2k3/html/#datagrid_control_topic1)
[The DataGrid Control](http://msdn.microsoft.com/library/en-us/dnppc2k3/html/#datagrid_control_topic2)
[DataGrid Sample](http://msdn.microsoft.com/library/en-us/dnppc2k3/html/#datagrid_control_topic3)
[Code Walkthrough](http://msdn.microsoft.com/library/en-us/dnppc2k3/html/#datagrid_control_topic4)
[Conclusion](http://msdn.microsoft.com/library/en-us/dnppc2k3/html/#datagrid_control_topic5)

## Table Data

      在实现企业级Pocket PC应用程序时，几乎总是需要以表格的方式浏览数据。大多数通用的方案是你输入许多查找标准，得到作为一个表的查找结果集，并从中搜索或选择。一个表格是一个非常直接的显示数据的途径，可以被你直接使用与屏幕空间中。

## The DataGrid Control

     包括在Microsoft® Visual Studio® .NET 2003中的DataGrid控件是一个强大的控件，使你能够以多种方式查看数据。这个控件可以被绑定到一个DataTable,DataView,ArrayList或者任何其他支持IlistSource或者Ilist接口的对象中。

让我们来看DataGrid控件是如何被用于一个Pocket
 PC应用程序实例的。

## DataGrid Sample

     这个Pocket PC的应用程序实例使用Visual
 Studio .NET 2003, C#, 和[Microsoft
 .NET Compact Framework](http://msdn.microsoft.com/vstudio/device/compactfx.asp)创建。它将演示如何使用DataGrid控件查看并更新数据。这个程序由一个form组成：

![](http://blog.csdn.net/Develop/ArticleImages/21/21844/CSDN_Dev_Image_2003-10-291436460.gif)

**Figure 1. DataGrid sample**

     这个实例开始时载入订单表（一个来自Northwind sample database的原始订单表的子集）到一个grid中，然后使用多种column
 style来创建一个好看的界面。点击column header可以对每一个column进行排序，再点击排序后的column
 header可以在升序和降序间切换。

一个非常好的功能是grid中的每一个cell都是可编辑的。当一个cell被点击并获得输入焦点时，cell中的值可以被更新（见图一）。当其他cell被选择时，被编辑过的cell被更新，新的cell变为可编辑。注意，当一个cell得到输入焦点，软键盘（SIP）是被自动激活（弹出）。一个可能的增进是例子可以拖动被选择的cell到可见，当它被SIP覆盖的时候。

现在，让我们看一下代码是如何实现的。



## Code Walkthrough
    在实例代码中，DataGrid控件被命名为grdOrders。将订单表（被存成XML文件）加载到DataGrid控件，你只需要下面的代码：

```
DataSet ds = new DataSet();
ds.ReadXml(@"/Program Files/DGrid/orders.xml");
grdOrders.DataSource = ds.Tables[0];
```
事实上，如果你只做了这些，那么将得到一个非常简单的grid，每个column header上将会显示表字段的名字。为了让grid看上去更好，使用table styles。一个table style可以包含多种table styles——你想在grid中查看每个column。在这个例子中，columns包括订购信息和收货人姓名。下面的代码设置状态：

```
DataGridTableStyle ts = new DataGridTableStyle();
ts.MappingName = "Order";
 
// Order date column style
DataGridColumnStyle orderDate = new DataGridTextBoxColumn();
orderDate.MappingName = "OrderDate";
orderDate.HeaderText = "Date";
ts.GridColumnStyles.Add(orderDate);
 
// Shipping name column style
DataGridColumnStyle shipName = new DataGridTextBoxColumn();
shipName.MappingName = "ShipName";
shipName.HeaderText = "Customer";
shipName.Width = this.Width - orderDate.Width - 37;
ts.GridColumnStyles.Add(shipName);
 
grdOrders.TableStyles.Add(ts);
```
每个column style(DataGridColumnStyle)设置一个表字段名称映射到(MappingName property)，header text用于显示(HeaderText property)，并自定义column宽度(Width property)。两个column styles被加入到table style中，最后将table style加入到DataGrid控件(grdOrders)中。在这个例子里styles是被代码创建的，但是styles同样可以在forms
 designer中被创建。


当这个grid看上去很好时，有许多common features在grid中可以被支持。编辑就是其中之一。但编辑在DataGrid控件中不是被自动支持的，这就需要手工实现。一种方法就是使用一个隐藏的TextBox控件，当一个cell被选中时显示，当编辑结束时隐藏。这就需要你声明一些变量：

```
private DataGridCell editCell;
private bool inEditMode = false;
private bool inUpdateMode = false;
```
我们需要获取哪个cell被编辑激活(editCell)，这个cell是否在编辑状态(inEditMode)，和我们是否正在更新一个cell(inUpdateMode).控制编辑代码如下：

```
private void grdOrders_CurrentCellChanged(object sender,
  System.EventArgs e)
{
  if (!inUpdateMode)
  {
    if (inEditMode && !grdOrders.CurrentCell.Equals(editCell))
    {
      // Update edited cell
      inUpdateMode = true;
      grdOrders.Visible = false;
      DataGridCell currentCell = grdOrders.CurrentCell;
      grdOrders[editCell.RowNumber, editCell.ColumnNumber] =
        txtEdit.Text;
      grdOrders.CurrentCell = currentCell;
      grdOrders.Visible = true;
      inUpdateMode = false;
      txtEdit.Visible = false;
      inEditMode = false;
    }
 
    // Enter edit mode
    editCell = grdOrders.CurrentCell;
    txtEdit.Text = (string)grdOrders[editCell.RowNumber,
      editCell.ColumnNumber];
    Rectangle cellPos = grdOrders.GetCellBounds(editCell.RowNumber,
      editCell.ColumnNumber);
    txtEdit.Left = cellPos.Left - 1;
    txtEdit.Top = cellPos.Top + grdOrders.Top - 1;
    txtEdit.Width = cellPos.Width + 2;
    txtEdit.Height = cellPos.Height + 2;
    txtEdit.Visible = true;
    inEditMode = true;
  }
}
```
当一个cell被点击（inEditMode和inUpdate被设置为false）时，当前的cell被保存(editCell)。然后，隐藏的TextBox(txtEdit)得到当前cell的内容，重新设置到当前cell的位置，并使其可见(在当前的cell上边)。当编辑完成、其他cell被选择时，事件代码不可用(inUpdateMode设置成true),当被编辑的cell从TextBox控件得到后，将新的cell保存(currentCell)。在update过程中，grid没有被update(Visible设置成false)，当update完成TextBox被重新隐藏。


其他的common grid feature是通过点击column header进行排序。代码如下：

```
private void grdOrders_MouseUp(object sender,
  System.Windows.Forms.MouseEventArgs e)
{
  DataGrid.HitTestInfo hitTest = grdOrders.HitTest(e.X, e.Y);
  if (hitTest.Type == DataGrid.HitTestType.ColumnHeader)
  {
    // Exit edit mode
    txtEdit.Visible = false;
    inEditMode = false;
 
    // Sort tapped column
    DataTable dataTable = (DataTable)grdOrders.DataSource;
    DataView dataView = dataTable.DefaultView;
    string columnName = dataTable.Columns[hitTest.Column].ColumnName;
    if (dataView.Sort == columnName)
      dataView.Sort = columnName + " DESC";
    else
      dataView.Sort = columnName;
  }
}
```
如果header被点击，第一件事是确保任何当前cell的编辑被取消。然后一个DataView被创建并通过点击的column排序。如果一个column再次得到点击事件而排序顺序将被改成递减。后来的点击将使排序在递增和递减间切换，直到其他的column被点击。

### Conclusion

      DataGrid控件是一个查看（然后是编辑）数据的强大工具，用于你的Pocket PC应用程序。这个控件提供多样性的features，而且这篇文章包含一些有趣的特性：数据绑定，grid内编辑，排序。你的下一步是在这个例子上做一些深层次的开发，探索这个控件中更多更cool的窍门。




另外还可以参考这里来实现DataGrid的使用：[http://www.cnblogs.com/dearsj001/archive/2009/07/30/WMOwnerDrawnGrid.html](http://www.cnblogs.com/dearsj001/archive/2009/07/30/WMOwnerDrawnGrid.html)







