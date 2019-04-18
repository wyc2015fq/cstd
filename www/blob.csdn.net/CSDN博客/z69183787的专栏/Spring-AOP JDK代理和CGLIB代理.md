# Spring-AOP: JDK代理和CGLIB代理 - z69183787的专栏 - CSDN博客
2013年12月26日 16:29:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：961
spring对AOP的支持 
1、如果目标对象实现了接口，默认会采用JDK的动态代理机制实现AOP 
2、如果目标对象实现了接口，可以强制使用CGLIB实现AOP
3、如果目标对象没有实现接口，必须使用CGLIB生成代理，spring会自动在CGLIB和JDK动态代理之间切换
4.**如何强制使用CGLIB生成代理？**
* 添加CGLIB库，SPRING_HOME/lib/cglib/*.jar 
* 在spring的配置文件中加入： 
<aop:aspectj-autoproxy proxy-target-class="true"/> 
**JDK代理和CGLIB代理的区别？**
* JDK代理只能对实现了接口的类生成代理，而不能针对类 
* CGLIB是针对类实现代理的，主要对指定的类生成一个子类，并覆盖其中的方法， 
  因为是继承，所以不能使用final来修饰类或方法<aop:aspectj-autoproxy proxy-target-class="true"/> 
