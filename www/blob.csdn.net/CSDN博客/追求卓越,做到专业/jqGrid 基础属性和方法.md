
# jqGrid 基础属性和方法 - 追求卓越,做到专业 - CSDN博客


2012年10月26日 13:07:45[Waldenz](https://me.csdn.net/enter89)阅读数：4918


jqGrid SDK:
属性:[http://www.trirand.com/jqgridwiki/doku.php?id=wiki:options](http://www.trirand.com/jqgridwiki/doku.php?id=wiki:options)
方法:[http://www.trirand.com/jqgridwiki/doku.php?id=wiki:methods](http://www.trirand.com/jqgridwiki/doku.php?id=wiki:methods)
事件:[http://www.trirand.com/jqgridwiki/doku.php?id=wiki:events](http://www.trirand.com/jqgridwiki/doku.php?id=wiki:events)

**属性:**
1. 获取选中行的ID :  $(this).getGridParam("selrow") ;
2. 获取行数.
var count = $(this).getGridParam("reccount");  当前页的行数
var records = $(this).getGridParam("records"); 总行数
(如果grid显示为分页的话,定义`rowNum为20(即每页显示20行),server返回30行记录,则count=20, records=30)`

**方法:**
1. 动态增加一条数据
addRowData(rowid,data,position,srcrowid): 这个方法用于向Grid中插入新的一行。执行成功返回true，否则返回false。
rowid ：新行的id号；
data ：新行的数据对象，形式为{name1:value1,name2: value2…}，其中name为colModel中定义的列名称name；
position ：插入的位置（first：表格顶端；last：表格底端；before：srcrowid之前；after：srcrowid之后）；
srcrowid ：新行将插入到srcrowid指定行的前面或后面。
这个方法可以一次性插入多行，data参数必须是[{name1:value1,name2: value2…}, {name1:value1,name2: value2…}]这样的数组形式，而且rowid参数也应该设为data参数对象中代表id的field名称。不过，此时的rowid不用必须是
 colModel中的一部分。
2. 动态修改某行数据内容:
setRowData(rowid,data)
setCell(rowid, colname,data)//设置单元格内容, rowid为行号, colname可以为列名或列序号,data为空时 单元格内容不变.
3. 获取数据内容:getRowData(rowid) // 如果没有参数,则获取整个grid的数据
4. 动态选中某行: setSelection(rowid) //选中某行,如果该行已经选中,则取消选中.
5. 获取某一列的值:  getCol(colnum) //获取某列的值,参数可以为列的序号,或者列名.
6. 删除一行: delRowData(rowid) //在grid中删除一行记录, 但是并不是删除服务端的数据



