# MyBatis批量插入数据 - BlueSky - CSDN博客
2016年01月20日 19:32:14[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：383
个人分类：[Java Spring](https://blog.csdn.net/ASIA_kobe/article/category/6039238)

在程序中封装了一个List集合对象，然后需要把该集合中的实体插入到数据库中，由于项目使用了Spring+MyBatis的配置，所以打算使用MyBatis批量插入，由于之前没用过批量插入，在网上找了一些资料后最终实现了，把详细过程贴出来。
实体类TrainRecord结构如下：
Java代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- publicclass TrainRecord implements Serializable {  
- 
- privatestaticfinallong serialVersionUID = -1206960462117924923L;  
- 
- privatelong id;  
- 
- privatelong activityId;  
- 
- privatelong empId;  
- 
- privateint flag;  
- 
- private String addTime;  
- 
- //setter and getter 
- 
- }  
对应的mapper.xml中定义如下：
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <resultMaptype="TrainRecord"id="trainRecordResultMap">
- <idcolumn="id"property="id"jdbcType="BIGINT"/>
- <resultcolumn="add_time"property="addTime"jdbcType="VARCHAR"/>
- <resultcolumn="emp_id"property="empId"jdbcType="BIGINT"/>
- <resultcolumn="activity_id"property="activityId"jdbcType="BIGINT"/>
- <resultcolumn="flag"property="status"jdbcType="VARCHAR"/>
- </resultMap>
mapper.xml中批量插入方法的定义如下：
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <insertid="addTrainRecordBatch"useGeneratedKeys="true"parameterType="java.util.List">
- <selectKeyresultType="long"keyProperty="id"order="AFTER">
-         SELECT  
-         LAST_INSERT_ID()  
- </selectKey>
-     insert into t_train_record (add_time,emp_id,activity_id,flag)   
-     values  
- <foreachcollection="list"item="item"index="index"separator=",">
-         (#{item.addTime},#{item.empId},#{item.activityId},#{item.flag})  
- </foreach>
- </insert>
对于foreach标签的解释参考了网上的资料，具体如下：
foreach的主要用在构建in条件中，它可以在SQL语句中进行迭代一个集合。foreach元素的属性主要有 item，index，collection，open，separator，close。item表示集合中每一个元素进行迭代时的别名，index指 定一个名字，用于表示在迭代过程中，每次迭代到的位置，open表示该语句以什么开始，separator表示在每次进行迭代之间以什么符号作为分隔 符，close表示以什么结束，在使用foreach的时候最关键的也是最容易出错的就是collection属性，该属性是必须指定的，但是在不同情况
 下，该属性的值是不一样的，主要有一下3种情况：
1.如果传入的是单参数且参数类型是一个List的时候，collection属性值为list
2.如果传入的是单参数且参数类型是一个array数组的时候，collection的属性值为array
3.如果传入的参数是多个的时候，我们就需要把它们封装成一个Map了，当然单参数也可以封装成map
关于foreach的具体例子在这里就先不举，以后有机会可以把每一种情况都举一个例子列出来。
MysqlBaseDAO：
Java代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- publicclass MySqlBaseDAO extends SqlSessionDaoSupport {  
- /**
-      * insert:插入操作. <br/>
-      *
-      * @author chenzhou
-      * @param method 插入操作的方法名
-      * @param entity 查询参数或实体类
-      * @return 返回影响的行数
-      * @since JDK 1.6
-      */
- publicint insert(String method,Object entity){  
- returnthis.getSqlSession().insert(method, entity);  
-     }  
- 
- //其余方法省略
- }  
TrainRecord实体类对应的TrainRecordDAO 定义如下：
Java代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- publicclass TrainRecordDAO extends MySqlBaseDAO {  
- /**
-      * addTrainRecordBatch:批量插入培训记录. <br/>
-      *
-      * @author chenzhou
-      * @param trainRecordList 培训记录list集合
-      * @return 影响的行数
-      * @since JDK 1.6
-      */
- publicint addTrainRecordBatch(List<TrainRecord> trainRecordList){  
- returnthis.insert("addTrainRecordBatch", trainRecordList);  
-     }  
- 
- //省略其余的方法
- }  
然后直接调用TrainRecordDAO 中的 addTrainRecordBatch方法就可以批量插入了。
特别说明的是在尝试时碰到了一个让人无语的错误，折腾了我差不多1个小时才解决。就是我在定义mapper.xml中的插入方法时一般都会默认用<![CDATA[ ]]>标签把sql语句括起来，如下所示：
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <![CDATA[
-     select * from t_train_record t where t.activity_id=#{activityId}
- ]]>
这样做的目的主要是因为在 XML 元素中，"<" 和 "&" 是非法的。"<" 会产生错误，因为解析器会把该字符解释为新元素的开始。"&" 也会产生错误，因为解析器会把该字符解释为字符实体的开始。而sql语句或者脚本语句中可能会存在 "<" 或 "&" 字符。为了避免错误，可以将sql语句定义为 CDATA。CDATA 部分中的所有内容都会被解析器忽略。
当时我在addTrainRecordBatch方法中也用了这种用法：
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <![CDATA[
-     insert into t_train_record (add_time,emp_id,activity_id,flag) 
-     values
-     <foreach collection="list" item="item" index="index" separator="," >
-         (#{item.addTime},#{item.empId},#{item.activityId},#{item.flag})
-     </foreach>
- ]]>
结果程序在执行时老是报错: com.mysql.jdbc.exceptions.jdbc4.MySQLSyntaxErrorException，查看错误信息就是传入的参数都是null。纠结了很久，后面才发现原来是<![CDATA[ ]]>把xml中的<foreach>标签括起来后把标签直接当成字符串处理了。后面把外面的<![CDATA[ ]]>去掉后就能正常执行了。
