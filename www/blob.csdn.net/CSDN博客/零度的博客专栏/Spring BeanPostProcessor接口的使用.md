# Spring BeanPostProcessor接口的使用 - 零度的博客专栏 - CSDN博客
2017年12月01日 10:29:55[零度anngle](https://me.csdn.net/zmx729618)阅读数：234标签：[spring																[Bean后置处理器](https://so.csdn.net/so/search/s.do?q=Bean后置处理器&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/zmx729618/article/category/6168885)
有时我们需要在Spring IoC容器初始化受管Bean之前、属性设置之后对该Bean做一些预处理，或者在容器销毁受管Bean之前自己释放资源。那么该如何实现呢？Spring IoC为我们提供了多种方法来实现受管Bean的预处理和后处理。
在Spring中定义了BeanPostProcessors接口，代码如下：
Java代码  ![收藏代码](http://winneryj.iteye.com/images/icon_star.png)
- package org.springframework.beans.factory.config;  
- 
- import org.springframework.beans.BeansException;  
- 
- publicinterface BeanPostProcessor {  
- 
- Object postProcessBeforeInitialization(Object bean,String BeanName)throws BeansException;  
- 
- Object postProcessAfterInitialization(Object bean,String BeanName)throws BeansException;   
- }  
如果这个接口的某个实现类被注册到某个容器，那么该容器的每个受管Bean在调用初始化方法之前，都会获得该接口实现类的一个回调。容器调用接口定义的方法时会将该受管Bean的实例和名字通过参数传入方法，进过处理后通过方法的返回值返回给容器。根据这个原理，我们就可以很轻松的自定义受管Bean。
上面提到过，要使用BeanPostProcessor回调，就必须先在容器中注册实现该接口的类，那么如何注册呢？BeanFactory和ApplicationContext容器的注册方式不大一样：若使用BeanFactory，则必须要显示的调用其addBeanPostProcessor()方法进行注册，参数为BeanPostProcessor实现类的实例；如果是使用ApplicationContext，那么容器会在配置文件在中自动寻找实现了BeanPostProcessor接口的Bean，然后自动注册，我们要做的只是配置一个BeanPostProcessor实现类的Bean就可以了。
注意，假如我们使用了多个的BeanPostProcessor的实现类，那么如何确定处理顺序呢？其实只要实现Ordered接口，设置order属性就可以很轻松的确定不同实现类的处理顺序了。
例程3.5展示了如何使用BeanPostProcessor回调接口。创建Java工程，为其添加Spring开发能力后，创建ioc.test包。再创建一个名字为Animal的Bean，添加name、age成员和speak()方法。代码如下：
Java代码  ![收藏代码](http://winneryj.iteye.com/images/icon_star.png)
- package ioc.test;  
- 
- publicclass Animal {  
- 
- private String name;  
- 
- privateint age;  
- 
- public String speak(){  
- return"我的名字是："+this.name+"，年龄是："+this.age+"!\n";  
- }  
- //Geter和Seter省略
- }  
创建一个BeanPost类，实现BeanPostProcessor接口。在其postProcessAfterInitialization()方法中修改通过参数传入的受管Bean，然后返回。由于它处理容器中的每一个Bean，因此在修改前，应判断Bean是否为我们要处理的Bean。可以通过传入Bean的类型判定，也可以通过传入Bean的名字判定。代码如下：
Java代码  ![收藏代码](http://winneryj.iteye.com/images/icon_star.png)
- package ioc.test;  
- 
- //Import省略
- 
- publicclass BeanPost implements BeanPostProcessor {  
- 
- public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {  
- 
- System.out.println("BeanPostProcessor.postProcessAfterInitialization 正在预处理！");  
- 
- if ((bean instanceof Animal)) {  
- 
- Animal animal = (Animal) bean;  
- 
- animal.setAge(50);  
- 
- animal.setName("猴子");  
- 
- return bean;  
- 
- }  
- 
- return bean;  
- }  
- 
- public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {  
- 
- System.out.println("BeanPostProcessor.postProcessBeforeInitialization 正在预处理！");  
- 
- return bean;  
- 
- }  
- }  
定义好Animal Bean，为其属性随便注入一个值，再定义一个Bean，class为BeanPost类。代码如下：
Java代码  ![收藏代码](http://winneryj.iteye.com/images/icon_star.png)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans  
- xmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd">  
- <bean id="animal"class="ioc.test.Animal">  
- <property name="age"  value="5"></property>  
- <property name="name" value="猪"></property>  
- </bean>  
- <bean id="beanPost"class="ioc.test.BeanPost"></bean>  
- </beans> 
最后创建TestMain类输出结果，代码如下：
Java代码  ![收藏代码](http://winneryj.iteye.com/images/icon_star.png)
- package ioc.test;  
- 
- //Import省略
- 
- publicclass TestMain {  
- 
- publicstaticvoid main(String[] args) {  
- 
- ApplicationContext ac = new ClassPathXmlApplicationContext("applicationContext.xml");  
- Animal animal = (Animal)ac.getBean("animal");  
- System.out.println(animal.speak());      
- 
- }  
- }  
运行结果如下：
![](http://www.blogjava.net/images/blogjava_net/cmzy/clip_image003.gif)
可以看到，输出结果并不是我们在配置文件中注入的值，这说明BeanPost已经成功的修改了目标Bean。

