# Aspose.Words基本操作 - 毛小亮 - 博客园
# [Aspose.Words基本操作](https://www.cnblogs.com/xianerwonder/p/4552304.html)
一个非常好用的控件。
基本的操作可以参考伍大神的文章：
http://www.cnblogs.com/wuhuacong/archive/2012/08/30/2662961.html
在这里补充四点：
1、如果原表的宽度是指定宽度（固定宽度），那么如果要让新表的宽度值与原表相同，则必须加入以下代码：
builder.CellFormat.FitText = true;
2、在操作表时，如果要删除表中行或者MoveToCell至前面的表（即利用tableIndex这个参数来导航），需要结束表编辑：
builder.EndTable();
3、需要操作word时，Document新建对象→ DocumentBuilder 新建对象→DocumentBuilder.MovetoCell(或者MoveToBookmark)→DocumentBuilder.Write→Document.Save();
4、需要操作excel时：Workbook新建对象→Worksheet sheet = book.Worksheet[0];→Cells cells = sheet.Cells;→cells[0,0].PutValue();→book.Save();
再提出一个疑问：
我在书签处insertCell的时候，程序会新建表，而不是承接上面的表，跟伍大神的效果不同，不知为何？？？

