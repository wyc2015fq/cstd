# PostgreSQL 图形化客户端工具的使用技巧你都get了吗？ - DoubleLi - 博客园






PostgreSQL 数据库作为目前功能较强大的开源数据库，得到了广泛应用。其中，TSA就用到了这款数据库来存储处理后的一些业务数据。虽然PostgreSQL自身提供了命令行交互式客户端工具psql，但是字符界面工具需要频繁写sql语句进行查询，数据不直观可见。绿盟君利用晚上的业余时间整理了PostgreSQL 图形化客户端工具的使用技巧，分享给大家。





文章目录

- [1.     安装连接](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#1)
- [2.     非常小技巧](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#2)- [Where](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#Where)
- [Order by](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#Order_by)
- [Group by](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#Group_by)
- [SQL Editor](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#SQL_Editor)
- [注意事项](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#i)

- [3.     结束语](http://blog.nsfocus.net/postgresql-graphical-client-tool-tips/#3)


## 1.     安装连接

下载破解版的安装包postgresql_maestro_setup.exe，轻量安装非常方便，分分钟搞定。

安装完后，便可以使用maestro连接到数据库了，连接界面如下所示：
![postgresql1](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL1-300x241.jpg)

输入要连接数据库所在主机的IP、端口号、用户名及密码，next->next就可以啦。

![postgresql2](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL2-300x214.jpg)

双击左侧窗口对应的数据库名称，便打开数据库，双击对应Schema下的表名，便可查看该表的相关信息，如下图所示：
![postgresql3](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL3-268x300.jpg)

Properties展示数据库的属性，可一览数据库中的所有字段；

Data展示数据，选择Data可以查看到数据库中的数据,分行展示；

点击SQL可查看建表语句。

## 2.     非常小技巧

### Where

当表中页签切换到Data时，点击展示数据中有的所有event_type值
![postgresql4](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL4-300x171.jpg)
可以选择某一种事件类型过滤，这样就可以不必写select语句，就可以快速过滤出你想查询的数据。

### Order by

右击event_type，弹出菜菜单框。
![postgresql5](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL5-300x154.jpg)
可以将过滤出的数据再进行进一步处理

Sort Ascending：将数据升序排列；

Sort Desending: 将数据降序处理；

Advanced sort：高级排序选项框，可以根据需要配置

Clear Sorting: 清除排序

### Group by

右键点击需要group by的字段，选择Group By This Filed，结果如下所示：

![postgresql6](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL6-300x240.jpg)

这样数据就根据所选字段聚类了，点击分组后前的“+”，就可以查看该分组下的数据详情了。

**如何取消group by****呢？**

右键点击group by的字段，选择Remove from grouping，便可以取消了，当然你也可以选择直接将该字段拖动到原处的方法取消。
![postgresql7](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL7-300x245.jpg)

**Group By Box**

选择字段，右键点击选择Group By BOX， 界面展示如下
![postgresql8](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL8-300x77.jpg)

由上图可以只，Group By BOX本质其实是一种归类排序。

### SQL Editor

对于习惯写SQL语句的同学来说，Maestro同样提供了强大的编辑功能。

点击菜单栏Tools，选择SQL Editor，如下所示：

![postgresql9](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL9-300x195.jpg)

打开SQL Editor编辑框，输入SQL语句。
![postgresql10](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL10-300x58.jpg)

点击执行，界面就会展示查询结果，如下所示：
![postgresql11](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL11-300x284.jpg)

同理，Maestro还有SQL Script Editor的功能，如下所示：
![postgresql12](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL12-300x79.jpg)

### 注意事项

在使用过程中，你可能会发现1-3查询到的数据，和4或者和系统业务展示的数据不一致。是的，当表中数据不足1000条时，1-3查询到的数据和4通过sql查询到的数据完全一致。但是，当表中数据超过1000条时，1-3查询到的数据并不是全表数据。这是因为Maestro默认分页为1000条。
![postgresql13](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL13-300x51.jpg)

可以在上述红框中修改，不过刷新或重新登录时，又返回1000条。

最大值可设为5000，若设置大于5000时，会出现如下情况，无下拉黑色三角，功能1-3无法使用。
![postgresql14](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL14-300x91.jpg)

不要着急，这时只需要点击提示中的蓝色字体“Options”，也可以选择菜单栏中的tools->options，打开如下对话框
![postgresql15](http://blog.nsfocus.net/wp-content/uploads/2016/10/PostgreSQL15-300x279.jpg)

可以根据表中数据量修改默认值，调整为较大数值，一劳永逸，这样就可以畅通无阻，无忧无虑地使用了。

## 3.     结束语

熟悉了Masstro的使用技巧后，只需轻轻一点就可省去很多SQL语句的编写，简单快捷。如果上述使用技巧的总结整理，对新同学在使用过程中起到一些指导作用，同时也能微弱地提高一点其他同学的工作效率，那所用的整理时间就变得有意义啦。

当然还有其他一些图形化的客户端工具，如pgAdminIII、navicat for postgresql。只需花一点时间探索熟悉一下，就可以在日后的工作中熟练快捷地使用了。









