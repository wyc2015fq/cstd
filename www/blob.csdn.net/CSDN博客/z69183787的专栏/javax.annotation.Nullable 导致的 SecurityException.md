# javax.annotation.Nullable 导致的 SecurityException - z69183787的专栏 - CSDN博客
2018年12月06日 18:14:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：291
java.lang.SecurityException: class "javax.annotation.Nullable"'s signer information does not match signer information of other classes in the same package
## 1.1. 场景
代码中使用了 javax.annotation.Nullable，项目启动时报 SecurityException。
## 1.2. 问题分析
项目依赖的两个 jar 包中都有 javax.annotation，一个是 com.google.code.findbugs:annotations 另一个是 org.eclipse.jetty.orbit:javax.annotation。
两个分别引入的是 JSR 305 和 JSR 250，但是 jetty 中的 jar 是带签名的，而 findbugs 中没有签名，jvm 加载检查数字签名有问题，会抛异常。
## 1.3. 原理分析
TODO
参考文档
- [http://stackoverflow.com/questions/2877262/java-securityexception-signer-information-does-not-match](http://stackoverflow.com/questions/2877262/java-securityexception-signer-information-does-not-match)
- [http://blog.anthavio.net/2013/11/how-many-javaxannotation-jars-is-out.html](http://blog.anthavio.net/2013/11/how-many-javaxannotation-jars-is-out.html)
- [http://stackoverflow.com/questions/33447686/why-javax-annotation-of-different-organization-are-different](http://stackoverflow.com/questions/33447686/why-javax-annotation-of-different-organization-are-different)
jetty 8 (8.1.10.v20130312) 依赖 org.eclipse.jetty.orbit:javax.annotation；mtthrift、mtconfig 则依赖 com.google.code.findbugs:annotations。这两个依赖中都有 javax.annotation 包，但是前者 jar 是带签名的，后者没有签名。jvm 加载时，检查到数字签名有问题，就会抛出异常。
jetty 依赖的 javax.annotation 实现了 [JSR 250: Common Annotations for the JavaTM Platform](http://jcp.org/en/jsr/detail?id=250)：
- 
javax.annotation (Generated, ManagedBean , PostConstruct, PreDestroy, Resource, Resources)
- 
javax.annotation.security (DeclareRoles, DenyAll, PermitAll, RolesAllowed, RunAs) 
- 
javax.annotation.sql (DataSourceDefinition, DataSourceDefinitions) - since version 1.1
除了 Jetty Orbit，还有以下常见的 JSR 250 实现：
- 
jsr250-api.jar - [http://mvnrepository.com/artifact/javax.annotation/jsr250-api](http://mvnrepository.com/artifact/javax.annotation/jsr250-api)
- 
javax.annotation-api.jar - [http://mvnrepository.com/artifact/javax.annotation/javax.annotation-api](http://mvnrepository.com/artifact/javax.annotation/javax.annotation-api)
- 
Geronimo - [http://mvnrepository.com/artifact/org.apache.geronimo.specs/geronimo-annotation_1.0_spec](http://mvnrepository.com/artifact/org.apache.geronimo.specs/geronimo-annotation_1.0_spec)
- 
Glassfish - [http://mvnrepository.com/artifact/org.glassfish/javax.annotation](http://mvnrepository.com/artifact/org.glassfish/javax.annotation)
- 
And loads of others - [http://mavenhub.com/c/javax/annotation/postconstruct](http://mavenhub.com/c/javax/annotation/postconstruct)
而 findbugs:annotations 则实现了 [JSR 305: Annotations for Software Defect Detection](http://jcp.org/en/jsr/detail?id=305)：
- 
javax.annotation (CheckForNull, ... , Nonnull, Nullable, ... , WillNotClose)
- 
javax.annotation.concurrent (GuardedBy, ... , ThreadSafe)
- 
javax.annotation.meta (Exclusive, ... , When)
除了这个 findbugs:annotations，还有一个实现 jsr305.jar [http://mavenhub.com/c/javax/annotation/nullable/jar](http://mavenhub.com/c/javax/annotation/nullable/jar)。
