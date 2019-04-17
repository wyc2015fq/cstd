# Springmvc获取properties中的内容实践 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年07月25日 09:58:42[QuJack](https://me.csdn.net/A784586)阅读数：581








**Springmvc获取properties中的内容实践**

关于spring和springmvc的帖子很多，很多技术极少也很详细，也有关于获取properties内容的介绍。但是主流的几个帖子，我都尝试了，有些还是有问题，经过实测，给出其中一种方法。

1.spring-mvc.xml加入如下内容

xmlns:util="http://www.springframework.org/schema/util


http://www.springframework.org/schema/util

http://www.springframework.org/schema/util/spring-util-4.0.xsd



```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:mvc="http://www.springframework.org/schema/mvc"
xmlns:context="http://www.springframework.org/schema/context"
xmlns:util="http://www.springframework.org/schema/util"
xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc.xsd
http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd
http://www.springframework.org/schema/util
http://www.springframework.org/schema/util/spring-util-4.0.xsd">

<!-- 引入配置文件-->
<util:properties id="settings" location="classpath:conf.properties"/>
<context:component-scan base-package="com.sohuporndetect"/>
<!-- com.sohuporndetect 注入类所在的包名-->
```
**2.注入类**

```java
package com.sohuporndetect;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

/**
* Created by jjc on 2017/7/24.
*/
@Service
public class Conf {
@Value("#{settings['a']}")
public String aa;
@Value("#{settings['b']}")
public String bb;
}
```


3.con.properties


```java
a=123
b=aaaaa
```
注意，等号右边字符串不需要使用双引号


4.使用---在某个类中注入

```java
@Autowired
private Conf config;
```
在非静态代码块下可以使用如下方式调用；

```java
@Autowired
private Conf config;
```
















