# 学习ADO.NET技术（一） - u014677855的博客 - CSDN博客
2018年08月10日 14:12:01[xiaobigben](https://me.csdn.net/u014677855)阅读数：536
[详细内容请查看原作者博客](http://www.cnblogs.com/liuhaorain/archive/2012/02/12/2347914.html)
一、什么是ADO.NET 
         ADO.NET是.NET Framework中的一个模块，一个重要组件，用于建立应用程序和数据源之间的连接。 
                简单的讲，ADO.NET是一组允许.Net开发人员使用的标准的，结构化的，与数据交互的技术。数据源可以是应用程序创建在内存中的数据，也可以是与应用程序分离的，存储在存储区域的数据，比如SQL Server和XML文件等。 
                具体来说，ADO.NET对数据源提供一致的访问，使用ADO.NET来连接这些数据源，并检索、处理和更新所包含的数据。 
                ADO.NET类都位于`System.Data.dll`中，并与`System.Xml.dll`中的XML类集成。当编译使用了System.Data命名空间的代码时，需要引用`System.Data.dll`和`System.Xml.dll`。
![ADO.NET](https://images2017.cnblogs.com/blog/63651/201710/63651-20171023090555785-1366516311.png)
二、ADO.NET的组成
## 1、组成部分
```
System.Data命名空间提供了不同的ADO.Net类，该类库包含两组重要的类：一组是负责处理软件内部的实际数据（DataSet），一组负责与外部数据通信(DataProvider）。如下图所示：
 ![ADO.NET类组成](https://images2017.cnblogs.com/blog/63651/201710/63651-20171023091053051-396149880.png)
```
- DataSet类：为了实现独立于任何数据源的数据访问。DataSet包含一个或多个DataTable对象的集合，这些数据由数据行和数据列以及主键、外键、约束和有关DataTable对象数据中的关系信息组成。
- Data Provider（数据提供程序）元素，用于连接到数据库，执行命令并检索数据记录。它还允许将数据放入`DataSet`中以在应用程序中进一步使用。主要包括以下几个类：
- Connection：提供与数据源的连接
- Command：执行查询来执行数据库操作。
- DataReader：从数据源读取数据。
- DataAdapter：填充DataSet，对象提供连接DataSet和数据源的桥梁。DataAdapter对象在数据源中执行SQL命令，以便将数据加载到DataSet中，并使在DataSet中数据的更改与数据源中保持一致。
## 2、ADO.NET扩展
使用ADO.NET的一个关键的优势就是提供一致的数据访问。 
    .Net Framework提供了可以在应用程序中使用的下列数据提供者。 
![这里写图片描述](https://img-blog.csdn.net/20180810145845575?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180810150022508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
