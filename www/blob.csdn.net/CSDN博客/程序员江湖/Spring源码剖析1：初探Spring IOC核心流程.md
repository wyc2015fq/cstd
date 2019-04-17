# Spring源码剖析1：初探Spring IOC核心流程 - 程序员江湖 - CSDN博客





2017年05月24日 23:13:28[黄小斜](https://me.csdn.net/a724888)阅读数：1292标签：[Spring																[IOC](https://so.csdn.net/so/search/s.do?q=IOC&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/a724888/article/category/6797661)

所属专栏：[Spring与SpringMVC源码解析](https://blog.csdn.net/column/details/21851.html)










本文大致地介绍了IOC容器的初始化过程，只列出了比较重要的过程和代码，可以从中看出IOC容器执行的大致流程。

接下来的文章会更加深入剖析Bean容器如何解析xml，注册和初始化bean，以及如何获取bean实例等详细的过程。




转自：http://www.importnew.com/19243.html




1. 初始化




大致单步跟了下Spring IOC的初始化过程，整个脉络很庞大，初始化的过程主要就是读取XML资源，并解析，最终注册到Bean Factory中：

![](https://cloud.githubusercontent.com/assets/1736354/7897341/032179be-070b-11e5-9ecf-d7befc804e9d.png)

在完成初始化的过程后，Bean们就在BeanFactory中蓄势以待地等调用了。下面通过一个具体的例子，来详细地学习一下初始化过程，例如当加载下面一个bean：



```
```java
<bean id=
```

```java
"XiaoWang"
```

```java
class
```

```java
=
```

```java
"com.springstudy.talentshow.SuperInstrumentalist"
```

```java
>
```



```java

```

```java
<property name=
```

```java
"instruments"
```

```java
>
```



```java

```

```java
<list>
```



```java

```

```java
<ref bean=
```

```java
"piano"
```

```java
/>
```



```java

```

```java
<ref bean=
```

```java
"saxophone"
```

```java
/>
```



```java

```

```java
</list>
```



```java

```

```java
</property>
```



```java
</bean>
```
```



加载时需要读取、解析、注册bean，这个过程具体的调用栈如下所示：
![](https://cloud.githubusercontent.com/assets/1736354/7896285/8a488060-06e6-11e5-9ad9-4ddd3375984f.png)

下面对每一步的关键的代码进行详细分析：

#### 1.1 准备

保存配置位置，并刷新
在调用ClassPathXmlApplicationContext后，先会将配置位置信息保存到configLocations，供后面解析使用，之后，会调用`AbstractApplicationContext`的refresh方法进行刷新：



```
```java
public
```

```java
ClassPathXmlApplicationContext(String[] configLocations,
```

```java
boolean
```

```java
refresh,
```



```java

```

```java
ApplicationContext parent)
```

```java
throws
```

```java
BeansException {
```





```java

```

```java
super
```

```java
(parent);
```



```java

```

```java
// 保存位置信息，比如`com/springstudy/talentshow/talent-show.xml`
```



```java

```

```java
setConfigLocations(configLocations);
```



```java

```

```java
if
```

```java
(refresh) {
```



```java

```

```java
// 刷新
```



```java

```

```java
refresh();
```



```java

```

```java
}
```



```java
}
```





```java
public
```

```java
void
```

```java
refresh()
```

```java
throws
```

```java
BeansException, IllegalStateException {
```



```java

```

```java
synchronized
```

```java
(
```

```java
this
```

```java
.startupShutdownMonitor) {
```



```java

```

```java
// Prepare this context for refreshing.
```



```java

```

```java
prepareRefresh();
```



```java

```

```java
// Tell the subclass to refresh the internal bean factory.
```



```java

```

```java
ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();
```



```java

```

```java
// Prepare the bean factory for use in this context.
```



```java

```

```java
prepareBeanFactory(beanFactory);
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
// Allows post-processing of the bean factory in context subclasses.
```



```java

```

```java
postProcessBeanFactory(beanFactory);
```



```java

```

```java
// Invoke factory processors registered as beans in the context.
```



```java

```

```java
invokeBeanFactoryPostProcessors(beanFactory);
```



```java

```

```java
// Register bean processors that intercept bean creation.
```



```java

```

```java
registerBeanPostProcessors(beanFactory);
```



```java

```

```java
// Initialize message source for this context.
```



```java

```

```java
initMessageSource();
```



```java

```

```java
// Initialize event multicaster for this context.
```



```java

```

```java
initApplicationEventMulticaster();
```



```java

```

```java
// Initialize other special beans in specific context subclasses.
```



```java

```

```java
onRefresh();
```



```java

```

```java
// Check for listener beans and register them.
```



```java

```

```java
registerListeners();
```



```java

```

```java
// Instantiate all remaining (non-lazy-init) singletons.
```



```java

```

```java
finishBeanFactoryInitialization(beanFactory);
```



```java

```

```java
// Last step: publish corresponding event.
```



```java

```

```java
finishRefresh();
```



```java

```

```java
}
```



```java

```

```java
catch
```

```java
(BeansException ex) {
```



```java

```

```java
// Destroy already created singletons to avoid dangling resources.
```



```java

```

```java
destroyBeans();
```



```java

```

```java
// Reset 'active' flag.
```



```java

```

```java
cancelRefresh(ex);
```



```java

```

```java
// Propagate exception to caller.
```



```java

```

```java
throw
```

```java
ex;
```



```java

```

```java
}
```



```java

```

```java
}
```



```java
}
```
```



创建载入BeanFactory



```
```java
protected
```

```java
final
```

```java
void
```

```java
refreshBeanFactory()
```

```java
throws
```

```java
BeansException {
```



```java

```

```java
// ... ...
```



```java

```

```java
DefaultListableBeanFactory beanFactory = createBeanFactory();
```



```java

```

```java
// ... ...
```



```java

```

```java
loadBeanDefinitions(beanFactory);
```



```java

```

```java
// ... ...
```



```java
}
```
```



创建XMLBeanDefinitionReader



```
```java
protected
```

```java
void
```

```java
loadBeanDefinitions(DefaultListableBeanFactory beanFactory)
```



```java

```

```java
throws
```

```java
BeansException, IOException {
```



```java

```

```java
// Create a new XmlBeanDefinitionReader for the given BeanFactory.
```



```java

```

```java
XmlBeanDefinitionReader beanDefinitionReader =
```

```java
new
```

```java
XmlBeanDefinitionReader(beanFactory);
```



```java

```

```java
// ... ...
```



```java

```

```java
// Allow a subclass to provide custom initialization of the reader,
```



```java

```

```java
// then proceed with actually loading the bean definitions.
```



```java

```

```java
initBeanDefinitionReader(beanDefinitionReader);
```



```java

```

```java
loadBeanDefinitions(beanDefinitionReader);
```



```java
}
```
```



#### 1.2 读取

创建处理每一个resource



```
```java
public
```

```java
int
```

```java
loadBeanDefinitions(String location, Set<Resource> actualResources)
```



```java

```

```java
throws
```

```java
BeanDefinitionStoreException {
```



```java

```

```java
// ... ...
```



```java

```

```java
// 通过Location来读取Resource
```



```java

```

```java
Resource[] resources = ((ResourcePatternResolver) resourceLoader).getResources(location);
```



```java

```

```java
int
```

```java
loadCount = loadBeanDefinitions(resources);
```



```java

```

```java
// ... ...
```



```java
}
```





```java
public
```

```java
int
```

```java
loadBeanDefinitions(Resource... resources)
```

```java
throws
```

```java
BeanDefinitionStoreException {
```



```java

```

```java
Assert.notNull(resources,
```

```java
"Resource array must not be null"
```

```java
);
```



```java

```

```java
int
```

```java
counter =
```

```java
0
```

```java
;
```



```java

```

```java
for
```

```java
(Resource resource : resources) {
```



```java

```

```java
// 载入每一个resource
```



```java

```

```java
counter += loadBeanDefinitions(resource);
```



```java

```

```java
}
```



```java

```

```java
return
```

```java
counter;
```



```java
}
```
```



处理XML每个元素



```
```java
protected
```

```java
void
```

```java
parseBeanDefinitions(Element root, BeanDefinitionParserDelegate delegate) {
```



```java

```

```java
// ... ...
```



```java

```

```java
NodeList nl = root.getChildNodes();
```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
; i < nl.getLength(); i++) {
```



```java

```

```java
Node node = nl.item(i);
```



```java

```

```java
if
```

```java
(node
```

```java
instanceof
```

```java
Element) {
```



```java

```

```java
Element ele = (Element) node;
```



```java

```

```java
if
```

```java
(delegate.isDefaultNamespace(ele)) {
```



```java

```

```java
// 处理每个xml中的元素，可能是import、alias、bean
```



```java

```

```java
parseDefaultElement(ele, delegate);
```



```java

```

```java
}
```



```java

```

```java
else
```

```java
{
```



```java

```

```java
delegate.parseCustomElement(ele);
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
// ... ...
```



```java
}
```
```



解析和注册bean



```
```java
protected
```

```java
void
```

```java
processBeanDefinition(Element ele, BeanDefinitionParserDelegate delegate) {
```



```java

```

```java
// 解析
```



```java

```

```java
BeanDefinitionHolder bdHolder = delegate.parseBeanDefinitionElement(ele);
```



```java

```

```java
if
```

```java
(bdHolder !=
```

```java
null
```

```java
) {
```



```java

```

```java
bdHolder = delegate.decorateBeanDefinitionIfRequired(ele, bdHolder);
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
// 注册
```



```java

```

```java
// Register the final decorated instance.
```



```java

```

```java
BeanDefinitionReaderUtils.registerBeanDefinition(
```



```java

```

```java
bdHolder, getReaderContext().getRegistry());
```



```java

```

```java
}
```



```java

```

```java
catch
```

```java
(BeanDefinitionStoreException ex) {
```



```java

```

```java
getReaderContext().error(
```

```java
"Failed to register bean definition with name '"
```

```java
+
```



```java

```

```java
bdHolder.getBeanName() +
```

```java
"'"
```

```java
, ele, ex);
```



```java

```

```java
}
```



```java

```

```java
// Send registration event.
```



```java

```

```java
getReaderContext().fireComponentRegistered(
```

```java
new
```

```java
BeanComponentDefinition(bdHolder));
```



```java

```

```java
}
```



```java
}
```
```



本步骤中，通过`parseBeanDefinitionElement`将XML的元素解析为`BeanDefinition`，然后存在`BeanDefinitionHolder`中，然后再利用`BeanDefinitionHolder`将`BeanDefinition`注册，实质就是把`BeanDefinition`的实例put进`BeanFactory`中，和后面将详细的介绍解析和注册过程。

#### 1.3 解析

![](https://cloud.githubusercontent.com/assets/1736354/7896302/eae02bc6-06e6-11e5-941a-d1f59e3b363f.png)

处理每个Bean的元素



```
```java
public
```

```java
AbstractBeanDefinition parseBeanDefinitionElement(
```



```java

```

```java
Element ele, String beanName, BeanDefinition containingBean) {
```





```java

```

```java
// ... ...
```



```java

```

```java
// 创建beandefinition
```



```java

```

```java
AbstractBeanDefinition bd = createBeanDefinition(className, parent);
```





```java

```

```java
parseBeanDefinitionAttributes(ele, beanName, containingBean, bd);
```



```java

```

```java
bd.setDescription(DomUtils.getChildElementValueByTagName(ele, DESCRIPTION_ELEMENT));
```





```java

```

```java
parseMetaElements(ele, bd);
```



```java

```

```java
parseLookupOverrideSubElements(ele, bd.getMethodOverrides());
```



```java

```

```java
parseReplacedMethodSubElements(ele, bd.getMethodOverrides());
```



```java

```

```java
// 处理“Constructor”
```



```java

```

```java
parseConstructorArgElements(ele, bd);
```



```java

```

```java
// 处理“Preperty”
```



```java

```

```java
parsePropertyElements(ele, bd);
```



```java

```

```java
parseQualifierElements(ele, bd);
```



```java

```

```java
// ... ...
```



```java
}
```
```



处理属性的值



```
```java
public
```

```java
Object parsePropertyValue(Element ele, BeanDefinition bd, String propertyName) {
```



```java

```

```java
String elementName = (propertyName !=
```

```java
null
```

```java
) ?
```



```java

```

```java
"<property> element for property '"
```

```java
+ propertyName +
```

```java
"'"
```

```java
:
```



```java

```

```java
"<constructor-arg> element"
```

```java
;
```





```java

```

```java
// ... ...
```



```java

```

```java
if
```

```java
(hasRefAttribute) {
```



```java

```

```java
// 处理引用
```



```java

```

```java
String refName = ele.getAttribute(REF_ATTRIBUTE);
```



```java

```

```java
if
```

```java
(!StringUtils.hasText(refName)) {
```



```java

```

```java
error(elementName +
```

```java
" contains empty 'ref' attribute"
```

```java
, ele);
```



```java

```

```java
}
```



```java

```

```java
RuntimeBeanReference ref =
```

```java
new
```

```java
RuntimeBeanReference(refName);
```



```java

```

```java
ref.setSource(extractSource(ele));
```



```java

```

```java
return
```

```java
ref;
```



```java

```

```java
}
```



```java

```

```java
else
```

```java
if
```

```java
(hasValueAttribute) {
```



```java

```

```java
// 处理值
```



```java

```

```java
TypedStringValue valueHolder =
```

```java
new
```

```java
TypedStringValue(ele.getAttribute(VALUE_ATTRIBUTE));
```



```java

```

```java
valueHolder.setSource(extractSource(ele));
```



```java

```

```java
return
```

```java
valueHolder;
```



```java

```

```java
}
```



```java

```

```java
else
```

```java
if
```

```java
(subElement !=
```

```java
null
```

```java
) {
```



```java

```

```java
// 处理子类型（比如list、map等）
```



```java

```

```java
return
```

```java
parsePropertySubElement(subElement, bd);
```



```java

```

```java
}
```



```java

```

```java
// ... ...
```



```java
}
```
```



#### 1.4 注册



```
```java
public
```

```java
static
```

```java
void
```

```java
registerBeanDefinition(
```



```java

```

```java
BeanDefinitionHolder definitionHolder, BeanDefinitionRegistry registry)
```



```java

```

```java
throws
```

```java
BeanDefinitionStoreException {
```





```java

```

```java
// Register bean definition under primary name.
```



```java

```

```java
String beanName = definitionHolder.getBeanName();
```



```java

```

```java
registry.registerBeanDefinition(beanName, definitionHolder.getBeanDefinition());
```





```java

```

```java
// Register aliases for bean name, if any.
```



```java

```

```java
String[] aliases = definitionHolder.getAliases();
```



```java

```

```java
if
```

```java
(aliases !=
```

```java
null
```

```java
) {
```



```java

```

```java
for
```

```java
(String alias : aliases) {
```



```java

```

```java
registry.registerAlias(beanName, alias);
```



```java

```

```java
}
```



```java

```

```java
}
```



```java
}
```





```java
public
```

```java
void
```

```java
registerBeanDefinition(String beanName, BeanDefinition beanDefinition)
```



```java

```

```java
throws
```

```java
BeanDefinitionStoreException {
```





```java

```

```java
// ......
```





```java

```

```java
// 将beanDefinition注册
```



```java

```

```java
this
```

```java
.beanDefinitionMap.put(beanName, beanDefinition);
```





```java

```

```java
// ......
```



```java
}
```
```



注册过程中，最核心的一句就是：`this.beanDefinitionMap.put(beanName, beanDefinition)`，也就是说注册的实质就是以beanName为key，以beanDefinition为value，将其put到HashMap中。

### 2. 注入依赖

当完成初始化IOC容器后，如果bean没有设置lazy-init(延迟加载)属性，那么bean的实例就会在初始化IOC完成之后，及时地进行初始化。初始化时会先建立实例，然后根据配置利用反射对实例进行进一步操作，具体流程如下所示：
![](https://cloud.githubusercontent.com/assets/1736354/7929429/615570ea-0930-11e5-8097-ae982ef7709d.png)

创建bean的实例
创建bean的实例过程函数调用栈如下所示：
![](https://cloud.githubusercontent.com/assets/1736354/7929379/cec01bcc-092f-11e5-81ad-88c285f33845.png)

注入bean的属性
注入bean的属性过程函数调用栈如下所示：
![](https://cloud.githubusercontent.com/assets/1736354/7929381/db58350e-092f-11e5-82a4-caaf349291ea.png)

在创建bean和注入bean的属性时，都是在doCreateBean函数中进行的，我们重点看下：



```
```java
protected
```

```java
Object doCreateBean(
```

```java
final
```

```java
String beanName,
```

```java
final
```

```java
RootBeanDefinition mbd,
```



```java

```

```java
final
```

```java
Object[] args) {
```



```java

```

```java
// Instantiate the bean.
```



```java

```

```java
BeanWrapper instanceWrapper =
```

```java
null
```

```java
;
```



```java

```

```java
if
```

```java
(mbd.isSingleton()) {
```



```java

```

```java
instanceWrapper =
```

```java
this
```

```java
.factoryBeanInstanceCache.remove(beanName);
```



```java

```

```java
}
```



```java

```

```java
if
```

```java
(instanceWrapper ==
```

```java
null
```

```java
) {
```



```java

```

```java
// 创建bean的实例
```



```java

```

```java
instanceWrapper = createBeanInstance(beanName, mbd, args);
```



```java

```

```java
}
```





```java

```

```java
// ... ...
```





```java

```

```java
// Initialize the bean instance.
```



```java

```

```java
Object exposedObject = bean;
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
// 初始化bean的实例，如注入属性
```



```java

```

```java
populateBean(beanName, mbd, instanceWrapper);
```



```java

```

```java
if
```

```java
(exposedObject !=
```

```java
null
```

```java
) {
```



```java

```

```java
exposedObject = initializeBean(beanName, exposedObject, mbd);
```



```java

```

```java
}
```



```java

```

```java
}
```





```java

```

```java
// ... ...
```



```java
}
```
```



理解了以上两个过程，我们就可以自己实现一个简单的Spring框架了。于是，我根据自己的理解实现了一个简单的IOC框架[Simple Spring](https://github.com/Yikun/simple-spring)，有兴趣可以看看。





[](http://www.jiathis.com/share?uid=1745061)





