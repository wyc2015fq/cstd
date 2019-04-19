# CGlib之LazyLoader实现延迟加载对象 - 零度的博客专栏 - CSDN博客
2016年11月14日 11:26:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：853
 一、延迟加载器LazyLoader作用：
       说到延迟加载，应该经常接触到，尤其是使用Hibernate的时候，本篇将通过一个实例分析延迟加载的实现方式。LazyLoader接口继承了Callback，因此也算是CGLib中的一种Callback类型。
二、示例：
        首先定义一个实体类LoaderBean，该Bean内有一个需要延迟加载的属性对象PropertyBean。
LoaderBean.java:
Java代码  ![收藏代码](http://shensy.iteye.com/images/icon_star.png)
- publicclass LoaderBean {  
- private String loaderName;  
- privateint loaderValue;  
- private PropertyBean propertyBean;  
- public LoaderBean(){  
- this.loaderName="loaderNameA";  
- this.loaderValue=123;  
- this.propertyBean=createPropertyBean();  
-     }  
- protected PropertyBean createPropertyBean(){  
-         Enhancer enhancer=new Enhancer();  
-         enhancer.setSuperclass(PropertyBean.class);  
- return (PropertyBean)enhancer.create(PropertyBean.class,new ConcreteClassLazyLoader());  
-     }  
- 
- //setter/getter...
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
PropertyBean.java:
Java代码  ![收藏代码](http://shensy.iteye.com/images/icon_star.png)
- publicclass PropertyBean {  
- private String propertyName;  
- privateint propertyValue;  
- 
- //setter/getter
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
在LoaderBean的构造方法中，对属性Bean进行了代理类生成，使用了CGLib中的LazyLoader回调接口。
Java代码  ![收藏代码](http://shensy.iteye.com/images/icon_star.png)
- publicclass ConcreteClassLazyLoader implements LazyLoader{  
- public Object loadObject() throws Exception {  
-         System.out.println("LazyLoader loadObject() ...");  
-         PropertyBean bean=new PropertyBean();  
-         bean.setPropertyName("lazy-load object propertyName!");  
-         bean.setPropertyValue(11);  
- return bean;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
验证延迟加载：
Java代码  ![收藏代码](http://shensy.iteye.com/images/icon_star.png)
- LoaderBean loader=new LoaderBean();  
- System.out.println(loader.getLoaderName());  
- System.out.println(loader.getLoaderValue());  
- PropertyBean propertyBean=loader.getPropertyBean();//访问延迟加载对象
- System.out.println(propertyBean.getPropertyName());  
- System.out.println(propertyBean.getPropertyValue());  
- System.out.println("after...");  
- //当再次访问延迟加载对象时,就不会再执行回调了
- System.out.println(propertyBean.getPropertyName());  
![](http://static.blog.csdn.net/images/save_snippets.png)
 控制台输出：
控制台代码  ![收藏代码](http://shensy.iteye.com/images/icon_star.png)
- loaderNameA  
- 123
- LazyLoader loadObject() ...  
- lazy-load object propertyName!  
- 11
- after...  
- lazy-load object propertyName!  
[]()
       注意，第一次获取property bean的属性时，会触发代理类回调方法。第二次再获取property bean的属性时，就直接返回属性值而不会再次触发代理类回调方法了。
       可见，延迟加载原理：对需要延迟加载的对象添加代理，在获取该对象属性时先通过代理类回调方法进行对象初始化。在不需要加载该对象时，只要不去获取该对象内属性，该对象就不会被初始化了（在CGLib的实现中只要去访问该对象内属性的getter方法，就会自动触发代理类回调）。
