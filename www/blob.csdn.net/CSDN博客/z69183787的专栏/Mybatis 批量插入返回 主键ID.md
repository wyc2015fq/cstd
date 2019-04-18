# Mybatis 批量插入返回 主键ID - z69183787的专栏 - CSDN博客
2017年08月04日 15:47:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3336
[http://blog.csdn.net/jiangeeq/article/details/55047116](http://blog.csdn.net/jiangeeq/article/details/55047116)
官方修复 https://github.com/mybatis/mybatis-3/pull/547
**注：使用时 mapper中的 collection 参数必须为 list，即外部的mapper接口中只接受两种情况：**
**1、不Param注解，默认值list**
**2、使用param注解，但注解的value也必须为list**
**原因如下，代码中默认取了key为 list 的对象，可参考[http://blog.csdn.net/z69183787/article/details/76686930](http://blog.csdn.net/z69183787/article/details/76686930)**
```java
private Collection<Object> getParameters(Object parameter) {
 
    Collection<Object> parameters = null;
 
    if (parameter instanceof Collection) {
 
      parameters = (Collection) parameter;
 
    } else if (parameter instanceof Map) {
 
      Map parameterMap = (Map) parameter;
 
      if (parameterMap.containsKey("collection")) {
 
        parameters = (Collection) parameterMap.get("collection");
 
      } else if (parameterMap.containsKey("list")) {
 
        parameters = (List) parameterMap.get("list");
 
      } else if (parameterMap.containsKey("array")) {
 
        parameters = Arrays.asList((Object[]) parameterMap.get("array"));
 
      }
 
    }
 
    if (parameters == null) {
 
      parameters = new ArrayList<Object>();
 
      parameters.add(parameter);
 
    }
 
    return parameters;
 
  }
```
**正文：**
之前版本的mybatis 3.3.X有Bug，故无法获取或报错，以下版本可以正确使用：
```
<dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>3.4.4</version>
        </dependency>
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>1.3.1</version>
        </dependency>
```
mapper：
```java
int batchInsert(List<X> list);
```
xml：
```
<insert id="batchInsert" useGeneratedKeys="true" keyProperty="ID" parameterType="java.util.ArrayList">
        insert into X(ActivityId,UserId, Ext,Status, CreateTime,
        UpdateTime)
        VALUES
        <foreach collection="list" item="item" index="index" separator=",">
            (#{item.activityId},#{item.userId},#{item.ext},#{item.status}, NOW(),NOW())
        </foreach>
    </insert>
```
session：
```
<bean id="sqlSession" class="org.mybatis.spring.SqlSessionTemplate">
        <constructor-arg index="0" ref="sessionFactory" />
    </bean>
	<bean id="sessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
		<property name="dataSource" ref="dataSource" />
		<property name="configLocation" value="classpath:config/mybatis-config.xml" />
	</bean>
```
test：
```java
@Autowired
    private SqlSessionTemplate sqlSessionTemplate;
    @Autowired
    private XMapper xMapper;
    @Test
    public void testBatchInsert() throws Exception {
        List<X> list = new ArrayList<>();
        list.add(new CocaShareInfoPoJo(11,11,"11",11));
        list.add(new CocaShareInfoPoJo(22,22,"22",22));
        list.add(new CocaShareInfoPoJo(33,33,"33",33));
        list.add(new CocaShareInfoPoJo(44,44,"44",44));
        //int result = sqlSessionTemplate.insert("com.xxxx.xMapper.batchInsert",list);
        xMapper.batchInsert(list);
        System.out.println();
    }
```
主键存储在 入参 list中 各个对象的ID 字段中。
