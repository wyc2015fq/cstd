# Spring中PropertyPlaceholderConfigurer的使用 - aisoo的专栏 - CSDN博客
2013年03月01日 16:18:30[aisoo](https://me.csdn.net/aisoo)阅读数：514
### 基本的使用方法是：
<bean id="propertyConfigurerForAnalysis" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
    <property name="location">
        <value>classpath:/spring/include/dbQuery.properties</value>
    </property>
</bean>
其中classpath是引用src目录下的文件写法。
当存在多个Properties文件时，配置就需使用locations了：(2)
<bean id="propertyConfigurer" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
    <property name="locations">
       <list>
          <value>classpath:/spring/include/jdbc-parms.properties</value>
          <value>classpath:/spring/include/base-config.properties</value>
        </list>
    </property>
</bean>
接下来我们要使用多个PropertyPlaceholderConfigurer来分散配置，达到整合多工程下的多个分散的Properties 文件，其配置如下：(3)
<bean id="propertyConfigurerForProject1" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
    <property name="order" value="1" />
    <property name="ignoreUnresolvablePlaceholders" value="true" />
    <property name="location">
       <value>classpath:/spring/include/dbQuery.properties</value>
    </property>
</bean><bean id="propertyConfigurerForProject2" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
    <property name="order" value="2" />
    <property name="ignoreUnresolvablePlaceholders" value="true" />
    <property name="locations">
      <list>
        <value>classpath:/spring/include/jdbc-parms.properties</value>
        <value>classpath:/spring/include/base-config.properties</value>
      </list>
    </property>
</bean>
其中order属性代表其加载顺序，而ignoreUnresolvablePlaceholders为是否忽略不可解析的 Placeholder，如配置了多个PropertyPlaceholderConfigurer，则需设置为true
