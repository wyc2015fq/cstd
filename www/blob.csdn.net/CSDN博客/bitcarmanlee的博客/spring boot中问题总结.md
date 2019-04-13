
# spring boot中问题总结 - bitcarmanlee的博客 - CSDN博客


2018年06月01日 18:21:10[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：191标签：[spring boot																](https://so.csdn.net/so/search/s.do?q=spring boot&t=blog)[异常																](https://so.csdn.net/so/search/s.do?q=异常&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=spring boot&t=blog)个人分类：[spring serial																](https://blog.csdn.net/bitcarmanlee/article/category/7706968)



## 1.nested exception is org.hibernate.AnnotationException: No identifier specified for entity:com.xxx.xxx.xxx
当spring报这个错误的时候，一般原因是xxx这个实体类没有指定数据库表中对应的主键。在使用hibernate的映射表的时候entity类是必须要主键的，否则就会报出这个异常。在对应主键字段上加上@Id注解即可。
## 2.javax.validation.ConstraintViolationException: Validation failed for classes [com.xxx.xxx.xxx] during persist time for groups [javax.validation.groups.Default, ] List of constraint violations:[ConstraintViolationImpl{interpolatedMessage=’不能为null’, propertyPath=applyTime, rootBeanClass=class com.xxx.xxx.xxx, messageTemplate=’{javax.validation.constraints.NotNull.message}’}
这个错误相对还是比较明显，就是xxx对象中某个属性要求非空，结果实例化的时候却没有给该属性赋值。

