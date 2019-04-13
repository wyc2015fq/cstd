
# JQGrid 列头和单元格内容换行 - 追求卓越,做到专业 - CSDN博客


2012年12月21日 11:34:50[Waldenz](https://me.csdn.net/enter89)阅读数：8566个人分类：[jqGrid																](https://blog.csdn.net/enter89/article/category/1309802)


对JQGrid 列标题和单元格内容换行

```python
.ui-jqgrid tr.jqgrow td 
{
	/* jqGrid cell content wrap  */
	white-space: normal !important;
        height :auto;
}
th.ui-th-column div
{
	/* jqGrid columns name wrap  */
	white-space:normal !important;
	height:auto !important;
	padding:0px;
}
```



