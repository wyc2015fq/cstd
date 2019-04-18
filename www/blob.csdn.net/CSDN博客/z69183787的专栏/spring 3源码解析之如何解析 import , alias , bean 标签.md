# spring 3源码解析之如何解析"import", "alias", "bean"标签 - z69183787的专栏 - CSDN博客
2016年12月02日 10:26:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：693
个人分类：[Spring-源码分析](https://blog.csdn.net/z69183787/article/category/6552286)

解析的步骤： 
**1、加载web.xml、加载监听器**
<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class> 
**2、ContextLoaderListener **
初始化initWebApplicationContext方法创建 
org.springframework.web.context.support. XmlWebApplicationContext对象 
**3、XmlWebApplicationContext**
调用loadBeanDefinitions方法，该方法主要做两件事情：初始化XmlBeanDefinitionReader、获取applicationContext.xml配置文件的路径、然后把事情交给XmlBeanDefinitionReader来处理 
**4、XmlBeanDefinitionReader**
获取到applicationContext.xml配置文件的路径、读取配置文件的内容得到一个输入流、对输入流转码操作、然后封装成一个inputSource对象、再然后封装成一个document对象；在生成document对象的同事也生成了一个Resource对象、这两个对象分部是：document对象承载配置文件的主要内容信息、Resource承载配置文件的描述信息以及一些验证信息。 
再由Resource对象创建一个XmlReaderContext。完成了以上操作XmlBeanDefinitionReader就把document对象和XmlReaderContext对象交给DefaultBeanDefinitionDocumentReader来处理 
**5、DefaultBeanDefinitionDocumentReader**
1）、对XmlReaderContext装饰成一个BeanDefinitionParserDelegate对象； 
2）、迭代document对象、把document对象拆分成Element元素逐个逐个解析； 
3）、使用BeanDefinitionParserDelegate装饰对象解析Element元素或者说标签。 
这里的Element元素有两种：一种是DefaultElement、另一种是CustomElement；DefaultElement包括alias、import、bean，CustomElement包括DefaultElement以外的所有元素 
我们所关心的应该是第一中元素DefaultElement，看下DefaultBeanDefinitionDocumentReader解析该元素的方法： 
Java代码  ![收藏代码](http://goodscript.iteye.com/images/icon_star.png)
- privatevoid parseDefaultElement(Element ele, BeanDefinitionParserDelegate delegate) {  
- if (delegate.nodeNameEquals(ele, IMPORT_ELEMENT)) {  
-             importBeanDefinitionResource(ele);  
-         }  
- elseif (delegate.nodeNameEquals(ele, ALIAS_ELEMENT)) {  
-             processAliasRegistration(ele);  
-         }  
- elseif (delegate.nodeNameEquals(ele, BEAN_ELEMENT)) {  
-             processBeanDefinition(ele, delegate);  
-         }  
-     }  
解析import标签的方法： 
Java代码  ![收藏代码](http://goodscript.iteye.com/images/icon_star.png)
- protectedvoid importBeanDefinitionResource(Element ele) {  
-         String location = ele.getAttribute(RESOURCE_ATTRIBUTE);  
- if (!StringUtils.hasText(location)) {  
-             getReaderContext().error("Resource location must not be empty", ele);  
- return;  
-         }  
- 
- // Resolve system properties: e.g. "${user.dir}"
-         location = SystemPropertyUtils.resolvePlaceholders(location);  
- 
-         Set<Resource> actualResources = new LinkedHashSet<Resource>(4);  
- 
- // Discover whether the location is an absolute or relative URI 
- boolean absoluteLocation = false;  
- try {  
-             absoluteLocation = ResourcePatternUtils.isUrl(location) || ResourceUtils.toURI(location).isAbsolute();  
-         }  
- catch (URISyntaxException ex) {  
- // cannot convert to an URI, considering the location relative
- // unless it is the well-known Spring prefix "classpath*:"
-         }  
- 
- // Absolute or relative?
- if (absoluteLocation) {  
- try {  
- int importCount = getReaderContext().getReader().loadBeanDefinitions(location, actualResources);  
- if (logger.isDebugEnabled()) {  
-                     logger.debug("Imported " + importCount + " bean definitions from URL location [" + location + "]");  
-                 }  
-             }  
- catch (BeanDefinitionStoreException ex) {  
-                 getReaderContext().error(  
- "Failed to import bean definitions from URL location [" + location + "]", ele, ex);  
-             }  
-         }  
- else {  
- // No URL -> considering resource location as relative to the current file.
- try {  
- int importCount;  
-                 Resource relativeResource = getReaderContext().getResource().createRelative(location);  
- if (relativeResource.exists()) {  
-                     importCount = getReaderContext().getReader().loadBeanDefinitions(relativeResource);  
-                     actualResources.add(relativeResource);  
-                 }  
- else {  
-                     String baseLocation = getReaderContext().getResource().getURL().toString();  
-                     importCount = getReaderContext().getReader().loadBeanDefinitions(  
-                             StringUtils.applyRelativePath(baseLocation, location), actualResources);  
-                 }  
- if (logger.isDebugEnabled()) {  
-                     logger.debug("Imported " + importCount + " bean definitions from relative location [" + location + "]");  
-                 }  
-             }  
- catch (IOException ex) {  
-                 getReaderContext().error("Failed to resolve current resource location", ele, ex);  
-             }  
- catch (BeanDefinitionStoreException ex) {  
-                 getReaderContext().error("Failed to import bean definitions from relative location [" + location + "]",  
-                         ele, ex);  
-             }  
-         }  
-         Resource[] actResArray = actualResources.toArray(new Resource[actualResources.size()]);  
-         getReaderContext().fireImportProcessed(location, actResArray, extractSource(ele));  
-     }  
解析alias标签的方法： 
Java代码  ![收藏代码](http://goodscript.iteye.com/images/icon_star.png)
- protectedvoid processAliasRegistration(Element ele) {  
-     String name = ele.getAttribute(NAME_ATTRIBUTE);  
-     String alias = ele.getAttribute(ALIAS_ATTRIBUTE);  
- boolean valid = true;  
- if (!StringUtils.hasText(name)) {  
-         getReaderContext().error("Name must not be empty", ele);  
-         valid = false;  
-     }  
- if (!StringUtils.hasText(alias)) {  
-         getReaderContext().error("Alias must not be empty", ele);  
-         valid = false;  
-     }  
- if (valid) {  
- try {  
-             getReaderContext().getRegistry().registerAlias(name, alias);  
-         }  
- catch (Exception ex) {  
-             getReaderContext().error("Failed to register alias '" + alias +  
- "' for bean with name '" + name + "'", ele, ex);  
-         }  
-         getReaderContext().fireAliasRegistered(name, alias, extractSource(ele));  
-     }  
- }  
解析bean标签的方法： 
Java代码  ![收藏代码](http://goodscript.iteye.com/images/icon_star.png)
- protectedvoid processBeanDefinition(Element ele, BeanDefinitionParserDelegate delegate) {  
-     BeanDefinitionHolder bdHolder = delegate.parseBeanDefinitionElement(ele);  
- if (bdHolder != null) {  
-         bdHolder = delegate.decorateBeanDefinitionIfRequired(ele, bdHolder);  
- try {  
- // Register the final decorated instance.
-             BeanDefinitionReaderUtils.registerBeanDefinition(bdHolder, getReaderContext().getRegistry());  
-         }  
- catch (BeanDefinitionStoreException ex) {  
-             getReaderContext().error("Failed to register bean definition with name '" +  
-                     bdHolder.getBeanName() + "'", ele, ex);  
-         }  
- // Send registration event.
-         getReaderContext().fireComponentRegistered(new BeanComponentDefinition(bdHolder));  
-     }  
- }  
方法调用关系图： 
![](http://dl.iteye.com/upload/attachment/542853/6eb5e70d-3cb6-3d83-accc-39d30b4d8e4b.jpg)
