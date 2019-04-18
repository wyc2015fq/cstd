# SPRING 事务控制 （PROPAGATION_NESTED） - z69183787的专栏 - CSDN博客
2017年07月27日 16:56:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4674
```java
在 SPRING 中一共定义了六种事务传播属性
PROPAGATION_REQUIRED -- 支持当前事务，如果当前没有事务，就新建一个事务。这是最常见的选择。 
PROPAGATION_SUPPORTS -- 支持当前事务，如果当前没有事务，就以非事务方式执行。 
PROPAGATION_MANDATORY -- 支持当前事务，如果当前没有事务，就抛出异常。 
PROPAGATION_REQUIRES_NEW -- 新建事务，如果当前存在事务，把当前事务挂起。 
PROPAGATION_NOT_SUPPORTED -- 以非事务方式执行操作，如果当前存在事务，就把当前事务挂起。 
PROPAGATION_NEVER -- 以非事务方式执行，如果当前存在事务，则抛出异常。 
PROPAGATION_NESTED -- 如果当前存在事务，则在嵌套事务内执行。如果当前没有事务，则进行与PROPAGATION_REQUIRED类似的操作。 
最容易弄混淆的其实是PROPAGATION_REQUIRES_NEW 和 PROPAGATION_NESTED 
PROPAGATION_REQUIRES_NEW 启动一个新的, 不依赖于环境的 "内部" 事务. 这个事务将被完全 
commited 或 rolled back 而不依赖于外部事务,它拥有自己的隔离范围, 自己的锁, 等等. 
当内部事务开始执行时, 外部事务将被挂起, 内务事务结束时, 外部事务将继续执行. 
PROPAGATION_REQUIRES_NEW常用于日志记录,或者交易失败仍需要留痕
另一方面, PROPAGATION_NESTED 开始一个 "嵌套的" 事务,  它是已经存在事务的一个真正
的子事务. 潜套事务开始执行时,  它将取得一个 savepoint.
如果这个嵌套事务失败, 我们将回滚到此 savepoint. 潜套事务是外部事务的一部分,
 只有外部事务结束后它才会被提交. 
由此可见, PROPAGATION_REQUIRES_NEW 和 PROPAGATION_NESTED 的最大区别在于, 
PROPAGATION_REQUIRES_NEW 完全是一个新的事务, 而 PROPAGATION_NESTED 
则是外部事务的子事务, 如果外部事务 commit, 潜套事务也会被 commit,
 这个规则同样适用于 roll back. 
几个例子理解REQUIRED、REQUIRES_NEW、NESTED 的使用注意事项(TRY...CATCH配合使用)
1、REQUIRED的使用注意项
1.1 REQUIRED保证其处理过程同一个事务,如果调用的同一个类的配置的REQUIRED的方法,且此方法存在TRY CATCH
代码块, 如果此代码块出现异常,程序可以继续执行。
1.2 但如果调用的其他类的配置REQUIRED方法,且TRY CATCH住,则全部的提交全部回滚,且报出异常：
Transaction rolled back because it has been marked as rollback-only
因为事务报出异常后要全部回滚,包括父类的调用。
1.3 如果service中包含多个dao的方法,其都属于同一个事务,其中报错全部回滚,try catch住不影响程序代码的继续执行.
class A{
   //PROPAGATION_REQUIRED
	 void methodA() {
        try{
          methodB(); //可以继续执行,因为是同一个类
        }catch(Exception ex){
          ex.printStrace();
        }
        
        try{
          methodC(); //报错Transaction rolled back because it has been marked as rollback-only
                     //因为回滚整个事务,不能用try catch住.当然通过不会try catch一个事务的部分代码
        }catch(Exception ex){
          ex.printStrace();
        }
   }
   
   //PROPAGATION_REQUIRED
   void methodB() {
   
   }
}
class B{
   
   //PROPAGATION_REQUIRED
   void methodC() {
   
   }
}
2、NESTED的具体用途如下：
在此方法出现异常时,通过TRY CATCH 代码块包含住, 继续往下执行或者执行CATCH中的处理.
此点REUQIRED做不到, REQUIRED_NEW能做到, 但它是单独的事务，不与父类一块提交的。
ServiceA { 
/**
 
* 事务属性配置为 PROPAGATION_REQUIRED
 
*/
 
void methodA() {
 
try {
 
//savepoint
 
ServiceB.methodB(); //PROPAGATION_NESTED 级别
 
} catch (SomeException) {
 
// 执行其他业务, 如 ServiceC.methodC();
}}}
```
