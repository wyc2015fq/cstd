# 编程式事务管理—PlatformTransactionManager - z69183787的专栏 - CSDN博客
2013年01月04日 10:30:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3704
JDBC的事务管理在Spring2.0下有两种编程式的实现
PlatformTransactionManager
TransactionTemplate
对于PlatformTransctionManager，通常，不直接使用它，而是使用它的实现类DataSourceTransactionManager。具体用法如下：
1. 实例化的时候，需要用个数据源作参数
2. 建立事务的定义，使用DefaultTransactionDefinition类，直接new个新的就可以了
3. 调用DefaultTransactionDefinition类的setPropagationBehavior方法，参数是TransactionDefinition的常量，例如PROPAGATION_REQUIRED等。
4. 在使用到具体的SQL操作之前，创建一个TransactionStatus类的实例： TransactionStatus status = transactionManager.getTransaction(def)。
其中，transactionManager是DataSourceTransactionManager类的实例，def是DefaultTransactionDefinition的实例
5. 之后使用具体的SQL操作，发生异常了，就使用transactionManager.rollback(status)
6. 正常操作的话，就使用transactionManager.commit(status)
例子：
JdbcTemplate template = new JdbcTemplate(datasource);
DataSourceTransactionManager tran = new DataSourceTransactionManager(datasource);
DefaultTransactionDefinition def = new DefaultTransactionDefinition();//事务定义类
def.setPropagationBehavior(TransactionDefinition.PROPAGATION_REQUIRED);
TransactionStatus status = tran.getTransaction(def);//返回事务对象
try {
template.update("Insert into userinfo(username,password)
 values('aaaaa','bbbbb')");
template.update("Insert into userinfo(username,password)
 values('cccc','ddd')");
tran.commit(status);
} catch (Exception ex) {
tran.rollback(status);
}
=========================================================
编程式事务管理——TransactionTemplate
对这个类的使用比上个更简单
1. 在具体的SQL语句操作签，实例化它： TransactioinTemplate transactionTemplate = new TransactionTemplate(trasactionManager);
2. 然后就是具体的SQL操作，不过写起来有点复杂：
transactionTemplate.execute(
new TransactionCallback() {
public Object doInTransaction(TransactionStatus status) {
// 这里执行SQL的操作
// 发生异常的时候status.setRollbackOnly()，正常的时候直接返回结果
}
}
);
3. 如果没有返回值，可以使用TransactionCallbackWithoutResult类。里面的方法调用一样，只不过用了TransactionCallbackWithoutResult的匿名继承。
例子：
final JdbcTemplate template = new JdbcTemplate(datasource);
DataSourceTransactionManager tran = new DataSourceTransactionManager( datasource);
TransactionTemplate trantemplate = new TransactionTemplate(tran);
trantemplate.execute(new TransactionCallback() {
public Object doInTransaction(TransactionStatus status) {
int i = 0;
try {
template.update("Insert into userinfo(username,password) values('jjj','kkk')");
template.update("Insert into userinfo(username,password) values('llll','mmm')");
i = 1;
} catch (Exception ex) {
ex.printStackTrace();
status.setRollbackOnly();
i = 0;
}
return new Integer(i);
}
