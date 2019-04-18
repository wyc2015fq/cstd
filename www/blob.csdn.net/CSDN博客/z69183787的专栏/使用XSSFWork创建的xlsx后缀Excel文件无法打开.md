# 使用XSSFWork创建的xlsx后缀Excel文件无法打开 - z69183787的专栏 - CSDN博客
2018年09月17日 11:58:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：255
[https://blog.csdn.net/iaiti/article/details/45153627](https://blog.csdn.net/iaiti/article/details/45153627)
**总结一下：空excel需要建一个sheet**
这次需要自己写个自用的Excel导出工具：
照着POI的官方文档新建一个Excel文件。
- 
`Workbook wb = new XSSFWorkbook();`
- 
`FileOutputStream fileOut = new FileOutputStream("workbook.xlsx");`
- 
`wb.write(fileOut);`
- 
`fileOut.close();`
导出的xlsx后缀的Excel文件打开时，Office提示错误，发现“xxx.xlsx”中的部分内容有问题，是否让我们尽量尝试恢复，如果您信任此工作簿的源，请单击“是”。
但是点击了是之后发现提示，Microsoft Excel无法打开或修复此工作簿，因为它已损害。
搜索引擎查不到相关问题问题，文件写出导入了下载的POI里面的所有JAR包。
后面继续写了下去，往工作簿里插入了一张工作表：
`Sheet sheet1 = wb.createSheet("new sheet");`
然后xlsx文件正常打开了。
其实就是整个Excel文件格式的问题，虽然新建的工作簿，但是里面是一张工作表都没有的，而Office自己新建的时候会默认生成3张工作表，但是自己用编程创建的Excel，Office打开的时候便报错了。
官方文档这样一个New Workbook的介绍确实有点缺陷，应该对应说明。
