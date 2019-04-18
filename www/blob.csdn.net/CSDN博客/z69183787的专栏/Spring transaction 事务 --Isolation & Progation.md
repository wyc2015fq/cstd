# Spring transaction 事务 --Isolation & Progation - z69183787的专栏 - CSDN博客
2014年04月02日 19:56:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6931
**注意的几点:**
1  @Transactional 只能被应用到public方法上, 对于其它非public的方法,如果标记了@Transactional也不会报错,但方法没有事务功能.
2 默认情况下,一个有事务方法, 遇到**RuntiomeException** 时**会回滚** .  遇到 **受检查的异常** 是**不会回滚** 的. 要想所有异常都回滚,要加上 @Transactional( rollbackFor={Exception.class,其它异常}) .
**@Transactional  的所有可选属性如下:**
|属性|类型|默认值|说明|
|----|----|----|----|
|propagation|Propagation枚举|REQUIRED|事务传播属性 (下有说明)|
|isolation|isolation枚举|DEFAULT|事务隔离级别 (另有说明)|
|readOnly|boolean|false|是否只读|
|timeout|int|-1|超时(秒)|
|rollbackFor|Class[]|{}|需要回滚的异常类|
|rollbackForClassName|String[]|{}|需要回滚的异常类名|
|noRollbackFor|Class[]|{}|不需要回滚的异常类|
|noRollbackForClassName|String[]|{}|不需要回滚的异常类名|
 事务的隔离级别 有如下可选:
可以去看spring源码 : org.springframework.transaction.annotation.Isolation
(用时，导入org.springframework.transaction.annotation.Isolation，再在Transactional括号里用如isolation = Isolation.DEFAULT)
我的["隔离级别"相关文章](http://blog.csdn.net/hz_chenwenbiaotmb/article/details/admin/blogs/328050)   有不明白的,可以去看看.
|DEFAULT|采用数据库默认隔离级别|
|----|----|
|READ_UNCOMMITTED|请看["隔离级别"相关文章](http://blog.csdn.net/hz_chenwenbiaoTMB/archive/2010/08/06/5793561.aspx)|
|READ_COMMITTED|请看["隔离级别"相关文章](http://blog.csdn.net/hz_chenwenbiaoTMB/archive/2010/08/06/5793561.aspx)|
|REPEATABLE_READ|请看["隔离级别"相关文章](http://blog.csdn.net/hz_chenwenbiaoTMB/archive/2010/08/06/5793561.aspx)|
|SERIALIZABLE|请看["隔离级别"相关文章](http://blog.csdn.net/hz_chenwenbiaoTMB/archive/2010/08/06/5793561.aspx)[](http://blog.csdn.net/hz_chenwenbiaoTMB/archive/2010/08/06/5793561.aspx)|
数据库提供了四种事务隔离级别, 不同的隔离级别采用不同的锁类开来实现.
在四种隔离级别中, Serializable的级别最高, Read Uncommited级别最低.
大多数数据库的默认隔离级别为: Read Commited,如Sql Server , Oracle.
少数数据库默认的隔离级别为Repeatable Read, 如MySQL InnoDB存储引擎
即使是最低的级别,也不会出现 第一类丢失更新问题 .
- Read Uncommited :读未提交数据( 会出现脏读,不可重复读,幻读 ,避免了第一类丢失更新)
- Read Commited :读已提交的数据(会出现不可重复读,幻读)
- Repeatable Read :可重复读(会出现幻读)
- Serializable :串行化
**丢失 更新   : **
当两个或多个事务选择同一行，然后基于最初选定的值更新该行时，会发生丢失更新问题。每个事务都不知道其它事务的存在。最后的更新将重写由其它事务所做的更新，这将导致数据丢失。 　　
例:
事务A和事务B同时修改某行的值，
1.事务A将数值改为1并提交
2.事务B将数值改为2并提交。
这时数据的值为2，事务A所做的更新将会丢失。 
解决办法：对行加锁，只允许并发一个更新事务。
**脏读:** 一个事务读到另一个事务未提交的更新数据
例:
1.Mary的原工资为1000, 财务人员将Mary的工资改为了8000(但未提交事务)         
2.Mary读取自己的工资 ,发现自己的工资变为了8000，欢天喜地！
3.而财务发现操作有误，回滚了事务,Mary的工资又变为了1000, 像这样,Mary记取的工资数8000是一个脏数据。
**不可重复读:** 在同一个事务中,多次读取同一数据,返回的结果有所不同. 换句话说就是,后续读取可以读到另一个事务已提交的更新数据. 相反"可重复读"在同一事务多次读取数据时,能够保证所读数据一样,也就是后续读取不能读到另一事务已提交的更新数据.
例:
1.在事务1中，Mary 读取了自己的工资为1000,操作并没有完成
2.在事务2中，这时财务人员修改了Mary的工资为2000,并提交了事务.
3.在事务1中，Mary 再次读取自己的工资时，工资变为了2000
解决办法：如果只有在修改事务完全提交之后才可以读取数据，则可以避免该问题。
**幻读:** 一个事务读取到另一个事务已提交的insert数据.
例:
第一个事务对一个表中的数据进行了修改，这种修改涉及到表中的全部数据行。同时(此时第一事务还未提交)，第二个事务向表中插入一行新数据。这时第一个事务再去读取表时,发现表中还有没有修改的数据行，就好象发生了幻觉一样。
事务的传播属性,有如下可选
可以去看spring源码 : org.springframework.transaction.annotation.Propagation
(用时，导入org.springframework.transaction.annotation.Propagation，再在Transactional括号里用如propagation = Propagation.REQUIRED)
|REQUIRED|业务方法需要在一个事务中运行,如果方法运行时,已处在一个事务中,那么就加入该事务,否则自己创建一个新的事务.这是spring默认的传播行为.|
|----|----|
|SUPPORTS|如果业务方法在某个事务范围内被调用,则方法成为该事务的一部分,如果业务方法在事务范围外被调用,则方法在没有事务的环境下执行.|
|MANDATORY|只能在一个已存在事务中执行,业务方法不能发起自己的事务,如果业务方法在没有事务的环境下调用,就抛异常|
|REQUIRES_NEW|业务方法总是会为自己发起一个新的事务,如果方法已运行在一个事务中,则原有事务被挂起,新的事务被创建,直到方法结束,新事务才结束,原先的事务才会恢复执行.|
|NOT_SUPPORTED|声明方法需要事务,如果方法没有关联到一个事务,容器不会为它开启事务.如果方法在一个事务中被调用,该事务会被挂起,在方法调用结束后,原先的事务便会恢复执行.|
|NEVER|声明方法绝对不能在事务范围内执行,如果方法在某个事务范围内执行,容器就抛异常.只有没关联到事务,才正常执行.|
|NESTED|如果一个活动的事务存在,则运行在一个嵌套的事务中.如果没有活动的事务,则按REQUIRED属性执行.它使用了一个单独的事务, 这个事务拥有多个可以回滚的保证点.内部事务回滚不会对外部事务造成影响, 它只对DataSourceTransactionManager 事务管理器起效.|


**[java]**[view
 plain](http://blog.csdn.net/hz_chenwenbiaotmb/article/details/5793538#)[copy](http://blog.csdn.net/hz_chenwenbiaotmb/article/details/5793538#)
- //事务传播属性
- @Transactional(propagation=Propagation.REQUIRED) //如果有事务,那么加入事务,没有的话新建一个(不写的情况下)
- @Transactional(propagation=Propagation.NOT_SUPPORTED) //容器不为这个方法开启事务
- @Transactional(propagation=Propagation.REQUIRES_NEW) //不管是否存在事务,都创建一个新的事务,原来的挂起,新的执行完毕,继续执行老的事务
- @Transactional(propagation=Propagation.MANDATORY) //必须在一个已有的事务中执行,否则抛出异常
- @Transactional(propagation=Propagation.NEVER) //必须在一个没有的事务中执行,否则抛出异常(与Propagation.MANDATORY相反)
- @Transactional(propagation=Propagation.SUPPORTS) //如果其他bean调用这个方法,在其他bean中声明事务,那就用事务.如果其他bean没有声明事务,那就不用事务.
- 
- 
- @Transactional(propagation=Propagation.NESTED)   
- @Transactional (propagation = Propagation.REQUIRED,readOnly=true) //readOnly=true只读,不能更新,删除 
- @Transactional (propagation = Propagation.REQUIRED,timeout=30)//设置超时时间 
- @Transactional (propagation = Propagation.REQUIRED,isolation=Isolation.DEFAULT)//设置数据库隔离级别
用 spring 事务管理器,由spring来负责数据库的打开,提交,回滚.
默认遇到运行期例外(throw new RuntimeException("注释");)会回滚，即遇到不受检查（unchecked）的例外时回滚；
而遇到需要捕获的例外(throw new Exception("注释");)不会回滚,即遇到受检查的例外（就是非运行时抛出的异常，编译器会检查到的异常叫受检查例外或说受检查异常）时，需我们指定方式来让事务回滚 ,如下:
@Transactional(rollbackFor=Exception.class) //指定回滚,遇到异常Exception时回滚
    public void methodName() {
       throw new Exception("注释");
    } 
@Transactional(noRollbackFor=Exception.class)//指定不回滚,遇到运行期例外(throw new RuntimeException("注释");)会回滚
    public ItimDaoImpl getItemDaoImpl() {
        throw new RuntimeException("注释");
    } 
