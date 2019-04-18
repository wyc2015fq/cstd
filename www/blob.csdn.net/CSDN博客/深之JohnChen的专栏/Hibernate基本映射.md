# Hibernate基本映射 - 深之JohnChen的专栏 - CSDN博客

2017年10月05日 23:59:27[byxdaz](https://me.csdn.net/byxdaz)阅读数：274
个人分类：[hibernate](https://blog.csdn.net/byxdaz/article/category/6892926)



ORM（Object Relational Mapping），即对象关系映射。它的作用就是在关系型数据库和对象之间做了一个映射。从对象（Object）映射到关系（Relation），再从关系映射到对象。

         没有Hibernate时，我们需要通过JDBC+手动写SQL语句来操作数据库，现在，有了Hibernate，它将JDBC+SQL进行了高度封装，我们不需要再去和复杂SQL打交道，只要像操作对象一样操作数据库就可以了。

       ORM的实现思想就是将数据库中表的数据映射成对象，Hibernate可以使我们采用对象化的思维操作关系型数据库。

映射文件

 Hibernate在实现ORM功能的时候主要用到的文件有：

    1、 映射类（*.Java）：它是描述数据库表的结构，表中的字段在类中被描述成属性，将来就可以实现把表中的记录映射成为该类的对象了。

    2、映射文件（*.hbm.xml）：它是指定数据库表和映射类之间的关系，包括映射类和数据库表的对应关系、表字段和类属性类型的对应关系以及表字段和类属性名称的对应关系等。

    3、 hibernate核心配置文件（*.cfg.xml）：它指定hibernate的一些核心配置，包含与数据库连接时需要的连接信息，比如连接哪种数据库、登录数据库的用户名、登录密码以及连接字符串等。映射文件的地址信息也放在这里。

映射分类

映射是指表之间关系映射，包括一对一、多对一、一对多、多对多。

编写映射文件的过程：

（1）写注释

         格式为：？属性，表达的是本对象与？的？关系。

         解释：在写映射文件之前先写注释，将问号的地方填上相应的内容。例如：<!--group属性，表达的是本对象（User）与Group的多对一的关系-->

（2）写映射的框架（拷模版）
|多对一|<many-to-one name=“” class=“” column=“”/>|
|----|----|
|一对多（Set）|<set name="">        <key column=""></key>        <one-to-many class=""/></set>|
|多对多（Set）|<set name="" table="">        <key column=""></key>        <many-to-many class="" column=""/></set>|

（3）填空

          填空，就是将映射的框架信息，填写完成，完成映射文件。

          •name属性：属性名（注释中的第1问号）

          • class属性：关联的实体类型（注释中的第2个问号）

          • column属性：

               ○ <many-to-onecolumn="..">：一般可以写成属性名加Id后缀，如属性为group，则column值写成groupId。 但group与SQL中的关键字重复，所以需要重新命名字段(column="groupid")。这样这个字段(groupid)会作为外键参照数据库中group表(t_group也叫一的一端)，也就是就在多的一端加入一个外键指向一的一端。

               ○ 一对多中的<keycolumn="..">：从关联的对方（对方是多对一）映射中把column值拷贝过来。 

               ○ 多对多中的<key column=“..”>：一般可以写成本对象的名加Id后缀，如本对象名为User，则写为userId。

               ○ 多对多中的<many-to-manycolumn=“..”>：一般可以写为关联对象的名称加Id后缀。

（4）完成

          将映射文件添加到hibernate.hbm.xml中。

一对一实例

两个实体对象之间是一对一的关联映射，即一个对象只能与另外唯一的一个对象相对应。例如：一个人(Person)只有一张身份证(IdCard)。人(Person)持有身份证(IdCard)的引用，所以，两个对象关系维护由person端决定。从对象模型映射成关系模型，有两种方式：主键关联和唯一外键关联。

主键关联:不需要多余的外键字段来维护关系，仅通过主键来关联，即Person的主键要依赖IdCard的主键，他们共用一个主键值。

一对多实例

一对多关联映射和多对一关联映射的映射原理是一致的，都是在多的一端加入一个外键，指向一的一端。关联关系都是由多端维护，只是在写映射时发生了变化。

Group持有User的一个引用。由于是单向关联，所以数据在加载Group时，会把User加载上来，但是User并不知道Group的存在。

一对多双向关联的映射方式：

 1）在一的一端的集合上采用<key>标签，在多的一端加入一个外键

 2）在多的一端采用<many-to-one>标签

注意：<key>标签和<many-to-one>标签加入的字段保持一直，否则会产生数据混乱。

inverse属性：

        inverse属性可以用在一对多和多对多双向关联上，inverse属性默认为false，为false表示本端维护关系，如果inverse为true，则本端不能维护关系，会交给另一端维护关系，本端失效。所以一对多关联映射我们通常在多的一端维护关系，让一的一端失效，所以设置为inverse为true。

注意：inverse属性，只影响数据的存储，也就是持久化。

多对多实例

多对多关联映射，在实体类中，跟一对多关联映射一样，也是用集合来表示的。<set>标签中用table属性重命名中间表名称，<key>标签定义当前表的主键，用<many-to-many>标签来关联另一张表。

具体实例参考下面链接的文章

Hibernate映射——多对一单向关联映射
[http://blog.csdn.net/jiuqiyuliang/article/details/40345389](http://blog.csdn.net/jiuqiyuliang/article/details/40345389)

Hibernate映射——一对一单向关联映射
[http://blog.csdn.net/jiuqiyuliang/article/details/40350515](http://blog.csdn.net/jiuqiyuliang/article/details/40345389)

Hibernate映射——一对一双向关联映射
[http://blog.csdn.net/jiuqiyuliang/article/details/40384463](http://blog.csdn.net/jiuqiyuliang/article/details/40384463)

Hibernate映射——一对多关联映射
[http://blog.csdn.net/jiuqiyuliang/article/details/41015641](http://blog.csdn.net/jiuqiyuliang/article/details/41015641)

Hibernate映射——多对多关联映射
[http://blog.csdn.net/jiuqiyuliang/article/details/41015645](http://blog.csdn.net/jiuqiyuliang/article/details/41015641)

