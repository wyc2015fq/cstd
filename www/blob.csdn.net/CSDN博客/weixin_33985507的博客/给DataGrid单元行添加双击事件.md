# 给DataGrid单元行添加双击事件 - weixin_33985507的博客 - CSDN博客
2004年08月16日 14:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**给DataGrid单元行添加双击事件**
**hbzxf（阿好）[http://www.cnblogs.com/hbzxf](http://www.cnblogs.com/hbzxf)**
**        现在我需要做到的功能是当我单击DataGrid某行时显示相对应选中的数据信息，在双击此相同行时弹出删除对话框，应该怎么做呢。由于单击问题很简单就不再阐述了，下面我说一下双击事件是怎么实现的。**
**        这里用到了DataGrid的ItemDataBound事件，我们可以把下面的代码加入到所需的程序中就可实现双击的功能。详细源码如下：**
private void DataGrid1_ItemDataBound(object sender, System.Web.UI.WebControls.DataGridItemEventArgs e)
  {
   if ((e.Item.ItemType == ListItemType.Item) || (e.Item.ItemType == ListItemType.AlternatingItem) || (e.Item.ItemType == ListItemType.SelectedItem) ) 
   {
    e.Item.Attributes.Add ("ondblclick", "javascript:return confirm('确定删除" + e.Item.Cells[1].Text + "?');");
   }
  }
**其实就是一个小技巧**
