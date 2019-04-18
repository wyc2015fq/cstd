# Spring自定义占位符替换(PropertyPlaceholderConfigurer) - z69183787的专栏 - CSDN博客
2016年09月05日 13:57:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4343
提示：由于作者水平和时间有限，请仅以参考的态度阅读。
## 引言
在使用SpringMVC做Web开发的时候，为了便于统一管理配置项，常常会看到用占位符的配置方式。这样，可以将分散在spring配置文件中的配置项的值集中到一个(多个)属性文件中，方便管理。
比如定义了一个bean，属性的值使用占位符，如下(applicationContext.xml)
**[html]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- <beanid= "funnelData"class ="com.company.project.web.FunnelData">
- <propertyname="name"value="${funnel.name}"/>
- <propertyname="value"value="${funnel.value}"/>
- </bean>
接着在其他properties文件中指定占位符所代表的值，如下(bean.properties)
**[plain]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- funnel.name= kiseki  
- funnel.value=1234  
然后告诉spring这个properties文件的位置，这是通过配置PropertyPlaceholderConfigurer的bean来做到的，如下(applicationContext.xml)
**[html]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- <beanid= "placeHolder"
- class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
- <propertyname="location">
- <value>classpath:/application.properties</value>
- </property>
- </bean>
## 分析
虽然在3.1开始官方文档推荐优先使用PropertySourcesPlaceholderConfigurer取代PropertyPlaceholderConfigurer，但是研究分析下PropertyPlaceholderConfigurer还是会有不少收获。spring占位符替换主要涉及到BeanFactoryPostProcessor接口和PropertyPlaceholderConfigurer、PlaceholderConfigurerSupport、PropertyResourceConfigurer三个类。
Spring提供了的一种叫做BeanFactoryPostProcessor的容器扩展机制。它允许我们在容器实例化对象之前，对容器中的BeanDefinition中的信息做一定的修改(比如对某些字段的值进行修改，这就是占位符替换的根本)。于是就需要说下BeanFactoryPostProcessor接口了，以下BeanFactoryPostProcessor的定义
**[java]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- publicinterface BeanFactoryPostProcessor {  
- void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException;  
- }  
在web项目的spring上下文初始化中，spring在实例化bean之前，会先实例化出实现了BeanFactoryPostProcessor接口的bean，并调用postProcessBeanFactory方法，对BeanFactory中的BeanDefinition进行处理。

看看PropertyPlaceholderConfigurer、PlaceholderConfigurerSupport、PropertyResourceConfigurer三个类的定义
**[java]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- publicclass PropertyPlaceholderConfigurer extends PlaceholderConfigurerSupport  
- publicabstractclass PlaceholderConfigurerSupport extends PropertyResourceConfigurer  
- implements BeanNameAware, BeanFactoryAware  
- publicabstractclass PropertyResourceConfigurer extends PropertiesLoaderSupport  
- implements BeanFactoryPostProcessor, PriorityOrdered  
可以看到PropertyPlaceholderConfigurer类的祖先类PropertyResourceConfigurer实现了BeanFactoryPostProcessor接口。
先看看PropertyResourceConfigurer的postProcessBeanFactory()方法
**[java]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- publicvoid postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {  
- try {  
-       Properties mergedProps = mergeProperties();  
- // Convert the merged properties, if necessary.
-       convertProperties(mergedProps);  
- // Let the subclass process the properties.
-       processProperties(beanFactory, mergedProps);  
-    } catch (IOException ex) {  
- thrownew BeanInitializationException("Could not load properties", ex);  
-    }  
- }  
这个方法整合好Properties，然后以BeanFactory和Properties作为参数调用PropertyPlaceholderConfigurer的processProperties方法。
接着看PropertyPlaceholderConfigurer的processProperties()方法
**[java]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- protectedvoid processProperties(ConfigurableListableBeanFactory beanFactoryToProcess, Properties props)  
- throws BeansException {  
-    StringValueResolver valueResolver = new PlaceholderResolvingStringValueResolver(props);  
- this.doProcessProperties(beanFactoryToProcess, valueResolver);  
- }  
这个类就实例化了一个StringValueResolver对象，然后用BeanFactory和StringValueResolver对象调用PlaceholderConfigurerSupport#doProcessProperties()
再看PlaceholderConfigurerSupport的doProcessProperties()方法
**[java]**[view
 plain](http://blog.csdn.net/saylessanddomore/article/details/42168543#)[copy](http://blog.csdn.net/saylessanddomore/article/details/42168543#)
- protectedvoid doProcessProperties(ConfigurableListableBeanFactory beanFactoryToProcess,  
-       StringValueResolver valueResolver) {  
-    BeanDefinitionVisitor visitor = new BeanDefinitionVisitor(valueResolver);  
-    String[] beanNames = beanFactoryToProcess.getBeanDefinitionNames();  
- for (String curName : beanNames) {  
- // Check that we're not parsing our own bean definition,
- // to avoid failing on unresolvable placeholders in properties file locations.
- if (!(curName.equals(this.beanName) && beanFactoryToProcess.equals(this .beanFactory ))) {  
-          BeanDefinition bd = beanFactoryToProcess.getBeanDefinition(curName);  
- try {  
-             visitor.visitBeanDefinition(bd);  
-          } catch (Exception ex) {  
- thrownew BeanDefinitionStoreException(bd.getResourceDescription(), curName, ex.getMessage());  
-          }  
-       }  
-    }  
- 
- // New in Spring 2.5: resolve placeholders in alias target names and aliases as well.
-    beanFactoryToProcess.resolveAliases(valueResolver);  
- 
- // New in Spring 3.0: resolve placeholders in embedded values such as annotation attributes.
-    beanFactoryToProcess.addEmbeddedValueResolver(valueResolver);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这个方法就会取出BeanFactory中的BeanDefinition，然后循环处理除了本身以外的bean的占位符替换
## 扩展
在了解了PropertyPlaceholderConfigurer做占位符替换的流程之后，我们应该有所启发：
1、要定制自己的占位符替换实现，入口就在BeanFactoryPostProcessor接口。实现BeanFactoryPostProcessor接口，并替换掉PropertyPlaceholderConfigurer即可。
2、占位符替换过程中，最主要的是Properties，整合出自己的Properties后，spring现成的很多代码可以继续使用。
目前我们采用Zookeeper做配置中心，用于管理多个APP实例上的配置，而基本的思路就是实现BeanFactoryPostProcessor接口，从Zookeeper上取相应节点，构造出Properties。
附上PropertyPlaceholderConfigurer的源码，可以基于该类的实现自己的 占位符替换功能（properties或缓存中）
```java
/*
 * Copyright 2002-2008 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.springframework.beans.factory.config;
import java.util.HashSet;
import java.util.Properties;
import java.util.Set;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanDefinitionStoreException;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.BeanFactoryAware;
import org.springframework.beans.factory.BeanNameAware;
import org.springframework.core.Constants;
import org.springframework.util.StringUtils;
import org.springframework.util.StringValueResolver;
/**
 * A property resource configurer that resolves placeholders in bean property values of
 * context definitions. It <i>pulls</i> values from a properties file into bean definitions.
 *
 * <p>The default placeholder syntax follows the Ant / Log4J / JSP EL style:
 *
 * <pre class="code">${...}</pre>
 *
 * Example XML context definition:
 *
 * <pre class="code"><bean id="dataSource" class="org.springframework.jdbc.datasource.DriverManagerDataSource">
 *   <property name="driverClassName"><value>${driver}</value></property>
 *   <property name="url"><value>jdbc:${dbname}</value></property>
 * </bean></pre>
 *
 * Example properties file:
 *
 * <pre class="code">driver=com.mysql.jdbc.Driver
 * dbname=mysql:mydb</pre>
 *
 * PropertyPlaceholderConfigurer checks simple property values, lists, maps,
 * props, and bean names in bean references. Furthermore, placeholder values can
 * also cross-reference other placeholders, like:
 *
 * <pre class="code">rootPath=myrootdir
 * subPath=${rootPath}/subdir</pre>
 *
 * In contrast to PropertyOverrideConfigurer, this configurer allows to fill in
 * explicit placeholders in context definitions. Therefore, the original definition
 * cannot specify any default values for such bean properties, and the placeholder
 * properties file is supposed to contain an entry for each defined placeholder.
 *
 * <p>If a configurer cannot resolve a placeholder, a BeanDefinitionStoreException
 * will be thrown. If you want to check against multiple properties files, specify
 * multiple resources via the "locations" setting. You can also define multiple
 * PropertyPlaceholderConfigurers, each with its <i>own</i> placeholder syntax.
 *
 * <p>Default property values can be defined via "properties", to make overriding
 * definitions in properties files optional. A configurer will also check against
 * system properties (e.g. "user.dir") if it cannot resolve a placeholder with any
 * of the specified properties. This can be customized via "systemPropertiesMode".
 *
 * <p>Note that the context definition <i>is</i> aware of being incomplete;
 * this is immediately obvious to users when looking at the XML definition file.
 * Hence, placeholders have to be resolved; any desired defaults have to be
 * defined as placeholder values as well (for example in a default properties file).
 *
 * <p>Property values can be converted after reading them in, through overriding
 * the {@link #convertPropertyValue} method. For example, encrypted values can
 * be detected and decrypted accordingly before processing them.
 *
 * @author Juergen Hoeller
 * @since 02.10.2003
 * @see #setLocations
 * @see #setProperties
 * @see #setPlaceholderPrefix
 * @see #setPlaceholderSuffix
 * @see #setSystemPropertiesModeName
 * @see System#getProperty(String)
 * @see #convertPropertyValue
 * @see PropertyOverrideConfigurer
 */
public class PropertyPlaceholderConfigurer extends PropertyResourceConfigurer
    implements BeanNameAware, BeanFactoryAware {
	/** Default placeholder prefix: "${" */
	public static final String DEFAULT_PLACEHOLDER_PREFIX = "${";
	/** Default placeholder suffix: "}" */
	public static final String DEFAULT_PLACEHOLDER_SUFFIX = "}";
	/** Never check system properties. */
	public static final int SYSTEM_PROPERTIES_MODE_NEVER = 0;
	/**
	 * Check system properties if not resolvable in the specified properties.
	 * This is the default.
	 */
	public static final int SYSTEM_PROPERTIES_MODE_FALLBACK = 1;
	/**
	 * Check system properties first, before trying the specified properties.
	 * This allows system properties to override any other property source.
	 */
	public static final int SYSTEM_PROPERTIES_MODE_OVERRIDE = 2;
	private static final Constants constants = new Constants(PropertyPlaceholderConfigurer.class);
	private String placeholderPrefix = DEFAULT_PLACEHOLDER_PREFIX;
	private String placeholderSuffix = DEFAULT_PLACEHOLDER_SUFFIX;
	private int systemPropertiesMode = SYSTEM_PROPERTIES_MODE_FALLBACK;
	private boolean searchSystemEnvironment = true;
	private boolean ignoreUnresolvablePlaceholders = false;
	private String nullValue;
	private String beanName;
	private BeanFactory beanFactory;
	/**
	 * Set the prefix that a placeholder string starts with.
	 * The default is "${".
	 * @see #DEFAULT_PLACEHOLDER_PREFIX
	 */
	public void setPlaceholderPrefix(String placeholderPrefix) {
		this.placeholderPrefix = placeholderPrefix;
	}
	/**
	 * Set the suffix that a placeholder string ends with.
	 * The default is "}".
	 * @see #DEFAULT_PLACEHOLDER_SUFFIX
	 */
	public void setPlaceholderSuffix(String placeholderSuffix) {
		this.placeholderSuffix = placeholderSuffix;
	}
	/**
	 * Set the system property mode by the name of the corresponding constant,
	 * e.g. "SYSTEM_PROPERTIES_MODE_OVERRIDE".
	 * @param constantName name of the constant
	 * @throws java.lang.IllegalArgumentException if an invalid constant was specified
	 * @see #setSystemPropertiesMode
	 */
	public void setSystemPropertiesModeName(String constantName) throws IllegalArgumentException {
		this.systemPropertiesMode = constants.asNumber(constantName).intValue();
	}
	/**
	 * Set how to check system properties: as fallback, as override, or never.
	 * For example, will resolve ${user.dir} to the "user.dir" system property.
	 * <p>The default is "fallback": If not being able to resolve a placeholder
	 * with the specified properties, a system property will be tried.
	 * "override" will check for a system property first, before trying the
	 * specified properties. "never" will not check system properties at all.
	 * @see #SYSTEM_PROPERTIES_MODE_NEVER
	 * @see #SYSTEM_PROPERTIES_MODE_FALLBACK
	 * @see #SYSTEM_PROPERTIES_MODE_OVERRIDE
	 * @see #setSystemPropertiesModeName
	 */
	public void setSystemPropertiesMode(int systemPropertiesMode) {
		this.systemPropertiesMode = systemPropertiesMode;
	}
	/**
	 * Set whether to search for a matching system environment variable
	 * if no matching system property has been found. Only applied when
	 * "systemPropertyMode" is active (i.e. "fallback" or "override"), right
	 * after checking JVM system properties.
	 * <p>Default is "true". Switch this setting off to never resolve placeholders
	 * against system environment variables. Note that it is generally recommended
	 * to pass external values in as JVM system properties: This can easily be
	 * achieved in a startup script, even for existing environment variables.
	 * <p><b>NOTE:</b> Access to environment variables does not work on the
	 * Sun VM 1.4, where the corresponding {@link System#getenv} support was
	 * disabled - before it eventually got re-enabled for the Sun VM 1.5.
	 * Please upgrade to 1.5 (or higher) if you intend to rely on the
	 * environment variable support.
	 * @see #setSystemPropertiesMode
	 * @see java.lang.System#getProperty(String)
	 * @see java.lang.System#getenv(String)
	 */
	public void setSearchSystemEnvironment(boolean searchSystemEnvironment) {
		this.searchSystemEnvironment = searchSystemEnvironment;
	}
	/**
	 * Set whether to ignore unresolvable placeholders. Default is "false":
	 * An exception will be thrown if a placeholder cannot be resolved.
	 */
	public void setIgnoreUnresolvablePlaceholders(boolean ignoreUnresolvablePlaceholders) {
		this.ignoreUnresolvablePlaceholders = ignoreUnresolvablePlaceholders;
	}
	/**
	 * Set a value that should be treated as <code>null</code> when
	 * resolved as a placeholder value: e.g. "" (empty String) or "null".
	 * <p>Note that this will only apply to full property values,
	 * not to parts of concatenated values.
	 * <p>By default, no such null value is defined. This means that
	 * there is no way to express <code>null</code> as a property
	 * value unless you explictly map a corresponding value here.
	 */
	public void setNullValue(String nullValue) {
		this.nullValue = nullValue;
	}
	/**
	 * Only necessary to check that we're not parsing our own bean definition,
	 * to avoid failing on unresolvable placeholders in properties file locations.
	 * The latter case can happen with placeholders for system properties in
	 * resource locations.
	 * @see #setLocations
	 * @see org.springframework.core.io.ResourceEditor
	 */
	public void setBeanName(String beanName) {
		this.beanName = beanName;
	}
	/**
	 * Only necessary to check that we're not parsing our own bean definition,
	 * to avoid failing on unresolvable placeholders in properties file locations.
	 * The latter case can happen with placeholders for system properties in
	 * resource locations.
	 * @see #setLocations
	 * @see org.springframework.core.io.ResourceEditor
	 */
	public void setBeanFactory(BeanFactory beanFactory) {
		this.beanFactory = beanFactory;
	}
	protected void processProperties(ConfigurableListableBeanFactory beanFactoryToProcess, Properties props)
			throws BeansException {
		StringValueResolver valueResolver = new PlaceholderResolvingStringValueResolver(props);
		BeanDefinitionVisitor visitor = new BeanDefinitionVisitor(valueResolver);
		String[] beanNames = beanFactoryToProcess.getBeanDefinitionNames();
		for (int i = 0; i < beanNames.length; i++) {
			// Check that we're not parsing our own bean definition,
			// to avoid failing on unresolvable placeholders in properties file locations.
			if (!(beanNames[i].equals(this.beanName) && beanFactoryToProcess.equals(this.beanFactory))) {
				BeanDefinition bd = beanFactoryToProcess.getBeanDefinition(beanNames[i]);
				try {
					visitor.visitBeanDefinition(bd);
				}
				catch (BeanDefinitionStoreException ex) {
					throw new BeanDefinitionStoreException(bd.getResourceDescription(), beanNames[i], ex.getMessage());
				}
			}
		}
		// New in Spring 2.5: resolve placeholders in alias target names and aliases as well.
		beanFactoryToProcess.resolveAliases(valueResolver);
	}
	/**
	 * Parse the given String value recursively, to be able to resolve
	 * nested placeholders (when resolved property values in turn contain
	 * placeholders again).
	 * @param strVal the String value to parse
	 * @param props the Properties to resolve placeholders against
	 * @param visitedPlaceholders the placeholders that have already been visited
	 * during the current resolution attempt (used to detect circular references
	 * between placeholders). Only non-null if we're parsing a nested placeholder.
	 * @throws BeanDefinitionStoreException if invalid values are encountered
	 * @see #resolvePlaceholder(String, java.util.Properties, int)
	 */
	protected String parseStringValue(String strVal, Properties props, Set visitedPlaceholders)
	    throws BeanDefinitionStoreException {
		StringBuffer buf = new StringBuffer(strVal);
		int startIndex = strVal.indexOf(this.placeholderPrefix);
		while (startIndex != -1) {
			int endIndex = findPlaceholderEndIndex(buf, startIndex);
			if (endIndex != -1) {
				String placeholder = buf.substring(startIndex + this.placeholderPrefix.length(), endIndex);
				if (!visitedPlaceholders.add(placeholder)) {
					throw new BeanDefinitionStoreException(
							"Circular placeholder reference '" + placeholder + "' in property definitions");
				}
				// Recursive invocation, parsing placeholders contained in the placeholder key.
				placeholder = parseStringValue(placeholder, props, visitedPlaceholders);
				// Now obtain the value for the fully resolved key...
				String propVal = resolvePlaceholder(placeholder, props, this.systemPropertiesMode);
				if (propVal != null) {
					// Recursive invocation, parsing placeholders contained in the
					// previously resolved placeholder value.
					propVal = parseStringValue(propVal, props, visitedPlaceholders);
					buf.replace(startIndex, endIndex + this.placeholderSuffix.length(), propVal);
					if (logger.isTraceEnabled()) {
						logger.trace("Resolved placeholder '" + placeholder + "'");
					}
					startIndex = buf.indexOf(this.placeholderPrefix, startIndex + propVal.length());
				}
				else if (this.ignoreUnresolvablePlaceholders) {
					// Proceed with unprocessed value.
					startIndex = buf.indexOf(this.placeholderPrefix, endIndex + this.placeholderSuffix.length());
				}
				else {
					throw new BeanDefinitionStoreException("Could not resolve placeholder '" + placeholder + "'");
				}
				visitedPlaceholders.remove(placeholder);
			}
			else {
				startIndex = -1;
			}
		}
		return buf.toString();
	}
	private int findPlaceholderEndIndex(CharSequence buf, int startIndex) {
		int index = startIndex + this.placeholderPrefix.length();
		int withinNestedPlaceholder = 0;
		while (index < buf.length()) {
			if (StringUtils.substringMatch(buf, index, this.placeholderSuffix)) {
				if (withinNestedPlaceholder > 0) {
					withinNestedPlaceholder--;
					index = index + this.placeholderSuffix.length();
				}
				else {
					return index;
				}
			}
			else if (StringUtils.substringMatch(buf, index, this.placeholderPrefix)) {
				withinNestedPlaceholder++;
				index = index + this.placeholderPrefix.length();
			}
			else {
				index++;
			}
		}
		return -1;
	}
	/**
	 * Resolve the given placeholder using the given properties, performing
	 * a system properties check according to the given mode.
	 * <p>Default implementation delegates to <code>resolvePlaceholder
	 * (placeholder, props)</code> before/after the system properties check.
	 * <p>Subclasses can override this for custom resolution strategies,
	 * including customized points for the system properties check.
	 * @param placeholder the placeholder to resolve
	 * @param props the merged properties of this configurer
	 * @param systemPropertiesMode the system properties mode,
	 * according to the constants in this class
	 * @return the resolved value, of null if none
	 * @see #setSystemPropertiesMode
	 * @see System#getProperty
	 * @see #resolvePlaceholder(String, java.util.Properties)
	 */
	protected String resolvePlaceholder(String placeholder, Properties props, int systemPropertiesMode) {
		String propVal = null;
		if (systemPropertiesMode == SYSTEM_PROPERTIES_MODE_OVERRIDE) {
			propVal = resolveSystemProperty(placeholder);
		}
		if (propVal == null) {
			propVal = resolvePlaceholder(placeholder, props);
		}
		if (propVal == null && systemPropertiesMode == SYSTEM_PROPERTIES_MODE_FALLBACK) {
			propVal = resolveSystemProperty(placeholder);
		}
		return propVal;
	}
	/**
	 * Resolve the given placeholder using the given properties.
	 * The default implementation simply checks for a corresponding property key.
	 * <p>Subclasses can override this for customized placeholder-to-key mappings
	 * or custom resolution strategies, possibly just using the given properties
	 * as fallback.
	 * <p>Note that system properties will still be checked before respectively
	 * after this method is invoked, according to the system properties mode.
	 * @param placeholder the placeholder to resolve
	 * @param props the merged properties of this configurer
	 * @return the resolved value, of <code>null</code> if none
	 * @see #setSystemPropertiesMode
	 */
	protected String resolvePlaceholder(String placeholder, Properties props) {
		return props.getProperty(placeholder);
	}
	/**
	 * Resolve the given key as JVM system property, and optionally also as
	 * system environment variable if no matching system property has been found.
	 * @param key the placeholder to resolve as system property key
	 * @return the system property value, or <code>null</code> if not found
	 * @see #setSearchSystemEnvironment
	 * @see java.lang.System#getProperty(String)
	 * @see java.lang.System#getenv(String)
	 */
	protected String resolveSystemProperty(String key) {
		try {
			String value = System.getProperty(key);
			if (value == null && this.searchSystemEnvironment) {
				value = System.getenv(key);
			}
			return value;
		}
		catch (Throwable ex) {
			if (logger.isDebugEnabled()) {
				logger.debug("Could not access system property '" + key + "': " + ex);
			}
			return null;
		}
	}
	/**
	 * BeanDefinitionVisitor that resolves placeholders in String values,
	 * delegating to the <code>parseStringValue</code> method of the
	 * containing class.
	 */
	private class PlaceholderResolvingStringValueResolver implements StringValueResolver {
		private final Properties props;
		public PlaceholderResolvingStringValueResolver(Properties props) {
			this.props = props;
		}
		public String resolveStringValue(String strVal) throws BeansException {
			String value = parseStringValue(strVal, this.props, new HashSet());
			return (value.equals(nullValue) ? null : value);
		}
	}
}
```
别人的理解：http://blog.csdn.net/turkeyzhou/article/details/2915438
在第二编对BeanFactory的分析中，我们老能看见BeanFactoyPostProcessor的身影，那么在这一节中，我们来详细的讨论一下
BeanFactoryPostProcessor的代码结构，从中学习他的优秀之处；
BeanFactoryPostProcessor能够修改bean配置文件中的bean的定义；使得我们能够进行一些额外的处理；
在spring中，我们几乎不用自己扩展这个接口，因为他内置了很多实现，但是，我们从原理上和代码上来分析其功能的实现；
一下是他的类图实现：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/turkeyzhou/EntryImages/20080911/BeanFactoryPostProcessor.png)
拿PropertyPlaceholderConfigurer举例；
PropertyPlaceholderConfigurer的功能是这样的，我们在配置文件中配置如下Bean:
- <beanid="PropertyPlaceholderConfigurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
- <propertyname="order">1</property>
- <propertyname="locations">
- <list>
- <value>userinfo.properties</value>
- </list>
- </property>
- </bean>
- 
- <beanid="user"class="org.test.UserInfo">
- <propertyname="order" value="${db.userName}"></property>
- <propertyname="order" value="${db.password}"></property>
- </property>
- </bean>
userinfo.properties:
db.username:scott
db.password:tiger
然后在ApplicationContext下面会自动调用这个PostProcessor;把${db.userName}转换为scott；
在BeanFactory下面的话，必须手动生成以上PostProcesor对象，并且手动调用postProcessorBeanFactory(configureableBeanFactory）方法；
那么我们现在来看一下代码的实现
在PropertyResourceConfigurer中，定义了postProcessBeanFactory方法，定义了方法执行的流程，使用了模板模式；
将具体算法的实现暴露到了子类；
public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        try {
            Properties mergedProps = mergeProperties();
            // Convert the merged properties, if necessary.
            convertProperties(mergedProps);
            // Let the subclass process the properties.
            processProperties(beanFactory, mergedProps);
        }
        catch (IOException ex) {
            throw new BeanInitializationException("Could not load properties", ex);
        }
    }
模板方法1：mergeProperties()如下：
protected Properties mergeProperties() throws IOException {
        Properties result = new Properties();
        if (this.localOverride) {
            // Load properties from file upfront, to let local properties override.
            loadProperties(result);
        }
        if (this.localProperties != null) {
            for (int i = 0; i < this.localProperties.length; i++) {
                Properties props = this.localProperties[i];
                // Use propertyNames enumeration to also catch default properties.
                for (Enumeration en = props.propertyNames(); en.hasMoreElements();) {
                    String key = (String) en.nextElement();
                    result.setProperty(key, props.getProperty(key));
                }
            }
        }
        if (!this.localOverride) {
            // Load properties from file afterwards, to let those properties override.
            loadProperties(result);
        }
        return result;
    }
在这个方法中，将加载你在构造bean的时候传入的properties值，然后存储到这个PostProcessor中，方便覆盖bean定义的元数据，如${db.username}等等；
模板方法processProperties被推到子类实现了：
protected void processProperties(ConfigurableListableBeanFactory beanFactoryToProcess, Properties props)
            throws BeansException {
        //构造一个BeanDefinition访问器（前一节已经分析过），是其内部类：
        BeanDefinitionVisitor visitor = new PlaceholderResolvingBeanDefinitionVisitor(props);
        //得到所有beanName
        String[] beanNames = beanFactoryToProcess.getBeanDefinitionNames();
        for (int i = 0; i < beanNames.length; i++) {
            // Check that we're not parsing our own bean definition,
            // to avoid failing on unresolvable placeholders in properties file locations.
            if (!(beanNames[i].equals(this.beanName) && beanFactoryToProcess.equals(this.beanFactory))) {
                BeanDefinition bd = beanFactoryToProcess.getBeanDefinition(beanNames[i]);
                try {
                    //在这段代码中会遍历所有的Bean定义的带来${}的属性，包括map ,list，String等等；
                    visitor.visitBeanDefinition(bd);
                }
                catch (BeanDefinitionStoreException ex) {
                    throw new BeanDefinitionStoreException(bd.getResourceDescription(), beanNames[i], ex.getMessage());
                }
            }
        }
    }
内部的BeanDefinition访问器：
private class PlaceholderResolvingBeanDefinitionVisitor extends BeanDefinitionVisitor {
        private final Properties props;
        public PlaceholderResolvingBeanDefinitionVisitor(Properties props) {
            this.props = props;
        }
        protected String resolveStringValue(String strVal) throws BeansException {
            return parseStringValue(strVal, this.props, new HashSet());
        }
    }
在访问器中visitBeanDefinition(bd)会遍历此BeanDefinition的proerty,constructor等等可以设置${}的地方例如propertiy:
protected void visitPropertyValues(MutablePropertyValues pvs) {
        PropertyValue[] pvArray = pvs.getPropertyValues();
        for (int i = 0; i < pvArray.length; i++) {
            PropertyValue pv = pvArray[i];
            //分别解决每个属性字段的解析；
            Object newVal = resolveValue(pv.getValue());
            if (!ObjectUtils.nullSafeEquals(newVal, pv.getValue())) {
                pvs.addPropertyValue(pv.getName(), newVal);
            }
        }
    }
如何解析，是暴露到子类中去进行的如PropertyPlaceholderConfigurer是对${}进行外部文件的替换，我们也可以自己实现别的替换方式，如：****替换位"corey"，很无聊吧: ->;
resolveStringValue((String) value);：
protected Object resolveValue(Object value) {
        if (value instanceof BeanDefinition) {
            visitBeanDefinition((BeanDefinition) value);
        }
        else if (value instanceof BeanDefinitionHolder) {
            visitBeanDefinition(((BeanDefinitionHolder) value).getBeanDefinition());
        }
        else if (value instanceof RuntimeBeanReference) {
      RuntimeBeanReference ref = (RuntimeBeanReference) value;
      String newBeanName = resolveStringValue(ref.getBeanName());
            if (!newBeanName.equals(ref.getBeanName())) {
                return new RuntimeBeanReference(newBeanName);
            }
        }
        else if (value instanceof List) {
            visitList((List) value);
        }
        else if (value instanceof Set) {
            visitSet((Set) value);
        }
        else if (value instanceof Map) {
            visitMap((Map) value);
        }
        else if (value instanceof TypedStringValue) {
            TypedStringValue typedStringValue = (TypedStringValue) value;
            String visitdString = resolveStringValue(typedStringValue.getValue());
            typedStringValue.setValue(visitdString);
        }
        else if (value instanceof String) {
            return resolveStringValue((String) value);
        }
        return value;
    }
那＄｛userName｝举例：在PropertyPlaceholderConfigurer中：
protected String parseStringValue(String strVal, Properties props, Set visitedPlaceholders)
        throws BeanDefinitionStoreException {
        StringBuffer buf = new StringBuffer(strVal);
        //提取出${}中间的字符串，
        int startIndex = strVal.indexOf(this.placeholderPrefix);
        while (startIndex != -1) {
            int endIndex = buf.toString().indexOf(
                this.placeholderSuffix, startIndex + this.placeholderPrefix.length());
            if (endIndex != -1) {
                String placeholder = buf.substring(startIndex + this.placeholderPrefix.length(), endIndex);
                if (!visitedPlaceholders.add(placeholder)) {
                    throw new BeanDefinitionStoreException(
                            "Circular placeholder reference '" + placeholder + "' in property definitions");
                }
                //用System.getEnv和外部的properties文件替代了${}中间的值
                String propVal = resolvePlaceholder(placeholder, props, this.systemPropertiesMode);
                if (propVal != null) {
                    // Recursive invocation, parsing placeholders contained in the
                    // previously resolved placeholder value.
                //嵌套执行;直至无法解析；
                    propVal = parseStringValue(propVal, props, visitedPlaceholders);
                    buf.replace(startIndex, endIndex + this.placeholderSuffix.length(), propVal);
                    if (logger.isDebugEnabled()) {
                        logger.debug("Resolved placeholder '" + placeholder + "' to value [" + propVal + "]");
                    }
                    startIndex = buf.toString().indexOf(this.placeholderPrefix, startIndex + propVal.length());
                }
                else if (this.ignoreUnresolvablePlaceholders) {
                    // Proceed with unprocessed value.
                    startIndex = buf.toString().indexOf(this.placeholderPrefix, endIndex + this.placeholderSuffix.length());
                }
                else {
                    throw new BeanDefinitionStoreException("Could not resolve placeholder '" + placeholder + "'");
                }
                visitedPlaceholders.remove(placeholder);
            }
            else {
                startIndex = -1;
            }
        }
        return buf.toString();
    }
在这里，模板模式再一次展现了他的魅力，我想在这里讨论一下：PropertiesLoaderSupport和PropertyResourceConfigurer的关系，我们看见PropertiesLoaderSupport提供了properties文件的加载，在这里继承抽象类PropertiesLoaderSupport其实是达到与复用；
我们继承一个抽象类的原因有两个：
1）：与其它类功能方法之间的复用（比如这里的PropertiesLoaderSupport）；而不是从分类学上面属于一类，这样的抽象类属于工具类；这里的功能复用，有两种手段可以实现，一种是组合，一种是继承；
2）：抽象类中约定类流程，把算法的具体实现暴露给子类；
