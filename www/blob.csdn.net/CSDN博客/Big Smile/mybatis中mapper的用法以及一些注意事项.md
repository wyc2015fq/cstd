# mybatis中mapper的用法以及一些注意事项 - Big Smile - CSDN博客
2017年09月30日 08:18:14[王啸tr1912](https://me.csdn.net/tr1912)阅读数：3466标签：[java																[mybatis](https://so.csdn.net/so/search/s.do?q=mybatis&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java学习																[项目经验](https://blog.csdn.net/tr1912/article/category/6211594)](https://blog.csdn.net/tr1912/article/category/6453602)
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        这几天在公司做项目，到了收尾的阶段，但是发现自己在使用mybatis的时候给自己留下了很多的坑，于是乎花了两天的时间来调试和重新构思自己写下的mapper.xml文件，总算是有些收获，在这里分享给大家。
# 一、何为mapper.xml
        mapper.xml是mybatis中的重要的组成部分，也算是核心之一，我们可以看到的就是他是由一些定义好的查询数据库的语句和一些xml规范定义好的。我们在使用mybatis的时候可以看到，在这里mapper文件相当于是一个dao层接口的实现，因为mapper中定义的查询，id都必须要和dao层的方法是一致的，这样才可以进行查询。这里最能体现mybatis特点的就是这个mapper配置文件了，因为mybatis能从里面的内容读取映射，生成sql语句进行查询。
**二、mapper的写法和注意事项**
        我们的mapper有很多种使用方法，这里简要说明与总结一下：
**（1）、直接当做sql语句使用**
        我们可以使用mapper中的<select><udpate><insert><delete>标签来拼接我们的sql语句，这里写的标签为mybatis解析的头，所以我们对应的sql语句必须写在对应的标签下面，方可成功。
**（2）、加判断的条件查询语句**
         在mapper中，我们可以对条件加上应有的判断，来进行一些分条件的查询，尤其是在where子句中，这里注意的是，使用判断，也就是if也是一个标签为<if test=""></if> 其中在test中为if的判断条件，这里的判断就是一些取值的判断，并没有其他的作用但是写法需要注意，可以直接使用Dao接口中传下来的变量直接进行判断。如下语句为增加了一个判断用于拼接sql语句用的：
```
<if test='state=="0"'>
   AND t_acceptanceform.acceptancestate!='0'
</if>
<if test='state!=0'>
   AND t_acceptanceform.acceptancestate =#{state}
</if>
```
        这里就是在where中通过变量不同的条件来选择不同的语句，使sql语句更加的灵活。
**（3）、增加一些变量集合，用于查询或者返回值**
         这里所指的变量集合其实是两种，一种是map类型的集合，一种是sql中自定义查询字段集合，我们在做返回值还有mybatis自动生成的代码中常见的就是这种map集合，一般叫做resultmap，他是可以和pojo等类型的实体类进行对应的，用于对于查询返回值的接收和发送，可以被mybatis机制自动转换为一些List集合或者其他的集合之类的东西。
格式：
```
<resultMap id="" type="" >     <!--id为标识这个map的唯一标志，不能重复，type为对应到的pojo包路径-->
    <id column="" property="" jdbcType="" />    <!--column是列名，property为对应projo对象名，type就是数据库中的类型-->
    <result column="" property="" jdbcType="" />
</resultMap>
```
第二个是<sql>标签为头的一般查询结果集，他的好处是可以自定义，也可以加入一些判断，我们在做复杂查询的时候一般可以用到他，下面是我写的一个例子：
```
<sql id="outPutOrderDetail">
		outputrepositorycode,
		outdate,
		consignee,
		ordersno,
		repocode,
		transferpath,
		acceptanceformcode,
		address,
		totalproducts,
		createtime,
		batchcount,
		comments,
		customerid,
        pitposition
</sql>
```
使用的查询：
```java
<select id="queryOutPutHead" resultType="com.cn.echuxianshengshop.pojo.ext.OutPutOrderDetail">
	SELECT
	 <include refid="outPutOrderDetail"/>
	FROM
	t_outputorderb , t_acceptanceform ,t_orders
	WHERE
	  ordercode=belongorderid AND ordercode=ordersNo AND outputrepositorycode=#{opCode}
</select>
```
        这里再附上软件自动生成的<sql>标签内容，没有仔细研究过里面的东西，但是用到了for循环和if，应该就是逻辑判断的一种，和写条件都是差不多的。
```
<sql id="Example_Where_Clause" >
    <where >
      <foreach collection="oredCriteria" item="criteria" separator="or" >
        <if test="criteria.valid" >
          <trim prefix="(" suffix=")" prefixOverrides="and" >
            <foreach collection="criteria.criteria" item="criterion" >
              <choose >
                <when test="criterion.noValue" >
                  and ${criterion.condition}
                </when>
                <when test="criterion.singleValue" >
                  and ${criterion.condition} #{criterion.value}
                </when>
                <when test="criterion.betweenValue" >
                  and ${criterion.condition} #{criterion.value} and #{criterion.secondValue}
                </when>
                <when test="criterion.listValue" >
                  and ${criterion.condition}
                  <foreach collection="criterion.value" item="listItem" open="(" close=")" separator="," >
                    #{listItem}
                  </foreach>
                </when>
              </choose>
            </foreach>
          </trim>
        </if>
      </foreach>
    </where>
  </sql>
```
这里就是mybatis里面用于拼接查询条件进行查询的一个拼接判断，可以好好研究一下。
**三、传值的注意 **
        我们都知道，在mapper中写查询的时候，我们都会用到一些查询条件和传入变量进行关联的事情，一般这种都用的是#{变量名}或者是${变量名}来做取值，那么他们到底有什么区别呢？
        1、#{param}会产生PreparedStatement，并且可以安全地设置参数(=?)的值。以为sql语句已经预编译好了，传入参数的时候，不会重新生产sql语句。安全性高。
        2、${parem}则直接将{}号中的param插入字符串，会产生sql注入的问题：
例如：select * from userwhere userName= ${userName} 
输出的结果为
select * from userwhere userName= “小明”
        3、在特定场景下，例如如果在使用诸如order by '{param}'，这时候就可以使用${}
        4、#{}方式能够很大程度防止sql注入，${}方式无法防止sql注入
        5、${}方式一般用于传入数据库对象，例如传入表名
**字符串替换：**
        默认情况下，使用#{}格式的语法会导致MyBatis创建预处理语句属性并以它为背景设置安全的值（比如?）。这样做很安全，很迅速也是首选做法，有时你只是想直接在SQL语句中插入一个不改变的字符串。比如，像ORDER BY，你可以这样来使用：
       ORDER BY ${columnName}
       这里MyBatis不会修改或转义字符串。
       重要：接受从用户输出的内容并提供给语句中不变的字符串，这样做是不安全的。这会导致潜在的SQL注入攻击，因此你不应该允许用户输入这些字段，或者通常自行转义并检查。
      这里一定要注意的是，可以用#{param}的时候尽量不要用${param}。
