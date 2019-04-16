# Java深入 - MyBatis的常用方法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年10月21日 17:25:41[initphp](https://me.csdn.net/initphp)阅读数：6526
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









MyBatis我们这篇文章主要记录一些常用的操作方法，这样在开发和使用的过程中这篇文章可以当做工具书来使用。

### MyBatis的数据源配置



```java
<bean id="dataSource" class="org.springframework.jdbc.datasource.DriverManagerDataSource"> 
    <property name="driverClassName" value="${jdbc.driverClassName}" /> 
    <property name="url" value="${jdbc.url}" /> 
</bean> 
	<!-- 加载myBatis-config.xml配置文件，以及扫描myBatis/目录下每个DAO对应的SQL配置的XML文件 -->
	<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
		<property name="dataSource" ref="dataSource" />
		<property name="configLocation" value="classpath:myBatis-config.xml" />
		<property name="mapperLocations" value="classpath:mybatis/*.xml" />
	</bean>
	<!-- 将basePackage下的interface，转换为spring bean,service中可以直接 注入使用 -->
	<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
		<property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"></property>
		<property name="basePackage" value="com.test.my.dal.dao"></property>
	</bean>
```




### 背景

背景：

1. 数据表，我们有一张user的数据表：

![](https://img-blog.csdn.net/20141021171457860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. Dao的持久化对象：



```java
public class UserPojo extends AbstractEntity {

    /**
     * 
     */
    private static final long serialVersionUID = -4963909230377087790L;

    private Integer           id;

    private String            username;

    private String            password;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

}
```




### 常用操作

#### 1. myBatis-config.xml配置



```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
	<settings>
		<!-- 是否启用 数据中 a_column 自动映射 到 java类中驼峰命名的属性。[默认:false] -->
		<setting name="mapUnderscoreToCamelCase" value="true" />
	</settings>
	<typeAliases>
		<!-- 映射关系 -->
		<typeAlias alias="UserPojo" type="com.test.my.dal.dao.domain.UserPojo" />
	</typeAliases>
	<mappers></mappers>
</configuration>
```




#### 2. insert操作



```
<!-- 新增 -->
	<insert id="addUser" parameterType="UserPojo" useGeneratedKeys="true"
		keyProperty="id">
		INSERT INTO
		user
		<set>
			<if test="username != null">
				username = #{username},
			</if>
			<if test="password != null">
				password = #{password},
			</if>
		</set>
	</insert>
```


Dao中：





```java
void addUser(UserPojo pojo);
```


设置了useGeneratedKeys="true"和keyProperty="id"后，主键ID会放置到UserPojo的id属性值中。






#### 3. update操作



```
<!-- 更新一条数据 -->
	<update id="updateUser" parameterType="UserPojo">
		UPDATE user
		<set>
			<if test="username != null">
				username = #{username},
			</if>
			<if test="password != null">
				password = #{password},
			</if>
		</set>
		WHERE id = #{id}
	</update>
```


Dao中：





```java
int updateUser(UserPojo pojo);
```


返回的结果为int类型，是更新的影响条数，如果更新成功，则大于0，更新失败，则为0





#### 4. delete操作



```
<!-- 删除一条数据 -->
<delete id="delete" parameterType="int" >
DELETE FROM user WHERE id = #{id}
</delete>
```


Dao中：





```java
int delete(int id);
```


返回结果为int类型，如果删除成功，则返回影响记录数，否则返回0





#### 5. SQL块使用

例如：



```
<!-- 字段 -->
	<sql id="field">
		`id`,
		`username`,
		`password`
	</sql>
```


使用：




`<include refid="field"/>`




#### 6. SELECT使用



```
<!-- 通过一个ID搜索 -->
	<select id="getById" parameterType="int" resultType="UserPojo">
		SELECT <include refid="field"/> FROM user WHERE id = #{id}
	</select>

	<!-- 搜索列表 -->
	<select id="getByList" parameterType="int" resultType="UserPojo">
		SELECT <include refid="field"/>
		FROM user ORDER BY id DESC LIMIT 10
	</select>
```




#### 7. if语句使用



```
<if test="username != null">
	username = #{username},
</if>
<if test="password != null">
	password = #{password},
</if>
```




#### 8. where语句组装



```
<where>
<if test=”state != null”>
state = #{state}
</if>
<if test=”title != null”>
AND title like #{title}
</if>
<if test=”author != null and author.name != null”>
AND title like #{author.name}
</if>
</where>
```




#### 9. SET语句组装



```
<set>
<if test="username != null">username=#{username},</if>
<if test="password != null">password=#{password},</if>
<if test="email != null">email=#{email},</if>
<if test="bio != null">bio=#{bio}</if>
</set>
```




#### 10. 条件语句



```
<choose>
<when test=”title != null”>
AND title like #{title}
</when>
<when test=”author != null and author.name != null”>
AND title like #{author.name}
</when>
<otherwise>
AND featured = 1
</otherwise>
</choose>
```




#### 11. foreach语句使用



```
WHERE ID in
<foreach item="item" index="index" collection="list"
open="(" separator="," close=")">
#{item}
</foreach>
```


其中collection可以选择list或者数组，一般使用list比较多一些。






#### 12. @Param使用

如果Dao中有多个参数传递进来，可以使用



```java
@Param("uids") List<Long> uids
```


类似于这样的方式来处理，这样在XML中就可以直接使用 




`#{uids}`


























