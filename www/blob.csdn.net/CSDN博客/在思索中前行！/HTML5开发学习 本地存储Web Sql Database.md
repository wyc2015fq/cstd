# HTML5开发学习:本地存储Web Sql Database - 在思索中前行！ - CSDN博客





2014年07月05日 19:56:01[_Tham](https://me.csdn.net/txl16211)阅读数：2113标签：[本地web sql](https://so.csdn.net/so/search/s.do?q=本地web sql&t=blog)
个人分类：[Web前端](https://blog.csdn.net/txl16211/article/category/2377621)










   Web Sql Database，中文翻译作“本地数据库”，是随着HTML5规范加入的在浏览器端运行的轻量级数据库。


   在HTML5中，大大丰富了客户端本地可以存储的内容，添加了很多功能来将原本必须保存在服务器上的数据转为保存在客户端本地，从而大大提高了Web应用程序的性能，减轻了服务器端的负担，使Web时代重新回到了“客户端为重，服务器为轻”的时代。

在这其中，一项非常重要的功能就是数据库的本地存储功能。在HTML5中内置了一个可以通过SQL语言来访问的数据库。在HTML4中，数据库只能放在服务器端，只能通过服务器来访问数据库，但是在HTML5中，可以就像访问本地文件那样轻松的对内置的数据库进行直接访问。现在，像这种不需要存储在服务器上的，被称为“SQLite”的文件型SQL数据库已经得到了很广泛的利用，所以HTML5中也采用了这种数据库来作为本地数据库。









Web SQL Database （目前只谷歌浏览器支持）：我把它理解成一个Html5环境下可以用Js执行CRUD的Web数据库。

对于简单的数据，使用sessionStorage和localStorage能够很好地完成存取，但是对于处理复杂的关系型数据，它就力不从心了。这也是 HTML 5 的“Web SQL Database”API 接口的应用所在。 

下面我将一一将介绍怎样 创建打开数据库，创建表，添加数据，更新数据，删除数据，删除表 。



先介绍三个核心方法

1、openDatabase：这个方法使用现有数据库或创建新数据库创建数据库对象。

2、transaction：这个方法允许我们根据情况控制事务提交或回滚。

3、executeSql：这个方法用于执行真实的SQL查询。



**第一步：打开连接并创建数据库**


```
var dataBase = openDatabase("student", "1.0", "学生表", 1024 * 1024, function () { });
if (!dataBase) {

    alert("数据库创建失败！");

} else {

    alert("数据库创建成功！");

}
```



 解释一下openDatabase方法打开一个已经存在的数据库，如果数据库不存在，它还可以创建数据库。几个参数意义分别是：

1，数据库名称。

2，版本号 目前为1.0,不管他，写死就OK。

3，对数据库的描述。

4，设置数据的大小。

5，回调函数(可省略)。

初次调用时创建数据库，以后就是建立连接了。

创建的数据库就存在本地，路径如下：

C:\Users\Administrator\AppData\Local\Google\Chrome\User Data\Default\databases\http_localhost_4987 。

创建的是一个sqllite数据库，完全可以用sqllite developer（[点击下载](http://west263.onlinedown.net:82/down/SqliteDev386.zip)） 打开文件，可以看到里面的数据。







**第二步：创建数据表**

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)


this.createTable=function() {


  dataBase.transaction( function(tx) { 

    tx.executeSql(

        "create table if not exists stu (id REAL UNIQUE, name TEXT)", 

        [], 

        function(tx,result){ alert('创建stu表成功'); }, 

        function(tx, error){ alert('创建stu表失败:' + error.message); 

    });

 });

}


![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



解释一下，

executeSql函数有四个参数，其意义分别是：

1）表示查询的字符串，使用的SQL语言是SQLite 3.6.19。

2）插入到查询中问号所在处的字符串数据。

3）成功时执行的回调函数。返回两个参数：tx和执行的结果。

4）一个失败时执行的回调函数。返回两个参数：tx和失败的错误信息。



**第三步：执行增删改查**

1）添加数据：


```
![复制代码](http://common.cnblogs.com/images/copycode.gif)


this.insert = function () {

  dataBase.transaction(function (tx) {

            tx.executeSql(

        "insert  into  stu (id, name) values(?, ?)",

        [id, '徐明祥'],

        function () { alert('添加数据成功'); },

        function (tx, error) { alert('添加数据失败: ' + error.message); 

        } );

    });


![复制代码](http://common.cnblogs.com/images/copycode.gif)
```







2）查询数据

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)


this.query = function () {

  dataBase.transaction(function (tx) {

            tx.executeSql(

        "select  *  from  stu", [],

         function (tx, result) {  //执行成功的回调函数

             //在这里对result 做你想要做的事情吧...........

            },

         function (tx, error) {

            alert('查询失败: ' + error.message);

         } );

      });

}


![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


解释一下

上面代码中执行成功的回调函数有一参数result。 

result：查询出来的数据集。其数据类型为 SQLResultSet ，就如同C#中的DataTable。 

SQLResultSet 的定义为：

 interface  SQLResultSet {

             readonly  attribute  long  insertId;

             readonly  attribute  long  rowsAffected;

             readonly  attribute  SQLResultSetRowList  rows;

             };

其中最重要的属性—SQLResultSetRowList 类型的 rows 是数据集的“行” 。 

rows 有两个属性：length、item 。

故，获取查询结果的某一行某一列的值 ：result.rows[i].item[fieldname]  。 


3）更新数据

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)


this.update = function (id, name) {

    dataBase.transaction(function (tx) {

            tx.executeSql(

        "update  stu  set  name =  ?  where  id= ?",

        [name, id],

         function (tx, result) {

         },

        function (tx, error) {

            alert('更新失败: ' + error.message);

        });

   });

}


![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

   4）删除数据

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)


   this.del = function (id) {

            dataBase.transaction(function (tx) {

            tx.executeSql(

        "delete  from  stu  where  id= ?",

        [id],

         function (tx, result) {

         },

        function (tx, error) {

            alert('删除失败: ' + error.message);

        });

     });

   }


![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

 5）删除数据表

```
this.dropTable = function () {

              dataBase.transaction(function (tx) {

            tx.executeSql('drop  table  stu');

        });

    }
```

我这里附上一个 web sql database 增删改查的demo，[猛点下载](http://files.cnblogs.com/xumingxiang/WebStorage.rar)。


了解更多：[http://www.w3.org/TR/webdatabase/](http://www.w3.org/TR/webdatabase/)


注：

1) 查看谷歌浏览器的 “Chrome\User Data\Default ”发现百度、腾讯等好多网站已经在运用localStorage 和 Web Sql Databases 。

2) localStorage 和 Web Sql DataBase 其实都是在本地创建了一个sqllite 数据库。

    localStorage 是一个简单的数据库，只建了一张表，表名叫 “ItemTable”，并且这个表里只有两个字段 “key”和“Value”。

   而Web Sql DataBase 就是一个完全交给用户去操作的数据库了，复杂或简单完全由用户自己根据需求而决定。



