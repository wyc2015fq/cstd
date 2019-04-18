# Spring @Autowired 注入集合类型的问题 - z69183787的专栏 - CSDN博客
2017年12月18日 16:39:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1018
从spring 2.5版本开始，spring提供了基于注解方式的依赖注入。在容器的xml配置文件中，添加如下的配置
[html][view
 plain](http://blog.csdn.net/qianlibie/article/details/44079647#)[copy](http://blog.csdn.net/qianlibie/article/details/44079647#)
- <context:annotation-config/>
- <context:component-scanbase-package="com.example"/>
即可扫描com.example包及其子包下所有使用特定注解注明的类，创建他们的实例并完成他们之间的依赖注入。非常的方便，大大方便了系统开发的配置。
昨天一个同事在使用@Autowired自动注入依赖的集合bean时碰到了问题。定义了Manager，并声明自动注入一个Set<String>
[java][view
 plain](http://blog.csdn.net/qianlibie/article/details/44079647#)[copy](http://blog.csdn.net/qianlibie/article/details/44079647#)
- package com.example;  
- 
- import java.util.Set;  
- 
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.context.ApplicationListener;  
- import org.springframework.context.event.ApplicationContextEvent;  
- import org.springframework.context.event.ContextRefreshedEvent;  
- import org.springframework.stereotype.Component;  
- 
- @Component
- publicclass Manager implements ApplicationListener<ApplicationContextEvent> {  
- 
- @Autowired
- private Set<String> locations;  
- 
- @Override
- publicvoid onApplicationEvent(ApplicationContextEvent event) {  
- if (event instanceof ContextRefreshedEvent) {  
- for (String loc : locations) {  
-                 System.out.println("location -> " + loc);  
-             }  
-         }  
-     }  
- }  
容器配置文件如下
[html][view
 plain](http://blog.csdn.net/qianlibie/article/details/44079647#)[copy](http://blog.csdn.net/qianlibie/article/details/44079647#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:util="http://www.springframework.org/schema/util"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="  
-             http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.1.xsd  
-             http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.1.xsd  
-             http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-4.1.xsd">
- 
- <context:annotation-config/>
- 
- <context:component-scanbase-package="com.example"/>
- 
- <beanid="resourcePackage"class="java.lang.String">
- <constructor-argvalue="com.example.resource"/>
- </bean>
- 
- <beanid="resourceLocation"class="java.lang.String">
- <constructor-argvalue="classpath:config"/>
- </bean>
- 
- <util:setid="locations"value-type="java.lang.String">
- <value>com.example.module</value>
- <value>com.example.common.entity</value>
- </util:set>
- </beans>
期望的是注入配置文件中id为locations的set，而事实却是容器启动后，输出的却是
location -> com.example.resource
location -> classpath:config
很奇怪，debug跟踪源码，在org.springframework.beans.factory.support.DefaultListableBeanFactory.doResolveDependency(DependencyDescriptor, String, Set<String>,
 TypeConverter) 方法中发现了原因
[java][view
 plain](http://blog.csdn.net/qianlibie/article/details/44079647#)[copy](http://blog.csdn.net/qianlibie/article/details/44079647#)
- if (type.isArray()) {  
-     Class<?> componentType = type.getComponentType();  
-     DependencyDescriptor targetDesc = new DependencyDescriptor(descriptor);  
-     targetDesc.increaseNestingLevel();  
-     Map<String, Object> matchingBeans = findAutowireCandidates(beanName, componentType, targetDesc);  
- if (matchingBeans.isEmpty()) {  
- if (descriptor.isRequired()) {  
-             raiseNoSuchBeanDefinitionException(componentType, "array of " + componentType.getName(), descriptor);  
-         }  
- returnnull;  
-     }  
- if (autowiredBeanNames != null) {  
-         autowiredBeanNames.addAll(matchingBeans.keySet());  
-     }  
-     TypeConverter converter = (typeConverter != null ? typeConverter : getTypeConverter());  
-     Object result = converter.convertIfNecessary(matchingBeans.values(), type);  
- if (this.dependencyComparator != null && result instanceof Object[]) {  
-         Arrays.sort((Object[]) result, adaptDependencyComparator(matchingBeans));  
-     }  
- return result;  
- }  
- elseif (Collection.class.isAssignableFrom(type) && type.isInterface()) {  
-     Class<?> elementType = descriptor.getCollectionType();  
- if (elementType == null) {  
- if (descriptor.isRequired()) {  
- thrownew FatalBeanException("No element type declared for collection [" + type.getName() + "]");  
-         }  
- returnnull;  
-     }  
-     DependencyDescriptor targetDesc = new DependencyDescriptor(descriptor);  
-     targetDesc.increaseNestingLevel();  
-     Map<String, Object> matchingBeans = findAutowireCandidates(beanName, elementType, targetDesc);  
- if (matchingBeans.isEmpty()) {  
- if (descriptor.isRequired()) {  
-             raiseNoSuchBeanDefinitionException(elementType, "collection of " + elementType.getName(), descriptor);  
-         }  
- returnnull;  
-     }  
- if (autowiredBeanNames != null) {  
-         autowiredBeanNames.addAll(matchingBeans.keySet());  
-     }  
-     TypeConverter converter = (typeConverter != null ? typeConverter : getTypeConverter());  
-     Object result = converter.convertIfNecessary(matchingBeans.values(), type);  
- if (this.dependencyComparator != null && result instanceof List) {  
-         Collections.sort((List<?>) result, adaptDependencyComparator(matchingBeans));  
-     }  
- return result;  
- }  
- elseif (Map.class.isAssignableFrom(type) && type.isInterface()) {  
-     Class<?> keyType = descriptor.getMapKeyType();  
- if (keyType == null || !String.class.isAssignableFrom(keyType)) {  
- if (descriptor.isRequired()) {  
- thrownew FatalBeanException("Key type [" + keyType + "] of map [" + type.getName() +  
- "] must be assignable to [java.lang.String]");  
-         }  
- returnnull;  
-     }  
-     Class<?> valueType = descriptor.getMapValueType();  
- if (valueType == null) {  
- if (descriptor.isRequired()) {  
- thrownew FatalBeanException("No value type declared for map [" + type.getName() + "]");  
-         }  
- returnnull;  
-     }  
-     DependencyDescriptor targetDesc = new DependencyDescriptor(descriptor);  
-     targetDesc.increaseNestingLevel();  
-     Map<String, Object> matchingBeans = findAutowireCandidates(beanName, valueType, targetDesc);  
- if (matchingBeans.isEmpty()) {  
- if (descriptor.isRequired()) {  
-             raiseNoSuchBeanDefinitionException(valueType, "map with value type " + valueType.getName(), descriptor);  
-         }  
- returnnull;  
-     }  
- if (autowiredBeanNames != null) {  
-         autowiredBeanNames.addAll(matchingBeans.keySet());  
-     }  
- return matchingBeans;  
- }  
- else {  
-     Map<String, Object> matchingBeans = findAutowireCandidates(beanName, type, descriptor);  
- if (matchingBeans.isEmpty()) {  
- if (descriptor.isRequired()) {  
-             raiseNoSuchBeanDefinitionException(type, "", descriptor);  
-         }  
- returnnull;  
-     }  
- if (matchingBeans.size() > 1) {  
-         String primaryBeanName = determineAutowireCandidate(matchingBeans, descriptor);  
- if (primaryBeanName == null) {  
- thrownew NoUniqueBeanDefinitionException(type, matchingBeans.keySet());  
-         }  
- if (autowiredBeanNames != null) {  
-             autowiredBeanNames.add(primaryBeanName);  
-         }  
- return matchingBeans.get(primaryBeanName);  
-     }  
- // We have exactly one match.
-     Map.Entry<String, Object> entry = matchingBeans.entrySet().iterator().next();  
- if (autowiredBeanNames != null) {  
-         autowiredBeanNames.add(entry.getKey());  
-     }  
- return entry.getValue();  
- }  
对于@Autowired声明的数组、集合类型，spring并不是根据beanName去找容器中对应的bean，而是把容器中所有类型与集合（数组）中元素类型相同的bean构造出一个对应集合，注入到目标bean中。对应到上问配置文件中，就是把容器中所有类型为java.lang.String的bean放到新建的Set中，然后注入到Manager bean中。也就是把resourcePackage和resourceLoaction这两个String注入了，导致上面的输出结果。
在spring reference中也发现相关说明。
[@Autowired](http://docs.spring.io/spring/docs/4.1.0.RELEASE/spring-framework-reference/html/beans.html#beans-autowired-annotation)
If you intend to express annotation-driven injection by name, do not primarily use @Autowired, even if is technically capable of referring to a bean name through @Qualifier values. Instead, use the JSR-250 @Resource annotation, which is semantically defined
 to identify a specific target component by its unique name, with the declared type being irrelevant for the matching process.
As a specific consequence of this semantic difference, beans that are themselves defined as a collection or map type cannot be injected through @Autowired, because type matching is not properly applicable to them. Use @Resource for such beans, referring to
 the specific collection or map bean by unique name.
@Autowired applies to fields, constructors, and multi-argument methods, allowing for narrowing through qualifier annotations at the parameter level. By contrast, @Resource is supported only for fields and bean property setter methods with a single argument.
 As a consequence, stick with qualifiers if your injection target is a constructor or a multi-argument method.
从上面的说明中找到解决办法就是注入集合类型不要使用@Autowired，而使用@Resource注解。同时Spring官方也是不推荐使用@Autowired的。
