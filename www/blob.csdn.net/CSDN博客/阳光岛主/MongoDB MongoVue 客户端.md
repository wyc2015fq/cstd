
# MongoDB MongoVue 客户端 - 阳光岛主 - CSDN博客

2013年12月25日 22:54:29[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：21290


**[MongoVue](http://www.mongovue.com/)**，是一款MongoDB的客户端工具，1.0版本的开始收费了，本文最后提供免费破解版v1.5.3。
其中，1.0版之后超过15天后功能受限，可以通过删除以下注册表项来解除限制：
[HKEY_CURRENT_USER\Software\Classes\CLSID\{B1159E65-821C3-21C5-CE21-34A484D54444}\4FF78130]
注册表中查找B1159E65-821C3-21C5-CE21-34A484D54444中的子项4FF78130 ，删除其下的三个子项即可。
![](https://img-blog.csdn.net/20131226093904906)
把这个项下的值全删掉就可以了，这样剩余时间又会回到15天前

**连接mongodb**
1）点击连接，填入如下参数
![](https://img-blog.csdn.net/20131029160132140)

2）测试连接成功，但展开“Collections”时，出现如下错误
![](https://img-blog.csdn.net/20131029160219265)

3）错误原因，是mongodb对应的数据库（test）没有权限，此权限是mongodb对应的test数据库用户名和密码权限，不是mongodb所在服务器的用户名和密码。因此，需要先给mongodb的test数据库添加用户名和密码，如下：
![](https://img-blog.csdn.net/20131029160226890)

4）利用步骤3添加的用户名和密码，重新登录，成功！
![](https://img-blog.csdn.net/20131029160237328)

下面介绍MongoVue用法：
1、 首先在mongodb中插入两条数据
![](https://img-blog.csdn.net/20131226095114250)
2、配置连接
（1） 先设置上面测试数据库（testdb）的用户名和密码
![](https://img-blog.csdn.net/20131226094647718)
（2）MongoVue 连接测试数据库（testdb）
![](https://img-blog.csdn.net/20131226100218750)
3、右键 testdb数据库，点击“Add Collection..."，添加collection，其名”testcoll2“，如下图：
![](https://img-blog.csdn.net/20131226101025281)
创建testcoll2结果如下：
![](https://img-blog.csdn.net/20131226101239937)
4、右键2中创建testcoll2，点击”Add Document..."，插入一条记录（似乎不可一次插入多条），如下图：
![](https://img-blog.csdn.net/20131226101959984)
5、查看插入的数据，数据可以通过多种方式展示（树形、表格、文本）
![](https://img-blog.csdn.net/20131226102347343)
上面我们都是通过图形界面的操作的吧，下面有一个窗口列出了上述操作的客户端命令，这是学习的好资源，在用图形界面的时候依然可以学习熟悉下命令行。
![](https://img-blog.csdn.net/20131226102530031)
当然上述只是介绍了下最基本的功能，还有更新，删除数据库，从mysql数据库导入数据等等功能，想了解更详细的内容请访问[官方网站 ](http://www.mongovue.com/)
6、界面条件查询，如下图：
![](https://img-blog.csdn.net/20131226103107671)

下载破解版[MongoVUE.v1.5.3](http://download.csdn.net/detail/sunboy_2050/6770483)

**Linux 下的mongo GUI工具：**
[Robo Mongo](http://www.robomongo.org/)
[umongo](http://edgytech.com/umongo/)
[MonjaDB](http://www.jumperz.net/index.php?i=2&a=0&b=9)

**参考推荐：**
[MongoDB 客户端 MongoVue](http://www.cnblogs.com/shanyou/archive/2011/05/20/2052354.html)
[MongoVUE简单操作手册](http://wenku.baidu.com/view/ee9bac4de45c3b3567ec8bfe.html)
[MongoDB 安装配置](http://blog.csdn.net/ithomer/article/details/8555545)
[nosqlfan](http://blog.nosqlfan.com/tags/mongodb)（推荐）
[What is a good MongoDB GUI client](http://askubuntu.com/questions/196136/what-is-a-good-mongodb-gui-client)



