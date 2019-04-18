# mybatis中的typeAlias - z69183787的专栏 - CSDN博客
2017年01月05日 15:08:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3228
Mybatis主配置文件 configLocation.xml
```
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <!--dataource-->
        <property name="dataSource" ref="dataSource"/>
        <!--Mapper files-->
        <property name="mapperLocations" value="classpath*:config/mybatis/local/*.xml" />
        <!--Entity package-->
        <!--<property name="typeAliasesPackage" value="com.dianping.zebra.dao.entity" />-->
        <property name="typeAliasesPackage" value="com.dianping.emidas.lottery.dao.entity"/>
        <property name="configLocation" value="classpath:config/mybatis/**mybatis-configuration.xml**" />
    </bean>
```
typeAlias通过一个简单的别名来表示一个冗长的类型，这样可以降低复杂度。
表示如下：
<!-- business typeAlias Strart -->
<typeAlias alias="SisUserRole" type="com.gzduanxun.sysmgr.model.SisUserRole"/>
<!-- business typeAlias end -->
若不加这句会报如下错误：
org.springframework.beans.factory.BeanCreationException: 
Error creating bean with name 'xxxxxxxxxxx' defined in ServletContext resource [/WEB-INF/spring_business.xml]: Invocation of init method failed; nested exception is[Java](http://lib.csdn.net/base/javase).lang.IllegalArgumentException:
 java.lang.RuntimeException: Error parsing Mapper XML. Cause: org.apache.ibatis.builder.BuilderException: Error resolving class . Cause: org.apache.ibatis.type.TypeException: Could not resolve type alias 'SisUserRole'.  Cause: java.lang.ClassNotFoundException:
 Cannot find class: SisUserRole
表示[spring](http://lib.csdn.net/base/javaee)找不到该类SisUserRole；
为什么会找不到？
因为在mybatis中的sql写成了省略的部分SisUserRole，是找不到该类的。增加了typeAlias之后就不用写com.gzduanxun.sysmgr.model了。
<insert id="save" parameterType="SisUserRole" keyProperty="id">
参考：[http://www.yiibai.com/mybatis/mybatis_typealiases.html](http://www.yiibai.com/mybatis/mybatis_typealiases.html)
别名是一个较短的Java 类型的名称。这只是与XML 配置文件相关联，减少输入多余的完整类
名。例如：
<typeAliases>
	<typeAlias alias="Author" type="domain.blog.Author"/>
	<typeAlias alias="Blog" type="domain.blog.Blog"/>
	<typeAlias alias="Comment" type="domain.blog.Comment"/>
	<typeAlias alias="Post" type="domain.blog.Post"/>
	<typeAlias alias="Section" type="domain.blog.Section"/>
	<typeAlias alias="Tag" type="domain.blog.Tag"/>
</typeAliases>
在这个配置中，您就可以在想要使用"domain.blog.Blog"的地方使用别名“Blog”了。
对常用的java 类型，已经内置了一些别名支持。这些别名都是不区分大小写的。注意java
的基本数据类型，它们进行了特别处理，加了“_”前缀。
![](http://www.yiibai.com/uploads/allimg/140806/1-140P620191NF.png)
![](http://www.yiibai.com/uploads/allimg/140806/1-140P6201933641.png)
