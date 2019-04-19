# Java 依赖注入标准 JSR-330 简介 - 定义域 - CSDN博客
2016年03月01日 16:33:43[定义域](https://me.csdn.net/u010278882)阅读数：4579
# Java 依赖注入标准 JSR-330 简介
## 概念
`JSR-330` 是 `Java` 的依赖注入标准。定义了如下的术语描述依赖注入：
- A 类型依赖 B类型（或者说 B 被 A 依赖），则 A类型 称为”**依赖(物) dependency**”
- 运行时查找依赖的过程，称为”**解析 resolving**“依赖
- 如果找不到依赖的实例，称该依赖是”**不能满足的 unsatisfied**”
- 在”依赖注入 dependency injection”机制中，提供依赖的工具称为”依赖注入器 dependency injector”
> 
在标准中, 依赖是类型而不是实例/对象; 在程序中（运行时）, 需要的是依赖的实例.
标准对依赖注入的使用进行了定义, 但是对实现和配置未定义。`Java EE`包`javax.inject`对应该标准。其中也仅定义了依赖注入的使用（即通过注解），同样也未定义依赖注入的配置方式和实现方式。实际上，依赖配置方式和实现方式由”**依赖注入器**“定义和实现。注入器的配置方式常见有`XML`、`注解`、`DSL`、`Java 代码`; 实现方式有反射和代码生成。依赖注入器在构建时，会验证所有依赖是否满足（通过识别注解来得知其依赖），在运行时为类注入其需要的依赖的实例。（因为依赖注入器对依赖的识别过程就是对注解的处理，所以依赖注入器也是[注解处理器](http://blog.csdn.net/u010278882/article/details/50766809)）。
## `javax.injects` 包
该包提供了如下5个注解（Inject、Qualifier、Named、Scope、Singleton）和1个接口（Provider）。
### @Inject
标识某个类中，需要由注入器注入的类成员（被标识的成员称为”可注入的”）。
使用规则
- 可用于注解构造器、字段、方法这些类成员（对成员是静态与否、最好是public的）
- 每个类只能有一个构造器可以被标记可注入；空构造器可以不用@Inject注解。
- 可注入的字段不能为 final 的
- 可注入的方法不能为 abstract 的
注入器的依赖注入顺序
- 构造器 > 字段 > 方法
- 父类 > 子类
- 一个类的两个可注入字段或其他成员无注入顺序
另外的四个注解对依赖注入进一步进行配置。
### @Qualifier 和 @Named
其中，@Qualifiery用于创建限定器。限定器是一个自定义的注解，可注解字段或方法的参数，用于限制可注入的依赖的类型。限定器注解必须被 @Qualifier 和 @Retention(RetentionPolicy.RUNTIME) 注解。
```java
// 定义限定器
@Qualifier
@Retention(RetentionPolicy.RUNTIME)
public @interface Big {
}
// 使用限定器
@Inject
public void foo(@ Big Bar bar) {
    // some code
}
```
@Named 就是一个通过 @Qualifier 定义的限定器。
### @Scope 和 @Singleton
其中，@Scope 用于创建作用域。作用域是一个自定义的注解，可注解构造器，用于要求注入器对注入的实例的创建方式。比如，是每次构造器被调用就创建一个依赖的实例，还是就创建一个依赖的实例然后重用。作用域注解必须被 @Scope 和 @Retention(RetentionPolicy.RUNTIME) 注解。
@ Singleton 就是一个通过 @Scope 定义的作用域。
### `Provider<T>`
Provider 作为另一种提供依赖的定义（有一种是 @Inject 注解），其实例提供 T 类型的实例。与 @Inject 注解相比，其还能：
- 返回的实例可以是多个
- 返回的实例可以是延迟返回的
- 返回的实例来自指定作用域内（不明觉厉）
## 参考
- [Java 依赖注入标准（JSR-330）简介](http://blog.csdn.net/DL88250/article/details/4838803)
- [Inject Javadoc](http://tool.oschina.net/uploads/apidocs/javaEE6/javax/inject/package-summary.html)
