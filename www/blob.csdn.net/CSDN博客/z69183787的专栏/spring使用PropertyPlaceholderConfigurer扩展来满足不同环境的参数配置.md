# spring使用PropertyPlaceholderConfigurer扩展来满足不同环境的参数配置 - z69183787的专栏 - CSDN博客
2016年10月13日 14:46:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1456
[spring](http://lib.csdn.net/base/javaee)使用PropertyPlaceholderConfigurer扩展来满足不同环境的参数配置，
来自：[http://www.javaarch.net/jiagoushi/548.htm](http://www.javaarch.net/jiagoushi/548.htm)
PropertyPlaceholderConfigurer是spring提供我们来把一些环境变量（[数据库](http://lib.csdn.net/base/mysql)连接相关参数，文件路径等）统一管理起来，然后在bean中指定对应的变量的。但是往往开发环境，测试环境，生成环境的这些参数配置是不同的，那么我们如何使用PropertyPlaceholderConfigurer扩展来满足不同环境的配置需求，而不需要在不同环境需要修改代码或者配置。
1.我们扩展下PropertyPlaceholderConfigurer，可以通过在properties中production.mode默认配置来或者不同环境的配置，然后有限通过取系统环境变量的这个值来作为我们开发环境，测试环境，生产环境的选择。
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8955825#)[copy](http://blog.csdn.net/zhongweijian/article/details/8955825#)
- import java.io.IOException;  
- import java.util.Map.Entry;  
- import java.util.Properties;  
- import java.util.Set;  
- 
- import org.apache.commons.lang.StringUtils;  
- import org.springframework.beans.factory.InitializingBean;  
- import org.springframework.beans.factory.config.PropertyPlaceholderConfigurer;  
- /**
-  * 可以按照不同的运行模式启用相应的配置
-  * 
-  */
- publicclass MutilPropertyPlaceholderConfigurer extends PropertyPlaceholderConfigurer implements InitializingBean {  
- 
- privatestaticfinal String PRODUCTION_MODE = "production.mode";  
- // 缓存所有的属性配置
- private Properties          properties;  
- /**
-      * @return the mode
-      */
- public String getMode() {  
- return properties.getProperty(PRODUCTION_MODE);  
-     }  
- @Override
- protected Properties mergeProperties() throws IOException {  
-         Properties mergeProperties = super.mergeProperties();  
- // 根据路由原则，提取最终生效的properties
- this.properties = new Properties();  
- // 获取路由规则,系统属性设置mode优先
-         String mode = System.getProperty(PRODUCTION_MODE);  
- if (StringUtils.isEmpty(mode)) {  
-             String str = mergeProperties.getProperty(PRODUCTION_MODE);  
-             mode = str != null ? str : "ONLINE";  
-         }  
-         properties.put(PRODUCTION_MODE, mode);  
-         String[] modes = mode.split(",");  
-         Set<Entry<Object, Object>> es = mergeProperties.entrySet();  
- for (Entry<Object, Object> entry : es) {  
-             String key = (String) entry.getKey();  
- int idx = key.lastIndexOf('_');  
-             String realKey = idx == -1 ? key : key.substring(0, idx);  
- if (!properties.containsKey(realKey)) {  
-                 Object value = null;  
- for (String md : modes) {  
-                     value = mergeProperties.get(realKey + "_" + md);  
- if (value != null) {  
-                         properties.put(realKey, value);  
- break;  
-                     }  
-                 }  
- if (value == null) {  
-                     value = mergeProperties.get(realKey);  
- if (value != null) {  
-                         properties.put(realKey, value);  
-                     } else {  
- thrownew RuntimeException("impossible empty property for " + realKey);  
-                     }  
-                 }  
-             }  
-         }  
- return properties;  
-     }  
- /**
-      * 开放此方法给需要的业务
-      * 
-      * @param key
-      * @return
-      */
- public String getProperty(String key) {  
- return resolvePlaceholder(key, properties);  
-     }  
- @Override
- publicvoid afterPropertiesSet() throws Exception {  
- // TODO Auto-generated method stub  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8955825#)[copy](http://blog.csdn.net/zhongweijian/article/details/8955825#)
- 然后我们在properties中可以这么配置：也就是默认配置是ONLINE 生产模式，那么只要在系统变量中没有配置production.mode，则我们取ONLINE的配置，也就是下面的参数取后缀为_ONLINE的配置。  
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8955825#)[copy](http://blog.csdn.net/zhongweijian/article/details/8955825#)
- production.mode=ONLINE  
- 
- 
- #lucene index data dir  
- lucene.index.dir_DEV=e:\\logs\\lucene  
- lucene.index.dir_ONLINE=/home/admin/data  
- 
- #velocity  
- file.resource.loader.cache_DEV=false
- file.resource.loader.modificationCheckInterval_DEV=2
- file.resource.loader.cache_ONLINE=true
- file.resource.loader.modificationCheckInterval_ONLINE=-1
 对应spring的配置为：
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8955825#)[copy](http://blog.csdn.net/zhongweijian/article/details/8955825#)
- <!-- velocity -->  
- <import resource="classpath*:*.xml" />  
- <bean id="velocityConfigurer"
- class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">  
-     <property name="resourceLoaderPath">  
-         <value>WEB-INF/velocity/</value>  
-     </property>  
-     <property name="velocityProperties">  
-         <props>  
-             <prop key="directive.foreach.counter.name">velocityCount</prop>  
-             <prop key="directive.foreach.counter.initial.value">1</prop>  
-             <prop key="input.encoding">GBK</prop>  
-             <prop key="output.encoding">GBK</prop>  
-             <prop key="file.resource.loader.cache">${file.resource.loader.cache}</prop>  
-             <prop key="file.resource.loader.modificationCheckInterval">${file.resource.loader.modificationCheckInterval}</prop>  
-             <prop key="velocimacro.library.autoreload">false</prop>  
-             <prop key="velocimacro.library">macro.vm</prop>  
-         </props>  
-     </property>  
- </bean>  
这种参数包括数据库连接串，文件路径等都可以这么配，因为velocity在测试环境不需要cache，能够修改即生效，但是线上环境加上cache则能提高性能，所以，默认使用ONLINE的配置，但是在测试环境的VM参数中加上-Dproduction.mode=DEV，则在开发环境用的是_DEV后缀的配置，到了线上代码则不用改。非常方便。
最好就是架上placeholder的自定义配置bean
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8955825#)[copy](http://blog.csdn.net/zhongweijian/article/details/8955825#)
- <bean id="placeholder"
- class="org.springweb.core.MutilPropertyPlaceholderConfigurer">  
-     <property name="locations">  
-         <list>  
-             <value>classpath:jdbc.properties</value>  
-             <value>classpath*:*-placeholder.properties</value>  
-         </list>  
-     </property>  
- </bean>  
