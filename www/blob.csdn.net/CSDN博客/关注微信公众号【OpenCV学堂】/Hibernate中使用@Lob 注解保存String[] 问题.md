# Hibernate中使用@Lob 注解保存String[] 问题 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年12月16日 12:03:03[gloomyfish](https://me.csdn.net/jia20003)阅读数：9839










## Hibernate中使用@Lob 注解保存String[] 问题

在Hibernate注解中如何你想保存一个字段为String数组类型，如果你想尝试保存为clob类型的话，一般情况下为定义为：



```java
@Entity
Public class Person {
…
@Lob
String[] stringArray;
….
}
```

多数时候当你自动创建表的时候，Hibernate会告诉你Json无法转换或者类转换错误，原因在于你保存text类型文本，Hibernate只接受String类型的，当你定义为String 数组类型，Hibernate就无法处理了，各种奇怪的错误就来了，要不你就死在这个API，要不就通过如下方法来解决这个问题，解决的方法很简单，直接去掉注解，让hibernate自动识别是什么类型就搞成什么类型，最终工作的代码示例如下：

```java
@Entity
Public class Person {
…
// 注意，无需注解
List<String> stringArray;
….
}
```

这样就可以成功保存一个String List/array到你对应的Entity中去了，这个在用Hibernate做Web应用开发中还是挺有用的一个使用技巧。




