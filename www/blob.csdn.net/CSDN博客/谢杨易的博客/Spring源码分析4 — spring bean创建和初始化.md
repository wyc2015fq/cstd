
# Spring源码分析4 — spring bean创建和初始化 - 谢杨易的博客 - CSDN博客

2017年07月14日 17:23:08[谢杨易](https://me.csdn.net/u013510838)阅读数：1947标签：[spring																](https://so.csdn.net/so/search/s.do?q=spring&t=blog)[源码																](https://so.csdn.net/so/search/s.do?q=源码&t=blog)[bean																](https://so.csdn.net/so/search/s.do?q=bean&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=源码&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=spring&t=blog)个人分类：[Java																](https://blog.csdn.net/u013510838/article/category/7025289)
[
																								](https://so.csdn.net/so/search/s.do?q=spring&t=blog)



# 1 介绍
创建并初始化spring容器中，refresh()方法中解析xml配置文件，注册容器后处理器，bean后处理器，初始化MessageSource，ApplicationEventMulticaster广播器，注册完ApplicationListener监听器后，关键一步就是创建和初始化其他非lazy-init的singleton beans。这样在容器初始化好的时候，这些singleton beans就已经创建和初始化好了，可以大大提高bean的访问效率。这个过程比较复杂，本文将详细分析整个流程。先看涉及到的关键类。
AbstractApplicationContext: 定义了spring容器初始化的大部分流程方法，子类必须遵循这个流程，但可以修改流程中的方法，典型的模板模式。bean创建的入口方法finishBeanFactoryInitialization也在这个方法中。
DefaultListableBeanFactory：一种BeanFactory容器实现，实现了ConfigurableListableBeanFactory接口
BeanDefinition：描述bean结构，对应XML中的或者注解中的@Component
AbstractBeanFactory：继承了BeanFactory容器，主要负责getBean创建Bean实例。
# 2 流程
## 2.1 finishBeanFactoryInitialization
初始化spring容器中的refresh()方法中，会调用finishBeanFactoryInitialization()方法，它是创建和初始化其他非lazy-init的singleton的bean的入口。下面从这个方法开始分析。
```python
protected
```
```python
void
```
```python
finishBeanFactoryInitialization(ConfigurableListableBeanFactory beanFactory) {
```
```python
// 初始化conversionService类型转换bean，它可以服务于其他bean的类型转换
```
```python
if
```
```python
(beanFactory.containsBean(CONVERSION_SERVICE_BEAN_NAME) &&
         beanFactory.isTypeMatch(CONVERSION_SERVICE_BEAN_NAME, ConversionService.
```
```python
class
```
```python
)) {
      beanFactory.setConversionService(
            beanFactory.getBean(CONVERSION_SERVICE_BEAN_NAME, ConversionService.
```
```python
class
```
```python
));
   }
```
```python
// 注册字符串解析器，用来解析注解中的属性
```
```python
if
```
```python
(!beanFactory.hasEmbeddedValueResolver()) {
      beanFactory.addEmbeddedValueResolver(strVal -> getEnvironment().resolvePlaceholders(strVal));
   }
```
```python
// 初始化LoadTimeWeaverAware bean
```
```python
String[] weaverAwareNames = beanFactory.getBeanNamesForType(LoadTimeWeaverAware.
```
```python
class
```
```python
,
```
```python
false
```
```python
,
```
```python
false
```
```python
);
```
```python
for
```
```python
(String weaverAwareName : weaverAwareNames) {
      getBean(weaverAwareName);
   }
```
```python
// 停止使用临时的ClassLoader，
```
```python
beanFactory.setTempClassLoader(
```
```python
null
```
```python
);
   beanFactory.freezeConfiguration();
```
```python
// 这儿才是最关键的一步，创建和初始化非lazy-init的singleton beans
```
```python
beanFactory.preInstantiateSingletons();
}
```
finishBeanFactoryInitialization()做了初始化conversionService类型转换器等的工作，这些不是关键点。关键点在preInstantiateSingletons()方法中，它会做创建和初始化singleton bean的工作。下面接着分析
```python
public
```
```python
void
```
```python
preInstantiateSingletons
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
// 获取XML配置文件解析时，解析到的所有beanname
```
```python
List<String> beanNames =
```
```python
new
```
```python
ArrayList<>(
```
```python
this
```
```python
.beanDefinitionNames);
```
```python
// 遍历所有没有标注lazy-init的singleton的beanname,创建bean
```
```python
for
```
```python
(String beanName : beanNames) {
```
```python
// 利用beanname获取BeanDefinition，在XML解析时会生成BeanDefinition对象，将XML中的各属性添加到BeanDefinition的相关标志位中，比如abstractFlag，scope等
```
```python
RootBeanDefinition bd = getMergedLocalBeanDefinition(beanName);
```
```python
// 非abstract，非lazy-init的singleton bean才需要在容器初始化阶段创建
```
```python
if
```
```python
(!bd.isAbstract() && bd.isSingleton() && !bd.isLazyInit()) {
```
```python
// 处理FactoryBean
```
```python
if
```
```python
(isFactoryBean(beanName)) {
```
```python
// 获取FactoryBean实例，FactoryBean前面会加一个&符号
```
```python
final
```
```python
FactoryBean<?> factory = (FactoryBean<?>) getBean(FACTORY_BEAN_PREFIX + beanName);
```
```python
boolean
```
```python
isEagerInit;
```
```python
if
```
```python
(System.getSecurityManager() !=
```
```python
null
```
```python
&& factory
```
```python
instanceof
```
```python
SmartFactoryBean) {
               isEagerInit = AccessController.doPrivileged((PrivilegedAction<Boolean>) () ->
                     ((SmartFactoryBean<?>) factory).isEagerInit(),
                     getAccessControlContext());
            }
```
```python
else
```
```python
{
               isEagerInit = (factory
```
```python
instanceof
```
```python
SmartFactoryBean &&
                     ((SmartFactoryBean<?>) factory).isEagerInit());
            }
```
```python
if
```
```python
(isEagerInit) {
               getBean(beanName);
            }
         }
```
```python
// 非Factorybean,直接调用getBean方法，关键所在，后续分析
```
```python
else
```
```python
{
            getBean(beanName);
         }
      }
   }
```
```python
// bean创建后，对SmartInitializingSingleton回调afterSingletonsInstantiated()方法，这儿不用太care
```
```python
for
```
```python
(String beanName : beanNames) {
      Object singletonInstance = getSingleton(beanName);
```
```python
if
```
```python
(singletonInstance
```
```python
instanceof
```
```python
SmartInitializingSingleton) {
```
```python
final
```
```python
SmartInitializingSingleton smartSingleton = (SmartInitializingSingleton) singletonInstance;
```
```python
if
```
```python
(System.getSecurityManager() !=
```
```python
null
```
```python
) {
            AccessController.doPrivileged((PrivilegedAction<Object>) () -> {
               smartSingleton.afterSingletonsInstantiated();
```
```python
return
```
```python
null
```
```python
;
            }, getAccessControlContext());
         }
```
```python
else
```
```python
{
            smartSingleton.afterSingletonsInstantiated();
         }
      }
   }
}
```
preInstantiateSingletons流程稍微复杂点，主要有
获取XML解析时的beanNames
遍历beanNames，获取BeanDefinition。对非abstract，非lazy-init的singleton bean的进行实例化
如果是FactoryBean，则需要判断isEagerInit，来确定是否调用getBean创建对应的bean。
如果不是，则直接调用getBean创建对应bean
bean创建后，对SmartInitializingSingleton回调afterSingletonsInstantiated()方法。
## 2.2 getBean 创建和初始化bean实例
下面我们着重来分析bean的创建，也就是getBean()方法。
```python
public
```
```python
Object
```
```python
getBean
```
```python
(String name)
```
```python
throws
```
```python
BeansException {
```
```python
return
```
```python
doGetBean(name,
```
```python
null
```
```python
,
```
```python
null
```
```python
,
```
```python
false
```
```python
);
}
```
```python
protected
```
```python
<T> T
```
```python
doGetBean
```
```python
(
```
```python
final
```
```python
String name, @Nullable
```
```python
final
```
```python
Class<T> requiredType,
      @Nullable
```
```python
final
```
```python
Object[] args,
```
```python
boolean
```
```python
typeCheckOnly)
```
```python
throws
```
```python
BeansException {
```
```python
// beanname转换，去掉FactoryBean的&前缀，处理alias声明。细节可自行分析
```
```python
final
```
```python
String beanName = transformedBeanName(name);
   Object bean;
   Object sharedInstance = getSingleton(beanName);
```
```python
if
```
```python
(sharedInstance !=
```
```python
null
```
```python
&& args ==
```
```python
null
```
```python
) {
```
```python
// 判断singleton bean是否已经创建好了，创建好了则直接从内存取出。
```
```python
bean = getObjectForBeanInstance(sharedInstance, name, beanName,
```
```python
null
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
```
```python
// 之前没创建的，则需要创建。
```
```python
// 正在创建，则直接异常返回
```
```python
if
```
```python
(isPrototypeCurrentlyInCreation(beanName)) {
```
```python
throw
```
```python
new
```
```python
BeanCurrentlyInCreationException(beanName);
      }
```
```python
// 检查是否有beanname对应的BeanDefinition
```
```python
BeanFactory parentBeanFactory = getParentBeanFactory();
```
```python
if
```
```python
(parentBeanFactory !=
```
```python
null
```
```python
&& !containsBeanDefinition(beanName)) {
```
```python
// 没有找到BeanDefinition，看看parent工厂中有没有，调用parent工厂的getBean
```
```python
// 获取原始的name，包含了FactoryBean前缀，&符号
```
```python
String nameToLookup = originalBeanName(name);
```
```python
if
```
```python
(parentBeanFactory
```
```python
instanceof
```
```python
AbstractBeanFactory) {
```
```python
return
```
```python
((AbstractBeanFactory) parentBeanFactory).doGetBean(
                  nameToLookup, requiredType, args, typeCheckOnly);
         }
```
```python
else
```
```python
if
```
```python
(args !=
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
(T) parentBeanFactory.getBean(nameToLookup, args);
         }
```
```python
else
```
```python
{
```
```python
return
```
```python
parentBeanFactory.getBean(nameToLookup, requiredType);
         }
      }
```
```python
if
```
```python
(!typeCheckOnly) {
         markBeanAsCreated(beanName);
      }
```
```python
try
```
```python
{
```
```python
// 找到了beanname对应的BeanDefinition，合并parent的BeanDefinition(XML中的parent属性)
```
```python
final
```
```python
RootBeanDefinition mbd = getMergedLocalBeanDefinition(beanName);
         checkMergedBeanDefinition(mbd, beanName, args);
```
```python
// 处理dependsOn属性
```
```python
String[] dependsOn = mbd.getDependsOn();
```
```python
if
```
```python
(dependsOn !=
```
```python
null
```
```python
) {
```
```python
// 遍历所有的dependOn bean，要先注册和创建依赖的bean
```
```python
for
```
```python
(String dep : dependsOn) {
```
```python
// check是否两个bean是循环依赖，spring不能出现bean的循环依赖
```
```python
if
```
```python
(isDependent(beanName, dep)) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(mbd.getResourceDescription(), beanName,
```
```python
"Circular depends-on relationship between '"
```
```python
+ beanName +
```
```python
"' and '"
```
```python
+ dep +
```
```python
"'"
```
```python
);
               }
```
```python
// 注册并创建依赖的bean
```
```python
registerDependentBean(dep, beanName);
               getBean(dep);
            }
         }
```
```python
// 处理scope属性
```
```python
if
```
```python
(mbd.isSingleton()) {
```
```python
// singleton, 必须保证线程安全情况下创建bean，保证单例
```
```python
sharedInstance = getSingleton(beanName, () -> {
```
```python
try
```
```python
{
```
```python
// 反射创建bean实例，这个过程很复杂，稍后分析
```
```python
return
```
```python
createBean(beanName, mbd, args);
               }
```
```python
catch
```
```python
(BeansException ex) {
```
```python
// 异常处理，清除掉bean
```
```python
destroySingleton(beanName);
```
```python
throw
```
```python
ex;
               }
            });
```
```python
// 获取bean实例
```
```python
bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd);
         }
```
```python
else
```
```python
if
```
```python
(mbd.isPrototype()) {
```
```python
// prototype，创建一个全新的实例
```
```python
Object prototypeInstance =
```
```python
null
```
```python
;
```
```python
try
```
```python
{
```
```python
// 创建前的回调
```
```python
beforePrototypeCreation(beanName);
```
```python
// 反射创建bean实例，稍后详细分析
```
```python
prototypeInstance = createBean(beanName, mbd, args);
            }
```
```python
finally
```
```python
{
```
```python
// 创建后的回调，清除inCreation的标志
```
```python
afterPrototypeCreation(beanName);
            }
            bean = getObjectForBeanInstance(prototypeInstance, name, beanName, mbd);
         }
```
```python
else
```
```python
{
```
```python
// 其他scope值
```
```python
String scopeName = mbd.getScope();
```
```python
final
```
```python
Scope scope =
```
```python
this
```
```python
.scopes.get(scopeName);
```
```python
if
```
```python
(scope ==
```
```python
null
```
```python
) {
```
```python
// scope属性不能接收空值
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"No Scope registered for scope name '"
```
```python
+ scopeName +
```
```python
"'"
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
               Object scopedInstance = scope.get(beanName, () -> {
                  beforePrototypeCreation(beanName);
```
```python
try
```
```python
{
```
```python
return
```
```python
createBean(beanName, mbd, args);
                  }
```
```python
finally
```
```python
{
                     afterPrototypeCreation(beanName);
                  }
               });
               bean = getObjectForBeanInstance(scopedInstance, name, beanName, mbd);
            }
```
```python
catch
```
```python
(IllegalStateException ex) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(beanName,
```
```python
"Scope '"
```
```python
+ scopeName +
```
```python
"' is not active for the current thread; consider "
```
```python
+
```
```python
"defining a scoped proxy for this bean if you intend to refer to it from a singleton"
```
```python
,
                     ex);
            }
         }
      }
```
```python
catch
```
```python
(BeansException ex) {
         cleanupAfterBeanCreationFailure(beanName);
```
```python
throw
```
```python
ex;
      }
   }
```
```python
// check 创建的bean是否是requiredType指明的类型。如果不是，先做转换，转换不成的话只能类型不匹配抛出异常了
```
```python
if
```
```python
(requiredType !=
```
```python
null
```
```python
&& bean !=
```
```python
null
```
```python
&& !requiredType.isInstance(bean)) {
```
```python
try
```
```python
{
```
```python
// 尝试将创建的bean转换为requiredType指明的类型
```
```python
return
```
```python
getTypeConverter().convertIfNecessary(bean, requiredType);
        }
```
```python
catch
```
```python
(TypeMismatchException ex) {
```
```python
// 转换不成功，抛出异常
```
```python
throw
```
```python
new
```
```python
BeanNotOfRequiredTypeException(name, requiredType, bean.getClass());
        }
   }
```
```python
return
```
```python
(T) bean;
}
```
doGetBean概括了bean创建和初始化的主要流程，十分复杂，步骤主要为
beanname转换，去掉FactoryBean的&前缀，处理alias声明
判断singleton bean是否已经创建好了，创建好了则直接从内存取出
没有创建好，则检查是否有beanname对应的BeanDefinition，没有则到parent工厂中查找，命中则使用parent工厂再次调用getBean以及doGetBean创建
有BeanDefinition，则合并parent属性指向的中的属性，这主要是处理bean的parent属性。子bean会继承parent bean的属性。
处理dependsOn属性。必须先创建好所有的依赖的bean
处理scope属性，如果是singleton的，则必须保证线程安全情况下创建单例。如果是prototype，则必须保证创建一个全新的bean。创建bean通过createBean()反射创建。
## 2.3 createBean 反射创建bean实例
下面来分析createBean()方法，这个过程也是相当复杂的。
```python
protected
```
```python
Object
```
```python
createBean
```
```python
(String beanName, RootBeanDefinition mbd, @Nullable Object[] args)
```
```python
throws
```
```python
BeanCreationException {
   RootBeanDefinition mbdToUse = mbd;
```
```python
// 拷贝一个新的RootBeanDefinition供创建bean使用
```
```python
Class<?> resolvedClass = resolveBeanClass(mbd, beanName);
```
```python
if
```
```python
(resolvedClass !=
```
```python
null
```
```python
&& !mbd.hasBeanClass() && mbd.getBeanClassName() !=
```
```python
null
```
```python
) {
      mbdToUse =
```
```python
new
```
```python
RootBeanDefinition(mbd);
      mbdToUse.setBeanClass(resolvedClass);
   }
```
```python
// 处理bean中定义的覆盖方法，主要是xml:lookup-method或replace-method。标记override的方法为已经加载过的，避免不必要的参数检查开销。这儿不详细展开了。
```
```python
try
```
```python
{
      mbdToUse.prepareMethodOverrides();
   }
```
```python
catch
```
```python
(BeanDefinitionValidationException ex) {
```
```python
throw
```
```python
new
```
```python
BeanDefinitionStoreException(mbdToUse.getResourceDescription(),
            beanName,
```
```python
"Validation of method overrides failed"
```
```python
, ex);
   }
```
```python
// 调用BeanPostProcessors bean后处理器，使得bean后处理器可以返回一个proxy bean，从而代替我们要创建的bean。回调后处理器的postProcessBeforeInstantiation()方法，如果这个方法中返回了一个bean，也就是使用了proxy，则再回调postProcessAfterInitialization()方法。之后返回这个Proxy bean即可。
```
```python
try
```
```python
{
      Object bean = resolveBeforeInstantiation(beanName, mbdToUse);
```
```python
if
```
```python
(bean !=
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
bean;
      }
   }
```
```python
catch
```
```python
(Throwable ex) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(mbdToUse.getResourceDescription(), beanName,
```
```python
"BeanPostProcessor before instantiation of bean failed"
```
```python
, ex);
   }
```
```python
// doCreateBean创建bean实例，后面详细分析
```
```python
try
```
```python
{
      Object beanInstance = doCreateBean(beanName, mbdToUse, args);
```
```python
return
```
```python
beanInstance;
   }
```
```python
// 各种异常，省略
```
```python
... 
}
```
createBean()方法大概步骤如下
拷贝一个新的RootBeanDefinition供创建bean使用
处理lookup-method或replace-method
调用BeanPostProcessors后处理器
doCreateBean创建bean实例
下面我们重点分析doCreateBean方法
```python
protected
```
```python
Object
```
```python
doCreateBean
```
```python
(
```
```python
final
```
```python
String beanName,
```
```python
final
```
```python
RootBeanDefinition mbd,
```
```python
final
```
```python
@Nullable Object[] args)
```
```python
throws
```
```python
BeanCreationException {
```
```python
// 创建bean实例，如果是singleton，先尝试从缓存中取，取不到则创建
```
```python
BeanWrapper instanceWrapper =
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
(mbd.isSingleton()) {
      instanceWrapper =
```
```python
this
```
```python
.factoryBeanInstanceCache.remove(beanName);
   }
```
```python
if
```
```python
(instanceWrapper ==
```
```python
null
```
```python
) {
```
```python
// 反射创建bean实例，后面详细说
```
```python
instanceWrapper = createBeanInstance(beanName, mbd, args);
   }
```
```python
final
```
```python
Object bean = (instanceWrapper !=
```
```python
null
```
```python
? instanceWrapper.getWrappedInstance() :
```
```python
null
```
```python
);
   Class<?> beanType = (instanceWrapper !=
```
```python
null
```
```python
? instanceWrapper.getWrappedClass() :
```
```python
null
```
```python
);
   mbd.resolvedTargetType = beanType;
```
```python
// 回调MergedBeanDefinitionPostProcessor.postProcessMergedBeanDefinition，它可以修改bean属性
```
```python
if
```
```python
(beanType !=
```
```python
null
```
```python
) {
```
```python
synchronized
```
```python
(mbd.postProcessingLock) {
```
```python
if
```
```python
(!mbd.postProcessed) {
```
```python
try
```
```python
{
               applyMergedBeanDefinitionPostProcessors(mbd, beanType, beanName);
            }
```
```python
catch
```
```python
(Throwable ex) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(mbd.getResourceDescription(), beanName,
```
```python
"Post-processing of merged bean definition failed"
```
```python
, ex);
            }
            mbd.postProcessed =
```
```python
true
```
```python
;
         }
      }
   }
```
```python
// 曝光单例对象的引用，主要是为了解决单例间的循环依赖问题，以及依赖的bean比较复杂时的初始化性能问题
```
```python
boolean
```
```python
earlySingletonExposure = (mbd.isSingleton() &&
```
```python
this
```
```python
.allowCircularReferences &&
         isSingletonCurrentlyInCreation(beanName));
```
```python
if
```
```python
(earlySingletonExposure) {
      addSingletonFactory(beanName, () -> getEarlyBeanReference(beanName, mbd, bean));
   }
```
```python
// 初始化bean，后面详细介绍
```
```python
Object exposedObject = bean;
```
```python
try
```
```python
{
      populateBean(beanName, mbd, instanceWrapper);
```
```python
if
```
```python
(exposedObject !=
```
```python
null
```
```python
) {
         exposedObject = initializeBean(beanName, exposedObject, mbd);
      }
   }
```
```python
// 省略异常处理
```
```python
// 单例曝光对象的处理，不用太在意
```
```python
if
```
```python
(earlySingletonExposure) {
      Object earlySingletonReference = getSingleton(beanName,
```
```python
false
```
```python
);
```
```python
if
```
```python
(earlySingletonReference !=
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
(exposedObject == bean) {
            exposedObject = earlySingletonReference;
         }
```
```python
else
```
```python
if
```
```python
(!
```
```python
this
```
```python
.allowRawInjectionDespiteWrapping && hasDependentBean(beanName)) {
            String[] dependentBeans = getDependentBeans(beanName);
            Set<String> actualDependentBeans =
```
```python
new
```
```python
LinkedHashSet<>(dependentBeans.length);
```
```python
for
```
```python
(String dependentBean : dependentBeans) {
```
```python
if
```
```python
(!removeSingletonIfCreatedForTypeCheckOnly(dependentBean)) {
                  actualDependentBeans.add(dependentBean);
               }
            }
```
```python
if
```
```python
(!actualDependentBeans.isEmpty()) {
```
```python
// 抛出异常，省略代码
```
```python
}
         }
      }
   }
```
```python
// 注册bean为可销毁的bean，bean销毁时，会回调destroy-method
```
```python
if
```
```python
(bean !=
```
```python
null
```
```python
) {
```
```python
try
```
```python
{
         registerDisposableBeanIfNecessary(beanName, bean, mbd);
      }
```
```python
catch
```
```python
(BeanDefinitionValidationException ex) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(
               mbd.getResourceDescription(), beanName,
```
```python
"Invalid destruction signature"
```
```python
, ex);
      }
   }
```
```python
return
```
```python
exposedObject;
}
```
doCreateBean方法主要流程为
createBeanInstance() 创建bean实例
回调postProcessMergedBeanDefinition(), 可以修改bean属性
initializeBean() 初始化bean实例，包括后处理器的调用，init-method的调用等
注册bean为可销毁的，这样在bean销毁时，就可以回调到destroy-method.
### 2.3.1 createBeanInstance 反射创建bean实例
我们先分析如何创建bean实例的。
```python
个
```
```python
protected
```
```python
BeanWrapper
```
```python
createBeanInstance
```
```python
(String beanName, RootBeanDefinition mbd, @Nullable Object[] args) {
```
```python
// 先创建class对象，反射的套路。利用bean的class属性进行反射，所以class属性一定要是bean的实现类
```
```python
Class<?> beanClass = resolveBeanClass(mbd, beanName);
```
```python
// class如果不是public的，则抛出异常。因为没法进行实例化
```
```python
if
```
```python
(beanClass !=
```
```python
null
```
```python
&& !Modifier.isPublic(beanClass.getModifiers()) && !mbd.isNonPublicAccessAllowed()) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(mbd.getResourceDescription(), beanName,
```
```python
"Bean class isn't public, and non-public access not allowed: "
```
```python
+ beanClass.getName());
   }
```
```python
//
```
```python
Supplier<?> instanceSupplier = mbd.getInstanceSupplier();
```
```python
if
```
```python
(instanceSupplier !=
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
obtainFromSupplier(instanceSupplier, beanName);
   }
```
```python
// 使用FactoryBean的factory-method来创建，支持静态工厂和实例工厂
```
```python
if
```
```python
(mbd.getFactoryMethodName() !=
```
```python
null
```
```python
)  {
```
```python
return
```
```python
instantiateUsingFactoryMethod(beanName, mbd, args);
   }
```
```python
// 无参数情况时，创建bean。调用无参构造方法
```
```python
boolean
```
```python
resolved =
```
```python
false
```
```python
;
```
```python
boolean
```
```python
autowireNecessary =
```
```python
false
```
```python
;
```
```python
if
```
```python
(args ==
```
```python
null
```
```python
) {
```
```python
synchronized
```
```python
(mbd.constructorArgumentLock) {
```
```python
if
```
```python
(mbd.resolvedConstructorOrFactoryMethod !=
```
```python
null
```
```python
) {
            resolved =
```
```python
true
```
```python
;
            autowireNecessary = mbd.constructorArgumentsResolved;
         }
      }
   }
```
```python
if
```
```python
(resolved) {
```
```python
if
```
```python
(autowireNecessary) {
```
```python
// autoWire创建 自动装配
```
```python
return
```
```python
autowireConstructor(beanName, mbd,
```
```python
null
```
```python
,
```
```python
null
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
```
```python
// 普通创建
```
```python
return
```
```python
instantiateBean(beanName, mbd);
      }
   }
```
```python
// 有参数情况时，创建bean。先利用参数个数，类型等，确定最精确匹配的构造方法。
```
```python
Constructor<?>[] ctors = determineConstructorsFromBeanPostProcessors(beanClass, beanName);
```
```python
if
```
```python
(ctors !=
```
```python
null
```
```python
||
         mbd.getResolvedAutowireMode() == RootBeanDefinition.AUTOWIRE_CONSTRUCTOR ||
         mbd.hasConstructorArgumentValues() || !ObjectUtils.isEmpty(args))  {
```
```python
return
```
```python
autowireConstructor(beanName, mbd, ctors, args);
   }
```
```python
// 有参数时，又没获取到构造方法，则只能调用无参构造方法来创建实例了(兜底方法)
```
```python
return
```
```python
instantiateBean(beanName, mbd);
}
```
instantiateBean，使用无参构造方法，反射创建bean实例代码如下
```python
protected
```
```python
BeanWrapper
```
```python
instantiateBean
```
```python
(
```
```python
final
```
```python
String beanName,
```
```python
final
```
```python
RootBeanDefinition mbd) {
```
```python
try
```
```python
{
      Object beanInstance;
```
```python
final
```
```python
BeanFactory parent =
```
```python
this
```
```python
;
```
```python
if
```
```python
(System.getSecurityManager() !=
```
```python
null
```
```python
) {
         beanInstance = AccessController.doPrivileged((PrivilegedAction<Object>) () ->
               getInstantiationStrategy().instantiate(mbd, beanName, parent),
               getAccessControlContext());
      }
```
```python
else
```
```python
{
```
```python
// 创建实例，关键点，其他都不用care
```
```python
beanInstance = getInstantiationStrategy().instantiate(mbd, beanName, parent);
      }
      BeanWrapper bw =
```
```python
new
```
```python
BeanWrapperImpl(beanInstance);
      initBeanWrapper(bw);
```
```python
return
```
```python
bw;
   }
```
```python
catch
```
```python
(Throwable ex) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(
            mbd.getResourceDescription(), beanName,
```
```python
"Instantiation of bean failed"
```
```python
, ex);
   }
}
```
这个方法没什么要注意的，关键点在instantiate方法
```python
public
```
```python
Object
```
```python
instantiate
```
```python
(RootBeanDefinition bd, @Nullable String beanName, BeanFactory owner) {
```
```python
// Don't override the class with CGLIB if no overrides.
```
```python
if
```
```python
(bd.getMethodOverrides().isEmpty()) {
      Constructor<?> constructorToUse;
```
```python
// 保证线程安全情况下，获取Constructor
```
```python
synchronized
```
```python
(bd.constructorArgumentLock) {
```
```python
// 获取构造方法或factory-method
```
```python
constructorToUse = (Constructor<?>) bd.resolvedConstructorOrFactoryMethod;
```
```python
if
```
```python
(constructorToUse ==
```
```python
null
```
```python
) {
```
```python
// BeanDefinition中如果没有Constructor或者factory-method，则直接使用默认无参构造方法。
```
```python
final
```
```python
Class<?> clazz = bd.getBeanClass();
```
```python
if
```
```python
(clazz.isInterface()) {
```
```python
throw
```
```python
new
```
```python
BeanInstantiationException(clazz,
```
```python
"Specified class is an interface"
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
if
```
```python
(System.getSecurityManager() !=
```
```python
null
```
```python
) {
                  constructorToUse = AccessController.doPrivileged(
                        (PrivilegedExceptionAction<Constructor<?>>) () ->
                              clazz.getDeclaredConstructor());
               }
```
```python
else
```
```python
{
```
```python
// 获取默认无参构造方法
```
```python
constructorToUse = clazz.getDeclaredConstructor();
               }
               bd.resolvedConstructorOrFactoryMethod = constructorToUse;
            }
```
```python
catch
```
```python
(Throwable ex) {
```
```python
throw
```
```python
new
```
```python
BeanInstantiationException(clazz,
```
```python
"No default constructor found"
```
```python
, ex);
            }
         }
      }
```
```python
// 使用上一步得到的Constructor，反射获取bean实例
```
```python
return
```
```python
BeanUtils.instantiateClass(constructorToUse);
   }
```
```python
else
```
```python
{
```
```python
// Must generate CGLIB subclass.
```
```python
return
```
```python
instantiateWithMethodInjection(bd, beanName, owner);
   }
}
```
instantiate方法主要做两件事
确定Constructor或者factory-method
利用Constructor，反射创建bean实例。
分析到这儿Bean的创建就结束了，这个过程实在是太复杂了！
### 2.3.2 initializeBean 初始化bean实例
bean创建完后，容器会对它进行初始化，包括后处理的调用，init-method的调用等。请看下面详解。
```python
protected
```
```python
Object
```
```python
initializeBean
```
```python
(
```
```python
final
```
```python
String beanName,
```
```python
final
```
```python
Object bean, @Nullable RootBeanDefinition mbd) {
```
```python
// 回调各种aware method，如BeanNameAware， BeanFactoryAware等
```
```python
if
```
```python
(System.getSecurityManager() !=
```
```python
null
```
```python
) {
      AccessController.doPrivileged((PrivilegedAction<Object>) () -> {
         invokeAwareMethods(beanName, bean);
```
```python
return
```
```python
null
```
```python
;
      }, getAccessControlContext());
   }
```
```python
else
```
```python
{
      invokeAwareMethods(beanName, bean);
   }
```
```python
// 回调beanPostProcessor的postProcessBeforeInitialization()方法
```
```python
Object wrappedBean = bean;
```
```python
if
```
```python
(mbd ==
```
```python
null
```
```python
|| !mbd.isSynthetic()) {
      wrappedBean = applyBeanPostProcessorsBeforeInitialization(wrappedBean, beanName);
   }
```
```python
// init-method 和 postProcessAfterInitialization
```
```python
if
```
```python
(wrappedBean !=
```
```python
null
```
```python
) {
```
```python
try
```
```python
{
```
```python
// 回调init-method
```
```python
invokeInitMethods(beanName, wrappedBean, mbd);
      }
```
```python
catch
```
```python
(Throwable ex) {
```
```python
throw
```
```python
new
```
```python
BeanCreationException(
               (mbd !=
```
```python
null
```
```python
? mbd.getResourceDescription() :
```
```python
null
```
```python
),
               beanName,
```
```python
"Invocation of init method failed"
```
```python
, ex);
      }
```
```python
// 回调beanPostProcessor的postProcessAfterInitialization()方法
```
```python
if
```
```python
(mbd ==
```
```python
null
```
```python
|| !mbd.isSynthetic()) {
         wrappedBean = applyBeanPostProcessorsAfterInitialization(wrappedBean, beanName);
      }
   }
```
```python
return
```
```python
wrappedBean;
}
```
由此可见，initializeBean(),也就是bean的初始化流程为
回调各种aware method，如BeanNameAware，将容器中相关引用注入到bean中，供bean使用
回调beanPostProcessor的postProcessBeforeInitialization(), 后处理器的初始化前置调用
回调init-method， 注解和XML中都可以声明
回调beanPostProcessor的postProcessAfterInitialization()方法，后处理器的初始化后置调用。
从这个流程，我们也能清晰的分析出容器后处理器两个方法的调用时机。分析源码可以大大加深我们对spring API的理解。
# 3 总结
Bean实例的创建和初始化流程还是十分复杂的。从源码中可以清晰的分析出spring bean的各种特性。如factory-method, BeanPostProcessor等。有助于我们spring bean行为的理解。所以分析源码还是十分值得的。

