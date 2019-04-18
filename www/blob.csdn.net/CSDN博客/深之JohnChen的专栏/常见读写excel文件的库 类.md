# 常见读写excel文件的库/类 - 深之JohnChen的专栏 - CSDN博客

2018年10月31日 11:03:18[byxdaz](https://me.csdn.net/byxdaz)阅读数：432


读写EXCEL表格，有这几种方法：COM方式、ODBC方式、OLE方式、纯底层格式分析方式。

|库/类|方式|功能|备注|
|----|----|----|----|
|basicexcel|com方式|可读可写excel|开源，windows平台。|
|CSpreadSheet|ODBC方式|可读可写excel|开源，windows平台。|
|libxls|纯底层格式分析方式|读excel|开源，跨平台。|
|xlslib|纯底层格式分析方式|写excel|开源，跨平台。|
|libxl|纯底层格式分析方式|可读可写excel|功能强大，但是收费的。跨平台。|

Basicexcel

使用方法：[https://www.cnblogs.com/paullam/p/3705924.html](https://www.cnblogs.com/paullam/p/3705924.html)

源码：[http://www.codeproject.com/KB/office/BasicExcel.aspx](http://www.codeproject.com/KB/office/BasicExcel.aspx)

缺点：

1）、不支持格式化；

2）、不支持公式；

3）、不支持图表；

4）、不支持Unicode UTF-32；

5）、中文支持不好；

CSpreadSheet

中文文档：[https://www.oschina.net/question/76782_12420](https://www.oschina.net/question/76782_12420)

源码：[https://www.codeproject.com/Articles/1636/CSpreadSheet-A-Class-to-Read-and-Write-to-Excel-an](https://www.codeproject.com/Articles/1636/CSpreadSheet-A-Class-to-Read-and-Write-to-Excel-an)

缺点：

1）、CSpreadSheet的构造问题：

CSpreadSheet(CString File, CString SheetOrSeparator, bool Backup = true);

第三个参数默认为true，这个参数为true的原始意义是读取表格时额外做个备份，但是如果你的xls表格不是每列每行都全有数据的时候，它便会严重破坏你的表格数据，这可能是作者的无心之失，实验的数据可能全是满值的结果吧。这个问题想改也很容易，就是删除表格重新写入的过程。所以我构造这个对象的时候最后一个参数为false，我不需要有备份，这样便会省去删除表格又重新写的麻烦了。

2）、CSpreadSheet由于采用的是数据库方式，所以有一个字段数量限制，这是微软的ADO访问数据库的一个限制，最多255个字段，多了就不行了，详情请参照微软核心代码dbcore。

3）、AddRow()函数问题。这个问题也有很多人没有注意到，它的效率极其低下！当然如果添加行数比较少的话，没什么影响，但是当你添加上百行数据的时候，这个函数的问题就凸现出来了。原因就在于AddRow（）函数调用了Commit()。

4）、数字类型，字段被转换成字符串。

5）、支持xls，不支持新的xlsx格式。

Xlslib

使用方式：[https://blog.csdn.net/byxdaz/article/details/83505475](https://blog.csdn.net/byxdaz/article/details/83505475)

源码：[http://sourceforge.net/projects/xlslib](http://sourceforge.net/projects/xlslib)

Libxls

使用方法：[https://blog.csdn.net/zhangqiu1989/article/details/8822853](https://blog.csdn.net/zhangqiu1989/article/details/8822853)

libxls windows版本编译方式

[https://blog.csdn.net/qq_21950929/article/details/78655342](https://blog.csdn.net/qq_21950929/article/details/78655342)

源码：[http://sourceforge.net/projects/libxls/](http://sourceforge.net/projects/libxls/)

**Libxl**

使用方法：[https://www.cnblogs.com/pjl1119/p/8059126.html](https://www.cnblogs.com/pjl1119/p/8059126.html)

官网：[http://www.libxl.com](http://www.libxl.com)


