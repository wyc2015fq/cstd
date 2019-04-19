# Spring Boot+MyBatis使用原生SQL，执行动态自定义SQL语句 - Ms.Fox的专栏 - CSDN博客
2019年01月14日 00:26:03[lulu-lu](https://me.csdn.net/smbluesky)阅读数：59
因为项目中需要做一些操作，比如动态在数据库中创建表，动态给数据库添加字段等等；
当时网上找了很多资料没看懂，后来看了一篇文章总算来了灵感，但没保存链接，所以一时没法贴出来。
毕竟自己在做的过程中遇到了很多坎坷，所以就自己整理一下了。
----------------------------------   割背景   -------------------------------------------
其实MyBatis是可以支持原生的SQL的，而且不需要做什么额外的操作，比如我们要新增一条记录的时候，我们是：
<insert id="saveTableColumn" >
    insert into table_column (id,table_id,column_label,column_code)
    values (#{id},#{table_id},#{column_label},#{column_code})
</insert>
这种方式是我们常用的MyBatis新增记录的方式，如果我想给某张表添加一个字段的话，语句是：
alter table 表名 add column 字段名  + 字段类型等等……
同样地，在MyBatis中，我们也可以写成这样：
<insert id="addColumn"  parameterType="java.util.Map">
    alter  table ${table} add column ${column} ${type} ;
</insert>
注意这里是使用的是${}，而不是上面的#{}，而且多了一个
parameterType="java.util.Map"
这是因为如果我们使用#{}的时候，MyBatis会自动帮我们把我们的字段自动加上单引号''，使用${}的时候就不会。
使用Map传参，MyBatis可以直接根据你的key取值。
在DAO中，我们写个对应的方法名就可以了
public int addColumn(Map map);
那么在其他地方调用这个的时候，只需要直接往Map中put值就行。
map.put("table",操作的表名);
map.put("column",字段名称);
// 调用设置字段类型的方法
map.put("type",拼接字段类型);
我的字段类型是使用了一个方法，根据需要拼接为字符串即可，方法里面有项目资料，这里就不贴出来了。
执行增加字段的方法，可以看到输出的信息：
我的语句里字段类型给错了，大家不要介意，先实现，再优化哈！
补充：
利用上面的方法，如果想要完全地自定义SQL也是没有问题的，直接把你的语句写在${}里面就行了，比如
<insert id="addColumn"  parameterType="java.util.Map">
    ${sql};
</insert>
当然，如果要返回指定的类型，如结果集等，就在上面那里加上resultType="返回类型"就可以了。
另外就是，Mapper中的方法不一定是要<insert><update><delete>等等也可以，里面具体的原理我就不是很清楚了。
--------------------- 
作者：潇I洒 
来源：CSDN 
原文：https://blog.csdn.net/tg928600774/article/details/80620640 
版权声明：本文为博主原创文章，转载请附上博文链接！
