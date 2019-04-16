# Java基础知识 - youbingchen的博客 - CSDN博客





2017年08月10日 23:42:42[youbingchen](https://me.csdn.net/youbingchen)阅读数：229








出来混，要学习java，本人原先具有C++的功力，所以学习java并不觉得难，但是还有一些特性难以时刻记住，所以在此做一些笔记，方便以后快速查询。

# 注解（元数据）

> 
第一次接触注解的概念，先了解一下注解的使用地方

- 生成文档，这个是最常见的功能，常用的注解有@param @return @see
- 跟踪代码的依赖性，实现替代配置文件功能
- 在编译时进行格式检查。如@override 放在方法前，如果你这个方法并不是覆盖了超类方法，则编译时就能检查出。

声明注解的注意事项：
- 所有注解都只能包括方法的声明，不能为方法提供方法体。
- 注解不能包括extends语句

> 
注解借口在`java.lang.annotation`包中声明


### 注解的保留策略

> 
Java定义了3种策略，它们被封装在`java.lang.annotation.RetentionPolicy`枚举中。这三种策略分别是SOURCE、CLASS和RUNTIME

- 使用SOURCE保留策略的注解，只在源文件中保留，在编译期间会被抛弃
- 使用CLASS保留策略的注解，在编译时会被存储在.class文件中，但是，在运行时通过JVM不能得到这些注解
- 使用RUNTIME保留策略的注解，在编译时存储在.class文件中，并且在运行时可以通过JVM获取这些注解，因此，RUNTIME保留策略提供了最永久的注解。

局部变量声明的注解是不能存储在.class文件中

保留策略是通过Java的内置注解—-@Retention

```java
@Retention(retention-policy)
```

#### 参考文章

[深入理解Java：注解（Annotation）自定义注解入门](http://www.cnblogs.com/peida/archive/2013/04/24/3036689.html)
[深入理解Java：注解（Annotation）–注解处理器](http://www.cnblogs.com/peida/archive/2013/04/26/3038503.html)
[Java基础加强总结(一)——注解(Annotation)](http://www.cnblogs.com/xdp-gacl/p/3622275.html)
[深入理解Java注解——注解之基本概念](http://blog.csdn.net/Ssunsets/article/details/51042494)
[深入理解Java注解——元注解及自定义注解](blog.csdn.net/Ssunsets/article/details/51042496)
# 泛型

> 
Java泛型工作原理和C++的模板是不相同的

- 
泛型只使用的引用类型,不能使用基本类型

- 
基于不同类型参数的泛型类是不同的。

- 
泛型提升类型安全性的原理

- 
有界类型


```java
< T extends superclass> # 这样指定的T只能被superclass或其子类替代。会限制T的范围也会阻止创建非数值的Stats对象。
```
- 
使用通配符参数

> 
为了创建泛型的方法，必须使用Java泛型的另一个特性：通配符，通配符参数是由`?`指定的，表示未知类型。通配符不会影响创建什么类型的对象。


- 
通配符的有界类型


```java
< ? extends superclass>
```

### 泛型方法

> 
引用


[Java总结篇系列：Java泛型](http://www.cnblogs.com/lwbqqyumidi/p/3837629.html)




