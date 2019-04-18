# SSM实现多表连接查询（一对多关联） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月23日 22:42:50[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：4692


    SSM 各种配置信息 就 不再赘述，之前一篇写过。

表1 对应的 pojo      - - - 表1 字段   id、name          List<Two>是表2 查询返回来的结果集

![](https://img-blog.csdn.net/20180323221451543)

表2 对应的 pojo    ---- 表2 字段 id、name、pid             List<Three> 是表3查询返回来的结果集

![](https://img-blog.csdn.net/20180323221516470)

表3 对应的 pojo    ---- 表3 字段 id、name、pid

![](https://img-blog.csdn.net/20180323221526822)

id字段 均 自增。

为了实现 查询 表一的 信息 连带 将表二和 表三的 数据 显示出来

在 维护sql语句的 xml 文件中 配置如下信息。

![](https://img-blog.csdn.net/20180323221902754)

标签 属性 解释：

<mapper namespace="  ">

需要扫描的dao层接口所在的位置


 <resultMap type="  " id="  ">  

resultMap : 这个标签就是用来给 pojo中的字段配置注释的。 使用时机：pojo变量和数据库字段名不一样。以及多表连接查询

type : pojo完全限定名。如果配置了typeAliasesPackage，则可省略包路径。

id:  就是给 这个被 配置了 字段的 pojo 起个别名 ，之后会用到。

<result property="   " column="    "/>

property：pojo中 需要加注释的变量名，

column：注释，对应的数据库中的字段名。由于我第二个表的查询是基于 第一个表的id来查的，而下面需要提供column，所以即使 pojo的变量名，和数据库中的字段名一样 ，也需要配置。

<collection property="twos" column="id" javaType="java.util.List" ofType="Two" select="getTwosByPid">

collection ：给pojo中的子表 集合变量 进行 相关配置。

property：子表对应的pojo中的变量名

column ：关联的外键。

如  select id,name,pId from two where id=    就是基于表1的 id 查询的。

 javaType ： 集合的 类型

 ofType：集合泛型的类型

select ： 集合变量 是通过 哪个查询语句 查询出来的

  <select id="getTwosByPid" resultMap="twoMap" parameterType="java.lang.Integer" >

    select标签： 配置查询语句

    id : dao层接口 对应的方法 名

    resultMap： 返回的结果 需要映射到的 pojo 类名（如果没配置typeAliasesPackage，则需要写类全名）。 

    如果该pojo被 配置过字段，则需要填写 配置pojo的那个标签的id

    parameterType： 执行该 查询语句 传入的 参数类型。 名也要写全


