# 关于springMVC和struts2 自己的理解 （@scope（""）的理解，以及prototype和singleton 的使用场景） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月06日 13:46:14[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：144


区别一：

springMVC 只有第一次访问controller的时候才会调用构造方法，也就是说只会创建一次controller对象。

struts2 每一次进入就会调用一次构造方法，也就是每访问一次action 就会创建一个对象。

原因： struts2 默认 是 原型模型 即 @scope("prototype") ，这样设计的原因是struts2 是通过成员变量来接收前端传过来的值得，如果是单例模式的话，成员变量的值 还会是上一次请求传过来的参数的值。springMVC没有这样设计是因为它是通过方法的参数来接收前端的请求数据。作用域仅仅是方法体内，方法执行完毕以后pojo对象就会被回收。从这点来看，springMVC的性能就要优于struts2。

区别二：

对于 controller和action 中的 pojo，是通过每次前端发送过来的请求来访问指定的方法，如果该方法中用到了pojo这个全局变量了，那么就会给我们新new 一个pojo的实例，然后来接收前端传过来的参数。

区别三：

struts2 必须给接收前端参数的pojo全局变量 写对应的 get、set方法，pojo中也要给每一个全局变量写get、set方法，而springMVC中只需要在 pojo中写get、set方法，而不需要在controller中写get、set方法。


说一下 @scope 常用的 两个 值 singleton 和 prototype  。

@scope（"prototype"） 使用场景  ： spring 创建的 bean 是有状态的，则需要设置 prototype

什么叫有状态的？ 

就是 这个类 的成员变量 的值 会被修改，或 成员变量中 包含 一个 实例变量，这个实例变量的中的 成员变量 可以被修改。

无状态 反之。

有状态的 bean 会引发的问题： 就是前面提到 struts2 的 action 使用 @scope("prototype")的原因。

@scope（"singleton "） 使用场景  ： 如果你不写 scope这个注解，默认就是singleton 。  

一般我们的 dao层和service 默认都是 singleton，因为 这两层 一般都没有 成员变量，仅仅是一些 方法。不涉及高并发带来的安全隐患问题。

总结 ： 有状态的bean 用 @scope（"prototype"），无状态的bean用@scope（"singleton "）。


