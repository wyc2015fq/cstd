# Spring3 MVC 集成Velocity中文支持 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年01月27日 23:32:46[gloomyfish](https://me.csdn.net/jia20003)阅读数：5309








**Maven中加上Velocity的依赖如下：**



```
<!-- Velocity -->
		<dependency>
			<groupId>org.apache.velocity</groupId>
			<artifactId>velocity</artifactId>
			<version>1.5</version>
		</dependency>
		<dependency>
			<groupId>velocity-tools</groupId>
			<artifactId>velocity-tools-generic</artifactId>
			<version>1.2</version>
		</dependency>
```
**配置VelocityConfig**


```
<bean id="velocityConfig" class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
        <property name="resourceLoaderPath">
            <value>/</value>
        </property>
        <property name="velocityProperties">
	      <props>
	       <prop key="input.encoding">utf-8</prop>
	       <prop key="output.encoding">utf-8</prop>
	       <prop key="contentType">text/html;charset=utf-8</prop>
	      </props>
     	</property>
    </bean>
```
**配置ViewReslover**

```
<bean name="negotiationViewResolver" class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver"> 
		<property name="order" value="1"></property> 
		<property name="mediaTypes">
            <map>
                <entry key="json" value="application/json" />
            </map>
        </property>
        <property name="viewResolvers">
			<list>
			    <bean id="viewResolver" class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
			    <property name="order" value="2" />
			    	<property name="prefix"><value>/velocity/</value></property>
			        <property name="suffix"><value>.vm</value></property>
			        <property name="exposeSpringMacroHelpers"><value>true</value></property>
			        <property name="contentType"><value>text/html;charset=utf-8</value></property>
				</bean>
			</list>
		</property>
		<property name="defaultViews">
			<list>
				<bean class="org.springframework.web.servlet.view.json.MappingJacksonJsonView" />
			</list>
		</property>
	</bean>
```
在.vm的文件中加上如下的html header属性：



```
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta http-equiv="Content-Language" content="zh-CN" />
```
**特别要注意的是：**
一定在IDE中确保你的文件保存格式为utf-8否则中文还是会出现乱码。

这博客写，自己都感觉太水！属于敷衍自己！















