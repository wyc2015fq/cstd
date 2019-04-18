# spring配置文件标签中使用${}占位符获得配置文件的属性值 - z69183787的专栏 - CSDN博客
2016年12月01日 16:29:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3650
一般情况下我们在Spring的配置文件中使用<import>标签是这样的，<import resource="spring-other.xml">，但是最近项目中使用到其他工程的依赖jar包，在自己的spring配置文件中需要这样写
<context:property-placeholder location="classpath:eoa/eoa_config.properties" />
<import resource="classpath:spring-fs-db-${env}.xml" />
其中env的值是从eoa_config.properties里面获取。
如果是以上这种写法，在启动时spring报错，无法解析env，原因很简单，在import动作时在属性文件加载之前。
没办法只能翻spring源码，ContextLoader
```java
protected void configureAndRefreshWebApplicationContext(ConfigurableWebApplicationContext wac, ServletContext sc) {
        if (ObjectUtils.identityToString(wac).equals(wac.getId())) {
// The application context id is still set to its original default value
// -> assign a more useful id based on available information
            String idParam = sc.getInitParameter(CONTEXT_ID_PARAM);
            if (idParam != null) {
                wac.setId(idParam);
            }
            else {
// Generate default id...
                wac.setId(ConfigurableWebApplicationContext.APPLICATION_CONTEXT_ID_PREFIX +
                        ObjectUtils.getDisplayString(sc.getContextPath()));
            }
        }
        wac.setServletContext(sc);
        String configLocationParam = sc.getInitParameter(CONFIG_LOCATION_PARAM);
        if (configLocationParam != null) {
            wac.setConfigLocation(configLocationParam);
        }
// The wac environment‘s #initPropertySources will be called in any case when the context
// is refreshed; do it eagerly here to ensure servlet property sources are in place for
// use in any post-processing or initialization that occurs below prior to #refresh
        ConfigurableEnvironment env = wac.getEnvironment();
        if (env instanceof ConfigurableWebEnvironment) {
            ((ConfigurableWebEnvironment) env).initPropertySources(sc, null);
        }
        customizeContext(sc, wac);
        wac.refresh();
    }
    //initPropertySources这个方法可以自己定义属性文件的加载时机。
    @Override
    public void initPropertySources(ServletContext servletContext, ServletConfig servletConfig) {
        WebApplicationContextUtils.initServletPropertySources(getPropertySources(), servletContext, servletConfig);
    }
```
~~也就是说我们需要在getPropertySources()方法调用之前，就已经做好属性文件的加载顺序 ？好像不是~~
需要在PropertyPlaceholderConfigurer 重载的后置处理器BeanFactoryPostProcessor（修改bean配置信息） 调用postProcessBeanFactory 前加载properties属性文件
spring提供了ApplicationContextInitializer这个接口，实现该接口
```java
public class CustomerApplicationContextInitializer implements ApplicationContextInitializer<ConfigurableApplicationContext>{
	private static Logger logger = LoggerFactory.getLogger(CustomerApplicationContextInitializer.class);
	@Override
	public void initialize(ConfigurableApplicationContext applicationContext) {
		ResourcePropertySource propertySource = null;
		try {
			propertySource = new ResourcePropertySource("classpath:eoa/eoa_config.properties");
		} catch (IOException e) {
			logger.error("eoa_config.properties is not exists");
		}
		applicationContext.getEnvironment().getPropertySources().addFirst(propertySource);
	}
}
```
此外，还需要在web.xml中配置这个类
```
<context-param>
	<param-name>contextInitializerClasses</param-name>
	<param-value>com.lfex.eoa.base.CustomerApplicationContextInitializer</param-value>
</context-param>
```
因为在customizeContext(sc, wac);会调用所有的contextInitializerClasses中的initialize方法。
则会在 PropertyPlaceholderConfigurer 继承的BeanFactoryPostProcessor 的postProcessBeanFactory 方法之前加载 properties文件
至此，问题解决了，总结一下分为以下几步
- 
提供实现ApplicationContextInitializer接口的实现类
- 
在web.xml中配置，见上
- 
在spring的主配置文件中使用<import resource="spring-${env}.xml">
