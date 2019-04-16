# excel数据导入MySQL数据库 - littlely_ll的博客 - CSDN博客





2017年05月20日 22:45:39[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：296








今天要把excel中数据导入到MySQL数据库，以前只知道创建表，但是怎么把excel导入到MySQL还不知道，今天查了一些网上的资料，其实很简单，下面记录一下步骤，以备后用。

# 更改数据格式

把excel的数据格式更改为txt文件，一般是逗号分隔符分割。并且要注意到把数据的表头要去掉。

# 创建表

根据需要的字段创建一个表，比如我创建的是：

```
CREATE TABLE linearregression
(
year char(4) NULL,
income FLOAT NULL,
gdp FLOAT NULL,
food double NULL,
cpi decimal(4,1) NULL,
industry FLOAT NULL,
population FLOAT NULL
)engine=InnoDB;
```

# 导入txt数据

导入数据只需要一句话：
`load data local infile 'C:/your data path/linearregression.txt' into table linearregression fields terminated by ',';`
因为我的数据是用逗号分割的，所以是terminated by ‘,’，根据自己的情况而定。

# 查看数据

![这里写图片描述](https://img-blog.csdn.net/20170520222129309?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# MySQL导出到excel

```
select * from linearregression
into outfile 'C:/ProgramData/MySQL/MySQL Server 5.7/Uploads/linearreg.xls' 
;
```

在这里一个问题是只能导入到路径”C:/ProgramData/MySQL/MySQL Server 5.7/Uploads”里，这是因为secure_file_priv设定的值为就是”C:/ProgramData/MySQL/MySQL Server 5.7/Uploads”，限制了导出的路径。



