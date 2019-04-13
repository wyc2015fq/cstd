
# jqGrid使可编辑列获取焦点 - 追求卓越,做到专业 - CSDN博客


2012年10月26日 12:54:01[Waldenz](https://me.csdn.net/enter89)阅读数：3441


**editCell**方法,使可编辑列获取焦点,参数描述等如下.
|editCell|iRow, iCol, edit|edit a cell with the row index iRow( do not mix with rowid) in index column iCol. If the edit is set to false the cell is just selected and not edited. If set to true the cell is selected and edited.|
$(this).editCell(1, 2, true); //将第一行,第三列选中并显示可编辑状态


