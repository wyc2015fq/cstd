# Spring整合MyBatis错误：org.apache.ibatis.binding.BindingException - 零度的博客专栏 - CSDN博客
2016年07月14日 15:27:25[零度anngle](https://me.csdn.net/zmx729618)阅读数：26024
     MyBatis Mapper使用Spring注入： applicationContext.xml
```
<!--这一段让我们的MapperInterface能够最终在Spring中生成Bean最后被AutoWired-->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <property name="basePackage" value="com.zmx.ssm.user.mapper" /> 
        <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"/>            
    </bean>
```
       使用上面方式注入时需要注意下面问题： 
        - interface = com.zmx.ssm.user.mapper.UserMapper.class 
        - mapper file = com.zmx.ssm.user.mapper.UserMapper.xml  
        - mapper namespace = com.zmx.ssm.user.mapper.UserMapper
       这三者应该设置为相同，否则会出现：org.apache.ibatis.binding.BindingException: Invalid bound statement 异常。
       使用上述配置，-mapper file也可以和-interface、-namespace不同的（注意：interface和namespace必须相同），但这时需要进行配置。指明mapper file的位置。
        配置如下(两种方式)：
**       方式一：**
1.在MyBatis配置文件中添加mappers：mybatis-config.xml 
```
<mappers>
        <mapper resource="mapper/UserMapper.xml" /> 
    </mappers>
```
<mappers>
	<mapper resource="com/itsyc/fmpp/mappers/StudentMapper.xml" /> <!-- Mapper文件可以和interface不在相同路径下！ -->
</mappers>
      2.使用configLocation，在Spring配置文件中添加MyBatis配置文件的引用：applicationContext.xml 
```
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="dataSource" ref="dataSource" />      
        <!--指定了Mybatis配置文件的路径-->
        <property name="configLocation" value="classpath:mybatis-config.xml"/>           
    </bean>
```
**     方式二：**
      使用mapperLocations，MyBatis-Spring会自动地扫描该路径下的所有Mapper文件： applicationContext.xml
```
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="dataSource" ref="dataSource" />      
        <!--等价于上面的配置-->
        <property name="mapperLocations" value="classpath:/mapper/*Mapper.xml"/>  
        <property name="typeAliasesPackage" value="com.zmx.ssm.user.domain" />          
    </bean>
```
