# 无法添加外键约束的原因（cannot add foreign key constraint） - Machine Learning with Peppa - CSDN博客





2017年12月01日 12:15:00[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1383
所属专栏：[深入浅出数据库与Mysql](https://blog.csdn.net/column/details/18665.html)









首先看下面这张表

```
Create table student
(sno int(10) unsigned not null,
sname varchar(20) not null,
sex enum('男','女') default null,
sage smallint unsigned,
sdept varchar(10) not null
Primary key (sno)
);
```

插入三条记录后：
`SELECT * FROM students_id.student;`
![这里写图片描述](https://img-blog.csdn.net/20171201113354490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这张已经建成的STUDENT表中，SNO为Primary Key, 类型为 
**int(10) unsigned not null**

然后，创建下面这张表，其中SNO为外键，参照关系是SC，被参照是STUDENT

```
Create table SC(
cno char(4) not null,
sno smallint,  //外键
grade smallint,
primary key(cno),
foreign key (sno) references students_id.student(Sno)
);
```

创建失败

![这里写图片描述](https://img-blog.csdn.net/20171201120917606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原因是在SC这张表中，作为参照关系，SNO的数据类型没有保持和被参照的表STUDENT**完全一致。**

应改为如下形式：

```
Create table SC(
cno char(4) not null,
sno int(10) unsigned not null, 
grade smallint,
primary key(cno),
foreign key (sno) references students_id.student(Sno)
);
```

此时创建成功。需要注意的是，在SQL中，参照关系可以为主键，但在MYSQL中，参照关系不能为主键。 

总结： 

1.外键不能为新建表的主键； 

2.外键所参照的键必须为被参照表的主键； 

3.参照表的值需要在所参考的表的那一个主键中已经存在的（即不能为空） 

更多相关代码可以访问 
[我的GITHUB](https://github.com/hatwanghacker/DataBase-Mysql)



