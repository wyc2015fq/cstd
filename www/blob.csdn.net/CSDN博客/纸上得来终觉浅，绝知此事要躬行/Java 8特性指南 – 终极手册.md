# Java 8特性指南 – 终极手册 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 21:28:28[boonya](https://me.csdn.net/boonya)阅读数：821









From：[http://www.javacodegeeks.com/zh-hans/2015/02/java-8%e7%89%b9%e6%80%a7%e6%8c%87%e5%8d%97.html](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html)

编者：距离Java
 8的发布已经有段时间了，就目前看来它的确是一个非常重要的版本。我们之前已经在JavaCodeGeeks发布了一系列专题文章，如[玩转Java 8-Lambda表达式和并发](http://www.javacodegeeks.com/2014/04/playing-with-java-8-lambdas-and-concurrency.html)，[Java
 8日期/时间API入门：LocalDateTime](http://www.javacodegeeks.com/2014/04/java-8-date-time-api-tutorial-localdatetime.html)以及 [JDK8时代的抽象类vs.接口](http://www.javacodegeeks.com/2014/04/abstract-class-versus-interface-in-the-jdk-8-era.html)。同时我们也从其它来源处引用了[15个必读的Java
 8教程](http://www.javacodegeeks.com/2014/04/15-must-read-java-8-tutorials.html)。当然，我们也对它的一些短板进行了验证，如[Java
 8的暗面](http://www.javacodegeeks.com/2014/04/java-8-friday-the-dark-side-of-java-8.html)。为了读者们更好的阅读体验，现在是时候整合所有Java 8的主要特性于一处了。一起来享受它吧！



### 目录
- [1. 简介](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#introduction)
- [2. Java语言的新特性](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#new_features)
- [2.1. Lambda表达式和函数式接口](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#lambdas_and_functional)
- [2.2. 接口的默认方法和静态方法](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#Interface_Default)
- [2.3. 方法引用](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#method_references)
- [2.4. 重复型注解](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#repeating_annotations)
- [2.5. 更强大的类型推断](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#better_type_inference)
- [2.6. 扩展的注解支持](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#extended_annotations_support)
- [3. Java编译器的新特性](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#new_features_in_java)
- [3.1. 参数名](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#parameter_names)
- [4. Java类库的新特性](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#New%20Features%20in%20Java%20libraries)
- [4.1. Optional](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#Optional)
- [4.2. Streams](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#streams)
- [4.3. 日期/时间API (JSR 310)](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#date/Time_API_(JSR%20310))
- [4.4. Nashorn JavaScript引擎](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#nashorn_JavaScript_engine)
- [4.5. Base64](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#base64)
- [4.6. 并行数组](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#parallel_arrays)
- [4.7. 并发](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#concurrency)
- [5. 新的Java工具类New Java tools](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#new_java_tools)
- [5.1. Nashorn引擎: jjs](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#jjs)
- [5.2. 类的依赖关系分析: jdeps](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#jdeps)
- [6. Java运行时(JVM)的新特性](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#new_features_in_Java_runtime_(JVM))
- [7. 结语](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#conclusions)
- [8. 相关资源](http://www.javacodegeeks.com/zh-hans/2015/02/java-8%E7%89%B9%E6%80%A7%E6%8C%87%E5%8D%97.html#Resources)


注：由于原文内容庞大，所以这里只转载了目录。 


这里还有更多资源更加深入的讨论了Java 8的不同特性：
- 
What’s New in JDK 8: [http://www.oracle.com/technetwork/java/javase/8-whats-new-2157071.html](http://www.oracle.com/technetwork/java/javase/8-whats-new-2157071.html)
- 
The Java Tutorials: [http://docs.oracle.com/javase/tutorial/](http://docs.oracle.com/javase/tutorial/)
- 
WildFly 8, JDK 8, NetBeans 8, Java EE 7: [http://blog.arungupta.me/2014/03/wildfly8-jdk8-netbeans8-javaee7-excellent-combo-enterprise-java/](http://blog.arungupta.me/2014/03/wildfly8-jdk8-netbeans8-javaee7-excellent-combo-enterprise-java/)
- 
Java 8 Tutorial: [http://winterbe.com/posts/2014/03/16/java-8-tutorial/](http://winterbe.com/posts/2014/03/16/java-8-tutorial/)
- 
JDK 8 Command-line Static Dependency Checker: [http://marxsoftware.blogspot.ca/2014/03/jdeps.html](http://marxsoftware.blogspot.ca/2014/03/jdeps.html)
- 
The Illuminating Javadoc of JDK 8: [http://marxsoftware.blogspot.ca/2014/03/illuminating-javadoc-of-jdk-8.html](http://marxsoftware.blogspot.ca/2014/03/illuminating-javadoc-of-jdk-8.html)
- 
The Dark Side of Java 8: [http://blog.jooq.org/2014/04/04/java-8-friday-the-dark-side-of-java-8/](http://blog.jooq.org/2014/04/04/java-8-friday-the-dark-side-of-java-8/)
- 
Installing Java™ 8 Support in Eclipse Kepler SR2: [http://www.eclipse.org/downloads/java8/](http://www.eclipse.org/downloads/java8/)
- 
Java 8: [http://www.baeldung.com/java8](http://www.baeldung.com/java8)
- 
Oracle Nashorn. A Next-Generation JavaScript Engine for the JVM: [http://www.oracle.com/technetwork/articles/java/jf14-nashorn-2126515.html](http://www.oracle.com/technetwork/articles/java/jf14-nashorn-2126515.html)

|Translated by: Vincent Jia|
|----|
|This post is a translation of [Java 8 Features Tutorial – The ULTIMATE Guide](http://www.javacodegeeks.com/2014/05/java-8-features-tutorial.html) from [Andrey Redko](http://www.javacodegeeks.com/author/Andrey-Redko/)|






