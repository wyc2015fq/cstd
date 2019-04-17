# Java —— 注解（Annotation） - Alex_McAvoy的博客 - CSDN博客





2018年10月23日 14:28:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：39
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

注解，就是代码中的特殊标记，这些标记可在编译、类加载、运行时被读取，并进行相应处理，通过注解，可以在不改变原有逻辑的情况下，在源文件中嵌入一些补充信息。

注解可以像修饰符一样被使用，可用于修饰包、类、构造器、方法、成员变量、参数、局部变量的声明，这些信息被保存在注解的 name=value 对中。

注解还可以被用来为程序元素（类、方法、成员变量等）设置元数据。

# 【基本的注解】

使用注解要在其前加 @ 符号，并把该注解当成一个修饰符来使用，用于修饰它支持的程序元素。

三个基本的注解：
- @Override：显示的声明其下的方法是重写父类方法（该注释只可用于方法）
- @Deprecated：表示某个程序元素已过时，即在程序元素名上打一横线，但不影响使用，其作用是提醒程序员当前程序元素以后会更新，现在写程序时尽量避免使用
- @SuppressWarning：抑制编译器警告

![](https://img-blog.csdnimg.cn/20181027232052434.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_27,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181027232155867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_27,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181027232345343.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_27,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181027232412452.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_27,color_FFFFFF,t_70)

# 【自定义注解】

需要自定义注解时，将 class 换为 @interface 即可

```java
@myAnnotation(value="xxxxx")
public static void deal(String str) {
}
public @interface myAnnotation{
	String value() default "Hello";
}
```

# 【元注解】

元注解是用于修饰其他注解的，在实际应用中，使用较少。

JDK 中提供了以下 4 个常用的元注解：
- Retention：用于修饰一个注解可以保留多少时间，使用时要指定 value 值
- Target：用于指定被修饰注解能用于修饰哪些程序元素，使用时要指定 value 值
- Documented：用于指定被其修饰的注解类将被 javadoc 工具提取称文档
- Inherited：被其修饰的注解将具有继承性，即注解的子类拥有注解



