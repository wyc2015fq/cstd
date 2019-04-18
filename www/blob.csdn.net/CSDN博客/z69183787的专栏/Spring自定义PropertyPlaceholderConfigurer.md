# Spring自定义PropertyPlaceholderConfigurer - z69183787的专栏 - CSDN博客
2016年10月13日 14:38:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1955
1. CustomPropertyConfigurer.java
Java代码  ![收藏代码](http://antlove.iteye.com/images/icon_star.png)
- package propertyconfig;  
- import java.util.HashMap;  
- import java.util.Map;  
- import java.util.Properties;  
- import java.util.Map.Entry;  
- import org.springframework.beans.BeansException;  
- import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;  
- import org.springframework.beans.factory.config.PropertyPlaceholderConfigurer;  
- import org.springframework.util.PropertyPlaceholderHelper;  
- 
- publicclass CustomPropertyConfigurer extends PropertyPlaceholderConfigurer{  
- privatestatic Map<String,String> properties = new HashMap<String,String>();  
- protectedvoid processProperties(  
-             ConfigurableListableBeanFactory beanFactoryToProcess,  
-             Properties props) throws BeansException {  
- // cache the properties
-         PropertyPlaceholderHelper helper = new PropertyPlaceholderHelper(  
-                 DEFAULT_PLACEHOLDER_PREFIX, DEFAULT_PLACEHOLDER_SUFFIX, DEFAULT_VALUE_SEPARATOR, false);  
- for(Entry<Object,Object> entry:props.entrySet()){  
-             String stringKey = String.valueOf(entry.getKey());  
-             String stringValue = String.valueOf(entry.getValue());  
-             stringValue = helper.replacePlaceholders(stringValue, props);  
-             properties.put(stringKey, stringValue);  
-         }  
- super.processProperties(beanFactoryToProcess, props);  
-     }  
- 
- publicstatic Map<String, String> getProperties() {  
- return properties;  
-     }  
- 
- publicstatic String getProperty(String key){  
- return properties.get(key);  
-     }  
- }  
 2. applicationContext.xml
Xml代码  ![收藏代码](http://antlove.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:p="http://www.springframework.org/schema/p"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd     
-     http://www.springframework.org/schema/aop    
-     http://www.springframework.org/schema/aop/spring-aop-3.0.xsd"    
- default-lazy-init="true"default-autowire="byName"default-init-method=""default-destroy-method="">
- <beanid="propertyConfigurer"class="propertyconfig.CustomPropertyConfigurer">
- <propertyname="locations">
- <list>
- <value>classpath:propertyconfig/project.properties</value>
- </list>
- </property>
- </bean>
- </beans>
 3. project.properties
Properties代码  ![收藏代码](http://antlove.iteye.com/images/icon_star.png)
- site=iteye  
- blog=antlove  
- url=${site}/${blog}  
 4. Main.java测试类
Java代码  ![收藏代码](http://antlove.iteye.com/images/icon_star.png)
- package propertyconfig;  
- import java.util.Map;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- publicclass Main {  
- publicstaticvoid main(String[] args) {  
- new ClassPathXmlApplicationContext("propertyconfig/applicationContext.xml");  
- 
-         Map<String,String> properties = CustomPropertyConfigurer.getProperties();  
- 
-         System.out.println(properties);  
-     }  
- }  
