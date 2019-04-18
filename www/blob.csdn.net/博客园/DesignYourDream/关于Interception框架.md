# 关于Interception框架 - DesignYourDream - 博客园
# [关于Interception框架](https://www.cnblogs.com/designyourdream/p/4527533.html)
对于OOP来说，是个树形结构，如果要实现多个子数之间的共享服务，例如很多子树都用到日志服务，这时候AOP的横切关注 cross cutting concerns就非常有用了。如果要使每个类具备一项功能，势必要修改每个需要的类结构，这样显然麻烦，而且不容易维护。AOP避免了这一点，将横切关注封装到了一个独立的单元，这样既使每个类都具备这项功能，又不破坏类的结构。Aspect，也就是一个方面，是一组可重用的代码片段，可以用来注入到已经存在的类里，而不改变类的代码。一般来说，aspect的代码会被框架处理一次，再进行代码注入code injection。注意，这里代码注入和依赖注入dependency injection是不一样的，
所谓代码注入，例如一个类需要日志服务，会在这个类需要调用日志的地方插入aspect的公共调用代码。有的AOP框架，例如PostSharp，会定义attribute，然后给需要的类的某个方法加上这个attribute，这样postsharp会把attribute里的部分代码插入到这个类的方法里。
实现AOP需要以下几个方面：
1.JoinPoints，连接点。需要某个aspect的target class的代码点，可以是方法等。可以在joinPoints的前，后，或者周围插入。
2.Pointcuts，切点。对JoinPoints的一次封装。
3.Advice or Introductions。是要插入到target class的aspect 代码段
4.Weaver。 编译器，用来执行代码注入的工具。
Unity Container应用了一个aop的feature，即interception mechanism，应用这个可以让代码有那么一点aop的意思。unity里的拦截器机制将会在指定方法的某个执行阶段，进行拦截，然后进行配置和重写目标行为。简单来说，拦截器机制就是给对象增加了行为，普通给对象加行为的方法是派生接口，但拦截机器不用修改对象的代码。Unity的interception是对decorator模式的一种实现，这种模式用来在运行时扩展对象的功能。
Unity的拦截器非常类似于一个pipeline，经过重重行为的附加filter，最终到达target class。从概念上说，分成三类，interceptor（proxy），behaviour pipeline，the behaviour(aspect)。interceptor是 拦截机制对外暴露的接口，当需要使用拦截机制时，需要使用interceptor，它检查所有已经注册的行为behaviour，把传给它的数据传给behaviour，并通过behaviour pipeline来执行这些行为。 Unity提供了两种interceptor，分别是instance interceptor 和 type interceptor，对应处理对象和类型的拦截。

