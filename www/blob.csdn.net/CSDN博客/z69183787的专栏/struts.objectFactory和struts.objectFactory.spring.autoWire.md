# struts.objectFactory和struts.objectFactory.spring.autoWire - z69183787的专栏 - CSDN博客
2015年08月12日 22:14:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：549
struts.objectFactory这个属性用于说明Struts2的对象池创建工厂，Struts2也有自己的对象池，就像Spring那样，在配置文件中你可以引用对象池中的对象，你可以借助于Spring中的对象池，当想要得到Spring中的对象池时，申明struts.objectFactory为Spring的对象池构建工厂。当指定struts.objectFactory为spring时，struts2框架就会把bean转发给spring来创建，装配，注入。但是bean创建完成之后，还是由struts容器来管理其生命周期。
在struts.xml中的代码如下：<constant name="struts.objectFactory" value="spring" />
struts.objectFactory.spring.autoWire是用spring插件通过覆盖(override)Struts2的 ObjectFactory来增强核心框架对象的创建。当创建一个对象的时候，它会用Struts2配置文件中的class属性去和Spring配置文件中的id属性进行关联，如果能找到则由Spring创建，否则由Struts2框架自身创建，然后由Spring来装配。
Spring插件具体有如下几个作用： 
1． 允许spring来创建Action、Interceptror和Result 
2． 由Struts创建的对象能够被Spring装配 
3． 提供了2个拦截器来自动装配action，如果没有使用Spring ObjectFactory的话 
这里要注意的是，我们不必在Spring中去注册action，尽管我们可以这么去做，通常Struts框架会自动的从action mapping中创建action对象。 
这样就是让spring去管理这些bean
