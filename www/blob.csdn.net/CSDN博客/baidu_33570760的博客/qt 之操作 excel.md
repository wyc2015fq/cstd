# qt 之操作 excel - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:57:50[carman_风](https://me.csdn.net/baidu_33570760)阅读数：349
个人分类：[qt](https://blog.csdn.net/baidu_33570760/article/category/6898155)
转载自：http://blog.sina.com.cn/s/blog_a6fb6cc90101gv2p.html
Visual Basic forApplications（VBA）是一种VisualBasic的一种宏语言，主要能用来扩展Windows的应用程式功能，特别是MicrosoftOffice软件。也可说是一种应用程式视觉化的Basic Script。1994年发行的Excel5.0版本中，即具备了VBA的宏功能。 
 在VBA的参考手册中就可以看到具体函数、属性的用法，Qt操作Excel主要通过 QAxObject +Excel VBA来实现！
   关于Qt对Excel的操作，网上的资料挺多的，但大多数都是比较基础的，关于插入工作表（至最后一行）、删除工作表、合并/拆分单元格、设置单元格背景色、设置单元格边框色、设置单元格字体（类型、大小、加粗、斜体、下划线、颜色等）、以及设置单元格对齐方式等用法都没有怎么提到，今天就总结一下有关Qt对Excel的操作。
   Qt操作Excel，无论后缀是xls还是xlsx都可以。
    如下，是我下载的一个ExcelVBA参考手册，内容不算太全！
![](https://img-blog.csdn.net/20170515151108716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Excel读取**
   为了便于测试，假设已存在一个excel文件，操作内容已经被红色标记出来。如下所示：
**主要读取内容：**
- 标题
- 工作表数目
- 工作表名称
- 起始行
- 起始列
- 行数
- 列数
- 单元格内容
代码如下：
QAxObject excel("Excel.Application");  
excel.setProperty("Visible", true);  
QAxObject *work_books = excel.querySubObject("WorkBooks"); 
work_books->dynamicCall("Open (const QString&)",QString("E:/test.xlsx"));
QVariant title_value = excel.property("Caption"); //获取标题
qDebug()<<QString("excel title : ")<<title_value; 
QAxObject *work_book = excel.querySubObject("ActiveWorkBook"); 
QAxObject *work_sheets =work_book->querySubObject("Sheets"); //Sheets也可换用WorkSheets
int sheet_count = work_sheets->property("Count").toInt(); //获取工作表数目
qDebug()<<QString("sheet count : ")<<sheet_count; 
for(int i=1; i<=sheet_count; i++)  
{  
QAxObject *work_sheet =work_book->querySubObject("Sheets(int)", i); //Sheets(int)也可换用Worksheets(int)
QString work_sheet_name =work_sheet->property("Name").toString(); //获取工作表名称
QString message = QString("sheet ")+QString::number(i, 10)+QString(" name");
qDebug()<<message<<work_sheet_name; 
}
if(sheet_count > 0)
{
QAxObject *work_sheet =work_book->querySubObject("Sheets(int)", 1); 
QAxObject *used_range =work_sheet->querySubObject("UsedRange"); 
QAxObject *rows = used_range->querySubObject("Rows"); 
QAxObject *columns = used_range->querySubObject("Columns"); 
int row_start = used_range->property("Row").toInt(); //获取起始行  
int column_start = used_range->property("Column").toInt(); //获取起始列
int row_count = rows->property("Count").toInt(); //获取行数
int column_count = columns->property("Count").toInt(); //获取列数
for(int i=row_start; i
{  
for(int j=column_start; j
{  
QAxObject *cell =work_sheet->querySubObject("Cells(int,int)", i, j); 
QVariant cell_value = cell->property("Value"); //获取单元格内容
QString message = QString("row-")+QString::number(i,10)+QString("-column-")+QString::number(j, 10)+QString(":");
qDebug()<<message<<cell_value; 
}  
}  
}
效果如下：
![](https://img-blog.csdn.net/20170515151123763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Excel增、删、改**
**主要操作：**
- 设置标题
- 插入工作表（至最后一行）
- 设置工作表名称
- 删除工作表
- 设置单元格内容
- 设置单元格字体（类型、大小、加粗、斜体、下划线、颜色等）
- 设置单元格对齐方式
- 设置单元格高度、宽度
- 设置单元格背景色、边框色
- 合并/拆分单元格
- 清空单元格
代码如下：
QAxObject excel("Excel.Application");  
excel.setProperty("Visible", true);  
QAxObject *work_books = excel.querySubObject("WorkBooks"); 
work_books->dynamicCall("Open(const QString&)","E:\\test.xlsx");
excel.setProperty("Caption", "Qt Excel");
QAxObject *work_book = excel.querySubObject("ActiveWorkBook"); 
QAxObject *work_sheets =work_book->querySubObject("Sheets"); //Sheets也可换用WorkSheets
//删除工作表（删除第一个）
QAxObject *first_sheet =work_sheets->querySubObject("Item(int)", 1);
first_sheet->dynamicCall("delete");
//插入工作表（插入至最后一行）
int sheet_count = work_sheets->property("Count").toInt(); //获取工作表数目
QAxObject *last_sheet =work_sheets->querySubObject("Item(int)", sheet_count);
QAxObject *work_sheet =work_sheets->querySubObject("Add(QVariant)",last_sheet->asVariant());
last_sheet->dynamicCall("Move(QVariant)",work_sheet->asVariant());
work_sheet->setProperty("Name", "Qt Sheet"); //设置工作表名称
//操作单元格（第2行第2列）
QAxObject *cell =work_sheet->querySubObject("Cells(int,int)", 2, 2);
cell->setProperty("Value", "Java C++ C# PHP Perl PythonDelphi Ruby");  //设置单元格值
cell->setProperty("RowHeight", 50); //设置单元格行高
cell->setProperty("ColumnWidth", 30); //设置单元格列宽
cell->setProperty("HorizontalAlignment", -4108);//左对齐（xlLeft）：-4131  居中（xlCenter）：-4108 右对齐（xlRight）：-4152
cell->setProperty("VerticalAlignment", -4108); //上对齐（xlTop）-4160 居中（xlCenter）：-4108 下对齐（xlBottom）：-4107
cell->setProperty("WrapText", true); //内容过多，自动换行
//cell->dynamicCall("ClearContents()"); //清空单元格内容
QAxObject* interior =cell->querySubObject("Interior");
interior->setProperty("Color", QColor(0, 255, 0));  //设置单元格背景色（绿色）
QAxObject* border =cell->querySubObject("Borders"); 
border->setProperty("Color", QColor(0, 0, 255));  //设置单元格边框色（蓝色）
QAxObject *font = cell->querySubObject("Font"); //获取单元格字体
font->setProperty("Name", QStringLiteral("华文彩云")); //设置单元格字体
font->setProperty("Bold", true); //设置单元格字体加粗
font->setProperty("Size", 20); //设置单元格字体大小
font->setProperty("Italic", true); //设置单元格字体斜体
font->setProperty("Underline", 2); //设置单元格下划线
font->setProperty("Color", QColor(255, 0, 0)); //设置单元格字体颜色（红色）
//设置单元格内容，并合并单元格（第5行第3列-第8行第5列）
QAxObject *cell_5_6 =work_sheet->querySubObject("Cells(int,int)", 5, 3);
cell_5_6->setProperty("Value", "Java"); //设置单元格值
QAxObject *cell_8_5 =work_sheet->querySubObject("Cells(int,int)", 8, 5);
cell_8_5->setProperty("Value", "C++");
QString merge_cell;
merge_cell.append(QChar(3 - 1 + 'A')); //初始列
merge_cell.append(QString::number(5)); //初始行
merge_cell.append(":");
merge_cell.append(QChar(5 - 1 + 'A')); //终止列
merge_cell.append(QString::number(8)); //终止行
QAxObject *merge_range =work_sheet->querySubObject("Range(const QString&)",merge_cell);
merge_range->setProperty("HorizontalAlignment",-4108);
merge_range->setProperty("VerticalAlignment", -4108);
merge_range->setProperty("WrapText", true);
merge_range->setProperty("MergeCells", true); //合并单元格
//merge_range->setProperty("MergeCells", false); //拆分单元格
//work_book->dynamicCall("Save()"); //保存文件（为了对比test与下面的test2文件，这里不做保存操作）work_book->dynamicCall("SaveAs(const QString&)","E:\\test2.xlsx"); //另存为另一个文件 
work_book->dynamicCall("Close(Boolean)", false); //关闭文件
excel.dynamicCall("Quit(void)");  //退出
效果如下：
操作前：
![](https://img-blog.csdn.net/20170515151146011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
操作后：
![](https://img-blog.csdn.net/20170515151201089?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   到这里很多人也许都在纳闷，单元格的宏怎么获取的？比如对齐方式（居中对齐），为什么值是-4108，而不是其他值呢？当然那不是我随便写的，自己可以录制宏，然后跟踪。
看下图：![](https://img-blog.csdn.net/20170515151221293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如上所讲，已经基本可以满足常用的操作，如有更多专业需求，请参考Excel VBA...
- [Excel Object ModelReference](http://msdn.microsoft.com/en-us/library/bb149081%28v=office.12%29.aspx).

