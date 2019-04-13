
# Spring源码分析3 — spring XML配置文件的解析流程 - 谢杨易的博客 - CSDN博客

2017年07月13日 21:08:57[谢杨易](https://me.csdn.net/u013510838)阅读数：1660



# 1 介绍
创建并初始化spring容器中，关键一步就是读取并解析spring XML配置文件。这个过程比较复杂，本文将详细分析整个流程。先看涉及到的关键类。
XmlWebApplicationContext：web应用的默认Spring容器
XmlBeanDefinitionReader：读取XML并解析xml文件
DocumentLoader：文件先被读取为了原始的输入流InputStream，然后封装为InputSource。DocumentLoader加载inputSource，解析后得到Document对象
Document：代表一个XML或者HTML标记文件，包含docType，各种element节点等。
BeanDefinition：XML中bean在spring容器中的表示。Document会被解析为BeanDefinition。在Bean创建和初始化中它们会大展拳脚。
BeanDefinitionDocumentReader：解析Document中的节点元素Element，转换为BeanDefinition，并注册他们到BeanDefinition注册表中。默认实现类为DefaultBeanDefinitionDocumentReader
BeanDefinitionParserDelegate：实际解析Document中的节点元素，采用了代理模式。
# 2 流程
## 2.1 obtainFreshBeanFactory
初始化spring容器中的refresh()方法中，会调用obtainFreshBeanFactory()方法，它是读取并解析spring xml配置文件的入口。详细过程可以参看上一篇文章。下面从这个方法开始分析。
```python
// obtainFreshBeanFactory 加载spring XML配置文件
```
```python
protected
```
```python
ConfigurableListableBeanFactory
```
```python
obtainFreshBeanFactory
```
```python
() {
```
```python
// 关键所在，后面分析
```
```python
refreshBeanFactory();
```
```python
// log之类的东西，不是很关键了
```
```python
ConfigurableListableBeanFactory beanFactory = getBeanFactory();
```
```python
if
```
```python
(logger.isDebugEnabled()) {
      logger.debug(
```
```python
"Bean factory for "
```
```python
+ getDisplayName() +
```
```python
": "
```
```python
+ beanFactory);
   }
```
```python
return
```
```python
beanFactory;
}
```
```python
protected
```
```python
final
```
```python
void
```
```python
refreshBeanFactory
```
```python
()
```
```python
throws
```
```python
BeansException {
```
```python
// BeanFactory已存在，则先销毁它
```
```python
if
```
```python
(hasBeanFactory()) {
      destroyBeans();
      closeBeanFactory();
   }
```
```python
try
```
```python
{
```
```python
// new DefaultListableBeanFactory，创建容器，设置id，个性化配置等
```
```python
DefaultListableBeanFactory beanFactory = createBeanFactory();
      beanFactory.setSerializationId(getId());
      customizeBeanFactory(beanFactory);
```
```python
// 加载xml配置文件，具体子ApplicationContext会实现它。不同子类实现会不同。重点节点，后面分析
```
```python
loadBeanDefinitions(beanFactory);
```
```python
synchronized
```
```python
(
```
```python
this
```
```python
.beanFactoryMonitor) {
```
```python
this
```
```python
.beanFactory = beanFactory;
      }
   }
```
```python
catch
```
```python
(IOException ex) {
```
```python
throw
```
```python
new
```
```python
ApplicationContextException(
```
```python
"I/O error parsing bean definition source for "
```
```python
+ getDisplayName(), ex);
   }
}
```
## 2.2 loadBeanDefinitions
下面我们来分析web应用中默认的spring容器，也就是XmlWebApplicationContext，中的loadBeanDefinitions。
通过XmlBeanDefinitionReader来读取xml配置文件。
```python
protected
```
```python
void
```
```python
loadBeanDefinitions
```
```python
(DefaultListableBeanFactory beanFactory)
```
```python
throws
```
```python
BeansException, IOException {
```
```python
// 创建XmlBeanDefinitionReader，用它来读取XML配置文件
```
```python
XmlBeanDefinitionReader beanDefinitionReader =
```
```python
new
```
```python
XmlBeanDefinitionReader(beanFactory);
```
```python
// 配置beanDefinitionReader的环境和属性
```
```python
beanDefinitionReader.setEnvironment(getEnvironment());
```
```python
// ApplicationContext也继承了ResourceLoader接口
```
```python
beanDefinitionReader.setResourceLoader(
```
```python
this
```
```python
);
```
```python
// entityResolver在parse时会用到
```
```python
beanDefinitionReader.setEntityResolver(
```
```python
new
```
```python
ResourceEntityResolver(
```
```python
this
```
```python
));
```
```python
// 初始化beanDefinitionReader，子类可以实现这个方法，做一些个性化配置和初始化
```
```python
initBeanDefinitionReader(beanDefinitionReader);
```
```python
// 开始load xml文件，这一步开始才是真正读取XML文件了。前面都是做一些环境配置之类的事情
```
```python
loadBeanDefinitions(beanDefinitionReader);
}
```
loadBeanDefinitions()中先创建beanDefinitionReader，然后配置它的环境，设置成员属性，最后才是真正干活的，也就是读取XML配置文件。我们来看真正读取XML的这一步。
```python
protected
```
```python
void
```
```python
loadBeanDefinitions
```
```python
(XmlBeanDefinitionReader reader)
```
```python
throws
```
```python
IOException {
```
```python
// 获取XML配置文件的地址，还记得前面讲到过的web.xml中的contextConfigLocation元素吧，它指明了XML配置文件的地址。如果web.xml中没有配置，则读取默认的地址，参看后面分析
```
```python
String[] configLocations = getConfigLocations();
```
```python
if
```
```python
(configLocations !=
```
```python
null
```
```python
) {
```
```python
// 遍历读取每个配置文件
```
```python
for
```
```python
(String configLocation : configLocations) {
         reader.loadBeanDefinitions(configLocation);
      }
   }
}
```
## 2.3 getConfigLocations()
我们先分析下getConfigLocations方法, 它会获取到spring XML配置文件的地址。
```python
// 获取配置文件地址，从web.xml中读取。读取不到，则使用默认地址
```
```python
protected
```
```python
String[]
```
```python
getConfigLocations
```
```python
() {
```
```python
return
```
```python
(
```
```python
this
```
```python
.configLocations !=
```
```python
null
```
```python
?
```
```python
this
```
```python
.configLocations : getDefaultConfigLocations());
}
```
```python
// 获取默认的xml配置文件地址
```
```python
public
```
```python
static
```
```python
final
```
```python
String DEFAULT_CONFIG_LOCATION_PREFIX =
```
```python
"/WEB-INF/"
```
```python
;
```
```python
public
```
```python
static
```
```python
final
```
```python
String DEFAULT_CONFIG_LOCATION_SUFFIX =
```
```python
".xml"
```
```python
;
```
```python
public
```
```python
static
```
```python
final
```
```python
String DEFAULT_CONFIG_LOCATION =
```
```python
"/WEB-INF/applicationContext.xml"
```
```python
;
```
```python
protected
```
```python
String[]
```
```python
getDefaultConfigLocations
```
```python
() {
```
```python
if
```
```python
(getNamespace() !=
```
```python
null
```
```python
) {
```
```python
// 设置了容器namespace时，返回/WEB-INF/ + ApplicationContext的namespace + .xml
```
```python
return
```
```python
new
```
```python
String[] {DEFAULT_CONFIG_LOCATION_PREFIX + getNamespace() + DEFAULT_CONFIG_LOCATION_SUFFIX};
   }
```
```python
else
```
```python
{
```
```python
// 没有设置namespace时，直接返回 /WEB-INF/applicationContext.xml
```
```python
return
```
```python
new
```
```python
String[] {DEFAULT_CONFIG_LOCATION};
   }
}
```
我们先获取XML文件地址，然后再读取XML文件，下面重点分析reader如何读取xml文件的
## 2.4 XmlBeanDefinitionReader.loadBeanDefinitions()
```python
// XmlBeanDefinitionReader读取XML文件
```
```python
public
```
```python
int
```
```python
loadBeanDefinitions
```
```python
(EncodedResource encodedResource)
```
```python
throws
```
```python
BeanDefinitionStoreException {
```
```python
// 一段log，省略
```
```python
// 将Resource对象添加到hashSet中，不是很关键
```
```python
Set<EncodedResource> currentResources =
```
```python
this
```
```python
.resourcesCurrentlyBeingLoaded.get();
```
```python
if
```
```python
(currentResources ==
```
```python
null
```
```python
) {
      currentResources =
```
```python
new
```
```python
HashSet<>(
```
```python
4
```
```python
);
```
```python
this
```
```python
.resourcesCurrentlyBeingLoaded.set(currentResources);
   }
```
```python
if
```
```python
(!currentResources.add(encodedResource)) {
```
```python
throw
```
```python
new
```
```python
BeanDefinitionStoreException(
```
```python
"Detected cyclic loading of "
```
```python
+ encodedResource +
```
```python
" - check your import definitions!"
```
```python
);
   }
```
```python
try
```
```python
{
```
```python
// 获取Resource对象的输入流
```
```python
InputStream inputStream = encodedResource.getResource().getInputStream();
```
```python
try
```
```python
{
```
```python
// 将inputStream封装到InputSource对象中，并设置编码格式
```
```python
InputSource inputSource =
```
```python
new
```
```python
InputSource(inputStream);
```
```python
if
```
```python
(encodedResource.getEncoding() !=
```
```python
null
```
```python
) {
            inputSource.setEncoding(encodedResource.getEncoding());
         }
```
```python
// 加载封装好的inputSource对象，读取XML配置文件。关键步骤，后面分析
```
```python
return
```
```python
doLoadBeanDefinitions(inputSource, encodedResource.getResource());
      }
```
```python
finally
```
```python
{
         inputStream.close();
      }
   }
```
```python
catch
```
```python
(IOException ex) {
```
```python
// 异常处理
```
```python
throw
```
```python
new
```
```python
BeanDefinitionStoreException(
```
```python
"IOException parsing XML document from "
```
```python
+ encodedResource.getResource(), ex);
   }
```
```python
finally
```
```python
{
```
```python
// 资源释放
```
```python
currentResources.remove(encodedResource);
```
```python
if
```
```python
(currentResources.isEmpty()) {
```
```python
this
```
```python
.resourcesCurrentlyBeingLoaded.remove();
      }
   }
}
```
XmlBeanDefinitionReader做了一些成员变量设置后，获取传入的Resource对象的输入流，封装成InputSource后，开始真正解析配置文件。下面来看doLoadBeanDefinitions()如何解析XML文件。
```python
protected
```
```python
int
```
```python
doLoadBeanDefinitions
```
```python
(InputSource inputSource, Resource resource)
```
```python
throws
```
```python
BeanDefinitionStoreException {
```
```python
try
```
```python
{
```
```python
// 加载并解析XML文件，解析方案为社区通用方法，不是Spring所特有的
```
```python
Document doc = doLoadDocument(inputSource, resource);
```
```python
return
```
```python
registerBeanDefinitions(doc, resource);
   }
```
```python
// 各种异常处理，省略
```
```python
catch
```
```python
(BeanDefinitionStoreException ex) {
   }
}
```
先利用documentLoader加载XML配置文件，然后注册beans。
### 2.4.1 doLoadDocument 加载xml文件，将InputSource输入流转换为Document对象
```python
// 加载XML配置文件
```
```python
protected
```
```python
Document
```
```python
doLoadDocument
```
```python
(InputSource inputSource, Resource resource)
```
```python
throws
```
```python
Exception {
```
```python
// 获取entityResolver，errorHandler， validationMode，namespaceAware，它们都有默认值，也可以由用户来设置
```
```python
// entityResolver: 解析器，默认ResourceEntityResolver
```
```python
// errorHandler: 解析XML时错误处理，默认SimpleSaxErrorHandler
```
```python
// validationMode: xml验证模式，默认VALIDATION_XSD
```
```python
// namespaceAware: XML命名空间是否敏感，默认false
```
```python
return
```
```python
this
```
```python
.documentLoader.loadDocument(inputSource, getEntityResolver(),
```
```python
this
```
```python
.errorHandler,
         getValidationModeForResource(resource), isNamespaceAware());
}
```
```python
// DefaultDocumentLoader的loadDocument方法
```
```python
public
```
```python
Document
```
```python
loadDocument
```
```python
(InputSource inputSource, EntityResolver entityResolver,
            ErrorHandler errorHandler,
```
```python
int
```
```python
validationMode,
```
```python
boolean
```
```python
namespaceAware)
```
```python
throws
```
```python
Exception {
```
```python
// 创建DocumentBuilderFactory，对应多个实现类，默认com.sun.org.apache.xerces.internal.jaxp.DocumentBuilderFactoryImpl
```
```python
DocumentBuilderFactory factory = createDocumentBuilderFactory(validationMode, namespaceAware);
```
```python
if
```
```python
(logger.isDebugEnabled()) {
        logger.debug(
```
```python
"Using JAXP provider ["
```
```python
+ factory.getClass().getName() +
```
```python
"]"
```
```python
);
    }
```
```python
// 创建DocumentBuilder，通过factory创建
```
```python
DocumentBuilder builder = createDocumentBuilder(factory, entityResolver, errorHandler);
```
```python
// 解析输入流，并返回一个Document对象。解析采用的是通用的DocumentBuilderImpl对象，使用DomParser解析xml文件。解析这一步是通用的，不是Spring特有的方法。比较复杂，不展开了。只要知道通过parse后得到了Document对象就可以了。
```
```python
return
```
```python
builder.parse(inputSource);
}
```
### 2.4.2 registerBeanDefinitions 将读取XML后的Document转换为BeanDefinition
```python
public
```
```python
int
```
```python
registerBeanDefinitions
```
```python
(Document doc, Resource resource)
```
```python
throws
```
```python
BeanDefinitionStoreException {
```
```python
// 反射创建documentReader实例，默认为DefaultBeanDefinitionDocumentReader
```
```python
BeanDefinitionDocumentReader documentReader = createBeanDefinitionDocumentReader();
```
```python
// 获取容器中当前beans数量，已经注册的BeanDefinition会存储在一个Map中，获取Map的size即可。
```
```python
int
```
```python
countBefore = getRegistry().getBeanDefinitionCount();
```
```python
// 注册beanDefinition，这是关键所在，后面分析
```
```python
documentReader.registerBeanDefinitions(doc, createReaderContext(resource));
```
```python
// 返回本次注册的数量
```
```python
return
```
```python
getRegistry().getBeanDefinitionCount() - countBefore;
}
```
```python
// 反射创建documentReader，默认为DefaultBeanDefinitionDocumentReader
```
```python
private
```
```python
Class<?> documentReaderClass = DefaultBeanDefinitionDocumentReader.class;
```
```python
protected
```
```python
BeanDefinitionDocumentReader
```
```python
createBeanDefinitionDocumentReader
```
```python
() {
```
```python
return
```
```python
BeanDefinitionDocumentReader.class.cast(BeanUtils.instantiateClass(
```
```python
this
```
```python
.documentReaderClass));
    }
```
registerBeanDefinitions的作用就是将上一步中，输入流转换为的Document对象，转换为BeanDefinition对象。主要工作在documentReader.registerBeanDefinitions()中，下面来分析。
```python
public
```
```python
void
```
```python
registerBeanDefinitions
```
```python
(Document doc, XmlReaderContext readerContext) {
```
```python
this
```
```python
.readerContext = readerContext;
   logger.debug(
```
```python
"Loading bean definitions"
```
```python
);
```
```python
// root为<beans />标签
```
```python
Element root = doc.getDocumentElement();
   doRegisterBeanDefinitions(root);
}
```
```python
// 采用代理进行解析，代理为BeanDefinitionParserDelegate
```
```python
protected
```
```python
void
```
```python
doRegisterBeanDefinitions
```
```python
(Element root) {
```
```python
// 创建代理
```
```python
BeanDefinitionParserDelegate parent =
```
```python
this
```
```python
.delegate;
```
```python
this
```
```python
.delegate = createDelegate(getReaderContext(), root, parent);
```
```python
if
```
```python
(
```
```python
this
```
```python
.delegate.isDefaultNamespace(root)) {
            String profileSpec = root.getAttribute(PROFILE_ATTRIBUTE);
```
```python
if
```
```python
(StringUtils.hasText(profileSpec)) {
                String[] specifiedProfiles = StringUtils.tokenizeToStringArray(
                        profileSpec, BeanDefinitionParserDelegate.MULTI_VALUE_ATTRIBUTE_DELIMITERS);
```
```python
if
```
```python
(!getReaderContext().getEnvironment().acceptsProfiles(specifiedProfiles)) {
```
```python
return
```
```python
;
                }
            }
        }
```
```python
// 解析前的处理，DefaultBeanDefinitionDocumentReader没有实现它，子类可以实现，来扩展功能
```
```python
preProcessXml(root);
```
```python
// 解析root内的XML标签，如<import> <alias> <bean>等
```
```python
parseBeanDefinitions(root,
```
```python
this
```
```python
.delegate);
```
```python
// 解析后的处理，同样没有实现它，子类可以实现。
```
```python
postProcessXml(root);
```
```python
this
```
```python
.delegate = parent;
    }
```
registerBeanDefinitions() 解析下的XML标签，通过BeanDefinitionParserDelegate代理来进行。具体工作在parseBeanDefinitions()中。这里是本篇文章中比较关键的地方。
```python
protected
```
```python
void
```
```python
parseBeanDefinitions
```
```python
(Element root, BeanDefinitionParserDelegate delegate) {
```
```python
if
```
```python
(delegate.isDefaultNamespace(root)) {
```
```python
// 获取<beans>的子节点
```
```python
NodeList nl = root.getChildNodes();
```
```python
// 遍历子节点
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < nl.getLength(); i++) {
         Node node = nl.item(i);
```
```python
if
```
```python
(node
```
```python
instanceof
```
```python
Element) {
```
```python
// 子节点是Element对象，默认的子节点，如<import>都是Element对象
```
```python
Element ele = (Element) node;
```
```python
if
```
```python
(delegate.isDefaultNamespace(ele)) {
```
```python
// 在默认的命名空间url中的元素，是默认定义好的节点，采用parseDefaultElement方法解析
```
```python
parseDefaultElement(ele, delegate);
            }
```
```python
else
```
```python
{
```
```python
// 用户自定义的命名空间url中的元素，采用parseCustomElement方法解析
```
```python
delegate.parseCustomElement(ele);
            }
         }
      }
   }
```
```python
else
```
```python
{
```
```python
// 子节点不是标准的Element元素，比如用户自定义的，采用parseCustomElement方法解析
```
```python
delegate.parseCustomElement(root);
   }
}
```
parseBeanDefinitions()方法会循环遍历的子节点。如果是默认命名空间内的Element，则采用parseDefaultElement()方法解析，否则采用parseCustomElement()方法。DefaultElement包括、、、嵌套的，其余都为CustomElement，如。
#### 2.4.2.1 parseDefaultElement() 解析DefaultElement
```python
public
```
```python
static
```
```python
final
```
```python
String IMPORT_ELEMENT =
```
```python
"import"
```
```python
;
```
```python
public
```
```python
static
```
```python
final
```
```python
String ALIAS_ATTRIBUTE =
```
```python
"alias"
```
```python
;
```
```python
public
```
```python
static
```
```python
final
```
```python
String BEAN_ELEMENT =
```
```python
"bean"
```
```python
;
```
```python
public
```
```python
static
```
```python
final
```
```python
String NESTED_BEANS_ELEMENT =
```
```python
"beans"
```
```python
;
```
```python
private
```
```python
void
```
```python
parseDefaultElement
```
```python
(Element ele, BeanDefinitionParserDelegate delegate) {
```
```python
// 解析<import>
```
```python
if
```
```python
(delegate.nodeNameEquals(ele, IMPORT_ELEMENT)) {
      importBeanDefinitionResource(ele);
   }
```
```python
// 解析<alias>
```
```python
else
```
```python
if
```
```python
(delegate.nodeNameEquals(ele, ALIAS_ELEMENT)) {
      processAliasRegistration(ele);
   }
```
```python
// 解析<bean>
```
```python
else
```
```python
if
```
```python
(delegate.nodeNameEquals(ele, BEAN_ELEMENT)) {
      processBeanDefinition(ele, delegate);
   }
```
```python
// 解析<beans>
```
```python
else
```
```python
if
```
```python
(delegate.nodeNameEquals(ele, NESTED_BEANS_ELEMENT)) {
```
```python
// recurse
```
```python
doRegisterBeanDefinitions(ele);
   }
}
```
下面我们重点分析下processBeanDefinition(),因为这个和spring息息相关。这个过程十分复杂，所以我们不进行很细致的分析，抓住主要流程就OK了。也不建议非要去了解每行代码做了什么事情，避免过度陷入其中，而忽略了主流程。
```python
protected
```
```python
void
```
```python
processBeanDefinition
```
```python
(Element ele, BeanDefinitionParserDelegate delegate) {
```
```python
// 解析Element为BeanDefinition，这是重点，后面详细分析
```
```python
BeanDefinitionHolder bdHolder = delegate.parseBeanDefinitionElement(ele);
```
```python
if
```
```python
(bdHolder !=
```
```python
null
```
```python
) {
      bdHolder = delegate.decorateBeanDefinitionIfRequired(ele, bdHolder);
```
```python
try
```
```python
{
```
```python
// 将BeanDefinition注册到BeanDefinitionMap中，key为beanName
```
```python
BeanDefinitionReaderUtils.registerBeanDefinition(bdHolder, getReaderContext().getRegistry());
      }
```
```python
catch
```
```python
(BeanDefinitionStoreException ex) {
         getReaderContext().error(
```
```python
"Failed to register bean definition with name '"
```
```python
+
               bdHolder.getBeanName() +
```
```python
"'"
```
```python
, ele, ex);
      }
```
```python
// 发送注册的消息，相应的监听器就会收到并处理消息了
```
```python
getReaderContext().fireComponentRegistered(
```
```python
new
```
```python
BeanComponentDefinition(bdHolder));
   }
}
```
```python
public
```
```python
BeanDefinitionHolder
```
```python
parseBeanDefinitionElement
```
```python
(Element ele, @Nullable BeanDefinition containingBean) {
```
```python
// 获取bean的id属性
```
```python
String id = ele.getAttribute(ID_ATTRIBUTE);
```
```python
// 获取bean的name属性
```
```python
String nameAttr = ele.getAttribute(NAME_ATTRIBUTE);
```
```python
// 解析name属性，支持多个name
```
```python
List<String> aliases =
```
```python
new
```
```python
ArrayList<>();
```
```python
if
```
```python
(StringUtils.hasLength(nameAttr)) {
      String[] nameArr = StringUtils.tokenizeToStringArray(nameAttr, MULTI_VALUE_ATTRIBUTE_DELIMITERS);
      aliases.addAll(Arrays.asList(nameArr));
   }
```
```python
// id属性赋值到beanName变量中，注意不是name属性。如果没有id属性，则使用name属性的第一个值
```
```python
String beanName = id;
```
```python
if
```
```python
(!StringUtils.hasText(beanName) && !aliases.isEmpty()) {
      beanName = aliases.remove(
```
```python
0
```
```python
);
   }
```
```python
// 校验beanname的唯一性，这也是为啥id属性值必须唯一的原因
```
```python
if
```
```python
(containingBean ==
```
```python
null
```
```python
) {
      checkNameUniqueness(beanName, aliases, ele);
   }
```
```python
// 解析bean节点为GenericBeanDefinition，后面详细分析
```
```python
AbstractBeanDefinition beanDefinition = parseBeanDefinitionElement(ele, beanName, containingBean);
```
```python
// 后面不是很重要了
```
```python
if
```
```python
(beanDefinition !=
```
```python
null
```
```python
) {
```
```python
if
```
```python
(!StringUtils.hasText(beanName)) {
```
```python
try
```
```python
{
```
```python
if
```
```python
(containingBean !=
```
```python
null
```
```python
) {
               beanName = BeanDefinitionReaderUtils.generateBeanName(
                     beanDefinition,
```
```python
this
```
```python
.readerContext.getRegistry(),
```
```python
true
```
```python
);
            }
```
```python
else
```
```python
{
               beanName =
```
```python
this
```
```python
.readerContext.generateBeanName(beanDefinition);
```
```python
// Register an alias for the plain bean class name, if still possible,
```
```python
// if the generator returned the class name plus a suffix.
```
```python
// This is expected for Spring 1.2/2.0 backwards compatibility.
```
```python
String beanClassName = beanDefinition.getBeanClassName();
```
```python
if
```
```python
(beanClassName !=
```
```python
null
```
```python
&&
                     beanName.startsWith(beanClassName) && beanName.length() > beanClassName.length() &&
                     !
```
```python
this
```
```python
.readerContext.getRegistry().isBeanNameInUse(beanClassName)) {
                  aliases.add(beanClassName);
               }
            }
         }
```
```python
catch
```
```python
(Exception ex) {
            error(ex.getMessage(), ele);
```
```python
return
```
```python
null
```
```python
;
         }
      }
      String[] aliasesArray = StringUtils.toStringArray(aliases);
```
```python
return
```
```python
new
```
```python
BeanDefinitionHolder(beanDefinition, beanName, aliasesArray);
   }
```
```python
return
```
```python
null
```
```python
;
}
```
```python
public
```
```python
static
```
```python
final
```
```python
String PARENT_ATTRIBUTE =
```
```python
"parent"
```
```python
;
```
```python
public
```
```python
static
```
```python
final
```
```python
String CLASS_ATTRIBUTE =
```
```python
"class"
```
```python
;
```
```python
public
```
```python
AbstractBeanDefinition
```
```python
parseBeanDefinitionElement
```
```python
(Element ele, String beanName, @Nullable BeanDefinition containingBean) {
```
```python
this
```
```python
.parseState.push(
```
```python
new
```
```python
BeanEntry(beanName));
```
```python
// 获取class和parent属性
```
```python
String className =
```
```python
null
```
```python
;
```
```python
if
```
```python
(ele.hasAttribute(CLASS_ATTRIBUTE)) {
        className = ele.getAttribute(CLASS_ATTRIBUTE).trim();
    }
    String parent =
```
```python
null
```
```python
;
```
```python
if
```
```python
(ele.hasAttribute(PARENT_ATTRIBUTE)) {
        parent = ele.getAttribute(PARENT_ATTRIBUTE);
    }
```
```python
try
```
```python
{
```
```python
// 反射实例化bean为GenericBeanDefinition
```
```python
AbstractBeanDefinition bd = createBeanDefinition(className, parent);
```
```python
// 解析节点中其他属性，如scope， singleton等。后面详细分析
```
```python
parseBeanDefinitionAttributes(ele, beanName, containingBean, bd);
        bd.setDescription(DomUtils.getChildElementValueByTagName(ele, DESCRIPTION_ELEMENT));
```
```python
// 解析子节点meta属性,如果有的话
```
```python
parseMetaElements(ele, bd);
```
```python
// 解析子节点lookup-method属性
```
```python
parseLookupOverrideSubElements(ele, bd.getMethodOverrides());
```
```python
// 解析子节点replaced-method属性
```
```python
parseReplacedMethodSubElements(ele, bd.getMethodOverrides());
```
```python
// 解析constructor-arg属性
```
```python
parseConstructorArgElements(ele, bd);
```
```python
// 解析property属性
```
```python
parsePropertyElements(ele, bd);
```
```python
// 解析qualifier属性
```
```python
parseQualifierElements(ele, bd);
        bd.setResource(
```
```python
this
```
```python
.readerContext.getResource());
        bd.setSource(extractSource(ele));
```
```python
return
```
```python
bd;
    }
```
```python
// 各种异常处理
```
```python
catch
```
```python
(ClassNotFoundException ex) {
        error(
```
```python
"Bean class ["
```
```python
+ className +
```
```python
"] not found"
```
```python
, ele, ex);
    }
    ...
```
```python
return
```
```python
null
```
```python
;
}
```
```python
// 反射实例化，创建GenericBeanDefinition对象
```
```python
public
```
```python
static
```
```python
AbstractBeanDefinition
```
```python
createBeanDefinition
```
```python
(
      @Nullable String parentName, @Nullable String className, @Nullable ClassLoader classLoader)
```
```python
throws
```
```python
ClassNotFoundException {
   GenericBeanDefinition bd =
```
```python
new
```
```python
GenericBeanDefinition();
   bd.setParentName(parentName);
```
```python
if
```
```python
(className !=
```
```python
null
```
```python
) {
```
```python
if
```
```python
(classLoader !=
```
```python
null
```
```python
) {
         bd.setBeanClass(ClassUtils.forName(className, classLoader));
      }
```
```python
else
```
```python
{
         bd.setBeanClassName(className);
      }
   }
```
```python
return
```
```python
bd;
}
```
```python
// 解析<bean>中的各种属性，比如scope，lazy-init等
```
```python
public
```
```python
AbstractBeanDefinition
```
```python
parseBeanDefinitionAttributes
```
```python
(Element ele, String beanName,
      @Nullable BeanDefinition containingBean, AbstractBeanDefinition bd) {
```
```python
// 解析singleton
```
```python
if
```
```python
(ele.hasAttribute(SINGLETON_ATTRIBUTE)) {
      error(
```
```python
"Old 1.x 'singleton' attribute in use - upgrade to 'scope' declaration"
```
```python
, ele);
   }
```
```python
// 解析scope
```
```python
else
```
```python
if
```
```python
(ele.hasAttribute(SCOPE_ATTRIBUTE)) {
      bd.setScope(ele.getAttribute(SCOPE_ATTRIBUTE));
   }
```
```python
else
```
```python
if
```
```python
(containingBean !=
```
```python
null
```
```python
) {
```
```python
// Take default from containing bean in case of an inner bean definition.
```
```python
bd.setScope(containingBean.getScope());
   }
```
```python
// 解析abstract
```
```python
if
```
```python
(ele.hasAttribute(ABSTRACT_ATTRIBUTE)) {
      bd.setAbstract(TRUE_VALUE.equals(ele.getAttribute(ABSTRACT_ATTRIBUTE)));
   }
```
```python
// 解析lazy-init
```
```python
String lazyInit = ele.getAttribute(LAZY_INIT_ATTRIBUTE);
```
```python
if
```
```python
(DEFAULT_VALUE.equals(lazyInit)) {
      lazyInit =
```
```python
this
```
```python
.defaults.getLazyInit();
   }
   bd.setLazyInit(TRUE_VALUE.equals(lazyInit));
```
```python
// 解析autowire
```
```python
String autowire = ele.getAttribute(AUTOWIRE_ATTRIBUTE);
   bd.setAutowireMode(getAutowireMode(autowire));
```
```python
// 解析depends-on
```
```python
if
```
```python
(ele.hasAttribute(DEPENDS_ON_ATTRIBUTE)) {
      String dependsOn = ele.getAttribute(DEPENDS_ON_ATTRIBUTE);
      bd.setDependsOn(StringUtils.tokenizeToStringArray(dependsOn, MULTI_VALUE_ATTRIBUTE_DELIMITERS));
   }
```
```python
// 解析autowire-candidate
```
```python
String autowireCandidate = ele.getAttribute(AUTOWIRE_CANDIDATE_ATTRIBUTE);
```
```python
if
```
```python
(
```
```python
""
```
```python
.equals(autowireCandidate) || DEFAULT_VALUE.equals(autowireCandidate)) {
      String candidatePattern =
```
```python
this
```
```python
.defaults.getAutowireCandidates();
```
```python
if
```
```python
(candidatePattern !=
```
```python
null
```
```python
) {
         String[] patterns = StringUtils.commaDelimitedListToStringArray(candidatePattern);
         bd.setAutowireCandidate(PatternMatchUtils.simpleMatch(patterns, beanName));
      }
   }
```
```python
else
```
```python
{
      bd.setAutowireCandidate(TRUE_VALUE.equals(autowireCandidate));
   }
```
```python
// 解析primary
```
```python
if
```
```python
(ele.hasAttribute(PRIMARY_ATTRIBUTE)) {
      bd.setPrimary(TRUE_VALUE.equals(ele.getAttribute(PRIMARY_ATTRIBUTE)));
   }
```
```python
// 解析init-method
```
```python
if
```
```python
(ele.hasAttribute(INIT_METHOD_ATTRIBUTE)) {
      String initMethodName = ele.getAttribute(INIT_METHOD_ATTRIBUTE);
```
```python
if
```
```python
(!
```
```python
""
```
```python
.equals(initMethodName)) {
         bd.setInitMethodName(initMethodName);
      }
   }
```
```python
else
```
```python
if
```
```python
(
```
```python
this
```
```python
.defaults.getInitMethod() !=
```
```python
null
```
```python
) {
      bd.setInitMethodName(
```
```python
this
```
```python
.defaults.getInitMethod());
      bd.setEnforceInitMethod(
```
```python
false
```
```python
);
   }
```
```python
// 解析destroy-method
```
```python
if
```
```python
(ele.hasAttribute(DESTROY_METHOD_ATTRIBUTE)) {
      String destroyMethodName = ele.getAttribute(DESTROY_METHOD_ATTRIBUTE);
      bd.setDestroyMethodName(destroyMethodName);
   }
```
```python
else
```
```python
if
```
```python
(
```
```python
this
```
```python
.defaults.getDestroyMethod() !=
```
```python
null
```
```python
) {
      bd.setDestroyMethodName(
```
```python
this
```
```python
.defaults.getDestroyMethod());
      bd.setEnforceDestroyMethod(
```
```python
false
```
```python
);
   }
```
```python
// 解析factory-method
```
```python
if
```
```python
(ele.hasAttribute(FACTORY_METHOD_ATTRIBUTE)) {
      bd.setFactoryMethodName(ele.getAttribute(FACTORY_METHOD_ATTRIBUTE));
   }
```
```python
// 解析factory-bean
```
```python
if
```
```python
(ele.hasAttribute(FACTORY_BEAN_ATTRIBUTE)) {
      bd.setFactoryBeanName(ele.getAttribute(FACTORY_BEAN_ATTRIBUTE));
   }
```
```python
return
```
```python
bd;
}
```
#### 2.4.2.2 parseCustomElement() 解析CustomElement
```python
public
```
```python
BeanDefinition
```
```python
parseCustomElement
```
```python
(Element ele, @Nullable BeanDefinition containingBd) {
   String namespaceUri = getNamespaceURI(ele);
```
```python
if
```
```python
(namespaceUri ==
```
```python
null
```
```python
) {
```
```python
return
```
```python
null
```
```python
;
   }
```
```python
// 根据命名空间url获取具体的NamespaceHandler，比如<context:component-scan>对应的就是用户自定义的ContextNamespaceHandler。
```
```python
NamespaceHandler handler =
```
```python
this
```
```python
.readerContext.getNamespaceHandlerResolver().resolve(namespaceUri);
```
```python
if
```
```python
(handler ==
```
```python
null
```
```python
) {
      error(
```
```python
"Unable to locate Spring NamespaceHandler for XML schema namespace ["
```
```python
+ namespaceUri +
```
```python
"]"
```
```python
, ele);
```
```python
return
```
```python
null
```
```python
;
   }
```
```python
// 交给用户自定义的NamespaceHandler来解析用户自定义的CustomElement
```
```python
return
```
```python
handler.parse(ele,
```
```python
new
```
```python
ParserContext(
```
```python
this
```
```python
.readerContext,
```
```python
this
```
```python
, containingBd));
}
```
parseCustomElement()首先根据自定义标签的命名空间，生成具体的NamespaceHandler。一般要由用户自己定义。然后调用parse方法进行解析，这个也是用户自定义的。这里就不展开分析了。
# 3 流程图
![](https://img.alicdn.com/tfs/TB1F4QTSXXXXXXoXVXXXXXXXXXX-906-1230.png)

