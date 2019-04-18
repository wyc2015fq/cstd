# 简单比较init-method，afterPropertiesSet和BeanPostProcessor - z69183787的专栏 - CSDN博客
2017年11月01日 16:07:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2271
一、简单介绍
1、init-method方法，初始化bean的时候执行，可以针对某个具体的bean进行配置。init-method需要在applicationContext.xml配置文档中bean的定义里头写明。例如：
这样，当TestBean在初始化的时候会执行TestBean中定义的init方法。
2、afterPropertiesSet方法，初始化bean的时候执行，可以针对某个具体的bean进行配置。afterPropertiesSet 必须实现 InitializingBean接口。实现 InitializingBean接口必须实现afterPropertiesSet方法。
3、BeanPostProcessor，针对所有Spring上下文中所有的bean，可以在配置文档applicationContext.xml中配置一个BeanPostProcessor，然后对所有的bean进行一个初始化之前和之后的代理。BeanPostProcessor接口中有两个方法： postProcessBeforeInitialization和postProcessAfterInitialization。 postProcessBeforeInitialization方法在bean初始化之前执行， postProcessAfterInitialization方法在bean初始化之后执行。
总之，afterPropertiesSet 和init-method之间的执行顺序是afterPropertiesSet 先执行，init-method 后执行。从BeanPostProcessor的作用，可以看出最先执行的是postProcessBeforeInitialization，然后是afterPropertiesSet，然后是init-method，然后是postProcessAfterInitialization。
二、相关用法及代码测试
![](http://upload-images.jianshu.io/upload_images/1049928-f774246f01ac12af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
该PostProcessor类要作为bean定义到applicationContext.xml中，如下
2、TestBean类，用做测试Bean，观察该Bean初始化过程中上面4个方法执行的先后顺序和内容。实现InitializingBean接口，并且实现接口中的afterPropertiesSet方法。最后定义作为init-method的init方法。
![](http://upload-images.jianshu.io/upload_images/1049928-171cf53e6fe19de1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
结果如下：
------------------------------
对象TestBean开始实例化
******************************
afterPropertiesSet is called
******************************
init-method is called
******************************
对象TestBean实例化完成
------------------------------
3、比较BeanFactoryPostProcessor和BeanPostProcessor
BeanFactoryPostProcessor在bean实例化之前执行，之后实例化bean（**调用构造函数，并调用set方法注入属性值**），然后在调用两个初始化方法前后，执行了BeanPostProcessor。初始化方法的执行顺序是，先执行afterPropertiesSet，再执行init-method。
作者：小程故事多
链接：http://www.jianshu.com/p/890446a3d477
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            
