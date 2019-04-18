# 五种方式让你在java中读取properties文件内容不再是难题 - z69183787的专栏 - CSDN博客
2018年01月04日 17:17:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：213
个人分类：[Spring-属性替换&转换](https://blog.csdn.net/z69183787/article/category/7065996)
[https://www.cnblogs.com/hafiz/p/5876243.html](https://www.cnblogs.com/hafiz/p/5876243.html)
### 一、背景
　　最近，在项目开发的过程中，遇到需要在properties文件中定义一些自定义的变量，以供java程序动态的读取，修改变量，不再需要修改代码的问题。就借此机会把Spring+SpringMVC+Mybatis整合开发的项目中通过java程序读取properties文件内容的方式进行了梳理和分析，现和大家共享。
### 二、项目环境介绍
    Spring 4.2.6.RELEASE
    SpringMvc 4.2.6.RELEASE
    Mybatis 3.2.8
    Maven 3.3.9
    Jdk 1.7
    Idea 15.04
### 三、五种实现方式
方式1.通过context:property-placeholder加载配置文件jdbc.properties中的内容
```
<context:property-placeholder location="classpath:jdbc.properties" ignore-unresolvable="true"/>
```
　　上面的配置和下面配置等价，是对下面配置的简化
```
<bean id="propertyConfigurer" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
   <property name="ignoreUnresolvablePlaceholders" value="true"/>
   <property name="locations">
      <list>
         <value>classpath:jdbc.properties</value>
      </list>
    </property>
</bean>
```
注意：这种方式下，如果你在spring-mvc.xml文件中有如下配置，则一定不能缺少下面的红色部分，关于它的作用以及原理，参见另一篇博客：[context:component-scan标签的use-default-filters属性的作用以及原理分析](http://www.cnblogs.com/hafiz/p/5875770.html)
```
<!-- 配置组件扫描，springmvc容器中只扫描Controller注解 -->
<context:component-scan base-package="com.hafiz.www" use-default-filters="false">
    <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>
</context:component-scan>
```
方式2.使用注解的方式注入，主要用在java代码中使用注解注入properties文件中相应的value值
```
<bean id="prop" class="org.springframework.beans.factory.config.PropertiesFactoryBean">
   <!-- 这里是PropertiesFactoryBean类，它也有个locations属性，也是接收一个数组，跟上面一样 -->
   <property name="locations">
       <array>
          <value>classpath:jdbc.properties</value>
       </array>
   </property>
</bean>
```
方式3.使用util:properties标签进行暴露properties文件中的内容
```
<util:properties id="propertiesReader" location="classpath:jdbc.properties"/>
```
注意：使用上面这行配置，需要在spring-dao.xml文件的头部声明以下红色的部分
```
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:util="http://www.springframework.org/schema/util"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
        http://www.springframework.org/schema/context 
        http://www.springframework.org/schema/context/spring-context-3.2.xsd
        http://www.springframework.org/schema/util 
　　　　 http://www.springframework.org/schema/util/spring-util.xsd">
```
方式4.通过PropertyPlaceholderConfigurer在加载上下文的时候暴露properties到自定义子类的属性中以供程序中使用
```
<bean id="propertyConfigurer" class="com.hafiz.www.util.PropertyConfigurer">
   <property name="ignoreUnresolvablePlaceholders" value="true"/>
   <property name="ignoreResourceNotFound" value="true"/>
   <property name="locations">
       <list>
          <value>classpath:jdbc.properties</value>
       </list>
   </property>
</bean>
```
自定义类PropertyConfigurer的声明如下:
```
package com.hafiz.www.util;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.beans.factory.config.PropertyPlaceholderConfigurer;
import java.util.Properties;
/**
 * Desc:properties配置文件读取类
 * Created by hafiz.zhang on 2016/9/14.
 */
public class PropertyConfigurer extends PropertyPlaceholderConfigurer {
    private Properties props;       // 存取properties配置文件key-value结果
    @Override
    protected void processProperties(ConfigurableListableBeanFactory beanFactoryToProcess, Properties props)
                            throws BeansException {
        super.processProperties(beanFactoryToProcess, props);
        this.props = props;
    }
    public String getProperty(String key){
        return this.props.getProperty(key);
    }
    public String getProperty(String key, String defaultValue) {
        return this.props.getProperty(key, defaultValue);
    }
    public Object setProperty(String key, String value) {
        return this.props.setProperty(key, value);
    }
}
```
使用方式：在需要使用的类中使用@Autowired注解注入即可。
方式5.自定义工具类PropertyUtil，并在该类的static静态代码块中读取properties文件内容保存在static属性中以供别的程序使用
```
package com.hafiz.www.util;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.io.*;
import java.util.Properties;
/**
 * Desc:properties文件获取工具类
 * Created by hafiz.zhang on 2016/9/15.
 */
public class PropertyUtil {
    private static final Logger logger = LoggerFactory.getLogger(PropertyUtil.class);
    private static Properties props;
    static{
        loadProps();
    }
    synchronized static private void loadProps(){
        logger.info("开始加载properties文件内容.......");
        props = new Properties();
        InputStream in = null;
        try {
　　　　　　　<!--第一种，通过类加载器进行获取properties文件流-->
            in = PropertyUtil.class.getClassLoader().getResourceAsStream("jdbc.properties");
　　　　　　  <!--第二种，通过类进行获取properties文件流-->
            //in = PropertyUtil.class.getResourceAsStream("/jdbc.properties");
            props.load(in);
        } catch (FileNotFoundException e) {
            logger.error("jdbc.properties文件未找到");
        } catch (IOException e) {
            logger.error("出现IOException");
        } finally {
            try {
                if(null != in) {
                    in.close();
                }
            } catch (IOException e) {
                logger.error("jdbc.properties文件流关闭出现异常");
            }
        }
        logger.info("加载properties文件内容完成...........");
        logger.info("properties文件内容：" + props);
    }
    public static String getProperty(String key){
        if(null == props) {
            loadProps();
        }
        return props.getProperty(key);
    }
    public static String getProperty(String key, String defaultValue) {
        if(null == props) {
            loadProps();
        }
        return props.getProperty(key, defaultValue);
    }
}
```
说明：这样的话，在该类被加载的时候，它就会自动读取指定位置的配置文件内容并保存到静态属性中，高效且方便，一次加载，可多次使用。
### 四、注意事项及建议
　　以上五种方式，前三种方式比较死板，而且如果你想在带有@Controller注解的Bean中使用，你需要在SpringMVC的配置文件spring-mvc.xml中进行声明，如果你想在带有@Service、@Respository等非@Controller注解的Bean中进行使用，你需要在Spring的配置文件中spring.xml中进行声明。原因请参见另一篇博客：[Spring和SpringMVC父子容器关系初窥](http://www.cnblogs.com/hafiz/p/5875740.html)
　　我个人比较建议第四种和第五种配置方式，第五种为最好，它连工具类对象都不需要注入，直接调用静态方法进行获取，而且只一次加载，效率也高。而且前三种方式都不是很灵活，需要修改@Value的键值。
### 五、测试验证是否可用
1.首先我们创建PropertiesService
```
package com.hafiz.www.service;
/**
 * Desc:java程序获取properties文件内容的service
 * Created by hafiz.zhang on 2016/9/16.
 */
public interface PropertiesService {
    /**
     * 第一种实现方式获取properties文件中指定key的value
     *
     * @return
     */
    String getProperyByFirstWay();
    /**
     * 第二种实现方式获取properties文件中指定key的value
     *
     * @return
     */
    String getProperyBySecondWay();
    /**
     * 第三种实现方式获取properties文件中指定key的value
     *
     * @return
     */
    String getProperyByThirdWay();
    /**
     * 第四种实现方式获取properties文件中指定key的value
     *
     * @param key
     *
     * @return
     */
    String getProperyByFourthWay(String key);
    /**
     * 第四种实现方式获取properties文件中指定key的value
     *
     * @param key
     *
     * @param defaultValue
     *
     * @return
     */
    String getProperyByFourthWay(String key, String defaultValue);
    /**
     * 第五种实现方式获取properties文件中指定key的value
     *
     * @param key
     *
     * @return
     */
    String getProperyByFifthWay(String key);
    /**
     * 第五种实现方式获取properties文件中指定key的value
     *
     * @param key
     *
     * @param defaultValue
     *
     * @return
     */
    String getProperyByFifthWay(String key, String defaultValue);
}
```
2.创建实现类PropertiesServiceImpl
```
package com.hafiz.www.service.impl;
import com.hafiz.www.service.PropertiesService;
import com.hafiz.www.util.PropertyConfigurer;
import com.hafiz.www.util.PropertyUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
/**
 * Desc:java程序获取properties文件内容的service的实现类
 * Created by hafiz.zhang on 2016/9/16.
 */
@Service
public class PropertiesServiceImpl implements PropertiesService {
    @Value("${test}")
    private String testDataByFirst;
    @Value("#{prop.test}")
    private String testDataBySecond;
    @Value("#{propertiesReader[test]}")
    private String testDataByThird;
    @Autowired
    private PropertyConfigurer pc;
    @Override
    public String getProperyByFirstWay() {
        return testDataByFirst;
    }
    @Override
    public String getProperyBySecondWay() {
        return testDataBySecond;
    }
    @Override
    public String getProperyByThirdWay() {
        return testDataByThird;
    }
    @Override
    public String getProperyByFourthWay(String key) {
        return pc.getProperty(key);
    }
    @Override
    public String getProperyByFourthWay(String key, String defaultValue) {
        return pc.getProperty(key, defaultValue);
    }
    @Override
    public String getProperyByFifthWay(String key) {
        return PropertyUtil.getPropery(key);
    }
    @Override
    public String getProperyByFifthWay(String key, String defaultValue) {
        return PropertyUtil.getProperty(key, defaultValue);
    }
}
```
3.控制器类PropertyController
```
package com.hafiz.www.controller;
import com.hafiz.www.service.PropertiesService;
import com.hafiz.www.util.PropertyUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
/**
 * Desc:properties测试控制器
 * Created by hafiz.zhang on 2016/9/16.
 */
@Controller
@RequestMapping("/prop")
public class PropertyController {
    @Autowired
    private PropertiesService ps;
    @RequestMapping(value = "/way/first", method = RequestMethod.GET)
    @ResponseBody
    public String getPropertyByFirstWay(){
        return ps.getProperyByFirstWay();
    }
    @RequestMapping(value = "/way/second", method = RequestMethod.GET)
    @ResponseBody
    public String getPropertyBySecondWay(){
        return ps.getProperyBySecondWay();
    }
    @RequestMapping(value = "/way/third", method = RequestMethod.GET)
    @ResponseBody
    public String getPropertyByThirdWay(){
        return ps.getProperyByThirdWay();
    }
    @RequestMapping(value = "/way/fourth/{key}", method = RequestMethod.GET)
    @ResponseBody
    public String getPropertyByFourthWay(@PathVariable("key") String key){
        return ps.getProperyByFourthWay(key, "defaultValue");
    }
    @RequestMapping(value = "/way/fifth/{key}", method = RequestMethod.GET)
    @ResponseBody
    public String getPropertyByFifthWay(@PathVariable("key") String key){
        return PropertyUtil.getProperty(key, "defaultValue");
    }
}
```
4.jdbc.properties文件
```
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://192.168.1.196:3306/dev?useUnicode=true&characterEncoding=UTF-8
jdbc.username=root
jdbc.password=123456
jdbc.maxActive=200
jdbc.minIdle=5
jdbc.initialSize=1
jdbc.maxWait=60000
jdbc.timeBetweenEvictionRunsMillis=60000
jdbc.minEvictableIdleTimeMillis=300000
jdbc.validationQuery=select 1 from t_user
jdbc.testWhileIdle=true
jdbc.testOnReturn=false
jdbc.poolPreparedStatements=true
jdbc.maxPoolPreparedStatementPerConnectionSize=20
jdbc.filters=stat
#test data
test=com.hafiz.www
```
5.项目结果图
![](https://images2015.cnblogs.com/blog/894443/201609/894443-20160916122326461-398284536.png)
6.项目GitHub地址
[https://github.com/hafizzhang/SSM/branches](https://github.com/hafizzhang/SSM/branches) 页面下的propertiesConfigurer分支。
7.测试结果
　　第一种方式
![](https://images2015.cnblogs.com/blog/894443/201609/894443-20160916122358602-892327193.png)
　　第二种方式
![](https://images2015.cnblogs.com/blog/894443/201609/894443-20160916122417508-1338748030.png)
　　第三种方式
![](https://images2015.cnblogs.com/blog/894443/201609/894443-20160916122434180-44221761.png)
　　第四种方式
![](https://images2015.cnblogs.com/blog/894443/201609/894443-20160916122447398-461335937.png)
　　第五种方式
![](https://images2015.cnblogs.com/blog/894443/201609/894443-20160916122458477-1735576253.png)
### 六、总结
　　通过本次的梳理和测试，我们理解了Spring和SpringMVC的父子容器关系以及context:component-scan标签包扫描时最容易忽略的use-default-filters属性的作用以及原理。能够更好地定位和快速解决再遇到的问题。总之，棒棒哒~~~
