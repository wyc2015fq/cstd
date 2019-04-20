# 学习ADO.NET技术（九）—DataAdapter对象 - u014677855的博客 - CSDN博客
2018年08月16日 14:49:18[xiaobigben](https://me.csdn.net/u014677855)阅读数：73
前面几篇学习了Command对象和Connection对象，我们说过，ADO.NET的核心组件有独立于数据源的DataSet组件和访问外部数据源的数据提供程序。基于连接的数据提供程序组件常用于实时地从数据库中检索数据。而基于非连接的DataSet组件仅仅用于在本地内存中存储数据提供程序提供的数据表或集合。那么本文就学习这两个核心组件之间是如何联系起来的？
## 目录
- - [目录](#目录)
- [1、DataAdapter是什么？](#1dataadapter是什么)
- [2、DataAdapter工作原理](#2dataadapter工作原理)
- [3、DapaAdapter对象的重要成员](#3dapaadapter对象的重要成员)
- [4、DataAdapter的使用](#4dataadapter的使用)
- [说明](#说明)
## 1、DataAdapter是什么？
Connection对象，Command对象和DataReader对象，均属于Data Provider的一部分，而且都是基于连接的。这些对象可以让我们连接一个数据源，执行SQL语句，检索只读的数据流等。这些基于连接的对象都对应于特定的数据源。换句话说，对于不同的数据源，我们需要找到对应的数据提供程序来匹配。但是，如果我们每次检索数据库中的表或者行都需要连接一次数据库，那么性能和效率是十分低下的。实际上，ADO.NET提供了基于非连接的组件DataSet。DataSet可以让我们在本地内存中操作数据表等，就好比操作数据库中的表一样。 
那么问题来了，DataSet存储的数据集是哪里来的呢？它既然不需要连接数据库，那么它所操作的数据又来自哪里？其实它虽没有直接连接数据库，确有一位中介来提供给它数据集。这个中介就是：DataAdapter。   DataAdapter数据适配器：为外部数据源和Dataset集合搭建了一座桥梁，将从外部数据源检索到的数据集合理正确地调配到本地内存中的DataSet集合中。
## 2、DataAdapter工作原理
尽管DataAdapter内部实现机制较为复杂，但是提供给开发人员的接口却是异常的简单。DataAdapter本质上就是一个数据调配器。当我们需要查询数据时，它从数据库检索数据，并填充到本地的DataSet或者DataTable中；当我们需要更新数据库时，它将本地内存的数据路由到数据库，并执行更新命令。下图展示了DataAdapter的工作过程。 
![这里写图片描述](https://img-blog.csdn.net/20180816141759642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
总体上来说，DataAdapter主要有三个功能：
- **数据检索**：尽可能用最简单的方法将检索到的数据源填充到本地的Dataset中。细致的说，DataAdapter用一个DataReader的实例来检索数据，因此必须提供查询语句和连接字符串。
- **数据更新**：将本地修改的数据源返回给外部的数据源。更新数据库则需要区分Update、Delete和Insert语句。
- **表或列名映射**：维护本地DataSet表名和列名与外部数据源表名与列名的映射关系。
## 3、DapaAdapter对象的重要成员
作为数据提供程序的对象之一，DataAdapter对象跟其他对象具有相似的特征：都是基于连接的，都继承于基类，不同的数据源对应不同的对象类型。
```
public abstract class DbDataAdapter : DataAdapter, 
    IDbDataAdapter, IDataAdapter, ICloneable
```
抽象基类，不能被实例化，并且继承自DapaAdapter和接口。 
常用的DataAdapter对象成员有：
- SelectedCommand属性：获取或设置在数据源选择记录的命令。
- UpdateCommand属性：获取或设置用于更新数据源中的记录的命令。
- DeleteCommand属性：获取或设置用于从数据源中删除记录的命令。
-  InsertCommand属性：获取或设置用于将新记录插入数据源中的命令。
- Fill()方法：填充数据集。
- Update()方法：更新数据源。
## 4、DataAdapter的使用
```
OleDbDataAdapter oda = new OleDbDataAdapter("SELECT * FROM UserInfo", objConnection);//创建适配对象
            DataSet ds = new DataSet();//数据集
            oda.Fill(ds);//用适配对象填充表对象
            dataGridView1.DataSource = ds.Tables[0];//将表对象作为DataGridView的数据源
```
## 说明
[原作者文章详情请戳](https://www.cnblogs.com/liuhaorain/archive/2012/03/25/2399510.html)
