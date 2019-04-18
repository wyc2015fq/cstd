# Spring 生命周期 各种初始化方法 执行顺序 - z69183787的专栏 - CSDN博客
2017年11月01日 15:47:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2252
源码：[http://blog.csdn.net/z69183787/article/details/51311749](http://blog.csdn.net/z69183787/article/details/51311749)
0、BeanFactoryPostProcessor调用postProcessBeanFactory方法
1、BeanPostProcessor#postProcessBeforeInitialization
（1.1 @PostConstruct（CommonAnnotationBeanPostProcessor实现，Order大 低优先级））

2、afterPropertiesSet （postProcessBeforeInitialization后执行）
3、init-method
4、BeanPostProcessor#postProcessAfterInitialization
初始化成功
Person [address=广州, name=张三, phone=110]
关闭容器
DiposibleBean#DiposibleBean.destory()
destroy-method
参考：http://blog.csdn.net/z69183787/article/details/78415611
Spring 容器中的 Bean 是有生命周期的，Spring 允许在 Bean 在初始化完成后以及 Bean 销毁前执行特定的操作，常用的设定方式有以下三种：
通过实现 InitializingBean/DisposableBean 接口来定制初始化之后/销毁之前的操作方法；
通过 元素的 init-method/destroy-method属性指定初始化之后 /销毁之前调用的操作方法；
在指定方法上加上@PostConstruct 或@PreDestroy注解来制定该方法是在初始化之后还是销毁之前调用。 
这是我们就有个疑问，这三种方式是完全等同的吗，孰先孰后？
下面我们将带着这个疑问，试图通过测试代码以及分析Spring源码找到答案。
首先，我们还是编写一个简单的测试代码：
Java代码 复制代码 收藏代码
public class InitSequenceBean implements InitializingBean {   
    public InitSequenceBean() {   
       System.out.println("InitSequenceBean: constructor");   
    }   
    @PostConstruct  
    public void postConstruct() {   
       System.out.println("InitSequenceBean: postConstruct");   
    }   
    public void initMethod() {   
       System.out.println("InitSequenceBean: init-method");   
    }   
    @Override  
    public void afterPropertiesSet() throws Exception {   
       System.out.println("InitSequenceBean: afterPropertiesSet");   
    }   
}  
并且在配置文件中添加如下Bean定义：
好了，我们启动Spring容器，观察输出结果，就可知道三者的先后顺序了：
InitSequenceBean: constructor
InitSequenceBean: postConstruct
InitSequenceBean: afterPropertiesSet
InitSequenceBean: init-method
通过上述输出结果，三者的先后顺序也就一目了然了：
Constructor > @PostConstruct > InitializingBean > init-method
先大致分析下为什么会出现这些的结果：构造器（Constructor）被率先调用毋庸置疑，InitializingBean先于init-method我们也可以理解（在也谈Spring容器的生命周期中已经讨论过），但是PostConstruct为何率先于InitializingBean执行呢？
我们再次带着这个疑问去查看Spring源代码来一探究竟。
通过Debug并查看调用栈，我们发现了这个类org.springframework.context.annotation.CommonAnnotationBeanPostProcessor，从命名上，我们就可以得到某些信息——这是一个BeanPostProcessor。想到了什么？在也谈Spring容器的生命周期中，我们提到过BeanPostProcessor的postProcessBeforeInitialization是在Bean生命周期中afterPropertiesSet和init-method之前执被调用的。
再次观察CommonAnnotationBeanPostProcessor这个类，它继承自InitDestroyAnnotationBeanPostProcessor。InitDestroyAnnotationBeanPostProcessor顾名思义，就是在Bean初始化和销毁的时候所作的一个前置/后置处理器。
通过查看InitDestroyAnnotationBeanPostProcessor类下的postProcessBeforeInitialization方法：
Java代码 复制代码 收藏代码
public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {   
       LifecycleMetadata metadata = findLifecycleMetadata(bean.getClass());   
       try {   
           metadata.invokeInitMethods(bean, beanName);   
       }   
       catch (InvocationTargetException ex) {   
           throw new BeanCreationException(beanName, "Invocation of init method failed", ex.getTargetException());   
       }   
       catch (Throwable ex) {   
           throw new BeanCreationException(beanName, "Couldn't invoke init method", ex);   
       }   
        return bean;   
    }  
查看findLifecycleMetadata方法，继而我们跟踪到buildLifecycleMetadata这个方法体中，看下buildLifecycleMetadata这个方法体的内容：
Java代码 复制代码 收藏代码
private LifecycleMetadata buildLifecycleMetadata(final Class clazz) {   
       final LifecycleMetadata newMetadata = new LifecycleMetadata();   
       final boolean debug = logger.isDebugEnabled();   
       ReflectionUtils.doWithMethods(clazz, new ReflectionUtils.MethodCallback() {   
           public void doWith(Method method) {   
              if (initAnnotationType != null) {   
                  if (method.getAnnotation(initAnnotationType) != null) {   
                     newMetadata.addInitMethod(method);   
                     if (debug) {   
                         logger.debug("Found init method on class [" + clazz.getName() + "]: " + method);   
                     }   
                  }   
              }   
              if (destroyAnnotationType != null) {   
                  if (method.getAnnotation(destroyAnnotationType) != null) {   
                     newMetadata.addDestroyMethod(method);   
                     if (debug) {   
                         logger.debug("Found destroy method on class [" + clazz.getName() + "]: " + method);   
                     }   
                  }   
              }   
           }   
       });   
       return newMetadata;   
}  
分析这段代码发现，在这里会去判断某方法有没有被initAnnotationType/destroyAnnotationType注释，如果有，则添加到init/destroy队列中，后续一一执行。
initAnnotationType/destroyAnnotationType注释是什么呢，我们在CommonAnnotationBeanPostProcessor的构造函数中看到下面这段代码：
Java代码 复制代码 收藏代码
public CommonAnnotationBeanPostProcessor() {   
       setOrder(Ordered.LOWEST_PRECEDENCE - 3);   
       setInitAnnotationType(PostConstruct.class);   
       setDestroyAnnotationType(PreDestroy.class);   
       ignoreResourceType("javax.xml.ws.WebServiceContext");   
}  
一切都清晰了吧。一言以蔽之，@PostConstruct注解后的方法在BeanPostProcessor前置处理器中就被执行了，所以当然要先于InitializingBean和init-method执行了。
最后，给出本文的结论，Bean在实例化的过程中：
Constructor > @PostConstruct > InitializingBean > init-method
