# Spring 事务管理@Transactional - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月03日 13:22:46[boonya](https://me.csdn.net/boonya)阅读数：749标签：[spring																[事务](https://so.csdn.net/so/search/s.do?q=事务&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)








一个业务操作，要保证它操作的原子性就必须将所有内部子操作都加入事务容器管理，这样一旦出现异常就能实现整个事务的回滚，从而确保业务操作的原子性保护，避免因事务不会滚而造成的垃圾数据。

**@Transactional的属性**


|属性名|类型|说明|
|----|----|----|
|isolation|枚举org.springframework.transaction.annotation.Isolation的值|事务隔离级别|
|noRollbackFor|Class<? extends Throwable>[]|一组异常类，遇到时不回滚。默认为{}|
|noRollbackForClassName|Stirng[]|一组异常类名，遇到时不回滚，默认为{}|
|propagation|枚举org.springframework.transaction.annotation.Propagation的值|事务传播行为|
|readOnly|boolean|事务读写性|
|rollbackFor|Class<? extends Throwable>[]|一组异常类，遇到时回滚|
|rollbackForClassName|Stirng[]|一组异常类名，遇到时回滚|
|timeout|int|超时时间，以秒为单位|
|value|String|可选的限定描述符，指定使用的事务管理器|




**@Transactional标注的位置**
@Transactional注解可以标注在类和方法上，也可以标注在定义的接口和接口方法上。


**如果我们在接口上标注@Transactional注解，会留下这样的隐患：因为注解不能被继承，所以业务接口中标注的@Transactional注解不会被业务实现类继承。所以可能会出现不启动事务的情况。所以，Spring建议我们将@Transaction注解在实现类上。在方法上的@Transactional注解会覆盖掉类上的@Transactional。**
一般地，在service类前加上@Transactional，声明这个service所有方法需要事务管理。每一个业务方法开始时都会打开一个事务。


Spring默认情况下会对运行期例外(RunTimeException)进行事务回滚。这个例外是unchecked

如果遇到checked意外就不回滚。

如何改变默认规则：

1 让checked例外也回滚：在整个方法前加上 @Transactional(rollbackFor=Exception.class)

2 让unchecked例外不回滚： @Transactional(notRollbackFor=RunTimeException.class)

3 不需要事务管理的(只查询的)方法：@Transactional(propagation=Propagation.NOT_SUPPORTED)



注意： 如果异常被try｛｝catch｛｝了，事务就不回滚了，如果想让事务回滚必须再往外抛try｛｝catch｛throw Exception｝。





@Transactional设置：


propagation：事务传播性设置，Propagation枚举类型。Spring支持的事务传播属性包括7种：

> 
    PROPAGATION_MANDATORY：方法必须在事务中执行，否则抛出异常。

    PROPAGATION_NESTED：使方法运行在嵌套事务中，否则和PROPAGATION_REQUIRED一样。

    PROPAGATION_NEVER ：当前方法永远不在事务中运行，否则抛出异常。

    PROPAGATION_NOT_SUPPORTED：定义为当前事务不支持的方法，在该方法执行期间正在运行的事务会被暂停

    PROPAGATION_REQUIRED：当前的方法必须运行在事务中，如果没有事务就新建一个事务。新事务和方法一起开始，随着方法返回或者抛出异常时终止。

    PROPAGATION_REQUIRED_NEW ：当前方法必须新建一个事务，如果当前的事务正在运行则暂停。

    PROPAGATION_SUPPORTS ：规定当前方法支持当前事务，但是如果没有事务在运行就使用非事务方法执行。



isolation：事务隔离性级别设置，Isolation枚举类型

> 
    ISOLATION_DEFAULT ：使用数据库默认的隔离级别

    ISOLATION_COMMITTED：允许其他事务已经提交的更新（防止脏读取）

    ISOLATION_READ_UNCOMMITTED：允许读取其他事务未提交的更新，会导致三个缺陷发生。执行速度最快

    ISOLATION_REPEATABLE_READ ：除非事务自身更改了数据，否则事务多次读取的数据相同（防止脏数据，多次重复读取）

    ISOLATION_SERIALIZABLE：隔离级别最高，可以防止三个缺陷，但是速度最慢，影响性能。





readOnly：读写性事务，只读性事务，布尔型

> 
    对数据库的操作中，查询是使用最频繁的操作，每次执行查询时都要从数据库中重新读取数据，有时多次读取的数据都是相同的，这样的数据操作不仅浪费了系统资源，还影响了系统速度。对访问量大的程序来说，节省这部分资源可以大大提    升系统速度。

   将事务声明为只读的，那么数据库可以根据事务的特性优化事务的读取操作



timeout：超时时间，单位秒

> 
事务可能因为某种原因很长时间没有反应，这期间可能锁定了数据库表，影响性能。设置超时时间，如果超过该时间，事务自动回滚。



rollbackFor：一组异常类的实例，遇到时必须进行回滚


rollbackForClassname：一组异常类的名字，遇到时必须进行回滚


noRollbackFor：一组异常类的实例，遇到时必须不回滚


noRollbackForClassname：一组异常类的名字，遇到时必须不回滚



@Transactional






**spring 事务注解**



```java
// 指定回滚
@Transactional(rollbackFor=Exception.class) 
    public void methodName() {
       // 不会回滚
       throw new Exception("...");
    } 
//指定不回滚
@Transactional(noRollbackFor=Exception.class)
    public ItimDaoImpl getItemDaoImpl() {
        // 会回滚
        throw new RuntimeException("注释");
    } 

    // 如果有事务,那么加入事务,没有的话新建一个(不写的情况下)
    @Transactional(propagation=Propagation.REQUIRED) 
    // 容器不为这个方法开启事务
    @Transactional(propagation=Propagation.NOT_SUPPORTED)
    // 不管是否存在事务,都创建一个新的事务,原来的挂起,新的执行完毕,继续执行老的事务
    @Transactional(propagation=Propagation.REQUIRES_NEW) 
    // 必须在一个已有的事务中执行,否则抛出异常
    @Transactional(propagation=Propagation.MANDATORY)
    // 必须在一个没有的事务中执行,否则抛出异常(与Propagation.MANDATORY相反)
    @Transactional(propagation=Propagation.NEVER) 
    // 如果其他bean调用这个方法,在其他bean中声明事务,那就用事务.如果其他bean没有声明事务,那就不用事务.
    @Transactional(propagation=Propagation.SUPPORTS) 
    
    /*
    public void methodName(){
       // 本类的修改方法 1
       update();
       // 调用其他类的修改方法
       otherBean.update();
       // 本类的修改方法 2
       update();
    }
    other失败了不会影响 本类的修改提交成功
    本类update的失败,other也失败
    */
@Transactional(propagation=Propagation.NESTED) 
// readOnly=true只读,不能更新,删除 
@Transactional (propagation = Propagation.REQUIRED,readOnly=true) 
// 设置超时时间
@Transactional (propagation = Propagation.REQUIRED,timeout=30)
// 设置数据库隔离级别
@Transactional (propagation = Propagation.REQUIRED,isolation=Isolation.DEFAULT)
```

**参考资料：**


http://blog.chinaunix.net/uid-20586655-id-3370000.html

http://blog.csdn.net/seng3018/article/details/6690527










