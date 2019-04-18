# Spring ClassPathXmlApplicationContext和FileSystemXmlApplicationContext - z69183787的专栏 - CSDN博客
2012年11月26日 19:12:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1155
个人分类：[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)
先说:ClassPathXmlApplicationContext 
这个类,默认获取的是WEB-INF/classes/下的路径,也就是在myeclipse的src下的路径,所以用这个是获取不到WEB-INF下的配置文件的... 
下面是几个用法,引src下的一个文件,有以下几种方式 
Java代码  ![收藏代码](http://heisetoufa.iteye.com/images/icon_star.png)
- BeanFactory ac = new ClassPathXmlApplicationContext("applicationContext.xml");  
- BeanFactory ac = new ClassPathXmlApplicationContext("classpath:applicationContext.xml");  
- BeanFactory ac = new ClassPathXmlApplicationContext("classpath:/applicationContext.xml");
ApplicationContext ctx = new ClassPathXmlApplicationContext("classpath:/applicationContext.xml");
- ac.getBean("beanID");  
classpath:和classpath*:的区别: 
classpath:   只能加载一个配置文件,如果配置了多个,则只加载第一个 
classpath*:  可以加载多个配置文件,如果有多个配置文件,就用这个 
引src下的多个文件,有以下几种方式 
Java代码  ![收藏代码](http://heisetoufa.iteye.com/images/icon_star.png)
- BeanFactory ac = new ClassPathXmlApplicationContext(new String[]{"applicationContext.xml","applicationContext-test.xml"});  
- BeanFactory ac = new ClassPathXmlApplicationContext("classpath*:/applicationContext*.xml");  
- BeanFactory ac = new ClassPathXmlApplicationContext("classpath*:src/folder/applicationContext*.xml");  
再说:FileSystemXmlApplicationContext 
这个类,默认获取的是项目路径,就是项目名的这级.但是我测试,在myeclipse下,执行main方法,获取配置文件,路径用WebRoot/WEB-INF/是没问题的,但是发布到tomcat下,就怎么配都不成功了,无奈,只能把配置文件全部移到src下了,因为担心发布到linux上后,又出现路径问题 
下面是几个用法,引WEB-INF下的一个文件,有以下几种方式 
Java代码  ![收藏代码](http://heisetoufa.iteye.com/images/icon_star.png)
- BeanFactory factory = new FileSystemXmlApplicationContext("WebRoot/WEB-INF/applicationContext.xml");  
- BeanFactory factory = new FileSystemXmlApplicationContext("file:D:/workspace/testproject/WebRoot/WEB-INF/applicationContext.xml");  
其中,FileSystemXmlApplicationContext的起始路径是项目路径,所以可以直接这样写WebRoot/WEB-INF 
如果前边加了file:则说明后边的路径就要写全路径了,就是绝对路径 
下面是几个用法,引WEB-INF/classes下的一个文件,有以下几种方式 
Java代码  ![收藏代码](http://heisetoufa.iteye.com/images/icon_star.png)
- BeanFactory factory = new FileSystemXmlApplicationContext("classpath:applicationContext.xml");  
这其实跟ClassPathXmlApplicationContext是一样的,多个就加通配符 
下面是几个用法,引WEB-INF下的多个文件,有以下几种方式 
Java代码  ![收藏代码](http://heisetoufa.iteye.com/images/icon_star.png)
- BeanFactory factory = new FileSystemXmlApplicationContext(new String[]{"WebRoot/WEB-INF/applicationContext.xml","WebRoot/WEB-INF/applicationContext-test.xml"});  
- BeanFactory factory = new FileSystemXmlApplicationContext("WebRoot/WEB-INF/applicationContext*.xml");  
- BeanFactory factory = new FileSystemXmlApplicationContext("file:D:/workspace/testproject/WebRoot/WEB-INF/applicationContext*.xml");  
引WEB-INF/classes下的多个文件,也是一样的 
Java代码  ![收藏代码](http://heisetoufa.iteye.com/images/icon_star.png)
- BeanFactory factory = new FileSystemXmlApplicationContext(new String[]{"classpath:applicationContext.xml","classpath:applicationContext*.xml"});  
- BeanFactory factory = new FileSystemXmlApplicationContext("classpath*:applicationContext*.xml");  
